// HsfcItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "HsfcItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHsfcItemDlg dialog


CHsfcItemDlg::CHsfcItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CHsfcItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHsfcItemDlg)
	m_type = -1;
	m_csFuncName = _T("");
	m_wndDataType = -1;
	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bGridInit = FALSE;

	m_aConstCtrl.Add(IDC_HSFC_CONST_FRAME);
	m_aConstCtrl.Add(IDC_HSFC_CONST_TEMPER_TITLE);
	m_aConstCtrl.Add(IDC_HSFC_CONST_TEMPER_EDIT);
	m_aConstCtrl.Add(IDC_HSFC_CONST_TEMPER_UNIT);

	m_aUserCtrl.Add(IDC_HSFC_TABLE);

	m_aCodeCtrl.Add(IDC_HSFC_CODEUSA_FRAME);
	m_aCodeCtrl.Add(IDC_HSFC_STATIC2);
	m_aCodeCtrl.Add(IDC_HSFC_STATIC3);
	m_aCodeCtrl.Add(IDC_HSFC_STATIC4);
	m_aCodeCtrl.Add(IDC_HSFC_MAX_EDIT);
	m_aCodeCtrl.Add(IDC_HSFC_MAX_UNIT);
	m_aCodeCtrl.Add(IDC_HSFC_REACT_EDIT);//
	//m_aCodeCtrl.Add(IDC_STATIC_K);
	//m_aCodeCtrl.Add(IDC_STATIC_A);
	m_aCodeCtrl.Add(IDC_CHECK_USE_CONC);
	m_aCodeCtrl.Add(IDC_HSFC_USE_CONC_FRAME);
	m_aCodeCtrl.Add(IDC_STATIC_CEMENT);
	m_aCodeCtrl.Add(IDC_STATIC_TEMPERATURE);
	m_aCodeCtrl.Add(IDC_COB_CEMENT_TYPE);
	m_aCodeCtrl.Add(IDC_COB_TEMPERATURE_TYPE);
	m_aCodeCtrl.Add(IDC_STATIC_CEMENT_CONTENT);
	m_aCodeCtrl.Add(IDC_EDIT_CEMENT_CONTENT);
	m_aCodeCtrl.Add(IDC_STATIC_KG);
	
	m_aCtrlDataType.Add(IDC_HSFC_DATA_TYPE_RDO);
	m_aCtrlDataType.Add(IDC_HSFC_DATA_TYPE_RDO2);
}


void CHsfcItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHsfcItemDlg)
	DDX_Control(pDX, IDC_EDIT_CEMENT_CONTENT, m_editCementContent);
	DDX_Control(pDX, IDC_CHECK_USE_CONC, m_btnUseConc);
	DDX_Control(pDX, IDC_COB_TEMPERATURE_TYPE, m_ctrlCobTemperature);
	DDX_Control(pDX, IDC_COB_CEMENT_TYPE, m_ctrlCobCement);
	DDX_Control(pDX, IDC_ETC_HSFC_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_ETC_HSFC_CHART_XLog, m_btnXLog);
	DDX_Control(pDX, IDC_HSFC_CONST_TEMPER_UNIT, m_temperunit);
	DDX_Control(pDX, IDC_HSFC_SCALE, m_wndScaleFactor);
	DDX_Control(pDX, IDC_HSFC_REACT_EDIT, m_reactive);
	DDX_Control(pDX, IDC_HSFC_MAX_UNIT, m_maxtemperUnit);
	DDX_Control(pDX, IDC_HSFC_MAX_EDIT, m_maxtemper);
	DDX_Control(pDX, IDC_HSFC_CONST_TEMPER_EDIT, m_temper);
	DDX_Radio(pDX, IDC_HSFC_TYPE_CONST, m_type);
	DDX_Text(pDX, IDC_HSFC_FUNC_NAME, m_csFuncName);
	DDX_Radio(pDX, IDC_HSFC_DATA_TYPE_RDO, m_wndDataType);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_HSFC_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP
	if (pDX->m_bSaveAndValidate)
	{
		m_ParamData.strFuncName = m_csFuncName;
		m_ParamData.nFuncType = m_type+1;
		m_ParamData.nDataType = m_wndDataType;
		if (!m_wndScaleFactor.GetEditValue(m_ParamData.dScaleFactor))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_scale_factor_));
			pDX->Fail();
		}
		CString str;
		m_ParamData.dTemperature = m_temper.GetEditValue();
		m_ParamData.dK = m_maxtemper.GetEditValue();
		m_ParamData.dAlpha = m_reactive.GetEditValue();
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


BEGIN_MESSAGE_MAP(CHsfcItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CHsfcItemDlg)
	ON_BN_CLICKED(IDC_HSFC_TYPE_CODE, OnHsfcSelChangeType)
	ON_BN_CLICKED(IDC_HSFC_SIN_REDRAW_GRAPH, OnHsfcSinRedrawGraph)
	ON_BN_CLICKED(IDC_ETC_HSFC_CHART_XLog, OnETCHSFCCHARTXLog)
	ON_BN_CLICKED(IDC_ETC_HSFC_CHART_YLog, OnETCHSFCCHARTYLog)
	ON_BN_CLICKED(IDC_CHECK_USE_CONC, OnCheckUseConc)
	ON_CBN_SELCHANGE(IDC_COB_CEMENT_TYPE, OnSelchangeCobCementType)
	ON_CBN_SELCHANGE(IDC_COB_TEMPERATURE_TYPE, OnSelchangeCobTemperatureType)
	ON_EN_CHANGE(IDC_EDIT_CEMENT_CONTENT, OnChangeEditCementContent)
	ON_EN_UPDATE(IDC_EDIT_CEMENT_CONTENT, OnUpdateEditCementContent)
	ON_CBN_SETFOCUS(IDC_COB_CEMENT_TYPE, OnSetfocusCobCementType)
	ON_BN_CLICKED(IDC_HSFC_TYPE_CONST, OnHsfcSelChangeType)
	ON_BN_CLICKED(IDC_HSFC_TYPE_USER, OnHsfcSelChangeType)
	ON_BN_CLICKED(IDC_HSFC_DATA_TYPE_RDO, OnHsfcDataTypeRdo)
	ON_BN_CLICKED(IDC_HSFC_DATA_TYPE_RDO2, OnHsfcDataTypeRdo)
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHsfcItemDlg message handlers

BOOL CHsfcItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	AlignControls();

	InitComboBox();

	// init unit /////////////////////////////////////////////////
	m_maxtemperUnit.SetUnitType(CUnitCtrl::m_HSFC_UNIT.dK);
	m_temperunit.SetUnitType(CUnitCtrl::m_HSFC_UNIT.dTemperature);
	//////////////////////////////////////////////////////////////

	if(!m_bModify)
	{
		m_ParamData.Initialize();
		m_ParamData.nFuncType = 3;
	}

	CString str;
	m_csFuncName = m_ParamData.strFuncName;
	m_type = m_ParamData.nFuncType-1;
	m_wndDataType = m_ParamData.nDataType;
	str.Format(_T("%g"), m_ParamData.dScaleFactor);
	m_wndScaleFactor.SetRange(1, SHRT_MAX);
	m_wndScaleFactor.SetWindowText(str);
	m_temper.SetEditUnit(m_ParamData.dTemperature);
	m_maxtemper.SetEditUnit(m_ParamData.dK);
	m_reactive.SetEditUnit(m_ParamData.dAlpha);

	/////////////  6.4.0 추가 코드 by kn.s ///////////////////////////////////////////////
	m_editCementContent.SetEditUnit(m_ParamData.dCementContent); // 추가
	m_btnUseConc.SetCheck(m_ParamData.bUseConcData); //
	
	int nCount = m_ctrlCobTemperature.GetCount();
	int i = 0;
	for ( i=0; i<nCount; i++)
	{
		if (m_ParamData.nTemperature == m_ctrlCobTemperature.GetItemData(i)) break;
	}
	ASSERT(i != nCount);
	m_ctrlCobTemperature.SetCurSel(i);

	int nCount2 = m_ctrlCobCement.GetCount();
	int j = 0;
	for (j=0; j<nCount2; j++)
	{
		if (m_ParamData.nCementType == m_ctrlCobCement.GetItemData(j)) break;
	}
	ASSERT(j != nCount2);
	m_ctrlCobCement.SetCurSel(j);
	//////////////////////////////////////////////////////////////////////////////

	//////ComboBox Dynamic Change///////////////
	CString str1; CSize sz; int dx = 0;
	CDC*  pDC = GetDC();

	for(int h=0; h<m_ctrlCobCement.GetCount(); h++)
	{
		m_ctrlCobCement.GetLBText(h,str1);
		sz = pDC->GetTextExtent(str1);

		if(sz.cx > dx) dx = sz.cx;
	}
	m_ctrlCobCement.ReleaseDC(pDC);
	if (dx > m_ctrlCobCement.GetDroppedWidth()) m_ctrlCobCement.SetDroppedWidth(dx);
	//m_ctrlCobCement.SetHorizontalExtent(dx);
	//m_ctrlCobCement.ReleaseDC(pDC);
	////////////////////////////////////////////

	InitGrid();
//  m_LogFlagX = FALSE;
//  m_LogFlagY = FALSE;
	InitChart();
	m_wndGrid.MakeItemEx();

	UpdateData(FALSE);
	OnCheckUseConc();
	OnHsfcSelChangeType();

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영

	return TRUE;  
	              
}

void CHsfcItemDlg::InitComboBox()
{
	CStringArray aName;
	CArray<UINT, UINT> aData;
#define M_NAME_DATA_ADD(NAME, DATA)  aName.Add(NAME); aData.Add(DATA)
	M_NAME_DATA_ADD(_LS(IDS_CMD_HSFC_normal_portland),  0);
	M_NAME_DATA_ADD(_LS(IDS_CMD_HSFC_moderate_heat_portland), 1);
	M_NAME_DATA_ADD(_LS(IDS_CMD_HSFC_high_early_strength_portland), 2);
	M_NAME_DATA_ADD(_LS(IDS_CMD_HSFC_blast_furnance_slay_B_40), 3);
	M_NAME_DATA_ADD(_LS(IDS_CMD_HSFC_fly_ash_B_20), 4);
#undef M_NAME_DATA_ADD
	
	int ix;
	m_ctrlCobCement.ResetContent();
	for (int i=0; i<aName.GetSize(); i++)
	{
		ix = m_ctrlCobCement.AddString(aName[i]);
		m_ctrlCobCement.SetItemData(ix, aData[i]);
	}
	m_ctrlCobCement.SetCurSel(0);

	CStringArray tName;
	CArray<UINT,UINT> tData;   
#define M_NAME_DATA_ADD(NAME, DATA)  tName.Add(NAME); tData.Add(DATA)
	M_NAME_DATA_ADD(_T("10"),  0);
	M_NAME_DATA_ADD(_T("20"), 1);
	M_NAME_DATA_ADD(_T("30"), 2);
#undef M_NAME_DATA_ADD

	int jx;
	m_ctrlCobTemperature.ResetContent();
	for(int j=0; j<3; j++)
	{
		jx = m_ctrlCobTemperature.AddString(tName[j]);
		m_ctrlCobTemperature.SetItemData(jx, tData[j]);
	}
}

void CHsfcItemDlg::AlignControls()
{
	CRect rRef;
	CRect rToMoveConst, rToMoveCode, rToMoveJAP;
	int		nDistYConst, nDistYCode;


	// 1st Move
	GetDlgItem(IDC_HSFC_TABLE)->GetWindowRect(rRef);
	GetDlgItem(IDC_HSFC_CONST_FRAME)->GetWindowRect(rToMoveConst);
	GetDlgItem(IDC_HSFC_CODEUSA_FRAME)->GetWindowRect(rToMoveCode);

	nDistYConst = rRef.top - rToMoveConst.top;
	nDistYCode  = rRef.top - rToMoveCode.top;
	CDlgUtil::CtrlMoveDistY(this, m_aConstCtrl, nDistYConst);
	CDlgUtil::CtrlMoveDistY(this, m_aCodeCtrl,  nDistYCode);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);

	MoveWindow(r);
}

void CHsfcItemDlg::OnHsfcSelChangeType() 
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
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCodeCtrl, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDataType, FALSE);
		m_wndScaleFactor.SetWindowText(_T("1.0"));
		m_wndScaleFactor.EnableWindow(FALSE);
		UpdateData(FALSE);
	}else if(m_type == 1)
	{
		CDlgUtil::CtrlShowHide(this, m_aConstCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCodeCtrl, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDataType, FALSE);
		m_wndScaleFactor.SetWindowText(_T("1.0"));
		m_wndScaleFactor.EnableWindow(FALSE);
		UpdateData(FALSE);
	}else if(m_type == 2)
	{
		CDlgUtil::CtrlShowHide(this, m_aConstCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aUserCtrl, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCodeCtrl, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlDataType, TRUE);
		m_wndScaleFactor.EnableWindow(TRUE);
		m_wndGrid.RearrangeTable(m_wndDataType);
	}

	DrawChart();
	m_ParamData.nFuncType = m_type+1;
}

T_HSFC_D& CHsfcItemDlg::GetParamData()
{
	return m_ParamData;
}

void CHsfcItemDlg::SetParamData(T_HSFC_D& data)
{
	m_bModify		= TRUE;
	m_ParamData = data;
	m_csOldName = m_ParamData.strFuncName;
}
void CHsfcItemDlg::OnBtnDyGen()
{
	UpdateData(TRUE);
	T_HSFC_K Key;
//   CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
	Key = m_pDoc->m_pAttrCtrl->GetHsfcKey(m_ParamData.strFuncName);
	
	T_UCHT_D UchtD;
	UchtD.nType = D_TYPE_HS_FUNC;  // Heat Source Func.
	UchtD.SetType(UchtD.nType);
	UchtD.nKey = Key;
	UchtD.nFuncType = m_type;
	UchtD.dTemper = m_temper.GetEditValue();
	UchtD.dMaxTemper = m_maxtemper.GetEditValue();
	UchtD.dReactive = m_reactive.GetEditValue();
	CUnitCtrl::m_UCHT_UNIT.dTemper    = CUnitCtrl::m_HSFC_UNIT.dK;
	CUnitCtrl::m_UCHT_UNIT.dMaxTemper = CUnitCtrl::m_HSFC_UNIT.dTemperature;
	
	CString csName; csName.Empty();
	int nCount = m_pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if (nCount == 0) csName = _LS(IDS_CMD_DYNA_RPT_HSFUNC_NAME);
	else csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_HSFUNC_NAME), nCount + 1);
		
	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if(dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;
		UchtD.bLogX = m_btnXLog.GetCheck();
		UchtD.bLogY = m_btnYLog.GetCheck();
		if(!m_pDoc->m_pDataCtrl->AddUcht(UchtD)) ASSERT(0);
	}
}
void CHsfcItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData()) return;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bSuccess;

	if (m_bModify)
		bSuccess = pDoc->m_pDataCtrl->ModifyHsfc(m_csOldName, m_ParamData);
	else
		bSuccess = pDoc->m_pDataCtrl->AddHsfc(m_ParamData);

	if (!bSuccess) return;
	CDialogMove::OnOK();
}
///////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
// Grid
void CHsfcItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_ParamData);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_bGridInit = TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////







///////////////////////////////////////////////////////////////////////////////////////////////////
// Chart
void CHsfcItemDlg::InitChart()
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
//	SetAxisLabelText(1,_T("Hsfc"));

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

void CHsfcItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CHsfcItemDlg::OnDataChanged()
{
	// Add handler to update graph
	T_HSFC_BASE data;
	int nSize = m_ParamData.aFuncData.GetSize();

	m_Graph.KillData();	
	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.aFuncData[i];
		SetValue(i, 0, data.dTime, data.dValue);
	}
	m_GraphView.Invalidate();
}

void CHsfcItemDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CHsfcItemDlg::DrawChart()
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

void CHsfcItemDlg::GraphConstant()
{
	T_HSFC_BASE data;

	m_Graph.KillData();
	for(int i=0; i <= 30; i++)
	{
		data.dTime = i;
		data.dValue = m_temper.GetEditValue();
		SetValue(i, 0, data.dTime, data.dValue);
	}
	m_GraphView.Invalidate();
}


void CHsfcItemDlg::GraphCode()
{
	T_HSFC_BASE data;

	m_Graph.KillData();
	for(int i=0; i <= 30; i++)
	{
		double k = m_maxtemper.GetEditValue();
		double a = m_reactive.GetEditValue();
		data.dTime = i;
		data.dValue = k * (1-exp(-a*i));
		SetValue(i, 0, data.dTime, data.dValue);
	}
	m_GraphView.Invalidate();
}


void CHsfcItemDlg::GraphUser()
{
	OnDataChanged();
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void CHsfcItemDlg::OnHsfcSinRedrawGraph() 
{
	// TODO: Add your control notification handler code here
	DrawChart();
}

void CHsfcItemDlg::OnETCHSFCCHARTXLog() 
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

void CHsfcItemDlg::OnETCHSFCCHARTYLog() 
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

void CHsfcItemDlg::OnCheckUseConc() 
{
	UpdateData();

	m_ParamData.bUseConcData = m_btnUseConc.GetCheck(); // 추가 
	m_maxtemper.SetReadOnly(m_btnUseConc.GetCheck()); //IDC_HSFC_MAX_EDIT
	m_reactive.SetReadOnly(m_btnUseConc.GetCheck()); //IDC_HSFC_REACT_EDIT
	CDlgUtil::CtrlEnableDisable(this, IDC_EDIT_CEMENT_CONTENT, m_btnUseConc.GetCheck());
	CDlgUtil::CtrlEnableDisable(this, IDC_COB_CEMENT_TYPE, m_btnUseConc.GetCheck());
	CDlgUtil::CtrlEnableDisable(this, IDC_COB_TEMPERATURE_TYPE, m_btnUseConc.GetCheck());
}

void CHsfcItemDlg::OnSelchangeCobCementType() 
{
	Calcu();
}

void CHsfcItemDlg::OnSelchangeCobTemperatureType() 
{	
	Calcu();
}

void CHsfcItemDlg::OnChangeEditCementContent() 
{
	Calcu();
}

void CHsfcItemDlg::OnUpdateEditCementContent() 
{	
	Calcu();
}

BOOL CHsfcItemDlg::Calcu()
{
	double dK,dAlpha = 0;
	int nIndexC =  m_ctrlCobCement.GetCurSel();
	int nCementType = m_ctrlCobCement.GetItemData(nIndexC);
	int nIndexT = m_ctrlCobTemperature.GetCurSel();
	int nTemperature = m_ctrlCobTemperature.GetItemData(nIndexT);
	double dCementContent = m_editCementContent.GetEditValue();

	if( !m_pDoc->m_pEditData->CalcHeatSourceData(nCementType, nTemperature, dCementContent, dK, dAlpha) )
		return FALSE;

	m_maxtemper.SetEditUnit(dK); //IDC_HSFC_MAX_EDIT
	m_reactive.SetEditUnit(dAlpha); //IDC_HSFC_REACT_EDIT
	this->Invalidate();

	m_ParamData.nCementType = nCementType; // 멤버 변수에 최신정보 할당 
	m_ParamData.nTemperature = nTemperature;
	m_ParamData.dCementContent = dCementContent;

	return TRUE;
}

void CHsfcItemDlg::OnSetfocusCobCementType() // combobox dynamic change
{
}

void CHsfcItemDlg::OnHsfcDataTypeRdo() 
{
	UpdateData();
	m_wndGrid.RearrangeTable(m_wndDataType);	
}
