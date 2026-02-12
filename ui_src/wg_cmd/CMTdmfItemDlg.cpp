#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmfItemDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMTdmfItemDlg dialog
CCMTdmfItemDlg::CCMTdmfItemDlg(CWnd* pParent /*=NULL*/)
: CDialogMove(CCMTdmfItemDlg::IDD, pParent)
{
	m_nMatlFuncType     = TDMF_FUNC_CREEP;
	m_bSetParam         = FALSE;
	m_bGridInit         = FALSE;
	m_ParamData.Initialize();

	m_csFuncName        = _T("");
	m_csDescription     = _T("");
	m_nCreepType    = -1;

	m_arCreepTypeGrp.RemoveAll();
	m_arCreepTypeGrp.Add(IDC_CMD_TDMF_CREEP_FRAME);
	m_arCreepTypeGrp.Add(IDC_CMD_TDMF_SPECIFIC_CREEP);
	m_arCreepTypeGrp.Add(IDC_CMD_TDMF_CREEP_COMPLIANCE);
	m_arCreepTypeGrp.Add(IDC_CMD_TDMF_CREEP_COEFFICIENT);
	m_arCreepTypeGrp.Add(IDC_CMD_TDMF_ELAST_T);
	m_arCreepTypeGrp.Add(IDC_CMD_TDMF_ELAST_E);
	m_arCreepTypeGrp.Add(IDC_CMD_TDMF_ELAST_U);

	m_arBtnGridCtrlID.RemoveAll();
	m_arBtnGridCtrlID.Add(IDC_CMD_TDMF_IMPORT);
	m_arBtnGridCtrlID.Add(IDC_CMD_TDMF_TABLE);

	m_arRelaxTimeGrp.RemoveAll();
	m_arRelaxTimeGrp.Add(IDC_CMD_TDMF_RELAX_TIME_GRP);
	m_arRelaxTimeGrp.Add(IDC_CMD_TDMF_RELAX_TIME_HOUR_RDO);
	m_arRelaxTimeGrp.Add(IDC_CMD_TDMF_RELAX_TIME_DAY_RDO);

	m_arRelaxTimeRdo.RemoveAll();
	m_arRelaxTimeRdo.Add(IDC_CMD_TDMF_RELAX_TIME_HOUR_RDO);
	m_arRelaxTimeRdo.Add(IDC_CMD_TDMF_RELAX_TIME_DAY_RDO);
}

void CCMTdmfItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_TDMF_ELAST_E, m_wndElast);
	DDX_Control(pDX, IDC_CMD_TDMF_ELAST_U, m_wndElastUnit);
	DDX_Control(pDX, IDC_CMD_TDMF_SCALE, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CMD_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_CMD_CHART_XLog, m_btnXLog);
	DDX_Text(pDX, IDC_CMD_TDMF_NAME, m_csFuncName);
	DDX_Text(pDX, IDC_CMD_TDMF_DESCRIPTION, m_csDescription);
	DDX_Radio(pDX, IDC_CMD_TDMF_SPECIFIC_CREEP, m_nCreepType);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_CMD_TDMF_TABLE, m_wndGrid);
	if (pDX->m_bSaveAndValidate)
	{
		m_ParamData.MatlFuncName = m_csFuncName;
		m_ParamData.nCreepType = m_nCreepType + 1;
		m_ParamData.Description = m_csDescription;
		if (!m_wndScaleFactor.GetEditValue(m_ParamData.ScaleFactor))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_BAIRITU));
			pDX->Fail();
		}
		m_ParamData.dElast = m_wndElast.GetEditValue();
	}
	else
	{
		if (!m_bGridInit) return;
		BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
		ROWCOL nCount = m_ParamData.arMatlFuncData.GetSize();
		m_wndGrid.SetRowCount(nCount+1);
		m_wndGrid.SetCurrentCell(nCount+1, 1);

		m_wndGrid.LockUpdate(bOldLock);
		if (!bOldLock) m_wndGrid.Redraw();
	}
}

BEGIN_MESSAGE_MAP(CCMTdmfItemDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_TDMF_SPECIFIC_CREEP, OnChangeCreepType)
	ON_BN_CLICKED(IDC_CMD_TDMF_CREEP_COMPLIANCE, OnChangeCreepType)
	ON_BN_CLICKED(IDC_CMD_TDMF_CREEP_COEFFICIENT, OnChangeCreepType)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CMD_CHART_XLog, OnETCSPFCCHARTXLog)
	ON_BN_CLICKED(IDC_CMD_CHART_YLog, OnETCSPFCCHARTYLog)
	ON_BN_CLICKED(IDC_CMD_TDMF_IMPORT, OnImportFile)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	ON_BN_CLICKED(IDC_CMD_TDMF_RELAX_TIME_HOUR_RDO, OnRdoRelaxTime)
	ON_BN_CLICKED(IDC_CMD_TDMF_RELAX_TIME_DAY_RDO, OnRdoRelaxTime)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTdmfItemDlg message handlers
BOOL CCMTdmfItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	Data2Dlg();

	// Grid
	InitGrid();
	MakeGridData();

	// Chart
	InitChart();
	MakeChartData();

	AlignControls();
	SetDynReportCtrl();

	UpdateData(FALSE);

	return TRUE;
}

void CCMTdmfItemDlg::Data2Dlg()
{
	if (!m_bSetParam) 
	{
		m_ParamData.Initialize();
		m_ParamData.nMatlFuncType = m_nMatlFuncType;
	}
	else 
		m_nMatlFuncType = m_ParamData.nMatlFuncType;

	m_wndElast.SetUnitType(CUnitCtrl::m_TDMF_UNIT.dElast);
	m_wndElastUnit.SetUnitType(CUnitCtrl::m_TDMF_UNIT.dElast);

	m_csFuncName = m_ParamData.MatlFuncName;
	m_nCreepType = (m_ParamData.nCreepType == 0) ? 0 : m_ParamData.nCreepType-1;

	CString csScale;
	csScale.Format(_T("%g"), m_ParamData.ScaleFactor);
	m_wndScaleFactor.SetWindowText(csScale);

	m_csDescription = m_ParamData.Description;
	m_wndElast.SetEditUnit(m_ParamData.dElast);

	m_LogFlagX = FALSE;
	m_LogFlagY = FALSE;

	int nRelaxTime = m_ParamData.nRelaxTime;
	CDlgUtil::CtrlRadioSetCheck(this, m_arRelaxTimeRdo, nRelaxTime);
}

void CCMTdmfItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_ParamData);
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	m_bGridInit = TRUE;
}

void CCMTdmfItemDlg::MakeGridData() 
{
	m_wndGrid.MakeItemEx();
}

void CCMTdmfItemDlg::AlignControls()
{
	CDlgUtil::CtrlShowHide(this, m_arCreepTypeGrp, m_nMatlFuncType == TDMF_FUNC_CREEP);
	if(m_nMatlFuncType == TDMF_FUNC_CREEP) return;

	if(m_nMatlFuncType == TDMF_FUNC_SHRINKAGE)
	{
		CRect rRef, rToMove, rTableOrg, rTableNew;
		m_wndGrid.GetWindowRect(rTableOrg);
		int nDistY = 0;
		GetDlgItem(IDC_CMD_TDMF_CREEP_FRAME)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_TDMF_IMPORT)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_arBtnGridCtrlID, nDistY, TRUE);

		// resize table
		m_wndGrid.GetWindowRect(rTableNew);
		rTableNew.bottom = rTableOrg.bottom;
		ScreenToClient(rTableNew);
		m_wndGrid.MoveWindow(rTableNew);
	}
	else if(m_nMatlFuncType == TDMF_FUNC_RELAXATION)
	{
		CRect rRef, rToMove;
		int nDistX = 0;
		int nDistY = 0;
		GetDlgItem(IDC_CMD_TDMF_CREEP_FRAME)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_TDMF_RELAX_TIME_GRP)->GetWindowRect(rToMove);
		nDistX = rRef.left - rToMove.left;
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistXY(this, m_arRelaxTimeGrp, nDistX, nDistY, TRUE);
	}
	else 
		ASSERT(0);
}

void CCMTdmfItemDlg::SetDynReportCtrl()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_TDMF_K Key = 0;
	Key = pDoc->m_pAttrCtrl->GetTdmfKey(m_ParamData.MatlFuncName);

	BOOL bEnable = FALSE;
	if(Key > 0) bEnable = TRUE;
	else bEnable = FALSE;

	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage (_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));
	GetDlgItem(IDC_CMD_DYGEN)->EnableWindow(bEnable);

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영
}

T_TDMF_D& CCMTdmfItemDlg::GetParamData()
{
	return m_ParamData;
}

void CCMTdmfItemDlg::SetParamData(T_TDMF_D& data)
{
	m_bSetParam = TRUE;
	m_ParamData = data;
	m_csOldName = m_ParamData.MatlFuncName;
}

void CCMTdmfItemDlg::OnBtnDyGen()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_TDMF_K Key;
	Key = pDoc->m_pAttrCtrl->GetTdmfKey(m_ParamData.MatlFuncName);

	T_KEY_LIST aKey;
	pDoc->m_pAttrCtrl->GetUchtKeyList(aKey);

	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_TM_FUNC; // Time Dependent Material Function.
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = Key;
	UchtD.nFuncType = m_nCreepType;
	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if(nCount==0) csName = _LS(IDS_CMD_DYNA_RPT_TDMFUNC_NAME);//_T("Time Dependent Material Function");
	else csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_TDMFUNC_NAME), nCount+1);

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

void CCMTdmfItemDlg::OnChangeCreepType() 
{
	CString YLabel = _T("");
	const MSG* pMsg = GetCurrentMessage();
	
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_CMD_TDMF_SPECIFIC_CREEP   :  m_nCreepType = 0; YLabel =_LS(IDS_WG_CMD__Specific_Creep);	break;
	case IDC_CMD_TDMF_CREEP_COMPLIANCE :  m_nCreepType = 1; YLabel =_LS(IDS_WG_CMD__Creep_Function);	break;
	case IDC_CMD_TDMF_CREEP_COEFFICIENT:  m_nCreepType = 2; YLabel =_LS(IDS_WG_CMD__Creep_Coefficient);break;
	}
	SetAxisLabelText(1, YLabel);
	m_ParamData.nCreepType = m_nCreepType+1;
	
	T_UNIT_SYSTEM us;
	m_wndGrid.SetHeaderTitle(us, FALSE);
	MakeChartData();
}

void CCMTdmfItemDlg::OnRdoRelaxTime() 
{
	int nRelaxTime = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_arRelaxTimeRdo, nRelaxTime);

	CString strXLabel = _T("");

	switch(nRelaxTime)
	{
	case TDMF_RELAX_TIME_HOUR:  strXLabel =_LS(IDS_WG_CMD__ADDD__Time__hr_);	break;
	case TDMF_RELAX_TIME_DAY :  strXLabel =_LS(IDS_WG_CMD__ADDD__Time__day_);	break;
	default: ASSERT(0);
	}

	m_ParamData.nRelaxTime = nRelaxTime;

	SetAxisLabelText(0, strXLabel);
	m_GraphView.Invalidate();

	T_UNIT_SYSTEM us;
	m_wndGrid.SetHeaderTitle(us, FALSE);
}

BOOL CCMTdmfItemDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN )
	{
	  if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		{
		  if(pMsg->wParam == VK_RETURN)
			if(pMsg->wParam == VK_ESCAPE)
			
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);

			return TRUE;        
		}
	}
	return CDialogMove::PreTranslateMessage(pMsg);
}

void CCMTdmfItemDlg::MakeChartData()
{
	T_TDMF_BASE data;
	int nSize = m_ParamData.arMatlFuncData.GetSize();
	
	m_Graph.KillData();	
	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.arMatlFuncData[i];
		SetValue(i,0,data.dDay, data.dValue);
	}
	m_GraphView.Invalidate();
}

void CCMTdmfItemDlg::SetValue(int nIndex, int nGroup, double x,double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CCMTdmfItemDlg::Dlg2Data() 
{
	int nRelaxTime = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_arRelaxTimeRdo, nRelaxTime);
	
	m_ParamData.nRelaxTime = nRelaxTime;
}

BOOL CCMTdmfItemDlg::Apply() 
{
	if (!UpdateData(TRUE)) return FALSE;
	
	Dlg2Data();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	if (m_bSetParam)
		return pDoc->m_pDataCtrl->ModifyTdmf(m_csOldName, m_ParamData);
	else
		return pDoc->m_pDataCtrl->AddTdmf(m_ParamData);
}

void CCMTdmfItemDlg::OnOK() 
{
	if( !Apply() ) return;

	CDialogMove::OnOK();
}

void CCMTdmfItemDlg::OnApply() 
{
	if( !Apply() ) return;
		
	NewData();
	SetFuncName(_T(""));
	SetDescription(_T(""));

	MakeChartData();
	MakeGridData();
}

void CCMTdmfItemDlg::NewData()
{
	m_ParamData.arMatlFuncData.RemoveAll();
	m_Graph.KillData();
	m_btnXLog.SetCheck(FALSE);
	m_btnYLog.SetCheck(FALSE);
	OnETCSPFCCHARTYLog();
	OnETCSPFCCHARTXLog();
}

void CCMTdmfItemDlg::SetFuncName(CString FuncName)
{
	CWnd* pWnd = GetDlgItem(IDC_CMD_TDMF_NAME);
	pWnd->SetWindowText(FuncName);
	m_csFuncName = FuncName;
}

void CCMTdmfItemDlg::SetDescription(CString Descript)
{
	CWnd* descript = GetDlgItem(IDC_CMD_TDMF_DESCRIPTION);
	descript->SetWindowText(Descript);
	m_csDescription = Descript;
}

void CCMTdmfItemDlg::InitChart()
{
	/////////////////////////////////////////////////////////////
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
	if(m_nMatlFuncType == TDMF_FUNC_RELAXATION && m_ParamData.nRelaxTime == TDMF_RELAX_TIME_HOUR )
		SetAxisLabelText(0,_LS(IDS_WG_CMD__ADDD__Time__hr_));
	else
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

	if(m_nMatlFuncType == TDMF_FUNC_CREEP)
	{	
		CString YLabel;
		switch(m_nCreepType)
		{
			case 0: SetAxisLabelText(1,_LS(IDS_WG_CMD__Specific_Creep));	break;
			case 1: SetAxisLabelText(1,_LS(IDS_WG_CMD__Creep_Function));	break;
			case 2: SetAxisLabelText(1,_LS(IDS_WG_CMD__Creep_Coefficient));break;
		}
	}
	else if(m_nMatlFuncType == TDMF_FUNC_SHRINKAGE)
		SetAxisLabelText(1,_LS(IDS_WG_CMD__Shrinkage_Strain));
	else if(m_nMatlFuncType == TDMF_FUNC_RELAXATION)
		SetAxisLabelText(1,_LS(IDS_CMD_RELAXATION_LOSS_));
	else
		ASSERT(0);

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

void CCMTdmfItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CCMTdmfItemDlg::OnETCSPFCCHARTXLog() 
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

void CCMTdmfItemDlg::OnETCSPFCCHARTYLog() 
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

void CCMTdmfItemDlg::OnImportFile() 
{
	const TCHAR * szFilter = _LS(IDS_CMD0417__TDM_File____tdm____tdm__);
	FILE* fp;
	CFileDialog	fd(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,szFilter);

	if (fd.DoModal() == IDOK) 
	{
		CString csExt = fd.GetFileExt();
		csExt.MakeUpper();
		if (csExt == _T("TDM"))
		{
			if(!( fp = _tfopen(fd.GetPathName().operator LPCTSTR(), _T("r")) ) )
			{ AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_opening_f));
			return;
			}

			if(!ReadTDMData(fp))
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occurred_while_reading_d));
				return;
			}

			SetDescription(fd.GetPathName());

			MakeGridData();
			SetFuncName(fd.GetFileTitle());
			MakeChartData();
		}
		else 
		{
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_extension_));
			return;
		}
	}
}

BOOL CCMTdmfItemDlg::ReadTDMData(FILE *fp)
{
	char		yytext[80];

	T_TDMF_BASE data;
	NewData();

	UpdateData(TRUE);
	m_ParamData.nCreepType = m_nCreepType + 1;

	// Read and Set Spectrum data
	//
	try
	{
		////////////////////////////////////////////////////////////////////
		// UNIT & Type

		// Unit
		CStringA sUnit,sType,sText;
		int nUnitLength, nUnitForce;
		T_UNIT_INDEX UnitIndexCurrent;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexCurrent); 

		lex(fp, yytext); // asterisk
		lex(fp, yytext);
		sText = yytext;
		sText.MakeUpper();
		sText.TrimLeft(); sText.TrimRight();

		CStringA csUnit = CStringA(_LS(IDS_WG_CMD__ADD2__UNIT)); csUnit.MakeUpper();
		CStringA csData = CStringA(_LS(IDS_WG_CMD__ADD2__DATA)); csData.MakeUpper();
				
		sUnit = "";
		if (sText == csUnit) 
		{
		  lex(fp, yytext);		// comma
		  lex(fp, yytext);
		  sUnit = yytext;
			sUnit.MakeUpper();
			sUnit.TrimLeft(); sUnit.TrimRight();
			if(sUnit == "MM" ) nUnitLength = 0;
			else if(sUnit == "CM" ) nUnitLength =1;
			else if(sUnit == "M" ) nUnitLength = 2;
			else if(sUnit == "INCH" || sUnit == "IN") nUnitLength =3;
			else if(sUnit == "FEET" || sUnit == "FT") nUnitLength =4;
			else 
			{
			 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_UNIT_type_));
			 fclose(fp);
			 return FALSE;
			}
			
		  lex(fp, yytext);		// comma
		  lex(fp, yytext);
		  sUnit = yytext;
			sUnit.MakeUpper();
			sUnit.TrimLeft(); sUnit.TrimRight();
			if(sUnit == "KG" || sUnit == "KGF" ) nUnitForce = 0;
			else if(sUnit == "TON" || sUnit == "TONF" ) nUnitForce =1;
			else if(sUnit == "N" ) nUnitForce = 2;
			else if(sUnit == "KN" ) nUnitForce =3;
			else if(sUnit == "LBF" ) nUnitForce =4;
			else if(sUnit == "KIP" || sUnit == "KIPS") nUnitForce =5;
			else 
			{
			 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_UNIT_type_));
			 fclose(fp);
			 return FALSE;
			}

			lex(fp, yytext);		// asterisk
		  lex(fp, yytext);
			sText = yytext;
			sText.MakeUpper();
		}
		else
		{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Data_Unit_Information___nReadi));
			  nUnitLength= UnitIndexCurrent.nBase_Length; // 단위변환 없음.
				nUnitForce = UnitIndexCurrent.nBase_Force;
		}

		//////////////////////////////////////////////////////////////
		T_UNIT_INDEX UnitIndexOld, UnitIndexNew;
	  pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndexOld);
	  UnitIndexNew=UnitIndexOld;
	  UnitIndexNew.nBase_Length=nUnitLength;
		UnitIndexNew.nBase_Force =nUnitForce;

		pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexNew); // 읽은 단위계에서 현재 단위계로
	  pDoc->m_pUnitCtrl->SetUnitIndexCurrent(UnitIndexOld); // conversion 을 하기 위해서...
		////////////////////////////////////////////////////////////////
	
	  if(sText == csData) 
		{
			while (lex(fp, yytext)==1) // Read X-Axis Value
			{	
				// Set X-Axis Value
				data.dDay = atof(yytext);
				lex(fp, yytext);	// comma
				if(strcmp(yytext, ",") != 0) 
				{
				 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Wrong_Format_));
				 fclose(fp);
				 return FALSE;
				}
				
				lex(fp, yytext);

				// Set Y-Axis Value
				//data.dValue = UnitCvt(atof(yytext));
				data.dValue = atof(yytext);
				m_ParamData.arMatlFuncData.Add(data);				
			}
			pDoc->m_pUnitCtrl->ConvertUnitTdmfPrevious(m_ParamData);
		}
		else
		{
				 AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Data_or_Wrong_Format_));
		}

	}
	catch (CFileException* e)  /// useless because it was not CFile;
	{
		e;  // not used
		fclose(fp);
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__an_Error_occured_while_reading_da));
		return FALSE;
	}

	fclose(fp);
	return TRUE;
}

int CCMTdmfItemDlg::lex(FILE *fp, char *yytext)
{
	int		c, index;

	index	= 0;
	c		= getc(fp);

	// Discard white characters
	while ((c==' ') || (c=='\r') || (c=='\t') || (c=='\n')) {
		c = getc(fp);
	}

	if (c == EOF ) return -1;
	
	// Handle(Discard) Comments
	while (c == '*') {
		c = getc(fp);
		if (c == '*') {
			c = getc(fp);
			while (c != '\n')
				c = getc(fp);
			c = getc(fp);
		}
		else {
			ungetc(c, fp);
			yytext[index++] = '*';
			yytext[index] = '\0';
			return 1;
		}
	}

	switch (c) {
		case ',':
			yytext[index++] = c;
			yytext[index] = '\0';
			break ;
		default:
			while ((c!='*') && (c!=',') && (c!='\r') && (c!='\n')) {
				yytext[index++] = c;
				c = getc(fp);
				if (c == EOF || index >=120)
					return -1;
			}
			yytext[index] = '\0';
			if ((c=='*') || (c==','))
				ungetc(c, fp);
	}
	
	return 1;
}

double CCMTdmfItemDlg::UnitCvt(double dReadData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	return  pDoc->m_pUnitCtrl->ConvertUnitLengthPrevious(dReadData);
}

// 사용하지 않는 코드인듯.
// void CCMTdmfItemDlg::ChangeCreepType(int nType)
// {
//   if (m_nCreepType == nType) return;
//   UINT aID[] = { 
//     IDC_CMD_TDMF_SPECIFIC_CREEP,
//     IDC_CMD_TDMF_CREEP_COMPLIANCE,
//     IDC_CMD_TDMF_CREEP_COEFFICIENT,
//   };
//   // Turn off old radio
//   CButton* pRadio = (CButton*)GetDlgItem(aID[m_nCreepType]);
//   pRadio->SetCheck(0);
// 
//   // Turn on new radio
//   m_nCreepType = nType;
//   pRadio = (CButton*)GetDlgItem(aID[m_nCreepType]);
//   pRadio->SetCheck(1);
// 
//   T_UNIT_SYSTEM us;
//   m_ParamData.nCreepType = m_nCreepType+1;
//   m_wndGrid.SetHeaderTitle(us, FALSE);
// }