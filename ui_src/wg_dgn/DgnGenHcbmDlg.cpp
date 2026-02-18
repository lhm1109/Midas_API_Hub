// DgnGenHcbmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenHcbmDlg.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_GEN_HCBM

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenHcbmDlg dialog


CDgnGenHcbmDlg::CDgnGenHcbmDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenHcbmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenHcbmDlg)
	m_nPosition = 0;
	m_strName = _T("");
	//}}AFX_DATA_INIT

	m_pDoc = NULL;
}


void CDgnGenHcbmDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenHcbmDlg)
	DDX_Control(pDX, IDC_DGN_HCBM_L2_UNT,    m_untL2);
	DDX_Control(pDX, IDC_DGN_HCBM_L1_UNT,    m_untL1);
	DDX_Control(pDX, IDC_DGN_HCBM_LST,       m_lstHcbm);
	DDX_Control(pDX, IDC_DGN_HCBM_L2_EDT,    m_edtL2);
	DDX_Control(pDX, IDC_DGN_HCBM_L1_EDT,    m_edtL1);
	DDX_Control(pDX, IDC_DGN_HCBM_PARTC_EDT, m_edtPartC);
	DDX_Control(pDX, IDC_DGN_HCBM_PARTB_EDT, m_edtPartB);
	DDX_Control(pDX, IDC_DGN_HCBM_PARTA_EDT, m_edtPartA);
	DDX_Radio  (pDX, IDC_DGN_HCBM_PART_RDO,  m_nPosition);
	DDX_Text   (pDX, IDC_DGN_HCBM_NAME_EDT,  m_strName);
	DDX_Control(pDX, IDC_DGN_HCBM_FRAME,     m_wndHcbm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenHcbmDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenHcbmDlg)
	ON_BN_CLICKED(IDC_DGN_HCBM_PART_RDO,  OnDgnHcbmPosition)
	ON_BN_CLICKED(IDC_DGN_HCBM_ADD_BTN,   OnDgnHcbmAddBtn)
	ON_BN_CLICKED(IDC_DGN_HCBM_MOD_BTN,   OnDgnHcbmModBtn)
	ON_BN_CLICKED(IDC_DGN_HCBM_DEL_BTN,   OnDgnHcbmDelBtn)
	ON_BN_CLICKED(IDC_DGN_HCBM_CLOSE_BTN, OnDgnHcbmCloseBtn)
	ON_BN_CLICKED(IDC_DGN_HCBM_USER_RDO,  OnDgnHcbmPosition)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_HCBM_LST, OnItemchangedDgnHcbmLst)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_CLICKME,								 OnClickMeNotifyWindow)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,         OnElemListEnter)
END_MESSAGE_MAP()

void CDgnGenHcbmDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		{
			UpdateBuffer();
			// Coded by Seungjun ('20070516) MNet:No.XXXX.
			Update_InitDataByCode();
			break;
		}       
	case D_UPDATE_UNIT:
	  break;
	default: break;
	}
}

void CDgnGenHcbmDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount==0) return; 
	ASSERT(nCount==1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_HCBM_K Key,  KeyBack;
	T_HCBM_D Data, DataBack;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_HCBM_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetHcbm(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_HCBM_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetHcbm(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_HCBM_MFD):
				// 여기서는 저장만 하고 MFS에서 처리한다.
				pViewBuff->GetHcbm(nKey, Key, Data);
				KeyBack  = Key;
				DataBack = Data;
				bMFD = TRUE;
				break;
		case(UR_HCBM_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetHcbm(nKey, Key, Data);
				ModifyItem(Key, KeyBack, DataBack);
				bMFD = FALSE;
				break;
		default:
			break;
		}
	}
}

void CDgnGenHcbmDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_DGN_LIST_NAME), _LS(IDS_DGN_CHK_INDEX) };
	int nColWidth[] = {120, 75};

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstHcbm.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_lstHcbm.GetSafeHwnd(), dwStyle);

	for(int i=0; i<2; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		if(i==0) lvcolumn.fmt = LVCFMT_LEFT;
		else     lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_lstHcbm.InsertColumn(i+1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

void CDgnGenHcbmDlg::MakeItemEx()
{
	m_lstHcbm.DeleteAllItems();

	CArray<T_HCBM_K, T_HCBM_K> aHcbmK;
	m_pDoc->m_pAttrCtrl->GetHcbmKeyList(aHcbmK);
	int nSize = aHcbmK.GetSize();
	if(nSize<1) return;

	for(int i=0; i<nSize; i++)
	{
		T_HCBM_D HcbmD;
		if(!m_pDoc->m_pAttrCtrl->GetHcbm(aHcbmK[i], HcbmD)) ASSERT(0);
		InsertItem(aHcbmK[i], HcbmD);
	}
}

BOOL CDgnGenHcbmDlg::InsertItem(T_HCBM_K key, T_HCBM_D &data)
{
	int nItem = m_lstHcbm.GetItemCount();
	for(int i=0; i<2; i++)
	{
		LVITEM lvitem;
		lvitem.iItem    = nItem;
		lvitem.iSubItem = i;
		CString str     = DataToStr(i, key, data);
		lvitem.pszText  = str.GetBuffer(0);
		lvitem.mask     = LVIF_TEXT;

		if(i==0)
		{
			lvitem.mask   |= LVIF_PARAM;
			lvitem.lParam  = (LPARAM)key;
			nItem          = m_lstHcbm.InsertItem(&lvitem);
		}
		else m_lstHcbm.SetItem(&lvitem);

		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CDgnGenHcbmDlg::DeleteItem(T_HCBM_K key, T_HCBM_D &data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_lstHcbm.FindItem(&FindInfo);
	if(nItem==-1) return TRUE;

	m_lstHcbm.DeleteItem(nItem);

	return TRUE;
}

BOOL CDgnGenHcbmDlg::ModifyItem(T_HCBM_K KeyOld, T_HCBM_K key, T_HCBM_D &data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_lstHcbm.FindItem(&FindInfo);
	if(nItem == -1) return TRUE;

	for(int i=0; i<2; i++)
	{
		CString str = DataToStr(i, key, data);
		m_lstHcbm.SetItemText(nItem, i, str);
	}

	LVITEM lvitem;
	lvitem.iItem    = nItem;
	lvitem.iSubItem = 0;
	lvitem.mask     = LVIF_PARAM;
	lvitem.lParam   = (LPARAM)key;
	m_lstHcbm.SetItem(&lvitem);

	return TRUE;
}

CString CDgnGenHcbmDlg::DataToStr(int i, T_HCBM_K key, T_HCBM_D &data)
{  
	CString str = _T("");

	switch(i)
	{
	case 0:
		str = data.HaunchName;
		break;
	case 1:
		str.Format(_T("%d"), data.aElemListPart[0][0]); // partA의 첫번째 요소
		break;
	default: ASSERT(0); break;
	}
	
	return str;
}


BOOL CDgnGenHcbmDlg::Dlg2Data(BOOL bModify)
{
	UpdateData(TRUE);

	T_HCBM_K HcbmK = 0;
	if(bModify)
	{
		int nItem = m_lstHcbm.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
		HcbmK = (T_HCBM_K)m_lstHcbm.GetItemData(nItem);
	}

	T_HCBM_D HcbmD; HcbmD.Initialize();

	HcbmD.HaunchName = m_strName;
	CString str;
	// check PartA
	m_edtPartA.GetWindowText(str);
	if(!GetNodeList(str, HcbmD.aElemListPart[0])) return FALSE;
	if(HcbmD.aElemListPart[0].GetSize()<1) return FALSE;

	// check PartB
	m_edtPartB.GetWindowText(str);
	if(!GetNodeList(str, HcbmD.aElemListPart[1])) return FALSE;
	if(HcbmD.aElemListPart[1].GetSize()<1) return FALSE;

	// check PartC
	m_edtPartC.GetWindowText(str);
	if(!GetNodeList(str, HcbmD.aElemListPart[2])) return FALSE;
	if(HcbmD.aElemListPart[2].GetSize()<1) return FALSE;

	// Define Position
	HcbmD.nDesignPosition = m_nPosition;
	if(m_nPosition==1)
	{
		HcbmD.dL1 = m_edtL1.GetEditValue();
		HcbmD.dL2 = m_edtL2.GetEditValue();
	}

	BOOL bSuccess = FALSE;
	if(bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyHcbm(HcbmK, HcbmD);
	else        bSuccess = m_pDoc->m_pDataCtrl->AddHcbm(HcbmD);
 
	if(bSuccess) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);

	return bSuccess;
}

/////////////////////////////////////////////////////////////////////////////
// CDgnGenHcbmDlg message handlers

BOOL CDgnGenHcbmDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();

	CMenuBarChildDlg::OnInitDialog();
	
	m_edtL1.SetUnitType(CUnitCtrl::m_HCBM_UNIT.dL1);
	m_untL1.SetUnitType(CUnitCtrl::m_HCBM_UNIT.dL1);
	m_edtL2.SetUnitType(CUnitCtrl::m_HCBM_UNIT.dL2);
	m_untL2.SetUnitType(CUnitCtrl::m_HCBM_UNIT.dL2);
	
	m_edtPartA.SetAttElemList();
	m_edtPartA.SetLButtonDownNotifyWindow(this);
	m_edtPartA.SetEnterNotifyWindow(this);
	m_edtPartA.SetClickMeNotifyWindow(this);
	m_edtPartA.SetModeToUse(MOUSEEDIT_USE_GET_GPSELEM_LIST);

	m_edtPartB.SetAttElemList();
	m_edtPartB.SetLButtonDownNotifyWindow(this);
	m_edtPartB.SetEnterNotifyWindow(this);
	m_edtPartB.SetClickMeNotifyWindow(this);
	m_edtPartB.SetModeToUse(MOUSEEDIT_USE_GET_GPSELEM_LIST);

	m_edtPartC.SetAttElemList();
	m_edtPartC.SetLButtonDownNotifyWindow(this);
	m_edtPartC.SetEnterNotifyWindow(this);
	m_edtPartC.SetClickMeNotifyWindow(this);
	m_edtPartC.SetModeToUse(MOUSEEDIT_USE_GET_GPSELEM_LIST);
	
	SetHeaderTitle();
	MakeItemEx();

	GetDlgItem(IDC_DGN_HCBM_L1_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_HCBM_L2_EDT)->EnableWindow(FALSE);

	// Coded by Seungjun ('20070516) MNet:No.XXXX.
	Update_InitDataByCode();

	m_wndHcbm.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_hcbm.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenHcbmDlg::OnDgnHcbmPosition() 
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_HCBM_L1_EDT)->EnableWindow(m_nPosition==1);
	GetDlgItem(IDC_DGN_HCBM_L2_EDT)->EnableWindow(m_nPosition==1);
}

void CDgnGenHcbmDlg::OnDgnHcbmAddBtn() 
{
	Dlg2Data();
}

void CDgnGenHcbmDlg::OnDgnHcbmModBtn() 
{
	int nItem = m_lstHcbm.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM));
		return;
	}

	Dlg2Data(TRUE);

	m_lstHcbm.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CDgnGenHcbmDlg::OnDgnHcbmDelBtn() 
{
	int nItem = m_lstHcbm.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_SEL_ITEM));
		return;
	}
 
	T_HCBM_K HcbmK = (T_HCBM_K)m_lstHcbm.GetItemData(nItem);
	if(!m_pDoc->m_pDataCtrl->DelHcbm(HcbmK)) return;

	int nCount = m_lstHcbm.GetItemCount();
	if(nCount <= nItem) nItem -= 1;
	if(nItem >= 0) 
		m_lstHcbm.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CDgnGenHcbmDlg::OnDgnHcbmCloseBtn() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnGenHcbmDlg::OnItemchangedDgnHcbmLst(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	T_HCBM_D HcbmD;
	int nItem = m_lstHcbm.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nItem==-1)
	{
		*pResult = 0;
		return;
	}
	else
	{
		if(pNMListView->uNewState==0 || pNMListView->uNewState==1)
		{
			*pResult = 0;
			return;
		}
		
		nItem = pNMListView->iItem;
		T_HCBM_K HcbmK = m_lstHcbm.GetItemData(nItem);
		m_pDoc->m_pAttrCtrl->GetHcbm(HcbmK, HcbmD);
	}

	m_strName = HcbmD.HaunchName;

	CNumericOptimizer opt;
	CString str(_T(""));
	str = opt.OptimizeUnsortedArray(HcbmD.aElemListPart[0]);
	m_edtPartA.SetWindowText(str);
	
	str = opt.OptimizeUnsortedArray(HcbmD.aElemListPart[1]);
	m_edtPartB.SetWindowText(str);

	str = opt.OptimizeUnsortedArray(HcbmD.aElemListPart[2]);
	m_edtPartC.SetWindowText(str);
	
	m_nPosition = HcbmD.nDesignPosition;
	m_edtL1.SetEditUnit(HcbmD.dL1);
	m_edtL2.SetEditUnit(HcbmD.dL2);

	UpdateData(FALSE);
	OnDgnHcbmPosition();
	
	*pResult = 0;
}

LRESULT CDgnGenHcbmDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	CMouseEdit *pMEdit = GetMouseEditCtrl(lParam);
	if(pMEdit==NULL) return 0L;

	if(pMEdit->IsKeyPressed()) return 0L;
	int otype, ekey;
	if(pMEdit->GetCurPickedGPSObjectDataST(otype, ekey))
	{
		if(!IsHaunchElem(pMEdit, (T_ELEM_K)ekey)) return 0L;
		m_pDoc->m_pViewCtrl->SelectElem(NULL, ekey, FALSE);
	}

	return 0L;
}

BOOL CDgnGenHcbmDlg::IsHaunchElem(CMouseEdit* pMEdit, T_ELEM_K ElemK)
{
	ASSERT(pMEdit);
	if(!::IsWindow(pMEdit->m_hWnd)) { ASSERT(0); return FALSE; }

	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	CString str;
	pMEdit->GetWindowText(str);
	if(!GetNodeList(str, aElemK)) return FALSE;

	// 중복 검사
	BOOL bFind = FALSE;
	for(int i=0; i<aElemK.GetSize()-1; i++)
	{
		if(aElemK[i]==ElemK) { bFind = TRUE; break; }
	}

	// Beam
	T_ELEM_D ElemD;
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
	if(!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp)) bFind = TRUE;

	if(bFind)
	{
		aElemK.RemoveAt(aElemK.GetSize()-1);
	}

	CNumericOptimizer opt;
	str = opt.OptimizeUnsortedArray(aElemK);
	pMEdit->SetWindowText(str);

	pMEdit->SetFocus();

	return !bFind;
}

LRESULT CDgnGenHcbmDlg::OnClickMeNotifyWindow(WPARAM wParam, LPARAM lParam)
{
	CMouseEdit *pMEdit = GetMouseEditCtrl(lParam);
	if(pMEdit==NULL) return 0L;

	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
	
	return 0L;
}

LRESULT CDgnGenHcbmDlg::OnElemListEnter(WPARAM wParam, LPARAM lParam)
{
	CMouseEdit *pMEdit = GetMouseEditCtrl(lParam);
	if(pMEdit==NULL) return 0L;

	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);

	CArray<T_ELEM_K, T_ELEM_K> aTmpElemK, aElemK;
	CString str;
	pMEdit->GetWindowText(str);
	if(!GetNodeList(str, aTmpElemK)) return FALSE;
	// 중복성 검사
	for(int i=0; i<aTmpElemK.GetSize(); i++) for(int j=i+1; j<aTmpElemK.GetSize(); j++)
	{
		if(aTmpElemK[i]==aTmpElemK[j]) aTmpElemK.RemoveAt(j);
	}
	// Beam
	for(int i=0; i<aTmpElemK.GetSize(); i++)
	{
		T_ELEM_D ElemD;
		if(!m_pDoc->m_pAttrCtrl->GetElem(aTmpElemK[i], ElemD)) continue;
		if(!m_pDoc->m_pAttrCtrl->IsBeam(ElemD.eltyp)) continue;

		aElemK.Add(aTmpElemK[i]);
		m_pDoc->m_pViewCtrl->SelectElem(NULL, aTmpElemK[i], FALSE);
	}
	aElemK.FreeExtra();

	CNumericOptimizer opt;
	str = opt.OptimizeUnsortedArray(aElemK);
	pMEdit->SetWindowText(str);

	return 0L;
}

CMouseEdit* CDgnGenHcbmDlg::GetMouseEditCtrl(LPARAM lParam)
{
	CMouseEdit* pMEdit = NULL;

	if(m_edtPartA.GetSafeHwnd() == (HWND)lParam)
	{
		pMEdit = (CMouseEdit*)(CEdit*)GetDlgItem(IDC_DGN_HCBM_PARTA_EDT);
	}
	else if(m_edtPartB.GetSafeHwnd() == (HWND)lParam)
	{
		pMEdit = (CMouseEdit*)(CEdit*)GetDlgItem(IDC_DGN_HCBM_PARTB_EDT);
	}
	else if(m_edtPartC.GetSafeHwnd() == (HWND)lParam)
	{
		pMEdit = (CMouseEdit*)(CEdit*)GetDlgItem(IDC_DGN_HCBM_PARTC_EDT);
	}
	return pMEdit;
}

// Coded by Seungjun ('20070516) MNet:No.XXXX.
void CDgnGenHcbmDlg::Update_InitDataByCode()
{
	CString strTitle=_T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleGen(ID_DGN_GEN_HCBM, strTitle, bShow);

	GetDlgItem(IDC_DGN_HCBM_NAME_EDT)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_FRAME)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_PARTA_EDT)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_PARTB_EDT)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_PARTC_EDT)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_PART_RDO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_USER_RDO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_L1_EDT)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_L2_EDT)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_LST)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_ADD_BTN)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_MOD_BTN)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_HCBM_DEL_BTN)->EnableWindow(bShow);
}