// ThisListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ThisListDlg.h"
#include "EigvDlg.h"
#include "ExportFunc.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_main\wg_mainRes2.h"


#include "ThisItemDlg.h"
#include "ThisItemDlg_MEC.h"
#include "EigvDlg_MEC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CThisListDlg dialog


CThisListDlg::CThisListDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CThisListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThisListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CThisListDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CThisListDlg)
	DDX_Control(pDX, IDC_CMD_TH_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 3
/////////////////////////////////////////////////////////////////////////////
// CThisListDlg implementation functions
void CThisListDlg::SetHeaderTitle()
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

void CThisListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	CArray<T_THIS_K, T_THIS_K> aThisKeyList;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisKeyList, FALSE);
	int nItemCount = aThisKeyList.GetSize();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;  
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_THIS_K Key;
		T_THIS_D Data;

		for(nCount = 0 ; nCount < nItemCount; nCount++)
		{
			Key = aThisKeyList.GetAt(nCount);
			m_pDoc->m_pAttrCtrl->GetThis(Key,Data);
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

void CThisListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CThisListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_THIS_K Key, KeyBak;
	T_THIS_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_THIS_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetThis(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_THIS_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetThis(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_THIS_MFD):
				// 여기서는 저장만 하고 UR_THIS_MFS에서 처리한다.
				pViewBuff->GetThis(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_THIS_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetThis(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

BOOL CThisListDlg::InsertItem(T_THIS_K Key, T_THIS_D &Data)
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

BOOL CThisListDlg::DeleteItem(T_THIS_K Key, T_THIS_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CThisListDlg::ModifyItem(T_THIS_K KeyOld, T_THIS_K Key, T_THIS_D &Data)
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

CString CThisListDlg::DataToStr(int i, T_THIS_K Key, T_THIS_D &Data)
{
	CString str;
	CString strAnalTypeMethod = _T("");
	CString strLinear         = _LS(IDS_WG_CMD__ADDD__Linear);
	CString strNonlinear      = _LS(IDS_CMD_NONLINEAR);
	CString strModal          = _LS(IDS_CMD_MODAL);
	CString strDirectInt      = _LS(IDS_CMD_DIRECT_INTEGRATION);
	CString strStatic         = _LS(IDS_CMD_STATIC);
	CString strInitCondition  = _LS(IDS_WG_CMD__ADDD__None);

	if(Data.nAnalType == 1)        
		strAnalTypeMethod = strLinear;
	else if(Data.nAnalType == 2)   
		strAnalTypeMethod = strNonlinear;
	else 
		ASSERT(FALSE); 

	if(Data.nAnalMethod == 1)      
		strAnalTypeMethod += _T(" ") + strModal;
	else if(Data.nAnalMethod == 2) 
		strAnalTypeMethod += _T(" ") + strDirectInt;
	else if (Data.nAnalMethod == 3) 
		strAnalTypeMethod += _T(" ") + strStatic;
	else 
		ASSERT(FALSE);
	
	// Linear Modal
	// Linear Direct Integration
	// Nonlinear Modal
	// Nonlinear Direct Integration
	
	if(Data.nInitCondition == 1)  // Subsequent to
	{
		if(Data.nSubsequentType == 0)  // Load Case 
		{
			if(Data.nSubLoadCaseType == 0)  // Static Load Case
			{
				T_STLD_D DataStld;
				DataStld.Initialize();
				if(m_pDoc->m_pAttrCtrl->GetStld(Data.KeyFinalState, DataStld))
					strInitCondition = _T("ST : ") + DataStld.LoadCaseName;    
			}
			else if(Data.nSubLoadCaseType == 1)  // Construction Stage Load Case
			{
				CString strConstructionType[] = {_LS(IDS_CMD_THIS_STAG_TYPE1), _LS(IDS_CMD_THIS_STAG_TYPE2)};
				if(Data.KeyFinalState == 1)      strInitCondition = _T("CS : ") + strConstructionType[0];
				else if(Data.KeyFinalState == 2) strInitCondition = _T("CS : ") + strConstructionType[1];      
			}
			else if(Data.nSubLoadCaseType == 2)  // Time History Load Case
			{
				T_THIS_D DataThis;
				DataThis.Initialize();
				if(m_pDoc->m_pAttrCtrl->GetThis(Data.KeyFinalState, DataThis))
					strInitCondition = _T("TH : ") + DataThis.LoadCaseName;    
			}
			else ASSERT(FALSE);
		}
		else if(Data.nSubsequentType == 1)// Equilibrium Element Force
		{
			strInitCondition = _LS(IDS_CMD_THIS_EQUI_MEMB_FORCE);  // Equilibrium Member Force(Table)
		}
		else if(Data.nSubsequentType == 2)
		{
			
		}
		else if (Data.nSubsequentType == 3)
		{
			strInitCondition = _LS(IDS_CMD_THIS_FORCE_FOR_GRID_MODEL);
		}
		else ASSERT(FALSE);
	}

	if(i==0) str = Data.LoadCaseName;
	else if(i==1) str = strAnalTypeMethod;
	else if(i==2) str = strInitCondition;
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}


BEGIN_MESSAGE_MAP(CThisListDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CThisListDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_COPY, OnCmdBtnCopy)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_BN_CLICKED(IDC_CMD_EIGV_CTRL, OnCmdEigvCtrl)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_TH_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThisListDlg message handlers

BOOL CThisListDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	
	// TODO: Add extra initialization here
	SetHeaderTitle();
	MakeItemEx();	

	return TRUE;
}

void CThisListDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
#if defined (_MEC)
	CreateOrActivateDlg(m_pDoc, CThisItemDlg_MEC::IDD);
#else
	CreateOrActivateDlg(m_pDoc, CThisItemDlg::IDD);
#endif
}

void CThisListDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_time_history_));
		return;
	}

	T_THIS_K Key = static_cast<unsigned int>(m_List.GetItemData(iItem));
	if(Key == 0) return;

#if defined (_MEC)
	CreateOrActivateDlg(m_pDoc, CThisItemDlg_MEC::IDD, this, &Key);
#else
	CreateOrActivateDlg(m_pDoc, CThisItemDlg::IDD, this, &Key);
#endif

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CThisListDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_time_history_));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelThis(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CThisListDlg::OnCmdBtnCopy() 
{
	T_THIS_K Key;
	T_THIS_D Data;

	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if ( iItem == -1 )
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_time_history_));
		return;
	}

	Key = (T_THIS_K)m_List.GetItemData(iItem);
	if ( !m_pDoc->m_pAttrCtrl->GetThis(Key, Data) )
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Time_history_load_case_ke), Key);
		AfxMessageBox(msg);
		return;
	}

	CString strListName(Data.LoadCaseName); // get current list name
	CString strNewListName = GetNextListName(strListName);

 // add new list data
	Data.LoadCaseName = strNewListName;
	m_pDoc->m_pDataCtrl->AddThis(Data);
	int nCount = m_List.GetItemCount() - 1;
	// set focus on the new item
	m_List.SetItemState(nCount, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	m_List.EnsureVisible(nCount, TRUE); // Scroll down to the bottom
	m_List.EnsureVisible(nCount + 1, TRUE);// scroll back up just enough to show said item on top

	return;
}

void CThisListDlg::OnCmdBtnClose()
{
	// TODO: Add your control notification handler code here
	CCMDlgBase::OnOK();
}

void CThisListDlg::OnCmdEigvCtrl() 
{
	// TODO: Add your control notification handler code here
//	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_EIGENVALUE, 0));	
#if defined(_MEC)
	CEigvDlg_MEC dlg;
#else
	CEigvDlg dlg;
#endif
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
}

void CThisListDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	T_THIS_K Key = static_cast<unsigned int>(m_List.GetItemData(iItem));
	if(Key == 0) return;

#if defined (_MEC)
	CreateOrActivateDlg(m_pDoc, CThisItemDlg_MEC::IDD, this, &Key);
#else
	CreateOrActivateDlg(m_pDoc, CThisItemDlg::IDD, this, &Key);
#endif

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}

CString CThisListDlg::GetNextListName(const CString& listName)
{
	CString result = listName;

	// check if the listName ends with "_number"
	int lastUnderscoreIndex = result.ReverseFind('_');
	int lastDigitIndex = result.GetLength() - 1;
	while ( lastDigitIndex >= 0 && isdigit(result[lastDigitIndex]) )
	{
		--lastDigitIndex;
	}

	if ( lastUnderscoreIndex != -1 && lastDigitIndex == lastUnderscoreIndex )
	{
		CString numberStr = result.Mid(lastUnderscoreIndex + 1);
		int number = _ttoi(numberStr);

		result = result.Left(lastUnderscoreIndex + 1);
		result.AppendFormat(_T("%d"), number + 1);
	}
	else
	{
		result.AppendFormat(_T("_1"));
	}

	return result;
}