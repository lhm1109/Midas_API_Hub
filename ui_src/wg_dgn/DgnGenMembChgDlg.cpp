// DgnGenMembChgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenMembChgDlg.h"

//#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenMembChgDlg dialog


CDgnGenMembChgDlg::CDgnGenMembChgDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnGenMembChgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenMembChgDlg)
	m_nSelectType = 1;
	//}}AFX_DATA_INIT
	m_mapSelMembKey.InitHashTable(HASHSIZEMEMB);
	m_bSentMessage = FALSE;
 	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnGenMembChgDlg::~CDgnGenMembChgDlg()
{
}


void CDgnGenMembChgDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenMembChgDlg)
	DDX_Control(pDX, IDC_DGN_MEMB_CHG_SELECT_ALL, m_BtnSelect);
	DDX_Radio(pDX, IDC_DGN_MEMB_CHG_SELECT_ALL, m_nSelectType); 
	DDX_Control(pDX, IDC_DGN_MEMB_CHG_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnGenMembChgDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnGenMembChgDlg)
	ON_BN_CLICKED(IDC_DGN_MEMB_CHG_SELECT_ALL, OnDgnMemberSelect)
	ON_BN_CLICKED(IDC_DGN_MEMB_CHG_SELECT_SEL, OnDgnMemberSelect)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_MEMB_CHG_LIST, OnItemchanged)
	ON_NOTIFY(NM_SETFOCUS, IDC_DGN_MEMB_CHG_LIST, OnSetfocusList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenMembChgDlg message handlers
BOOL CDgnGenMembChgDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	SetListCtrlHeader();
	MakeItemEx();	

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenMembChgDlg::OnDgnMemberSelect()
{
	DisplaySelElemList();   // 선택된 요소를 Display한다.
}

void CDgnGenMembChgDlg::Execute() 
{
	UpdateData(TRUE);	
	T_ELEM_K KeyElem;
	T_ELEM_D DataElem;
	T_MEMB_K KeyMemb;
	T_MEMB_D DataMemb;
	CArray<T_ELEM_K, T_ELEM_K> aSelElem;   // Select된 Element List
	CArray<T_MEMB_K, T_MEMB_K> aSelMemb;
	m_mapSelMembKey.RemoveAll();
	aSelMemb.RemoveAll();

	if(m_nSelectType == 0)  // Selection Type : All
	{
		m_pDoc->m_pAttrCtrl->GetMembKeyList(aSelMemb);    
		for(int i = 0; i < aSelMemb.GetSize(); i++)
			m_mapSelMembKey.SetAt(aSelMemb[i], aSelMemb[i]);    
	}  
	else if(m_nSelectType == 1) // Selection Type : By Selection
	{
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	  pIGM->GetSelectedElemKeyListForDgn(aSelElem);
		if(aSelElem.GetSize() <= 0) 
		{
			GSaveHistoryFormatNF(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK); return; // Error : No selected element!
		}
		for(int i = 0; i < aSelElem.GetSize(); i++)  // 선택된 element중에서 Member가 지정된 element의 Member key를 모든다.
	  {
		  KeyElem = aSelElem.GetAt(i);		
		  DataElem.Initialize();      
		  if(m_pDoc->m_pAttrCtrl->GetElem(KeyElem, DataElem))
			{
				if(m_pDoc->m_pAttrCtrl->GetMembAssigned(KeyElem, KeyMemb))        
					m_mapSelMembKey.SetAt(KeyMemb, KeyMemb);
			}
	  }    
	}  
	else ASSERT(0);  

	ReverseMemberLocalDirection();
}

void CDgnGenMembChgDlg::ReverseMemberLocalDirection() 
{
	T_MEMB_K KeyMemb;
	T_MEMB_D DataMemb;
	CArray<T_MEMB_K, T_MEMB_K> aKeyMemb;  
	CArray<T_MEMB_D, T_MEMB_D&> aDataMemb;
	aKeyMemb.RemoveAll();
	aDataMemb.RemoveAll();
	
	POSITION pos = m_mapSelMembKey.GetStartPosition();
	while(pos != NULL)
	{
		m_mapSelMembKey.GetNextAssoc(pos, KeyMemb, KeyMemb);
		DataMemb.Initialize();
		m_pDoc->m_pAttrCtrl->GetMemb(KeyMemb, DataMemb);
		if(DataMemb.bReverseDir == TRUE) DataMemb.bReverseDir = FALSE;
		else if(DataMemb.bReverseDir == FALSE) DataMemb.bReverseDir = TRUE;
		aKeyMemb.Add(KeyMemb);
		aDataMemb.Add(DataMemb);    
	}  
	if(m_pDoc->m_pDataCtrl->AddMemb(aKeyMemb, aDataMemb)) InitSelectedItem();

	ModifyItem(aKeyMemb);
//  MakeItemEx();
}

void CDgnGenMembChgDlg::InitSelectedItem() // 선택된 요소와 절점을 해제한다.
{  
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CDgnGenMembChgDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_CHK_INDEX), _LS(IDS_DGN_R), _LS(IDS_DGN_CHK_ELEM_LIST)};  // _T("Index"), _T(" "), _T("Element List")
	int nColWidth[] = {50, 30, 160};
	int nAlign[] = {LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_LEFT};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;  
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	for(int i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CDgnGenMembChgDlg::MakeItemEx()
{
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMemb();
	if(nItemCount == 0) return;

	POSITION pos;
	int nCount, nIndex;  

	if(nItemCount)
	{
		T_MEMB_K Key;
		T_MEMB_D Data;
		T_ELEM_K KeyElem;
		CString strMembIndex, strElemList;;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMemb();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextMemb(pos, Key, Data);
		  KeyBuf[nCount++] = Data.aElemList[0];
		}
		qsort(KeyBuf, nItemCount, sizeof(T_ELEM_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  KeyElem = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetMembAssigned(KeyElem, Key);
			m_pDoc->m_pAttrCtrl->GetMemb(Key,Data);            
			CString strReverse = _T("");      
			strMembIndex.Format(_T("%d"), Data.aElemList[0]);
			if(Data.bReverseDir) strReverse = _T("*"); 
			nIndex = m_List.InsertItem(nCount, strMembIndex);
			m_List.SetItemText(nIndex, 1, strReverse);      
			CString csItem;
			strElemList = _T("");
			for(int i = 0; i < Data.aElemList.GetSize() - 1; i++)
			{
				csItem.Format(_T("%d"), Data.aElemList[i]);
				strElemList += csItem + _T(", ");
			}
			if(Data.aElemList.GetSize() > 0)
			{
				csItem.Format(_T("%d"), Data.aElemList[Data.aElemList.GetSize()-1]);
				strElemList += csItem;
			}
			m_List.SetItemText(nIndex, 2, strElemList);      
			m_List.SetItemData(nIndex, (DWORD)Key);
		}
		delete []KeyBuf;
	}
}

void CDgnGenMembChgDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;  
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for (UINT i=0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

void CDgnGenMembChgDlg::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	 
	if (pNMListView->iSubItem != 999)
	{
		if (!m_bSentMessage)
		{
			m_TempListView = *pNMListView;
			m_TempListView.iSubItem = 999;
			PostMessage(WM_NOTIFY, IDC_DGN_MEMB_LIST, (LPARAM)&m_TempListView);
			m_bSentMessage = TRUE;
		}
		*pResult = 0;
		return;
	}
	DisplaySelElemList();  // 선택된 요소를 Display한다.  
	m_bSentMessage = FALSE;
	*pResult = 0;
}

void CDgnGenMembChgDlg::OnSetfocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	DisplaySelElemList();  // 선택된 요소를 Display한다.  
	*pResult = 0;
}

//  CButton* SelBtn = (CButton*)GetDlgItem(IDC_DGN_MEMB_CHG_SELECT_SEL);  
void CDgnGenMembChgDlg::DisplaySelElemList()
{
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelElem;  
	aSelElem.RemoveAll();  
	
	if(m_nSelectType == 0) // All
	{
		GetAllElemList(aSelElem);
	}
	else if(m_nSelectType == 1)  // By Selection
	{
		GetSelElemList(aSelElem);
	}
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aSelElem, FALSE, TRUE);
}

void CDgnGenMembChgDlg::GetAllElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem) 
{
	T_MEMB_D DataMemb;      
	CArray<T_MEMB_K, T_MEMB_K> aSelMemb;  
	aSelMemb.RemoveAll();
	
	m_pDoc->m_pAttrCtrl->GetMembKeyList(aSelMemb);
	for(int i = 0; i < aSelMemb.GetSize(); i++)
	{      
		if(m_pDoc->m_pAttrCtrl->GetMemb(aSelMemb[i], DataMemb))
		{
			for(int j = 0; j < DataMemb.aElemList.GetSize(); j++)
				aSelElem.Add(DataMemb.aElemList[j]);
		}
	}
}

void CDgnGenMembChgDlg::GetSelElemList(CArray<T_ELEM_K, T_ELEM_K>& aSelElem)
{
	T_MEMB_D DataMemb;
	CArray<int, int> aSelItem;  
	CArray<T_MEMB_K, T_MEMB_K> aMembKey;
	aMembKey.RemoveAll();
	aSelItem.RemoveAll();
	aSelElem.RemoveAll();  
	
	GetSelectedItemList(&m_List, aSelItem);

	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;
	for(int i = 0; i < nSize; i++)
	{
		DWORD(KeyMemb) = m_List.GetItemData(aSelItem[i]);    
		aMembKey.Add(KeyMemb);
	}  
	m_pDoc->m_pAttrCtrl->GetKeyListMembUser(&aMembKey, 0, &aSelElem);
}

// 현재 쓰이지 않는 함수임
void CDgnGenMembChgDlg::HighlightListSelElem()
{
	CArray<T_ELEM_K, T_ELEM_K> aSelElem;   // Select된 Element List
	CArray<T_MEMB_K, T_MEMB_K> aSelMemb;  
	CArray<int, int> aListIndex;
	CMap<T_MEMB_K, T_MEMB_K, T_MEMB_K, T_MEMB_K> mapMemb;
	mapMemb.InitHashTable(HASHSIZEMEMB);  
	T_MEMB_D DataMemb;
	T_MEMB_K KeyMemb, KeyTemp;
	aSelMemb.RemoveAll();
	aSelElem.RemoveAll();
	mapMemb.RemoveAll();

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();      
	pIGM->GetSelectedElemKeyListForDgn(aSelElem);
	m_pDoc->m_pAttrCtrl->GetMembKeyList(aSelMemb);

	int nSelElemCount = aSelElem.GetSize();
	for(int i = 0; i < nSelElemCount; i++)
	{    
		if(m_pDoc->m_pAttrCtrl->GetMembAssigned(aSelElem[i], KeyMemb))
			mapMemb.SetAt(KeyMemb, KeyMemb); 
	}

	int nSelMembCount = mapMemb.GetCount();
	int nListCount = m_List.GetItemCount();
	CString strMembIndex;  
	for(int i = 0; i < nListCount; i++)
	{
		DWORD(Key) = m_List.GetItemData(i);
		if(mapMemb.Lookup(Key, KeyTemp)) aListIndex.Add(i);    
	}

	for(int i = 0; i < nListCount; i++)
		m_List.SetItemState(i, 0,  LVIS_SELECTED);
	for(int i = 0; i < aListIndex.GetSize(); i++)
	{
		m_List.SetItemState(aListIndex[i], LVIS_SELECTED,  LVIS_SELECTED);
	}  
}  

// 현재 쓰이지 않는 함수임
void CDgnGenMembChgDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DBALL:
		case D_UPDATE_DEFAULT:
//      UpdateBuffer();
//      MakeItemEx();
			break;
		case D_UPDATE_BUFFER_BEFORE: 
			break;
		case D_UPDATE_BUFFER_AFTER:	
//      UpdateBuffer();
//      MakeItemEx();
			break;
		case D_UPDATE_UNIT:	
			break;
		case D_UPDATE_SEL_ADD: 
		case D_UPDATE_SEL_DEL:
			break;
		default: ASSERT(TRUE);
	}
}

// 현재 쓰이지 않는 함수임
void CDgnGenMembChgDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MEMB_K Key;
	T_MEMB_D Data;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MEMB_ADD):
			{
				pViewBuff->GetMemb(nKey, Key, Data);
				InsertItem(Key);
			}
			break;
		case(UR_MEMB_DEL):
			{
				pViewBuff->GetMemb(nKey, Key, Data);
				DeleteItem(Key);
			}
			break;
		case(UR_MEMB_MFS):
		case(UR_MEMB_MFD):
			{
				pViewBuff->GetMemb(nKey, Key, Data);
				ModifyItem(Key);
			}
			break;
		default:
			break;
		}
	} 
}

// 현재 쓰이지 않는 함수임
void CDgnGenMembChgDlg::ModifyItem(T_MEMB_K Key)
{
	// 수정할 위치를 찾는다.
	int nSize = m_List.GetItemCount();
    int nCount = 0;
	for(nCount = 0; nCount < nSize; nCount++)
	{
		if(m_List.GetItemData(nCount) == Key) break;
	}  
	CString strReverse = _T("");  
	T_MEMB_D Data;  
	if(!m_pDoc->m_pAttrCtrl->GetMemb(Key, Data)) Data.Initialize();
	if(Data.bReverseDir) strReverse = _T("*");
	if(nCount != nSize) m_List.SetItemText(nCount, 1, strReverse);  
}

// 현재 쓰이지 않는 함수임
void CDgnGenMembChgDlg::InsertItem(T_MEMB_K Key)
{
	// 삽입 위치를 찾는다.
	T_MEMB_D Data;
	int nSize = m_List.GetItemCount();
	m_pDoc->m_pAttrCtrl->GetMemb(Key, Data);  
	CString strMembIndex;     
	int nElemKey;
    int nCount = 0;
	for(nCount = 0; nCount < nSize; nCount++)
	{
		strMembIndex = m_List.GetItemText(nCount, 0);
		nElemKey = _ttoi(strMembIndex);
		if(nElemKey > Data.aElemList[0]) break;
	}  
	CString strReverse = _T("");
	strMembIndex.Format(_T("%d"), Data.aElemList[0]);
	if(Data.bReverseDir) strReverse = _T("*"); 
	int nIndex = m_List.InsertItem(nCount, strMembIndex);
	m_List.SetItemText(nIndex, 1, strReverse);      
	CString csItem;
	CString strElemList = _T("");
	for(int i = 0; i < Data.aElemList.GetSize() - 1; i++)
	{
		csItem.Format(_T("%d"), Data.aElemList[i]);
		strElemList += csItem + _T(", ");
	}
	if(Data.aElemList.GetSize() > 0)
	{
		csItem.Format(_T("%d"), Data.aElemList[Data.aElemList.GetSize()-1]);
		strElemList += csItem;
	}
	m_List.SetItemText(nIndex, 2, strElemList);      
	m_List.SetItemData(nIndex, (DWORD)Key);
}

// 현재 쓰이지 않는 함수임
void CDgnGenMembChgDlg::DeleteItem(T_MEMB_K Key)
{
	int nSize = m_List.GetItemCount();
    int nCount = 0;
	for(nCount = 0; nCount < nSize; nCount++)
	{
		if(m_List.GetItemData(nCount) == Key) break;
	}
	if(nCount != nSize) m_List.DeleteItem(nCount);
}

void CDgnGenMembChgDlg::ModifyItem(CArray<T_MEMB_K, T_MEMB_K>& aKeyMemb)
{
	int nKeyNum = aKeyMemb.GetSize();
	T_MEMB_K Key;
	for(int i = 0; i < nKeyNum; i++)
	{
		Key = aKeyMemb[i];
		int nSize = m_List.GetItemCount();
        int nCount = 0;
		for(nCount = 0; nCount < nSize; nCount++)
		{
			if(m_List.GetItemData(nCount) == Key) break;
		}  
		CString strReverse = _T("");  
		T_MEMB_D Data;  
		if(!m_pDoc->m_pAttrCtrl->GetMemb(Key, Data)) Data.Initialize();
		if(Data.bReverseDir) strReverse = _T("*");
		if(nCount != nSize) m_List.SetItemText(nCount, 1, strReverse);      
	}
}


