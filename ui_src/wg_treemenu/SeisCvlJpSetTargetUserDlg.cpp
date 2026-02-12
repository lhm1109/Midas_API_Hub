// CSeisCvlJpSetTargetUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetTargetUserDlg.h"
#include "SeisCvlJpSetTargetDlg.h"

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

#define COMBO_SHEAR_AXIS_DIR_Y				_LS(IDS_IDD_TM_SIGR_USER_IDC_COMBO_SHEAR_AXIS_DIR_Y)
#define COMBO_SHEAR_AXIS_DIR_Z				_LS(IDS_IDD_TM_SIGR_USER_IDC_COMBO_SHEAR_AXIS_DIR_Z)
#define COMBO_SHEAR_AXIS_DIR_YZ				_LS(IDS_IDD_TM_SIGR_USER_IDC_COMBO_SHEAR_AXIS_DIR_YZ)
#define LIST_SIGR_USER_NAME					_LS(IDS_IDD_TM_SIGR_USER_IDC_LIST_SIGR_USER_NAME)
#define LIST_SIGR_USER_ELEMENT				_LS(IDS_IDD_TM_SIGR_USER_IDC_LIST_SIGR_USER_ELEMENT)
#define LIST_SIGR_USER_AXIS_DIR				_LS(IDS_IDD_TM_SIGR_USER_IDC_LIST_SIGR_USER_AXIS_DIR)

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetUserDlg dialog

CSeisCvlJpSetTargetUserDlg::CSeisCvlJpSetTargetUserDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlJpSetTargetUserDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nOption = 0;
	m_pParent = (CSeisCvlJpSetTargetDlg*)pParent;
}

CSeisCvlJpSetTargetUserDlg::~CSeisCvlJpSetTargetUserDlg()
{
	m_pDoc = NULL;
}

void CSeisCvlJpSetTargetUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Radio(pDX, IDC_TM_SIGR_USER_ADD_RDO, m_nOption);
	DDX_Control(pDX, IDC_TM_SIGR_USER_NAME_EDT, m_name);
	DDX_Control(pDX, IDC_TM_SIGR_USER_INCLUD_ELEM_EDT, m_elements);
	DDX_Control(pDX, IDC_TM_SIGR_USER_DIR_CMB, m_shearAxisDir);
	DDX_Control(pDX, IDC_TM_SIGR_USER_LST, m_sigrUserList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisCvlJpSetTargetUserDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlJpSetTargetUserDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SIGR_USER_LST, OnNMClickSigrUserList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSeisCvlJpSetTargetUserDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	
	InitControl();
	InitSigrUserListCtrl();

	return TRUE;// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlJpSetTargetUserDlg::OnNMClickSigrUserList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1)	{
		*pResult = 0;
		return;
	}

	int nItem = m_sigrUserList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) {
		ResetControls();
		*pResult = 0;
		return;
	}

	nItem = pNMListView->iItem;
	T_SIGR_USER_K key = (T_SIGR_USER_K)m_sigrUserList.GetItemData(nItem);

	ResetControls();
	UpdataDialog(key);
}

void CSeisCvlJpSetTargetUserDlg::Execute()
{
	UpdateData(TRUE);

	BOOL bModify = FALSE;
	int nItem = m_sigrUserList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem != -1) bModify = TRUE;

	T_SIGR_USER_K sigrUserK;
	if (nItem != -1)
		sigrUserK = (T_SIGR_USER_K)m_sigrUserList.GetItemData(nItem);

	BOOL bResult = FALSE;
	if (m_nOption == 0) {
		if (bModify) {
			bResult = ModData(sigrUserK);
		}
		else {//Add
			bResult = AddData();
		}
	}
	else {//Delete
		bResult = DelData(sigrUserK);
	}

	if (bResult)
		ResetControls();
}

void CSeisCvlJpSetTargetUserDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CSeisCvlJpSetTargetUserDlg::InitControl()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_TM_SIGR_USER_ADD_RDO);
	pBtn->SetCheck(1);
	m_nOption = 0;

	m_elements.Connect(SC_ID_ELEM, &m_elements);

	m_shearAxisDir.ResetContent();
	m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Y);
	m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_Z);
	m_shearAxisDir.AddString(COMBO_SHEAR_AXIS_DIR_YZ);
	m_shearAxisDir.SetCurSel(0);

	ResetControls();
}

void CSeisCvlJpSetTargetUserDlg::InitSigrUserListCtrl()
{
	SetHeaderTitleListCtrl();

	CArray<T_SIGR_USER_K, T_SIGR_USER_K> keys;
	m_pDoc->m_pAttrCtrl2->GetQSigrUser()->GetKeyList(keys);
	for (int i = 0; i < keys.GetCount(); i++)
	{
		T_SIGR_USER_K& key = keys[i];
		T_SIGR_USER_D data;

		m_pDoc->m_pAttrCtrl2->GetQSigrUser()->Get(key, data);
		InsertItemListCtrl(key, data);
	}
}

void CSeisCvlJpSetTargetUserDlg::SetHeaderTitleListCtrl()
{
	CString aTitle[] = { LIST_SIGR_USER_NAME, LIST_SIGR_USER_ELEMENT, LIST_SIGR_USER_AXIS_DIR };
	int nColWidth[] = { 70, 70, 60 };

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_sigrUserList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_sigrUserList.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < 3; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_sigrUserList.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

BOOL CSeisCvlJpSetTargetUserDlg::InsertItemListCtrl(T_SIGR_USER_K Key, T_SIGR_USER_D& Data)
{
	int nItem = m_sigrUserList.GetItemCount();
	for (int i = 0; i < 3; i++)
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
			nItem = m_sigrUserList.InsertItem(&lvitem);
		}
		else m_sigrUserList.SetItem(&lvitem);

		str.ReleaseBuffer();
	}

	return TRUE;
}

CString CSeisCvlJpSetTargetUserDlg::DataToStr(const int& index, const T_SIGR_USER_D& data)
{
	CString str = _T("");
	BOOL bResult = FALSE;

	switch (index)
	{
	case 0://명칭
	{
		T_SIGR_D sigrD;
		bResult = m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(data.kSigr, sigrD);
		ASSERT(bResult);
		if(bResult)
			str = sigrD.strName;
		break;
	}
	case 1://요소
		GetStrKeyList(data.aInvestigatedElem, str);
		break;
	case 2://교축방향
		if (data.nDirShear == seis_jp::kY_Dir)
			str = COMBO_SHEAR_AXIS_DIR_Y;
		else if (data.nDirShear == seis_jp::kZ_Dir)
			str = COMBO_SHEAR_AXIS_DIR_Z;
		else if (data.nDirShear == seis_jp::kYZ_Dir)
			str = COMBO_SHEAR_AXIS_DIR_YZ;
		else { ASSERT(0); }
		break;
	break;
	default: ASSERT(0); break;
	}

	return str;
}

BOOL CSeisCvlJpSetTargetUserDlg::AddData()
{
	T_SIGR_D sigrD;
	sigrD.nSubType = T_SIGR_D::kUser;
	T_SIGR_USER_D userD;
	DlgToData(sigrD, userD);

	if (CheckData(sigrD, userD) == FALSE)
		return FALSE;

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_SIGR_USER)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	T_SIGR_K sigrKey = m_pDoc->m_pAttrCtrl2->GetQSigr()->GetStartNum();
	if (m_pDoc->m_pEditData->AddSigr(sigrKey, sigrD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}

	userD.kSigr = sigrKey;
	if (m_pDoc->m_pEditData->AddSigrUser(sigrKey, userD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIGR_USER_CMD);
	return TRUE;
}

BOOL CSeisCvlJpSetTargetUserDlg::ModData(const T_SIGR_USER_K& key)
{
	if (m_pDoc->m_pAttrCtrl2->GetQSigrUser()->Exist(key) == FALSE)
		return FALSE;

	T_SIGR_USER_D oldD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrUser()->Get(key, oldD) == FALSE)
		return FALSE;

	T_SIGR_D oldSigrD;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(oldD.kSigr, oldSigrD) == FALSE)
		return FALSE;

	T_SIGR_D sigrD;
	sigrD.nSubType = T_SIGR_D::kUser;
	T_SIGR_USER_D newD;
	newD.kSigr = oldD.kSigr;
	DlgToData(sigrD, newD);

	BOOL bCheckName = (oldSigrD.strName != sigrD.strName);
	if (CheckData(sigrD, newD, bCheckName) == FALSE)
		return FALSE;

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Modify_SIGR_USER)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	if (m_pDoc->m_pEditData->ModifySigr(oldD.kSigr, sigrD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	if (m_pDoc->m_pEditData->ModifySigrUser(key, newD) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIGR_USER_CMD);
	return TRUE;
}

BOOL CSeisCvlJpSetTargetUserDlg::DelData(const T_SIGR_USER_K& key)
{
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrUser()->Exist(key))
		return FALSE;

	T_SIGR_USER_D userD;
	if(m_pDoc->m_pAttrCtrl2->GetQSigrUser()->Get(key, userD) == FALSE)
		return FALSE;

	if (!m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Delete_SIGR_USER)), CMDTYPE_REMOVE_BRDG_SEIS_INVE_JP)) return FALSE;

	ASSERT(userD.kSigr == key);
// 	if (m_pDoc->m_pEditData->DelSigr(userD.kSigr) == FALSE) {
// 		ASSERT(FALSE);
// 		m_pDoc->m_pUndoCtrl->CancelEditDB();
// 		return FALSE;
// 	}
	if (m_pDoc->m_pEditData->DelSigrUser(key) == FALSE) {
		ASSERT(FALSE);
		m_pDoc->m_pUndoCtrl->CancelEditDB();
		return FALSE;
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, FALSE, LT_SIGR_USER_CMD);
	return TRUE;
}

void CSeisCvlJpSetTargetUserDlg::DlgToData(OUT T_SIGR_D& rSigrD, OUT T_SIGR_USER_D& rUserD)
{
	CString str;
	//명칭
	m_name.GetWindowText(rSigrD.strName);
	//대상요소
	m_elements.GetWindowText(str);
	GetNodeList(str, rUserD.aInvestigatedElem);
	//교축방향 전단력
	m_shearAxisDir.GetWindowText(str);
	rUserD.nDirShear = seis_jp::kDirShear_0;
	if (str == COMBO_SHEAR_AXIS_DIR_Y)
		rUserD.nDirShear = seis_jp::kY_Dir;
	else if (str == COMBO_SHEAR_AXIS_DIR_Z)
		rUserD.nDirShear = seis_jp::kZ_Dir;
	else if (str == COMBO_SHEAR_AXIS_DIR_YZ)
		rUserD.nDirShear = seis_jp::kYZ_Dir;
}

BOOL CSeisCvlJpSetTargetUserDlg::CheckData(IN T_SIGR_D& sigrD, const T_SIGR_USER_D& userD, BOOL bCheckName/*=TRUE*/)
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
	//대상요소 입력여부 체크
	if (userD.aInvestigatedElem.GetSize() < 1) {
		msg = _LS(IDS_IDD_TM_SIGR_MSG_EMPTY_ELEMENT);
		AfxMessageBox(msg);
		return FALSE;
	}

	//내진조사 모든 그룹에 요소가 등록되어 있지 않았는지 확인
	if (m_pParent->ExistElementAtSigrUser(userD.kSigr, userD.aInvestigatedElem) == TRUE)  return FALSE;
	if (m_pParent->ExistElementAtSigrUpper(userD.kSigr, userD.aInvestigatedElem) == TRUE)  return FALSE;
	if (m_pParent->ExistElementAtSigrLower(userD.kSigr, userD.aInvestigatedElem) == TRUE)  return FALSE;
	
	return TRUE;
}

void CSeisCvlJpSetTargetUserDlg::ResetControls()
{
	m_name.SetWindowText(_T(""));
	m_elements.SetWindowText(_T(""));
	m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CSeisCvlJpSetTargetUserDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SIGR_USER_K Key, KeyBack;
	T_SIGR_USER_D Data, DataBack;

	BOOL bUpdated = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_SIGR_USER_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetSigrUser(nKey, Key, Data);
			bUpdated = InsertItem(Key, Data);
			break;
		case(UR_SIGR_USER_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetSigrUser(nKey, Key, Data);
			bUpdated = DeleteItem(Key, Data);
			break;
		case(UR_SIGR_USER_MFD):
			// 여기서는 저장만 하고 MFS에서 처리한다.
			pViewBuff->GetSigrUser(nKey, Key, Data);
			KeyBack = Key;
			DataBack = Data;
			bUpdated = TRUE;
			break;
		case(UR_SIGR_USER_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bUpdated);
			pViewBuff->GetSigrUser(nKey, Key, Data);
			bUpdated = ModifyItem(KeyBack, DataBack);
			break;
		default:
			break;
		}
	}
}

BOOL CSeisCvlJpSetTargetUserDlg::InsertItem(const T_SIGR_USER_K& Key, const T_SIGR_USER_D& Data)
{
	int nItem = m_sigrUserList.GetItemCount();
	for (int i = 0; i < 3; i++)
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
			nItem = m_sigrUserList.InsertItem(&lvitem);
		}
		else m_sigrUserList.SetItem(&lvitem);

		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CSeisCvlJpSetTargetUserDlg::DeleteItem(const T_SIGR_USER_K& Key, const T_SIGR_USER_D& Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;

	int nItem = m_sigrUserList.FindItem(&FindInfo);
	if (nItem == -1) return TRUE;

	return m_sigrUserList.DeleteItem(nItem);
}

BOOL CSeisCvlJpSetTargetUserDlg::ModifyItem(const T_SIGR_USER_K& Key, const T_SIGR_USER_D& Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;

	int nItem = m_sigrUserList.FindItem(&FindInfo);
	if (nItem == -1) return TRUE;

	for (int i = 0; i < 3; i++)
	{
		CString str = DataToStr(i, Data);
		m_sigrUserList.SetItemText(nItem, i, str);
	}

	LVITEM lvitem;
	lvitem.iItem = nItem;
	lvitem.iSubItem = 0;
	lvitem.mask = LVIF_PARAM;
	lvitem.lParam = (LPARAM)Key;

	return m_sigrUserList.SetItem(&lvitem);
}

void CSeisCvlJpSetTargetUserDlg::UpdataDialog(const T_SIGR_USER_K& key)
{
	T_SIGR_USER_D data;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrUser()->Get(key, data) == FALSE)
		return;
	//명칭
	CString str = m_pDoc->m_pAttrCtrl2->GetQSigrUser()->GetSigrName(key);
	m_name.SetWindowText(str);
	//대상요소
	str = _T("");
	GetStrKeyList(data.aInvestigatedElem, str);
	m_elements.SetWindowText(str);
	//교축방향 단명력
	str = _T("");
	if(data.nDirShear == seis_jp::kY_Dir)
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

	//모델상에서 해당 요소가 선택/하이라이트
	m_pDoc->m_pViewCtrl->SelectElem(NULL, data.aInvestigatedElem, FALSE);
}