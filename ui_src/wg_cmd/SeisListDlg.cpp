// SeisListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisListDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

//#include "SeisItemDlg2.h"
#include "SeisItemDlgNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisListDlg dialog


CSeisListDlg::CSeisListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSeisListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisListDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 3
/////////////////////////////////////////////////////////////////////////////
// CSeisListDlg implementation functions
void CSeisListDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Load_Case), _LS(IDS_WG_CMD__ADD2__Code_Name), _LS(IDS_WG_CMD__ADDD__Description)};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 100; nColWidth[1] = 100; nColWidth[2] = 106;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
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

void CSeisListDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSeis();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SEIS_K Key;
		T_SEIS_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartSeis();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextSeis(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetSeis(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

void CSeisListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SEIS_K Key;
	T_SEIS_D Data;
	BOOL bStldChanged = FALSE;
 
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_SEIS_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetSeis(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_SEIS_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetSeis(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_STLD_MFD):
		case(UR_STLD_MFS):
			bStldChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while
	if (bStldChanged) MakeItemEx();
}

int CSeisListDlg::FindInsertionPos(T_SEIS_K Key)
{
	T_STLD_D stld;
	m_pDoc->m_pAttrCtrl->GetStld(Key, stld);
	int nID = stld.LoadCaseId;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(m_List.GetItemData(i), stld);
		if (stld.LoadCaseId > nID) break;
	}
	return i;
}

BOOL CSeisListDlg::InsertItem(T_SEIS_K Key, T_SEIS_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i = 0; i < COLCOUNT; i++)
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

BOOL CSeisListDlg::DeleteItem(T_SEIS_K Key, T_SEIS_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CSeisListDlg::ModifyItem(T_SEIS_K KeyOld, T_SEIS_K Key, T_SEIS_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
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

// [SeisCode] 추가시 수정
CString CSeisListDlg::DataToStr(int i, T_SEIS_K Key, T_SEIS_D &Data)
{
	CString str;

	ASSERT(Data.nCodeType >= KS_E_1992 && Data.nCodeType <= SEIS_CODE_END);		// modified by KYM

	if(i==0)
	{
		T_STLD_D stld;
		m_pDoc->m_pAttrCtrl->GetStld(Key, stld);
		str = stld.LoadCaseName;
	}
	else if(i==1) str = CDBLib::m_aDescSLCodeName[Data.nCodeType-1];
	else if(i==2) str = Data.Description;
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}


BEGIN_MESSAGE_MAP(CSeisListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisListDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisListDlg message handlers

BOOL CSeisListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	SetHeaderTitle();
	MakeItemEx();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisListDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CSeisItemDlgNew dlg;

	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CSeisListDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_SEIS_K Key;
	T_SEIS_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_seismic_load_));
		return;
	}
 
	Key = (T_SEIS_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetSeis(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Seismic_load_code_key__d_), Key);
		AfxMessageBox(msg);
		return;
	}

	CSeisItemDlgNew dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CSeisListDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_seismic_load_));
		return;
	}
	T_SEIS_K Key = (T_SEIS_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pDataCtrl->DelSeis(Key)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CSeisListDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_SEIS_K Key;
	T_SEIS_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = (T_SEIS_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetSeis(Key, Data)) return;

	CSeisItemDlgNew dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}

void CSeisListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CSeisListDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}
