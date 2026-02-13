// DgnSeisBearingPropListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisBearingPropListDlg.h"

#include "DgnSeisBearingPropItemDlg.h"

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
// CDgnSeisBearingPropListDlg dialog


CDgnSeisBearingPropListDlg::CDgnSeisBearingPropListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisBearingPropListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisBearingPropListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnSeisBearingPropListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisBearingPropListDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_LST, m_List);
	//}}AFX_DATA_MAP
}

void CDgnSeisBearingPropListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnSeisBearingPropListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_BRPR_K Key,  KeyBack;
	T_BRPR_D Data, DataBack;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_BRPR_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetBrpr(nKey, Key, Data);
			InsertItem(Key, Data);
			break;
		case(UR_BRPR_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetBrpr(nKey, Key, Data);
			DeleteItem(Key, Data);
			break;
		case(UR_BRPR_MFD):
			// 여기서는 저장만 하고 MFS에서 처리한다.
			pViewBuff->GetBrpr(nKey, Key, Data);
			KeyBack  = Key;
			DataBack = Data;
			bMFD = TRUE;
			break;
		case(UR_BRPR_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetBrpr(nKey, Key, Data);
			ModifyItem(Key, KeyBack, DataBack);
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBearingPropListDlg implementation functions
void CDgnSeisBearingPropListDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_DGN_SEIS_BEARING_PROP_NAME), _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL), _LS(IDS_DGN_SEIS_BEARING_PROP_ANCHOR) };
	int nColWidth[] = {90, 100, 172};

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

void CDgnSeisBearingPropListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountBrpr();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_BRPR_K Key;
		T_BRPR_D Data;

		int* KeyBuf=new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl2->GetStartBrpr();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl2->GetNextBrpr(pos, Key, Data);
			KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl2->GetBrpr(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

int CDgnSeisBearingPropListDlg::FindInsertionPos(int nID)
{
	int nCount = m_List.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
        T_BRPR_K key = m_List.GetItemData(i);
		if (nID < key) { return i;}
	}
	return nCount;
}

BOOL CDgnSeisBearingPropListDlg::InsertItem(T_BRPR_K Key, T_BRPR_D &Data)
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

BOOL CDgnSeisBearingPropListDlg::DeleteItem(T_BRPR_K Key, T_BRPR_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CDgnSeisBearingPropListDlg::ModifyItem(T_BRPR_K KeyOld, T_BRPR_K Key, T_BRPR_D &Data)
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

CString CDgnSeisBearingPropListDlg::DataToStr(int i, T_BRPR_K Key, T_BRPR_D &Data)
{
	CString str;

	if(i==0)
	{
		str = Data.strName;
	}
	else if(i==1)
	{
		if     (Data.nInstall == 0) str = _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST);
		else if(Data.nInstall == 1) str = _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST);
	}
	else if(i==2)
	{
		if(Data.nInstall == 0)
		{
			if     (Data.nAnchor == 0) str = _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_1);
			else if(Data.nAnchor == 1) str = _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_2);
			else if(Data.nAnchor == 2) str = _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_3);
			else if(Data.nAnchor == 3) str = _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_4);
		}
		else if(Data.nInstall == 1)
		{
			if     (Data.nAnchor == 0) str = _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_1);
			else if(Data.nAnchor == 1) str = _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_2);
			else if(Data.nAnchor == 2) str = _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_3);
			else if(Data.nAnchor == 3) str = _LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_4);
		}
	}
 	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

BOOL CDgnSeisBearingPropListDlg::CheckData(T_BRPR_K Key)
{
	CArray<T_ANEV_K, T_ANEV_K> rKeyList; rKeyList.RemoveAll();

	m_pDoc->m_pAttrCtrl2->GetAnevKeyList(rKeyList);

	T_ANEV_D rData;
	CArray<T_ELEM_K, T_ELEM_K> aBrprK;
	for(int m=0; m< rKeyList.GetSize(); m++)
	{
		m_pDoc->m_pAttrCtrl2->GetAnev(rKeyList[m], rData);

		for (int i = 0; i < rData.BearingProp.aBrprKs.GetSize(); i++)
		{
			for (int j = 0; j < rData.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
			{
				if (Key == rData.BearingProp.aBrprKs[i].aBrprK[j]) return FALSE;
			}
		}
	}

	return TRUE; 

}


BEGIN_MESSAGE_MAP(CDgnSeisBearingPropListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisBearingPropListDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_ADD_BTN,    OnSeisBearingPropAddBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_MOD_BTN,    OnSeisBearingPropModBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_DEL_BTN,    OnSeisBearingPropDelBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_COPY_BTN,   OnSeisBearingPropCopyBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_CLOSE_BTN,  OnSeisBearingPropCloseBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_DGN_SEIS_BEARING_PROP_LST, OnSeisBearingPropDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBearingPropListDlg message handlers

BOOL CDgnSeisBearingPropListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	SetHeaderTitle();
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisBearingPropListDlg::OnSeisBearingPropAddBtn() 
{
 	CDgnSeisBearingPropItemDlg dlg;
 	dlg.DoModal();
}

void CDgnSeisBearingPropListDlg::OnSeisBearingPropModBtn() 
{
	T_BRPR_K Key;
	T_BRPR_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_BEARING_PROP_NO_SEL));
		return;
	}

	Key = (T_BRPR_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetBrpr(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_DGN_SEIS_BEARING_PROP_NOT_EXIST), Data.strName);
		AfxMessageBox(msg);
		return;
	}
	
	CDgnSeisBearingPropItemDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
	
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CDgnSeisBearingPropListDlg::OnSeisBearingPropDelBtn() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_BEARING_PROP_NO_SEL));
		return;
	}
	T_BRPR_K Key;
	Key = (T_BRPR_K)m_List.GetItemData(iItem);
	
	if (!CheckData(Key))
	{
		T_BRPR_D Data;
		m_pDoc->m_pAttrCtrl2->GetBrpr(Key, Data);

		CString msg;
		msg.Format(_LS(IDS_DGN_SEIS_BEARING_PROP_USED), Data.strName);  //_T("Error : Bearing Property(Name %s) is Used in Anchor Evaluation.")
		AfxMessageBox(msg);
		return;
	}

	if (!m_pDoc->m_pDataCtrl->DelBrpr(Key)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CDgnSeisBearingPropListDlg::OnSeisBearingPropCopyBtn() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_BEARING_PROP_NO_SEL));
		return;
	}
	T_BRPR_K Key;
	Key = (T_BRPR_K)m_List.GetItemData(iItem);

	if(!m_pDoc->m_pDataCtrl->CopyBrpr(Key)) return;
}

void CDgnSeisBearingPropListDlg::OnSeisBearingPropCloseBtn() 
{
	CDialogMove::OnCancel();
}

void CDgnSeisBearingPropListDlg::OnSeisBearingPropDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	T_BRPR_K Key;
	T_BRPR_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	Key = (T_BRPR_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetBrpr(Key, Data)) return;
	
	CDgnSeisBearingPropItemDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
	
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);

	*pResult = 0;
}
