// IethListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "IethListDlg.h"


#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_main\wg_mainRes2.h"


#include "IethItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIethListDlg dialog


CIethListDlg::CIethListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CIethListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIethListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIethListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIethListDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 3

/////////////////////////////////////////////////////////////////////////////
// CIethListDlg implementation functions
void CIethListDlg::SetHeaderTitle()
{
	CString strAnalType = _LS(IDS_WG_CMD__ADDD__Analysis_Type);
	CString strMethod = _LS(IDS_WG_CMD__ADDD__Method);
	CString strAnalTypeMethod = strAnalType + _T(" & ") + strMethod;  
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__Load_Case), strAnalTypeMethod, _LS(IDS_CMD_INIT_CONDITION)};
	int nColWidth[COLCOUNT] = {100, 167, 100};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

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
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CIethListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountIeth();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;  
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_IETH_K Key;
		T_IETH_D Data;

	  nCount = 0;
		CArray<T_IETH_K, T_IETH_K> aIethKeyList;
		aIethKeyList.RemoveAll();
		m_pDoc->m_pAttrCtrl->GetIethKeyList(aIethKeyList);
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
			Key = aIethKeyList.GetAt(nCount);
			m_pDoc->m_pAttrCtrl->GetIeth(Key,Data);
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
	}
}

void CIethListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		break;
	}
}

void CIethListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_IETH_K Key, KeyBak;
	T_IETH_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_IETH_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetIeth(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_IETH_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetIeth(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_IETH_MFD):
				// 여기서는 저장만 하고 UR_IETH_MFS에서 처리한다.
				pViewBuff->GetIeth(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_IETH_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetIeth(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

BOOL CIethListDlg::InsertItem(T_IETH_K Key, T_IETH_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iItem = nItem;
		lvitem.iSubItem = i;
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

BOOL CIethListDlg::DeleteItem(T_IETH_K Key, T_IETH_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CIethListDlg::ModifyItem(T_IETH_K KeyOld, T_IETH_K Key, T_IETH_D &Data)
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

CString CIethListDlg::DataToStr(int i, T_IETH_K Key, T_IETH_D &Data)
{
	CString str;
	CString strAnalTypeMethod = _T("");
	CString strLinear         = _LS(IDS_WG_CMD__ADDD__Linear);
	CString strNonlinear      = _LS(IDS_CMD_NONLINEAR);
	CString strModal          = _LS(IDS_CMD_MODAL);
	CString strDirectInt      = _LS(IDS_CMD_DIRECT_INTEGRATION);
	CString strInitCondition  = _LS(IDS_WG_CMD__ADDD__None);

	if(Data.nAnalType == 1)        strAnalTypeMethod = strLinear;
	else if(Data.nAnalType == 2)   strAnalTypeMethod = strNonlinear;
	else ASSERT(FALSE); 

	if(Data.nAnalMethod == 1)      strAnalTypeMethod += _T(" ") + strModal;
	else if(Data.nAnalMethod == 2) strAnalTypeMethod += _T(" ") + strDirectInt;
	else ASSERT(FALSE);
	
	// Linear Modal
	// Linear Direct Integration
	// Nonlinear Modal
	// Nonlinear Direct Integration

	if(Data.nInitCondition == 2 || Data.nInitCondition == 3)
	{
		T_IETH_D DataFinalState;
		DataFinalState.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetIeth(Data.KeyFinalState, DataFinalState))
			strInitCondition = DataFinalState.LoadCaseName;    
	}

	if(i==0) str = Data.LoadCaseName;
	else if(i==1) str = strAnalTypeMethod;
	else if(i==2) str = strInitCondition;
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

BEGIN_MESSAGE_MAP(CIethListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIethListDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)	
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIethListDlg message handlers

BOOL CIethListDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetHeaderTitle();
	MakeItemEx();	

	return TRUE;
}

void CIethListDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CIethItemDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();		
}

void CIethListDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_IETH_K Key;
	T_IETH_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_IETH));  // _T("Error : No selected Inelastic Time History Load Case!\nChoose one and try again.")
		return;
	}
 
	Key = (T_IETH_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetIeth(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_IETH_KEY_NOT_EXIST), Key);  // _T("Error : Inelastic Time History Load Case key %d does not exits!")
		AfxMessageBox(msg);
		return;
	}

	CIethItemDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CIethListDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_IETH));  // _T("Error : No selected Inelastic Time History Load Case!\nChoose one and try again.")
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelIeth(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CIethListDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CIethListDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_IETH_K Key;
	T_IETH_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = (T_IETH_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetIeth(Key, Data)) return;

	CIethItemDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}
