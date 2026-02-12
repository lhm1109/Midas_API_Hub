// CSeisCvlJpSetTargetUpperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetTargetUpperDlg.h"
#include "SeisCvlJpSetTargetDlg.h"

#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COMBO_SHEAR_AXIS_DIR_Y	_LS(IDS_IDD_TM_SIGR_UPPER_IDC_COMBO_SHEAR_AXIS_DIR_Y)
#define COMBO_SHEAR_AXIS_DIR_Z	_LS(IDS_IDD_TM_SIGR_UPPER_IDC_COMBO_SHEAR_AXIS_DIR_Z)
#define COMBO_SHEAR_AXIS_DIR_YZ	_LS(IDS_IDD_TM_SIGR_UPPER_IDC_COMBO_SHEAR_AXIS_DIR_YZ)
#define LIST_SIGR_NAME			_LS(IDS_IDD_TM_SIGR_UPPER_IDC_LIST_SIGR_NAME)
#define LIST_SIGR_ELEMENT		_LS(IDS_IDD_TM_SIGR_UPPER_IDC_LIST_SIGR_ELEMENT)
#define LIST_SIGR_NODE_LEFT		_LS(IDS_IDD_TM_SIGR_UPPER_IDC_LIST_SIGR_NODE_LEFT)
#define LIST_SIGR_NODE_RIGHT	_LS(IDS_IDD_TM_SIGR_UPPER_IDC_LIST_SIGR_NODE_RIGHT)
#define LIST_SIGR_AXIS_DIR		_LS(IDS_IDD_TM_SIGR_UPPER_IDC_LIST_SIGR_AXIS_DIR)

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetUpperDlg dialog


CSeisCvlJpSetTargetUpperDlg::CSeisCvlJpSetTargetUpperDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlJpSetTargetUpperDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	m_pParent = (CSeisCvlJpSetTargetDlg*)pParent;
}

CSeisCvlJpSetTargetUpperDlg::~CSeisCvlJpSetTargetUpperDlg()
{
	m_pDoc = NULL;
}

void CSeisCvlJpSetTargetUpperDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Radio(pDX, IDC_TM_SIGR_UPPER_ADD_RDO, m_nOption);
	DDX_Control(pDX, IDC_TM_SIGR_UPPER_NAME_EDT, m_name);
	DDX_Control(pDX, IDC_TM_SIGR_UPPER_EXCLUD_ELEM_EDT, m_elements);
	DDX_Control(pDX, IDC_TM_SIGR_UPPER_POS_LEFT_EDT, m_nodeL);
	DDX_Control(pDX, IDC_TM_SIGR_UPPER_POS_RIGHT_EDT, m_nodeR);
	DDX_Control(pDX, IDC_TM_SIGR_UPPER_DIR_CMB, m_shearAxisDir);
	DDX_Control(pDX, IDC_TM_SIGR_UPPER_LST, m_sigrUpperList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetTargetUpperDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlJpSetTargetUpperDlg)
	ON_EN_SETFOCUS(IDC_TM_SIGR_UPPER_EXCLUD_ELEM_EDT, OnSetFocusElemEdt)
	ON_EN_SETFOCUS(IDC_TM_SIGR_UPPER_POS_LEFT_EDT, OnSetFocusNodeLEdt)
	ON_EN_SETFOCUS(IDC_TM_SIGR_UPPER_POS_RIGHT_EDT, OnSetFocusNodeREdt)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SIGR_UPPER_LST, OnNMClickSigrUpperList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSeisCvlJpSetTargetUpperDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	InitControl();
	InitSigrUpperListCtrl();
	return TRUE;// return TRUE unless you set the focus to a control
				// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlJpSetTargetUpperDlg::OnSetFocusElemEdt()
{
	m_nodeL.Disconnect();
	m_nodeR.Disconnect();

	CString strPreviousText = _T("");
	m_elements.GetWindowText(strPreviousText);
	
	m_elements.Connect(SC_ID_ELEM, &m_elements);
	
	m_elements.SetWindowText(strPreviousText);
	m_elements.SelectByStr(strPreviousText);

	ResetViewControls();
	
	CArray<T_ELEM_K, T_ELEM_K> aElements;
	GetNodeList(strPreviousText, aElements);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aElements, FALSE);
}

void CSeisCvlJpSetTargetUpperDlg::OnSetFocusNodeLEdt()
{
	m_nodeR.Disconnect();
	m_elements.Disconnect();
	m_elements.Invalidate(FALSE);

	CString strPreviousText = _T("");
	m_nodeL.GetWindowText(strPreviousText);

	m_nodeL.Connect(CURSOR_ID_NODE);
	
	ResetViewControls();

	CArray<T_NODE_K, T_NODE_K> aNodes;
	m_nodeL.SetWindowText(strPreviousText);
	GetNodeList(strPreviousText, aNodes);
	m_pDoc->m_pViewCtrl->SelectNode(NULL, aNodes, FALSE);
}

void CSeisCvlJpSetTargetUpperDlg::OnSetFocusNodeREdt()
{
	m_nodeL.Disconnect();
	m_elements.Disconnect();
	m_elements.Invalidate(FALSE);

	CString strPreviousText = _T("");
	m_nodeR.GetWindowText(strPreviousText);

	m_nodeR.Connect(CURSOR_ID_NODE);

	ResetViewControls();

	CArray<T_NODE_K, T_NODE_K> aNodes;
	m_nodeR.SetWindowText(strPreviousText);
	GetNodeList(strPreviousText, aNodes);
	m_pDoc->m_pViewCtrl->SelectNode(NULL, aNodes, FALSE);
}

void CSeisCvlJpSetTargetUpperDlg::OnNMClickSigrUpperList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1) {
		*pResult = 0;
		return;
	}

	int nItem = m_sigrUpperList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		ResetControls();
		*pResult = 0;
		return;
	}

	nItem = pNMListView->iItem;
	T_SIGR_UPPER_K key = (T_SIGR_UPPER_K)m_sigrUpperList.GetItemData(nItem);

	ResetControls();
	UpdataDialog(key);
}

void CSeisCvlJpSetTargetUpperDlg::Execute()
{
	UpdateData(TRUE);

	BOOL bModify = FALSE;
	int nItem = m_sigrUpperList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem != -1) bModify = TRUE;

	T_SIGR_UPPER_K sigrUpperK;
	if (nItem != -1)
		sigrUpperK = (T_SIGR_UPPER_K)m_sigrUpperList.GetItemData(nItem);

	BOOL bResult = FALSE;
	if (m_nOption == 0) {
		if (bModify) {
			bResult = ModData(sigrUpperK);
		}
		else {//Add
			bResult = AddData();
		}
	}
	else {//Delete
		bResult = DelData(sigrUpperK);
	}

	if (bResult) {
		ResetControls();
		OnSetFocusElemEdt();
	}
}

void CSeisCvlJpSetTargetUpperDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetUserDlg diagnostics
// 
void CSeisCvlJpSetTargetUpperDlg::InitControl()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SIGR_UPPER_ADD_RDO);
	pBtn->SetCheck(1);
	m_nOption = 0;

	m_nodeL.SetCount(1);
	m_nodeR.SetCount(1);
	//m_nodeL.SetMoveCursur(TRUE);
	//m_nodeR.SetMoveCursur(TRUE);
	m_elements.Connect(SC_ID_ELEM, &m_elements);

	m_shearAxisDir.ResetContent();
	m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Y);
	m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Z);
	m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_YZ);
	m_shearAxisDir.SetCurSel(0);

	ResetControls();
}

void CSeisCvlJpSetTargetUpperDlg::InitSigrUpperListCtrl()
{
	SetHeaderTitleListCtrl();

	CArray<T_SIGR_UPPER_K, T_SIGR_UPPER_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIGR_UPPER_K& key = keys[i];
		T_SIGR_UPPER_D data;

		m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(key, data);
		InsertItemListCtrl(key, data);
	}
}

void CSeisCvlJpSetTargetUpperDlg::SetHeaderTitleListCtrl()
{
	CString aTitle[] = { LIST_SIGR_NAME, LIST_SIGR_ELEMENT, LIST_SIGR_NODE_LEFT, LIST_SIGR_NODE_RIGHT, LIST_SIGR_AXIS_DIR };
	int nColWidth[] = { 70, 80, 60, 60, 60 };

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_sigrUpperList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_sigrUpperList.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < 5; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_sigrUpperList.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

BOOL CSeisCvlJpSetTargetUpperDlg::InsertItemListCtrl(const T_SIGR_UPPER_K& Key, T_SIGR_UPPER_D& Data)
{
	int nItem = m_sigrUpperList.GetItemCount();
	for (int i = 0; i < 5; i++)
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
			nItem = m_sigrUpperList.InsertItem(&lvitem);
		}
		else m_sigrUpperList.SetItem(&lvitem);

		str.ReleaseBuffer();
	}

	return TRUE;
}

CString CSeisCvlJpSetTargetUpperDlg::DataToStr(const int& index, const T_SIGR_UPPER_D& data)
{
	CString str = _T("");

	switch (index)
	{
	case 0://명칭
		str = m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->GetSigrName(data.kSigr);
		break;
	case 1://조사대상 요소
		GetStrKeyList(data.aElements, str);
		break;
	case 2://응답변위 산출 절점(거더 좌단)
		str.Format(_T("%d"), data.kNodeL);
		break;
	case 3://응답변위 산출 절점(거더 우단)
		str.Format(_T("%d"), data.kNodeR);
		break;
	case 4://교축방향
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


BOOL CSeisCvlJpSetTargetUpperDlg::AddData()
{
	T_SIGR_D sigrD;
	sigrD.nSubType = T_SIGR_D::kUpper;
	T_SIGR_UPPER_D upperD;
	if (DlgToData(sigrD, upperD) == FALSE)
		return FALSE;

	if (CheckData(sigrD, upperD) == FALSE)
		return FALSE;

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_SIGR_UPPER)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	T_SIGR_K sigrKey = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetStartNum();
	if (m_pDoc->m_pEditData->AddSigr(sigrKey, sigrD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}

	upperD.kSigr = sigrKey;
	if (m_pDoc->m_pEditData->AddSigrUpper(sigrKey, upperD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIGR_UPPER_CMD);
	return TRUE;
}

BOOL CSeisCvlJpSetTargetUpperDlg::ModData(const T_SIGR_UPPER_K& key)
{
	if (m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Exist(key) == FALSE)
		return FALSE;

	T_SIGR_UPPER_D oldD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(key, oldD) == FALSE)
		return FALSE;

	T_SIGR_D oldSigrD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(oldD.kSigr, oldSigrD) == FALSE)
		return FALSE;

	T_SIGR_D sigrD;
	sigrD.nSubType = T_SIGR_D::kUpper;
	T_SIGR_UPPER_D newD;
	newD.kSigr = oldD.kSigr;
	if (DlgToData(sigrD, newD) == FALSE)
		return FALSE;

	BOOL bCheckName = (oldSigrD.strName != sigrD.strName);
	if (CheckData(sigrD, newD, bCheckName) == FALSE)
		return FALSE;

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Modify_SIGR_UPPER)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	if (m_pDoc->m_pEditData->ModifySigr(oldD.kSigr, sigrD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}

	if (m_pDoc->m_pEditData->ModifySigrUpper(key, newD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIGR_UPPER_CMD);
	return TRUE;
}

BOOL CSeisCvlJpSetTargetUpperDlg::DelData(const T_SIGR_UPPER_K& key)
{
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Exist(key))
		return FALSE;

	T_SIGR_UPPER_D upperD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(key, upperD) == FALSE)
		return FALSE;

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Delete_SIGR_UPPER)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	ASSERT(upperD.kSigr == key);
// 	if (m_pDoc->m_pEditData->DelSigr(upperD.kSigr) == FALSE) {
// 		ASSERT(FALSE);
// 		m_pDoc->m_pUndoCtrl->CancelEditDB();
// 		return FALSE;
// 	}
	if (m_pDoc->m_pEditData->DelSigrUpper(key) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIGR_UPPER_CMD);
	return TRUE;
}

BOOL CSeisCvlJpSetTargetUpperDlg::DlgToData(OUT T_SIGR_D& rSigrD, OUT T_SIGR_UPPER_D& rUpperD)
{
	CString str;
	//명칭
	m_name.GetWindowText(rSigrD.strName);
	//조사대상 요소
	m_elements.GetWindowText(str);
	GetNodeList(str, rUpperD.aElements);
	//응답변위 산출 절점
	CArray<T_NODE_K, T_NODE_K> nodes;
	//(거더 좌단)
	m_nodeL.GetWindowText(str);
	GetNodeList(str, nodes);
	//개수 체크(1개만 허용)
	if (nodes.GetCount() != 1) {
		str = _LS(IDS_IDD_TM_SIGR_UPPER_MSG_INPUT_NODE_LEFT);
		AfxMessageBox(str);
		return FALSE;
	}
	rUpperD.kNodeL = nodes[0];
	//응답변위 산출 절점(거더 우단)
	nodes.RemoveAll();
	m_nodeR.GetWindowText(str);
	GetNodeList(str, nodes);
	//개수 체크(1개만 허용)
	if (nodes.GetCount() != 1) {
		str = _LS(IDS_IDD_TM_SIGR_UPPER_MSG_INPUT_NODE_RIGHT);
		AfxMessageBox(str);
		return FALSE;
	}
	rUpperD.kNodeR = nodes[0];
	//교축방향 전단력
	m_shearAxisDir.GetWindowText(str);
	rUpperD.nDirShear = seis_jp::kDirShear_0;
	if (str == COMBO_SHEAR_AXIS_DIR_Y)
		rUpperD.nDirShear = seis_jp::kY_Dir;
	else if (str == COMBO_SHEAR_AXIS_DIR_Z)
		rUpperD.nDirShear = seis_jp::kZ_Dir;
	else if (str == COMBO_SHEAR_AXIS_DIR_YZ)
		rUpperD.nDirShear = seis_jp::kYZ_Dir;

	return TRUE;
}

BOOL CSeisCvlJpSetTargetUpperDlg::CheckData(IN T_SIGR_D& sigrD, const T_SIGR_UPPER_D& upperD, BOOL bCheckName/*=TRUE*/)
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

	//응답변위 산출위치 공백체크
	if (upperD.kNodeL == 0 || upperD.kNodeR == 0) {
		msg = _LS(IDS_IDD_TM_SIGR_UPPER_MSG_EXCEEDED_NUMBER_OF_NODE);
		AfxMessageBox(msg);
		return FALSE;
	}

	//기둥과 같은 수직 부재가 포함되어 있는지 체크(보 부재만 허용)
	if (IsOnlyBeam(upperD.aElements) == FALSE) {		
		return FALSE;
	}
	
	//내진조사 모든 그룹에 요소가 등록되어 있지 않았는지 확인
	if (m_pParent->ExistElementAtSigrUser(upperD.kSigr, upperD.aElements) == TRUE) return FALSE;
	if (m_pParent->ExistElementAtSigrLower(upperD.kSigr, upperD.aElements) == TRUE) return FALSE;
	// #3073 곡선교 모델을 테스트로 인해 좌,우단 절점을 달리하는 중복되는 상부구조 요소그룹을 만들 필요가 있어 주석처리 함.
	//if (m_pParent->ExistElementAtSigrUpper(upperD.kSigr, upperD.aElements) == TRUE) return FALSE;

	return TRUE;
}

BOOL CSeisCvlJpSetTargetUpperDlg::IsOnlyBeam(const T_ELEM_K_LIST& elemKeys)
{
	CString msg;
	for (int i = 0; i < elemKeys.GetCount(); i++)
	{
		const T_ELEM_K& kElem = elemKeys[i];
		int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(kElem);
		if (nMembType != D_MBTP_BEAM) {
			msg.Format(_LS(IDS_IDD_TM_SIGR_UPPER_MSG_IS_NOT_BEAM), kElem);
			AfxMessageBox(msg);
			return FALSE;
		}
	}

	return TRUE;
}

void CSeisCvlJpSetTargetUpperDlg::ResetControls()
{
	m_name.SetWindowText(_T(""));
	m_elements.SetWindowText(_T(""));
	m_nodeL.SetWindowText(_T(""));
	m_nodeR.SetWindowText(_T(""));
	ResetViewControls();
}

void CSeisCvlJpSetTargetUpperDlg::ResetViewControls()
{
	m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CSeisCvlJpSetTargetUpperDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SIGR_UPPER_K Key, KeyBack;
	T_SIGR_UPPER_D Data, DataBack;

	BOOL bUpdated = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_SIGR_UPPER_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetSigrUpper(nKey, Key, Data);
			bUpdated = InsertItem(Key, Data);
			break;
		case(UR_SIGR_UPPER_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetSigrUpper(nKey, Key, Data);
			bUpdated = DeleteItem(Key, Data);
			break;
		case(UR_SIGR_UPPER_MFD):
			// 여기서는 저장만 하고 MFS에서 처리한다.
			pViewBuff->GetSigrUpper(nKey, Key, Data);
			KeyBack = Key;
			DataBack = Data;
			bUpdated = TRUE;
			break;
		case(UR_SIGR_UPPER_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bUpdated);
			pViewBuff->GetSigrUpper(nKey, Key, Data);
			bUpdated = ModifyItem(KeyBack, DataBack);
			break;
		default:
			break;
		}
	}
}

BOOL CSeisCvlJpSetTargetUpperDlg::InsertItem(const T_SIGR_UPPER_K& Key, const T_SIGR_UPPER_D& Data)
{
	int nItem = m_sigrUpperList.GetItemCount();
	for (int i = 0; i < 5; i++)
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
			nItem = m_sigrUpperList.InsertItem(&lvitem);
		}
		else m_sigrUpperList.SetItem(&lvitem);

		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CSeisCvlJpSetTargetUpperDlg::DeleteItem(const T_SIGR_UPPER_K& Key, const T_SIGR_UPPER_D& Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;

	int nItem = m_sigrUpperList.FindItem(&FindInfo);
	if (nItem == -1) return TRUE;

	return m_sigrUpperList.DeleteItem(nItem);
}

BOOL CSeisCvlJpSetTargetUpperDlg::ModifyItem(const T_SIGR_UPPER_K& Key, const T_SIGR_UPPER_D& Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;

	int nItem = m_sigrUpperList.FindItem(&FindInfo);
	if (nItem == -1) return TRUE;

	for (int i = 0; i < 5; i++)
	{
		CString str = DataToStr(i, Data);
		m_sigrUpperList.SetItemText(nItem, i, str);
	}

	LVITEM lvitem;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 0;
	lvitem.mask = LVIF_PARAM;
	lvitem.lParam = (LPARAM)Key;

	return m_sigrUpperList.SetItem(&lvitem);
}

void CSeisCvlJpSetTargetUpperDlg::UpdataDialog(const T_SIGR_UPPER_K& key)
{
	T_SIGR_UPPER_D data;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(key, data) == FALSE)
		return;
	//모델상에서 해당 요소가 선택/하이라이트
	CArray<T_NODE_K, T_NODE_K> aNodes;
	aNodes.Add(data.kNodeL);
	aNodes.Add(data.kNodeR);

	m_pDoc->m_pViewCtrl->SelectNode(NULL, aNodes, FALSE);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, data.aElements, FALSE);
	//명칭
	CString str = m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->GetSigrName(key);
	m_name.SetWindowText(str);
	//조사대상
	str = _T("");
	GetStrKeyList(data.aElements, str);
	m_elements.SetWindowText(str);
	//응답변위 산출 절점(거더 좌단)
	str.Format(_T("%d"), data.kNodeL);
	m_nodeL.SetWindowText(str);
	//응답변위 산출 절점(거더 우단)
	str.Format(_T("%d"), data.kNodeR);
	m_nodeR.SetWindowText(str);
	//교축방향 단명력
	str = _T("");
	if (data.nDirShear == seis_jp::kY_Dir)
		str = COMBO_SHEAR_AXIS_DIR_Y;
	else if (data.nDirShear == seis_jp::kZ_Dir)
		str = COMBO_SHEAR_AXIS_DIR_Z;
	else if (data.nDirShear == seis_jp::kYZ_Dir)
		str = COMBO_SHEAR_AXIS_DIR_YZ;
	else { ASSERT(FALSE); }

	if (str.IsEmpty() == false) {
		int iIndex = m_shearAxisDir.FindStringExact(-1, str);
		if (iIndex >= 0) m_shearAxisDir.SetCurSel(iIndex);
		else			 m_shearAxisDir.SetCurSel(0);
	}
}