// DgnSeisAnchorEvalListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorEvalListDlg.h"

#include "DgnSeisAnchorEvalItemDlg.h"
#include "DgnSeisAnchorEvalItemNewDlg.h"
#include "DgnSeisAnchorEvalItem2NewDlg.h"

#include "..\wg_base\TestEnvMgr.h"
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
// CDgnSeisAnchorEvalListDlg dialog


CDgnSeisAnchorEvalListDlg::CDgnSeisAnchorEvalListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisAnchorEvalListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisAnchorEvalListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnSeisAnchorEvalListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisAnchorEvalListDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_ANCHOR_EVAL_LST, m_List);
	//}}AFX_DATA_MAP
}

void CDgnSeisAnchorEvalListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnSeisAnchorEvalListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_ANEV_K Key,  KeyBack;
	T_ANEV_D Data, DataBack;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_ANEV_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetAnev(nKey, Key, Data);
			InsertItem(Key, Data);
			break;
		case(UR_ANEV_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetAnev(nKey, Key, Data);
			DeleteItem(Key, Data);
			break;
		case(UR_ANEV_MFD):
			// 여기서는 저장만 하고 MFS에서 처리한다.
			pViewBuff->GetAnev(nKey, Key, Data);
			KeyBack  = Key;
			DataBack = Data;
			bMFD = TRUE;
			break;
		case(UR_ANEV_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetAnev(nKey, Key, Data);
			ModifyItem(Key, KeyBack, DataBack);
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalListDlg implementation functions
void CDgnSeisAnchorEvalListDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_DGN_SEIS_ANCHOR_EVAL_NAME), _LS(IDS_DGN_SEIS_ANCHOR_EVAL_BEARING_EVAL_NAME), _LS(IDS_DGN_LIST_DESCRIPTION) };
	int nColWidth[] = {90, 130, 142};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < 3; i++)
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

void CDgnSeisAnchorEvalListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountAnev();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_ANEV_K Key;
		T_ANEV_D Data;

		int* KeyBuf=new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl2->GetStartAnev();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl2->GetNextAnev(pos, Key, Data);
			KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl2->GetAnev(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

int CDgnSeisAnchorEvalListDlg::FindInsertionPos(int nID)
{
	int nCount = m_List.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
        T_ANEV_K key = m_List.GetItemData(i);
		if (nID < key) { return i;}
	}
	return nCount;    
}

BOOL CDgnSeisAnchorEvalListDlg::InsertItem(T_ANEV_K Key, T_ANEV_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i = 0; i < 3; i++)
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

BOOL CDgnSeisAnchorEvalListDlg::DeleteItem(T_ANEV_K Key, T_ANEV_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CDgnSeisAnchorEvalListDlg::ModifyItem(T_ANEV_K KeyOld, T_ANEV_K Key, T_ANEV_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < 3; i++)
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

CString CDgnSeisAnchorEvalListDlg::DataToStr(int i, T_ANEV_K Key, T_ANEV_D &Data)
{
	CString str;

	if(i==0)
	{
		str = Data.strName;
	}
	else if(i==1)
	{
		T_BREV_D BrevD;
		if(!m_pDoc->m_pAttrCtrl2->GetBrev(Data.BrevK, BrevD))
		{
			ASSERT(0);
			return str;
		}

		str = BrevD.strName;
	}
	else if(i==2)
	{
		str = Data.strDescription;
	}
 	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

BEGIN_MESSAGE_MAP(CDgnSeisAnchorEvalListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisAnchorEvalListDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_ADD_BTN,    OnSeisAnchorEvalAddBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_MOD_BTN,    OnSeisAnchorEvalModBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_DEL_BTN,    OnSeisAnchorEvalDelBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_COPY_BTN,   OnSeisAnchorEvalCopyBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_ANCHOR_EVAL_CLOSE_BTN,  OnSeisAnchorEvalCloseBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_DGN_SEIS_ANCHOR_EVAL_LST, OnSeisAnchorEvalDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalListDlg message handlers

BOOL CDgnSeisAnchorEvalListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	SetHeaderTitle();
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisAnchorEvalListDlg::OnSeisAnchorEvalAddBtn() 
{
 	//CDgnSeisAnchorEvalItemDlg dlg;
//	if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
	{
		CDgnSeisAnchorEvalItem2NewDlg dlg(this, TRUE);
		dlg.SetInitPos(D_INIT_POS_LT);
		dlg.DoModal();
	}
// 	else
// 	{
// 	CDgnSeisAnchorEvalItemNewDlg dlg(this, TRUE);
// 	dlg.SetInitPos(D_INIT_POS_LT);
//  	dlg.DoModal();
// 	}

}

void CDgnSeisAnchorEvalListDlg::OnSeisAnchorEvalModBtn() 
{
	T_ANEV_K Key;
	T_ANEV_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_ANCHOR_EVAL_NO_SEL));
		return;
	}

	Key = (T_ANEV_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetAnev(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_DGN_SEIS_ANCHOR_EVAL_NOT_EXIST), Data.strName);
		AfxMessageBox(msg);
		return;
	}
	
///	if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
	{
		CDgnSeisAnchorEvalItem2NewDlg dlg(this);
		dlg.SetParamData(Key, Data);
		dlg.SetInitPos(D_INIT_POS_LT);
		dlg.DoModal();
	}
// 	else
// 	{
// 	//CDgnSeisAnchorEvalItemDlg dlg;
// 	CDgnSeisAnchorEvalItemNewDlg dlg(this);
// 	dlg.SetParamData(Key, Data);
// 	dlg.SetInitPos(D_INIT_POS_LT);
// 	dlg.DoModal();
// 	}
	
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CDgnSeisAnchorEvalListDlg::OnSeisAnchorEvalDelBtn() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_ANCHOR_EVAL_NO_SEL));
		return;
	}
	T_ANEV_K Key;
	Key = (T_ANEV_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pDataCtrl->DelAnev(Key)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CDgnSeisAnchorEvalListDlg::OnSeisAnchorEvalCopyBtn() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_ANCHOR_EVAL_NO_SEL));
		return;
	}
	T_ANEV_K Key;
	Key = (T_ANEV_K)m_List.GetItemData(iItem);

	if(!m_pDoc->m_pDataCtrl->CopyAnev(Key)) return;
}

void CDgnSeisAnchorEvalListDlg::OnSeisAnchorEvalCloseBtn() 
{
	CDialogMove::OnCancel();
}

void CDgnSeisAnchorEvalListDlg::OnSeisAnchorEvalDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	T_ANEV_K Key;
	T_ANEV_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	Key = (T_ANEV_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetAnev(Key, Data)) return;
	
//	if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
	{
		//CDgnSeisAnchorEvalItemDlg dlg;
		CDgnSeisAnchorEvalItem2NewDlg dlg(this);
		dlg.SetParamData(Key, Data);
		dlg.SetInitPos(D_INIT_POS_LT);
		dlg.DoModal();
	}
// 	else
// 	{
// 	//CDgnSeisAnchorEvalItemDlg dlg;
// 	CDgnSeisAnchorEvalItemNewDlg dlg(this);
// 	dlg.SetParamData(Key, Data);
// 	dlg.SetInitPos(D_INIT_POS_LT);
// 	dlg.DoModal();
// 	}
	
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);

	*pResult = 0;
}
