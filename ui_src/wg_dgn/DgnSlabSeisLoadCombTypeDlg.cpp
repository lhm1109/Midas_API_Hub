// DgnSlabSeisLoadCombTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSlabSeisLoadCombTypeDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSlabSeisLoadCombTypeDlg dialog
CDgnSlabSeisLoadCombTypeDlg::CDgnSlabSeisLoadCombTypeDlg(CWnd* pParent /*=NULL*/)
	: CDgnDlgBase(CDgnSlabSeisLoadCombTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSlabSeisLoadCombTypeDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
	m_pDoc = NULL;
	m_nMembType=0;
	m_nSeisType=0;
}

CDgnSlabSeisLoadCombTypeDlg::~CDgnSlabSeisLoadCombTypeDlg()
{
}

void CDgnSlabSeisLoadCombTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDgnDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSlabSeisLoadCombTypeDlg)
	DDX_Control(pDX, IDC_DGN_SLAB_SEIS_LOAD_COMB_SEL_LIST           , m_List);
	DDX_Radio  (pDX, IDC_DGN_SLAB_SEIS_LOAD_COMB_MEMB_TYPE_SLAB_RDO , m_nMembType);
	DDX_Control(pDX, IDC_DGN_SLAB_SEIS_LOAD_COMB_MEMB_NAME_EDT      , m_edtName );
	DDX_Control(pDX, IDC_DGN_SLAB_SEIS_LOAD_COMB_MEMB_ELEM_EDT      , m_edtElem);
	DDX_Radio  (pDX, IDC_DGN_SLAB_SEIS_LOAD_COMB_TYPE_SPECIAL_RDO   , m_nSeisType);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnSlabSeisLoadCombTypeDlg, CDgnDlgBase)
	//{{AFX_MSG_MAP(CDgnSlabSeisLoadCombTypeDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DGN_SLAB_SEIS_LOAD_COMB_SEL_LIST, OnItemChangedList)
	ON_BN_CLICKED(IDC_DGN_SLAB_SEIS_LOAD_COMB_TYPE_ADD_BTN, OnAddReplace)
	ON_BN_CLICKED(IDC_DGN_SLAB_SEIS_LOAD_COMB_TYPE_DEL_BTN, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDgnSlabSeisLoadCombTypeDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
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
	default:
		break;
	}
}

void CDgnSlabSeisLoadCombTypeDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bChange = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SSLC_ADD):    case(UR_SSLC_DEL):    case(UR_SSLC_MFS):    case(UR_SSLC_MFD):
		case(UR_SSLA_ADD):    case(UR_SSLA_DEL):    case(UR_SSLA_MFS):    case(UR_SSLA_MFD): 
			bChange = TRUE; break;
		default: break;
		}
	}
	if(bChange)
	{
		m_bInit = FALSE;
		MakeItemEx();
		m_bInit = TRUE;
	}
}

int CDgnSlabSeisLoadCombTypeDlg::GetSlabElemList(T_ELEM_K_LIST& raElemK)
{
	INT_PTR nSize = raElemK.GetSize();
	if (nSize < 1) return 0;

	T_ELEM_K_LIST aElemK; aElemK.Copy(raElemK);
	T_ELEM_D ElemD;
	raElemK.RemoveAll();

	for (INT_PTR i=0; i<nSize; ++i)
	{
		T_ELEM_K ElemK = aElemK[i];
		if(m_nMembType==0) // Slab/Mat
		{      
			// 판요소인가?
			if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); continue; }
			if (!m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp)) { continue; }
			raElemK.Add(ElemK);
		}
		else if(m_nMembType==1) // Column Strip
		{
			// Strip이 할당된 요소인가?
			T_MEMB_K MembK = 0;
			if (!m_pDoc->m_pAttrCtrl->GetMembAssigned(ElemK,MembK)) { continue; }
			if (m_pDoc->m_pAttrCtrl2->GetMstrKByMemb(MembK)<1) { continue; }
			raElemK.Add(ElemK);
		}   
	}
	return (int)raElemK.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSlabSeisLoadCombTypeDlg message handlers

BOOL CDgnSlabSeisLoadCombTypeDlg::OnInitDialog() 
{
	CDgnDlgBase::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	InitCtrls();
	Data2Dlg();
	m_bInit = TRUE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDgnSlabSeisLoadCombTypeDlg::DestroyWindow()
{
	return CDgnDlgBase::DestroyWindow();
}
void CDgnSlabSeisLoadCombTypeDlg::OnCancel()
{
	DestroyWindow();
}
void CDgnSlabSeisLoadCombTypeDlg::OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bInit)   OnClkListItem(pNMHDR, pResult);

	*pResult = 0;
}

void CDgnSlabSeisLoadCombTypeDlg::OnAddReplace()
{
	if(!Dlg2Data())   return;

	CStringArray aStrSelectName;
	int nSize = GetSelectedItems(aStrSelectName);

	CArray<T_ELEM_K,T_ELEM_K> aElemK;
	m_pDoc->m_pViewCtrl->GetAllSelectedElem(aElemK);
	
	// PMS 5319-6 : 요소 선택 안 해도 SSLC 추가는 되도록 (Slab Rebars for Checking과 동일하게 동작하도록)
//   if(aStrSelectName.IsEmpty() && aElemK.IsEmpty())
//     return;	

	CString strName;
	m_edtName.GetWindowText(strName);
	if(strName.IsEmpty())
		return;

	T_SSLC_K SslcK = m_pDoc->m_pAttrCtrl2->GetSslcKey(strName);
	T_SSLC_D SslcD;
	SslcD.strName   = strName;
	SslcD.nMembType = m_nMembType;
	SslcD.nSeisType = m_nSeisType;

	if(!m_pDoc->m_pUndoCtrl->StartEditDB(_LS(IDS_DB_DATACTRL_Add_SSLC), CMDTYPE_FREE)) return;
	
	// SSLC
	if(SslcK==0) // Add
	{
		SslcK = m_pDoc->m_pAttrCtrl2->GetStartNumSslc();
		m_pDoc->m_pEditData->AddSslc(SslcK,SslcD);
	}
	else // Replace
	{
		m_pDoc->m_pEditData->ModifySslc(SslcK,SslcD);
	}

	// SSLA
	CArray<T_SSLA_K,T_SSLA_K> aSslaK;
	m_pDoc->m_pAttrCtrl2->GetSslcSslaKeyList(SslcK, aSslaK);
	for(int i=0; i<aSslaK.GetSize(); i++)
	{
		m_pDoc->m_pEditData->DelSsla(aSslaK[i]);
	}

	T_ELEM_D ElemD;
	T_SSLA_D SslaD;
	SslaD.SslcK=SslcK;
	for(int i=0; i<aElemK.GetSize(); i++)
	{
		T_ELEM_K ElemK = aElemK[i];

		if(m_nMembType==0) // Slab/Mat
		{      
			// 판요소인가?
			if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); continue; }
			if(!m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp)) continue;
		}
		else if(m_nMembType==1) // Column Strip
		{
			// Strip이 할당된 요소인가?
			T_MEMB_K MembK;
			if(!m_pDoc->m_pAttrCtrl->GetMembAssigned(ElemK,MembK)) continue;
			if(m_pDoc->m_pAttrCtrl2->GetMstrKByMemb(MembK)<1) continue;
		}   

		m_pDoc->m_pEditData->AddSsla(ElemK, SslaD);
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB();
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CDgnSlabSeisLoadCombTypeDlg::OnDelete()
{
	CStringArray aStrName;
	int nSize = GetSelectedItems(aStrName);
	if(nSize < 1)   return;

	if(!m_pDoc->m_pUndoCtrl->StartEditDB(_LS(IDS_DB_DATACTRL_Add_SSLC), CMDTYPE_FREE)) return;
	for(int i=0; i<aStrName.GetSize(); i++)
	{
		T_SSLC_K SslcK = m_pDoc->m_pAttrCtrl2->GetSslcKey(aStrName[i]);
		m_pDoc->m_pEditData->DelSslc(SslcK);
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB();
	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CDgnSlabSeisLoadCombTypeDlg::Data2Dlg()
{
	MakeItemEx();

	LRESULT Result;
	OnClkListItem(NULL, &Result);
}

BOOL CDgnSlabSeisLoadCombTypeDlg::Dlg2Data()
{
	UpdateData(TRUE);

	return TRUE;
}

void CDgnSlabSeisLoadCombTypeDlg::InitCtrls()
{
	SetListCtrlHeader();
	m_edtElem.Connect(SC_ID_ELEM, &m_edtElem);
}

// List Ctrl 함수들
void CDgnSlabSeisLoadCombTypeDlg::OnClkListItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	UpdateData(TRUE);

	CString strName;
	if(pNMHDR)
	{
		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
		int nIndex = pNMListView->iItem;
		if(nIndex == -1)   return;
		strName = m_List.GetItemText(nIndex, 0);
	}
	else
	{
		GetSelectedItem(strName);
	}

	CStringArray aStrName;
	GetSelectedItems(aStrName);  
	if(aStrName.IsEmpty()) return;

	CString strNameList;
	int  nMembType=0, nSeisType=0;
	BOOL bEnableMemb=TRUE, bEnableSeis=TRUE;

	CArray<T_ELEM_K,T_ELEM_K> aElemK, aElemKAll;
	for(int i=0; i<aStrName.GetSize(); i++)
	{
		T_SSLC_K SslcK = m_pDoc->m_pAttrCtrl2->GetSslcKey(aStrName[i]);
		T_SSLC_D SslcD;
		if(!m_pDoc->m_pAttrCtrl2->GetSslc(SslcK,SslcD)) { ASSERT(FALSE); SslcD.Initialize(); }

		if(i==0)
		{
			nMembType = SslcD.nMembType;
			nSeisType = SslcD.nSeisType;
			strNameList = SslcD.strName;
		}
		else
		{
			if(nMembType!=SslcD.nMembType) bEnableMemb=FALSE;
			if(nSeisType!=SslcD.nSeisType) bEnableSeis=FALSE;
			strNameList += _T(",")+SslcD.strName;
		}

		m_pDoc->m_pAttrCtrl2->GetSslcSslaKeyList(SslcK, aElemK);
		aElemKAll.Append(aElemK);
	}

	GetDlgItem(IDC_DGN_SLAB_SEIS_LOAD_COMB_TYPE_ADD_BTN       )->EnableWindow(aStrName.GetSize()<2);
	GetDlgItem(IDC_DGN_SLAB_SEIS_LOAD_COMB_MEMB_NAME_EDT      )->EnableWindow(aStrName.GetSize()<2);
	GetDlgItem(IDC_DGN_SLAB_SEIS_LOAD_COMB_MEMB_TYPE_SLAB_RDO )->EnableWindow(bEnableMemb);
	GetDlgItem(IDC_DGN_SLAB_SEIS_LOAD_COMB_MEMB_TYPE_STRP_RDO )->EnableWindow(bEnableMemb);  
	GetDlgItem(IDC_DGN_SLAB_SEIS_LOAD_COMB_TYPE_SPECIAL_RDO   )->EnableWindow(bEnableSeis);
	GetDlgItem(IDC_DGN_SLAB_SEIS_LOAD_COMB_TYPE_VERTICAL_RDO  )->EnableWindow(bEnableSeis);
	m_pDoc->m_pViewCtrl->SelectElem(NULL, aElemKAll, FALSE, TRUE);

	//m_List.SetFocus();

	m_edtName.SetWindowText(strNameList);
	m_nMembType = nMembType;
	m_nSeisType = nSeisType;
	UpdateData(FALSE);

	*pResult = 0;
}

void CDgnSlabSeisLoadCombTypeDlg::SetListCtrlHeader()
{
	LV_COLUMN lvcolumn;

	CRect rect;
	GetDlgItem(IDC_DGN_SLAB_SEIS_LOAD_COMB_SEL_LIST)->GetWindowRect(rect);
	int nColWidth = rect.Width();

	// set full row select mode
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.iSubItem = 0;
	lvcolumn.cx = nColWidth-globalUtils.ScaleByDPI(10);
	CString strTitle = _LS(IDS_DGN_NAME);
	lvcolumn.pszText = strTitle.GetBuffer(0);
	m_List.InsertColumn(0,&lvcolumn);
}

BOOL CDgnSlabSeisLoadCombTypeDlg::GetSelectedItem(CString& strName)
{
	int nItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1) return FALSE;

	strName = m_List.GetItemText(nItem, 0);

	return TRUE;
}

int CDgnSlabSeisLoadCombTypeDlg::GetSelectedItems(CStringArray& aStrName)
{
	aStrName.RemoveAll();

	POSITION pos = m_List.GetFirstSelectedItemPosition();

	while (pos)
	{
		int nItem = m_List.GetNextSelectedItem(pos);      

		aStrName.Add(m_List.GetItemText(nItem, 0));
	}

	return aStrName.GetSize();
}

void CDgnSlabSeisLoadCombTypeDlg::MakeItemEx()
{
	m_List.DeleteAllItems();
	CStringArray aStrName;

	CArray<T_SSLC_K,T_SSLC_K> aSslcK;
	m_pDoc->m_pAttrCtrl2->GetSslcKeyList(aSslcK);
	T_SSLC_D SslcD;
	for(int i = 0; i < aSslcK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetSslc(aSslcK[i], SslcD))     {  ASSERT(0);		return ;	}
		aStrName.Add(SslcD.strName);
	}

	int nDataSize = aStrName.GetSize();
	for(int nIndex = 0; nIndex < nDataSize; nIndex++)
	{
		m_List.InsertItem(nIndex, _T(""));

		LVITEM lvitem;

		lvitem.iItem=nIndex;
		lvitem.iSubItem=0;

		lvitem.pszText=aStrName[nIndex].GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;
		m_List.SetItem(&lvitem);
	}

	LRESULT Result;
	OnClkListItem(NULL, &Result);
}
