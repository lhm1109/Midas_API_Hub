// CSeisCvlJpSetTargetLowerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetTargetLowerDlg.h"
#include "SeisCvlJpSetTargetDlg.h"
#include "SeisSigrLowerAutoDlg.h"

#include "..\wg_base\PL_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LIST_COLUMN_NUM_BEAR	5
#define LIST_COLUMN_NUM_COLM	4
#define LIST_COLUMN_NUM_BEAM	3
#define LIST_COLUMN_NUM_FOOT	3
#define LIST_COLUMN_NUM_LOWER	5

#define LIST_SIGR_NAME				_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_SIGR_NAME)
#define LIST_SIGR_BEARING			_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_SIGR_BEARING)
#define LIST_SIGR_COLUMN			_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_SIGR_COLUMN)
#define LIST_SIGR_BEAM				_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_SIGR_BEAM)
#define LIST_SIGR_FOOTING			_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_SIGR_FOOTING)
#define COMBO_MATERIAL_TYPE_RC		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_MATERIAL_TYPE_RC)
#define COMBO_MATERIAL_TYPE_ST		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_MATERIAL_TYPE_ST)
#define COMBO_MATERIAL_TYPE_SRC		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_MATERIAL_TYPE_SRC)
#define COMBO_MEMBER_TYPE_BEAR		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_MEMBER_TYPE_BEAR)
#define COMBO_MEMBER_TYPE_COLM		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_MEMBER_TYPE_COLM)
#define COMBO_MEMBER_TYPE_BEAM		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_MEMBER_TYPE_BEAM)
#define COMBO_MEMBER_TYPE_FOOT		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_MEMBER_TYPE_FOOT)
#define COMBO_SHEAR_AXIS_DIR_X		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_SHEAR_AXIS_DIR_X)
#define COMBO_SHEAR_AXIS_DIR_Y		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_SHEAR_AXIS_DIR_Y)
#define COMBO_SHEAR_AXIS_DIR_Z		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_SHEAR_AXIS_DIR_Z)
#define COMBO_SHEAR_AXIS_DIR_YZ		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_SHEAR_AXIS_DIR_YZ)
#define COMBO_BEAR_TYPE_RIGID_LINK	_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_BEAR_TYPE_RIGID_LINK)
#define COMBO_BEAR_TYPE_FIXED		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_BEAR_TYPE_FIXED)
#define COMBO_BEAR_TYPE_MOVING		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_BEAR_TYPE_MOVING)
#define COMBO_BEAR_TYPE_ELASTIC		_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_BEAR_TYPE_ELASTIC)
#define COMBO_BEAR_TYPE_VIBRA_ISOLA	_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_BEAR_TYPE_VIBRA_ISOLA)
#define LIST_HEAD_NAME				_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_HEAD_NAME)
#define LIST_HEAD_NODE				_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_HEAD_NODE)
#define LIST_HEAD_ELEMENT			_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_HEAD_ELEMENT)
#define LIST_HEAD_SECT_DIR			_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_HEAD_SECT_DIR)
#define LIST_HEAD_SHER_DIR			_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_HEAD_SHER_DIR)
#define LIST_HEAD_BEAR_LINE			_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_HEAD_BEAR_LINE)
#define LIST_HEAD_BEAR_TYPE			_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_HEAD_BEAR_TYPE)
#define LIST_HEAD_REBAR_CUT			_LS(IDS_IDD_TM_SIGR_LOWER_IDC_LIST_HEAD_REBAR_CUT)

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetLowerDlg dialog


CSeisCvlJpSetTargetLowerDlg::CSeisCvlJpSetTargetLowerDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlJpSetTargetLowerDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	m_nAutoRdo = 0;
	m_pParent = (CSeisCvlJpSetTargetDlg*)pParent;

	m_aBearingCtrl.RemoveAll();
	m_aBearingCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_LINE_STC);
	m_aBearingCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_LINE_CMB);
	m_aBearingCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_TYPE_STC);
	m_aBearingCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_TYPE_CMB);

	m_aColumnCtrl.RemoveAll();
	m_aColumnCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_REBAR_CUT_CHK);
	m_aColumnCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_REBAR_CUT_GRP);
	m_aColumnCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_STC);
	m_aColumnCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_EDT);
	m_aColumnCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_CMB);
	m_aColumnCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_STC);
	m_aColumnCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_EDT);
	m_aColumnCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_CMB);

	m_aBottunCtrl.RemoveAll();
	m_aBottunCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_ADD_BTN);
	m_aBottunCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_MOD_BTN);
	m_aBottunCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_DEL_BTN);

	m_aBearingListCtrl.RemoveAll();
	m_aBearingListCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_LST);

	m_aColumnListCtrl.RemoveAll();
	m_aColumnListCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_COLUMN_LST);

	m_aBeamListCtrl.RemoveAll();
	m_aBeamListCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEAM_LST);

	m_aFootingListCtrl.RemoveAll();
	m_aFootingListCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_FOOTING_LST);

	m_aMemberElemCtrl.RemoveAll();
	m_aMemberElemCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_EDT);
	m_aMemberNodeCtrl.RemoveAll();
	m_aMemberNodeCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_NODE_EDT);

	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_CMB);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_NAME_EDT);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_EDT);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_NODE_EDT);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_DIR_CMB);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_REBAR_CUT_CHK);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_EDT);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_CMB);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_EDT);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_CMB);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_LINE_CMB);
	m_aAutoCtrl.Add(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_TYPE_CMB);
}

CSeisCvlJpSetTargetLowerDlg::~CSeisCvlJpSetTargetLowerDlg()
{
	m_pDoc = NULL;
}

void CSeisCvlJpSetTargetLowerDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Radio(pDX, IDC_TM_SIGR_LOWER_ADD_RDO, m_nOption);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_NAME_EDT, m_name);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_POS_AXIS_EDT, m_nodeAxis);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_POS_PERP_EDT, m_nodePerp);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MAT_CMB, m_materialType);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_CMB, m_memberType);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_NAME_EDT, m_memberName);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_EDT, m_membersElem);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_NODE_EDT, m_membersNode);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_DIR_CMB, m_shearAxisDir);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_LINE_CMB, m_bearingLine);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_TYPE_CMB, m_bearingType);
	DDX_Check(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_REBAR_CUT_CHK, m_rebarCut);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_EDT, m_axisElem);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_EDT, m_perAxisElem);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_CMB, m_axisEndRle);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_CMB, m_perAxisEndRle);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_LST, m_bearingList);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_COLUMN_LST, m_columnList);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_BEAM_LST, m_beamList);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_MEMB_TYPE_FOOTING_LST, m_footingList);
	DDX_Control(pDX, IDC_TM_SIGR_LOWER_LST, m_sigrLowerList);
	DDX_Radio(pDX, IDC_TM_SIGR_LOWER_INPUT_TYPE1_RDO, m_nAutoRdo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetTargetLowerDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlJpSetTargetLowerDlg)
	ON_CBN_SELCHANGE(IDC_TM_SIGR_LOWER_MAT_CMB, OnSelectedChangedMaterialTypeCmb)
	ON_CBN_SELCHANGE(IDC_TM_SIGR_LOWER_MEMB_TYPE_CMB, OnSelectedChangedMemberTypeCmb)
	ON_EN_SETFOCUS(IDC_TM_SIGR_LOWER_POS_AXIS_EDT, OnSetFocusNodeAxisEdt)
	ON_EN_SETFOCUS(IDC_TM_SIGR_LOWER_POS_PERP_EDT, OnSetFocusNodePerpEdt)
	ON_EN_SETFOCUS(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_EDT, OnSetFocusNodeAndElemEdt)
	ON_EN_SETFOCUS(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_NODE_EDT, OnSetFocusNodeAndElemEdt)
	ON_EN_SETFOCUS(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_EDT, OnSetFocusAxisEdt)
	ON_EN_SETFOCUS(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_EDT, OnSetFocusPerAxisEdt)
	ON_BN_CLICKED(IDC_TM_SIGR_LOWER_MEMB_TYPE_REBAR_CUT_CHK, OnBnClickedCheck)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_LST, OnNMClickBearingList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SIGR_LOWER_MEMB_TYPE_COLUMN_LST, OnNMClickColumnList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SIGR_LOWER_MEMB_TYPE_BEAM_LST, OnNMClickBeamList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SIGR_LOWER_MEMB_TYPE_FOOTING_LST, OnNMClickFootingList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SIGR_LOWER_LST, OnNMClickSigrLowerList)
	ON_BN_CLICKED(IDC_TM_SIGR_LOWER_MEMB_TYPE_ADD_BTN, OnAddBtn)
	ON_BN_CLICKED(IDC_TM_SIGR_LOWER_MEMB_TYPE_MOD_BTN, OnModBtn)
	ON_BN_CLICKED(IDC_TM_SIGR_LOWER_MEMB_TYPE_DEL_BTN, OnDelBtn)
	ON_BN_CLICKED(IDC_TM_SIGR_LOWER_INPUT_TYPE1_RDO, OnChangeAutoRdo)
	ON_BN_CLICKED(IDC_TM_SIGR_LOWER_INPUT_TYPE2_RDO, OnChangeAutoRdo)
	ON_BN_CLICKED(IDC_TM_SIGR_LOWER_AUTO_FIND_BTN, OnClickAutoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSeisCvlJpSetTargetLowerDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	MoveBearingCtrl();
	InitControl();
	InitListControls();
	return TRUE;// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlJpSetTargetLowerDlg::Execute()
{
	UpdateData(TRUE);

	BOOL bModify = FALSE;
	int nItem = m_sigrLowerList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem != -1) bModify = TRUE;

	T_SIGR_LOWER_K sigrLowerK;
	if (nItem != -1)
		sigrLowerK = (T_SIGR_LOWER_K)m_sigrLowerList.GetItemData(nItem);

	BOOL bResult = FALSE;
	if (m_nOption == 0) {
		
		if (bModify) {
			bResult = ModData(sigrLowerK);
		}
		else {//Add
			bResult = AddData();
		}
	}
	else {//Delete
		bResult = DelData(sigrLowerK);
	}
}

void CSeisCvlJpSetTargetLowerDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
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
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		break;
	default: ASSERT(TRUE);
	}
}

void CSeisCvlJpSetTargetLowerDlg::OnSelectedChangedMaterialTypeCmb()
{
	UpdateData(TRUE);

	//교각의 재료 타입
	CString str;
	m_materialType.GetWindowText(str);
	
	BOOL bSteel = (str == COMBO_MATERIAL_TYPE_ST);
	if (bSteel == TRUE) {
		CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_REBAR_CUT_CHK);
		pBtn->SetCheck(FALSE);
	}
	
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_REBAR_CUT_CHK)->EnableWindow(!bSteel);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_EDT)->EnableWindow(!bSteel);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_CMB)->EnableWindow(!bSteel);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_EDT)->EnableWindow(!bSteel);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_CMB)->EnableWindow(!bSteel);
}

void CSeisCvlJpSetTargetLowerDlg::OnSelectedChangedMemberTypeCmb()
{
	UpdateControlByMemberType();
	OnSetFocusNodeAndElemEdt();
	ResetMemberControls();
}

void CSeisCvlJpSetTargetLowerDlg::OnSetFocusNodeAxisEdt()
{
	m_nodePerp.Disconnect();
	m_membersElem.Disconnect();
	m_membersElem.Invalidate(FALSE);
	m_membersNode.Disconnect();
	m_axisElem.Disconnect();
	m_perAxisElem.Disconnect();

	CString strPreviousText = _T("");
	m_nodeAxis.GetWindowText(strPreviousText);

	m_nodeAxis.Connect(CURSOR_ID_NODE);

	ResetViewControls();

	CArray<T_NODE_K, T_NODE_K> aNodes;
	m_nodeAxis.SetWindowText(strPreviousText);
	GetNodeList(strPreviousText, aNodes);
	m_pDoc->m_pViewCtrl->SelectNode(NULL, aNodes, FALSE);
}

void CSeisCvlJpSetTargetLowerDlg::OnSetFocusNodePerpEdt()
{
	m_nodeAxis.Disconnect();
	m_membersElem.Disconnect();
	m_membersElem.Invalidate(FALSE);
	m_membersNode.Disconnect();
	m_axisElem.Disconnect();
	m_perAxisElem.Disconnect();

	CString strPreviousText = _T("");
	m_nodePerp.GetWindowText(strPreviousText);

	m_nodePerp.Connect(CURSOR_ID_NODE);

	ResetViewControls();

	CArray<T_NODE_K, T_NODE_K> aNodes;
	m_nodePerp.SetWindowText(strPreviousText);
	GetNodeList(strPreviousText, aNodes);
	m_pDoc->m_pViewCtrl->SelectNode(NULL, aNodes, FALSE);
}

void CSeisCvlJpSetTargetLowerDlg::OnSetFocusNodeAndElemEdt()
{
	m_nodeAxis.Disconnect();
	m_nodePerp.Disconnect();
	m_axisElem.Disconnect();
	m_perAxisElem.Disconnect();

	SetMemberCmbConnectType();
}

void CSeisCvlJpSetTargetLowerDlg::OnSetFocusAxisEdt()
{
	m_nodeAxis.Disconnect();
	m_nodePerp.Disconnect();
	m_membersElem.Disconnect();
	m_membersElem.Invalidate(FALSE);
	m_membersNode.Disconnect();
	m_perAxisElem.Disconnect();

	CString strPreviousText = _T("");
	m_axisElem.GetWindowText(strPreviousText);

	m_axisElem.Connect(CURSOR_ID_ELEM);

	ResetViewControls();

	CArray<T_ELEM_K, T_ELEM_K> aElements;
	m_axisElem.SetWindowText(strPreviousText);
	GetNodeList(strPreviousText, aElements);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aElements, FALSE);
}

void CSeisCvlJpSetTargetLowerDlg::OnSetFocusPerAxisEdt()
{
	m_nodeAxis.Disconnect();
	m_nodePerp.Disconnect();
	m_membersElem.Disconnect();
	m_membersElem.Invalidate(FALSE);
	m_membersNode.Disconnect();
	m_axisElem.Disconnect();

	CString strPreviousText = _T("");
	m_perAxisElem.GetWindowText(strPreviousText);

	m_perAxisElem.Connect(CURSOR_ID_ELEM);

	ResetViewControls();

	CArray<T_ELEM_K, T_ELEM_K> aElements;
	m_perAxisElem.SetWindowText(strPreviousText);
	GetNodeList(strPreviousText, aElements);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aElements, FALSE);
}

void CSeisCvlJpSetTargetLowerDlg::OnBnClickedCheck()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_EDT)->EnableWindow(m_rebarCut);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_CMB)->EnableWindow(m_rebarCut);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_EDT)->EnableWindow(m_rebarCut);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_CMB)->EnableWindow(m_rebarCut);
}

void CSeisCvlJpSetTargetLowerDlg::OnNMClickBearingList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1) {
		*pResult = 0;
		return;
	}

	int nItem = m_bearingList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		ResetMemberControls();
		*pResult = 0;
		return;
	}

	nItem = pNMListView->iItem;
	
	ResetViewControls();
	ResetMemberControls();
	UpdataDialog_BEAR(nItem);
}

void CSeisCvlJpSetTargetLowerDlg::OnNMClickColumnList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1) {
		*pResult = 0;
		return;
	}

	int nItem = m_columnList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		ResetMemberControls();
		*pResult = 0;
		return;
	}

	nItem = pNMListView->iItem;

	ResetViewControls();
	ResetMemberControls();
	UpdataDialog_COLM(nItem);
	OnBnClickedCheck();
}

void CSeisCvlJpSetTargetLowerDlg::OnNMClickBeamList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1) {
		*pResult = 0;
		return;
	}

	int nItem = m_beamList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		ResetMemberControls();
		*pResult = 0;
		return;
	}

	nItem = pNMListView->iItem;

	ResetViewControls();
	ResetMemberControls();
	UpdataDialog_BEAM(nItem);
}

void CSeisCvlJpSetTargetLowerDlg::OnNMClickFootingList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1) {
		*pResult = 0;
		return;
	}

	int nItem = m_footingList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		ResetMemberControls();
		*pResult = 0;
		return;
	}

	nItem = pNMListView->iItem;

	ResetViewControls();
	ResetMemberControls();
	UpdataDialog_FOOT(nItem);
}

void CSeisCvlJpSetTargetLowerDlg::OnNMClickSigrLowerList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1) {
		*pResult = 0;
		return;
	}

	int nItem = m_sigrLowerList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		*pResult = 0;
		return;
	}

	nItem = pNMListView->iItem;
	T_SIGR_LOWER_K key = (T_SIGR_LOWER_K)m_sigrLowerList.GetItemData(nItem);

	T_SIGR_LOWER_D data;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(key, data) == FALSE)
		return;

	ResetViewControls();
	ResetMemberControls();
	UpdateDialog(data);
}

void CSeisCvlJpSetTargetLowerDlg::OnAddBtn()
{
	UpdateData(TRUE);

	CString str = _T("");
	m_memberType.GetWindowText(str);

	BOOL bResult = FALSE;
	if (str == COMBO_MEMBER_TYPE_BEAR)
		bResult = AddBtnBearing();
	else if (str == COMBO_MEMBER_TYPE_COLM)
		bResult = AddBtnColumn();
	else if (str == COMBO_MEMBER_TYPE_BEAM)
		bResult = AddBtnBeam();
	else if (str == COMBO_MEMBER_TYPE_FOOT)
		bResult = AddBtnFooting();

	if (bResult == FALSE)
		return;

	ResetViewControls();
	ResetMemberControls();
}

void CSeisCvlJpSetTargetLowerDlg::OnModBtn()
{
	UpdateData(TRUE);

	CString str = _T("");
	m_memberType.GetWindowText(str);

	if (str == COMBO_MEMBER_TYPE_BEAR)
		ModBtnBearing();
	else if (str == COMBO_MEMBER_TYPE_COLM)
		ModBtnColumn();
	else if (str == COMBO_MEMBER_TYPE_BEAM)
		ModBtnBeam();
	else if (str == COMBO_MEMBER_TYPE_FOOT)
		ModBtnFooting();
}

void CSeisCvlJpSetTargetLowerDlg::OnDelBtn()
{
	UpdateData(TRUE);

	CString str = _T("");
	m_memberType.GetWindowText(str);

	if (str == COMBO_MEMBER_TYPE_BEAR)
		DelBtnBearing();
	else if (str == COMBO_MEMBER_TYPE_COLM)
		DelBtnColumn();
	else if (str == COMBO_MEMBER_TYPE_BEAM)
		DelBtnBeam();
	else if (str == COMBO_MEMBER_TYPE_FOOT)
		DelBtnFooting();
}

void CSeisCvlJpSetTargetLowerDlg::OnChangeAutoRdo()
{
	UpdateData(TRUE);

	BOOL bEnable = (m_nAutoRdo == 0) ? TRUE : FALSE;
	if (bEnable == TRUE) {
		for (int i = 0; i < m_aAutoCtrl.GetCount(); i++) {
			GetDlgItem(m_aAutoCtrl.GetAt(i))->EnableWindow(FALSE);
		}
		GetDlgItem(IDC_TM_SIGR_LOWER_AUTO_FIND_BTN)->EnableWindow(TRUE);
		OnSetFocusNodeAndElemEdt();
	}
	else {
		for (int i = 0; i < m_aAutoCtrl.GetCount(); i++) {
			if(m_aAutoCtrl.GetAt(i) == IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_EDT || 
				m_aAutoCtrl.GetAt(i) == IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIAL_LOC_CMB ||
				m_aAutoCtrl.GetAt(i) == IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_EDT ||
				m_aAutoCtrl.GetAt(i) == IDC_TM_SIGR_LOWER_MEMB_TYPE_AXIS_PERP_CMB
				)
				continue;

			if (m_aAutoCtrl.GetAt(i) == IDC_TM_SIGR_LOWER_MEMB_TYPE_DIR_CMB)
			{
				CString strMember = _T("");
				m_memberType.GetWindowText(strMember);
				GetDlgItem(m_aAutoCtrl.GetAt(i))->EnableWindow(strMember != COMBO_MEMBER_TYPE_FOOT);
				continue;
			}
			GetDlgItem(m_aAutoCtrl.GetAt(i))->EnableWindow(TRUE);
		}
		GetDlgItem(IDC_TM_SIGR_LOWER_AUTO_FIND_BTN)->EnableWindow(FALSE);
	}
}

void CSeisCvlJpSetTargetLowerDlg::OnClickAutoDlg()
{
	CSeisSigrLowerAutoDlg dlg(this);
	if (dlg.DoModal() == IDOK)
	{ }
}
/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetLowerDlg diagnostics
//

void CSeisCvlJpSetTargetLowerDlg::MoveBearingCtrl()
{
	//Bearing 관련 컨트롤 패널영역(좌측)으로 이동
	CRect rTarget, rSource;
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_LINE_STC)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_REBAR_CUT_GRP)->GetWindowRect(rTarget);

	int nDistX = rTarget.left - rSource.left;
	CDlgUtil::CtrlMoveDistX(this, m_aBearingCtrl, nDistX);

	//Bearing 리스트 컨트롤 패널영역(좌측)으로 이동
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_LST)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_COLUMN_LST)->GetWindowRect(rTarget);

	nDistX = rTarget.left - rSource.left;
	CDlgUtil::CtrlMoveDistX(this, m_aBearingListCtrl, nDistX);

	//Beam 리스트 컨트롤 패널영역(좌측)으로 이동
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEAM_LST)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_COLUMN_LST)->GetWindowRect(rTarget);

	nDistX = rTarget.left - rSource.left;
	CDlgUtil::CtrlMoveDistX(this, m_aBeamListCtrl, nDistX);

	//Footing 리스트 컨트롤 패널영역(좌측)으로 이동
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_FOOTING_LST)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_COLUMN_LST)->GetWindowRect(rTarget);

	nDistX = rTarget.left - rSource.left;
	CDlgUtil::CtrlMoveDistX(this, m_aFootingListCtrl, nDistX);
}

void CSeisCvlJpSetTargetLowerDlg::InitControl()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SIGR_LOWER_ADD_RDO);
	pBtn->SetCheck(1);
	m_nOption = 0;

	m_materialType.ResetContent();
	m_materialType.AddString(COMBO_MATERIAL_TYPE_RC);
	m_materialType.AddString(COMBO_MATERIAL_TYPE_ST);
	m_materialType.AddString(COMBO_MATERIAL_TYPE_SRC);
	m_materialType.SetCurSel(0);

	m_memberType.ResetContent();
	m_memberType.AddString(COMBO_MEMBER_TYPE_BEAR);
	m_memberType.AddString(COMBO_MEMBER_TYPE_COLM);
	m_memberType.AddString(COMBO_MEMBER_TYPE_BEAM);
	m_memberType.AddString(COMBO_MEMBER_TYPE_FOOT);
	m_memberType.SetCurSel(0);

	//ResetMemberControls() 여기서 설정
// 	m_shearAxisDir.ResetContent();
// 	m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Y);
// 	m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Z);
// 	m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_YZ);
// 	m_shearAxisDir.SetCurSel(0);

	m_bearingLine.ResetContent();
	m_bearingLine.AddString(_T("1"));
	m_bearingLine.AddString(_T("2"));
	m_bearingLine.SetCurSel(0);

	m_bearingType.ResetContent();
	m_bearingType.AddString(COMBO_BEAR_TYPE_RIGID_LINK);
	m_bearingType.AddString(COMBO_BEAR_TYPE_FIXED);
	m_bearingType.AddString(COMBO_BEAR_TYPE_MOVING);
	m_bearingType.AddString(COMBO_BEAR_TYPE_ELASTIC);
	m_bearingType.AddString(COMBO_BEAR_TYPE_VIBRA_ISOLA);
	m_bearingType.SetCurSel(0);

	m_axisEndRle.ResetContent();
	m_axisEndRle.AddString(_T("I"));
	m_axisEndRle.AddString(_T("J"));
	m_axisEndRle.SetCurSel(0);

	m_perAxisEndRle.ResetContent();
	m_perAxisEndRle.AddString(_T("I"));
	m_perAxisEndRle.AddString(_T("J"));
	m_perAxisEndRle.SetCurSel(0);

	m_nodeAxis.SetCount(1);
	m_nodePerp.SetCount(1);
	m_axisElem.SetCount(1);
	m_perAxisElem.SetCount(1);
	m_membersNode.SetCount(2);

	UpdateControlByMemberType();
	ResetAllControls();
}

void CSeisCvlJpSetTargetLowerDlg::ResetAllControls()
{
	m_nAutoRdo = 0;
	m_name.SetWindowText(_T(""));
	m_nodeAxis.SetWindowText(_T(""));
	m_nodePerp.SetWindowText(_T(""));
	ResetViewControls();
	ResetMemberControls();
	ResetMemberListControls();
	ResetAutoMemberControls();
	UpdateData(FALSE);
}

void CSeisCvlJpSetTargetLowerDlg::ResetViewControls()
{
	m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CSeisCvlJpSetTargetLowerDlg::ResetMemberControls()
{
	CString strMember = _T("");
	m_memberType.GetWindowText(strMember);
	m_shearAxisDir.ResetContent();
	if (strMember == COMBO_MEMBER_TYPE_BEAR) {
		m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Y);
		m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Z);
	}
	else if (strMember == COMBO_MEMBER_TYPE_FOOT) {
		m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Z);
	}
	else {
		m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Y);
		m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Z);
		m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_YZ);
	}
	m_shearAxisDir.SetCurSel(0);
	m_shearAxisDir.EnableWindow(strMember != COMBO_MEMBER_TYPE_FOOT && m_nAutoRdo != 0);
	m_shearAxisDir.SetWindowText(_T(""));
	m_memberName.SetWindowText(_T(""));
	m_membersElem.SetWindowText(_T(""));
	m_axisElem.SetWindowText(_T(""));
	m_perAxisElem.SetWindowText(_T(""));
}

void CSeisCvlJpSetTargetLowerDlg::ResetMemberListControls()
{
	m_bearingList.DeleteAllItems();
	m_columnList.DeleteAllItems();
	m_beamList.DeleteAllItems();
	m_footingList.DeleteAllItems();
}

void CSeisCvlJpSetTargetLowerDlg::ResetAutoMemberControls()
{
	for (int i = 0; i < m_aAutoCtrl.GetCount(); i++) {
		GetDlgItem(m_aAutoCtrl.GetAt(i))->EnableWindow(m_nAutoRdo != 0);
	}
	GetDlgItem(IDC_TM_SIGR_LOWER_AUTO_FIND_BTN)->EnableWindow(m_nAutoRdo == 0);
}

void CSeisCvlJpSetTargetLowerDlg::InitListControls()
{
	SetHeaderTitleBearingListCtrl();
	SetHeaderTitleColumnListCtrl();
	SetHeaderTitleBeamListCtrl();
	SetHeaderTitleFootingListCtrl();
	SetHeaderTitleListCtrl();

	CArray<T_SIGR_LOWER_K, T_SIGR_LOWER_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetKeyList(keys);

	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIGR_LOWER_K& key = keys[i];
		T_SIGR_LOWER_D data;

		m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(key, data);
		InsertItemSigrLowerListCtrl(key, data);
	}
}

void CSeisCvlJpSetTargetLowerDlg::SetHeaderTitleListCtrl()
{
	CString aTitle[] = { LIST_SIGR_NAME, LIST_SIGR_BEARING, LIST_SIGR_COLUMN, LIST_SIGR_BEAM, LIST_SIGR_FOOTING };
	int nColWidth[] = { 40, 40, 40, 40, 40 };

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_sigrLowerList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_sigrLowerList.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < LIST_COLUMN_NUM_LOWER; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_sigrLowerList.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

void CSeisCvlJpSetTargetLowerDlg::SetHeaderTitleBearingListCtrl()
{
	CString aTitle[] = { LIST_HEAD_NAME, LIST_HEAD_NODE, LIST_HEAD_SECT_DIR, LIST_HEAD_BEAR_LINE, LIST_HEAD_BEAR_TYPE };
	int nColWidth[] = { 40, 60, 60, 50, 60 };

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_bearingList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_bearingList.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < LIST_COLUMN_NUM_BEAR; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_bearingList.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

void CSeisCvlJpSetTargetLowerDlg::SetHeaderTitleColumnListCtrl()
{
	CString aTitle[] = { LIST_HEAD_NAME, LIST_HEAD_ELEMENT, LIST_HEAD_SHER_DIR, LIST_HEAD_REBAR_CUT };
	int nColWidth[] = { 40, 60, 60, 92 };

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_columnList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_columnList.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < LIST_COLUMN_NUM_COLM; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_columnList.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

void CSeisCvlJpSetTargetLowerDlg::SetHeaderTitleBeamListCtrl()
{
	CString aTitle[] = { LIST_HEAD_NAME, LIST_HEAD_ELEMENT, LIST_HEAD_SHER_DIR };
	int nColWidth[] = { 40, 60, 60 };

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_beamList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_beamList.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < LIST_COLUMN_NUM_BEAM; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_beamList.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

void CSeisCvlJpSetTargetLowerDlg::SetHeaderTitleFootingListCtrl()
{
	CString aTitle[] = { LIST_HEAD_NAME, LIST_HEAD_ELEMENT, LIST_HEAD_SHER_DIR };
	int nColWidth[] = { 40, 60, 60 };

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_footingList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_footingList.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < LIST_COLUMN_NUM_FOOT; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_footingList.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

BOOL CSeisCvlJpSetTargetLowerDlg::InsertItemSigrLowerListCtrl(const T_SIGR_LOWER_K& Key, T_SIGR_LOWER_D& Data)
{
	int nItem = m_sigrLowerList.GetItemCount();
	for (int i = 0; i < LIST_COLUMN_NUM_LOWER; i++)
	{
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem = i;
		CString str = DataToStr(i, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_sigrLowerList.InsertItem(&lvitem);
		}
		else m_sigrLowerList.SetItem(&lvitem);

		str.ReleaseBuffer();
	}

	return TRUE;
}

CString CSeisCvlJpSetTargetLowerDlg::DataToStr_BEAR(const int& index, const T_SIGR_LOWER_BEAR_D& data)
{
	CString str = _T("");

	switch (index)
	{
	case 0://명칭
		str = data.strName;
		break;
	case 1://대상절점
		GetStrKeyList(data.aPosition, str);
		break;
	case 2://교축방향 전단력
		if (data.nDirShear == seis_jp::kY_Dir)
			str = COMBO_SHEAR_AXIS_DIR_Y;
		else if (data.nDirShear == seis_jp::kZ_Dir)
			str = COMBO_SHEAR_AXIS_DIR_Z;
		else if (data.nDirShear == seis_jp::kYZ_Dir)
			str = COMBO_SHEAR_AXIS_DIR_YZ;
		else { ASSERT(0); }
		break;
	case 3://지승선
		if (data.nLineNum == 1)
			str = _T("1");
		else if (data.nLineNum == 2)
			str = _T("2");
		else { ASSERT(0); }
		break;
	case 4://지승조건
		if (data.nType == T_SIGR_LOWER_BEAR_D::kRigidLink)
			str = COMBO_BEAR_TYPE_RIGID_LINK;
		else if (data.nType == T_SIGR_LOWER_BEAR_D::kFixed)
			str = COMBO_BEAR_TYPE_FIXED;
		else if (data.nType == T_SIGR_LOWER_BEAR_D::kMovable)
			str = COMBO_BEAR_TYPE_MOVING;
		else if (data.nType == T_SIGR_LOWER_BEAR_D::kElastic)
			str = COMBO_BEAR_TYPE_ELASTIC;
		else if (data.nType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation)
			str = COMBO_BEAR_TYPE_VIBRA_ISOLA;
		else { ASSERT(0); }
		break;
	default: ASSERT(0); break;
	}

	return str;
}

void GetRebarCutText(const T_SIGR_LOWER_COLM_D& data, CString& rStr)
{
	if (data.bRebarCut == FALSE) {
		rStr = _T("-");
		return;
	}

	CString strAxis, strPerAxis;
	if (data.nAxialLoc == T_SIGR_LOWER_COLM_D::kI)
		strAxis = _T("I");
	else if (data.nAxialLoc == T_SIGR_LOWER_COLM_D::kJ)
		strAxis = _T("J");
	if (data.nAxisPerpLoc == T_SIGR_LOWER_COLM_D::kI)
		strPerAxis = _T("I");
	else if (data.nAxisPerpLoc == T_SIGR_LOWER_COLM_D::kJ)
		strPerAxis = _T("J");

	rStr.Format(_T("%d_%s,%d_%s"), data.kAxialElem, strAxis, data.kAxisPerpElem, strPerAxis);
}

CString CSeisCvlJpSetTargetLowerDlg::DataToStr_COLM(const int& index, const T_SIGR_LOWER_COLM_D& data)
{
	CString str = _T("");

	switch (index)
	{
	case 0://명칭
		str = data.strName;
		break;
	case 1://대상요소
		GetStrKeyList(data.aInvestigatedElem, str);
		break;
	case 2://교축방향 전단력
		if (data.nDirShear == seis_jp::kY_Dir)
			str = COMBO_SHEAR_AXIS_DIR_Y;
		else if (data.nDirShear == seis_jp::kZ_Dir)
			str = COMBO_SHEAR_AXIS_DIR_Z;
		else if (data.nDirShear == seis_jp::kYZ_Dir)
			str = COMBO_SHEAR_AXIS_DIR_YZ;
		else { ASSERT(0); }
		break;
	case 3://철근 단락부
		GetRebarCutText(data, str);
		break;
	default: ASSERT(0); break;
	}

	return str;
}

CString CSeisCvlJpSetTargetLowerDlg::DataToStr_BEAM(const int& index, const T_SIGR_LOWER_BEAM_D& data)
{
	CString str = _T("");

	switch (index)
	{
	case 0://명칭
		str = data.strName;
		break;
	case 1://대상요소
		GetStrKeyList(data.aInvestigatedElem, str);
		break;
	case 2://교축방향 전단력
		if (data.nDirShear == seis_jp::kY_Dir)
			str = COMBO_SHEAR_AXIS_DIR_Y;
		else if (data.nDirShear == seis_jp::kZ_Dir)
			str = COMBO_SHEAR_AXIS_DIR_Z;
		else if (data.nDirShear == seis_jp::kYZ_Dir)
			str = COMBO_SHEAR_AXIS_DIR_YZ;
		else { ASSERT(0); }
		break;
	default: ASSERT(0); break;
	}

	return str;
}

CString CSeisCvlJpSetTargetLowerDlg::DataToStr_FOOT(const int& index, const T_SIGR_LOWER_FOOT_D& data)
{
	CString str = _T("");

	switch (index)
	{
	case 0://명칭
		str = data.strName;
		break;
	case 1://대상요소
		GetStrKeyList(data.aInvestigatedElem, str);
		break;
	case 2://교축방향 전단력
// 		if (data.nDir == T_SIGR_LOWER_FOOT_D::kY_Dir)
// 			str = COMBO_SHEAR_AXIS_DIR_Y;
// 		else if (data.nDir == T_SIGR_LOWER_FOOT_D::kZ_Dir)
// 			str = COMBO_SHEAR_AXIS_DIR_Z;
// 		else if (data.nDir == T_SIGR_LOWER_FOOT_D::kYZ_Dir)
// 			str = COMBO_SHEAR_AXIS_DIR_YZ;
// 		else { ASSERT(0); }
		break;
	default: ASSERT(0); break;
	}

	return str;
}

CString CSeisCvlJpSetTargetLowerDlg::DataToStr(const int& index, const T_SIGR_LOWER_D& data)
{
	CString str = _T("");

	switch (index)
	{
	case 0://명칭
		str = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetSigrName(data.kSigr);
		break;
	case 1://지승 그룹이름
		str = data.GetBearGroupText();
		break;
	case 2://기둥 그룹이름
		str = data.GetColmGroupText();
		break;
	case 3://보 그룹이름
		str = data.GetBeamGroupText();
		break;
	case 4://기초 그룹이름
		str = data.GetFootGroupText();
		break;
	default: ASSERT(0); break;
	}

	return str;
}

void CSeisCvlJpSetTargetLowerDlg::UpdateControlByMemberType()
{
	CString str = _T("");
	m_memberType.GetWindowText(str);

	if (str == COMBO_MEMBER_TYPE_BEAR)
	{
		UpdateControlSelectedBearing();
		UpdateControlMemberNode();
	}
	else if (str == COMBO_MEMBER_TYPE_COLM)
	{
		UpdateControlSelectedColumn();
		UpdateControlMemberElem();
	}
	else if (str == COMBO_MEMBER_TYPE_BEAM)
	{
		UpdateControlSelectedBeam();
		UpdateControlMemberElem();
	}
	else if (str == COMBO_MEMBER_TYPE_FOOT)
	{
		UpdateControlSelectedFooting();
		UpdateControlMemberElem();
	}
}

void CSeisCvlJpSetTargetLowerDlg::UpdateControlMemberNode()
{
	CDlgUtil::CtrlShowHide(this, m_aMemberElemCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aMemberNodeCtrl, TRUE);

	CRect rTarget, rSouce;
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_NODE_EDT)->GetWindowRect(rSouce);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_EDT)->GetWindowRect(rTarget);

	int nDistx = rTarget.left - rSouce.left;
	CDlgUtil::CtrlMoveDistX(this, m_aMemberNodeCtrl, nDistx);
}

void CSeisCvlJpSetTargetLowerDlg::UpdateControlMemberElem()
{
	CDlgUtil::CtrlShowHide(this, m_aMemberNodeCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aMemberElemCtrl, TRUE);
}

void CSeisCvlJpSetTargetLowerDlg::UpdateControlSelectedBearing()
{
	CDlgUtil::CtrlShowHide(this, m_aColumnCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aColumnListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aBeamListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aFootingListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aBearingCtrl, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aBearingListCtrl, TRUE);

	CRect rTarget, rSource;
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_LINE_CMB)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_DIR_CMB)->GetWindowRect(rTarget);

	int nDistY = rTarget.bottom - rSource.top + globalUtils.ScaleByDPI(7);
	CDlgUtil::CtrlMoveDistY(this, m_aBearingCtrl, nDistY);

	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_ADD_BTN)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_BEARING_TYPE_CMB)->GetWindowRect(rTarget);

	nDistY = rTarget.bottom - rSource.top + globalUtils.ScaleByDPI(7);
	MoveY_MemberTypeListCtrl(nDistY);

	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_STC)->SetWindowText(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_TM_SIGR_LOWER_BEAR_TYPE_TWO_NODE_STC));
}

void CSeisCvlJpSetTargetLowerDlg::UpdateControlSelectedColumn()
{
	CDlgUtil::CtrlShowHide(this, m_aBearingCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aBearingListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aBeamListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aFootingListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aColumnCtrl, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aColumnListCtrl, TRUE);

	CRect rTarget, rSource;
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_ADD_BTN)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_REBAR_CUT_GRP)->GetWindowRect(rTarget);

	int nDistY = rTarget.bottom - rSource.top + globalUtils.ScaleByDPI(7);
	MoveY_MemberTypeListCtrl(nDistY);

	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_STC)->SetWindowText(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_STC));

	OnBnClickedCheck();
}

void CSeisCvlJpSetTargetLowerDlg::UpdateControlSelectedBeam()
{
	CDlgUtil::CtrlShowHide(this, m_aBearingCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aColumnCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aBearingListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aColumnListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aFootingListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aBeamListCtrl, TRUE);

	CRect rTarget, rSource;
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_ADD_BTN)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_DIR_STC)->GetWindowRect(rTarget);

	int nDistY = rTarget.bottom - rSource.top + globalUtils.ScaleByDPI(7);
	MoveY_MemberTypeListCtrl(nDistY);

	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_STC)->SetWindowText(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_STC));
}

void CSeisCvlJpSetTargetLowerDlg::UpdateControlSelectedFooting()
{
	CDlgUtil::CtrlShowHide(this, m_aBearingCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aColumnCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aBearingListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aColumnListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aBeamListCtrl, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aFootingListCtrl, TRUE);

	CRect rTarget, rSource;
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_ADD_BTN)->GetWindowRect(rSource);
	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_DIR_STC)->GetWindowRect(rTarget);

	int nDistY = rTarget.bottom - rSource.top + globalUtils.ScaleByDPI(7);
	MoveY_MemberTypeListCtrl(nDistY);

	GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_STC)->SetWindowText(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_TM_SIGR_LOWER_MEMB_TYPE_INCLUD_ELEM_STC));
}

void CSeisCvlJpSetTargetLowerDlg::MoveY_MemberTypeListCtrl(const int& nDistY)
{
	CDlgUtil::CtrlMoveDistY(this, m_aBottunCtrl, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aBearingListCtrl, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aColumnListCtrl, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aBeamListCtrl, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aFootingListCtrl, nDistY);
}

BOOL CSeisCvlJpSetTargetLowerDlg::AddBtnBearing()
{
	T_SIGR_LOWER_BEAR_D data;
	DlgToData_BEAR(data);

	if (CheckData_BEAR(data) == FALSE)
		return FALSE;

	int nCount = m_bearingList.GetItemCount();
	InsertItem_BEAR(nCount, data);

	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::SetMemberCmbConnectType()
{
	CString strPreviousText = _T("");

	int nKind = 0;
	CString str;
	m_memberType.GetWindowText(str);
	if (str == COMBO_MEMBER_TYPE_BEAR)
	{
		m_membersElem.Disconnect();
		m_membersNode.GetWindowText(strPreviousText);
		nKind = SC_ID_NODE;
		m_membersNode.Connect(CURSOR_ID_NODE);
		m_membersNode.SetWindowText(strPreviousText);
	}
	else
	{
		m_membersNode.Disconnect();
		m_membersElem.GetWindowText(strPreviousText);
		nKind = SC_ID_ELEM;
		m_membersElem.Connect(nKind, &m_membersElem);
		m_membersElem.SetWindowText(strPreviousText);
	}

	ResetViewControls();

	if (nKind == SC_ID_NODE) {
		CArray<T_NODE_K, T_NODE_K> aNodes;
		GetNodeList(strPreviousText, aNodes);
		m_pDoc->m_pViewCtrl->SelectNode(NULL, aNodes, FALSE);
	}
	else if (nKind == SC_ID_ELEM) {
		CArray<T_ELEM_K, T_ELEM_K> aElements;
		GetNodeList(strPreviousText, aElements);
		m_pDoc->m_pViewCtrl->SelectElem(NULL, aElements, FALSE);
	}
}

void CSeisCvlJpSetTargetLowerDlg::DlgToData_BEAR(T_SIGR_LOWER_BEAR_D& rData)
{
	CString str;
	rData.Initialize();

	//명칭
	m_memberName.GetWindowText(rData.strName);
	//양단 절점
	m_membersNode.GetWindowText(str);
	GetNodeList(str, rData.aPosition);
	//교축방향 전단력
	m_shearAxisDir.GetWindowText(str);
	rData.nDirShear = seis_jp::kDirShear_0;
	if (str == COMBO_SHEAR_AXIS_DIR_Y)
		rData.nDirShear = seis_jp::kY_Dir;
	else if (str == COMBO_SHEAR_AXIS_DIR_Z)
		rData.nDirShear = seis_jp::kZ_Dir;
	else { ASSERT(FALSE); }
// 	else if (str == COMBO_SHEAR_AXIS_DIR_YZ)
// 		rData.nDir = T_SIGR_LOWER_BEAR_D::kYZ_Dir;
	//지승선
	m_bearingLine.GetWindowText(str);
	rData.nLineNum = T_SIGR_LOWER_BEAR_D::kLine0;
	if (str == _T("1"))
		rData.nLineNum = T_SIGR_LOWER_BEAR_D::kLine1;
	else if (str == _T("2"))
		rData.nLineNum = T_SIGR_LOWER_BEAR_D::kLine2;
	//지승조건
	m_bearingType.GetWindowText(str);
	rData.nType = T_SIGR_LOWER_BEAR_D::kUnKnown;
	if (str == COMBO_BEAR_TYPE_RIGID_LINK)
		rData.nType = T_SIGR_LOWER_BEAR_D::kRigidLink;
	else if (str == COMBO_BEAR_TYPE_FIXED)
		rData.nType = T_SIGR_LOWER_BEAR_D::kFixed;
	else if (str == COMBO_BEAR_TYPE_MOVING)
		rData.nType = T_SIGR_LOWER_BEAR_D::kMovable;
	else if (str == COMBO_BEAR_TYPE_ELASTIC)
		rData.nType = T_SIGR_LOWER_BEAR_D::kElastic;
	else if (str == COMBO_BEAR_TYPE_VIBRA_ISOLA)
		rData.nType = T_SIGR_LOWER_BEAR_D::kSeismicIsolation;
}

BOOL CSeisCvlJpSetTargetLowerDlg::CheckData_BEAR(const T_SIGR_LOWER_BEAR_D& data, BOOL bCheckName/*=TRUE*/)
{
	CString msg;
	//명칭 공백체크
	if (data.strName.IsEmpty() || data.strName == _T("")) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_NAME_BEAR);
		AfxMessageBox(msg);
		return FALSE;
	}
	//명칭 중복체크
	if (bCheckName == TRUE && IsDuplicatedMemberName(data.strName) == TRUE) {
		msg = _LS(IDS_IDD_TM_SIGR_MSG_EXIST_SIGRNAME);
		AfxMessageBox(msg);
		return FALSE;
	}
	//양단 절점 개수체크
	if (data.aPosition.GetCount() != 2) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_NODE_BEAR);
		AfxMessageBox(msg);
		return FALSE;
	}
	
	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::InsertItem_BEAR(const int& nIndex, IN T_SIGR_LOWER_BEAR_D& data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for (int i = 0; i < LIST_COLUMN_NUM_BEAR; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr_BEAR(i, data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
			nItem = m_bearingList.InsertItem(&lvitem);
		else
			m_bearingList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

BOOL CSeisCvlJpSetTargetLowerDlg::AddBtnColumn()
{
	T_SIGR_LOWER_COLM_D data;
	DlgToData_COLM(data);

	if (CheckData_AddCOLM(data) == FALSE)
		return FALSE;

	int nCount = m_columnList.GetItemCount();
	InsertItem_COLM(nCount, data);

	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::DlgToData_COLM(T_SIGR_LOWER_COLM_D& rData)
{
	CString str;
	rData.Initialize();

	//명칭
	m_memberName.GetWindowText(rData.strName);
	//대상 요소
	m_membersElem.GetWindowText(str);
	GetNodeList(str, rData.aInvestigatedElem);
	//교축방향 전단력
	m_shearAxisDir.GetWindowText(str);
	rData.nDirShear = seis_jp::kDirShear_0;
	if (str == COMBO_SHEAR_AXIS_DIR_Y)
		rData.nDirShear = seis_jp::kY_Dir;
	else if (str == COMBO_SHEAR_AXIS_DIR_Z)
		rData.nDirShear = seis_jp::kZ_Dir;
	else if (str == COMBO_SHEAR_AXIS_DIR_YZ)
		rData.nDirShear = seis_jp::kYZ_Dir;
	//철근 단락부
	rData.bRebarCut = m_rebarCut;
	//철근 단락부(각 방향별 설정 값)
	if (m_rebarCut == FALSE) {
		rData.kAxialElem = 0;
		rData.kAxisPerpElem = 0;
		rData.nAxialLoc = T_SIGR_LOWER_COLM_D::kUnKnown;
		rData.nAxisPerpLoc = T_SIGR_LOWER_COLM_D::kUnKnown;
		return;
	}

	CString msg;

	//철근 단락부(교축방향)
	T_KEY_LIST keys;
	m_axisElem.GetWindowText(str);
	GetNodeList(str, keys);
	if (keys.GetCount() > 0) {
		rData.kAxialElem = keys[0];

		if (keys.GetCount() > 1) {
			msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_REBAR_CUT_INPUT_ONE_NODE);
			AfxMessageBox(msg);

			str.Format(_T("%d"), keys[0]);
			m_axisElem.SetWindowText(str);
		}
	}
		

	m_axisEndRle.GetWindowText(str);
	rData.nAxialLoc = T_SIGR_LOWER_COLM_D::kUnKnown;
	if (str == _T("I"))
		rData.nAxialLoc = T_SIGR_LOWER_COLM_D::kI;
	else if (str == _T("J"))
		rData.nAxialLoc = T_SIGR_LOWER_COLM_D::kJ;
	//철근 단락부(교축직각방향)
	keys.RemoveAll();
	m_perAxisElem.GetWindowText(str);
	GetNodeList(str, keys);
	if (keys.GetCount() > 0) {
		rData.kAxisPerpElem = keys[0];

		if (keys.GetCount() > 1) {
			msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_REBAR_CUT_INPUT_ONE_NODE);
			AfxMessageBox(msg);

			str.Format(_T("%d"), keys[0]);
			m_perAxisElem.SetWindowText(str);
		}
	}

	m_perAxisEndRle.GetWindowText(str);
	rData.nAxisPerpLoc = T_SIGR_LOWER_COLM_D::kUnKnown;
	if (str == _T("I"))
		rData.nAxisPerpLoc = T_SIGR_LOWER_COLM_D::kI;
	else if (str == _T("J"))
		rData.nAxisPerpLoc = T_SIGR_LOWER_COLM_D::kJ;
}

BOOL CSeisCvlJpSetTargetLowerDlg::CheckData_AddCOLM(const T_SIGR_LOWER_COLM_D& data)
{
	CString msg;
	//명칭 공백체크
	if (data.strName.IsEmpty() || data.strName == _T("")) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_NAME_COLM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//명칭 중복체크
	if (IsDuplicatedMemberName(data.strName) == TRUE) {
		msg = _LS(IDS_IDD_TM_SIGR_MSG_EXIST_SIGRNAME);
		AfxMessageBox(msg);
		return FALSE;
	}
	//대상요소 개수체크
	if (data.aInvestigatedElem.GetCount() < 1) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_ELEM_COLM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//각 부재의 조사 대상요소(지승 제외)와 중복되는지 체크
	if (IsDuplicatedAtMemberElem(data.aInvestigatedElem) == TRUE) {
		return FALSE;
	}
	//기둥의 모든 요소의 Local 축이 정렬되지 있는지 체크
	if (IsSortedLocalAxis(data.aInvestigatedElem) == FALSE) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_NOT_SORTED_LOCAL_AXIS);
		AfxMessageBox(msg);//경고만 하고 DB에 넣는 것은 허용
	}

	//철근 단락부 입력체크
	if (data.bRebarCut == FALSE)
		return TRUE;

	if (data.kAxialElem == 0) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_REBAR_CUT_ELEM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//대상요소에서 철근 단락부 요소가 선택 되었는지 체크(교축방향)
	const auto itr1 = std::find(&data.aInvestigatedElem[0], &data.aInvestigatedElem[0] + data.aInvestigatedElem.GetCount(), data.kAxialElem);
	if (itr1 == &data.aInvestigatedElem[0] + data.aInvestigatedElem.GetCount()) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_REBAR_CUT_UNSELECTED_ELEM);
		AfxMessageBox(msg);
		return FALSE;
	}

	if (data.kAxisPerpElem == 0) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_REBAR_CUT_ELEM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//대상요소에서 철근 단락부 요소가 선택 되었는지 체크(교축직각방향)
	const auto itr2 = std::find(&data.aInvestigatedElem[0], &data.aInvestigatedElem[0] + data.aInvestigatedElem.GetCount(), data.kAxisPerpElem);
	if (itr2 == &data.aInvestigatedElem[0] + data.aInvestigatedElem.GetCount()) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_REBAR_CUT_UNSELECTED_ELEM);
		AfxMessageBox(msg);
		return FALSE;
	}

	return TRUE;
}

BOOL CSeisCvlJpSetTargetLowerDlg::CheckData_ModCOLM(const T_SIGR_LOWER_COLM_D& data)
{
	CString msg;
	//명칭 공백체크
	if (data.strName.IsEmpty() || data.strName == _T("")) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_NAME_COLM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//대상요소 개수체크
	if (data.aInvestigatedElem.GetCount() < 1) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_ELEM_COLM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//각 부재의 조사 대상요소(지승 제외)와 중복되는지 체크
	if (IsDuplicatedAtMemberElem(data.aInvestigatedElem, data.strName) == TRUE) {
		return FALSE;
	}
	//기둥의 모든 요소의 Local 축이 정렬되지 있는지 체크
	if (IsSortedLocalAxis(data.aInvestigatedElem) == FALSE) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_NOT_SORTED_LOCAL_AXIS);
		AfxMessageBox(msg);//경고만 하고 DB에 넣는 것은 허용
	}

	//철근 단락부 입력체크
	if (data.bRebarCut == FALSE)
		return TRUE;

	if (data.kAxialElem == 0) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_REBAR_CUT_ELEM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//대상요소에서 철근 단락부 요소가 선택 되었는지 체크(교축방향)
	const auto itr1 = std::find(&data.aInvestigatedElem[0], &data.aInvestigatedElem[0] + data.aInvestigatedElem.GetCount(), data.kAxialElem);
	if (itr1 == &data.aInvestigatedElem[0] + data.aInvestigatedElem.GetCount()) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_REBAR_CUT_UNSELECTED_ELEM);
		AfxMessageBox(msg);
		return FALSE;
	}

	if (data.kAxisPerpElem == 0) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_REBAR_CUT_ELEM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//대상요소에서 철근 단락부 요소가 선택 되었는지 체크(교축직각방향)
	const auto itr2 = std::find(&data.aInvestigatedElem[0], &data.aInvestigatedElem[0] + data.aInvestigatedElem.GetCount(), data.kAxisPerpElem);
	if (itr2 == &data.aInvestigatedElem[0] + data.aInvestigatedElem.GetCount()) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_REBAR_CUT_UNSELECTED_ELEM);
		AfxMessageBox(msg);
		return FALSE;
	}

	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::InsertItem_COLM(const int& nIndex, IN T_SIGR_LOWER_COLM_D& data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for (int i = 0; i < LIST_COLUMN_NUM_COLM; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr_COLM(i, data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
			nItem = m_columnList.InsertItem(&lvitem);
		else
			m_columnList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

BOOL CSeisCvlJpSetTargetLowerDlg::AddBtnBeam()
{
	T_SIGR_LOWER_BEAM_D data;
	DlgToData_BEAM(data);

	if (CheckData_AddBEAM(data) == FALSE)
		return FALSE;

	int nCount = m_beamList.GetItemCount();
	InsertItem_BEAM(nCount, data);

	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::DlgToData_BEAM(T_SIGR_LOWER_BEAM_D& rData)
{
	CString str;
	rData.Initialize();

	//명칭
	m_memberName.GetWindowText(rData.strName);
	//대상 요소
	m_membersElem.GetWindowText(str);
	GetNodeList(str, rData.aInvestigatedElem);
	//교축방향 전단력
	m_shearAxisDir.GetWindowText(str);
	rData.nDirShear = seis_jp::kDirShear_0;
	if (str == COMBO_SHEAR_AXIS_DIR_Y)
		rData.nDirShear = seis_jp::kY_Dir;
	else if (str == COMBO_SHEAR_AXIS_DIR_Z)
		rData.nDirShear = seis_jp::kZ_Dir;
	else if (str == COMBO_SHEAR_AXIS_DIR_YZ)
		rData.nDirShear = seis_jp::kYZ_Dir;
}

BOOL CSeisCvlJpSetTargetLowerDlg::CheckData_AddBEAM(const T_SIGR_LOWER_BEAM_D& data)
{
	CString msg;
	//명칭 공백체크
	if (data.strName.IsEmpty() || data.strName == _T("")) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_NAME_BEAM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//명칭 중복체크
	if (IsDuplicatedMemberName(data.strName) == TRUE) {
		msg = _LS(IDS_IDD_TM_SIGR_MSG_EXIST_SIGRNAME);
		AfxMessageBox(msg);
		return FALSE;
	}
	//대상요소 개수체크
	if (data.aInvestigatedElem.GetCount() < 1) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_ELEM_BEAM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//각 부재의 조사 대상요소(지승 제외)와 중복되는지 체크
	if (IsDuplicatedAtMemberElem(data.aInvestigatedElem) == TRUE) {
		return FALSE;
	}

	return TRUE;
}

BOOL CSeisCvlJpSetTargetLowerDlg::CheckData_ModBEAM(const T_SIGR_LOWER_BEAM_D& data)
{
	CString msg;
	//명칭 공백체크
	if (data.strName.IsEmpty() || data.strName == _T("")) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_NAME_BEAM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//대상요소 개수체크
	if (data.aInvestigatedElem.GetCount() < 1) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_ELEM_BEAM);
		AfxMessageBox(msg);
		return FALSE;
	}
	//각 부재의 조사 대상요소(지승 제외)와 중복되는지 체크
	if (IsDuplicatedAtMemberElem(data.aInvestigatedElem, data.strName) == TRUE) {
		return FALSE;
	}

	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::InsertItem_BEAM(const int& nIndex, IN T_SIGR_LOWER_BEAM_D& data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for (int i = 0; i < LIST_COLUMN_NUM_BEAM; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr_BEAM(i, data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
			nItem = m_beamList.InsertItem(&lvitem);
		else
			m_beamList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

BOOL CSeisCvlJpSetTargetLowerDlg::AddBtnFooting()
{
	T_SIGR_LOWER_FOOT_D data;
	DlgToData_FOOT(data);

	if (CheckData_AddFOOT(data) == FALSE)
		return FALSE;

	int nCount = m_footingList.GetItemCount();
	InsertItem_FOOT(nCount, data);

	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::DlgToData_FOOT(T_SIGR_LOWER_FOOT_D& rData)
{
	CString str;
	rData.Initialize();

	//명칭
	m_memberName.GetWindowText(rData.strName);
	//대상 요소
	m_membersElem.GetWindowText(str);
	GetNodeList(str, rData.aInvestigatedElem);
	//교축방향 전단력
	m_shearAxisDir.GetWindowText(str);
	rData.nDirShear = seis_jp::kDirShear_0;
	//기초는 교축방향 전단력 값을 필요로 하지 않는다.
// 	if (str == COMBO_SHEAR_AXIS_DIR_Y)
// 		rData.nDirShear = seis_jp::kY_Dir;
// 	else if (str == COMBO_SHEAR_AXIS_DIR_Z)
// 		rData.nDirShear = seis_jp::kZ_Dir;
// 	else if (str == COMBO_SHEAR_AXIS_DIR_YZ)
// 		rData.nDirShear = seis_jp::kYZ_Dir;
}

BOOL CSeisCvlJpSetTargetLowerDlg::CheckData_AddFOOT(const T_SIGR_LOWER_FOOT_D& data)
{
	CString msg;
	//명칭 공백체크
	if (data.strName.IsEmpty() || data.strName == _T("")) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_NAME_FOOT);
		AfxMessageBox(msg);
		return FALSE;
	}
	//명칭 중복체크
	if (IsDuplicatedMemberName(data.strName) == TRUE) {
		msg = _LS(IDS_IDD_TM_SIGR_MSG_EXIST_SIGRNAME);
		AfxMessageBox(msg);
		return FALSE;
	}
	//대상요소 개수체크
	if (data.aInvestigatedElem.GetCount() < 1) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_ELEM_FOOT);
		AfxMessageBox(msg);
		return FALSE;
	}
	//각 부재의 조사 대상요소(지승 제외)와 중복되는지 체크
	if (IsDuplicatedAtMemberElem(data.aInvestigatedElem) == TRUE) {
		return FALSE;
	}

	return TRUE;
}

BOOL CSeisCvlJpSetTargetLowerDlg::CheckData_ModFOOT(const T_SIGR_LOWER_FOOT_D& data)
{
	CString msg;
	//명칭 공백체크
	if (data.strName.IsEmpty() || data.strName == _T("")) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_NAME_FOOT);
		AfxMessageBox(msg);
		return FALSE;
	}
	//대상요소 개수체크
	if (data.aInvestigatedElem.GetCount() < 1) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_ELEM_FOOT);
		AfxMessageBox(msg);
		return FALSE;
	}
	//각 부재의 조사 대상요소(지승 제외)와 중복되는지 체크
	if (IsDuplicatedAtMemberElem(data.aInvestigatedElem, data.strName) == TRUE) {
		return FALSE;
	}

	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::InsertItem_FOOT(const int& nIndex, IN T_SIGR_LOWER_FOOT_D& data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for (int i = 0; i < LIST_COLUMN_NUM_FOOT; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr_FOOT(i, data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
			nItem = m_footingList.InsertItem(&lvitem);
		else
			m_footingList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

BOOL CSeisCvlJpSetTargetLowerDlg::IsDuplicatedMemberName(const CString& memberName)
{
	CString msg;
	int nItem;
	for (nItem = 0; nItem < m_bearingList.GetItemCount(); nItem++)
	{
		CString str = m_bearingList.GetItemText(nItem, 0);//지승 명칭
		if (str == memberName)
			return TRUE;
	}
	for (nItem = 0; nItem < m_columnList.GetItemCount(); nItem++)
	{
		CString str = m_columnList.GetItemText(nItem, 0);//기둥 명칭
		if (str == memberName)
			return TRUE;
	}
	for (nItem = 0; nItem < m_beamList.GetItemCount(); nItem++)
	{
		CString str = m_beamList.GetItemText(nItem, 0);//보 명칭
		if (str == memberName)
			return TRUE;
	}
	for (nItem = 0; nItem < m_footingList.GetItemCount(); nItem++)
	{
		CString str = m_footingList.GetItemText(nItem, 0);//기초 명칭
		if (str == memberName)
			return TRUE;
	}
	//기존에 등록된 하부구조 그룹이름 및 부재 명칭확인
	for (nItem = 0; nItem < m_sigrLowerList.GetItemCount(); nItem++)
	{
		CString strSIGR = m_sigrLowerList.GetItemText(nItem, 0);//그룹 명칭
		CString strBEAR = m_sigrLowerList.GetItemText(nItem, 1);//지승 명칭
		CString strCOLM = m_sigrLowerList.GetItemText(nItem, 2);//기둥 명칭
		CString strBEAM = m_sigrLowerList.GetItemText(nItem, 3);//보   명칭
		CString strFOOT = m_sigrLowerList.GetItemText(nItem, 4);//기초 명칭

		if (IsContainString(memberName, strSIGR) == TRUE) return TRUE;
		if (IsContainString(memberName, strBEAR) == TRUE) return TRUE;
		if (IsContainString(memberName, strCOLM) == TRUE) return TRUE;
		if (IsContainString(memberName, strBEAM) == TRUE) return TRUE;
		if (IsContainString(memberName, strFOOT) == TRUE) return TRUE;
	}

	return FALSE;
}

BOOL CSeisCvlJpSetTargetLowerDlg::IsContainString(const CString& name, const CString& gridName)
{
	if (gridName == name)
		return TRUE;

	CString delimiter = _T(",");
	CStringArray results;
	CStrParser::SplitString(gridName, delimiter, results);

	for (int i = 0; i < results.GetCount(); i++)
	{
		if (results[i] == name)
			return TRUE;
	}

	return FALSE;
}

BOOL CSeisCvlJpSetTargetLowerDlg::IsDuplicatedAtMemberElem(const T_KEY_LIST& elems, CString myName/*=_T("")*/)
{
	T_KEY_LIST allElemKeyList;

	int nSize = m_columnList.GetItemCount();
	for (int nItem = 0; nItem < nSize; nItem++)
	{
		CString strName = m_columnList.GetItemText(nItem, 0);
		if (strName == myName)
			continue;

		T_KEY_LIST elemKeys;
		CString str = m_columnList.GetItemText(nItem, 1);
		GetNodeList(str, elemKeys);
		allElemKeyList.Append(elemKeys);
	}

	nSize = m_beamList.GetItemCount();
	for (int nItem = 0; nItem < nSize; nItem++)
	{
		CString strName = m_beamList.GetItemText(nItem, 0);
		if (strName == myName)
			continue;

		T_KEY_LIST elemKeys;
		CString str = m_beamList.GetItemText(nItem, 1);
		GetNodeList(str, elemKeys);
		allElemKeyList.Append(elemKeys);
	}

	nSize = m_footingList.GetItemCount();
	for (int nItem = 0; nItem < nSize; nItem++)
	{
		CString strName = m_footingList.GetItemText(nItem, 0);
		if (strName == myName)
			continue;

		T_KEY_LIST elemKeys;
		CString str = m_footingList.GetItemText(nItem, 1);
		GetNodeList(str, elemKeys);
		allElemKeyList.Append(elemKeys);
	}

	if (allElemKeyList.GetCount() < 1)
		return FALSE;

	for (int i = 0; i < elems.GetCount(); i++)
	{
		T_ELEM_K kElem = elems[i];

		const auto itr = std::find(&allElemKeyList[0], &allElemKeyList[0] + allElemKeyList.GetCount(), kElem);
		if (itr != &allElemKeyList[0] + allElemKeyList.GetCount()) {
			CString msg; msg.Format(_LS(IDS_IDD_TM_SIGR_LOWER_MSG_DUPLICATED_MEMBER_ELEM), kElem);
			AfxMessageBox(msg);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CSeisCvlJpSetTargetLowerDlg::IsSortedLocalAxis(const T_KEY_LIST& elems)
{
	/*
	double rx, ry, rz;
	double vx, vy, vz;
	CDgnMathFunc::mathNormalize(Data.Vector[0], Data.Vector[1], Data.Vector[2], vx, vy, vz);
	rx = vx * FrameLocalVector[0][0] + vy * FrameLocalVector[0][1] + vz * FrameLocalVector[0][2];
	ry = vx * FrameLocalVector[1][0] + vy * FrameLocalVector[1][1] + vz * FrameLocalVector[1][2];
	rz = vx * FrameLocalVector[2][0] + vy * FrameLocalVector[2][1] + vz * FrameLocalVector[2][2];
	*/

	double dLocalBefore[3][3] = { 0., };

	for (int i = 0; i < elems.GetCount(); i++)
	{
		double dLocalVector[3][3] = { 0., };
		if (m_pDoc->calcFrameLocalVector(elems[i], dLocalVector) == FALSE) {
			ASSERT(FALSE);
			continue;
		}

		if (i == 0) {
			copy(&dLocalVector[0][0], &dLocalVector[0][0] + 9, &dLocalBefore[0][0]);
		}
		else {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					
					if (dgn::EQ(dLocalBefore[i][j], dLocalVector[i][j]) == false)
						return FALSE;
				}
			}
				
		}
	}

	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::ModBtnBearing()
{
	int nItem = m_bearingList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}
	
	T_SIGR_LOWER_BEAR_D data;
	DlgToData_BEAR(data);

	if (CheckData_BEAR(data, FALSE) == FALSE)
		return;

	for (int i = 0; i < LIST_COLUMN_NUM_BEAR; i++)
	{
		CString str = DataToStr_BEAR(i, data);
		m_bearingList.SetItemText(nItem, i, str);
	}
}

void CSeisCvlJpSetTargetLowerDlg::ModBtnColumn()
{
	int nItem = m_columnList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	T_SIGR_LOWER_COLM_D data;
	DlgToData_COLM(data);

	//대상요소 중복 체크 함수를 일관적으로 적용하기 위해 일단 선택된 열의 대상요소는 삭제한다.
	CString strElem = m_columnList.GetItemText(nItem, 1);
	m_columnList.SetItemText(nItem, 1, _T(""));

	if (CheckData_ModCOLM(data) == FALSE) {
		m_columnList.SetItemText(nItem, 1, strElem);//원상복귀
		return;
	}

	for (int i = 0; i < LIST_COLUMN_NUM_COLM; i++)
	{
		CString str = DataToStr_COLM(i, data);
		m_columnList.SetItemText(nItem, i, str);
	}
}

void CSeisCvlJpSetTargetLowerDlg::ModBtnBeam()
{
	int nItem = m_beamList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	T_SIGR_LOWER_BEAM_D data;
	DlgToData_BEAM(data);

	if (CheckData_ModBEAM(data) == FALSE)
		return;

	for (int i = 0; i < LIST_COLUMN_NUM_BEAM; i++)
	{
		CString str = DataToStr_BEAM(i, data);
		m_beamList.SetItemText(nItem, i, str);
	}
}

void CSeisCvlJpSetTargetLowerDlg::ModBtnFooting()
{
	int nItem = m_footingList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	T_SIGR_LOWER_FOOT_D data;
	DlgToData_FOOT(data);

	if (CheckData_ModFOOT(data) == FALSE)
		return;

	for (int i = 0; i < LIST_COLUMN_NUM_FOOT; i++)
	{
		CString str = DataToStr_FOOT(i, data);
		m_footingList.SetItemText(nItem, i, str);
	}
}

void CSeisCvlJpSetTargetLowerDlg::DelBtnBearing()
{
	int nItem = m_bearingList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	m_bearingList.DeleteItem(nItem);
}

void CSeisCvlJpSetTargetLowerDlg::DelBtnColumn()
{
	int nItem = m_columnList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	m_columnList.DeleteItem(nItem);
}

void CSeisCvlJpSetTargetLowerDlg::DelBtnBeam()
{
	int nItem = m_beamList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	m_beamList.DeleteItem(nItem);
}

void CSeisCvlJpSetTargetLowerDlg::DelBtnFooting()
{
	int nItem = m_footingList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem < 0) {
		return;
	}

	m_footingList.DeleteItem(nItem);
}

void CSeisCvlJpSetTargetLowerDlg::UpdataDialog_BEAR(const int& nItem)
{
	//명칭
	CString str = m_bearingList.GetItemText(nItem, 0);
	m_memberName.SetWindowText(str);
	//양단 절점
	str = m_bearingList.GetItemText(nItem, 1);
	m_membersNode.SetWindowText(str);

	T_KEY_LIST keys;
	if(GetNodeList(str, keys) == TRUE)
		m_pDoc->m_pViewCtrl->SelectNode(NULL, keys, FALSE);

	//교축방향 단면력
	str = m_bearingList.GetItemText(nItem, 2);
	if (str.IsEmpty() == false) {
		int iIndex = m_shearAxisDir.FindStringExact(-1, str);
		if (iIndex >= 0) m_shearAxisDir.SetCurSel(iIndex);
		else			 m_shearAxisDir.SetCurSel(0);
	}
	//지승선
	str = m_bearingList.GetItemText(nItem, 3);
	if (str.IsEmpty() == false) {
		int iIndex = m_bearingLine.FindStringExact(-1, str);
		if (iIndex >= 0) m_bearingLine.SetCurSel(iIndex);
		else			 m_bearingLine.SetCurSel(0);
	}
	//지승조건
	str = m_bearingList.GetItemText(nItem, 4);
	if (str.IsEmpty() == false) {
		int iIndex = m_bearingType.FindStringExact(-1, str);
		if (iIndex >= 0) m_bearingType.SetCurSel(iIndex);
		else			 m_bearingType.SetCurSel(0);
	}
}

void CSeisCvlJpSetTargetLowerDlg::UpdataDialog_COLM(const int& nItem)
{
	//명칭
	CString str = m_columnList.GetItemText(nItem, 0);
	m_memberName.SetWindowText(str);
	//대상 요소
	str = m_columnList.GetItemText(nItem, 1);
	m_membersElem.SetWindowText(str);

	T_KEY_LIST keys;
	if (GetNodeList(str, keys) == TRUE)
		m_pDoc->m_pViewCtrl->SelectElem(NULL, keys, FALSE);

	//교축방향 단면력
	str = m_columnList.GetItemText(nItem, 2);
	if (str.IsEmpty() == false) {
		int iIndex = m_shearAxisDir.FindStringExact(-1, str);
		if (iIndex >= 0) m_shearAxisDir.SetCurSel(iIndex);
		else			 m_shearAxisDir.SetCurSel(0);
	}
	//철근 단락부
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SIGR_LOWER_MEMB_TYPE_REBAR_CUT_CHK);

	str = m_columnList.GetItemText(nItem, 3);
	if (str == _T("-")) {
		m_axisElem.SetWindowText(_T(""));
		m_perAxisElem.SetWindowText(_T(""));
		pBtn->SetCheck(FALSE);
		return;
	}
	
	pBtn->SetCheck(TRUE);

	CString delimiter = _T(",");
	CStringArray results;
	CStrParser::SplitString(str, delimiter, results);

	ASSERT(results.GetCount() == 2);
	if (results.GetCount() != 2)
		return;
	//교축방향
	str = results[0];
	delimiter = _T("_");
	CStringArray results2;
	CStrParser::SplitString(str, delimiter, results2);

	ASSERT(results2.GetCount() == 2);
	if (results2.GetCount() != 2)
		return;

	str = results2[0];
	m_axisElem.SetWindowText(str);

	str = results2[1];
	if (str.IsEmpty() == false) {
		int iIndex = m_axisEndRle.FindStringExact(-1, str);
		if (iIndex >= 0) m_axisEndRle.SetCurSel(iIndex);
		else			 m_axisEndRle.SetCurSel(0);
	}

	//교축직각방향
	str = results[1];
	delimiter = _T("_");
	results2.RemoveAll();
	CStrParser::SplitString(str, delimiter, results2);

	ASSERT(results2.GetCount() == 2);
	if (results2.GetCount() != 2)
		return;

	str = results2[0];
	m_perAxisElem.SetWindowText(str);

	str = results2[1];
	if (str.IsEmpty() == false) {
		int iIndex = m_perAxisEndRle.FindStringExact(-1, str);
		if (iIndex >= 0) m_perAxisEndRle.SetCurSel(iIndex);
		else			 m_perAxisEndRle.SetCurSel(0);
	}
}

void CSeisCvlJpSetTargetLowerDlg::UpdataDialog_BEAM(const int& nItem)
{
	//명칭
	CString str = m_beamList.GetItemText(nItem, 0);
	m_memberName.SetWindowText(str);
	//대상 요소
	str = m_beamList.GetItemText(nItem, 1);
	m_membersElem.SetWindowText(str);

	T_KEY_LIST keys;
	if (GetNodeList(str, keys) == TRUE)
		m_pDoc->m_pViewCtrl->SelectElem(NULL, keys, FALSE);

	//교축방향 단면력
	str = m_beamList.GetItemText(nItem, 2);
	if (str.IsEmpty() == false) {
		int iIndex = m_shearAxisDir.FindStringExact(-1, str);
		if (iIndex >= 0) m_shearAxisDir.SetCurSel(iIndex);
		else			 m_shearAxisDir.SetCurSel(0);
	}
}

void CSeisCvlJpSetTargetLowerDlg::UpdataDialog_FOOT(const int& nItem)
{
	//명칭
	CString str = m_footingList.GetItemText(nItem, 0);
	m_memberName.SetWindowText(str);
	//대상 요소
	str = m_footingList.GetItemText(nItem, 1);
	m_membersElem.SetWindowText(str);

	T_KEY_LIST keys;
	if (GetNodeList(str, keys) == TRUE)
		m_pDoc->m_pViewCtrl->SelectElem(NULL, keys, FALSE);

	//교축방향 단면력
	str = m_footingList.GetItemText(nItem, 2);
	if (str.IsEmpty() == false) {
		int iIndex = m_shearAxisDir.FindStringExact(-1, str);
		if (iIndex >= 0) m_shearAxisDir.SetCurSel(iIndex);
		else			 m_shearAxisDir.SetCurSel(0);
	}
}

BOOL CSeisCvlJpSetTargetLowerDlg::AddData()
{
	T_SIGR_D sigrD;
	sigrD.nSubType = T_SIGR_D::kLower;
	T_SIGR_LOWER_D lowerD;
	if (DlgToData(sigrD, lowerD) == FALSE)
		return FALSE;

	if (CheckData(sigrD, lowerD) == FALSE)
		return FALSE;

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_SIGR_LOWER)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	T_SIGR_K sigrKey = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetStartNum();
	if (m_pDoc->m_pEditData->AddSigr(sigrKey, sigrD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}

	lowerD.kSigr = sigrKey;
	if (m_pDoc->m_pEditData->AddSigrLower(sigrKey, lowerD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}

	ResetAllControls();
	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIGR_LOWER_CMD);
	return TRUE;
}

BOOL CSeisCvlJpSetTargetLowerDlg::ModData(const T_SIGR_LOWER_K& key)
{
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Exist(key) == FALSE)
		return FALSE;

	T_SIGR_LOWER_D oldD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(key, oldD) == FALSE)
		return FALSE;

	T_SIGR_D oldSigrD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(oldD.kSigr, oldSigrD) == FALSE)
		return FALSE;

	T_SIGR_D sigrD;
	sigrD.nSubType = T_SIGR_D::kLower;
	T_SIGR_LOWER_D newD;
	newD.kSigr = oldD.kSigr;
	if (DlgToData(sigrD, newD) == FALSE)
		return FALSE;

	BOOL bCheckName = (oldSigrD.strName != sigrD.strName);
	if (CheckData(sigrD, newD, bCheckName) == FALSE)
		return FALSE;

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Modify_SIGR_LOWER)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	if (m_pDoc->m_pEditData->ModifySigr(oldD.kSigr, sigrD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}

	if (m_pDoc->m_pEditData->ModifySigrLower(key, newD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIGR_LOWER_CMD);
	return TRUE;
}

BOOL CSeisCvlJpSetTargetLowerDlg::DelData(const T_SIGR_LOWER_K& key)
{
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Exist(key))
		return FALSE;

	T_SIGR_LOWER_D lowerD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(key, lowerD) == FALSE)
		return FALSE;

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Delete_SIGR_LOWER)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	ASSERT(lowerD.kSigr == key);
// 	if (m_pDoc->m_pEditData->DelSigr(lowerD.kSigr) == FALSE) {
// 		ASSERT(FALSE);
// 		m_pDoc->m_pUndoCtrl->CancelEditDB();
// 		return FALSE;
// 	}
	if (m_pDoc->m_pEditData->DelSigrLower(key) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}

	ResetAllControls();
	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIGR_LOWER_CMD);
	return TRUE;
}

BOOL CSeisCvlJpSetTargetLowerDlg::DlgToData(OUT T_SIGR_D& rSigrD, OUT T_SIGR_LOWER_D& rLowerD)
{
	CString str;
	//명칭
	m_name.GetWindowText(rSigrD.strName);
	//상부구조 관성력 작용 절점위치
	CArray<T_NODE_K, T_NODE_K> nodes;
	//(교축방향)
	m_nodeAxis.GetWindowText(str);
	GetNodeList(str, nodes);
	//개수 체크(1개만 허용)
	if (nodes.GetCount() != 1) {
		str = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_INPUT_NODE_AXIS);
		AfxMessageBox(str);
		return FALSE;
	}
	rLowerD.kNodeAxis = nodes[0];
	//(교축직각방향)
	nodes.RemoveAll();
	m_nodePerp.GetWindowText(str);
	GetNodeList(str, nodes);
	//개수 체크(1개만 허용)
	if (nodes.GetCount() != 1) {
		str = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_INPUT_NODE_PERP);
		AfxMessageBox(str);
		return FALSE;
	}
	rLowerD.kNodePerp = nodes[0];
	//교각의 재료타입
	m_materialType.GetWindowText(str);
	if (str == COMBO_MATERIAL_TYPE_RC)
		rLowerD.nMaterialType = T_SIGR_LOWER_D::kRC;
	else if (str == COMBO_MATERIAL_TYPE_ST)
		rLowerD.nMaterialType = T_SIGR_LOWER_D::kSteel;
	else if (str == COMBO_MATERIAL_TYPE_SRC)
		rLowerD.nMaterialType = T_SIGR_LOWER_D::kSRC;
	//지승
	ListToData_BEAR(rLowerD.BEAR_D);
	//기둥
	ListToData_COLM(rLowerD.COLM_D);
	//보
	ListToData_BEAM(rLowerD.BEAM_D);
	//기초
	ListToData_FOOT(rLowerD.FOOT_D);

	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::ListToData_BEAR(CArray<T_SIGR_LOWER_BEAR_D, T_SIGR_LOWER_BEAR_D&>& rData)
{
	int nSize = m_bearingList.GetItemCount();
	for (int nItem = 0; nItem < nSize; nItem++)
	{
		T_SIGR_LOWER_BEAR_D data;
		//명칭
		data.strName = m_bearingList.GetItemText(nItem, 0);
		//양단 절점
		CString str = m_bearingList.GetItemText(nItem, 1);
		GetNodeList(str, data.aPosition);
		//교축방향 단면력
		str = m_bearingList.GetItemText(nItem, 2);
		if (str == COMBO_SHEAR_AXIS_DIR_Y)
			data.nDirShear = seis_jp::kY_Dir;
		else if (str == COMBO_SHEAR_AXIS_DIR_Z)
			data.nDirShear = seis_jp::kZ_Dir;
		else if (str == COMBO_SHEAR_AXIS_DIR_YZ)
			data.nDirShear = seis_jp::kYZ_Dir;
		//지승선
		str = m_bearingList.GetItemText(nItem, 3);
		if (str == _T("1"))
			data.nLineNum = T_SIGR_LOWER_BEAR_D::kLine1;
		else if (str == _T("2"))
			data.nLineNum = T_SIGR_LOWER_BEAR_D::kLine2;
		//지승조건
		str = m_bearingList.GetItemText(nItem, 4);
		if (str == COMBO_BEAR_TYPE_RIGID_LINK)
			data.nType = T_SIGR_LOWER_BEAR_D::kRigidLink;
		else if (str == COMBO_BEAR_TYPE_FIXED)
			data.nType = T_SIGR_LOWER_BEAR_D::kFixed;
		else if (str == COMBO_BEAR_TYPE_MOVING)
			data.nType = T_SIGR_LOWER_BEAR_D::kMovable;
		else if (str == COMBO_BEAR_TYPE_ELASTIC)
			data.nType = T_SIGR_LOWER_BEAR_D::kElastic;
		else if (str == COMBO_BEAR_TYPE_VIBRA_ISOLA)
			data.nType = T_SIGR_LOWER_BEAR_D::kSeismicIsolation;

		rData.Add(data);
	}
}

void CSeisCvlJpSetTargetLowerDlg::ListToData_COLM(CArray<T_SIGR_LOWER_COLM_D, T_SIGR_LOWER_COLM_D&>& rData)
{
	int nSize = m_columnList.GetItemCount();
	for (int nItem = 0; nItem < nSize; nItem++)
	{
		T_SIGR_LOWER_COLM_D data;
		//명칭
		data.strName = m_columnList.GetItemText(nItem, 0);
		//대상 요소
		CString str = m_columnList.GetItemText(nItem, 1);
		GetNodeList(str, data.aInvestigatedElem);
		m_pDoc->m_pAttrCtrl2->SortTozyxElemList(data.aInvestigatedElem);
		//교축방향 단면력
		str = m_columnList.GetItemText(nItem, 2);
		if (str == COMBO_SHEAR_AXIS_DIR_Y)
			data.nDirShear = seis_jp::kY_Dir;
		else if (str == COMBO_SHEAR_AXIS_DIR_Z)
			data.nDirShear = seis_jp::kZ_Dir;
		else if (str == COMBO_SHEAR_AXIS_DIR_YZ)
			data.nDirShear = seis_jp::kYZ_Dir;
		//철근 단락부
		str = m_columnList.GetItemText(nItem, 3);
		if (str.IsEmpty() == true || str == _T("-"))
			data.bRebarCut = FALSE;
		else
			data.bRebarCut = TRUE;

		if (data.bRebarCut == TRUE)
			ListToData_COLM_RebarCut(str, data);

		rData.Add(data);
	}
}

void CSeisCvlJpSetTargetLowerDlg::ListToData_COLM_RebarCut(const CString& strGirdText, OUT T_SIGR_LOWER_COLM_D& rData)
{
	CString delimiter = _T(",");
	CStringArray results;
	CStrParser::SplitString(strGirdText, delimiter, results);

	ASSERT(results.GetCount() == 2);
	if (results.GetCount() != 2)
		return;

	//교축방향
	CString strAxis = results[0];
	delimiter = _T("_");
	CStringArray results2;
	CStrParser::SplitString(strAxis, delimiter, results2);

	ASSERT(results2.GetCount() == 2);
	if (results2.GetCount() != 2)
		return;

	T_KEY_LIST elemKeys;
	GetNodeList(results2[0], elemKeys);
	ASSERT(elemKeys.GetCount() == 1);
	if (elemKeys.GetCount() > 0)
		rData.kAxialElem = elemKeys[0];

	if (results2[1] == _T("I"))
		rData.nAxialLoc = T_SIGR_LOWER_COLM_D::kI;
	else if (results2[1] == _T("J"))
		rData.nAxialLoc = T_SIGR_LOWER_COLM_D::kJ;

	//교축직각방향
	CString strPerpAxis = results[1];
	delimiter = _T("_");
	results2.RemoveAll();
	CStrParser::SplitString(strPerpAxis, delimiter, results2);

	ASSERT(results2.GetCount() == 2);
	if (results2.GetCount() != 2)
		return;

	elemKeys.RemoveAll();
	GetNodeList(results2[0], elemKeys);
	ASSERT(elemKeys.GetCount() == 1);
	if (elemKeys.GetCount() > 0)
		rData.kAxisPerpElem = elemKeys[0];

	if (results2[1] == _T("I"))
		rData.nAxisPerpLoc = T_SIGR_LOWER_COLM_D::kI;
	else if (results2[1] == _T("J"))
		rData.nAxisPerpLoc = T_SIGR_LOWER_COLM_D::kJ;
}

void CSeisCvlJpSetTargetLowerDlg::ListToData_BEAM(CArray<T_SIGR_LOWER_BEAM_D, T_SIGR_LOWER_BEAM_D&>& rData)
{
	int nSize = m_beamList.GetItemCount();
	for (int nItem = 0; nItem < nSize; nItem++)
	{
		T_SIGR_LOWER_BEAM_D data;
		//명칭
		data.strName = m_beamList.GetItemText(nItem, 0);
		//대상 요소
		CString str = m_beamList.GetItemText(nItem, 1);
		GetNodeList(str, data.aInvestigatedElem);
		m_pDoc->m_pAttrCtrl2->SortTozyxElemList(data.aInvestigatedElem);

		//교축방향 단면력
		str = m_beamList.GetItemText(nItem, 2);
		if (str == COMBO_SHEAR_AXIS_DIR_Y)
			data.nDirShear = seis_jp::kY_Dir;
		else if (str == COMBO_SHEAR_AXIS_DIR_Z)
			data.nDirShear = seis_jp::kZ_Dir;
		else if (str == COMBO_SHEAR_AXIS_DIR_YZ)
			data.nDirShear = seis_jp::kYZ_Dir;

		rData.Add(data);
	}
}

void CSeisCvlJpSetTargetLowerDlg::ListToData_FOOT(CArray<T_SIGR_LOWER_FOOT_D, T_SIGR_LOWER_FOOT_D&>& rData)
{
	int nSize = m_footingList.GetItemCount();
	for (int nItem = 0; nItem < nSize; nItem++)
	{
		T_SIGR_LOWER_FOOT_D data;
		//명칭
		data.strName = m_footingList.GetItemText(nItem, 0);
		//대상 요소
		CString str = m_footingList.GetItemText(nItem, 1);
		GetNodeList(str, data.aInvestigatedElem);
		m_pDoc->m_pAttrCtrl2->SortTozyxElemList(data.aInvestigatedElem);

		//교축방향 단면력
		//str = m_footingList.GetItemText(nItem, 2);
		//if (str == COMBO_SHEAR_AXIS_DIR_Y)
		//	data.nDir = T_SIGR_LOWER_FOOT_D::kY_Dir;
		//else if (str == COMBO_SHEAR_AXIS_DIR_Z)
		//	data.nDir = T_SIGR_LOWER_FOOT_D::kZ_Dir;
		//else if (str == COMBO_SHEAR_AXIS_DIR_YZ)
		//	data.nDir = T_SIGR_LOWER_FOOT_D::kYZ_Dir;

		rData.Add(data);
	}
}

BOOL CSeisCvlJpSetTargetLowerDlg::CheckData(IN T_SIGR_D& sigrD, const T_SIGR_LOWER_D& lowerD, BOOL bCheckName/*=TRUE*/)
{
	CString msg;
	//내진조사 그룹이름 공백체크
	if (sigrD.strName.IsEmpty() || sigrD.strName == _T("")) {
		msg = _LS(IDS_IDD_TM_SIGR_MSG_EMPTY_SIGRNAME);
		AfxMessageBox(msg);
		return FALSE;
	}
	//내진조사 그룹이름 중복체크
	if (bCheckName == TRUE && m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKey(sigrD.strName) != 0) {
		msg = _LS(IDS_IDD_TM_SIGR_MSG_EXIST_SIGRNAME);
		AfxMessageBox(msg);
		return FALSE;
	}
	//상부구조 관성력 절점 위치가 입력 되었는지 체크
	if (lowerD.kNodeAxis == 0 || lowerD.kNodePerp == 0) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_NODE);
		AfxMessageBox(msg);
		return FALSE;
	}

	//기둥이 하나 이상 있는지 체크
	if (lowerD.COLM_D.GetCount() < 1) {
		msg = _LS(IDS_IDD_TM_SIGR_LOWER_MSG_EMPTY_COLUMN);
		AfxMessageBox(msg);
		return FALSE;
	}
	
	//내진조사 모든 그룹에 요소가 등록되어 있지 않았는지 확인
	T_KEY_LIST aInvestigatedElem;
	for (int i = 0; i < lowerD.COLM_D.GetCount(); i++) aInvestigatedElem.Append(lowerD.COLM_D[i].aInvestigatedElem);
	for (int i = 0; i < lowerD.BEAM_D.GetCount(); i++) aInvestigatedElem.Append(lowerD.BEAM_D[i].aInvestigatedElem);
	for (int i = 0; i < lowerD.FOOT_D.GetCount(); i++) aInvestigatedElem.Append(lowerD.FOOT_D[i].aInvestigatedElem);

	if (aInvestigatedElem.GetCount() < 1) {
		msg = _LS(IDS_IDD_TM_SIGR_MSG_EMPTY_ELEMENT);
		AfxMessageBox(msg);
		return FALSE;
	}

	if (m_pParent->ExistElementAtSigrUser(lowerD.kSigr, aInvestigatedElem) == TRUE) return FALSE;
	if (m_pParent->ExistElementAtSigrUpper(lowerD.kSigr, aInvestigatedElem) == TRUE) return FALSE;
	if (m_pParent->ExistElementAtSigrLower(lowerD.kSigr, aInvestigatedElem) == TRUE) return FALSE;

	return TRUE;
}

void CSeisCvlJpSetTargetLowerDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SIGR_LOWER_K Key, KeyBack;
	T_SIGR_LOWER_D Data, DataBack;

	BOOL bUpdated = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_SIGR_LOWER_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetSigrLower(nKey, Key, Data);
			bUpdated = InsertItem(Key, Data);
			break;
		case(UR_SIGR_LOWER_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetSigrLower(nKey, Key, Data);
			bUpdated = DeleteItem(Key, Data);
			break;
		case(UR_SIGR_LOWER_MFD):
			// 여기서는 저장만 하고 MFS에서 처리한다.
			pViewBuff->GetSigrLower(nKey, Key, Data);
			KeyBack = Key;
			DataBack = Data;
			bUpdated = TRUE;
			break;
		case(UR_SIGR_LOWER_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bUpdated);
			pViewBuff->GetSigrLower(nKey, Key, Data);
			bUpdated = ModifyItem(KeyBack, DataBack);
			break;
		default:
			break;
		}
	}
}

BOOL CSeisCvlJpSetTargetLowerDlg::InsertItem(const T_SIGR_LOWER_K& Key, const T_SIGR_LOWER_D& Data)
{
	int nItem = m_sigrLowerList.GetItemCount();
	for (int i = 0; i < LIST_COLUMN_NUM_LOWER; i++)
	{
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem = i;
		CString str = DataToStr(i, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_sigrLowerList.InsertItem(&lvitem);
		}
		else m_sigrLowerList.SetItem(&lvitem);

		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CSeisCvlJpSetTargetLowerDlg::DeleteItem(const T_SIGR_LOWER_K& Key, const T_SIGR_LOWER_D& Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;

	int nItem = m_sigrLowerList.FindItem(&FindInfo);
	if (nItem == -1) return TRUE;

	return m_sigrLowerList.DeleteItem(nItem);
}

BOOL CSeisCvlJpSetTargetLowerDlg::ModifyItem(const T_SIGR_LOWER_K& Key, const T_SIGR_LOWER_D& Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;

	int nItem = m_sigrLowerList.FindItem(&FindInfo);
	if (nItem == -1) return TRUE;

	for (int i = 0; i < LIST_COLUMN_NUM_LOWER; i++)
	{
		CString str = DataToStr(i, Data);
		m_sigrLowerList.SetItemText(nItem, i, str);
	}

	LVITEM lvitem;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 0;
	lvitem.mask = LVIF_PARAM;
	lvitem.lParam = (LPARAM)Key;

	return m_sigrLowerList.SetItem(&lvitem);
}

void CSeisCvlJpSetTargetLowerDlg::UpdateDialog(IN T_SIGR_LOWER_D& data)
{
	//모델상에서 해당 요소가 선택/하이라이트
	CArray<T_NODE_K, T_NODE_K> aNodes;
	aNodes.Add(data.kNodeAxis);
	aNodes.Add(data.kNodePerp);

	CArray<T_ELEM_K, T_ELEM_K> aInvestigatedElem;
	for (int i = 0; i < data.COLM_D.GetCount(); i++) aInvestigatedElem.Append(data.COLM_D[i].aInvestigatedElem);
	for (int i = 0; i < data.BEAM_D.GetCount(); i++) aInvestigatedElem.Append(data.BEAM_D[i].aInvestigatedElem);
	for (int i = 0; i < data.FOOT_D.GetCount(); i++) aInvestigatedElem.Append(data.FOOT_D[i].aInvestigatedElem);
	
	m_pDoc->m_pViewCtrl->SelectNode(NULL, aNodes, FALSE);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aInvestigatedElem, FALSE);
	//명칭
	CString str = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetSigrName(data.kSigr);
	m_name.SetWindowText(str);
	//상부구조 관성력 작용 위치(교축방향)
	str.Format(_T("%d"), data.kNodeAxis);
	m_nodeAxis.SetWindowText(str);
	//상부구조 관성력 작용 위치(교축직각방향)
	str.Format(_T("%d"), data.kNodePerp);
	m_nodePerp.SetWindowText(str);
	//교각의 재료 타입
	if (data.nMaterialType == T_SIGR_LOWER_D::kRC)
		str = COMBO_MATERIAL_TYPE_RC;
	else if (data.nMaterialType == T_SIGR_LOWER_D::kSteel)
		str = COMBO_MATERIAL_TYPE_ST;
	else if (data.nMaterialType == T_SIGR_LOWER_D::kSRC)
		str = COMBO_MATERIAL_TYPE_SRC;
	if (str.IsEmpty() == false) {
		int iIndex = m_materialType.FindStringExact(-1, str);
		if (iIndex >= 0) m_materialType.SetCurSel(iIndex);
		else			 m_materialType.SetCurSel(0);
	}
	//지승
	m_bearingList.DeleteAllItems();
	for (int nItem = 0; nItem < data.BEAR_D.GetCount(); nItem++)
		InsertItem_BEAR(nItem, data.BEAR_D[nItem]);
	//기둥
	m_columnList.DeleteAllItems();
	for (int nItem = 0; nItem < data.COLM_D.GetCount(); nItem++)
		InsertItem_COLM(nItem, data.COLM_D[nItem]);
	//보
	m_beamList.DeleteAllItems();
	for (int nItem = 0; nItem < data.BEAM_D.GetCount(); nItem++)
		InsertItem_BEAM(nItem, data.BEAM_D[nItem]);
	//기초
	m_footingList.DeleteAllItems();
	for (int nItem = 0; nItem < data.FOOT_D.GetCount(); nItem++)
		InsertItem_FOOT(nItem, data.FOOT_D[nItem]);
}

BOOL CSeisCvlJpSetTargetLowerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		CRect rBearing;
		m_bearingList.GetWindowRect(rBearing);
		if (rBearing.PtInRect(pMsg->pt))
		{
			ClearSelectList(&m_bearingList);
			ClearSelectList(&m_columnList);
			ClearSelectList(&m_beamList);
			ClearSelectList(&m_footingList);
		}

		CRect rSigrLower;
		m_sigrLowerList.GetWindowRect(rSigrLower);
		if (rSigrLower.PtInRect(pMsg->pt))
		{
			ClearSelectList(&m_sigrLowerList);
		}
	}
	return CMenuBarChildDlg::PreTranslateMessage(pMsg);
}

void CSeisCvlJpSetTargetLowerDlg::ClearSelectList(CListCtrl* pList)
{
	if (pList == NULL) return;

	int nCount = pList->GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		pList->SetItemState(i, 0, LVIS_SELECTED);
	}
}