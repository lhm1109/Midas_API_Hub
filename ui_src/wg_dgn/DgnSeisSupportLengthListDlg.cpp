// DgnSeisSupportLengthListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisSupportLengthListDlg.h"

#include "DgnSeisSupportLengthItemDlg.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisSupportLengthListDlg dialog


CDgnSeisSupportLengthListDlg::CDgnSeisSupportLengthListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisSupportLengthListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisSupportLengthListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnSeisSupportLengthListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisSupportLengthListDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_SUPPORT_LENGTH_LST, m_List);
	//}}AFX_DATA_MAP
}

void CDgnSeisSupportLengthListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnSeisSupportLengthListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SUPL_K Key,  KeyBack;
	T_SUPL_D Data, DataBack;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SUPL_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetSupl(nKey, Key, Data);
			InsertItem(Key, Data);
			break;
		case(UR_SUPL_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetSupl(nKey, Key, Data);
			DeleteItem(Key, Data);
			break;
		case(UR_SUPL_MFD):
			// 여기서는 저장만 하고 MFS에서 처리한다.
			pViewBuff->GetSupl(nKey, Key, Data);
			KeyBack  = Key;
			DataBack = Data;
			bMFD = TRUE;
			break;
		case(UR_SUPL_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetSupl(nKey, Key, Data);
			ModifyItem(Key, KeyBack, DataBack);
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisSupportLengthListDlg implementation functions
void CDgnSeisSupportLengthListDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_DGN_SEIS_SUPPORT_LENGTH_NAME), _LS(IDS_DGN_SEIS_SUPPORT_LENGTH_ANEV) };
	int nColWidth[] = {90, 272};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < 2; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CDgnSeisSupportLengthListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountSupl();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SUPL_K Key;
		T_SUPL_D Data;

		int* KeyBuf=new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl2->GetStartSupl();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl2->GetNextSupl(pos, Key, Data);
			KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl2->GetSupl(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

int CDgnSeisSupportLengthListDlg::FindInsertionPos(int nID)
{
	T_SUPL_D data;
	int nCount = m_List.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
        T_SUPL_K key = m_List.GetItemData(i);
		if (nID < key) { return i;}
	}
	return nCount;    
}

BOOL CDgnSeisSupportLengthListDlg::InsertItem(T_SUPL_K Key, T_SUPL_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i = 0; i < 2; i++)
	{
		lvitem.iSubItem=i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) 
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)Key;
			nItem = m_List.InsertItem(&lvitem);
		}
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CDgnSeisSupportLengthListDlg::DeleteItem(T_SUPL_K Key, T_SUPL_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CDgnSeisSupportLengthListDlg::ModifyItem(T_SUPL_K KeyOld, T_SUPL_K Key, T_SUPL_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < 2; i++)
		{
			str = DataToStr(i, Key, Data);
			m_List.SetItemText(nItem, i, str);
		}
		// key 재 설정 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_List.SetItem(&lvitem);
	}
	return TRUE;
}

CString CDgnSeisSupportLengthListDlg::DataToStr(int i, T_SUPL_K Key, T_SUPL_D &Data)
{
	CString str;

	if(i==0)
	{
		str = Data.strName;
	}
	else if(i==1)
	{
		T_ANEV_D AnevD; AnevD.Initialize();
		m_pDoc->m_pAttrCtrl2->GetAnev(Data.AnevK, AnevD);
		str = AnevD.strName;
	}
 	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

BEGIN_MESSAGE_MAP(CDgnSeisSupportLengthListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisSupportLengthListDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_SUPPORT_LENGTH_ADD_BTN,    OnSeisSupportLengthAddBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_SUPPORT_LENGTH_MOD_BTN,    OnSeisSupportLengthModBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_SUPPORT_LENGTH_DEL_BTN,    OnSeisSupportLengthDelBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_SUPPORT_LENGTH_COPY_BTN,   OnSeisSupportLengthCopyBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_SUPPORT_LENGTH_CLOSE_BTN,  OnSeisSupportLengthCloseBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_DGN_SEIS_SUPPORT_LENGTH_LST, OnSeisSupportLengthDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisSupportLengthListDlg message handlers

BOOL CDgnSeisSupportLengthListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	SetHeaderTitle();
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisSupportLengthListDlg::OnSeisSupportLengthAddBtn() 
{
 	CDgnSeisSupportLengthItemDlg dlg;
 	dlg.DoModal();
}

void CDgnSeisSupportLengthListDlg::OnSeisSupportLengthModBtn() 
{
	T_SUPL_K Key;
	T_SUPL_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_SUPPORT_LENGTH_NO_SEL));
		return;
	}

	Key = (T_SUPL_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSupl(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_DGN_SEIS_SUPPORT_LENGTH_NOT_EXIST), Data.strName);
		AfxMessageBox(msg);
		return;
	}
	
	CDgnSeisSupportLengthItemDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
	
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CDgnSeisSupportLengthListDlg::OnSeisSupportLengthDelBtn() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_SUPPORT_LENGTH_NO_SEL));
		return;
	}
	T_SUPL_K Key;
	Key = (T_SUPL_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pDataCtrl->DelSupl(Key)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CDgnSeisSupportLengthListDlg::OnSeisSupportLengthCopyBtn() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_SUPPORT_LENGTH_NO_SEL));
		return;
	}
	T_SUPL_K Key;
	Key = (T_SUPL_K)m_List.GetItemData(iItem);

	if(!m_pDoc->m_pDataCtrl->CopySupl(Key)) return;
}

void CDgnSeisSupportLengthListDlg::OnSeisSupportLengthCloseBtn() 
{
	CDialogMove::OnCancel();
}

void CDgnSeisSupportLengthListDlg::OnSeisSupportLengthDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	T_SUPL_K Key;
	T_SUPL_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	Key = (T_SUPL_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSupl(Key, Data)) return;
	
	CDgnSeisSupportLengthItemDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
	
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);

	*pResult = 0;
}
