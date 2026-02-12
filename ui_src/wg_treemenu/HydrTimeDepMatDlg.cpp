// HydrTimeDepMatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "HydrTimeDepMatDlg.h"
#include "..\wg_main\wg_mainres2.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_cmd\wg_cmd_Dialog.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_dbLock\LockOption.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHydrTimeDepMatDlg dialog
#define CDialog CMenuBarChildDlg

CHydrTimeDepMatDlg::CHydrTimeDepMatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHydrTimeDepMatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHydrTimeDepMatDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CHydrTimeDepMatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHydrTimeDepMatDlg)
	DDX_Control(pDX, IDC_TM_ELASTICITY_TYPECOMBO, m_CobxTdme);
	DDX_Control(pDX, IDC_TM_HMAT_TYPECOMBO, m_CobxMatType);
	DDX_Control(pDX, IDC_TM_SELECTED_MAT_TYPELIST, m_rlist);
	DDX_Control(pDX, IDC_TM_SOURCE_MAT_TYPELIST, m_llist);
	DDX_Control(pDX, IDC_TM_TDEF_MATLIST, m_wndTDefMat);
	//}}AFX_DATA_MAP
}

void CHydrTimeDepMatDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
	 	//UpdateBuffer();
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


void CHydrTimeDepMatDlg::UpdateBuffer()
{
	
	CViewBuff* pViewBuff=  CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount ==0 ) return;

	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdmtCommand = FALSE;
	BOOL bIsTmatCommand = FALSE;
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
		case(UR_TDMT_ADD):
		case(UR_TDMT_DEL):
		case(UR_TDMT_MFD): 
		case(UR_TDMT_MFS):
			bIsTdmtCommand = TRUE;
			break;
		case(UR_TMAT_ADD):
		case(UR_TMAT_DEL):
			bIsTmatCommand = TRUE;
			break;
		default:
			break;
		}
	}

	if(bIsTdmtCommand || bIsTmatCommand) 
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
/*
void CHydrTimeDepMatDlg::UpdateTdmtList()
{
	T_TDMT_D TdmtD;
	CArray<T_TDMT_K, T_TDMT_K> rKeyList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdmtKeyList(rKeyList);

	int nTdmt = rKeyList.GetSize();
	CStringArray Contents;
	CString      TempStr;
	for(int i = 0 ;i  < nTdmt; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdmt(rKeyList[i],TdmtD))
		{
			//TdmtD.strMatlName;  // second ...


		
		}
	}
}
*/
/*
void CHydrTimeDepMatDlg::UpdateRList()
{
	CArray<T_MATL_K, T_MATL_K> aMatlKeyList;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetMatlKeyList(aMatlKeyList);
	int nMatCount = aMatlKeyList.GetSize();
	T_MATL_D matData;
	CString str;

	m_llist.DeleteAllItems();

	for (int i=0; i<nMatCount; i++)
	{
		int ix = m_llist.InsertItem(i, _T(""));
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetMatl(aMatlKeyList[i], matData);
		str.Format(_T("%s"), aMatlKeyList[i], matData.Name);

		m_llist.SetItemText(ix, 0, str);
		m_llist.SetItemData(ix, aMatlKeyList[i]);
	}
}
*/

void CHydrTimeDepMatDlg::UpdateLList()
{
	CArray<T_MATL_K, T_MATL_K> MatlKeys;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(MatlKeys);
	
	int nMatls = MatlKeys.GetSize();
	m_llist.DeleteAllItems();
	
	if(nMatls)
	{
		CStringArray Contents;
		CString      TStr;
		T_MATL_D MatlD;  
		

		for(int i = 0 ;i < nMatls; i++)
		{
			if(m_pDoc->m_pAttrCtrl->GetMatl(MatlKeys[i],MatlD))
			{
				Contents.RemoveAll();
				TStr.Format(_T("%d:%s"),MatlKeys[i],MatlD.Name);
				Contents.Add(TStr);
				CDlgUtil::SetListItem(&m_llist,i,Contents,MatlKeys[i]);
			}
		}
	}
}

void CHydrTimeDepMatDlg::UpdateRList()
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

void CHydrTimeDepMatDlg::AddToRList(CArray<T_MATL_K, T_MATL_K> & rToAdd)
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

void CHydrTimeDepMatDlg::RemoveFromRList(CArray<T_MATL_K, T_MATL_K> & rToRemove)
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
	CString TStr;
	for(int i = 0 ; i< nItems; i++)
	{  
		if(m_pDoc->m_pAttrCtrl->GetMatl(rResult[i],MatlD))
		{
			Contents.RemoveAll();
			TStr.Format(_T("%d:%s"), rResult[i], MatlD.Name);
			Contents.Add(TStr);
			CDlgUtil::SetListItem(&m_rlist,i,Contents,rResult[i]);
		}
	}
}


BEGIN_MESSAGE_MAP(CHydrTimeDepMatDlg, CDialog)
	//{{AFX_MSG_MAP(CHydrTimeDepMatDlg)
	//ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_MAT_TYPEBTN, OnTmMatTypebtn)
	ON_BN_CLICKED(IDC_TM_HMAT_ADDBTN, OnTmHmatAddbtn)
	ON_BN_CLICKED(IDC_TM_HMAT_DELBTN, OnTmHmatDelbtn)
	ON_BN_CLICKED(IDC_TM_HMAT_LMOVEBTN, OnTmHmatLmovebtn)
	ON_BN_CLICKED(IDC_TM_HMAT_RMOVEBTN, OnTmHmatRmovebtn)
	ON_BN_CLICKED(IDC_TM_ELASTICITY_TYPEBTN, OnTmElasticityTypebtn)
	ON_NOTIFY(NM_DBLCLK, IDC_TM_SOURCE_MAT_TYPELIST, OnDblclkTmSourceMatTypelist)
	ON_NOTIFY(NM_DBLCLK, IDC_TM_SELECTED_MAT_TYPELIST, OnDblclkTmSelectedMatTypelist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_TDEF_MATLIST, OnItemchangedTmTdefMatlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CHydrTimeDepMatDlg message handlers

BOOL CHydrTimeDepMatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	SetTimeDefMatHeader();
	//InitLRList();
	UpdateLList();
	MakeItemExMatlist();

#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_STAGE)
		&& !m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_THERMAL_STRESS))
	{
		GetDlgItem(IDC_TM_MAT_TYPEBTN)->EnableWindow(FALSE);
	}
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHydrTimeDepMatDlg::SetTimeDefMatHeader()
{
	//this->m_wndTDefMat  
	CStringArray Titles;
	//CArray<float,float> ColWRatio;
	CArray<int,int>ColWidth;

	Titles.Add(_LS(IDS_WG_TREEMENU_TDMT_DLG_LISTCTRL_TITLE_NO));
	Titles.Add(_LS(IDS_WG_TREEMENU_TDMT_DLG_LISTCTRL_TITLE_MAT));
	Titles.Add(_LS(IDS_WG_TREEMENU_TDMT_DLG_LISTCTRL_TITLE_CREEP));
	Titles.Add(_LS(IDS_WG_TREEMENU_TDMT_DLG_LISTCTRL_TITLE_ELAST));
	ColWidth.Add(29);
	ColWidth.Add(55);
	ColWidth.Add(60);
	ColWidth.Add(60); 
	
	CDlgUtil::_SetListCtrlHeader(&m_wndTDefMat,Titles,NULL,&ColWidth);
	//CDlgUtil::SetListCtrlGridLine(&m_wndTDefMat);
}


///////////////////////////////////////////////////////////////////////////////////
// Left List, Right List
/*
void CHydrTimeDepMatDlg::UpdateLList()
{
	CRect listRect;
	m_llist.GetClientRect(&listRect);
	int nListWidth = (listRect.right - listRect.left);
	m_llist.InsertColumn(0, _T(""), LVCFMT_LEFT, nListWidth);
	m_rlist.InsertColumn(0, _T(""), LVCFMT_LEFT, nListWidth);

	CArray<T_MATL_K, T_MATL_K> aMatlKeyList;
	m_pDoc->m_pAttrCtrl->GetMatlKeyList(aMatlKeyList);
	int nMatCount = aMatlKeyList.GetSize();
	T_MATL_D matData;
	CString str;

	for (int i=0; i<nMatCount; i++)
	{
		int ix = m_llist.InsertItem(i, _T(""));
		m_pDoc->m_pAttrCtrl->GetMatl(aMatlKeyList[i], matData);
		str.Format(_LS(IDS_WG_TREEMENU__d__s), aMatlKeyList[i], matData.Name);

		m_llist.SetItemText(ix, 0, str);
		m_llist.SetItemData(ix, aMatlKeyList[i]);
	}
}
*/
void CHydrTimeDepMatDlg::MakeItemExMatlist()
{
	CArray<T_TMAT_K, T_TMAT_K> rKeyList;
	m_pDoc->m_pAttrCtrl->GetTmatKeyList(rKeyList);
	
	m_wndTDefMat.DeleteAllItems();
	
	int nKey = rKeyList.GetSize();
	if(nKey == 0) return;

	
 
	T_TMAT_D TMatD;
	T_MATL_D MatlD;
	T_TDMT_D TdmtD;
	T_TDME_D TdmeD;

	CStringArray Contents;
	CString      Tstr;

	for(int i = 0; i< nKey; i ++)
	{
		if(m_pDoc->m_pAttrCtrl->GetTmat(rKeyList[i],TMatD) &&
			 m_pDoc->m_pAttrCtrl->GetMatl(rKeyList[i],MatlD)   )
		{
			
			
			
			Contents.RemoveAll();
			Tstr.Format(_T("%d"),rKeyList[i]);
			Contents.Add(Tstr);
			Contents.Add(MatlD.Name);
			if(m_pDoc->m_pAttrCtrl->GetTdmt(TMatD.TdMatlTypeKey,TdmtD))
				Contents.Add(TdmtD.strMatlName);
			else
				Contents.Add(_T(" "));
			
			if(m_pDoc->m_pAttrCtrl->GetTdme(TMatD.TdMatlElastKey,TdmeD))
				Contents.Add(TdmeD.strMatlName);
			else
				Contents.Add(_T(" "));

			CDlgUtil::SetListItem(&m_wndTDefMat,i,Contents,rKeyList[i]);
		}
	}
	


	/*
	m_wndTDefMat.DeleteAllItems();

	int nItemCount = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetCountTdmt();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_TDMT_K Key;
		T_TDMT_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStartTdmt();
		while(pos != NULL)
		{
		  CDBDoc::GetDocPoint()->m_pAttrCtrl->GetNextTdmt(pos, Key, Data);
			if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->ExistTdmt(Key)) continue;
		  KeyBuf[nCount++] = Key;
		}

		for(nCount=0 ; nCount < nItemCount ; nCount++)
	  {
		  Key = KeyBuf[nCount];
			CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTdmt(Key, Data);
			InsertItemMatlist(Key, Data);
		}
		delete []KeyBuf;
	}
	*/
}

BOOL CHydrTimeDepMatDlg::InsertItemMatlist(T_TDMT_K Key, T_TDMT_D &Data)
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

int CHydrTimeDepMatDlg::FindInsertionPosMatlist(T_TDMT_K nID)
{
	T_TDMT_K key;
	int i = 0;
	int nCount = m_wndTDefMat.GetItemCount();
	for (i=0; i < nCount; i++)
	{
		key = m_wndTDefMat.GetItemData(i);
		if (nID <= key) break;
	}

	if (nID == key) m_wndTDefMat.DeleteItem(i);
	return i;
}

CString CHydrTimeDepMatDlg::DataToStrMatlist(int i, T_TDMT_K Key, T_TDMT_D &Data)
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

void CHydrTimeDepMatDlg::OnTmClose() 
{
	CloseDlg();
}

void CHydrTimeDepMatDlg::OnTmMatTypebtn() 
{
	CCMTimeDefMatTypeDlg Dlg;
	Dlg.DoModal();
}

void CHydrTimeDepMatDlg::OnTmElasticityTypebtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_HYDR_TIME_DEP_MATELAST_TYPE,0));
}


void CHydrTimeDepMatDlg::OnTmHmatAddbtn() 
{
	// TODO: Add your control notification handler code here
	T_TDMT_K TdmtK;
	T_TDME_K TdmeK;
	T_TMAT_D TMatD;
	
	m_CobxMatType.GetSelectedTdmt(TdmtK);
	m_CobxTdme.GetSelectedTdme(TdmeK);

	CArray<T_MATL_K, T_MATL_K> MatlKeys;
	CArray<DWORD, DWORD> arRet;

	int nItem = m_rlist.GetItemCount();
	for( int i = 0 ; i < nItem ;i ++)
	{
		MatlKeys.Add((T_MATL_K)m_rlist.GetItemData(i));
	}
		
	CArray<T_TMAT_K, T_TMAT_K > TMatKeys ;
	CArray<T_TMAT_D, T_TMAT_D&> TMatDatas;

	int nMat = MatlKeys.GetSize();
	for(int i = 0; i < nMat ; i++)
	{
		TMatKeys.Add(MatlKeys[i]);
		TMatD.TdMatlTypeKey  = TdmtK;
		TMatD.TdMatlElastKey = TdmeK;
		TMatDatas.Add(TMatD);
	}

	//TMatD.TdMatlElastKey;

	if(m_pDoc->m_pDataCtrl->AddTmat(TMatKeys,TMatDatas))
	{
		m_rlist.DeleteAllItems();
	}
}

void CHydrTimeDepMatDlg::OnTmHmatDelbtn() 
{
	CArray<DWORD,DWORD> rData;
	CArray<T_TMAT_K, T_TMAT_K> TmatKeys;
	CDlgUtil::GetSelectedListItemData(&m_wndTDefMat,rData);

	int nData = rData.GetSize();
	if(nData)
	{
		for(int i = 0; i < nData; i++)
		{
			TmatKeys.Add((T_TMAT_K)rData[i]);
		}
		m_pDoc->m_pDataCtrl->DelTmat(TmatKeys);
	}
	else
	{

	}
}

void CHydrTimeDepMatDlg::OnTmHmatLmovebtn() 
{
	CArray<DWORD, DWORD> SItem;
	CArray<T_MATL_K, T_MATL_K> SMatlKeys;
	CDlgUtil::GetSelectedListItemData(&m_rlist,SItem);

	int nItem = SItem.GetSize();

	for(int i = 0; i < nItem ;i++)
	{
		SMatlKeys.Add((T_MATL_K)SItem[i]);
	}

	RemoveFromRList(SMatlKeys);
}

void CHydrTimeDepMatDlg::OnTmHmatRmovebtn() 
{
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


void CHydrTimeDepMatDlg::OnDblclkTmSourceMatTypelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnTmHmatRmovebtn();
	
	*pResult = 0;
}

void CHydrTimeDepMatDlg::OnDblclkTmSelectedMatTypelist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnTmHmatLmovebtn();
	
	*pResult = 0;
}

void CHydrTimeDepMatDlg::OnItemchangedTmTdefMatlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//m_wndTDefMat.Get  
	CArray<int,int>arRet;
	CDlgUtil::GetSelectedListItemIndex(&m_wndTDefMat,arRet);
	if(arRet.GetSize() != 1) return;

	T_TMAT_K TmatK = (T_TMAT_K) m_wndTDefMat.GetItemData(arRet[0]);
	T_TMAT_D TmatD;
	if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetTmat(TmatK,TmatD))
	{
		CArray<T_MATL_K, T_MATL_K> AddMAT;
		
		this->m_CobxMatType.ChangeSelect(TmatD.TdMatlTypeKey);
		this->m_CobxTdme.ChangeSelect(TmatD.TdMatlElastKey);
		m_rlist.DeleteAllItems();
		AddMAT.Add(TmatK);
		this->AddToRList(AddMAT);
	}
/*
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
	*/
}

void CHydrTimeDepMatDlg::Execute()
{
	OnTmHmatAddbtn();
}