// FlueItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SteelTimeTempItemDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "ETC_DESIGN_SPECT.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"
#include "..\wg_base\DlgUtil.h"

#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSteelTimeTempItemDlg dialog


CSteelTimeTempItemDlg::CSteelTimeTempItemDlg(T_STTF_K sttfK /* = 0 */, CWnd* pParent /* = NULL */)
	: CDialogMove(CSteelTimeTempItemDlg::IDD, pParent),m_wndGrid(TRUE)
{
	if(sttfK != 0)
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		T_STTF_D SttfD;
		if (!pDoc->m_pAttrCtrl->GetSttf(sttfK,SttfD)) return;
		SetParamData(SttfD);
	}
	else
	{
		m_bSetParam = FALSE;
		m_ParamData.Initialize();
		m_csOldName = _T("");
		m_csFuncName = _T("");
		m_dScale = 1.0;
	}
	m_bGridInit = FALSE;

	//{{AFX_DATA_INIT(CSteelTimeTempItemDlg)
	
	m_aCodeContorl.Add(IDC_WG_CMD_STATIC3);
	m_aCodeContorl.Add(IDC_WG_CMD_STATIC5);
	m_aCodeContorl.Add(IDC_CMD_FLUE_COMBO_CODE);
	m_aCodeContorl.Add(IDC_FLUE_TYPE_FIBER);
	m_aCodeContorl.Add(IDC_FLUE_TYPE_HC);
	m_aCodeContorl.Add(IDC_WG_CMD_STATIC50);
	m_aCodeContorl.Add(IDC_WG_CMD_STATIC51);
	m_aCodeContorl.Add(IDC_FLUE_FUNC_DURATION);
	m_aCodeContorl.Add(IDC_WG_CMD_STATIC6);
	memset(m_dMnMxXVal,0,sizeof(m_dMnMxXVal));
	memset(m_dMnMxYVal,0,sizeof(m_dMnMxYVal));
}

void CSteelTimeTempItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSteelTimeTempItemDlg)
	
	//DDX_Control(pDX, IDC_FLUE_GRAVITY, m_wndGravity);
	//DDX_Control(pDX, IDC_FLUE_GRAVITY_UNIT, m_wndGravityUnit);
	DDX_Control(pDX, IDC_ETC_FLUE_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_ETC_FLUE_CHART_XLog, m_btnXLog);
	DDX_Text(pDX, IDC_FLUE_FUNC_NAME, m_csFuncName);
	DDX_Text(pDX, IDC_FLUE_FUNC_SCALE, m_dScale);
	//DDX_Control(pDX, IDC_FLUE_SCALE_UNIT, m_wndScaleUnit);
	//DDX_Control(pDX, IDC_FLUE_SCALE2, m_wndScaleMaxVal);
	//DDX_Control(pDX, IDC_CMD_APPLY, m_bAPPLY);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_FLUE_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP
}

T_STTF_D& CSteelTimeTempItemDlg::GetParamData()
{
	return m_ParamData;
}

void CSteelTimeTempItemDlg::SetParamData(T_STTF_D& data)
{
	m_bSetParam = TRUE;
	m_ParamData = data;    
	m_csOldName = m_ParamData.strSttfName;
	m_csFuncName = m_ParamData.strSttfName;
	m_dScale = m_ParamData.dFactor;
}

void CSteelTimeTempItemDlg::OnBtnDyGen()
{
	UpdateData(TRUE);

	T_STTF_K Key;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	Key = pDoc->m_pAttrCtrl->GetSttfKey(m_ParamData.strSttfName);

	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_STTF_FUNC;  // RS Func.
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = Key;

	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if (nCount == 0) csName = _LS(IDS_CMD_DYNA_RPT_STTFFUNC_NAME); //_T("Stl-Time-Temp Fun");
	else csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_STTFFUNC_NAME), nCount + 1);

	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if(dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;

		UchtD.bLogX = m_btnXLog.GetCheck();
		UchtD.bLogY = m_btnYLog.GetCheck();
		if(!pDoc->m_pDataCtrl->AddUcht(UchtD)) { ASSERT(0); return; }
	}

}
void CSteelTimeTempItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_ParamData.arTimeTempData);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	m_bGridInit = TRUE;
}

BEGIN_MESSAGE_MAP(CSteelTimeTempItemDlg, CDialogMove)
	ON_BN_CLICKED(IDC_ETC_FLUE_CHART_XLog, OnETCFLUECHARTXLog)
	ON_BN_CLICKED(IDC_ETC_FLUE_CHART_YLog, OnETCFLUECHARTYLog)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSteelTimeTempItemDlg message handlers

BOOL CSteelTimeTempItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (m_bSetParam) 
	{
		GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	}

	InitGrid();
	InitChart();

	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;

	m_wndGrid.MakeItemEx();

	
	OnETCFLUECHARTXLog();
	OnETCFLUECHARTYLog();	

	T_FLUE_K Key;
	CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
	Key = m_pDoc->m_pAttrCtrl->GetSttfKey(m_ParamData.strSttfName);
	//BOOL bEnable;
	//if(Key > 0) bEnable = TRUE;
	//else bEnable = FALSE;

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	//GetDlgItem(IDC_CMD_DYGEN)->EnableWindow(bEnable);

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영

	DrawGraphView();
	return TRUE;
}

void CSteelTimeTempItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE)) return;
	if(!Dlg2Data())
		return;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;

	if (m_bSetParam)
		bSuccess = pDoc->m_pDataCtrl->ModifySttf(m_csOldName, m_ParamData);
	else  
		bSuccess = pDoc->m_pDataCtrl->AddSttf(m_ParamData);

	if (!bSuccess) return;  

	CDialogMove::OnOK();
}

BOOL CSteelTimeTempItemDlg::PreTranslateMessage(MSG* pMsg) 
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

void CSteelTimeTempItemDlg::InitChart()
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
	pDisplay->SetRect(0.2,0,100,97.0);
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(FALSE);
	m_Graph.AddComponent(pDisplay);

	// Add Label Component for X-Axis	
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
	SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__FLUE_sec_));

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
	SetAxisLabelText(1,_LS(IDS_WG_CMD__ADDD__FLUE_TemPerature_));

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

void CSteelTimeTempItemDlg::SetValue(int nIndex, int nGroup, double x,double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CSteelTimeTempItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CSteelTimeTempItemDlg::OnETCFLUECHARTXLog() 
{
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLogX(m_btnXLog.GetCheck());
	m_GraphView.Invalidate();		
}

void CSteelTimeTempItemDlg::OnETCFLUECHARTYLog() 
{
	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->GetStyle()->SetLog(m_btnYLog.GetCheck());
	m_GraphView.Invalidate();	
}

void CSteelTimeTempItemDlg::NewData()
{
	m_ParamData.strSttfName.Empty();
	m_ParamData.arTimeTempData.RemoveAll();
	m_Graph.KillData();

	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;
	m_btnXLog.SetCheck(m_LogFlagX);
	m_btnYLog.SetCheck(m_LogFlagY);
	OnETCFLUECHARTYLog();
	OnETCFLUECHARTXLog();
}

void CSteelTimeTempItemDlg::SetFuncName(CString FuncName)
{
	CWnd* pWnd = GetDlgItem(IDC_FLUE_FUNC_NAME);
	pWnd->SetWindowText(FuncName);
}

void CSteelTimeTempItemDlg::OnApply() 
{
	// TODO: Add extra validation here
	if (!UpdateData(TRUE)) return;
	if(!Dlg2Data())
		return;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;

	if (m_bSetParam)
		bSuccess = pDoc->m_pDataCtrl->ModifySttf(m_csFuncName, m_ParamData);
	else  
		bSuccess = pDoc->m_pDataCtrl->AddSttf(m_ParamData);

	if (!bSuccess) return;

	NewData();
	SetFuncName(_T(""));
	//m_wndGrid.MakeItemEx();
}

BOOL CSteelTimeTempItemDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_ParamData.strSttfName = m_csFuncName;
	m_ParamData.dFactor = m_dScale;
	if(m_ParamData.arTimeTempData.IsEmpty())
		return FALSE;
	return TRUE;
}

void CSteelTimeTempItemDlg::OnChangeScaleRdo()
{
	UpdateData();
	BOOL bScale = TRUE;
	GetDlgItem(IDC_FLUE_FUNC_SCALE)->EnableWindow(bScale);
}

void CSteelTimeTempItemDlg::DrawGraphView()
{  
	// 1. Add handler to update graph    
	T_FLUE_BASE data;
	int nSize = m_ParamData.arTimeTempData.GetSize();

	memset(m_dMnMxXVal,0,sizeof(m_dMnMxXVal));
	memset(m_dMnMxYVal,0,sizeof(m_dMnMxYVal));

	m_Graph.KillData();	
	if(!nSize)
	{
		m_GraphView.Invalidate();  
		return;
	}

	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.arTimeTempData[i];
		SetValue(i,0,data.dblPeriod,data.dblValue);

		if(i==0)
		{
			m_dMnMxXVal[0] = data.dblPeriod;
			m_dMnMxXVal[1] = data.dblPeriod;
			m_dMnMxYVal[0] = data.dblValue;
			m_dMnMxYVal[1] = data.dblValue;
			continue; 
		}

		if(data.dblPeriod < m_dMnMxXVal[0]) m_dMnMxXVal[0] = data.dblPeriod;
		if(data.dblPeriod > m_dMnMxXVal[1]) m_dMnMxXVal[1] = data.dblPeriod;

		if(data.dblValue < m_dMnMxYVal[0]) m_dMnMxYVal[0] = data.dblValue;
		if(data.dblValue > m_dMnMxYVal[1]) m_dMnMxYVal[1] = data.dblValue;
	}

	double dX = (m_dMnMxXVal[1]-m_dMnMxXVal[0])/20.;
	double dY = (m_dMnMxYVal[1]-m_dMnMxYVal[0])/20.;

	SRGraphDisplay	*pD=(SRGraphDisplay *) m_Graph.GetComponent(0, IDS_SRG_DISPLAYTYPE);
	pD->SetMinRangeY(m_dMnMxYVal[0] - dY);   
	pD->SetMinRangeX(0.01); 
	pD->SetMaxRangeY(m_dMnMxYVal[1] + dY);   
	pD->SetMaxRangeX(m_dMnMxXVal[1] + dX);
	pD->GetStyle()->SetScaleLimit(FALSE);
	pD->GetStyle()->SetUseMaxScale(TRUE);    

	m_GraphView.Invalidate();  
}