// MTdmfItemShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MTdmfItemShowDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"

#include "..\wg_db\CMTdmtCalcCrpShk.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMTdmfItemShowDlg dialog


CMTdmfItemShowDlg::CMTdmfItemShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CMTdmfItemShowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMTdmfItemShowDlg)
	m_nDataType = 0;
	m_nOptionForIndia = 0;
	m_dLoadingAge = 10.0;
	m_dEndLoadingAge = 10000.0;
	m_nSteps = 24;
	m_bIndia = FALSE;
	m_bAustralia = FALSE;
	m_bRussia = FALSE;
	m_bNewzealand = FALSE;
	//}}AFX_DATA_INIT
}


void CMTdmfItemShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMTdmfItemShowDlg)
	DDX_Radio(pDX, IDC_CMD_TDMF_CREEP_COEFFICIENT, m_nDataType);
	DDX_Radio(pDX, IDC_CMD_TDMF_CREEP_INDIA_RDO, m_nOptionForIndia);
	DDX_Text(pDX, IDC_CMD_TDMF_LOADINGAGE, m_dLoadingAge);
	DDX_Control(pDX, IDC_CMD_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_CMD_CHART_XLog, m_btnXLog);
	DDX_Text(pDX, IDC_CMD_TDMF_LOADINGAGE2, m_dEndLoadingAge);
	DDX_Text(pDX, IDC_CMD_TDMF_STEPS, m_nSteps);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_CMD_TDMF_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMTdmfItemShowDlg, CDialogMove)
	//{{AFX_MSG_MAP(CMTdmfItemShowDlg)
	ON_BN_CLICKED(IDC_CMD_CHART_XLog, OnETCSPFCCHARTXLog)
	ON_BN_CLICKED(IDC_CMD_CHART_YLog, OnETCSPFCCHARTYLog)
	ON_BN_CLICKED(IDC_CMD_TDMF_SHOW_REDRAW, OnRedraw)
	ON_BN_CLICKED(IDC_CMD_TDMF_CREEP_COEFFICIENT, OnCmdTdmfDataType)
	ON_BN_CLICKED(IDC_CMD_TDMF_SHRINK_STRAIN, OnCmdTdmfDataType)
	ON_BN_CLICKED(IDC_CMD_TDMF_CREEP_INDIA_RDO, OnCmdTdmfOptionForIndia)
	ON_BN_CLICKED(IDC_CMD_TDMF_CREEP_INDIA_RDO2, OnCmdTdmfOptionForIndia)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMTdmfItemShowDlg message handlers


void CMTdmfItemShowDlg::InitGrid()
{
	m_wndGrid.SetReadOnlyMode(TRUE);
	m_wndGrid.Initialize(&m_ParamData);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_bGridInit = TRUE;
}


BOOL CMTdmfItemShowDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CDBDoc* pDoc  = CDBDoc::GetDocPoint();
								
	m_bIndia      = (m_tdmt_d.COMMON.nCode == TDMT_CODE_INDIA_IRC_18 /*|| m_tdmt_d.COMMON.nCode == 25*/);
	m_bAustralia  = (m_tdmt_d.COMMON.nCode == TDMT_CODE_AUSTRALIA_2009 || m_tdmt_d.COMMON.nCode == TDMT_CODE_AUSTRALIA_2011 ||
		             m_tdmt_d.COMMON.nCode == TDMT_CODE_AUSTRALIA_2016 || m_tdmt_d.COMMON.nCode == TDMT_CODE_AUSTRALIA_2017 ||
					 m_tdmt_d.COMMON.nCode == TDMT_CODE_AUSTRALIA_2018_AMD_2021 || m_tdmt_d.COMMON.nCode == TDMT_CODE_AUSTRALIA_2017_AMD_2024);
	m_bRussia     = (m_tdmt_d.COMMON.nCode == TDMT_CODE_RUSSIAN);
	m_bNewzealand = (m_tdmt_d.COMMON.nCode == TDMT_CODE_NEWZEALAND || m_tdmt_d.COMMON.nCode == TDMT_CODE_NEWZEALAND_2022);

	m_ParamData.Initialize();
	m_ParamData.nMatlFuncType = m_nDataType+1;
	m_ParamData.nCreepType = 3;
	InitGrid();
	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;
	InitChart();
	
	CString str = _LS(IDS_WG_CMD__Creep_Coefficient); // _T("Creep Coefficient");  
	if     (m_tdmt_d.COMMON.nCode == TDMT_CODE_JPJSCE ) str = _LS(IDS_WG_CMD__Creep_Strain_Stress); // _T("Creep Strain / Stress");  
	else if(m_tdmt_d.COMMON.nCode == TDMT_CODE_RUSSIAN) str = _LS(IDS_WG_CMD__Creep_Strain);        // _T("Creep Strain");  
	GetDlgItem(IDC_CMD_TDMF_CREEP_COEFFICIENT)->SetWindowText(str);    

	if(m_bIndia)
	{
		str = _LS(IDS_CMD0417__Creep);
		GetDlgItem(IDC_CMD_TDMF_CREEP_COEFFICIENT)->SetWindowText(str);
		AlignCtrlForIndia();
	}
	
	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	UpdateData(FALSE);

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영


	if (m_tdmt_d.COMMON.nCode == TDMT_CODE_CHINA_CHJTG_T_D65_2015)
	{
		GetDlgItem(IDC_CMD_TDMF_SHRINK_STRAIN)->ShowWindow(SW_HIDE);
	}

	OnRedraw();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMTdmfItemShowDlg::AlignCtrlForIndia()
{
	CArray<UINT, UINT> aIDs;

	aIDs.RemoveAll(); aIDs.Add(IDC_CMD_TDMF_CREEP_COEFFICIENT);
	CDlgUtil::CtrlMoveDistXY(this, aIDs, globalUtils.ScaleByDPI(-15), globalUtils.ScaleByDPI(-5));

	aIDs.RemoveAll(); aIDs.Add(IDC_CMD_TDMF_SHRINK_STRAIN);
	CDlgUtil::CtrlMoveDistXY(this, aIDs, globalUtils.ScaleByDPI(-15), globalUtils.ScaleByDPI(7));

	CRect rFrom, rTo;
	GetDlgItem(IDC_CMD_TDMF_CREEP_INDIA_RDO)->GetWindowRect(rFrom);
	GetDlgItem(IDC_CMD_TDMF_CREEP_COEFFICIENT)->GetWindowRect(rTo);
	int nDistY = rTo.top - rFrom.top;
	aIDs.RemoveAll();
	aIDs.Add(IDC_CMD_TDMF_CREEP_INDIA_RDO);
	aIDs.Add(IDC_CMD_TDMF_CREEP_INDIA_RDO2);
	CDlgUtil::CtrlMoveDistXY(this, aIDs, 0, nDistY + globalUtils.ScaleByDPI(16));
}


BOOL CMTdmfItemShowDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
	  if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{
		  if(pMsg->wParam == VK_RETURN)
			if(pMsg->wParam == VK_ESCAPE)
			
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;        
			// DO NOT process further
		}
	}
	return CDialogMove::PreTranslateMessage(pMsg);
}

void CMTdmfItemShowDlg::OnBtnDyGen()
{
	UpdateData(TRUE);

	T_TDMF_K Key;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	Key = pDoc->m_pAttrCtrl->GetTdmtKey(m_tdmt_d.strMatlName);

	CString str; str.Empty();
	str = m_tdmt_d.strMatlName;
	
	T_KEY_LIST aKey;
	pDoc->m_pAttrCtrl->GetUchtKeyList(aKey);
	
	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_ST_FUNC;  // Time Dependent Material Function.
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = Key;
	
	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if(nCount==0) csName = _LS(IDS_CMD_TDMT_DYNA_GEN_INITIAL_NAME);//_T("Show Time Dependent Material Function");
	else csName.Format(_LS(IDS_CMD_TDMT_DYNA_GEN_INITIAL_NAME) + _T("%d"), nCount+1);
	
	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if(dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;

		UchtD.nFuncType = m_nDataType;
		UchtD.Data.Grph.bTimeorFunc = m_bIndia;
		UchtD.Data.Grph.nGraphType = m_nOptionForIndia;
		UchtD.Data.Grph.nKeyX = m_nSteps;
		
		UchtD.dDelay = m_dLoadingAge;
		UchtD.dMaxTemper = m_dEndLoadingAge;

		UchtD.bLogX = m_btnXLog.GetCheck();
		UchtD.bLogY = m_btnYLog.GetCheck();
		if(!pDoc->m_pDataCtrl->AddUcht(UchtD)) { ASSERT(0); return; }
	}
	
}

void CMTdmfItemShowDlg::OnRedraw()
{
	UpdateData(TRUE);

	if(m_dLoadingAge <= 1.e-5 || m_dEndLoadingAge <= 1.e-5) return;

	T_UNIT_SYSTEM us;
	m_ParamData.nMatlFuncType = m_nDataType+1;
	m_ParamData.nCreepType = 3;
	m_wndGrid.SetHeaderTitle(us, FALSE);

	if(!GetTDMData())
	m_ParamData.Initialize();
	m_wndGrid.MakeItemEx();
	
	T_TDMF_BASE data;
	int nSize = m_ParamData.arMatlFuncData.GetSize();
	
	m_Graph.KillData();	
	double dValue;
	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.arMatlFuncData[i];

		if(m_nDataType == 0)
		{
			dValue =data.dValue;
			if(m_bIndia && m_nOptionForIndia==0) dValue *= 1.0e4;
			else if(m_bRussia)                   dValue *= 1.0e5;
		}
		else
		{
			// KYE-HONG-2003-09-25
			//dValue =fabs(data.dValue);
			if(m_bAustralia || m_bRussia || m_bNewzealand) dValue =data.dValue*1.0E4;
			else                          dValue =data.dValue*-1.0E4;
		}

		SetValue(i,0,data.dDay, dValue);
	}
	m_GraphView.Invalidate();
}

void CMTdmfItemShowDlg::SetValue(int nIndex, int nGroup, double x,double y)
{
	
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
	
}


void CMTdmfItemShowDlg::InitChart()
{
	////////////////////////////////////////////////////////////////
	// Initialize m_Graph
	
	m_GraphView.SetDragable(FALSE); // set data-drag unable
	// Add Background component
	//
	SRGraphBackground	*pB=new SRGraphBackground;
	pB->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pB->GetStyle()->SetColor(CXCLR_WHITE);
	m_Graph.AddComponent((SRGraphComponent *) pB);
	
	// Add Display Component
	//

	SRGraphDisplay *pDisplay=new SRGraphDisplay;	
	pDisplay->GetStyle()->SetGraphStyle(CX_GRAPH_XYSCATTERG_EX);
	pDisplay->GetStyle()->SetAxisStyle(CX_AXIS_XYSCATTER);
	pDisplay->GetStyle()->SetColor(CXCLR_WHITE);
	pDisplay->GetStyle()->SetGridColor(CXCLR_GRAY40);
	pDisplay->GetStyle()->SetComponentFillStyle(CX_SOLID_FILL);
	pDisplay->GetStyle()->SetComponentBorderStyle(CX_NO_BORDER);
	pDisplay->SetMeasurement(CX_PERCENT);
	pDisplay->SetSizeDefaults();
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
#ifdef _JP
	pDisplay->SetRect(0.2,0,100,95.0);
#else 
	pDisplay->SetRect(0.2,0,100,97.0);
#endif
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);
	
	
	// Logarithmic Axis Scaling
	//
	pDisplay->GetStyle()->SetLogX(m_LogFlagX);
	pDisplay->GetStyle()->SetLog(m_LogFlagY);

	//pDisplay->GetStyle()->SetShowXGrid(m_GridXFlag);
	//pDisplay->GetStyle()->SetShowYGrid(m_GridYFlag);
	

	m_Graph.AddComponent(pDisplay);

	// Add Label Component for X-Axis
	//
	SRGraphLabel	*pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::TopCenter);
	pLabel->SetTextPosition(50.0, 93.5);
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Normal);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	m_Graph.AddComponent(pLabel);
	SetAxisLabelText(0,_LS(IDS_WG_CMD__ADDD__Time__Day_));
	
	// Add Label Component for Y-Axis
	//
	pLabel=new SRGraphLabel;
	pLabel->SetMeasurement(SRGraphComponent::PERCENT);
	pLabel->SetLocationPoint(SRGraphLabel::MidCenter);
	pLabel->SetTextPosition(1.5, 50.0);
	pLabel->SetFontSize(9);
	pLabel->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pLabel->SetBlockMember(FALSE);
	pLabel->SetParent(&m_Graph);
	pLabel->SetLabelStandard(SRGraphLabel::Upward);
	pLabel->GetStyle()->SetColor(CXCLR_WHITE);
	//pLabel->GetStyle()->SetColor(CXCLR_BACKGRND);
	m_Graph.AddComponent(pLabel);

	CString YLabel;
	switch(m_nDataType)
	{
		case 0: SetAxisLabelText(1,_LS(IDS_WG_CMD__Creep_Coefficient));break;
		case 1: SetAxisLabelText(1,_LS(IDS_WG_CMD__Shrinkage_Strain2));	break;
	}
	if(m_bIndia && m_nDataType==0)
	{
		if(m_nOptionForIndia==0) SetAxisLabelText(1,_LS(IDS_WG_CMD__Creep_India));
		if(m_nOptionForIndia==1) SetAxisLabelText(1,_LS(IDS_WG_CMD__Creep_Coefficient));
	}
	if(m_bRussia && m_nDataType==0)
	{
		SetAxisLabelText(1, _LS(IDS_WG_CMD__Creep_Strain2));
	}
	
	/////////////////////////////////////////////////////////////
	// Attatch graphview to dialog's placeholder and connect graph to graphview
	
	CWnd* wnd = GetDlgItem(IDC_CMD_PLACEHOLDER);
	CRect rc;

	wnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	rc.left+=2;
	rc.top+=2;
	rc.right -= 2;
	rc.bottom -= 2;

	m_GraphView.Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, IDC_CMD_PLACEHOLDER);
	m_GraphView.m_pGraph = &m_Graph;
	
	m_GraphView.Invalidate();

}


void CMTdmfItemShowDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CMTdmfItemShowDlg::OnETCSPFCCHARTXLog() 
{
		
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLogX(m_btnXLog.GetCheck());
	
	if (m_btnXLog.GetCheck() == TRUE || m_btnYLog.GetCheck() == TRUE) 
	{
		pD->GetStyle()->SetScaleLimit(TRUE);
	  pD->SetMinRangeY(0.001);   
	  pD->SetMinRangeX(0.001);   
	}
	else 
	 	pD->GetStyle()->SetScaleLimit(FALSE);
	
	m_GraphView.Invalidate();	

	
}

void CMTdmfItemShowDlg::OnETCSPFCCHARTYLog() 
{
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLog(m_btnYLog.GetCheck());

	if (m_btnXLog.GetCheck() == TRUE || m_btnYLog.GetCheck() == TRUE) 
	{
		pD->GetStyle()->SetScaleLimit(TRUE);
	  pD->SetMinRangeY(0.001);
		pD->SetMinRangeX(0.001);
	}
	else 
	 	pD->GetStyle()->SetScaleLimit(FALSE);

	m_GraphView.Invalidate();	

}

void CMTdmfItemShowDlg::NewData()
{
	m_ParamData.arMatlFuncData.RemoveAll();
	m_Graph.KillData();
	m_btnXLog.SetCheck(FALSE);
	m_btnYLog.SetCheck(FALSE);
	OnETCSPFCCHARTYLog();
	OnETCSPFCCHARTXLog();
}


BOOL CMTdmfItemShowDlg::GetTDMData()
{
//	char		yytext[80];
	
	T_TDMF_BASE data;
	NewData();

	CArray<double,double> aDay;
	CArray<double,double> aValue;
	CCMTdmtCalcCrpShk calc;           // PMS:XXXX-JWKWON-20100203 : Dyna.Gen Chart부분에서도 사용하기 위해서 wg_db쪽으로 파일을 옮김
	T_TDMT_D TdmtData = m_tdmt_d;
	CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitTdmtIn(TdmtData);
	calc.Calc_CrpShk(TdmtData,m_nDataType,m_nSteps,m_dLoadingAge,m_dEndLoadingAge,aDay,aValue, m_nOptionForIndia);	
	
	for(int i=0; i<aDay.GetSize(); i++)
	{
		data.dDay = aDay[i];
		data.dValue = aValue[i];
		m_ParamData.arMatlFuncData.Add(data);				
	}
	return TRUE;
}


double CMTdmfItemShowDlg::UnitCvt(double dReadData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	return  pDoc->m_pUnitCtrl->ConvertUnitLengthPrevious(dReadData);
}


void CMTdmfItemShowDlg::OnCmdTdmfDataType() 
{
	UpdateData();
	switch(m_nDataType)
	{
		case 0: SetAxisLabelText(1,_LS(IDS_WG_CMD__Creep_Coefficient));break;
		case 1: SetAxisLabelText(1,_LS(IDS_WG_CMD__Shrinkage_Strain2));	break;
	}
	if(m_bIndia && m_nDataType==0)
	{
		if(m_nOptionForIndia==0) SetAxisLabelText(1,_LS(IDS_WG_CMD__Creep_India));
		if(m_nOptionForIndia==1) SetAxisLabelText(1,_LS(IDS_WG_CMD__Creep_Coefficient));
	}
	// India Sub Option En/Disable 처리
	if(m_bIndia)
	{
		GetDlgItem(IDC_CMD_TDMF_CREEP_INDIA_RDO)->EnableWindow(m_nDataType==0);
		GetDlgItem(IDC_CMD_TDMF_CREEP_INDIA_RDO2)->EnableWindow(m_nDataType==0);
	}
	if(m_bRussia && m_nDataType==0)
	{
		SetAxisLabelText(1, _LS(IDS_WG_CMD__Creep_Strain2));
	}
	OnRedraw();
}

void CMTdmfItemShowDlg::OnCmdTdmfOptionForIndia() 
{
	if(!m_bIndia) return;

	UpdateData();
	if(m_bIndia && m_nDataType==0)
	{
		if(m_nOptionForIndia==0) SetAxisLabelText(1,_LS(IDS_WG_CMD__Creep_India));
		if(m_nOptionForIndia==1) SetAxisLabelText(1,_LS(IDS_WG_CMD__Creep_Coefficient));
	}
	OnRedraw();
}
