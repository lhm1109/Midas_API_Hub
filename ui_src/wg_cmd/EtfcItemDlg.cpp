// EtfcItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EtfcItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtfcItemDlg dialog


CEtfcItemDlg::CEtfcItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEtfcItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEtfcItemDlg)
	m_type = -1;
	m_csFuncName = _T("");
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bGridInit = FALSE;

	m_aConstCtrl.Add(IDC_ETFC_CONST_FRAME);
	m_aConstCtrl.Add(IDC_ETFC_CONST_TEMPER_TITLE);
	m_aConstCtrl.Add(IDC_ETFC_CONST_TEMPER_EDIT);
	m_aConstCtrl.Add(IDC_ETFC_CONST_TEMPER_UNIT);

	m_aSineCtrl.Add(IDC_ETFC_SIN_FRAME);
	m_aSineCtrl.Add(IDC_ETFC_SIN_ST2);
	m_aSineCtrl.Add(IDC_ETFC_SIN_ST4);
	m_aSineCtrl.Add(IDC_ETFC_SIN_ST5);
	m_aSineCtrl.Add(IDC_ETFC_SIN_ST6);
	m_aSineCtrl.Add(IDC_ETFC_SIN_ST7);
	m_aSineCtrl.Add(IDC_ETFC_SIN_T_TITLE);
	m_aSineCtrl.Add(IDC_ETFC_SIN_T_EDIT);
	m_aSineCtrl.Add(IDC_ETFC_SIN_T_UNIT);
	m_aSineCtrl.Add(IDC_ETFC_SIN_TO_TITLE);
	m_aSineCtrl.Add(IDC_ETFC_SIN_TO_EDIT);
	m_aSineCtrl.Add(IDC_ETFC_SIN_TO_UNIT);
	m_aSineCtrl.Add(IDC_ETFC_SIN_TO_TITLE2);
	m_aSineCtrl.Add(IDC_ETFC_SIN_TO_EDIT2);
	m_aSineCtrl.Add(IDC_ETFC_SIN_TO_UNIT2);

	m_aUserCtrl.Add(IDC_ETFC_TABLE);
}


void CEtfcItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtfcItemDlg)
	DDX_Control(pDX, IDC_ETFC_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_ETFC_CHART_XLog, m_btnXLog);
	DDX_Control(pDX, IDC_ETFC_SIN_TO_UNIT2, m_meantemperUnit);
	DDX_Control(pDX, IDC_ETFC_SIN_TO_EDIT2, m_meantemper);
	DDX_Control(pDX, IDC_ETFC_SIN_TO_UNIT, m_delayUnit);
	DDX_Control(pDX, IDC_ETFC_SIN_TO_EDIT, m_delay);
	DDX_Control(pDX, IDC_ETFC_SIN_T_UNIT, m_maxtemperUnit);
	DDX_Control(pDX, IDC_ETFC_SIN_T_EDIT, m_maxtemper);
	DDX_Control(pDX, IDC_ETFC_CONST_TEMPER_UNIT, m_temperUnit);
	DDX_Control(pDX, IDC_ETFC_CONST_TEMPER_EDIT, m_temper);
	DDX_Control(pDX, IDC_ETFC_SCALE, m_wndScaleFactor);
	DDX_Radio(pDX, IDC_ETFC_TYPE_CONST, m_type);
	DDX_Text(pDX, IDC_ETFC_FUNC_NAME, m_csFuncName);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_ETFC_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		m_ParamData.strFuncName = m_csFuncName;
		m_ParamData.nFuncType = m_type+1;
		if (!m_wndScaleFactor.GetEditValue(m_ParamData.dScaleFactor))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_));
			pDX->Fail();
		}
		m_ParamData.dTemperature = m_temper.GetEditValue();
		m_ParamData.dMaxTemp = m_maxtemper.GetEditValue();
		m_ParamData.dDelayTime = m_delay.GetEditValue();
		m_ParamData.dMeanTemp = m_meantemper.GetEditValue();
	}
	else
	{
		if (!m_bGridInit) return;
		BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
		ROWCOL nCount = m_ParamData.aFuncData.GetSize();
		m_wndGrid.SetRowCount(nCount+1);
		m_wndGrid.SetCurrentCell(nCount+1, 1);

		m_wndGrid.LockUpdate(bOldLock);
		if (!bOldLock) m_wndGrid.Redraw();
	}
}


BEGIN_MESSAGE_MAP(CEtfcItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CEtfcItemDlg)
	ON_BN_CLICKED(IDC_ETFC_TYPE_CONST, OnEtfcSelChangeType)
	ON_BN_CLICKED(IDC_ETFC_SIN_REDRAW_GRAPH, OnEtfcSinRedrawGraph)
	ON_BN_CLICKED(IDC_ETFC_TYPE_SINE, OnEtfcSelChangeType)
	ON_BN_CLICKED(IDC_ETFC_TYPE_USER, OnEtfcSelChangeType)
	ON_BN_CLICKED(IDC_ETFC_CHART_XLog, OnETFCCHARTXLog)
	ON_BN_CLICKED(IDC_ETFC_CHART_YLog, OnETFCCHARTYLog)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEtfcItemDlg message handlers

BOOL CEtfcItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
		
	// TODO: Add extra initialization here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	AlignControls();

	// init unit /////////////////////////////////////////////////
	m_delayUnit.SetUnitType(CUnitCtrl::m_ETFC_UNIT.dDelayTime);
	m_maxtemperUnit.SetUnitType(CUnitCtrl::m_ETFC_UNIT.dMaxTemp);
	m_temperUnit.SetUnitType(CUnitCtrl::m_ETFC_UNIT.dTemperature);
	m_meantemperUnit.SetUnitType(CUnitCtrl::m_ETFC_UNIT.dMeanTemp);
	//////////////////////////////////////////////////////////////

	if(!m_bModify)
	{
		m_ParamData.Initialize();
		m_ParamData.nFuncType = 3;
	}

	CString str;
	m_csFuncName = m_ParamData.strFuncName;
	m_type = m_ParamData.nFuncType-1;
	str.Format(_T("%g"), m_ParamData.dScaleFactor);
	m_wndScaleFactor.SetRange(1, SHRT_MAX);
	m_wndScaleFactor.SetWindowText(str);
	m_temper.SetEditUnit(m_ParamData.dTemperature);
	m_maxtemper.SetEditUnit(m_ParamData.dMaxTemp);
	m_delay.SetEditUnit(m_ParamData.dDelayTime);
	m_meantemper.SetEditUnit(m_ParamData.dMeanTemp);



	InitGrid();
//  m_LogFlagX = FALSE;
//  m_LogFlagY = FALSE;
	InitChart();
	m_wndGrid.MakeItemEx();

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	UpdateData(FALSE);
	OnEtfcSelChangeType();

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEtfcItemDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove1, rToMove2;

	// 1st point 
	GetDlgItem(IDC_ETFC_TABLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_ETFC_CONST_FRAME)->GetWindowRect(rToMove1);
	GetDlgItem(IDC_ETFC_SIN_FRAME)->GetWindowRect(rToMove2);
	int nDistY1 = rRef.top - rToMove1.top;
	int nDistY2 = rRef.top - rToMove2.top;
	CDlgUtil::CtrlMoveDistY(this, m_aConstCtrl, nDistY1);
	CDlgUtil::CtrlMoveDistY(this, m_aSineCtrl, nDistY2);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

void CEtfcItemDlg::OnEtfcSelChangeType() 
{
	// TODO: Add your control notification handler code here
	int nPrevType = m_type;
	UpdateData();

	if (nPrevType == 2 && m_type != 2)  // user
	{
		if (!m_wndGrid.OnTerminate(D_TB_SAVE_ALWAYS))
			m_wndGrid.CancelEdit();
	}

	if(m_type == 0)
	{
		CDlgUtil::CtrlShowHide(this, m_aConstCtrl, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aSineCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, FALSE);
		m_wndScaleFactor.SetWindowText(_T("1.0"));
		m_wndScaleFactor.EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else if(m_type == 1)
	{
		CDlgUtil::CtrlShowHide(this, m_aConstCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSineCtrl, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, FALSE);
		m_wndScaleFactor.SetWindowText(_T("1.0"));
		m_wndScaleFactor.EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else if(m_type == 2)
	{
		CDlgUtil::CtrlShowHide(this, m_aConstCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aSineCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, TRUE);
		m_wndScaleFactor.EnableWindow(TRUE);
	}

	DrawChart();
	m_ParamData.nFuncType = m_type+1;
}

T_ETFC_D& CEtfcItemDlg::GetParamData()
{
	return m_ParamData;
}

void CEtfcItemDlg::SetParamData(T_ETFC_D& data)
{
	m_bModify		= TRUE;
	m_ParamData = data;
	m_csOldName = m_ParamData.strFuncName;
}
void CEtfcItemDlg::OnBtnDyGen()
{
	UpdateData(TRUE);
	T_ETFC_K Key;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	Key = pDoc->m_pAttrCtrl->GetEtfcKey(m_ParamData.strFuncName);
	
	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_AT_FUNC;  // Ambient Temp. Function.
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = Key;
	UchtD.nFuncType = m_type;
	UchtD.dTemper = m_temper.GetEditValue();
	UchtD.dMaxTemper = m_maxtemper.GetEditValue();
	UchtD.dDelay = m_delay.GetEditValue();
	UchtD.dMeanTemper = m_meantemper.GetEditValue();
	CUnitCtrl::m_UCHT_UNIT.dDelay       = CUnitCtrl::m_ETFC_UNIT.dDelayTime;
	CUnitCtrl::m_UCHT_UNIT.dMaxTemper   = CUnitCtrl::m_ETFC_UNIT.dMaxTemp;
	CUnitCtrl::m_UCHT_UNIT.dTemper      = CUnitCtrl::m_ETFC_UNIT.dTemperature;
	CUnitCtrl::m_UCHT_UNIT.dMeanTemper  = CUnitCtrl::m_ETFC_UNIT.dMeanTemp;

	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if (nCount == 0) csName = _LS(IDS_CMD_DYNA_RPT_ATFUNC_NAME);
	else csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_ATFUNC_NAME), nCount + 1);

	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if(dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;
		UchtD.bLogX = m_btnXLog.GetCheck();
		UchtD.bLogY = m_btnYLog.GetCheck();
		if(!pDoc->m_pDataCtrl->AddUcht(UchtD)) ASSERT(0);
	}
}
void CEtfcItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData()) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;

	if (m_bModify)
		bSuccess = pDoc->m_pDataCtrl->ModifyEtfc(m_csOldName, m_ParamData);
	else
		bSuccess = pDoc->m_pDataCtrl->AddEtfc(m_ParamData);

	if (!bSuccess) return;
	CDialogMove::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////////////////////
// Grid
void CEtfcItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_ParamData);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_bGridInit = TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////////////////////
// Chart
void CEtfcItemDlg::InitChart()
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
	pDisplay->SetRect(0.2,0,100,97.0);
	pDisplay->SetFontSize(8);
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pDisplay->GetStyle()->SetUseEmptyGraph(TRUE); // The empty graph will have a telltale in it.
	pDisplay->GetStyle()->SetUseMaxScale(FALSE);	
	pDisplay->GetStyle()->SetAlwaysShowZero(TRUE);
	
	
	// Logarithmic Axis Scaling
	//
//	pDisplay->GetStyle()->SetLogX(m_LogFlagX);
//	pDisplay->GetStyle()->SetLog(m_LogFlagY);

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
	SetAxisLabelText(0,_LS(IDS_WG_CMD__ADDD__Time__day_));
	
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
//	SetAxisLabelText(1,_T("Etfc"));
	

	
	

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

void CEtfcItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CEtfcItemDlg::OnDataChanged()
{
	// Add handler to update graph
	T_ETFC_BASE data;
	int nSize = m_ParamData.aFuncData.GetSize();

	m_Graph.KillData();	
	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.aFuncData[i];
		SetValue(i, 0, data.dTime, data.dValue);
	}
	m_GraphView.Invalidate();
}

void CEtfcItemDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CEtfcItemDlg::DrawChart()
{
	UpdateData();

	if(m_type == 0)
	{
		GraphConstant();
		SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Time__day_));
	}
	else if(m_type == 1)
	{
		GraphCode();
		SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Time__day_));
	}
	else if(m_type == 2)
	{
		GraphUser();
		SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Time__hr_));
	}
}

void CEtfcItemDlg::GraphConstant()
{
	T_ETFC_BASE data;

	m_Graph.KillData();
	for(int i=0; i <= 30; i++)
	{
		data.dTime = i;
		data.dValue = m_temper.GetEditValue();
		SetValue(i, 0, data.dTime, data.dValue);
	}
	m_GraphView.Invalidate();
}

void CEtfcItemDlg::GraphCode()
{
	T_ETFC_BASE data;

	m_Graph.KillData();
	for(int i=0; i <= 72; i++)
	{
		double t = m_maxtemper.GetEditValue();
		double d = m_delay.GetEditValue();
		double to = m_meantemper.GetEditValue();
		double pi = 3.141592654;
		double hour = (double)(1. / 24.*i);
		double CalHour = (double)(hour + (double)(1./24*d));
		data.dTime = hour;
		data.dValue = t * sin(2*pi*CalHour) + to;
		SetValue(i, 0, data.dTime, data.dValue);
	}
	m_GraphView.Invalidate();
}

void CEtfcItemDlg::GraphUser()
{
	OnDataChanged();
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void CEtfcItemDlg::OnEtfcSinRedrawGraph() 
{
	// TODO: Add your control notification handler code here
	DrawChart();
}

void CEtfcItemDlg::OnETFCCHARTXLog() 
{
	// TODO: Add your control notification handler code here
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

void CEtfcItemDlg::OnETFCCHARTYLog() 
{
	// TODO: Add your control notification handler code here
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
