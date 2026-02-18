// DgnStlRatingGroupSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlRatingGroupSettingDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\CheckDialogOpen.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define _PROFILING
//#include "profilewrite.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingGroupSettingDlg dialog


CDgnStlRatingGroupSettingDlg::CDgnStlRatingGroupSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlRatingGroupSettingDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();	

	//{{AFX_DATA_INIT(CDgnStlRatingGroupSettingDlg)
	m_dFactor = 1.0;
	m_bIEnd = FALSE;
	m_bJEnd = FALSE;
	//}}AFX_DATA_INIT

    m_pDataCtrl = new CDgnDataCtrl;
}

CDgnStlRatingGroupSettingDlg::~CDgnStlRatingGroupSettingDlg()
{
    _SAFE_DELETE(m_pDataCtrl);
}

void CDgnStlRatingGroupSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlRatingGroupSettingDlg)
	DDX_Control(pDX, IDC_DGN_RATING_GROUP_LIST, m_listGroup);
	DDX_Control(pDX, IDC_DGN_RATING_GROUP_CHECK_LIST, m_checkListSelectGroup);
	DDX_Text(pDX, IDC_DGN_DEFINE_RATING_CASE_PRIMARY_FACTOR_EDIT, m_dFactor);
	DDX_Check(pDX, IDC_DGN_RATING_GROUP_I_CHECK, m_bIEnd);
	DDX_Check(pDX, IDC_DGN_RATING_GROUP_J_CHECK, m_bJEnd);
	//}}AFX_DATA_MAP
}

BOOL CDgnStlRatingGroupSettingDlg::OnInitDialog() 
{
	//PROFILING(_T("CDgnStlRatingGroupSettingDlg::OnInitDialog()"));

	CDialogMove::OnInitDialog();

	InitSelectGroupList();

	SetListCtrlHeader();
	
	GetDBRsgs();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlRatingGroupSettingDlg::InitSelectGroupList() 
{
	//PROFILING(_T("CDgnStlRatingGroupSettingDlg::InitSelectGroupList()"));
    BOOL bIsPost = m_pDoc->m_pPostCtrl->IsPostEnable();

	m_pDoc->m_pAttrCtrl->GetGrupKeyList(m_arGrupKey);

	CArray<T_ELEM_K, T_ELEM_K> ElemKeyList;
	
	for(int i=0; i<m_arGrupKey.GetSize(); i++)
	{
		bool isBeam = TRUE;
		bool isConcret = TRUE;
		bool isSect = FALSE;
		T_GRUP_D GroupData;

		GroupData.Initialize();
		m_pDoc->m_pAttrCtrl->GetGrup(m_arGrupKey[i], GroupData);

		ElemKeyList.Copy(GroupData.arKeyElem);

		T_ELEM_D elemData;
		T_MATL_D MatlD;
		T_SECT_D sectData;
		
		for(int e=0 ; e<ElemKeyList.GetSize() ; e++)
		{
			elemData.Initialize();
			MatlD.Initialize();
            if ( bIsPost )
            {
                if ( !m_pDoc->m_pPostCtrl->GetElemPost(ElemKeyList[e], elemData) )
                {
                    ASSERT(0);
                    continue;
                }
                if ( !m_pDoc->m_pPostCtrl->GetMatlPost(elemData.elmat, MatlD) )
                {
                    ASSERT(0);
                    continue;
                }
                //SectK는 Post Control에있는 요소의 SectK를 사용 
                //(변단면 그룹을 사용한경우에는 PostControl을 사용해야함)
                if ( !m_pDoc->m_pPostCtrl->GetSectPost(elemData.elpro, sectData) )
                {
                    ASSERT(0);
                    continue;
                }
            }
            else
            {
                if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemKeyList[e], elemData) )
                {
                    ASSERT(0);
                    continue;
                }
                if ( !m_pDoc->m_pAttrCtrl->GetMatl(elemData.elmat, MatlD) )
                {
                    ASSERT(0);
                    continue;
                }
                //SectK는 Post Control에있는 요소의 SectK를 사용 
                //(변단면 그룹을 사용한경우에는 PostControl을 사용해야함)
                if ( !m_pDoc->m_pAttrCtrl->GetSect(elemData.elpro, sectData) )
                {
                    ASSERT(0);
                    continue;
                }
            }
			
			//1. Group에 포함된 요소는 모두 Beam이어야한다.(Column이 하나라도 있음 안된다)
			if(m_pDataCtrl->Get_DgnGenMbtp(ElemKeyList[e]) != 2)
				isBeam = FALSE;

			//2. Group에 포함된 요소의 Material은 모두 Steel 이어야 한다.(Steel이 하나라도 있음 안된다.)
			if(!(MatlD.Type==_T("S") || MatlD.Type==_T("SRC")))	
				isConcret = FALSE;
			
			//3. Section Check 는 모르겠다!!!!!!!
				isSect = TRUE;
		}
				

		if(isBeam && isConcret && isSect)
		{
			CString strGrupName = GroupData.GroupName;
		
			int nIndex = m_checkListSelectGroup.AddString(strGrupName);
			m_checkListSelectGroup.SetItemData(nIndex, DWORD(m_arGrupKey[i]));
					
			m_checkListSelectGroup.SetCheck(nIndex, FALSE);
		
			m_mapListKey.SetAt(m_arGrupKey[i], nIndex);
		}		
	}
}

void CDgnStlRatingGroupSettingDlg::SetListCtrlHeader()
{
	CString aTitle[] = {_LS(IDS_DGN_RATING_GROUP), _LS(IDS_DGN_RATING_FACTOR), _LS(IDS_DGN_RATING_POSITION)};  //_T("Group"), _T("Factor_T(", ")Position")
	int nColWidth[] = {60, 60, 100};
	int nAlign[] = {LVCFMT_LEFT, LVCFMT_LEFT, LVCFMT_LEFT};
	int nColNum = sizeof(nColWidth)/sizeof(int);
	CString title;  
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_listGroup.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_listGroup.GetSafeHwnd(), dwStyle);

	for(int i = 0; i < nColNum; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_listGroup.InsertColumn(i,&lvcolumn);
		m_listGroup.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

int CDgnStlRatingGroupSettingDlg::FindInsertionPos(T_RSGS_K nID)
{
	int nCount = m_listGroup.GetItemCount();
	for (int i=0; i < nCount; i++)
	{
        T_RSGS_K key = m_listGroup.GetItemData(i);
		if (nID < key) { return i; }
	}
	return nCount;
}

BOOL CDgnStlRatingGroupSettingDlg::InsertListItem(T_RSGS_K Key)
{
	//DB에서 Data를 가져온다

	T_RSGS_D Data;

	m_pDoc->m_pAttrCtrl2->GetRsgs(Key, Data);
	

	LVITEM lvitem;
	CString str;
	int nItem = m_listGroup.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i=0; i < 3; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if (i == 0) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_listGroup.InsertItem(&lvitem);
		}
		else m_listGroup.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	m_listGroup.SetItemData(nItem, (DWORD)Key);
	return TRUE;
}

CString CDgnStlRatingGroupSettingDlg::DataToStr(int i, T_RSGS_K Key, T_RSGS_D &Data)
{  
	CString str;
	if(i == 0)
		str = m_pDoc->m_pAttrCtrl->GetGrupName(Key);		
	else if(i == 1)
		str.Format(_T("%f"), Data.dConditionFactor);		
	else if(i == 2)
	{
		if(Data.iChkPosition == 0)
			str = _LS(IDS_DGN_RATING_I_END);//_T("I-End");
		else if(Data.iChkPosition == 1)
			str = _LS(IDS_DGN_RATING_J_END);//_T("J-End");
		else if(Data.iChkPosition == 2)
			str = _LS(IDS_DGN_RATING_I_J_END);//_T("I-End, J-End");
		else
			str = _LS(IDS_DGN_RATING_NOT_SELECT);//_T("Not Select");
	}	
	else ASSERT(0);

	return str;
}

BOOL CDgnStlRatingGroupSettingDlg::DeleteListItem(T_RSGS_K key)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_listGroup.FindItem(&FindInfo);
	if(nItem==-1) return TRUE;

	m_listGroup.DeleteItem(nItem);

	return TRUE;
}

BOOL CDgnStlRatingGroupSettingDlg::ModifyListItem(T_RSGS_K key)
{
	T_RSGS_D Data;

	Data = m_MainData;

	LVFINDINFO FindInfo;
	FindInfo.flags  = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)key;

	int nItem = m_listGroup.FindItem(&FindInfo);
	if(nItem == -1) return TRUE;

	for(int i=0; i<3; i++)
	{
		CString str = DataToStr(i, key, Data);
		m_listGroup.SetItemText(nItem, i, str);
	}

	LVITEM lvitem;
	lvitem.iItem    = nItem;
	lvitem.iSubItem = 0;
	lvitem.mask     = LVIF_PARAM;
	lvitem.lParam   = (LPARAM)key;
	m_listGroup.SetItem(&lvitem);

	return TRUE;
}

void CDgnStlRatingGroupSettingDlg::LoadDlgFactorPosition()
{
	m_dFactor = m_MainData.dConditionFactor;

	if(m_MainData.iChkPosition == 0)
	{
		m_bIEnd = TRUE;
		m_bJEnd = FALSE;
	}
	else if(m_MainData.iChkPosition == 1)
	{
		m_bIEnd = FALSE;
		m_bJEnd = TRUE;
	}
	else if(m_MainData.iChkPosition == 2)
	{
		m_bIEnd = TRUE;
		m_bJEnd = TRUE;
	}
	else //예외 경우
	{
		m_bIEnd = FALSE;
		m_bJEnd = FALSE;
	}

	UpdateData(FALSE);
}

BOOL CDgnStlRatingGroupSettingDlg::SaveDlgFactorPosition()
{
	UpdateData(TRUE);

		if(m_dFactor < 0.0)  // MQC 18. condition factor value는 0이상 1 이하로 제한
		{
			m_dFactor = 0.0; 
		} 
		else if(m_dFactor > 1.0)
		{
			m_dFactor = 1.0;
		}
		UpdateData(FALSE);

	m_MainData.dConditionFactor = m_dFactor;

	if(!m_bIEnd && !m_bJEnd)
	{
		CString Message = _T("");
		Message.Format(_LS(IDS_DGN_RATING_ERROR_MESSAGE_NOT_SELECT));//_T("I-End, J-End 을 선택하셔야 합니다.")
		AfxMessageBox(Message, MB_OK);

		return FALSE;
	}

	if(m_bIEnd && !m_bJEnd)
		m_MainData.iChkPosition = 0;
	else if(!m_bIEnd && m_bJEnd)
		m_MainData.iChkPosition = 1;
	if(m_bIEnd && m_bJEnd)
		m_MainData.iChkPosition = 2;
	

	return TRUE;
}

BOOL CDgnStlRatingGroupSettingDlg::SaveDlgSelectGroup()
{
	UpdateData(TRUE);

	DWORD nID;
	m_arSelectGrupKey.RemoveAll();

	int nCount = m_checkListSelectGroup.GetCount();
	
	for (int c = 0; c < nCount ; c++)
	{
		if(m_checkListSelectGroup.GetCheck(c) == 1)
		{
			nID = m_checkListSelectGroup.GetItemData(c);			
			m_arSelectGrupKey.Add(nID);
		}
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(CDgnStlRatingGroupSettingDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnStlRatingGroupSettingDlg)
	ON_BN_CLICKED(IDC_DGN_RATING_GROUP_ADD_BTN, OnDgnRatingGroupAddBtn)
	ON_BN_CLICKED(IDC_DGN_RATING_GROUP_MODIFY_BTN, OnDgnRatingGroupModifyBtn)
	ON_BN_CLICKED(IDC_DGN_RATING_GROUP_DELETE_BTN, OnDgnRatingGroupDeleteBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_RATING_GROUP_LIST, OnItemchangedDgnRatingGroupList)
	ON_BN_CLICKED(IDC_DGN_RATING_GROUP_CLOSE, OnDgnRatingGroupClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlRatingGroupSettingDlg message handlers

void CDgnStlRatingGroupSettingDlg::OnDgnRatingGroupAddBtn() 
{
	// TODO: Add your control notification handler code here
	if(!SaveDlgSelectGroup())
		return;
	if(!SaveDlgFactorPosition())
		return;

	CArray<T_RSGS_K, T_RSGS_K> arKey;

	for(int i=0 ; i<m_arSelectGrupKey.GetSize() ; i++)
	{
		T_RSGS_K key = m_arSelectGrupKey.GetAt(i);
		if(m_pDoc->m_pAttrCtrl2->ExistRsgs(key))
		{
			arKey.Add(key);
		}
		else
		{
			if(m_pDoc->m_pDataCtrl->AddRsgs(key, m_MainData))
				InsertListItem(key);
		}
	}

	CString Message = _T("");
	for(int i=0 ; i<arKey.GetSize() ; i++)
	{		
		CString temp = _T("");

		temp = m_pDoc->m_pAttrCtrl->GetGrupName(arKey.GetAt(i));
		if(i == 0)
			Message += temp;
		else
			Message += _T(", ") + temp;		
	}

	if(Message != _T(""))
		AfxMessageBox(Message + _LS(IDS_DGN_RATING_ERROR_MESSAGE_SELECTED_GROUP), MB_OK);//_T("은\n이미 선택되어진 Group입니다.")
}

void CDgnStlRatingGroupSettingDlg::OnDgnRatingGroupModifyBtn() 
{
	// TODO: Add your control notification handler code here
	if(!SaveDlgSelectGroup())
		return;
	if(!SaveDlgFactorPosition())
		return;

	CArray<T_RSGS_K, T_RSGS_K> arKey;

	for(int i=0 ; i<m_arSelectGrupKey.GetSize() ; i++)
	{
		T_RSGS_K key = m_arSelectGrupKey.GetAt(i);
		if(!m_pDoc->m_pAttrCtrl2->ExistRsgs(key))
		{
			arKey.Add(key);
		}
		else
		{
			if(m_pDoc->m_pDataCtrl->ModifyRsgs(key, key, m_MainData))
				ModifyListItem(key);
		}
	}

	CString Message = _T("");
	for(int i=0 ; i<arKey.GetSize() ; i++)
	{		
		CString temp = _T("");

		temp = m_pDoc->m_pAttrCtrl->GetGrupName(arKey.GetAt(i));
		if(i == 0)
			Message += temp;
		else
			Message += _T(", ") + temp;		
	}

	if(Message != _T(""))
		AfxMessageBox(Message + _LS(IDS_DGN_RATING_ERROR_MESSAGE_NOT_INPUT), MB_OK);//_T("은\n입력되지 않았습니다.")
}

void CDgnStlRatingGroupSettingDlg::OnDgnRatingGroupDeleteBtn() 
{
	// TODO: Add your control notification handler code here
	CArray<int, int> aSelItem;
	CArray<T_RSGS_K, T_RSGS_K> arKey;
	aSelItem.RemoveAll();
	arKey.RemoveAll();
	
	GetSelectedItemList(&m_listGroup, aSelItem);
	int nSize = aSelItem.GetSize();
	if(nSize == 0) return;

	for(int i = 0; i < nSize; i++)
	{
		DWORD(key) = m_listGroup.GetItemData(aSelItem[i]);
		arKey.Add(key);	
	}

	for(int i=0 ; i<arKey.GetSize() ; i++)
	{
		if(m_pDoc->m_pAttrCtrl2->ExistRsgs(arKey[i]))
		{
			if(m_pDoc->m_pDataCtrl->DelRsgs(arKey[i]))
				DeleteListItem(arKey[i]);  
		}
	}	
}

void CDgnStlRatingGroupSettingDlg::GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem)
{
	int  nItem = -1;
	UINT uSelectedCount = pList->GetSelectedCount();
	aSelItem.SetSize(uSelectedCount);
	for(UINT i = 0; i < uSelectedCount; i++)
	{
		nItem = pList->GetNextItem(nItem, LVNI_SELECTED);
		ASSERT(nItem != -1);
		aSelItem[i] = nItem;
	}
}

void CDgnStlRatingGroupSettingDlg::OnItemchangedDgnRatingGroupList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nIndex = pNMListView->iItem; 
	ChangeItem(nIndex);
	*pResult = 0;
}

void CDgnStlRatingGroupSettingDlg::ChangeItem(int nIndex)
{
	//PROFILING(_T("CDgnStlRatingGroupSettingDlg::ChangeItem(int nIndex)"));

	T_GRUP_K grupKey;
	int listKey;

	POSITION Pos = m_mapListKey.GetStartPosition();
	while(Pos)
	{
		m_mapListKey.GetNextAssoc(Pos,grupKey,listKey);
		m_checkListSelectGroup.SetCheck(listKey, FALSE);
	}  
		
	if (nIndex != -1)
	{
	  grupKey = m_listGroup.GetItemData(nIndex);
		
		m_mapListKey.Lookup(grupKey, listKey);	
		m_checkListSelectGroup.SetCheck(listKey, TRUE);  
	}

	m_pDoc->m_pAttrCtrl2->GetRsgs(grupKey, m_MainData);
	LoadDlgFactorPosition();
}

void CDgnStlRatingGroupSettingDlg::OnDgnRatingGroupClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CDgnStlRatingGroupSettingDlg::GetDBRsgs() 
{
	CArray<T_RSGS_K, T_RSGS_K> RsgsKeyList;

	m_pDoc->m_pAttrCtrl2->GetRsgsKeyList(RsgsKeyList);

	for(int i=0 ; i<RsgsKeyList.GetSize() ; i++)
	{
		InsertListItem(RsgsKeyList[i]);
	}

	if(RsgsKeyList.GetSize()>0)
	{
		m_pDoc->m_pAttrCtrl2->GetRsgs(RsgsKeyList[0], m_MainData);
		LoadDlgFactorPosition();
	}


}
