// DgnGenMembPlateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenMembPlateDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenMembPlateDlg dialog
CDgnGenMembPlateDlg::CDgnGenMembPlateDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenMembPlateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenMembPlateDlg)
	m_nOption = 0;
	m_nPlateMemb = 0;
	m_nLocalUCS = 0;
	m_nElemAvg = 0;
	m_nXYDir = 0;

	//}}AFX_DATA_INIT
	m_Data.Initialize();
}

CDgnGenMembPlateDlg::~CDgnGenMembPlateDlg()
{
}

void CDgnGenMembPlateDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenMembPlateDlg)
	DDX_Radio(pDX, IDC_DGN_MEMB_PLATE_AR_RDO, m_nOption);
	DDX_Control(pDX, IDC_DGN_MEMB_PLATE_NAME_EDT, m_edtName);
	DDX_Radio(pDX, IDC_DGN_MEMB_PLATE_DGN_TYPE_BEAM_RDO, m_nPlateMemb);
	DDX_Radio(pDX, IDC_DGN_MEMB_PLATE_LOCAL_RDO, m_nLocalUCS);
	DDX_Control(pDX, IDC_DGN_MEMB_PLATE_UCS_CMB, m_cmbUCS);
	DDX_Radio(pDX, IDC_DGN_MEMB_PLATE_ELEM_RDO, m_nElemAvg);
	DDX_Radio(pDX, IDC_DGN_MEMB_PLATE_X_RDO, m_nXYDir);
	DDX_Control(pDX, IDC_DGN_MEMB_PLATE_AVG_NODAL_CHK, m_chkAvgNodal);


	DDX_Control(pDX, IDC_DGN_MEMB_PLATE_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnGenMembPlateDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenMembPlateDlg)
	ON_BN_CLICKED(IDC_DGN_MEMB_PLATE_AR_RDO, OnDgnOptionRdo)
	ON_BN_CLICKED(IDC_DGN_MEMB_PLATE_DEL_RDO, OnDgnOptionRdo)
	ON_BN_CLICKED(IDC_DGN_MEMB_PLATE_LOCAL_RDO, OnDgnLocalUCSRdo)
	ON_BN_CLICKED(IDC_DGN_MEMB_PLATE_UCS_RDO, OnDgnLocalUCSRdo)

	ON_BN_CLICKED(IDC_DGN_MEMB_PLATE_ELEM_RDO, OnDgnRadioElemAvg)
	ON_BN_CLICKED(IDC_DGN_MEMB_PLATE_AVG_RDO, OnDgnRadioElemAvg)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_MEMB_PLATE_LIST, OnItemchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenMembPlateDlg message handlers
BOOL CDgnGenMembPlateDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();

	InitUCSCombo();
	SetListCtrlHeader();
	MakeItemEx();

	Data2Dlg();
	OnDgnLocalUCSRdo();
	OnDgnRadioElemAvg();

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenMembPlateDlg::OnDgnOptionRdo()
{
	ControlsEnableDisable();
}

void CDgnGenMembPlateDlg::OnDgnLocalUCSRdo()
{
	ControlsEnableDisable();
}

void CDgnGenMembPlateDlg::OnDgnRadioElemAvg()
{
	ControlsEnableDisable();
}

void CDgnGenMembPlateDlg::Execute()
{
	UpdateData(TRUE);

	Dlg2Data();

	if (m_Data.strName.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_DGN_GEN_MEMB_PLATE_NO_NAME_ERROR));
		return;
	}

	if (m_nOption == 0) // Add/Replace
	{
		if (m_pDoc->m_pAttrCtrl->ExistRcpl(m_Data.strName))
		{
			m_pDoc->m_pDataCtrl->ModifyRcpl(m_Data.strName, m_Data);
		}
		else
		{
			m_pDoc->m_pDataCtrl->AddRcpl(m_Data);
		}
	}
	else // Delete
	{
		m_pDoc->m_pDataCtrl->DelRcpl(m_Data.strName);
	}

	MakeItemEx();
	InitSelectedItem();
}

void CDgnGenMembPlateDlg::InitUCSCombo()
{
	m_cmbUCS.ResetContent();

	CArray<CString, CString&> arNUCSNames;
	m_pDoc->m_pAttrCtrl->GetNucsList(arNUCSNames);

	int nNUCSNamesSize = arNUCSNames.GetSize();

	for (int i = 0; i < nNUCSNamesSize; i++)
	{
		m_cmbUCS.AddString(arNUCSNames[i]);
	}
	m_cmbUCS.SetCurSel(0);
}

void CDgnGenMembPlateDlg::InitSelectedItem() // 선택된 요소와 절점을 해제한다.
{
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedNodeCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CDgnGenMembPlateDlg::ControlsEnableDisable()
{
	UpdateData(TRUE);

	CString strCode = CDBLib::GetConCodeName();
	BOOL bEnable_AddDel = FALSE;
	BOOL bEnable_DgnCode = FALSE;

	if (strCode == _T("KSCE-USD10") || strCode == _T("KSCE-RAIL-USD11") ||
		strCode == CONCODE_KDS_24_14_21_2022 || strCode == _T("KSCE-LSD15") || 
		strCode == _T("KCI-USD12") || strCode == CONCODE_KDS_14_20_00_2022)
	{
		bEnable_DgnCode = TRUE;

		if (m_nOption == 0) // Add/Replace
		{
			bEnable_AddDel = TRUE;
		}
		else
		{
			bEnable_AddDel = FALSE;
		}
	}

	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_MEMB_PLATE_MEMB_GRP, bEnable_AddDel, TRUE);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_MEMB_PLATE_OPT_GRP, bEnable_DgnCode, TRUE);
	GetDlgItem(IDC_DGN_MEMB_PLATE_LIST)->EnableWindow(bEnable_DgnCode);
	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bEnable_DgnCode);

	GetDlgItem(IDC_DGN_MEMB_PLATE_UCS_RDO)->EnableWindow(bEnable_AddDel && /*m_nLocalUCS == 1 && */m_cmbUCS.GetCount() != 0);
	GetDlgItem(IDC_DGN_MEMB_PLATE_UCS_CMB)->EnableWindow(bEnable_AddDel && m_nLocalUCS == 1 && m_cmbUCS.GetCount() != 0);


	GetDlgItem(IDC_DGN_MEMB_PLATE_AVG_NODAL_CHK)->EnableWindow(m_nElemAvg == 1);

}

void CDgnGenMembPlateDlg::Data2Dlg()
{
	//검토영역 선택
	m_edtName.SetWindowText(m_Data.strName);
	m_nPlateMemb = m_Data.nPlateMemb;
	m_nLocalUCS = m_Data.iAxisType;

	T_NUCS_D USCData;
	USCData.Initialize();
	m_pDoc->m_pAttrCtrl->GetNucs(m_Data.ucsKey, USCData);
	int Index = m_cmbUCS.FindStringExact(-1, USCData.UcsName);
	if (Index != -1)
		m_cmbUCS.SetCurSel(Index);
	else
		m_cmbUCS.SetCurSel(0);

	if (m_cmbUCS.GetCount() == 0) m_nLocalUCS = 0;

	m_nElemAvg = m_Data.iAnaType;
	m_nXYDir = m_Data.iDir;
	m_chkAvgNodal.SetCheck(m_Data.bActiveOnly);

	UpdateData(FALSE);
}

BOOL CDgnGenMembPlateDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.Initialize();

	m_edtName.GetWindowText(m_Data.strName);
	m_Data.nPlateMemb = m_nPlateMemb;

	m_pDoc->m_pViewCtrl->GetAllSelectedElem(m_Data.arElemK);

	m_Data.iAxisType = m_nLocalUCS;

	CString strTemp;
	m_cmbUCS.GetWindowText(strTemp);
	m_Data.ucsKey = m_pDoc->m_pAttrCtrl->GetNucsKey(strTemp);

	m_Data.iAnaType = m_nElemAvg;
	m_Data.iDir = m_nXYDir;
	m_Data.bActiveOnly = m_chkAvgNodal.GetCheck();


	// T_RCPL_D 의 dDt, dDb 는 무조건 50mm 를 넣어준다.
	m_Data.dDt = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 50.0);  //  mm
	m_Data.dDb = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 50.0);  //  mm

	return TRUE;
}

void CDgnGenMembPlateDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnGenMembPlateDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_RCPL_K Key;
	T_RCPL_D Data;

	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_RCPL_ADD):
		{
			pViewBuff->GetRcpl(nKey, Key, Data);
			InsertItem(Key);
		}
		break;
		case(UR_RCPL_DEL):
		{
			pViewBuff->GetRcpl(nKey, Key, Data);
			DeleteItem(Key);
		}
		break;
		//     case(UR_RCPL_MFD):
		//     case(UR_RCPL_MFS):
		//       {
		//         pViewBuff->GetRcpl(nKey, Key, Data);
		//         DeleteItem(Key);
		//         InsertItem(Key);        
		//       }
		//       break;
		case(UR_DCON_ADD): // Design Code 변경
		case(UR_DCON_DEL):
		{
			ControlsEnableDisable();
			break;
		}
		case(UR_NUCS_ADD):
		case(UR_NUCS_DEL):
			InitUCSCombo();
			break;
		default:
			break;
		}
	}
}

void CDgnGenMembPlateDlg::InsertItem(T_RCPL_K Key)
{
	// 삽입 위치를 찾는다.
	T_RCPL_D Data; Data.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetRcpl(Key, Data)) return;

	int nSize = m_List.GetItemCount();
	CString strMembName;
	strMembName.Format(_T("%s"), Data.strName);
	int nIndex = m_List.InsertItem(nSize, strMembName);
	CString csItem;
	CString strPlateMemb = _T("");
	CString strElemList = _T("");

	if (Data.nPlateMemb == 0) strPlateMemb = _LS(IDS_DGN_BEAM);
	else if (Data.nPlateMemb == 1) strPlateMemb = _LS(IDS_DGN_WALL);
	else ASSERT(0);
	m_List.SetItemText(nIndex, 1, strPlateMemb);

	for (int i = 0; i < Data.arElemK.GetSize() - 1; i++)
	{
		csItem.Format(_T("%d"), Data.arElemK[i]);
		strElemList += csItem + _T(", ");
	}
	if (Data.arElemK.GetSize() > 0)
	{
		csItem.Format(_T("%d"), Data.arElemK[Data.arElemK.GetSize() - 1]);
		strElemList += csItem;
	}
	m_List.SetItemText(nIndex, 2, strElemList);
	m_List.SetItemData(nIndex, (DWORD)Key);
}

void CDgnGenMembPlateDlg::DeleteItem(T_RCPL_K Key)
{
	int nSize = m_List.GetItemCount();
	int nCount = 0;
	for (nCount = 0; nCount < nSize; nCount++)
	{
		if (m_List.GetItemData(nCount) == Key) break;
	}
	if (nCount == nSize) return;
	else m_List.DeleteItem(nCount);
}

void CDgnGenMembPlateDlg::SetListCtrlHeader()
{
	CString aTitle[] = { _LS(IDS_DGN_LIST_NAME), _LS(IDS_DGN_LIST_TYPE), _LS(IDS_DGN_CHK_ELEM_LIST) };  // _T("Name"), _T("Type"), _T("Element List")
	int nColWidth[] = { 50, 50, 90 };
	int nAlign[] = { LVCFMT_CENTER, LVCFMT_LEFT };
	int nColNum = sizeof(nColWidth) / sizeof(int);
	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i, &lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CDgnGenMembPlateDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountRcpl();
	if (nItemCount == 0) return;

	POSITION pos;
	int nIndex;

	T_RCPL_K Key;
	T_RCPL_D Data;
	CString strMembName, strPlateMemb, strElemList;

	int* KeyBuf = new int[nItemCount];
	int nCount = 0;
	pos = m_pDoc->m_pAttrCtrl->GetStartRcpl();
	while (pos != NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextRcpl(pos, Key, Data);
		KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nItemCount, sizeof(T_ELEM_K), CCompFunc::UINTAsc);

	for (nCount = 0; nCount < nItemCount; nCount++)
	{
		Key = KeyBuf[nCount];
		m_pDoc->m_pAttrCtrl->GetRcpl(Key, Data);

		strMembName.Format(_T("%s"), Data.strName);
		nIndex = m_List.InsertItem(nCount, strMembName);

		if (Data.nPlateMemb == 0) strPlateMemb = _LS(IDS_DGN_BEAM);
		else if (Data.nPlateMemb == 1) strPlateMemb = _LS(IDS_DGN_WALL);
		else ASSERT(0);
		m_List.SetItemText(nIndex, 1, strPlateMemb);

		//CNumericOptimizer optimizer;
		//strElemList = optimizer.Optimize((long*)Data.aElemList.GetData(), Data.aElemList.GetSize());
		CString csItem;
		strElemList = _T("");
		for (int i = 0; i < Data.arElemK.GetSize() - 1; i++)
		{
			csItem.Format(_T("%d"), Data.arElemK[i]);
			strElemList += csItem + _T(", ");
		}
		if (Data.arElemK.GetSize() > 0)
		{
			csItem.Format(_T("%d"), Data.arElemK[Data.arElemK.GetSize() - 1]);
			strElemList += csItem;
		}
		m_List.SetItemText(nIndex, 2, strElemList);
		m_List.SetItemData(nIndex, (DWORD)Key);
	}
	delete[]KeyBuf;
}

void CDgnGenMembPlateDlg::OnDelKeyPressedInList()
{
	CArray<T_RCPL_K, T_RCPL_K> aKey;
	CArray<int, int> aSelItem;
	aSelItem.RemoveAll();
	aKey.RemoveAll();

	GetSelectedItemList(&m_List, aSelItem);
	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;

	for (int i = 0; i < nSize; i++)
	{
		DWORD(Key) = m_List.GetItemData(aSelItem[i]);
		if (m_pDoc->m_pAttrCtrl->ExistRcpl(Key)) aKey.Add(Key);
	}
	if (m_pDoc->m_pDataCtrl->DelRcpl(aKey)) InitSelectedItem();

	SetListFocus(aSelItem);
}

// member를 삭제한 경우 List 박스의 Focus를 다시 맞춤
void CDgnGenMembPlateDlg::SetListFocus(CArray<int, int>& aSelItem)
{
	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	int nFocusItem = aSelItem[0];
	for (int i = 0; i < nSize; i++)
	{
		if (aSelItem[i] < nFocusItem) nFocusItem = aSelItem[i];
	}
	int nCount = m_List.GetItemCount();
	if (nFocusItem >= nCount) nFocusItem -= 1;
	if (nFocusItem >= 0)
		m_List.SetItemState(nFocusItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

BOOL CDgnGenMembPlateDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DELETE && GetFocus() == &m_List)
		{
			OnDelKeyPressedInList();
			return TRUE;
		}
	}
	return CMenuBarChildDlg::PreTranslateMessage(pMsg);
}

void CDgnGenMembPlateDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	DisplaySelElemList();  // 선택된 요소를 Display한다. 
	Data2Dlg();
	ControlsEnableDisable();
	*pResult = 0;
}

void CDgnGenMembPlateDlg::DisplaySelElemList()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelElem;
	aSelElem.RemoveAll();
	GetSelElemList(aSelElem);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelElem, FALSE, TRUE);
}

void CDgnGenMembPlateDlg::GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem)
{
	T_RCPL_D Data;
	CArray<int, int> aSelItem;
	CArray<T_RCPL_K, T_RCPL_K> aKey;
	aKey.RemoveAll();
	aSelItem.RemoveAll();
	aSelElem.RemoveAll();

	GetSelectedItemList(&m_List, aSelItem);

	int nSize = aSelItem.GetSize();
	if (nSize == 0) return;
	for (int i = 0; i < nSize; i++)
	{
		DWORD(Key) = m_List.GetItemData(aSelItem[i]);
		aKey.Add(Key);
	}

	for (int i = 0; i < aKey.GetSize(); ++i)
	{
		m_pDoc->m_pAttrCtrl->GetRcpl(aKey[i], Data);

		for (int j = 0; j < Data.arElemK.GetSize(); ++j)
		{
			aSelElem.Add(Data.arElemK[j]);
		}
	}

	m_Data = Data; // 선택된것 중 마지막거 보여줄께요~~
}

void CDgnGenMembPlateDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for (UINT i = 0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}
