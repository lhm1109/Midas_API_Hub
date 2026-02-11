// HmtpItemDlg.cpp : implementation file
//

#include <math.h>
#include "stdafx.h"
#include "wg_cmd.h"
#include "HmtpItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHmtpItemDlg dialog


CHmtpItemDlg::CHmtpItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CHmtpItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHmtpItemDlg)
	m_type = -1;
	m_csTypeName = _T("");
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bGridInit = FALSE;

	m_aCodeCtrl.Add(IDC_HMTP_FUNC_FRAME);
	m_aCodeCtrl.Add(IDC_HMTP_STATIC1);
	m_aCodeCtrl.Add(IDC_HMTP_CODE_COMBO);

	m_aKSDCodeCtrl.Add(IDC_HMTP_FUNC_KS_FRAME);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_STATIC1);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_STATIC2);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_STATIC3);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_STATIC4);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_STATIC5);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_STATIC6);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_STATIC7);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_S91EDIT);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_S91UNIT);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_FACTOR_AEDIT);
	m_aKSDCodeCtrl.Add(IDC_HMTP_KS_FACTOR_BEDIT);

	m_aACICodeCtrl.Add(IDC_HMTP_FUNC_ACI_FRAME);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_STATIC1);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_STATIC2);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_STATIC3);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_STATIC4);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_STATIC5);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_STATIC6);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_STATIC7);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_S28EDIT);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_S28UNIT);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_FACTOR_AEDIT);
	m_aACICodeCtrl.Add(IDC_HMTP_ACI_FACTOR_BEDIT);

	m_aCEBCodeCtrl.Add(IDC_HMTP_FUNC_CEB_FRAME);
	m_aCEBCodeCtrl.Add(IDC_HMTP_CEB_STATIC1);
	m_aCEBCodeCtrl.Add(IDC_HMTP_CEB_STATIC2);
	m_aCEBCodeCtrl.Add(IDC_HMTP_CEB_STATIC3);
	m_aCEBCodeCtrl.Add(IDC_HMTP_CEB_STATIC4);
	m_aCEBCodeCtrl.Add(IDC_HMTP_CEB_S28EDIT);
	m_aCEBCodeCtrl.Add(IDC_HMTP_CEB_S28UNIT);
	m_aCEBCodeCtrl.Add(IDC_HMTP_CEB_COMBO);

	m_aOHZCodeCtrl.Add(IDC_HMTP_FUNC_OHZ_FRAME);
	m_aOHZCodeCtrl.Add(IDC_HMTP_OHZ_STATIC1);
	m_aOHZCodeCtrl.Add(IDC_HMTP_OHZ_STATIC2);
	m_aOHZCodeCtrl.Add(IDC_HMTP_OHZ_STATIC3);
	m_aOHZCodeCtrl.Add(IDC_HMTP_OHZ_STATIC4);
	m_aOHZCodeCtrl.Add(IDC_HMTP_OHZ_STATIC5);
	m_aOHZCodeCtrl.Add(IDC_HMTP_OHZ_S28EDIT);
	m_aOHZCodeCtrl.Add(IDC_HMTP_OHZ_S28UNIT);
	m_aOHZCodeCtrl.Add(IDC_HMTP_OHZ_COMBO);

	m_aUserCtrl.Add(IDC_HMTP_TABLE);
}


void CHmtpItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHmtpItemDlg)
	DDX_Control(pDX, IDC_ETC_HMTP_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_ETC_HMTP_CHART_XLog, m_btnXLog);
	DDX_Control(pDX, IDC_HMTP_OHZ_S28UNIT, m_dOHZCompStrengthUnit);
	DDX_Control(pDX, IDC_HMTP_OHZ_S28EDIT, m_dOHZCompStrength);
	DDX_Control(pDX, IDC_HMTP_OHZ_COMBO, m_OHZCementType);
	DDX_Control(pDX, IDC_HMTP_KS_S91UNIT, m_dKSDCompStrengthUnit);
	DDX_Control(pDX, IDC_HMTP_KS_S91EDIT, m_dKSDCompStrength);
	DDX_Control(pDX, IDC_HMTP_KS_FACTOR_BEDIT, m_dKSDFactorB);
	DDX_Control(pDX, IDC_HMTP_KS_FACTOR_AEDIT, m_dKSDFactorA);
	DDX_Control(pDX, IDC_HMTP_CEB_S28UNIT, m_dCEBCompStrengthUnit);
	DDX_Control(pDX, IDC_HMTP_CEB_S28EDIT, m_dCEBCompStrength);
	DDX_Control(pDX, IDC_HMTP_CEB_COMBO, m_CEBCementType);
	DDX_Control(pDX, IDC_HMTP_ACI_S28UNIT, m_dACICompStrengthUnit);
	DDX_Control(pDX, IDC_HMTP_ACI_S28EDIT, m_dACICompStrength);
	DDX_Control(pDX, IDC_HMTP_ACI_FACTOR_BEDIT, m_dACIFactorB);
	DDX_Control(pDX, IDC_HMTP_ACI_FACTOR_AEDIT, m_dACIFactorA);
	DDX_Control(pDX, IDC_HMTP_SHEAT_UNIT, m_SpecHeatUnit);
	DDX_Control(pDX, IDC_HMTP_SHEAT, m_SpecHeatEdit);
	DDX_Control(pDX, IDC_HMTP_HEATCOND_UNIT, m_HeatCondUnit);
	DDX_Control(pDX, IDC_HMTP_HEATCOND, m_HeatCondEdit);
	DDX_Control(pDX, IDC_HMTP_SCALE, m_wndScaleFactor);
	DDX_Control(pDX, IDC_HMTP_CODE_COMBO, m_CobxCode);
	DDX_Radio(pDX, IDC_HMTP_TYPE_CONST, m_type);
	DDX_Text(pDX, IDC_HMTP_FUNC_NAME, m_csTypeName);
	DDX_Control(pDX, IDC_HMTP_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		m_ParamData.strMatlName = m_csTypeName;
		m_ParamData.nMatlType = m_type+1;
		if (!m_wndScaleFactor.GetEditValue(m_ParamData.dScaleFactor))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_));
			pDX->Fail();
		}
		m_ParamData.dSpecificHeat = m_SpecHeatEdit.GetEditValue();
		m_ParamData.dHeatConduction = m_HeatCondEdit.GetEditValue();

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

BEGIN_MESSAGE_MAP(CHmtpItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CHmtpItemDlg)
	ON_BN_CLICKED(IDC_HMTP_TYPE_CODE, OnHmtpSelChangeType)
	ON_CBN_SELCHANGE(IDC_HMTP_CODE_COMBO, OnSelchangeHmtpCodeCombo)
	ON_BN_CLICKED(IDC_HMTP_SIN_REDRAW_GRAPH, OnHmtpSinRedrawGraph)
	ON_BN_CLICKED(IDC_HMTP_TYPE_USER, OnHmtpSelChangeType)
	ON_BN_CLICKED(IDC_HMTP_TYPE_CONST, OnHmtpSelChangeType)
	ON_BN_CLICKED(IDC_ETC_HMTP_CHART_XLog, OnETCHMTPCHARTXLog)
	ON_BN_CLICKED(IDC_ETC_HMTP_CHART_YLog, OnETCHMTPCHARTYLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHmtpItemDlg message handlers

void CHmtpItemDlg::Data2Dlg()
{
	CString str;
	m_csTypeName = m_ParamData.strMatlName;
	m_type = m_ParamData.nMatlType-1;
	str.Format(_T("%g"), m_ParamData.dScaleFactor);
	m_wndScaleFactor.SetWindowText(str);
	UpdateData(FALSE);

	m_SpecHeatEdit.SetEditUnit(m_ParamData.dSpecificHeat);
	m_HeatCondEdit.SetEditUnit(m_ParamData.dHeatConduction);

	int nNum = SelChangeCodeCobx(m_ParamData.strCode);

	switch(nNum)
	{
		case 0:
				{
					m_dKSDCompStrength.SetEditUnit(m_ParamData.dCompStrength);
					m_dKSDFactorA.SetEditUnit(m_ParamData.dFactorA);
					m_dKSDFactorB.SetEditUnit(m_ParamData.dFactorB);
				}
				break;
		case 1:
				{
					m_dACICompStrength.SetEditUnit(m_ParamData.dCompStrength);
					m_dACIFactorA.SetEditUnit(m_ParamData.dFactorA);
					m_dACIFactorB.SetEditUnit(m_ParamData.dFactorB);
				}
				break;
		case 2:
				{
					m_dCEBCompStrength.SetEditUnit(m_ParamData.dCompStrength);
					m_CEBCementType.SetCurSel(m_ParamData.nCementType-1);
				}
				break;
		case 3:
				{
					m_dOHZCompStrength.SetEditUnit(m_ParamData.dCompStrength);
					m_OHZCementType.SetCurSel(m_ParamData.nCementType-1);
				}
				break;
	}
}

void CHmtpItemDlg::Dlg2Data()
{
	m_CobxCode.GetWindowText(m_ParamData.strCode);
	int nNum = m_CobxCode.GetCurSel();

	switch(nNum)
	{
		case 0:
				{
					m_ParamData.dCompStrength = m_dKSDCompStrength.GetEditValue();
					m_ParamData.dFactorA = m_dKSDFactorA.GetEditValue();
					m_ParamData.dFactorB = m_dKSDFactorB.GetEditValue();
				}
				break;
		case 1:
				{
					m_ParamData.dCompStrength = m_dACICompStrength.GetEditValue();
					m_ParamData.dFactorA = m_dACIFactorA.GetEditValue();
					m_ParamData.dFactorB = m_dACIFactorB.GetEditValue();
				}
				break;
		case 2:
				{
					m_ParamData.dCompStrength = m_dCEBCompStrength.GetEditValue();
					m_ParamData.nCementType = m_CEBCementType.GetCurSel() + 1;
				}
				break;
		case 3:
				{
					m_ParamData.dCompStrength = m_dOHZCompStrength.GetEditValue();
					m_ParamData.nCementType = m_OHZCementType.GetCurSel() + 1;
				}
				break;
	}
}

BOOL CHmtpItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	AlignControls();

	// init unit /////////////////////////////////////////////////
	m_SpecHeatUnit.SetUnitType(CUnitCtrl::m_HMTP_UNIT.dSpecificHeat);
	m_HeatCondUnit.SetUnitType(CUnitCtrl::m_HMTP_UNIT.dHeatConduction);
	m_dOHZCompStrengthUnit.SetUnitType(CUnitCtrl::m_HMTP_UNIT.dCompStrength);
	m_dKSDCompStrengthUnit.SetUnitType(CUnitCtrl::m_HMTP_UNIT.dCompStrength);
	m_dCEBCompStrengthUnit.SetUnitType(CUnitCtrl::m_HMTP_UNIT.dCompStrength);
	m_dACICompStrengthUnit.SetUnitType(CUnitCtrl::m_HMTP_UNIT.dCompStrength);
	//////////////////////////////////////////////////////////////

	// !! 2001.9.12 Park Jong Bae
	m_CobxCode.AddString(_LS(IDS_WG_CMD__ADDD__Korea_Standard));
	m_CobxCode.AddString(_T("ACI"));
	m_CobxCode.AddString(_T("CEB-FIP(1990)"));
	m_CobxCode.AddString(_T("Ohzagi"));

	if(!m_bModify)
	{
		m_ParamData.Initialize();
		m_ParamData.nMatlType = 3;
		m_ParamData.strCode = _LS(IDS_WG_CMD__ADDD__Korea_Standard);
		m_CEBCementType.SetCurSel(0);
		m_OHZCementType.SetCurSel(0);
	}

	Data2Dlg();

	InitGrid();
//  m_LogFlagX = FALSE;
//  m_LogFlagY = FALSE;
	InitChart();
	m_wndGrid.MakeItemEx();


	OnHmtpSelChangeType();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHmtpItemDlg::AlignControls()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	// 1st move 
	GetDlgItem(IDC_HMTP_TABLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_HMTP_FUNC_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCodeCtrl, nDistY);

	// 2nd move
	GetDlgItem(IDC_HMTP_FUNC_FRAME)->GetWindowRect(rRef);
	GetDlgItem(IDC_HMTP_FUNC_KS_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aKSDCodeCtrl, nDistY);
	GetDlgItem(IDC_HMTP_FUNC_ACI_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aACICodeCtrl, nDistY);
	GetDlgItem(IDC_HMTP_FUNC_CEB_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCEBCodeCtrl, nDistY);
	GetDlgItem(IDC_HMTP_FUNC_OHZ_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aOHZCodeCtrl, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(16);

	MoveWindow(r);
}

void CHmtpItemDlg::OnHmtpSelChangeType() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_type == 0)
	{
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCodeCtrl, FALSE);
		ShowHideCode(FALSE);
		m_wndScaleFactor.SetWindowText(_T("1.0"));
		m_wndScaleFactor.EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else if(m_type == 1)
	{
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCodeCtrl, TRUE);
		ShowHideCode(TRUE);
		m_wndScaleFactor.SetWindowText(_T("1.0"));
		m_wndScaleFactor.EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else if(m_type == 2)
	{
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCodeCtrl, FALSE);
		ShowHideCode(FALSE);
		m_wndScaleFactor.EnableWindow(TRUE);
	}

	DrawChart();
	m_ParamData.nMatlType = m_type+1;
}

void CHmtpItemDlg::OnSelchangeHmtpCodeCombo() 
{
	// TODO: Add your control notification handler code here
	ShowHideCode(TRUE);
	DrawChart();
}

void CHmtpItemDlg::ShowHideCode(BOOL flag)
{
	int nNum;

	if(flag)
		nNum = m_CobxCode.GetCurSel();
	else
		nNum = 4;

	bool bKSD, bACI, bCEB, bOHZ;
	bKSD = bACI = bCEB = bOHZ = false;

	switch(nNum)
	{
		case 0:
			bKSD = true;	bACI = bCEB = bOHZ = false;
			break;
		case 1:
			bACI = true;	bKSD = bCEB = bOHZ = false;
			break;
		case 2:
			bCEB = true;	bKSD = bACI = bOHZ = false;
			break;
		case 3:
			bOHZ = true;	bKSD = bACI = bCEB = false;
			break;
	}

	CDlgUtil::CtrlShowHide(this, m_aKSDCodeCtrl, bKSD);
	CDlgUtil::CtrlShowHide(this, m_aACICodeCtrl, bACI);
	CDlgUtil::CtrlShowHide(this, m_aCEBCodeCtrl, bCEB);
	CDlgUtil::CtrlShowHide(this, m_aOHZCodeCtrl, bOHZ);
}

int CHmtpItemDlg::SelChangeCodeCobx(CString str)
{
	int nNum = m_CobxCode.SelectString(-1, str);
	m_CobxCode.SetCurSel(nNum);

	return nNum;
}

T_HMTP_D& CHmtpItemDlg::GetParamData()
{
	return m_ParamData;
}

void CHmtpItemDlg::SetParamData(T_HMTP_D& data)
{
	m_bModify		= TRUE;
	m_ParamData = data;
	m_csOldName = m_ParamData.strMatlName;
}

void CHmtpItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData()) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;

	if (m_bModify)
		bSuccess = pDoc->m_pDataCtrl->ModifyHmtp(m_csOldName, m_ParamData);
	else
		bSuccess = pDoc->m_pDataCtrl->AddHmtp(m_ParamData);

	if (!bSuccess) return;
	CDialogMove::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////////////////////
// Grid
void CHmtpItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_ParamData);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_bGridInit = TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////////////////////
// Chart
void CHmtpItemDlg::InitChart()
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
	SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__Time__day_));
	
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

void CHmtpItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CHmtpItemDlg::OnDataChanged()
{
	// Add handler to update graph
	T_HMTP_BASE data;
	int nSize = m_ParamData.aFuncData.GetSize();

	m_Graph.KillData();	
	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.aFuncData[i];
		SetValue(i, 0, data.dTime, data.dValue);
	}
	m_GraphView.Invalidate();
}

void CHmtpItemDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CHmtpItemDlg::DrawChart()
{
	UpdateData();

	if(m_type == 0)
	{
		GraphConstant();
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

void CHmtpItemDlg::GraphConstant()
{
//	T_HMTP_BASE data;

	m_Graph.KillData();
/*
	for(int i=0; i < 30; i++)
	{
		data.dTime = i;
		data.dValue = 0;
		SetValue(i, 0, data.dTime, data.dValue);
	}
*/
	m_GraphView.Invalidate();
}

void CHmtpItemDlg::GraphCode()
{
	UpdateData();
	T_HMTP_BASE data;
	CArray<T_HMTP_BASE, T_HMTP_BASE> acode;

	int ncode = m_CobxCode.GetCurSel();
	if(ncode == 0) 	// korea standard
	{
		for(int i=0; i <= 30; i++)
		{
			double S91 = m_dKSDCompStrength.GetEditValue();
			double a = m_dKSDFactorA.GetEditValue();
			double b = m_dKSDFactorB.GetEditValue();
			if(a == 0 && b == 0) break;
			data.dTime = i;
			data.dValue = S91 * (i / (a + b*i));
			acode.Add(data);
		}
	}
	else if(ncode == 1)  // ACI
	{
		for(int i=0; i <= 30; i++)
		{
			double S28 = m_dACICompStrength.GetEditValue();
			double a = m_dACIFactorA.GetEditValue();
			double b = m_dACIFactorB.GetEditValue();
			if(a == 0 && b == 0) break;
			data.dTime = i;
			data.dValue = S28 * (i / (a + b*i));
			acode.Add(data);
		}
	}
	else if(ncode == 2)  // CEB-FIP
	{
		for(int i=0; i <= 30; i++)
		{
			double time;
			double a[3] = { 0.2, 0.25, 0.38 };
			double S28 = m_dCEBCompStrength.GetEditValue();
			int nCement = m_CEBCementType.GetCurSel();
			if(nCement < 0 || nCement > 2) break;
			if(i == 0) time = 0.1; // 0으로 나누면 에러나쥐...
			else time = i;
			data.dTime = time;
			data.dValue = S28 * exp(a[nCement] * (1 - pow((28/time), 0.5)));
			acode.Add(data);
		}
	}
	else if(ncode == 3)  // Ohzagi
	{
		for(int i=0; i <= 1000; i++)
		{
			double S28 = m_dOHZCompStrength.GetEditValue();
			int nCement = m_OHZCementType.GetCurSel();
			if(nCement < 0 || nCement > 3) break;
			data.dTime = i;
			data.dValue = S28 * OhzagiY(nCement, i);
			acode.Add(data);
		}
	}

	m_Graph.KillData();
	int nNum = acode.GetSize();
	for(int i=0; i < nNum; i++)
	{
		SetValue(i, 0, acode[i].dTime, acode[i].dValue);
	}
	m_GraphView.Invalidate();
}

void CHmtpItemDlg::GraphUser()
{
	OnDataChanged();
}

double CHmtpItemDlg::OhzagiY(int nCement, double m)
{
	double a=0.0, b=0.0, c=0.0;
	double retY=0.0, x=0.0;

	if(nCement == 0)
	{
		if(m <= 30) a = 0.024,  b = -0.028, c = 0.01;
		else				a = -0.005, b = 0.162,  c = -0.286;
		if(m <= 6.626) return 1e-10;
	}
	else if(nCement == 1)
	{
		if(m <= 30) a = 0.012, b = -0.028, c = 0.02;
		else				a = 0.000, b = 0.112,  c = -0.347;
		if(m <= 8.488) return 1e-10;
	}
	else if(nCement == 2)
	{
		if(m <= 30) a = 0.01,  b = -0.027, c = 0.02;
		else				a = 0.006, b = 0.014,  c = -0.077;
		if(m <= 9.477) return 1e-10;
	}
	else if(nCement == 3)
	{
		if(m <= 30) a = 0.012, b = -0.034, c = 0.028;
		else				a = 0.002, b = 0.074,  c = -0.251;
		if(m <= 9.456) return 1e-10;
	}

	x = 2.389 * log(m/3.5) - 1.0;
	retY = a*pow(x, 2) + b*x + c;

	return retY;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void CHmtpItemDlg::OnHmtpSinRedrawGraph() 
{
	// TODO: Add your control notification handler code here
	DrawChart();
}

void CHmtpItemDlg::OnETCHMTPCHARTXLog() 
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

void CHmtpItemDlg::OnETCHMTPCHARTYLog() 
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
