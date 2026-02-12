// MhtpMaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "MhtpMaterialDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_cmd\wg_cmd_Dialog.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMhtpMaterialDlg dialog


CMhtpMaterialDlg::CMhtpMaterialDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CMhtpMaterialDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMhtpMaterialDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CMhtpMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMhtpMaterialDlg)
	DDX_Control(pDX, IDC_TM_MHTP_TYPECOMBO, m_CobxMatType);
	DDX_Control(pDX, IDC_TM_MHTP_MATLIST, m_wndTDefMat);
	DDX_Control(pDX, IDC_TM_MHTP_RLIST, m_rlist);
	DDX_Control(pDX, IDC_TM_MHTP_LLIST, m_llist);
	//}}AFX_DATA_MAP
}

void CMhtpMaterialDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		MakeItemExMatlist();
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

void CMhtpMaterialDlg::UpdateBuffer()
{

	CViewBuff* pViewBuff=  CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount ==0 ) return;

	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsMhtpCommand = FALSE;
	BOOL bIsMhatCommand = FALSE;
	BOOL bIsMatlCommand = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_MATL_ADD):
		case(UR_MATL_DEL):
		case(UR_MATL_MFD):
		case(UR_MATL_MFS):
			bIsMatlCommand = TRUE;
			break;
		case(UR_MHTP_ADD):
		case(UR_MHTP_DEL):
		case(UR_MHTP_MFD): 
		case(UR_MHTP_MFS):
			bIsMhtpCommand = TRUE;
			break;
		case(UR_MHAT_ADD):
		case(UR_MHAT_DEL):
			bIsMhatCommand = TRUE;
			break;
		default:
			break;
		}
	}

	if(bIsMhtpCommand || bIsMhatCommand) 
	{
		MakeItemExMatlist();
	}
	if(bIsMatlCommand)
	{
		UpdateLList();
		UpdateRList();
		MakeItemExMatlist();
	}

}

void CMhtpMaterialDlg::UpdateLList()
{
	CArray<T_MATL_K, T_MATL_K> MatlKeys;
	CArray<T_MATL_K, T_MATL_K> MatlStlKeys;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(MatlKeys);
	
	T_MATL_D MatlD;  
	for (int n=0; n<MatlKeys.GetCount(); n++)
	{
		MatlD.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetMatl(MatlKeys[n],MatlD))
		{
			if (MatlD.Type == _T("S"))
				MatlStlKeys.Add(MatlKeys[n]);
			else
				continue;
		}
	}

	int nMatls = MatlStlKeys.GetSize();
	m_llist.DeleteAllItems();

	if(nMatls)
	{
		CStringArray Contents;
		CString      TStr;
		
		for(int i = 0 ;i < nMatls; i++)
		{
			MatlD.Initialize();
			if(m_pDoc->m_pAttrCtrl->GetMatl(MatlStlKeys[i],MatlD))
			{
				Contents.RemoveAll();
				TStr.Format(_T("%d:%s"),MatlStlKeys[i],MatlD.Name);
				Contents.Add(TStr);
				CDlgUtil::SetListItem(&m_llist,i,Contents,MatlStlKeys[i]);
			}
		}
	}
}

void CMhtpMaterialDlg::UpdateRList()
{
	CArray<T_MATL_K, T_MATL_K> rMatlKeys;
	CArray<T_MATL_K, T_MATL_K> rLMatlKeys; // Local Matl Keys
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(rMatlKeys);

	int nItems = m_rlist.GetItemCount();

	for(int i = 0; i < nItems; i++)
	{
		rLMatlKeys.Add((T_MATL_K) m_rlist.GetItemData(i));
	}

	CArray<T_MATL_K, T_MATL_K> rResult;

	I_GENModelBase::GetCurMySelfST()->IntersectKey(rMatlKeys,rLMatlKeys,rResult);

	m_rlist.DeleteAllItems();

	T_MATL_D MatlD;

	nItems = rResult.GetSize();
	if(!nItems) return;

	CStringArray Contents;
	CString      TStr;
	for(int i=0; i< nItems; i++)
	{  
		if(m_pDoc->m_pAttrCtrl->GetMatl(rResult[i],MatlD))
		{
			Contents.RemoveAll();
			TStr.Format(_T("%d:%s"),rResult[i],MatlD.Name);
			Contents.Add(TStr);
			CDlgUtil::SetListItem(&m_rlist,i,Contents,rResult[i]);
		}
	}
}

void CMhtpMaterialDlg::AddToRList(CArray<T_MATL_K, T_MATL_K> & rToAdd)
{
	CArray<T_MATL_K, T_MATL_K> rLMatlKeys;

	T_MATL_K MatlK;

	int nItems = m_rlist.GetItemCount();

	for(int i = 0 ; i< nItems ; i++)
	{
		MatlK = (T_MATL_K) m_rlist.GetItemData(i);
		rLMatlKeys.Add(MatlK);
	}


	CArray<T_MATL_K, T_MATL_K> rResult;

	I_GENModelBase::GetCurMySelfST()->UnionKey(rToAdd,rLMatlKeys,rResult);

	m_rlist.DeleteAllItems();

	T_MATL_D MatlD;

	nItems = rResult.GetSize();
	if(!nItems) return;

	CStringArray Contents;
	CString TStr;
	for(int i=0; i< nItems; i++)
	{  
		if(m_pDoc->m_pAttrCtrl->GetMatl(rResult[i],MatlD))
		{
			Contents.RemoveAll();
			TStr.Format(_T("%d:%s"),rResult[i],MatlD.Name);
			Contents.Add(TStr);
			CDlgUtil::SetListItem(&m_rlist,i,Contents,rResult[i]);
		}
	}
}

void CMhtpMaterialDlg::RemoveFromRList(CArray<T_MATL_K, T_MATL_K> & rToRemove)
{
	CArray<T_MATL_K, T_MATL_K> rLMatlKeys;

	T_MATL_K MatlK;

	int nItems = m_rlist.GetItemCount();

	for(int i = 0 ; i< nItems ; i++)
	{
		MatlK = (T_MATL_K) m_rlist.GetItemData(i);
		rLMatlKeys.Add(MatlK);
	}


	CArray<T_MATL_K, T_MATL_K> rResult;

	I_GENModelBase::GetCurMySelfST()->SubtractKey(rLMatlKeys,rToRemove,rResult);

	m_rlist.DeleteAllItems();

	T_MATL_D MatlD;

	nItems = rResult.GetSize();
	if(!nItems) return;

	CStringArray Contents;
	for(int i = 0 ; i< nItems; i++)
	{  
		if(m_pDoc->m_pAttrCtrl->GetMatl(rResult[i],MatlD))
		{
			Contents.RemoveAll();
			Contents.Add(MatlD.Name);
			CDlgUtil::SetListItem(&m_rlist,i,Contents,rResult[i]);
		}
	}
}

BEGIN_MESSAGE_MAP(CMhtpMaterialDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CMhtpMaterialDlg)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_MHTP_TYPEBTN, OnTmMalTypebtn)
	ON_BN_CLICKED(IDC_TM_MHTP_ADDBTN, OnTmMhatAddbtn)
	ON_BN_CLICKED(IDC_TM_MHTP_DELBTN, OnTmMhatDelbtn)
	ON_BN_CLICKED(IDC_TM_MHTP_LMOVEBTN, OnTmMhatLmovebtn)
	ON_BN_CLICKED(IDC_TM_MHTP_RMOVEBTN, OnTmMhatRmovebtn)
	ON_NOTIFY(NM_DBLCLK, IDC_TM_MHTP_LLIST, OnDblclkTmMhatLlist)	
	ON_NOTIFY(NM_DBLCLK, IDC_TM_MHTP_RLIST, OnDblclkTmMhatRlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_MHTP_MATLIST, OnItemchangedTmTdefMatlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMhtpMaterialDlg message handlers

BOOL CMhtpMaterialDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	// TODO: Add extra initialization here
	SetHighTemperDefMatHeader();
	//InitLRList();
	UpdateLList();
	MakeItemExMatlist();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMhtpMaterialDlg::SetHighTemperDefMatHeader()
{
	CStringArray Titles;
	CArray<int,int>ColWidth;

	Titles.Add(_LS(IDS_WG_TREEMENU_MHTP_DLG_LISTCTRL_TITLE_NO));
	Titles.Add(_LS(IDS_WG_TREEMENU_MHTP_DLG_LISTCTRL_TITLE_MAT));
	Titles.Add(_LS(IDS_WG_TREEMENU_MHTP_DLG_LISTCTRL_TITLE_PARA));

	ColWidth.Add(55);
	ColWidth.Add(60);
	ColWidth.Add(80);
	CDlgUtil::_SetListCtrlHeader(&m_wndTDefMat,Titles,NULL,&ColWidth);
}

void CMhtpMaterialDlg::MakeItemExMatlist()
{
	CArray<T_MHAT_K, T_MHAT_K> rKeyList;
	m_pDoc->m_pAttrCtrl->GetMhatKeyList(rKeyList);
	
	m_wndTDefMat.DeleteAllItems();
	
	int nKey = rKeyList.GetSize();
	if(nKey == 0) return;

	
	T_MHAT_D MhatD;
	T_MATL_D MatlD;
	T_MHTP_D MhtpD;

	CStringArray Contents;
	CString      Tstr;

	for(int i = 0; i< nKey; i ++)
	{
		if(m_pDoc->m_pAttrCtrl->GetMhat(rKeyList[i],MhatD) &&
			m_pDoc->m_pAttrCtrl->GetMatl(rKeyList[i],MatlD))
		{
			Contents.RemoveAll();
			Tstr.Format(_T("%d"),rKeyList[i]);
			Contents.Add(Tstr);
			Contents.Add(MatlD.Name);
			if(m_pDoc->m_pAttrCtrl->GetMhtp(MhatD.HighMatlTypeKey,MhtpD))
				Contents.Add(MhtpD.strMatlName);
			else
				Contents.Add(_T(" "));

			CDlgUtil::SetListItem(&m_wndTDefMat,i,Contents,rKeyList[i]);
		}
	}
}

BOOL CMhtpMaterialDlg::InsertItemMatlist(T_MHTP_K Key, T_MHTP_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_wndTDefMat.GetItemCount();

	lvitem.iItem = FindInsertionPosMatlist(Key);
	for(int i=0; i < 3; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStrMatlist(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_wndTDefMat.InsertItem(&lvitem);
		}
		else m_wndTDefMat.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	m_wndTDefMat.SetItemData(lvitem.iItem, Key);

	return TRUE;
}

int CMhtpMaterialDlg::FindInsertionPosMatlist(T_MHTP_K nID)
{
	T_MHTP_K key;

	int nCount = m_wndTDefMat.GetItemCount();
	int i = 0;
	for (i=0; i < nCount; i++)
	{
		key = m_wndTDefMat.GetItemData(i);
		if (nID <= key) break;
	}

	if (nID == key) m_wndTDefMat.DeleteItem(i);
	return i;
}

CString CMhtpMaterialDlg::DataToStrMatlist(int i, T_MHTP_K Key, T_MHTP_D &Data)
{
	UpdateData();
	CString str;

	if (i == 0) str.Format(_T("%d"), Key);
	else if(i == 1)
	{
		T_MATL_D matdata;
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetMatl(Key, matdata);
		str.Format(_T("%s"), 	matdata.Name);
	}
	else if(i == 2)
	{
		str.Format(_T("%s"), 	Data.strMatlName);
	}

	return str;
}

///////////////////////////////////////////////////////////////////////////////////////

void CMhtpMaterialDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

#include "..\wg_cmd\TemperatureRisePropListDlg.h"
void CMhtpMaterialDlg::OnTmMalTypebtn() 
{
	CTemperatureRisePropListDlg Dlg;
	Dlg.DoModal();
}

void CMhtpMaterialDlg::OnTmMhatAddbtn() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString str;

	m_CobxMatType.GetWindowText(str);
	if (str.CompareNoCase(_LS(IDS_WG_TREEMENU_NONE)) == 0) return;

	T_MATL_K key;
	T_MHTP_K mhtpkey;
	T_MHAT_D data;

	int nNum = m_rlist.GetItemCount();
	for(int i=0; i < nNum; i++)
	{
		key = m_rlist.GetItemData(i);
		mhtpkey = m_pDoc->m_pAttrCtrl->GetMhtpKey(str);
		data.HighMatlTypeKey = mhtpkey;
		m_pDoc->m_pDataCtrl->AddMhat(key, data);
	}
	MakeItemExMatlist();
	m_rlist.DeleteAllItems();
}

void CMhtpMaterialDlg::OnTmMhatDelbtn() 
{
	CArray<DWORD,DWORD> rData;
	CArray<T_MHAT_K, T_MHAT_K> MhatKeys;
	CDlgUtil::GetSelectedListItemData(&m_wndTDefMat,rData);

	int nData = rData.GetSize();
	if(nData)
	{
		for(int i = 0; i < nData; i++)
		{
			MhatKeys.Add((T_MHAT_K)rData[i]);
		}
		m_pDoc->m_pDataCtrl->DelTmat(MhatKeys);
	}
	// TODO: Add your control notification handler code here
	int iItem = m_wndTDefMat.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_No_selected_Mhtpation_Material_type));
		return;
	}
	T_MHAT_K key;
	key = m_wndTDefMat.GetItemData(iItem);
	if(!m_pDoc->m_pDataCtrl->DelMhat(key)) return;
	MakeItemExMatlist();
	int nCount = m_wndTDefMat.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_wndTDefMat.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CMhtpMaterialDlg::OnTmMhatRmovebtn() 
{
	// TODO: Add your control notification handler code here
	CArray<DWORD, DWORD> SItem;
	CArray<T_MATL_K, T_MATL_K> SMatlKeys;
	CDlgUtil::GetSelectedListItemData(&m_llist,SItem);

	int nItem = SItem.GetSize();

	for(int i = 0; i < nItem ;i++)
	{
		SMatlKeys.Add((T_MATL_K)SItem[i]);
	}

	AddToRList(SMatlKeys);

}

void CMhtpMaterialDlg::OnDblclkTmMhatLlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnTmMhatRmovebtn();
	*pResult = 0;
}

void CMhtpMaterialDlg::OnTmMhatLmovebtn() 
{
	// TODO: Add your control notification handler code here
	POSITION pos;
	int curpos;

	int nNum = m_rlist.GetSelectedCount();
	pos = m_rlist.GetFirstSelectedItemPosition();
	for(int i=0; i < nNum; i++)
	{
		curpos = m_rlist.GetNextSelectedItem(pos);
		m_rlist.DeleteItem(curpos);
	}
}

void CMhtpMaterialDlg::OnDblclkTmMhatRlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnTmMhatLmovebtn();
	*pResult = 0;
}

void CMhtpMaterialDlg::OnItemchangedTmTdefMatlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//m_wndTDefMat.Get  
	CArray<int,int>arRet;
	CDlgUtil::GetSelectedListItemIndex(&m_wndTDefMat,arRet);
	if(arRet.GetSize() != 1) return;

	T_MHAT_K MhatK = (T_MHAT_K) m_wndTDefMat.GetItemData(arRet[0]);
	T_MHAT_D MhatD;
	if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetMhat(MhatK,MhatD))
	{
		CArray<T_MATL_K, T_MATL_K> AddMAT;
		
		this->m_CobxMatType.ChangeSelect(MhatD.HighMatlTypeKey);
		m_rlist.DeleteAllItems();
		AddMAT.Add(MhatK);
		this->AddToRList(AddMAT);
	}

}