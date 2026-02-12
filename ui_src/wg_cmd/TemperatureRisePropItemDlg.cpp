// MhtpItemDlg.cpp : implementation file
//

#include <math.h>
#include "stdafx.h"
#include "wg_cmd.h"
#include "TemperatureRisePropItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\DynamicReportTemplateDlg.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_main\wg_mainres2.h"

#include "..\wg_base\TestEnvMgr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
CTemperatureRisePropItemDlg::CTemperatureRisePropItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CTemperatureRisePropItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTemperatureRisePropItemDlg)
	m_nType = 0;
	m_nStlType = 0;
	m_nGraphType = 0;
	m_dScaleFactor = 1.0;
	m_csTypeName = _T("");

	//}}AFX_DATA_INIT
	m_bModify = FALSE;
	m_bGridInit = FALSE;


	m_aCodeCtrl.Add(IDC_CMD_TEMPERATURE_RISE_PROP_CODE_STATIC);
	m_aCodeCtrl.Add(IDC_CMD_TEMPERATURE_RISE_PROP_CODE_COMBO);
	m_aCodeCtrl.Add(IDC_CMD_TEMPERATURE_RISE_PROP_STL_TYPE_STATIC);
	m_aCodeCtrl.Add(IDC_CMD_TEMPERATURE_RISE_PROP_STRUCT_STL_TYPE);
	m_aCodeCtrl.Add(IDC_CMD_TEMPERATURE_RISE_PROP_FIRE_TYPE);
	
}


void CTemperatureRisePropItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTemperatureRisePropItemDlg)
	DDX_Control(pDX, IDC_CMD_TEMPERATURE_RISE_PROP_CHART_YLog, m_btnYLog);
	DDX_Control(pDX, IDC_CMD_TEMPERATURE_RISE_PROP_CHART_XLog, m_btnXLog);
	DDX_Control(pDX, IDC_CMD_TEMPERATURE_RISE_PROP_CODE_COMBO, m_CobxCode);
	DDX_Radio(pDX, IDC_CMD_TEMPERATURE_RISE_PROP_CODE, m_nType);
	DDX_Radio(pDX, IDC_CMD_TEMPERATURE_RISE_PROP_STRUCT_STL_TYPE, m_nStlType);
	DDX_Radio(pDX, IDC_CMD_TEMPERATURE_RISE_PROP_GRAPHTYPE_RDO1, m_nGraphType);
	DDX_Text(pDX, IDC_CMD_TEMPERATURE_RISE_PROP_NAME, m_csTypeName);
	DDX_Text(pDX, IDC_CMD_TEMPERATURE_RISE_PROP_SCALE, m_dScaleFactor);
	DDX_Control(pDX, IDC_CMD_DYGEN, m_btnDyGen);
	DDX_Control(pDX, IDC_CMD_TEMPERATURE_RISE_PROP_TABLE, m_wndGrid);
	//}}AFX_DATA_MAP

	if (pDX->m_bSaveAndValidate)
	{
		m_ParamData.strMatlName = m_csTypeName;
		m_ParamData.nMatlType = m_nType+1;
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

void CTemperatureRisePropItemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CTemperatureRisePropItemDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	POSITION pos = pViewBuff->GetStartBuffer();
	T_UDRD_BUFFER buffer_ur;
	BOOL bMATL = FALSE;
	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case UR_MATL_ADD:
		case UR_MATL_DEL:
		case UR_MATL_MFD:
		case UR_MATL_MFS:
			/*InitMatlCombo();
			OnTdmeMatlCmb();*/
			break;
		default:
			break;
		}
	}
}


BEGIN_MESSAGE_MAP(CTemperatureRisePropItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CTemperatureRisePropItemDlg)
	ON_BN_CLICKED(IDC_CMD_TEMPERATURE_RISE_PROP_CODE, OnMhtpSelChangeType)	
	ON_BN_CLICKED(IDC_CMD_TEMPERATURE_RISE_PROP_USER, OnMhtpSelChangeType)	
	ON_CBN_SELCHANGE(IDC_CMD_TEMPERATURE_RISE_PROP_CODE_COMBO, OnSelchangeMhtpCodeCombo)
	ON_BN_CLICKED(IDC_CMD_TEMPERATURE_RISE_PROP_STRUCT_STL_TYPE, OnMhtpSteelTypeChk)
	ON_BN_CLICKED(IDC_CMD_TEMPERATURE_RISE_PROP_FIRE_TYPE, OnMhtpSteelTypeChk)
	ON_BN_CLICKED(IDC_CMD_TEMPERATURE_RISE_PROP_SIN_REDRAW_GRAPH, OnMhtpSinRedrawGraph)
	ON_BN_CLICKED(IDC_CMD_TEMPERATURE_RISE_PROP_CHART_XLog, OnMhtpChartXLog)
	ON_BN_CLICKED(IDC_CMD_TEMPERATURE_RISE_PROP_CHART_YLog, OnMhtpChartYLog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CMD_TEMPERATURE_RISE_PROP_GRAPHTYPE_RDO1, OnMhtpSinRedrawGraph)	
	ON_BN_CLICKED(IDC_CMD_TEMPERATURE_RISE_PROP_GRAPHTYPE_RDO3, OnMhtpSinRedrawGraph)	
	ON_BN_CLICKED(IDC_CMD_DYGEN, OnBtnDyGen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTemperatureRisePropItemDlg message handlers
BOOL CTemperatureRisePropItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	CDBDoc::GetDocPoint()->m_pUnitCtrl->GetUnitIndexCurrent(m_CurrUnit);

	CDlgUtil::GetCtrlIDByIncRect(this, m_aCodeCtrl     , IDC_CMD_TEMPERATURE_RISE_PROP_CODE_TYPE     , TRUE);

	/////////////////////////////////////////////////////////////////
	
	m_nType = 0;
	m_nStlType = 0;
	
	InitGrid();

	InitChart();
	m_wndGrid.MakeItemEx();	

	OnSelchangeMhtpCodeCombo();	
	if (m_bModify)
		Data2Dlg();	
	OnMhtpSelChangeType();

	// Buttons
	m_btnDyGen.SetWindowText(_T(""));
	m_btnDyGen.SetImage(_T("SVG\\Illustration\\Button\\btn_report_chart_24.svg"));

	if(!CDBDoc::EnableDynaReport())
		GetDlgItem(IDC_CMD_DYGEN)->ShowWindow(SW_HIDE); // PMS:XXXX-JWKWON-20100502 : US 버전에만 반영

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTemperatureRisePropItemDlg::OnBtnDyGen()
{
	UpdateData(TRUE);
	T_MHTP_K Key;
	Key = m_pDoc->m_pAttrCtrl->GetMhtpKey(m_ParamData.strMatlName);

	T_KEY_LIST aKey;
	m_pDoc->m_pAttrCtrl->GetUchtKeyList(aKey);

	T_UCHT_D UchtD;
	if (m_nGraphType == 0)
		UchtD.nType = D_TYPE_RT_C_FUNC;  // Rising Temperature Material Function.
	else
		UchtD.nType = D_TYPE_RT_E_FUNC;
	UchtD.nKey = Key;

	CString csName; csName.Empty();
	int nCount = m_pDoc->m_pAttrCtrl->GetCountTypeUcht(UchtD.nType);
	if (nCount == 0)
	{
		csName = m_nGraphType == 0 ? _LS(IDS_CMD_DYNA_RPT_RTMCFUNC_NAME) : _LS(IDS_CMD_DYNA_RPT_RTMEFUNC_NAME);//_T("Rising Temperature Material");
	}
	else
	{
		if (m_nGraphType == 0)
			csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_RTMCFUNC_NAME), nCount + 1);
		else
			csName.Format(_T("%s%d"), _LS(IDS_CMD_DYNA_RPT_RTMEFUNC_NAME), nCount + 1);
	}

	CDynamicReportTemplateDlg dlg;
	dlg.SetNamePtr(&csName);
	if(dlg.DoModal() == IDOK)
	{
		UchtD.strName = csName;
		UchtD.bLogX = m_btnXLog.GetCheck();
		UchtD.bLogY = m_btnYLog.GetCheck();
		if(!m_pDoc->m_pDataCtrl->AddUcht(UchtD)) { ASSERT(0); return; }
	}
	
}

void CTemperatureRisePropItemDlg::Data2Dlg()
{
	UpdateData(TRUE);
	CString str;
	m_csTypeName = m_ParamData.strMatlName;
	m_nType = m_ParamData.nMatlType-1 < 0 ? 0 : m_ParamData.nMatlType-1;
	if (m_nType == 0)
	{
		m_nStlType = m_ParamData.nStlType-1 < 0 ? 0 : m_ParamData.nStlType-1;
		m_CobxCode.SetCurSel(0);  
		m_dScaleFactor = 1.0;
	}
	else
	{
		m_dScaleFactor = m_ParamData.dScaleFactor;
		CString str = _T("");
		str.Format(_T("%.1f"), m_dScaleFactor);
		GetDlgItem(IDC_CMD_TEMPERATURE_RISE_PROP_SCALE)->SetWindowText(str);
	}
	  
	UpdateData(FALSE);
}

void CTemperatureRisePropItemDlg::Dlg2Data()
{
	m_ParamData.nMatlType = m_nType+1;
	m_ParamData.nStlType = m_nStlType+1;
	m_ParamData.strMatlName = m_csTypeName;
	m_ParamData.dScaleFactor = m_dScaleFactor;
}

void CTemperatureRisePropItemDlg::OnMhtpSelChangeType() 
{
	// TODO: Add your control notification handler code here
	int nPrevType = m_nType;
	UpdateData(TRUE);

	if (nPrevType == 1 && m_nType != 1)  // user
	{
		if (!m_wndGrid.OnTerminate(D_TB_SAVE_ALWAYS))
			m_wndGrid.CancelEdit();
	}

	if (m_nType == 0)
	{
		CDlgUtil::CtrlShowHide(this, m_aCodeCtrl, TRUE);
		GetDlgItem(IDC_CMD_TEMPERATURE_RISE_PROP_TABLE)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_TEMPERATURE_RISE_PROP_SCALE)->SetWindowText(_T("1.0"));
		GetDlgItem(IDC_CMD_TEMPERATURE_RISE_PROP_SCALE)->EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else if (m_nType == 1)
	{
		CDlgUtil::CtrlShowHide(this, m_aCodeCtrl, FALSE);
		GetDlgItem(IDC_CMD_TEMPERATURE_RISE_PROP_TABLE)->ShowWindow(TRUE);
		GetDlgItem(IDC_CMD_TEMPERATURE_RISE_PROP_SCALE)->EnableWindow(TRUE);
	}

	DrawChart();
	UpdateData(FALSE);
}

void CTemperatureRisePropItemDlg::OnSelchangeMhtpCodeCombo() 
{
	// TODO: Add your control notification handler code here
	CString strCode = _T("GB51249-2017");
	m_CobxCode.AddString(strCode);
	m_CobxCode.SetCurSel(0);

	DrawChart();
}

T_MHTP_D& CTemperatureRisePropItemDlg::GetParamData()
{
	return m_ParamData;
}

void CTemperatureRisePropItemDlg::SetParamData(T_MHTP_D& data)
{
	m_bModify		= TRUE;
	m_ParamData = data;
	m_csOldName = m_ParamData.strMatlName;
	m_csTypeName = m_ParamData.strMatlName;
	m_nType = m_ParamData.nMatlType-1;
	m_nStlType = m_ParamData.nStlType-1;
	m_dScaleFactor = m_ParamData.dScaleFactor;
}

void CTemperatureRisePropItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!UpdateData()) return;
	Dlg2Data();
	BOOL bSuccess;

	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyMhtp(m_csOldName, m_ParamData);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddMhtp(m_ParamData);

	if (!bSuccess) return;
	CDialogMove::OnOK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Grid
void CTemperatureRisePropItemDlg::InitGrid()
{
	m_wndGrid.Initialize(&m_ParamData);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_bGridInit = TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
// Chart
void CTemperatureRisePropItemDlg::InitChart()
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
	SetAxisLabelText(0, _LS(IDS_WG_CMD__ADDD__FLUE_TemPerature_));
	
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



void CTemperatureRisePropItemDlg::SetAxisLabelText(int nComponent, LPCTSTR lbl)
{
	SRGraphLabel *pLabel=(SRGraphLabel *) m_Graph.GetComponent(nComponent, IDS_SRG_LABELTYPE);
	pLabel->SetAnnotation(lbl);
}

void CTemperatureRisePropItemDlg::SetValue(int nIndex, int nGroup, double x, double y)
{
	m_Graph.SetValue(nIndex, nGroup*2, x);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetObjectStyle(CX_OBJECT_LINE);
	m_Graph.GetGroup(nGroup*2)->GetIndex(nIndex)->GetStyle()->SetFrameWidth(2);
	m_Graph.SetValue(nIndex, nGroup*2+1, y);
}

void CTemperatureRisePropItemDlg::DrawChart()
{
	UpdateData();

	if(m_nType == 0)
		GraphCode();
	else if(m_nType == 1)
		GraphUser();
	
}

void CTemperatureRisePropItemDlg::GraphCode()
{
	UpdateData();
	T_MHTP_BASE data;
	m_acode.RemoveAll();
	int ncode = m_CobxCode.GetCurSel();
	if(ncode == 0) 	
	{
		for (int i=0; i<=1000; i++)
		{
			data.nC0 = i;
			data.dComp = m_nStlType==0 ? m_pDoc->m_pAttrCtrl->GetStructureStlStrengthCoef(i) : m_pDoc->m_pAttrCtrl->GetFireStlStrengthCoef(i);
			data.dElast = m_nStlType==0 ? m_pDoc->m_pAttrCtrl->GetStructureStlElastCoef(i) : m_pDoc->m_pAttrCtrl->GetFireStlElastCoef(i);
			m_acode.Add(data);
		}	
	}

	m_Graph.KillData();
	int nNum = m_acode.GetSize();
	for(int i=0; i < nNum; i++)
	{
		if (m_nGraphType == 0)
			SetValue(i, 0, m_acode[i].nC0, m_acode[i].dComp);
		else
			SetValue(i, 0, m_acode[i].nC0, m_acode[i].dElast);
	}
	m_GraphView.Invalidate();
}

void CTemperatureRisePropItemDlg::GraphUser()
{
	OnDataChanged();
}

void CTemperatureRisePropItemDlg::OnDataChanged()
{
	// Add handler to update graph
	T_MHTP_BASE data;
	int nSize = m_ParamData.aFuncData.GetSize();
	UpdateData();
	m_acode.RemoveAll();
	m_Graph.KillData();	
	for(int i=0; i<nSize; i++)
	{
		data = m_ParamData.aFuncData[i];
		if (m_nGraphType == 0)
			SetValue(i, 0, data.nC0, data.dComp);
		else
			SetValue(i, 0, data.nC0, data.dElast);
	}
	m_GraphView.Invalidate();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void CTemperatureRisePropItemDlg::OnMhtpSinRedrawGraph() 
{
	// TODO: Add your control notification handler code here
	DrawChart();
}

void CTemperatureRisePropItemDlg::OnMhtpChartXLog() 
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

void CTemperatureRisePropItemDlg::OnMhtpChartYLog() 
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

void CTemperatureRisePropItemDlg::OnMhtpSteelTypeChk()
{
	if (GetDlgItem(IDC_CMD_TEMPERATURE_RISE_PROP_STRUCT_STL_TYPE))
		m_nStlType = 0;
	else
		m_nStlType = 1;
}
