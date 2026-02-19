// DgnSeisMPhiListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisMPhiListDlg.h"

#include "DgnSeisMPhiItemDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
//#include "DgnSeisMPhiCurveDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiListDlg dialog


CDgnSeisMPhiListDlg::CDgnSeisMPhiListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisMPhiListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisMPhiListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnSeisMPhiListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisMPhiListDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_LST, m_List);
	//}}AFX_DATA_MAP
}

void CDgnSeisMPhiListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnSeisMPhiListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MPCC_K Key;
	T_MPCC_D Data;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_MPCC_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetMpcc(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_MPCC_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetMpcc(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		default:
			break;
		}
	} // end of while
}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiListDlg implementation functions
void CDgnSeisMPhiListDlg::SetHeaderTitle()
{
	CString aTitle[] = { _LS(IDS_DGN_SEIS_MPCC_NAME), _LS(IDS_DGN_SEIS_MPCC_EVAL_GROUP), _LS(IDS_DGN_SEIS_MPCC_EVAL_GROUP_COLUMN), _LS(IDS_DGN_SEIS_MPCC_EVAL_POS), _LS(IDS_DGN_SEIS_MPCC_SECT), _LS(IDS_DGN_SEIS_MPCC_SECT_POS) };
	int nColWidth[] = {80, 80, 80, 60, 80, 60};
	
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < 6; i++)
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

void CDgnSeisMPhiListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountMpcc();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_MPCC_K Key;
		T_MPCC_D Data;

		int* KeyBuf=new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl2->GetStartMpcc();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl2->GetNextMpcc(pos, Key, Data);
			KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl2->GetMpcc(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

int CDgnSeisMPhiListDlg::FindInsertionPos(int nID)
{
	int nCount = m_List.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
        T_MPCC_K key = m_List.GetItemData(i);
		if (nID < key) { return i; }
	}
	return nCount;    
}

BOOL CDgnSeisMPhiListDlg::InsertItem(T_MPCC_K Key, T_MPCC_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i = 0; i < 6; i++)
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

BOOL CDgnSeisMPhiListDlg::DeleteItem(T_MPCC_K Key, T_MPCC_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CDgnSeisMPhiListDlg::ModifyItem(T_MPCC_K KeyOld, T_MPCC_K Key, T_MPCC_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < 6; i++)
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

CString CDgnSeisMPhiListDlg::DataToStr(int i, T_MPCC_K Key, T_MPCC_D &Data)
{
	CString str;
		
	if (i==0) 
	{ 
		str = Data.strName;
	}
	else if(i==1)
	{
		if(Data.bEvgp)
		{
			T_EVGP_D EvgpD; EvgpD.Initialize();
			if(!m_pDoc->m_pAttrCtrl2->GetEvgp(Data.EvgpK, EvgpD)) EvgpD.Initialize();
			str = EvgpD.strName;
		}
		else
		{
			str = _T("-");
		}
	}
	else if(i==2)
	{
		if(Data.bEvgp)
		{
			T_EVGP_D EvgpD; EvgpD.Initialize();
			if(!m_pDoc->m_pAttrCtrl2->GetEvgp(Data.EvgpK, EvgpD)) EvgpD.Initialize();
			str = EvgpD.aEvgpColm[Data.nEvgpColm].strName;
		}
		else
		{
			str = _T("-");
		}
	}
	else if(i==3)
	{
		if(Data.bEvgp)
		{
			if     (Data.nEvgpPos == 0) str = _LS(IDS_DGN_SEIS_MPCC_TOP);
			else if(Data.nEvgpPos == 1) str = _LS(IDS_DGN_SEIS_MPCC_BOT);
		}
		else
		{
			str = _T("-");
		}		
	}
	else if (i==4)
	{
		T_SECT_D SectD; SectD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetSect(Data.SectK, SectD)) SectD.Initialize();
		str = SectD.SName;
	}
	else if(i==5)
	{
		if     (Data.nPosition == 0) str = _LS(IDS_DGN_SEIS_MPCC_I);
		else if(Data.nPosition == 1) str = _LS(IDS_DGN_SEIS_MPCC_M);
		else if(Data.nPosition == 2) str = _LS(IDS_DGN_SEIS_MPCC_J);
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

BEGIN_MESSAGE_MAP(CDgnSeisMPhiListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisMPhiListDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_ADD_BTN,   OnDgnAddBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_MOD_BTN,   OnDgnModBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_DEL_BTN,   OnDgnDelBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CLOSE_BTN, OnDgnCloseBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_DGN_SEIS_MPHI_LST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiListDlg message handlers

BOOL CDgnSeisMPhiListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
 
	SetHeaderTitle();
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisMPhiListDlg::OnDgnAddBtn() 
{
	CDgnSeisMPhiItemDlg dlg;
	dlg.DoModal();
}

void CDgnSeisMPhiListDlg::OnDgnModBtn() 
{
	// TODO: Add your control notification handler code here
	T_MPCC_K Key;
	T_MPCC_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_MPCC_ERROR_NO_SEL));
		return;
	}

	Key = (T_MPCC_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetMpcc(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_DGN_SEIS_MPCC_ERROR_NOT_EXITS), Data.strName);
		AfxMessageBox(msg);
		return;
	}
	/*
	CDgnSeisMPhiCurveDlg dlg;
	dlg.SetParamData(Data);
	dlg.MakeBmpAndClose(_T("c:\\testbmp.bmp"));
	dlg.DoModal();
	*/
	
	CDgnSeisMPhiItemDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
	
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CDgnSeisMPhiListDlg::OnDgnDelBtn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_DGN_SEIS_MPCC_ERROR_NO_SEL));
		return;
	}
	T_MPCC_K Key;
	Key = (T_MPCC_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pDataCtrl->DelMpcc(Key)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CDgnSeisMPhiListDlg::OnDgnCloseBtn() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

void CDgnSeisMPhiListDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_MPCC_K Key;
	T_MPCC_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	Key = (T_MPCC_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetMpcc(Key, Data)) return;
	
	CDgnSeisMPhiItemDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
	
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);

	*pResult = 0;
}
