// EtfcListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "EtfcListDlg.h"
#include "EtfcItemDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEtfcListDlg dialog


CEtfcListDlg::CEtfcListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CEtfcListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEtfcListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEtfcListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEtfcListDlg)
	DDX_Control(pDX, IDC_ETFC_LIST, m_List);
	//}}AFX_DATA_MAP
}

void CEtfcListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
//    UnitChanged();
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

void CEtfcListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_ETFC_K Key, KeyBak;
	T_ETFC_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_ETFC_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetEtfc(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_ETFC_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetEtfc(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_ETFC_MFD):
				// 여기서는 저장만 하고 UR_ETFC_MFS에서 처리한다.
				pViewBuff->GetEtfc(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_ETFC_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetEtfc(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

BEGIN_MESSAGE_MAP(CEtfcListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CEtfcListDlg)
	ON_BN_CLICKED(IDC_ETFC_BTN_ADD, OnEtfcBtnAdd)
	ON_BN_CLICKED(IDC_ETFC_BTN_MODIFY, OnEtfcBtnModify)
	ON_BN_CLICKED(IDC_ETFC_BTN_DELETE, OnEtfcBtnDelete)
	ON_BN_CLICKED(IDC_ETFC_BTN_CLOSE, OnEtfcBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_ETFC_LIST, OnDblclkEtfcList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define COLCOUNT 2
/////////////////////////////////////////////////////////////////////////////
// CEtfcListDlg message handlers

BOOL CEtfcListDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	SetHeaderTitle(TRUE);
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEtfcListDlg::SetHeaderTitle(BOOL bInit)
{
	CString aTitle[] = { _LS(IDS_CMD_ETFC_name), _LS(IDS_CMD_ETFC_type) };
//18japan  CString aTitle[] = {_LS(IDS_CMD0417__Function_Name), _T("Function Type")};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	if (bInit)
	{
		DWORD dwStyle;
		dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	  dwStyle |= LVS_EX_FULLROWSELECT; 
	  ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	}

	// Calculate width for each column
	nColWidth[0] = 100;
	nColWidth[1] = 100;

	// Set Title
	for(i=0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		// 제일 처음 시작일 때면 Column 추가
		// 아니면 Column Title만 변경
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		if (bInit)
			m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CEtfcListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountEtfc();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_ETFC_K Key;
		T_ETFC_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartEtfc();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextEtfc(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_ETFC_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetEtfc(Key, Data);
			for(int i = 0; i < COLCOUNT; i++)
			{
				lvitem.iItem=nCount;
				lvitem.iSubItem=i;
				str = DataToStr(i, Key, Data);
				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT ;
			  if( i == 0 )
				{
					lvitem.mask |= LVIF_PARAM;
					lvitem.lParam = (LPARAM)Key;
				  m_List.InsertItem(&lvitem);
				}
			  else
				  m_List.SetItem(&lvitem);
			  str.ReleaseBuffer();
			}
		}
		delete []KeyBuf;
	}
}

BOOL CEtfcListDlg::InsertItem(T_ETFC_K Key, T_ETFC_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iItem = nItem;
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

CString CEtfcListDlg::DataToStr(int i, T_ETFC_K Key, T_ETFC_D &Data)
{
	CString str;
	CString csFuncType[] = { _LS(IDS_CMD_ETFC_constant), _LS(IDS_CMD_ETFC_sine_function), _LS(IDS_CMD_ETFC_user) };
//18japan	CString csFuncType[] = {_T("Constant"), _T("Sine Function"), _LS(IDS_WG_CMD__ADD2__User)};

	ASSERT(Data.nFuncType >= 1 && Data.nFuncType <= 3);

	if(i==0) str = Data.strFuncName;
	else if(i==1) str = csFuncType[Data.nFuncType-1];
	else str = _LS(IDS_CMD_ETFC_error);

	return str;
}

BOOL CEtfcListDlg::DeleteItem(T_ETFC_K Key, T_ETFC_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CEtfcListDlg::ModifyItem(T_ETFC_K KeyOld, T_ETFC_K Key, T_ETFC_D &Data)
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

void CEtfcListDlg::OnEtfcBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CEtfcItemDlg dlg;
	dlg.DoModal();
}

void CEtfcListDlg::OnEtfcBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_ETFC_K Key;
	T_ETFC_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_Environmental_Tempera));
		return;
	}
 
	Key = (T_ETFC_K)m_List.GetItemData(iItem);
	if(!m_pDoc->m_pAttrCtrl->GetEtfc(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Environmental_Temperature_Functio), Key);
		AfxMessageBox(msg);
		return;
	}

	CEtfcItemDlg dlg;
	dlg.SetParamData(Data);
//  dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CEtfcListDlg::OnEtfcBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_Environmental_Tempera));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelEtfc(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CEtfcListDlg::OnEtfcBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();	
}

void CEtfcListDlg::OnDblclkEtfcList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnEtfcBtnModify();
	*pResult = 0;
}
