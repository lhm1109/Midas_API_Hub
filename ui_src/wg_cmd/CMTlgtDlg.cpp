// CMTlgtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTlgtDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_strParser.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_QSort.h"
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_dbLock\Lockoption.h"

#include "CMTlgtPreviewDlg.h"
#include "CMTlgtEurocodeTrainTypeBDlg.h"

#include "MIDAS_MSLib\MSExcel.h"

#define SPIN_MIN_VALUE 0
#define SPIN_MAX_VALUE 30000


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMTlgtDlg dialog


CCMTlgtDlg::CCMTlgtDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMTlgtDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pDStore = new CCMTlgtDataStore(m_pDoc);

	m_nSelectionMethod = 0;
	m_nScale = 0;

	m_nTrainCode = TRAINCODE_UNKNOWN;
	m_nTrainType = TRAINTYPE_UNKNOWN;
	m_nVehcleNum = 1;

	m_aCtrlSelectNode.Add(IDC_ETC_TLGT_2POINTS_RDO);
	m_aCtrlSelectNode.Add(IDC_ETC_TLGT_PICKING_RDO);
	m_aCtrlSelectNode.Add(IDC_ETC_TLGT_ELEMSELECT_RDO);

	m_aCtrlScale.Add(IDC_ETC_TLGT_SCALE_RDO1);
	m_aCtrlScale.Add(IDC_ETC_TLGT_SCALE_RDO2);

	m_aCtrlVehicleType.Add(IDC_ETC_TLGT_TRAIN_TYPE);
	m_aCtrlVehicleType.Add(IDC_ETC_TLGT_TRAIN_TYPE_CMB);

	m_aWheelCtrl.Add(IDC_ETC_TLGT_WHEEL);
	m_aWheelCtrl.Add(IDC_ETC_TLGT_WHEEL_EDT);

	m_aVehiclCtrl.Add(IDC_ETC_TLGT_VEHICLE);
	m_aVehiclCtrl.Add(IDC_ETC_TLGT_VEHICLE_EDT);
	m_aVehiclCtrl.Add(IDC_ETC_TLGT_VEHICLE_BTN);

	m_tlgtInfo.Initialize();
	if (CDBLib::AllowJPRail())
	{
		m_tlgtInfo.nVehicleCode = TRAINCODE_JAPANRAIL;
		m_tlgtInfo.nVehicleType = TRAINCODE_JAPANRAIL_N16;
	}

	bInit = FALSE;

	m_pXL = NULL;
}

CCMTlgtDlg::~CCMTlgtDlg()
{
	if (m_pXL)
	{
		delete m_pXL;
	}
}

void CCMTlgtDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		break;
	case D_UPDATE_UNIT:
		UpdateUnit();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
	case D_UPDATE_POST_STAGE_CHANGED:
	case D_UPDATE_POST_STEP_CHANGED:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMTlgtDlg::UpdateUnit()
{
	// 노드 테이블 단위 변경
	SetGridTitle();
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	double dDistance;
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		dDistance = _tstof(m_wndGrid.GetValueRowCol(i + 1, 2));
		dDistance = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_LENGTH, dDistance);
		m_wndGrid.SetValueRange(CGXRange(i + 1, 2), dDistance);
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();

	SetHeaderTitle(FALSE);

	// Train Info 리스트 단위 변경
	TLGT_ITEM item;
	long lSize = m_aItem.GetSize();
	for (int i = 0; i < lSize; i++)
	{
		item.dLength = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_LENGTH, m_aItem.GetData()[i].dLength);
		item.dForce = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_FORCE, m_aItem.GetData()[i].dForce);
		m_aItem.GetData()[i] = item;
	}
	lSize = m_aUserItem.GetSize();
	for (int i = 0; i < lSize; i++)
	{
		item.dLength = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_LENGTH, m_aUserItem.GetData()[i].dLength);
		item.dForce = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_FORCE, m_aUserItem.GetData()[i].dForce);
		m_aUserItem.GetData()[i] = item;
	}
	OnUpdateList();
}

void CCMTlgtDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTlgtDlg)

	DDX_Radio(pDX, IDC_ETC_TLGT_2POINTS_RDO, m_nSelectionMethod);
	DDX_Control(pDX, IDC_ETC_TLGT_2POINTS_EDIT1, m_wnd2PointsEdit1);
	DDX_Control(pDX, IDC_ETC_TLGT_2POINTS_EDIT2, m_wnd2PointsEdit2);
	DDX_Control(pDX, IDC_ETC_TLGT_2POINTS_UNIT1, m_2PointUnit1);
	DDX_Control(pDX, IDC_ETC_TLGT_2POINTS_UNIT2, m_2PointUnit2);

	DDX_Control(pDX, IDC_ETC_TLGT_PICKING_EDIT, m_wndPickingEdit);
	DDX_Control(pDX, IDC_ETC_TLGT_NODE_SELECT_EDIT, m_wndNodeSelectEdit);

	DDX_Control(pDX, IDC_ETC_TLGT_LIST, m_List);
	DDX_Control(pDX, IDC_ETC_TLGT_DYNAMIC_LOAD_CMB, m_cmbLoadCase);

	DDX_Control(pDX, IDC_ETC_TLGT_NAME_EDT, m_edtName);
	
	DDX_Control(pDX, IDC_ETC_TLGT_TRAIN_CODE_CMB, m_cmbTrainCode);
	DDX_Control(pDX, IDC_ETC_TLGT_TRAIN_TYPE_CMB, m_cmbTrainType);
	DDX_Control(pDX, IDC_ETC_TLGT_WHEEL_EDT, m_edtNumWheels);
	DDX_Control(pDX, IDC_ETC_TLGT_VEHICLE_EDT, m_edtNumWVehicle);

	DDX_Control(pDX, IDC_ETC_TLGT_SPEED_EDT, m_edtSpeed);

	DDX_Radio(pDX, IDC_ETC_TLGT_SCALE_RDO1, m_nScale);

	DDX_Control(pDX, IDC_ETC_TLGT_SCALE_EDT, m_edtScale);
	DDX_Control(pDX, IDC_ETC_TLGT_MAX_EDT, m_edtMax);

	DDX_Control(pDX, IDC_ETC_TLGT_TIME_EDT, m_edtTime);
	DDX_Control(pDX, IDC_ETC_TLGT_TIME_UNT, m_untTime);
	DDX_Control(pDX, IDC_ETC_TLGT_DIRECT_CMB, m_cmbDir);

	DDX_Control(pDX, IDC_ETC_TLGT_DISTANCE_EDIT, m_edtLength);
	DDX_Control(pDX, IDC_ETC_TLGT_LOAD_EDIT, m_edtForce);

	DDX_Control(pDX, IDC_ETC_TLGT_PLACEHOLDER, m_wndGrid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMTlgtDlg, CCMDlgBase)
	ON_BN_CLICKED(IDOK, OnCmdBtnOk)
	ON_BN_CLICKED(IDC_ETC_TLGT_2POINTS_RDO, OnCmdChangeDefineTracks)
	ON_BN_CLICKED(IDC_ETC_TLGT_PICKING_RDO, OnCmdChangeDefineTracks)
	ON_BN_CLICKED(IDC_ETC_TLGT_ELEMSELECT_RDO, OnCmdChangeDefineTracks)
	ON_BN_CLICKED(IDC_ETC_TLGT_NODE_OP_ADD, OnCmdOpAdd)
	ON_BN_CLICKED(IDC_ETC_TLGT_NODE_OP_INSERT, OnCmdOpInsert)
	ON_BN_CLICKED(IDC_ETC_TLGT_NODE_OP_DELETE, OnCmdOpDelete)
	ON_BN_CLICKED(IDC_ETC_TLGT_DYNAMIC_LOAD_BTN, OnCmdLoadcaseBtn)
	ON_CBN_SELCHANGE(IDC_ETC_TLGT_TRAIN_CODE_CMB, OnCmdChangedTrainCode)
	ON_CBN_SELCHANGE(IDC_ETC_TLGT_TRAIN_TYPE_CMB, OnCmdChangedTrainType)
	ON_BN_CLICKED(IDC_ETC_TLGT_VEHICLE_BTN, OnCmdVehicleBtn)
    ON_BN_CLICKED(IDC_ETC_TLGT_TRAIN_TYPE_OPT_BTN, OnCmdClickTrainTypeOptBtn)
    ON_BN_CLICKED(IDC_ETC_TLGT_ADD_BTN, OnCmdTrainloadAdd)
	ON_BN_CLICKED(IDC_ETC_TLGT_MODIFY_BTN, OnCmdTrainloadModify)
	ON_BN_CLICKED(IDC_ETC_TLGT_DEL_BTN, OnCmdTrainloadDelete)
	ON_BN_CLICKED(IDC_ETC_TLGT_INSERT_BTN, OnCmdTrainloadInsert)
	ON_NOTIFY(NM_CLICK, IDC_ETC_TLGT_LIST, OnClickedList)
	ON_BN_CLICKED(IDC_ETC_TLGT_IMPORT_BTN, OnCmdOpenBtn)
	ON_BN_CLICKED(IDC_ETC_TLGT_EXPORT_BTN, OnCmdSaveBtn)
	ON_BN_CLICKED(IDC_ETC_TLGT_PRIVIEW_BTN, OnCmdPreviewBtn)
	ON_BN_CLICKED(IDC_ETC_TLGT_SCALE_RDO1, OnCmdChangeScaling)
	ON_BN_CLICKED(IDC_ETC_TLGT_SCALE_RDO2, OnCmdChangeScaling)
	ON_BN_CLICKED(IDC_ETC_TLGT_IMPORT_EXCEL_BTN, OnCmdExcelImport)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_BN_CLICKED(IDCANCEL, OnCmdBtnClose)

END_MESSAGE_MAP()

#define D_NUM_COL 2
void CCMTlgtDlg::InitGrid()
{
	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);

	CGXGridParam* pParam = m_wndGrid.GetParam();
	ASSERT_VALID(pParam);

	CGXProperties* pProp = pParam->GetProperties();
	ASSERT_VALID(pProp);

	pParam->EnableUndo(FALSE);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	// Row and column headers
	m_wndGrid.ColHeaderStyle().SetReadOnly(FALSE).SetEnabled(FALSE)
		.SetFont(CGXFont_GC().SetBold(FALSE));
	m_wndGrid.RowHeaderStyle().SetEnabled(FALSE);
	m_wndGrid.ChangeStandardStyle(CGXStyle().SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetAllowEnter(FALSE));

	// Special grid settings
	pParam->EnableMoveRows(TRUE);                  // rows cannot be dragged
	pParam->EnableMoveCols(FALSE);                 // columns can be dragged
	pParam->EnableTrackColWidth(FALSE);             // disable resizing
	pParam->EnableTrackRowHeight(FALSE);
	pParam->EnableThumbTrack(FALSE);               // thumb-tracking is disabled
	pParam->SetNumberedColHeaders(FALSE);
	// Excel과 같은 셀 선택 : DoLButtonDblClk 참조
	pParam->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	pParam->SetHideCurrentCell(GX_HIDE_NEVER);

	// removing rows is not undoable for this kind of grid
	pParam->SetRemoveColsFlags(FALSE);
	pParam->SetRemoveRowsFlags(FALSE);

	pProp->SetMarkColHeader(FALSE);    // Turn off pressed button effect for column headers
	pProp->SetPrintRowHeaders(FALSE);  // Don't print column headers

	m_wndGrid.SetRowHeightByDPI(0, 0, 32);
	m_wndGrid.SetDrawingTechnique(gxDrawUsingMemDC);

	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	m_wndGrid.SetColCount(D_NUM_COL);
	SetGridTitle();

	m_wndGrid.SetColWidthByDPI(0, 0, 40);
	m_wndGrid.SetColWidthByDPI(1, 1, 100);
	m_wndGrid.SetColWidthByDPI(2, 2, 100);

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMTlgtDlg::SetGridTitle()
{
	CString csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	m_wndGrid.SetValueRange(CGXRange(0, 0), _LS(IDS_WG_CMD__ADDD__No));
	m_wndGrid.SetValueRange(CGXRange(0, 1), _LS(IDS_WG_CMD__ADDD__Node));
	m_wndGrid.SetValueRange(CGXRange(0, 2), CString(_LS(IDS_WG_CMD__ADDD__Distance)) + _T("(") + csUnit + _T(")"));

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMTlgtDlg::SetText()
{
	this->SetWindowText(_LS(IDS_CMD_TLGT_TITLE));
	GetDlgItem(IDC_ETC_TLGT_OBJECT)->SetWindowText(_LS(IDS_CMD_TLGT_DEFINE_TRACK));

	GetDlgItem(IDC_ETC_TLGT_2POINTS_RDO)->SetWindowText(_LS(IDS_CMD_TLGT_2POINTS));
	GetDlgItem(IDC_ETC_TLGT_PICKING_RDO)->SetWindowText(_LS(IDS_CMD_TLGT_PICKING));
	GetDlgItem(IDC_ETC_TLGT_ELEMSELECT_RDO)->SetWindowText(_LS(IDS_CMD_TLGT_NUMBER));
	
	GetDlgItem(IDC_ETC_TLGT_NODE_OP)->SetWindowText(_LS(IDS_CMD_TLGT_OPERATIONS));
	GetDlgItem(IDC_ETC_TLGT_NODE_OP_ADD)->SetWindowText(_LS(IDS_CMD_TLGT_ADD));
	GetDlgItem(IDC_ETC_TLGT_NODE_OP_INSERT)->SetWindowText(_LS(IDS_CMD_TLGT_INSERT));
	GetDlgItem(IDC_ETC_TLGT_NODE_OP_DELETE)->SetWindowText(_LS(IDS_CMD_TLGT_DELETE));

	GetDlgItem(IDC_ETC_TLGT_DYNAMIC_LOAD)->SetWindowText(_LS(IDS_CMD_TLGT_LC));
	GetDlgItem(IDC_ETC_TLGT_NAME)->SetWindowText(_LS(IDS_CMD_TLGT_NAME));
	GetDlgItem(IDC_ETC_TLGT_TRAIN_CODE)->SetWindowText(_LS(IDS_CMD_TLGT_CODE));
	GetDlgItem(IDC_ETC_TLGT_TRAIN_TYPE)->SetWindowText(_LS(IDS_CMD_TLGT_TYPE));
	GetDlgItem(IDC_ETC_TLGT_WHEEL)->SetWindowText(_LS(IDS_CMD_TLGT_WHEEL));
	GetDlgItem(IDC_ETC_TLGT_VEHICLE)->SetWindowText(_LS(IDS_CMD_TLGT_VEHICLE));
	
	GetDlgItem(IDC_ETC_TLGT_SPEED)->SetWindowText(_LS(IDS_CMD_TLGT_VELOCITY));
	GetDlgItem(IDC_ETC_TLGT_SPEED_UNT)->SetWindowText(_LS(IDS_CMD_TLGT_VELOCITY_UNT));

	GetDlgItem(IDC_ETC_TLGT_SCALE)->SetWindowText(_LS(IDS_CMD_TLGT_SCALING));
	GetDlgItem(IDC_ETC_TLGT_SCALE_RDO1)->SetWindowText(_LS(IDS_CMD_TLGT_SCALING_SCALE_FACTOR));
	GetDlgItem(IDC_ETC_TLGT_SCALE_RDO2)->SetWindowText(_LS(IDS_CMD_TLGT_SCALING_MAX_VALUE));

	GetDlgItem(IDC_ETC_TLGT_TIME)->SetWindowText(_LS(IDS_CMD_TLGT_TIME));
	GetDlgItem(IDC_ETC_TLGT_START_TIME)->SetWindowText(_LS(IDS_CMD_TLGT_TIME_START));

	GetDlgItem(IDC_ETC_TLGT_DIRECT)->SetWindowText(_LS(IDS_CMD_TLGT_DIRECTION));

	GetDlgItem(IDC_ETC_TLGT_ADD_BTN)->SetWindowText(_LS(IDS_CMD_TLGT_ADD));
	GetDlgItem(IDC_ETC_TLGT_MODIFY_BTN)->SetWindowText(_LS(IDS_CMD_TLGT_MODIFY));
	GetDlgItem(IDC_ETC_TLGT_DEL_BTN)->SetWindowText(_LS(IDS_CMD_TLGT_DELETE));
	GetDlgItem(IDC_ETC_TLGT_INSERT_BTN)->SetWindowText(_LS(IDS_CMD_TLGT_INSERT));
	
	GetDlgItem(IDC_ETC_TLGT_DISTANCE)->SetWindowText(_LS(IDS_CMD_TLGT_LENGTH));
	GetDlgItem(IDC_ETC_TLGT_LOAD)->SetWindowText(_LS(IDS_CMD_TLGT_FORCE));

	GetDlgItem(IDC_ETC_TLGT_IMPORT_BTN)->SetWindowText(_LS(IDS_CMD_OPEN_WIZ));
	GetDlgItem(IDC_ETC_TLGT_EXPORT_BTN)->SetWindowText(_LS(IDS_CMD_SAVE_WIZ));
	GetDlgItem(IDC_ETC_TLGT_PRIVIEW_BTN)->SetWindowText(_LS(IDS_CMD_SHOW_GRAPH));
	
}

void CCMTlgtDlg::SetControl()
{

	m_2PointUnit1.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_2PointUnit2.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);

	m_wnd2PointsEdit1.SetAttUcsPos();
	m_wnd2PointsEdit1.SetModeToUse(MOUSEEDIT_USE_SETP2_FIRST);
	m_wnd2PointsEdit2.SetAttUcsPos();
	m_wnd2PointsEdit2.SetModeToUse(MOUSEEDIT_USE_SETP2_SECOND);
	m_wnd2PointsEdit2.SetLButtonDownNotifyWindow(this);
	m_wnd2PointsEdit2.SetEnterNotifyWindow(this);
	m_wnd2PointsEdit1.SetNextLink(&m_wnd2PointsEdit2);

	m_wndPickingEdit.SetAttNodeList();
	m_wndPickingEdit.SetLButtonDownNotifyWindow(this);
	m_wndPickingEdit.SetEnterNotifyWindow(this);
	m_wndPickingEdit.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndPickingEdit.SetMaxNodeKeyNum(1);

	m_cmbLoadCase.SetLoadType(D_SELECTLOAD_HISTORY, TRUE);

	m_edtTime.SetUnitType(D_UNITSYS_BASE_TIME);
	m_untTime.SetUnitType(D_UNITSYS_BASE_TIME);

	m_edtScale.SetUnitType(D_UNITSYS_NONE);
	m_edtMax.SetUnitType(D_UNITSYS_NONE);

// 	m_spin.SetRange(SPIN_MIN_VALUE, SPIN_MAX_VALUE);
// 	m_spin.SetPos(SPIN_MIN_VALUE);

	m_cmbTrainCode.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbTrainCode, _LS(IDS_CMD_TLGT_CODE_KOREA),    TRAINCODE_KOREA);
	CDlgUtil::CobxAddItem(m_cmbTrainCode, _LS(IDS_CMD_TLGT_CODE_UK),       TRAINCODE_UK);
    CDlgUtil::CobxAddItem(m_cmbTrainCode, _LS(IDS_CMD_TLGT_CODE_EUROCODE), TRAINCODE_EUROCODE);
	CDlgUtil::CobxAddItem(m_cmbTrainCode, _LS(IDS_CMD_TLGT_CODE_JAPAN_RAIL),TRAINCODE_JAPANRAIL);
    CDlgUtil::CobxAddItem(m_cmbTrainCode, _LS(IDS_CMD_TLGT_CODE_USER),     TRAINCODE_USER);

	m_cmbTrainType.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_1), TRAINTYPE_KOREA_MKH_D2);
	CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_2), TRAINTYPE_KOREA_PMC_8);
	CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_3), TRAINTYPE_KOREA_KTX_20);
	CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_4), TRAINTYPE_KOREA_EL18_6);
	CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_5), TRAINTYPE_KOREA_EL18_8);
	CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_6), TRAINTYPE_KOREA_EL18_10);

	m_edtNumWheels.EnableWindow(FALSE);
	
	//
	CRect rRef;
	CRect rToMove;
	GetDlgItem(IDC_ETC_TLGT_WHEEL)->GetWindowRect(rRef);
	GetDlgItem(IDC_ETC_TLGT_VEHICLE)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;
	int nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aVehiclCtrl, nDistX, nDistY);
	//

	m_cmbDir.ResetContent();
	CDlgUtil::CobxAddItem(m_cmbDir, _LS(IDS_CMD_X), DIR_X);
	CDlgUtil::CobxAddItem(m_cmbDir, _LS(IDS_CMD_Y), DIR_Y);
	CDlgUtil::CobxAddItem(m_cmbDir, _LS(IDS_CMD_Z), DIR_Z);
	CDlgUtil::CobxAddItem(m_cmbDir, _LS(IDS_CMD_NEGATIVE_X), DIR_N_X);
	CDlgUtil::CobxAddItem(m_cmbDir, _LS(IDS_CMD_NEGATIVE_Y), DIR_N_Y);
	CDlgUtil::CobxAddItem(m_cmbDir, _LS(IDS_CMD_NEGATIVE_Z), DIR_N_Z);

	m_edtName.SetWindowText(_T(""));

	AutoAdjustComboWidth(m_cmbTrainType);
}

void CCMTlgtDlg::OnCmdChangeDefineTracks()
{
	UpdateData(TRUE);
	SelectMethodMan();
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	if (m_nSelectionMethod == 0) m_wnd2PointsEdit1.SetFocus();
	else if (m_nSelectionMethod == 1) m_wndPickingEdit.SetFocus();
	else m_wndNodeSelectEdit.SetFocus();
}

void CCMTlgtDlg::OnCmdChangeScaling()
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlScale, nCheck);
	if (nCheck == 0)
	{
		m_edtScale.EnableWindow(TRUE);
		m_edtMax.EnableWindow(FALSE);
	}
	else
	{
		m_edtScale.EnableWindow(FALSE);
		m_edtMax.EnableWindow(TRUE);
	}
}

BOOL CCMTlgtDlg::Grid_IsExistKey(int nKey)
{
	int nCount = m_wndGrid.GetRowCount();
	for (int i = 0; i < nCount; i++)
	{
		if (nKey == _tstol(m_wndGrid.GetValueRowCol(i + 1, 1))) return TRUE;
	}
	return FALSE;
}

void CCMTlgtDlg::Grid_InsertItem(int nPos, TLGT_NODE_ITEM &item)
{
	ROWCOL nRow;
	if (nPos < 0 || nPos > m_wndGrid.GetRowCount()) nRow = m_wndGrid.GetRowCount();
	else nRow = nPos;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	T_NODE_D node;
	m_pDoc->m_pAttrCtrl->GetNode(item.KeyNode, node);
	m_wndGrid.InsertRows(nRow + 1, 1);
	m_wndGrid.SetValueRange(CGXRange(nRow + 1, 1), (long)item.KeyNode);
	GetDistanceFromPrevNode(nRow + 1, item);
	m_wndGrid.SetValueRange(CGXRange(nRow + 1, 2), (double)item.dDistance);

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}


void CCMTlgtDlg::SelectNode()
{
	int nCount = m_wndGrid.GetRowCount();
	if (nCount >= 0)
	{
		CArray<T_NODE_K, T_NODE_K>arSelNode;
		for (int i = 0; i < nCount; i++) arSelNode.Add(_tstol(m_wndGrid.GetValueRowCol(i + 1, 1)));
		m_pDoc->m_pViewCtrl->SelectNode(NULL, arSelNode, TRUE, TRUE);
	}
}

void CCMTlgtDlg::Data2Dlg()
{
	TLGT_NODE_ITEM item; item.Initialize();

	if (m_wndGrid.GetRowCount() > 0)
		m_wndGrid.RemoveRows(1, m_wndGrid.GetRowCount());

	int nPos = -1;
	int nCount = m_tlgtInfo.arKeyNode.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		int nNodeKey = m_tlgtInfo.arKeyNode[i];

		T_NODE_D NodeD;
		if (!m_pDoc->m_pAttrCtrl->GetNode(nNodeKey, NodeD))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_not_exist_), nNodeKey);
			AfxMessageBox(msg);
			break;
		}

		item.KeyNode = (unsigned int)nNodeKey;
		Grid_InsertItem(nPos, item);
		if (nPos >= 0) nPos++;
	}

	// OnCmdChangedTrainCode 이전에 세팅
	m_edtNumWVehicle.SetEditUnit(m_tlgtInfo.nNumVehicle);

	m_cmbLoadCase.ChangeSelect(D_LOADCASE_HISTORY, m_tlgtInfo.keyThis);

	CString strName = m_tlgtInfo.strName;
	m_edtName.SetWindowText(strName);

	int nResult = CDlgUtil::CobxSetCurSelItemData(m_cmbTrainCode, m_tlgtInfo.nVehicleCode);
	if (nResult == CB_ERR) m_cmbTrainCode.SetCurSel(0);

	OnCmdChangedTrainCode();

	nResult = CDlgUtil::CobxSetCurSelItemData(m_cmbTrainType, m_tlgtInfo.nVehicleType);
	if (nResult == CB_ERR) m_cmbTrainType.SetCurSel(0);

	OnCmdChangedTrainType();

	//m_nVehcleNum = m_tlgtInfo.nNumVehicle;

	m_edtSpeed.SetEditUnit(m_tlgtInfo.dVelocity);

	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlScale, m_tlgtInfo.nScaling);
	m_edtScale.SetEditUnit(m_tlgtInfo.dSacleFactor);
	m_edtMax.SetEditUnit(m_tlgtInfo.dMaximumValue);
	OnCmdChangeScaling();


	m_edtTime.SetEditUnit(m_tlgtInfo.dTime);

	nResult = CDlgUtil::CobxSetCurSelItemData(m_cmbDir, m_tlgtInfo.nDir);
	if (nResult == CB_ERR) m_cmbDir.SetCurSel(0);


	m_edtLength.SetEditUnit(0.);
	m_edtForce.SetEditUnit(0.);
	
	if (m_tlgtInfo.nVehicleCode == TRAINCODE_USER)
		m_aUserItem.Copy(m_tlgtInfo.arTlgtItem);

	if (bInit)
	{
		m_aItem.RemoveAll();
		m_aItem.Copy(m_tlgtInfo.arTlgtItem);
		OnUpdateList();
	}
}

BOOL CCMTlgtDlg::Dlg2Data()
{
	// Node Array
	int nNodeCount = m_wndGrid.GetRowCount();
	if (nNodeCount < 3)
	{
		AfxMessageBox(_LS(IDS_CMD_ERR_TLGT_NODE_NUM));
		return FALSE;
	}
	m_tlgtInfo.arKeyNode.RemoveAll();
	for (int i = 0; i < nNodeCount; i++)
	{
		T_NODE_K NodeKey = _ttoi(m_wndGrid.GetValueRowCol(i + 1, 1));
		m_tlgtInfo.arKeyNode.Add(NodeKey);
	}

	// Dynamic Load Case
	UINT nLoadCaseType;
	if (!m_cmbLoadCase.GetSelectedLoad(nLoadCaseType, m_tlgtInfo.keyThis))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
		return FALSE;
	}

	// Name
	CString strName;
	m_edtName.GetWindowText(strName);
	strName.Trim();
	if (strName.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_CMD_ERR_TLGT_NAME));
		return FALSE;
	}
	m_tlgtInfo.strName = strName;

	DWORD dwCobxItemData = 0;

	// Vehicle Code
	int nResult = CDlgUtil::CobxGetItemDataByCurSel(m_cmbTrainCode, dwCobxItemData);
	if (nResult == CB_ERR)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_NO_SELECTED), _LS(IDS_CMD_TLGT_CODE));
		AfxMessageBox(msg);
		m_cmbTrainCode.SetCurSel(0);
		return FALSE;
	}
	m_tlgtInfo.nVehicleCode = dwCobxItemData;

	// Vehicle Type
	nResult = CDlgUtil::CobxGetItemDataByCurSel(m_cmbTrainType, dwCobxItemData);
	if (nResult == CB_ERR)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_NO_SELECTED), _LS(IDS_CMD_TLGT_TYPE));
		AfxMessageBox(msg);
		m_cmbTrainType.SetCurSel(0);
		return FALSE;
	}
	m_tlgtInfo.nVehicleType = dwCobxItemData;

	// Number of Wheels
	if (m_edtNumWheels.GetEditValue() < 1)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_WHEEL));
		AfxMessageBox(msg);
		return FALSE;
	}
	m_tlgtInfo.nNumWheel = m_edtNumWheels.GetEditValue();

	// Number of Wheels
	if (m_edtNumWVehicle.GetEditValue() < 1)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_VEHICLE));
		AfxMessageBox(msg);
		return FALSE;
	}
	m_tlgtInfo.nNumVehicle = m_edtNumWVehicle.GetEditValue();
	//m_nVehcleNum           = m_tlgtInfo.nNumVehicle;

	// Train Velocity
	if (m_edtSpeed.GetEditValue() < 1e-13)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_VELOCITY));
		AfxMessageBox(msg);
		return FALSE;
	}
	m_tlgtInfo.dVelocity = m_edtSpeed.GetEditValue();

	// Scaling
	int nScale;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlScale, nScale);
	m_tlgtInfo.nScaling = nScale;
	if (nScale == 0) // Scale Factor
	{
		m_tlgtInfo.dSacleFactor = m_edtScale.GetEditValue();
		if (fabs(m_tlgtInfo.dSacleFactor) < 1e-13)
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_SCALING_SCALE_FACTOR));
			AfxMessageBox(msg);
			return FALSE;
		}
	}
	else // Maximum Value
	{
		m_tlgtInfo.dMaximumValue = m_edtMax.GetEditValue();
		if (fabs(m_tlgtInfo.dMaximumValue) < 1e-13)
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_SCALING_MAX_VALUE));
			AfxMessageBox(msg);
			return FALSE;
		}
	}

	// Start Time
	if (m_edtTime.GetEditValue() < 0)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO_AND_EQUAL), _LS(IDS_CMD_TLGT_TIME_START));
		AfxMessageBox(msg);
		return FALSE;
	}
	m_tlgtInfo.dTime = m_edtTime.GetEditValue();

	// Direction
	nResult = CDlgUtil::CobxGetItemDataByCurSel(m_cmbDir, dwCobxItemData);
	if (nResult == CB_ERR)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_NO_SELECTED), _LS(IDS_CMD_TLGT_DIRECTION));
		AfxMessageBox(msg);
		return FALSE;
	}
	m_tlgtInfo.nDir = dwCobxItemData;

	// Train Load
	TLGT_ITEM tlgtItem;
	m_tlgtInfo.arTlgtItem.RemoveAll();
	for (long i = 0; i < m_aItem.GetSize(); i++)
	{
		tlgtItem.Initialize();
		tlgtItem.dLength = m_aItem.GetData()[i].dLength;
		tlgtItem.dForce = m_aItem.GetData()[i].dForce;
		m_tlgtInfo.arTlgtItem.Add(tlgtItem);
	}

	return TRUE;
}

void CCMTlgtDlg::SetHeaderTitle(BOOL bInit)
{
	// 속성
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// 사이즈
	RECT rt;
	m_List.GetClientRect(&rt);
	int width = (rt.right - rt.left)*.999;
	int nColWidth[3]; // id, length, force
	CString strTitle[] = { _LS(IDS_WG_CMD__ADDD__No), _LS(IDS_CMD_TLGT_LENGTH), _LS(IDS_CMD_TLGT_FORCE) };
	nColWidth[0] = width * 10 / 100;
	nColWidth[1] = width * 45 / 100;
	nColWidth[2] = width * 45 / 100;

	CString strFormat;
	LV_COLUMN lvcolumn;
	

	// Set Title
	for (int i = 0; i < 3; i++)
	{
		strFormat = strTitle[i];
		if (i != 0)
		{
			CString strLength, strForce;

			m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, strLength);
			m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_FORCE, strForce);
			strFormat.Format(_T("%s(%s)"), strTitle[i], (i == 1) ? strLength : strForce);
		}

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		lvcolumn.cx = nColWidth[i];
		lvcolumn.pszText = strFormat.GetBuffer(0);
		if(bInit)
			m_List.InsertColumn(i, &lvcolumn);
		else
			m_List.SetColumn(i, &lvcolumn);
		strFormat.ReleaseBuffer();
	}
}

void CCMTlgtDlg::OnUpdateList()
{
	m_List.DeleteAllItems();

	std::vector<CString> svValue;
	long lCount = m_aItem.GetSize();
	m_edtNumWheels.SetEditUnit(lCount);

	CStringArray aContents;
	CString      tmpStr;
	for (long i = 0; i < lCount; ++i)
	{
		aContents.RemoveAll();

		tmpStr.Format(_T("%d"), i+1); aContents.Add(tmpStr);
		tmpStr.Format(_T("%.3f"), m_aItem.GetData()[i].dLength); aContents.Add(tmpStr);
		tmpStr.Format(_T("%.3f"), m_aItem.GetData()[i].dForce); aContents.Add(tmpStr);
		CDlgUtil::SetListItem(&m_List, i, aContents, (DWORD)i);
	}
}

void CCMTlgtDlg::SetVehicleTypeList(int nCode)
{
	m_cmbTrainType.ResetContent();
	if (nCode == TRAINCODE_KOREA)
	{
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_1), TRAINTYPE_KOREA_MKH_D2);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_2), TRAINTYPE_KOREA_PMC_8);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_3), TRAINTYPE_KOREA_KTX_20);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_4), TRAINTYPE_KOREA_EL18_6);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_5), TRAINTYPE_KOREA_EL18_8);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_KOREA_6), TRAINTYPE_KOREA_EL18_10);
	}
	else if (nCode == TRAINCODE_UK)
	{
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_UK_1), TRAINTYPE_UK_ATC_SINGLE_AXLE);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_UK_2), TRAINTYPE_UK_ATC_DOUBLE_DECK_11);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_UK_3), TRAINTYPE_UK_ATC_DOUBLE_DECK_10);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_UK_4), TRAINTYPE_UK_ATC_DISTRIBUTED);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_UK_5), TRAINTYPE_UK_CVN_CURRENT);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_UK_6), TRAINTYPE_UK_CVN_OMNI);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_UK_7), TRAINTYPE_UK_CVN_FUTURE);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_UK_8), TRAINTYPE_UK_CVN_OMNI_28M);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_UK_9), TRAINTYPE_UK_CVN_SEGMENTED_28M);
	}
    else if (nCode == TRAINCODE_EUROCODE)
    {
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_A1),  TRAINTYPE_EUROCODE_A1);
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_A2),  TRAINTYPE_EUROCODE_A2);
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_A3),  TRAINTYPE_EUROCODE_A3);
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_A4),  TRAINTYPE_EUROCODE_A4);
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_A5),  TRAINTYPE_EUROCODE_A5);
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_A6),  TRAINTYPE_EUROCODE_A6);
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_A7),  TRAINTYPE_EUROCODE_A7);
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_A8),  TRAINTYPE_EUROCODE_A8);
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_A9),  TRAINTYPE_EUROCODE_A9);
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_A10), TRAINTYPE_EUROCODE_A10);
        CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_EUROCODE_B),   TRAINTYPE_EUROCODE_B);
    }
	else if (nCode == TRAINCODE_JAPANRAIL)
	{
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_JAPANRAIL_N16), TRAINCODE_JAPANRAIL_N16);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_JAPANRAIL_P16), TRAINCODE_JAPANRAIL_P16);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_JAPANRAIL_P17), TRAINCODE_JAPANRAIL_P17);
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_JAPANRAIL_M18), TRAINCODE_JAPANRAIL_M18);
	}
	else if (nCode == TRAINCODE_USER)
	{
		CDlgUtil::CobxAddItem(m_cmbTrainType, _LS(IDS_CMD_TLGT_TYPE_USER), TRAINTYPE_USER);
	}
	else
		ASSERT(0);
}

void CCMTlgtDlg::InitItemDataVar()
{
	m_wnd2PointsEdit1.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
	m_wnd2PointsEdit2.SetWindowText(_LS(IDS_WG_CMD__ADDD__0___0___0));
}

void CCMTlgtDlg::ShowHideByCode()
{
	BOOL bJP = (m_nTrainCode == TRAINCODE_JAPANRAIL) ? TRUE : FALSE;
	CDlgUtil::CtrlShowHide(this, m_aWheelCtrl, !bJP);
	CDlgUtil::CtrlShowHide(this, m_aVehiclCtrl, bJP);
}

BOOL CCMTlgtDlg::GetDistanceFromPrevNode(ROWCOL nRow, TLGT_NODE_ITEM& NodeItem)
{
	// 이전 노드와의 거리를 구하기 위해 사용
	if (nRow == 1)
	{
		NodeItem.dDistance = 0.0;
		return TRUE;
	}
	ROWCOL nRowPrev = nRow - 1;
	CString strNodeKey = m_wndGrid.GetValueRowCol(nRowPrev, 1);

	TLGT_NODE_ITEM NodeItemPrev;
	T_NODE_K NodeKeyPrev = _ttoi(strNodeKey);
	NodeItemPrev.KeyNode = NodeKeyPrev;

	T_NODE_D NodeDataCur, NodeDataPrev;
	if (!m_pDoc->m_pAttrCtrl->GetNode(NodeItem.KeyNode, NodeDataCur)) return FALSE;
	if (!m_pDoc->m_pAttrCtrl->GetNode(NodeItemPrev.KeyNode, NodeDataPrev)) return FALSE;

	double dDistance = m_pDStore->GetNodeDist(NodeDataCur, NodeDataPrev);
	NodeItem.dDistance = dDistance;
	return TRUE;
}

void CCMTlgtDlg::InsertBaseItem(int nPos, int nType)
{
	TLGT_NODE_ITEM item; item.Initialize();

	if (nType == 0)  // Select By 2 Points
	{

		CArray<T_NODE_K, T_NODE_K> aNodeKey;
		double ux, uy, uz;
		double WX1, WY1, WZ1;
		double WX2, WY2, WZ2;

		if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit1, ux, uy, uz))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___1st_position_is_invalid_));
			return;
		}
		m_wnd2PointsEdit1.GetWcsPos(ux, uy, uz, WX1, WY1, WZ1);
		if (!CFormulaEdit::GetPosition(&m_wnd2PointsEdit2, ux, uy, uz))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___2nd_position_is_invalid_));
			return;
		}
		m_wnd2PointsEdit2.GetWcsPos(ux, uy, uz, WX2, WY2, WZ2);

		// Get Node Key List
		double dbTolerance = m_pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
		m_pDoc->m_pDataCtrl->GetNodeBy2Point(WX1, WY1, WZ1, WX2, WY2, WZ2, dbTolerance, aNodeKey, TRUE);
		if (aNodeKey.GetSize() == 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_Node_on_line_));
			return;
		}

		int nNodeKey;
		int nCount = aNodeKey.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			nNodeKey = aNodeKey[i];
			if (Grid_IsExistKey(nNodeKey))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_already_ex), nNodeKey);
				AfxMessageBox(msg);
				return;
			}
			item.KeyNode = (unsigned int)nNodeKey;
			Grid_InsertItem(nPos, item);
			if (nPos >= 0) nPos++;
		}
		SelectNode();
		return;
	}
	else if (nType == 1) // Select By Picking
	{
		int nNodeKey;
		CFormulaEdit::GetEditValue(&m_wndPickingEdit, nNodeKey);

		if (nNodeKey <= 0)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_not_exist_), nNodeKey);
			AfxMessageBox(msg);
			return;
		}
		if (Grid_IsExistKey(nNodeKey))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No___d_is_already_ex), nNodeKey);
			AfxMessageBox(msg);
			return;
		}
		T_NODE_D Data;
		m_pDoc->m_pAttrCtrl->GetNode(nNodeKey, Data);
		item.KeyNode = (unsigned int)nNodeKey;
		Grid_InsertItem(nPos, item);
	}
	else if (nType == 2) // Select By Select Node
	{
		CArray<T_NODE_K, T_NODE_K> arNodeKeyList;
		CArray<long, long> arNodeList;

		CString strNodeNumber;
		m_wndNodeSelectEdit.GetWindowText(strNodeNumber);

		m_pDoc->m_pAttrCtrl->GetNodeKeyList(arNodeKeyList);

		CNumericOptimizer optimizer;
		optimizer.SelectNumByStrBase((long*)arNodeKeyList.GetData(), (long)arNodeKeyList.GetSize(),
			strNodeNumber, arNodeList);
		if (arNodeList.GetSize() <= 0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_node_));
			return;
		}

		for (int i = 0; i < arNodeList.GetSize(); i++)
		{
			if (Grid_IsExistKey((UINT)arNodeList[i]))
			{
				CString msg;
				msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Node_No__d__is_already_ex), (UINT)arNodeList[i]);
				AfxMessageBox(msg);
				return;
			}
		}
		T_NODE_D Data;
		int nInsertPos = nPos;
		for (int i = 0; i < arNodeList.GetSize(); i++)
		{
			m_pDoc->m_pAttrCtrl->GetNode((UINT)arNodeList[i], Data);
			item.KeyNode = (UINT)arNodeList[i];
			Grid_InsertItem(nPos, item);
			if (nInsertPos >= 0) nPos++;
		}
	}

	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	if (!bCurrent) ncCol = 1;

	if (nPos == -1) ncRow = m_wndGrid.GetRowCount();
	else ncRow = nPos + 1;

	m_wndGrid.SetCurrentCell(ncRow, ncCol);

	SelectNode();
}

void CCMTlgtDlg::OnCmdBtnOk()
{
	if (!m_pDoc->AllowCommand(D_OPTN_ID_HISTORY))
	{
		AfxMessageBox(_LS(IDS_DB_NO_LICENSE_HISTORY));
		return;
	}

#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_RAILWAY_BRIDGE_ANAL))
	{
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK8));
		AfxMessageBox(strErrMsg);
		return;
	}
#endif

#if defined(_CIVIL_ORG)
	if (!m_pDoc->CheckCivilKRMODS())
	{
		return;
	}
#endif

	if (!Dlg2Data())
		return;

	if (!GenerateData())
		return;
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	DestroyWindow();

	// Vehicle Code
	DWORD dwCobxItemData = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbTrainCode, dwCobxItemData);
	if(dwCobxItemData == TRAINCODE_UK)	
		IUsageCounter::Use(_T("TLGT_US"), FALSE);
}

BOOL CCMTlgtDlg::GenerateData()
{
	long lNodeSize = m_tlgtInfo.arKeyNode.GetSize();

	arThfcData.RemoveAll(); arThfcData.SetSize(lNodeSize);
	arThfcKey.RemoveAll(); arThfcKey.SetSize(lNodeSize);
	arThnlData.RemoveAll(); arThnlData.SetSize(lNodeSize);
	arThnlKey.RemoveAll(); arThnlKey.SetSize(lNodeSize);

	// Calc Train Load
	CArray<T_NODE_K, T_NODE_K> arKeyNode;
	arKeyNode.Copy(m_tlgtInfo.arKeyNode);

	CArray<double, double> arElemSize;
	CArray<double, double> arArrivalTime;
	arElemSize.SetSize(lNodeSize);
	arArrivalTime.SetSize(lNodeSize);


	T_NODE_D NodeData1, NodeData2;

	// Velocity 는 km/h 고정단위계로 받아오므로 현재단위계로 변경
	double dVelocity = (M_InitValueCurUnit(m_tlgtInfo.dVelocity*1000, N, M, D_UNITSYS_BASE_LENGTH)); // 1000*m (km)를 현재단위 L로 가져옴. 시간단위는 h
	dVelocity /= 3600.0; // 시간단위를 sec로 변경

	for (long i = 0; i < lNodeSize; ++i)
	{
		if (i == 0)
		{
			arElemSize[i] = 0.0;
			arArrivalTime[i] = 0.0;
			continue;
		}
		m_pDoc->m_pAttrCtrl->GetNode(arKeyNode[i - 1], NodeData1);
		m_pDoc->m_pAttrCtrl->GetNode(arKeyNode[i], NodeData2);
		arElemSize[i] = m_pDStore->GetNodeDist(NodeData1, NodeData2);

		m_pDoc->m_pAttrCtrl->GetNode(arKeyNode[0], NodeData1);
		m_pDoc->m_pAttrCtrl->GetNode(arKeyNode[i], NodeData2);
		arArrivalTime[i] = m_pDStore->GetNodeDist(NodeData1, NodeData2) / dVelocity;
	}

	CUnitCtrl* pUnitCtrl = ((CDBDoc*)CDBDoc::GetDocPoint())->m_pUnitCtrl;

	CArray<double, double> arTime, arValue;
	CArray<double, double> arTimeLast, arValueLast;

	for (long i = 0; i < lNodeSize; ++i)
	{
		double dLeftElemSize = 0.0;
		double dRightElemSize = 0.0;

		if (i == 0)
		{
			dLeftElemSize = 0.0;
			dRightElemSize = arElemSize[i + 1];
		}
		else if (i == lNodeSize - 1)
		{
			dLeftElemSize = arElemSize[i];
			dRightElemSize = 0.0;
		}
		else
		{
			dLeftElemSize = arElemSize[i];
			dRightElemSize = arElemSize[i + 1];
		}

		m_pDStore->CalcDynLoad(m_tlgtInfo, dLeftElemSize, dRightElemSize, arArrivalTime[i], arTime, arValue, dVelocity, FALSE);

		long nSize = arTime.GetSize();


		// 중복 제거
		for (long j = 0; j < nSize; j++)
		{
			if (j == 0)
			{
				arTimeLast.Add(arTime[j]);
				arValueLast.Add(arValue[j]);
			}
			else
			{
				if (arTime[j] != arTime[j - 1])
				{
					arTimeLast.Add(arTime[j]);
					arValueLast.Add(arValue[j]);
				}
				else // Time 같은경우 Load가 다르면 추가
				{
					if (arValue[j] != arValue[j - 1])
					{
						arTimeLast.Add(arTime[j]);
						arValueLast.Add(arValue[j]);
					}
				}
			}
		}

		nSize = arTimeLast.GetSize();

		for (long j = 0; j < nSize; j++)
		{
			//arTime[j] = pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_TIME, arTime[j]);
			//arValue[j] = pUnitCtrl->ConvertUnitForceOut(arValue[j]);

			T_THFC_BASE ThfcBase;
			ThfcBase.dblTime = arTimeLast[j];
			ThfcBase.dblValue = arValueLast[j];
			arThfcData[i].arThisFuncData.Add(ThfcBase);
		}

		arTimeLast.RemoveAll();
		arValueLast.RemoveAll();
		arTime.RemoveAll();
		arValue.RemoveAll();
	}

	// 에러 처리
	// 절점간 거리가 극적으로 차이나면 시간 이력 함수 생성 시 오류 발생
	for (long i = 0; i < lNodeSize; i++)
	{
		long lCount = arThfcData[i].arThisFuncData.GetCount();
		for (long j = 0; j < lCount - 1; j++)
		{
			double dFristTime = arThfcData[i].arThisFuncData[j].dblTime;
			double dNextTime = arThfcData[i].arThisFuncData[j+1].dblTime;
			if (dFristTime > dNextTime)
			{
				// error
				AfxMessageBox(_LS(IDS_CMD_ERR_TLGE_NODE_DISTANST));
				return FALSE;
			}
		}
	}

	// Generate Time History Function & Dynamic Nodal Load
	for (long i = 0; i < lNodeSize; i++)
	{
		arThfcData[i].ThisFuncName = m_tlgtInfo.strName;
		arThfcData[i].nThisFuncType = 1; // Time Function
		arThfcData[i].nThisDataType = 3; // Force
		arThfcData[i].nScaleMethod = m_tlgtInfo.nScaling;
		arThfcData[i].ScaleFactor = m_tlgtInfo.dSacleFactor;
		arThfcData[i].dMaxValue = m_tlgtInfo.dMaximumValue;
		arThfcData[i].Gravity = m_pDoc->m_pInitCtrl->DefaultGravity();
	}
	if (!(m_pDStore->MakeTimeHistoryFunc(arThfcData, arThfcKey)))
	{
		return FALSE;
	}
	for (long i = 0; i < lNodeSize; i++)
	{
		arThnlData[i].LoadCaseKey = m_tlgtInfo.keyThis;
		arThnlData[i].nLoadType = 1; // Force
		arThnlData[i].nThisFuncKey = arThfcKey[i];

		int nThnlDir = m_tlgtInfo.nDir;
		double dThnlScaleFactor = m_tlgtInfo.dSacleFactor;
		if (m_tlgtInfo.nDir >= DIR_N_X)
		{

			if (m_tlgtInfo.nDir == DIR_N_X) nThnlDir = DIR_X;
			else if (m_tlgtInfo.nDir == DIR_N_Y) nThnlDir = DIR_Y;
			else if (m_tlgtInfo.nDir == DIR_N_Z) nThnlDir = DIR_Z;
			else
			{
				ASSERT(0);
			}
			dThnlScaleFactor *= -1.0;
		}

		double dArrivalTimeMod = (i==0)? 0.0 : arArrivalTime[i-1];
		arThnlData[i].nDirection = nThnlDir + 1;
		arThnlData[i].dArrivalTime = dArrivalTimeMod;
		arThnlData[i].dFactor = dThnlScaleFactor;

		arThnlKey[i].key.entity = m_tlgtInfo.arKeyNode[i];
	}

	if (!(m_pDoc->m_pDataCtrl->AddThnl(arThnlKey, arThnlData, FALSE)))
	{
		return FALSE;
	}

	return TRUE;
}

void CCMTlgtDlg::SelectMethodMan()
{
	CArray<UINT, UINT> PointArr;
	CArray<UINT, UINT> PickingArr;
	CArray<UINT, UINT> ElemSelArr;

	PointArr.Add(IDC_ETC_TLGT_2POINTS_EDIT1);
	PointArr.Add(IDC_ETC_TLGT_2POINTS_EDIT2);
	PointArr.Add(IDC_ETC_TLGT_2POINTS_UNIT1);
	PointArr.Add(IDC_ETC_TLGT_2POINTS_UNIT2);
	PickingArr.Add(IDC_ETC_TLGT_PICKING_EDIT);
	ElemSelArr.Add(IDC_ETC_TLGT_NODE_SELECT_EDIT);

	if (m_nSelectionMethod == 0)
	{
		CDlgUtil::CtrlShowHide(this, PointArr, TRUE);
		CDlgUtil::CtrlShowHide(this, PickingArr, FALSE);
		CDlgUtil::CtrlShowHide(this, ElemSelArr, FALSE);
	}
	else if (m_nSelectionMethod == 1)
	{
		CRect DRect, SRect;

		m_wnd2PointsEdit1.GetWindowRect(&DRect);
		m_wndPickingEdit.GetWindowRect(&SRect);

		int nDistX = DRect.left - SRect.left;
		int nDistY = DRect.top - SRect.top;

		CDlgUtil::CtrlShowHide(this, PointArr, FALSE);
		CDlgUtil::CtrlShowHide(this, PickingArr, TRUE);
		CDlgUtil::CtrlShowHide(this, ElemSelArr, FALSE);

		CDlgUtil::CtrlMoveDistXY(this, PickingArr, nDistX, nDistY);
	}
	else
	{
		CRect DRect, SRect;
		m_wnd2PointsEdit1.GetWindowRect(&DRect);
		m_wndNodeSelectEdit.GetWindowRect(&SRect);

		int nDistX = DRect.left - SRect.left;
		int nDistY = DRect.top - SRect.top;

		CDlgUtil::CtrlShowHide(this, PointArr, FALSE);
		CDlgUtil::CtrlShowHide(this, PickingArr, FALSE);
		CDlgUtil::CtrlShowHide(this, ElemSelArr, TRUE);

		CDlgUtil::CtrlMoveDistXY(this, ElemSelArr, nDistX, nDistY);
	}
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
}

BOOL CCMTlgtDlg::OnInitDialog()
{
	CCMDlgBase::OnInitDialog();

	SetText();
	SetControl();
	SetHeaderTitle(TRUE);
	InitGrid();
	Data2Dlg();
	InitItemDataVar();
	ShowHideByCode();

	bInit = TRUE;
	return TRUE;
}

LRESULT CCMTlgtDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (m_nSelectionMethod == 0)
	{
		InsertBaseItem(-1, 0);
	}
	else if (m_nSelectionMethod == 1)
	{
		InsertBaseItem(-1, 1);
	}
	else
	{

	}
	return 0L;
}

void CCMTlgtDlg::OnCmdOpAdd()
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelectNode, nCheck);
	InsertBaseItem(-1, nCheck);
}

void CCMTlgtDlg::OnCmdOpInsert()
{
	// TODO: Add your control notification handler code here
	ROWCOL nRow, nCol;
	if (m_wndGrid.GetCurrentCell(nRow, nCol))
	{
		nRow = nRow - 1;
	}
	else nRow = -1;
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlSelectNode, nCheck);
	InsertBaseItem(nRow, nCheck);
}

void CCMTlgtDlg::OnCmdOpDelete()
{
	// TODO: Add your control notification handler code here
	ROWCOL ncRow, ncCol;
	BOOL bCurrent = m_wndGrid.GetCurrentCell(ncRow, ncCol);
	CRowColArray awRows;
	m_wndGrid.GetSelectedRows(awRows, TRUE, TRUE);
	if (awRows.GetSize() < 1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_item_to_delete_));
		return;
	}
	ROWCOL nRow;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i = awRows.GetSize() - 1; i >= 0; i--)
	{
		nRow = awRows.GetAt(i);
		if (nRow == 0) continue;
		m_wndGrid.RemoveRows(nRow, nRow);
	}

	int nRowCount = m_wndGrid.GetRowCount();
	if (bCurrent && nRowCount > 0)
	{
		if (nRowCount < ncRow) ncRow = nRowCount;
		m_wndGrid.SetCurrentCell(ncRow, ncCol);
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();

	SelectNode(); 
}

void CCMTlgtDlg::OnCmdTrainloadAdd()
{
	double dTempValue; // error check
	CString strLength, strForce;
	m_edtLength.GetWindowText(strLength);
	m_edtForce.GetWindowText(strForce);

	if (!CStrParser::GetFloatNumber(strLength, dTempValue))
	{
		CString strMsg = _T("");
		CString strTemp = _LS(IDS_CMD_TLGT_LENGTH);
		strMsg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE), strTemp);
		AfxMessageBox(strMsg);
		m_edtLength.SetFocus(); // Focus 옮기기
		m_edtLength.SetSel(0, -1, TRUE); // 하이라이트 주기
		return;
	}

	if (!CStrParser::GetFloatNumber(strForce, dTempValue))
	{
		CString strMsg = _T("");
		CString strTemp = _LS(IDS_CMD_TLGT_FORCE);
		strMsg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE), strTemp);
		AfxMessageBox(strMsg);
		m_edtForce.SetFocus(); // Focus 옮기기
		m_edtForce.SetSel(0, -1, TRUE); // 하이라이트 주기
		return;
	}
	
	TLGT_ITEM item;
	item.dLength = m_edtLength.GetEditValue();
	item.dForce = m_edtForce.GetEditValue();

	if (item.dLength < 0.0)
	{
		CString strMsg = _T("");
		CString strTemp = _LS(IDS_CMD_TLGT_LENGTH);
		strMsg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO_AND_EQUAL), strTemp);
		AfxMessageBox(strMsg);
		return;
	}

	m_aItem.Add(item);
	if (m_nTrainCode == TRAINCODE_USER)
		m_aUserItem.Add(item);

	OnUpdateList();
}

void CCMTlgtDlg::OnCmdTrainloadModify()
{
	double dTempValue; // error check
	CString strLength, strForce;
	m_edtLength.GetWindowText(strLength);
	m_edtForce.GetWindowText(strForce);
	if (!CStrParser::GetFloatNumber(strLength, dTempValue))
	{
		CString strMsg = _T("");
		CString strTemp = _LS(IDS_CMD_TLGT_LENGTH);
		strMsg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE), strTemp);
		AfxMessageBox(strMsg);
		m_edtLength.SetFocus(); // Focus 옮기기
		m_edtLength.SetSel(0, -1, TRUE); // 하이라이트 주기
		return;
	}

	if (!CStrParser::GetFloatNumber(strForce, dTempValue))
	{
		CString strMsg = _T("");
		CString strTemp = _LS(IDS_CMD_TLGT_FORCE);
		strMsg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE), strTemp);
		AfxMessageBox(strMsg);
		m_edtForce.SetFocus(); // Focus 옮기기
		m_edtForce.SetSel(0, -1, TRUE); // 하이라이트 주기
		return;
	}

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	if (!pos)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_modif));
		return;
	}

	int nItem = m_List.GetNextSelectedItem(pos);

	TLGT_ITEM item;
	item.dLength = m_edtLength.GetEditValue();
	item.dForce = m_edtForce.GetEditValue();

	if (item.dLength < 0.0)
	{
		CString strMsg = _T("");
		CString strTemp = _LS(IDS_CMD_TLGT_LENGTH);
		strMsg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO_AND_EQUAL), strTemp);
		AfxMessageBox(strMsg);
		return;
	}

	m_aItem.SetAt(nItem, item);
	if (m_nTrainCode == TRAINCODE_USER)
		m_aUserItem.SetAt(nItem, item);

	OnUpdateList();
}

void CCMTlgtDlg::OnCmdTrainloadDelete()
{
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	if (!pos)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
		return;
	}

	int nItem = m_List.GetNextSelectedItem(pos);

	m_aItem.RemoveAt(nItem);
	if (m_nTrainCode == TRAINCODE_USER)
		m_aUserItem.RemoveAt(nItem);

	OnUpdateList();
}

void CCMTlgtDlg::OnCmdTrainloadInsert()
{
	double dTempValue; // error check
	CString strLength, strForce;
	m_edtLength.GetWindowText(strLength);
	m_edtForce.GetWindowText(strForce);
	if (!CStrParser::GetFloatNumber(strLength, dTempValue))
	{
		CString strMsg = _T("");
		CString strTemp = _LS(IDS_CMD_TLGT_LENGTH);
		strMsg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE), strTemp);
		AfxMessageBox(strMsg);
		m_edtLength.SetFocus(); // Focus 옮기기
		m_edtLength.SetSel(0, -1, TRUE); // 하이라이트 주기
		return;
	}

	if (!CStrParser::GetFloatNumber(strForce, dTempValue))
	{
		CString strMsg = _T("");
		CString strTemp = _LS(IDS_CMD_TLGT_FORCE);
		strMsg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE), strTemp);
		AfxMessageBox(strMsg);
		m_edtForce.SetFocus(); // Focus 옮기기
		m_edtForce.SetSel(0, -1, TRUE); // 하이라이트 주기
		return;
	}

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	if (!pos)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_inser));
		return;
	}

	int nItem = m_List.GetNextSelectedItem(pos);

	TLGT_ITEM item;
	item.dLength = m_edtLength.GetEditValue();
	item.dForce = m_edtForce.GetEditValue();

	if (item.dLength < 0.0)
	{
		CString strMsg = _T("");
		CString strTemp = _LS(IDS_CMD_TLGT_LENGTH);
		strMsg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO_AND_EQUAL), strTemp);
		AfxMessageBox(strMsg);
		return;
	}

	m_aItem.InsertAt(nItem, item);
	if (m_nTrainCode == TRAINCODE_USER)
		m_aUserItem.InsertAt(nItem, item);

	OnUpdateList();
}

void CCMTlgtDlg::OnCmdOpenBtn()
{
	if (!m_pDStore->ReadFromFile(m_tlgtInfo)) return;
	m_wndGrid.Initialize();
	Data2Dlg();
}

void CCMTlgtDlg::OnCmdSaveBtn()
{
	if (!m_pDoc->AllowCommand(D_OPTN_ID_HISTORY))
	{
		AfxMessageBox(_LS(IDS_DB_NO_LICENSE_HISTORY));
		return;
	}

	if (!Dlg2Data()) return;
	if (!m_pDStore->SaveToFile(m_tlgtInfo)) return;
}

void CCMTlgtDlg::OnCmdPreviewBtn()
{
#if defined(_CIVIL_ORG)
	if (!m_pDoc->CheckCivilKRMODS())
	{
		return;
	}
#endif

	TLGT_INFO tTlgtInfo;

	// Node Array
	int nNodeCount = m_wndGrid.GetRowCount();
	if (nNodeCount < 3)
	{
		AfxMessageBox(_LS(IDS_CMD_ERR_TLGT_NODE_NUM));
		return;
	}
	tTlgtInfo.arKeyNode.RemoveAll();
	for (int i = 0; i < nNodeCount; i++)
	{
		T_NODE_K NodeKey = _ttoi(m_wndGrid.GetValueRowCol(i + 1, 1));
		tTlgtInfo.arKeyNode.Add(NodeKey);
	}

	// Number of Wheels
	if (m_edtNumWheels.GetEditValue() < 1)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_WHEEL));
		AfxMessageBox(msg);
		return;
	}
	tTlgtInfo.nNumWheel = m_edtNumWheels.GetEditValue();

	// Number of Wheels
	if (m_edtNumWVehicle.GetEditValue() < 1)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_VEHICLE));
		AfxMessageBox(msg);
		return;
	}
	tTlgtInfo.nNumVehicle = m_edtNumWVehicle.GetEditValue();

	// Train Velocity
	if (m_edtSpeed.GetEditValue() < 1e-13)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_VELOCITY));
		AfxMessageBox(msg);
		return;
	}
	tTlgtInfo.dVelocity = m_edtSpeed.GetEditValue();

	// Scaling
	int nScale;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlScale, nScale);
	tTlgtInfo.nScaling = nScale;
	if (nScale == 0) // Scale Factor
	{
		tTlgtInfo.dSacleFactor = m_edtScale.GetEditValue();
		if (fabs(tTlgtInfo.dSacleFactor) < 1e-13)
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_SCALING_SCALE_FACTOR));
			AfxMessageBox(msg);
			return;
		}
	}
	else // Maximum Value
	{
		tTlgtInfo.dMaximumValue = m_edtMax.GetEditValue();
		if (fabs(tTlgtInfo.dMaximumValue) < 1e-13)
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO), _LS(IDS_CMD_TLGT_SCALING_MAX_VALUE));
			AfxMessageBox(msg);
			return;
		}
	}

	// Start Time
	if (m_edtTime.GetEditValue() < 0)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_INVALID_VALUE_ZERO_AND_EQUAL), _LS(IDS_CMD_TLGT_TIME_START));
		AfxMessageBox(msg);
		return;
	}
	tTlgtInfo.dTime = m_edtTime.GetEditValue();

	// Direction
	DWORD dwCobxItemData = 0;
	int nResult = CDlgUtil::CobxGetItemDataByCurSel(m_cmbDir, dwCobxItemData);
	if (nResult == CB_ERR)
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_ERR_NO_SELECTED), _LS(IDS_CMD_TLGT_DIRECTION));
		AfxMessageBox(msg);
		return;
	}
	tTlgtInfo.nDir = dwCobxItemData;

	// Train Load
	TLGT_ITEM tlgtItem;
	tTlgtInfo.arTlgtItem.RemoveAll();
	for (long i = 0; i < m_aItem.GetSize(); i++)
	{
		tlgtItem.Initialize();
		tlgtItem.dLength = m_aItem.GetData()[i].dLength;
		tlgtItem.dForce = m_aItem.GetData()[i].dForce;
		tTlgtInfo.arTlgtItem.Add(tlgtItem);
	}

	CCMTlgtPreviewDlg dlg(m_pDoc, m_pDStore, this);
	dlg.SetData(tTlgtInfo);
	dlg.DoModal();
	
	return;
}

void CCMTlgtDlg::OnCmdChangedTrainType()
{
    m_aItem.RemoveAll();

    m_nTrainType = GetCurTrainType();
	m_nVehcleNum = m_edtNumWVehicle.GetEditValue();

    switch ( m_nTrainType )
    {
    case TRAINTYPE_EUROCODE_B:
        {
            EnableTrainTypeOptBtn(TRUE);
            SetFocusTrainTypeOptBtn(TRUE);

            if ( !GetDataEUTrainTypeB(m_aItem) )
            {
                ASSERT(0); return;
            }
        }
        break;
    default:
        {
            EnableTrainTypeOptBtn(FALSE);

            GetDataDefault(m_nTrainCode, m_nTrainType, m_nVehcleNum,  m_aUserItem, m_aItem);
        }
        break;
    }

    OnUpdateList();

    return;
}

void CCMTlgtDlg::OnCmdVehicleBtn()
{
	OnCmdChangedTrainType();
}

void CCMTlgtDlg::OnCmdClickTrainTypeOptBtn()
{
    const auto& CurCode = GetCurTrainCode();
    const auto& CurType = GetCurTrainType();

    switch ( CurCode )
    {
    case TRAINCODE_EUROCODE:
        {
            switch ( CurType )
            {
            case TRAINTYPE_EUROCODE_B:
                {
                    if ( !GetDataEUTrainTypeB(m_aItem) )
                    {
                        ASSERT(0); return;
                    }
                }
                break;
            default: ASSERT(0); return;
            }
        }
        break;
    default: ASSERT(0); return;
    }

    OnUpdateList();

    return;
}

BOOL CCMTlgtDlg::ExcelImport(CString csFileName, CArray<double>& arValLength, CArray<double>& aValForce)
{
	if (!m_pXL)
	{
		m_pXL = new CMSExcel;
	}
	else
	{
		delete m_pXL;
		m_pXL = new CMSExcel;
	}

	if (m_pXL == NULL) { AfxMessageBox(_T("NULL"));  return FALSE; }
	if (!m_pXL->IsExcel()) { AfxMessageBox(_LS(IDS_TB_NOT_EXCEL_APP));  return FALSE; }
	
	
	CStringArray aSheetsName;

	m_pXL->Open(csFileName);
	m_pXL->GetSheetsName(aSheetsName);

	int nSheetCount = aSheetsName.GetSize();

	if (nSheetCount <= 0)
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_TB_NOT_EXCEL_APP));  return FALSE;
	}

	m_pXL->SetActiveSheet(aSheetsName[0]);

	CString strCell;
	CString strCellNo;
	for (int i = 0; i <= 10000; i++)
	{
		double dID = m_pXL->GetValue(i, 0);
		if(i == 0 && dID == 0.0)
		{ 
			AfxMessageBox(_T("Invalid Data Format"));
		}
		else if (dID == 0.0)
		{
			break;
		}
		//strCell = m_pXL->GetCell(i, 1);
		double dBRow = m_pXL->GetValue(i, 1);
		//strCell = m_pXL->GetCell(i, 2);
		double dCRow = m_pXL->GetValue(i, 2);
		/*
		if (B.IsEmpty() || C.IsEmpty())
		{ 
			if (i == 0)
			{
				AfxMessageBox(_T("Invalid Data Format"));
			}
			break;
		}			

		double dBRow = _tstof(B);
		double dCRow = _tstof(C);
		*/
		arValLength.Add(dBRow);
		aValForce.Add(dCRow);
	}

	m_pXL->Quit();
	return TRUE;
}

void CCMTlgtDlg::AutoAdjustComboWidth(CComboBox& Combo)
{
	if (!Combo.GetSafeHwnd() || !::IsWindow(Combo.GetSafeHwnd()))
	{
		ASSERT(FALSE);
		return;
	}

	CDC& ComboDC = *Combo.GetDC();
	const int RestorePoint = ComboDC.SaveDC();
	CFont& ComboFont = *Combo.GetFont();

	ComboDC.SelectObject(&ComboFont);

	int MaxX = -1;

	const UINT ItemCount = Combo.GetCount();
	for (UINT Index = 0; Index < ItemCount; ++Index)
	{
		CString Text;
		Combo.GetLBText(Index, Text);

		// Get width of item string.
		const SIZE sz = ComboDC.GetTextExtent(Text);

		if (sz.cx > MaxX)
		{
			MaxX = sz.cx;
		}
	}// End for

	if (MaxX > 0)
	{
		const int ScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
		const int Border = ::GetSystemMetrics(SM_CXEDGE) * 2;
		Combo.SetDroppedWidth(MaxX + ScrollWidth * 2);
	}

	ComboDC.RestoreDC(RestorePoint);
}

void CCMTlgtDlg::EnableTrainTypeOptBtn(BOOL bEnable)
{
    return CDlgUtil::CtrlEnableDisable(this, IDC_ETC_TLGT_TRAIN_TYPE_OPT_BTN, bEnable);
}

void CCMTlgtDlg::SetFocusTrainTypeOptBtn(BOOL bFocus)
{
    GetDlgItem(IDC_ETC_TLGT_TRAIN_TYPE_OPT_BTN)->SetFocus();
}

void CCMTlgtDlg::GetDataDefault(int nCode, 
                                int nType, 
								int nVehcleNum,
                                ARR_TLGT_ITEM& raUser, 
                                ARR_TLGT_ITEM& raItem)
{
    return m_pDStore->GetDefaultData(nCode, nType, nVehcleNum, raUser, raItem);
}

bool CCMTlgtDlg::GetDataEUTrainTypeB(ARR_TLGT_ITEM& raItem)
{
    CCMTlgtEurocodeTrainTypeBDlg TypeBDlg(m_pDoc, this);
    
    if ( !TypeBDlg.SetSuperControl(GetDlgItem(IDC_ETC_TLGT_TRAIN_TYPE_OPT_BTN)) )
    {
        ASSERT(0); return false;
    }

    if ( TypeBDlg.DoModal() == IDOK )
    {
        auto unForceNumber = 0U;
        auto dSpace = 0.0;

        if ( !TypeBDlg.GetOptionValueNU(unForceNumber, dSpace) )
        {
            ASSERT(0); return false;
        }

        if ( !m_pDStore->GetDataEUTrainTypeB(unForceNumber, dSpace, raItem) )
        {
            ASSERT(0); return false;
        }
    }

    return true;
}

TRAIN_CODE CCMTlgtDlg::GetCurTrainCode()
{
    auto CurData = DWORD();
    if ( !CDlgUtil::CobxGetItemDataByCurSel(m_cmbTrainCode, CurData) )
    {
        ASSERT(0); return TRAIN_CODE();
    }

    return static_cast<TRAIN_CODE>(CurData);
}

TRAIN_TYPE CCMTlgtDlg::GetCurTrainType()
{
    auto CurData = DWORD();
    if ( !CDlgUtil::CobxGetItemDataByCurSel(m_cmbTrainType, CurData) )
    {
        ASSERT(0); return TRAIN_TYPE();
    }

    return static_cast<TRAIN_TYPE>(CurData);
}

void CCMTlgtDlg::OnCmdExcelImport()
{
	CFileDialog fd(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST | OFN_LONGNAMES,
		_T("Excel Files(*.xls,*.xlsx)|*.xls;*.xlsx||"), NULL);

	if (fd.DoModal() == IDOK)
	{
		CString csFileName = fd.GetPathName();

		CArray<double> arValLength, aValForce;
		if(!ExcelImport(csFileName, arValLength, aValForce))
			return;

		if (arValLength.GetCount() != aValForce.GetCount())
			return;

		m_aItem.RemoveAll();
		for (long i = 0; i < arValLength.GetCount(); i++)
		{
			TLGT_ITEM tlgtItem;
			tlgtItem.dLength = arValLength[i];
			tlgtItem.dForce = aValForce[i];
			m_aItem.Add(tlgtItem);
			m_aUserItem.Add(tlgtItem);
		}
		OnUpdateList();
	}
}

void CCMTlgtDlg::OnClickedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nItem = m_List.GetNextSelectedItem(pos);

		CStringArray Contents;
		DWORD dwItem;
		CDlgUtil::GetListItem(&m_List, nItem, Contents, dwItem);
		CString strItem;
		double dItem;

		strItem = m_List.GetItemText(nItem, 1);
		dItem = _tstof(strItem);
		m_edtLength.SetEditUnit(dItem);

		strItem = m_List.GetItemText(nItem, 2);
		dItem = _tstof(strItem);
		m_edtForce.SetEditUnit(dItem);
	}
}

void CCMTlgtDlg::OnCmdBtnClose()
{
	DestroyWindow();
}

void CCMTlgtDlg::OnCmdChangedTrainCode()
{
	DWORD dwCobxItemData = 0;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbTrainCode, dwCobxItemData);

	m_nTrainCode = dwCobxItemData;

	if (dwCobxItemData == TRAINCODE_USER)
	{
		CDlgUtil::CtrlEnableDisable(this, IDC_ETC_TLGT_TRAIN_TYPE_CMB, FALSE);
		CDlgUtil::CtrlEnableDisable(this, IDC_ETC_TLGT_IMPORT_EXCEL_BTN, TRUE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, IDC_ETC_TLGT_TRAIN_TYPE_CMB, TRUE);
		CDlgUtil::CtrlEnableDisable(this, IDC_ETC_TLGT_IMPORT_EXCEL_BTN, FALSE);
	}
	ShowHideByCode();
	SetVehicleTypeList(m_nTrainCode);
	m_cmbTrainType.SetCurSel(0);
	OnCmdChangedTrainType();
}

void CCMTlgtDlg::OnCmdLoadcaseBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_HISTORY_LOADCASE, 0));
}

BOOL CCMTlgtDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		CWnd* pWnd = GetFocus();
		if (pWnd && pWnd->GetSafeHwnd() && IsWindow(pWnd->GetSafeHwnd()) &&
			pMsg->wParam == VK_DELETE && pWnd->GetDlgCtrlID() == IDC_CMD_PLACEHOLDER)
		{
			OnCmdOpDelete();
			return TRUE;
		}
	}
	return CCMDlgBase::PreTranslateMessage(pMsg);
}