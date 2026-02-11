// CcfcItemDlg.cpp : implementation file
//

#include <math.h>
#include "stdafx.h"
#include "wg_cmd.h"
#include "CcfcItemDlg.h"

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
// CCcfcItemDlg dialog


CCcfcItemDlg::CCcfcItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCcfcItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCcfcItemDlg)
	m_csFuncName = _T("");
	m_type = -1;
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bGridInit = FALSE;

	m_aUserCtrl.Add(IDC_CCFC_TABLE);

	m_aConstCtrl.Add(IDC_CCFC_CONST_FRAME);
	m_aConstCtrl.Add(IDC_CCFC_CONST_COEFFI_TITLE);
	m_aConstCtrl.Add(IDC_CCFC_CONST_COEFFI_EDIT);
	m_aConstCtrl.Add(IDC_CCFC_CONST_COEFFI_UNIT);
}


void CCcfcItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCcfcItemDlg)
	DDX_Control(pDX, IDC_CCFC_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_CCFC_CHART_XLog, m_btnXLog);
	DDX_Control(pDX, IDC_CCFC_SCALE, m_wndScaleFactor);
	DDX_Control(pDX, IDC_CCFC_CONST_COEFFI_UNIT, m_CoefficientUnit);
	DDX_Control(pDX, IDC_CCFC_CONST_COEFFI_EDIT, m_Coefficient);
	DDX_Text(pDX, IDC_CCFC_FUNC_NAME, m_csFuncName);
	DDX_Radio(pDX, IDC_CCFC_TYPE_CONST, m_type);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_CCFC_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		m_ParamData.strFuncName = m_csFuncName;
		m_ParamData.nFuncType = m_type+1;
		m_ParamData.dCoefficient = m_Coefficient.GetEditValue();
		m_wndScaleFactor.GetEditValue(m_ParamData.dScaleFactor);
		Dlg2Data();
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


BEGIN_MESSAGE_MAP(CCcfcItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCcfcItemDlg)
	ON_BN_CLICKED(IDC_CCFC_REDRAW_GRAPH, OnCcfcRedrawGraph)
	ON_BN_CLICKED(IDC_CCFC_TYPE_CONST, OnSelChangeType)
	ON_BN_CLICKED(IDC_CCFC_TYPE_USER, OnSelChangeType)
	ON_BN_CLICKED(IDC_CCFC_CHART_XLog, OnCCFCCHARTXLog)
	ON_BN_CLICKED(IDC_CCFC_CHART_YLog, OnCCFCCHARTYLog)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCcfcItemDlg message handlers

void CCcfcItemDlg::Data2Dlg()
{
	CString str;
	m_csFuncName = m_ParamData.strFuncName;
	m_type = m_ParamData.nFuncType-1;
	UpdateData(FALSE);

	str.Format(_T("%g"), m_ParamData.dScaleFactor);
	m_wndScaleFactor.SetWindowText(str);
	m_Coefficient.SetEditUnit(m_ParamData.dCoefficient);
}

void CCcfcItemDlg::Dlg2Data()
{
}

BOOL CCcfcItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	AlignControls();

	// init unit /////////////////////////////////////////////////
	m_CoefficientUnit.SetUnitType(CUnitCtrl::m_CCFC_UNIT.dCoefficient);
	//////////////////////////////////////////////////////////////

	if(!m_bModify)
	{
		m_ParamData.Initialize();
		m_ParamData.nFuncType = 1;
	}

	Data2Dlg();

	InitGrid();
//  m_LogFlagX = FALSE;
//  m_LogFlagY = FALSE;
	InitChart();
	m_wndGrid.MakeItemEx();

	OnSelChangeType();

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));
	CString str;
	str.Format(_T("%g"), m_ParamData.dScaleFactor);
	m_wndScaleFactor.SetRange(1, SHRT_MAX);
	m_wndScaleFactor.SetWindowText(str);

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCcfcItemDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	// 1st move 
	GetDlgItem(IDC_CCFC_TABLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_CCFC_CONST_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aConstCtrl, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

T_CCFC_D& CCcfcItemDlg::GetParamData()
{
	return m_ParamData;
}

void CCcfcItemDlg::SetParamData(T_CCFC_D& data)
{
	m_bModify		= TRUE;
	m_ParamData = data;
	m_csOldName = m_ParamData.strFuncName;
}
void CCcfcItemDlg::OnBtnDyGen()
{
	UpdateData(TRUE);

	T_CCFC_K Key;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	Key = pDoc->m_pAttrCtrl->GetCcfcKey(m_ParamData.strFuncName);
	
	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_CC_FUNC; // Convection Coefficient Func.
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = Key;
	
	UchtD.nFuncType = m_type;
	UchtD.dTemper = m_Coefficient.GetEditValue();
	CUnitCtrl::m_UCHT_UNIT.dTemper = CUnitCtrl::m_CCFC_UNIT.dCoefficient;

	CString csName; csName.Empty();
	int nCount = pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	csName = _LS(IDS_CMD_DYNACHART_CCFC);//_T("Convection Coefficient Function");
	if(nCount==0) csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNACHART_CCFC), nCount+1);
	
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
void CCcfcItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData()) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;

	if (m_bModify)
		bSuccess = pDoc->m_pDataCtrl->ModifyCcfc(m_csOldName, m_ParamData);
	else
		bSuccess = pDoc->m_pDataCtrl->AddCcfc(m_ParamData);

	if (!bSuccess) return;
	CDialogMove::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////////////////////
// Grid
void CCcfcItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_ParamData);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_bGridInit = TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////////////////////
// Chart
void CCcfcItemDlg::InitChart()
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
	pDisplay->SetFaceName(_LS(IDS_GRAPH_FONT_ARIAL));
	pDisplay->SetRect(0.2,0,100,97.0);
	pDisplay->SetFontSize(8);
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
//	SetAxisLabelText(0,_T("Time (sec)"));
	
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
//	SetAxisLabelText(1,_T("Hmtp"));
	

	
	

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

void CCcfcItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CCcfcItemDlg::OnDataChanged()
{
	// Add handler to update graph
	T_CCFC_BASE data;
	int nSize = m_ParamData.aFuncData.GetSize();

	m_Graph.KillData();	
	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.aFuncData[i];
		SetValue(i, 0, data.dTime, data.dValue);
	}
	m_GraphView.Invalidate();
}

void CCcfcItemDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CCcfcItemDlg::DrawChart()
{
	UpdateData();

	if(m_type == 0)
	{
		GraphConstant();
		SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Time__day_));
	}
	else if(m_type == 1)
	{
		GraphUser();
		SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Time__hr_));
	}
}

void CCcfcItemDlg::GraphConstant()
{
	T_CCFC_BASE data;

	m_Graph.KillData();
	for(int i=0; i <= 30; i++)
	{
		data.dTime = i;
		data.dValue = m_Coefficient.GetEditValue();
		SetValue(i, 0, data.dTime, data.dValue);
	}
	m_GraphView.Invalidate();
}

void CCcfcItemDlg::GraphUser()
{
	OnDataChanged();
}

void CCcfcItemDlg::OnCcfcRedrawGraph() 
{
	// TODO: Add your control notification handler code here
	DrawChart();
}

void CCcfcItemDlg::OnSelChangeType() 
{
	// TODO: Add your control notification handler code here
	int nPrevType = m_type;
	UpdateData();

	if (nPrevType == 1 && m_type != 1)  // user
	{
		if (!m_wndGrid.OnTerminate(D_TB_SAVE_ALWAYS))
			m_wndGrid.CancelEdit();
	}

	if(m_type == 0) // const
	{
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aConstCtrl, TRUE);
		m_wndScaleFactor.SetWindowText(_T("1.0"));
		m_wndScaleFactor.EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else if(m_type == 1) // user
	{
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aConstCtrl, FALSE);
		m_wndScaleFactor.EnableWindow(TRUE);
	}

	DrawChart();
	m_ParamData.nFuncType = m_type+1;
}

void CCcfcItemDlg::OnCCFCCHARTXLog() 
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

void CCcfcItemDlg::OnCCFCCHARTYLog() 
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
