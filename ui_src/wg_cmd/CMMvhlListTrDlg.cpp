// CMMvhlListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlListTrDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"


#include "CMMvhlItemTr.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlListTrDlg dialog


CCMMvhlListTrDlg::CCMMvhlListTrDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlListTrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlListTrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMMvhlListTrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlListTrDlg)
	DDX_Control(pDX, IDC_CMD_BTN_ADD2, m_btnAdd2);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 2
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlListTrDlg implementation functions
void CCMMvhlListTrDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_MVHL_vehicle_name), _LS(IDS_CMD_MVHL_type)};
//18japan  CString aTitle[] = {_T("Vehicle Name"), _LS(IDS_WG_CMD__ADDD__Type)};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 120; nColWidth[1] = 90;

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

void CCMMvhlListTrDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	CArray<T_MVHLtr_K, T_MVHLtr_K> aMvhltrKey;
	m_pDoc->m_pAttrCtrl->GetMvhltrKeyList(aMvhltrKey);

	int nItemCount = aMvhltrKey.GetSize();
	if(nItemCount == 0) return;

	int nCount;

	if(nItemCount)
	{
		T_MVHLtr_K Key;
		T_MVHLtr_D Data;

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=aMvhltrKey[nCount];
			m_pDoc->m_pAttrCtrl->GetMvhltr(Key,Data);
			InsertItem(Key, Data);
		}
		//GetDlgItem(IDC_CMD_BTN_ADD2)->EnableWindow(FALSE);
	}
}

void CCMMvhlListTrDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MVHLtr_K Key, KeyBak;
	T_MVHLtr_D Data,DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MVHLtr_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetMvhltr(nKey, Key, Data);
				InsertItem(Key, Data);
				//GetDlgItem(IDC_CMD_BTN_ADD2)->EnableWindow(FALSE);
			}
			break;
		case(UR_MVHLtr_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetMvhltr(nKey, Key, Data);
				DeleteItem(Key, Data);
//         CArray<T_MVHLtr_K, T_MVHLtr_K> aMvhltrKey;
//         m_pDoc->m_pAttrCtrl->GetMvhltrKeyList(aMvhltrKey);
//         if(aMvhltrKey.GetSize()==0)
//           GetDlgItem(IDC_CMD_BTN_ADD2)->EnableWindow(TRUE);
			}
			break;
		case(UR_MVHLtr_MFD):
			{
				// 여기서는 저장만 하고 UR_STLD_MFS에서 처리한다.
				pViewBuff->GetMvhltr(nKey, KeyBak, DataBak);
				bMFD = TRUE;
			}
			break;
		case(UR_MVHLtr_MFS):  // Static Load Case의 Name 변경 처리
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			{
				ASSERT(bMFD);
				pViewBuff->GetMvhltr(nKey, Key, Data);
				DeleteItem(Key, Data);
				InsertItem(KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
			}
			break;
		default:
			break;
		}
	} // end of while
}

int CCMMvhlListTrDlg::FindInsertionPos(T_MVHLtr_K Key, T_MVHLtr_D& Data)
{
	CArray<T_MVHLtr_K, T_MVHLtr_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetMvhltrKeyList(aKeyList);
	int nSize = aKeyList.GetSize();
	int nIx = 0;
	int nItemCount = m_List.GetItemCount();
	for (int i = 0; i < nSize; i++)
	{
		if (nIx >= nItemCount) break;
		if (m_List.GetItemData(nIx) == aKeyList[i]) nIx++;
		else if (Key == aKeyList[i]) break;
	}
	
	return nIx;
}

BOOL CCMMvhlListTrDlg::InsertItem(T_MVHLtr_K Key, T_MVHLtr_D &Data)
{
	CString str;
	int nItem = FindInsertionPos(Key, Data);
	m_List.InsertItem(nItem, _T(""));
	for(int i = 0; i < COLCOUNT; i++)
	{
		str = DataToStr(i, Key, Data);
		m_List.SetItemText(nItem, i, str);
	}
	m_List.SetItemData(nItem, Key);

	return TRUE;
}

BOOL CCMMvhlListTrDlg::DeleteItem(T_MVHLtr_K Key, T_MVHLtr_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMMvhlListTrDlg::ModifyItem(T_MVHLtr_K KeyOld, T_MVHLtr_K Key, T_MVHLtr_D &Data)
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
		m_List.SetItemData(nItem, Key);
	}
	return TRUE;
}

CString CCMMvhlListTrDlg::DataToStr(int i, T_MVHLtr_K Key, T_MVHLtr_D &Data)
{
	CString str;

	if(i==0) str = Data.VehicleLoadName;
	else if(i==1) str =_LS(IDS_WG_CMD__ADDD__User_Designed);
	else str = _LS(IDS_WG_CMD__ADDD__Error);
	return str;
}

void CCMMvhlListTrDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

BEGIN_MESSAGE_MAP(CCMMvhlListTrDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlListTrDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD2, OnCmdBtnAddUser)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlListTrDlg message handlers

BOOL CCMMvhlListTrDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	SetHeaderTitle();
	MakeItemEx();

// US : 2004.02.06 고영현 차장님 지시사항 : Gen US인 경우 Standard Vehicle Load 사용하지 못하도록
// BY KYM
// MQC 11966 : 문의들어옴 > 순전히 사업상의 이유입니다.Civil을 안사고 Gen으로 교량을 설계하는 것을 방지하기 위해서입니다.
	GetDlgItem(IDC_CMD_BTN_ADD)->EnableWindow(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlListTrDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMMvhlItemTr dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();    	
}

void CCMMvhlListTrDlg::OnCmdBtnAddUser() 
{
	// TODO: Add your control notification handler code here
	CCMMvhlItemTr dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMvhlListTrDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_MVHLtr_K Key;
	T_MVHLtr_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_vehicle_load_));
		return;
	}
 
	Key = (T_MVHLtr_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetMvhltr(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Vehicle_load_key__d__does), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMMvhlItemTr dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);		
}

void CCMMvhlListTrDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_vehicle_load_));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelMvhltr(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMMvhlListTrDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CCMMvhlListTrDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_MVHLtr_K Key;
	T_MVHLtr_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return;  // no message, just return
 
	Key = (T_MVHLtr_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetMvhltr(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Error___Vehicle_load_key__d__does), Key);
		AfxMessageBox(msg);
		return;
	}

	
	CCMMvhlItemTr dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();


	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);		
}
