// CMRposListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRposListDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "CMRposItemDlg.h"
	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMRposListDlg dialog


CCMRposListDlg::CCMRposListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMRposListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMRposListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nInitPos = D_INIT_POS_LT;
}


void CCMRposListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRposListDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_listData);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 2
/////////////////////////////////////////////////////////////////////////////
// CCMRposListDlg implementation functions
void CCMRposListDlg::SetHeaderTitle(BOOL bInit)
{
	CString aTitle[] = { _LS(IDS_CMD0417__Name), _LS(IDS_CMD0417__Description) };
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	if (bInit)
	{
		DWORD dwStyle;
		dwStyle = ListView_GetExtendedListViewStyle(m_listData.GetSafeHwnd());
	  dwStyle |= LVS_EX_FULLROWSELECT; 
	  ListView_SetExtendedListViewStyle(m_listData.GetSafeHwnd(), dwStyle);
	}

	// Calculate width for each column
	nColWidth[0] = 100; nColWidth[1] = 128;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
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
			m_listData.InsertColumn(i,&lvcolumn);
		m_listData.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMRposListDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	if(!bUnitChanged)
		m_listData.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountRpos();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	CString str;
	int nCount;

	if(nItemCount)
	{
		CArray<T_RPOS_K, T_RPOS_K> aRposKey;
		T_RPOS_K Key;
		T_RPOS_D Data;

		m_pDoc->m_pAttrCtrl->GetRposKeyList(aRposKey);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=aRposKey[nCount];
			m_pDoc->m_pAttrCtrl->GetRpos(Key,Data);
			for(int i = 0; i < COLCOUNT; i++)
			{
				lvitem.iItem=nCount;
				lvitem.iSubItem=i;
				str = DataToStr(i, Key, Data);
				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT ;
			  if(!bUnitChanged)
			  {
				  if( i == 0 )
					{
						lvitem.mask |= LVIF_PARAM;
						lvitem.lParam = (LPARAM)Key;
					  m_listData.InsertItem(&lvitem);
					}
				  else
					  m_listData.SetItem(&lvitem);
			  }
			  else
			  {
				  m_listData.SetItemText(lvitem.iItem,lvitem.iSubItem,lvitem.pszText);
			  }
			  str.ReleaseBuffer();
			}
		}
	}
}

void CCMRposListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_RPOS_K Key, KeyBak;
	T_RPOS_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_RPOS_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetRpos(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_RPOS_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetRpos(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_RPOS_MFD):
				// 여기서는 저장만 하고 UR_RPOS_MFS에서 처리한다.
				pViewBuff->GetRpos(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_RPOS_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetRpos(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

void CCMRposListDlg::UnitChanged()
{
	// unit 변경과 관련 없다.
	return;
}


BOOL CCMRposListDlg::InsertItem(T_RPOS_K Key, T_RPOS_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_listData.GetItemCount();
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
			nItem = m_listData.InsertItem(&lvitem);
		}
		else m_listData.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CCMRposListDlg::DeleteItem(T_RPOS_K Key, T_RPOS_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_listData.FindItem(&FindInfo);
	if (nItem != -1) m_listData.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMRposListDlg::ModifyItem(T_RPOS_K KeyOld, T_RPOS_K Key, T_RPOS_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_listData.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Key, Data);
			m_listData.SetItemText(nItem, i, str);
		}
		// key 재 설정 
		LVITEM lvitem;
		lvitem.iItem = nItem;
		lvitem.iSubItem=0;
		lvitem.mask = LVIF_PARAM;
		lvitem.lParam = (LPARAM)Key;
		m_listData.SetItem(&lvitem);
	}
	return TRUE;
}

CString CCMRposListDlg::DataToStr(int i, T_RPOS_K Key, T_RPOS_D &Data)
{
	CString str;

	if(i==0)str = Data.Name;
	else if(i==1) str = Data.Description;

	return str;
}


BEGIN_MESSAGE_MAP(CCMRposListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMRposListDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMRposListDlg message handlers

BOOL CCMRposListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	SetHeaderTitle(TRUE);
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMRposListDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	if (m_pDoc->m_pAttrCtrl->GetCountStag() <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD0417__Error___No_stage_exists_));
		return;
	}
	CCMRposItemDlg dlg;
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();	
}

void CCMRposListDlg::OnCmdBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_RPOS_K Key;
	T_RPOS_D Data;

	int iItem = m_listData.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__No_selected_reaction_position_data));
		return;
	}
 
	Key = (T_RPOS_K)m_listData.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetRpos(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__Reaction_position_key__d_does_not_exis), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMRposItemDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_listData.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMRposListDlg::OnCmdBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_listData.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD0417__No_selected_reaction_position_data));
		return;
	}
	CString csName = m_listData.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelRpos(csName)) return;

	int nCount = m_listData.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_listData.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);		
}

void CCMRposListDlg::OnCmdBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CCMRposListDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_RPOS_K Key;
	T_RPOS_D Data;

	int iItem = m_listData.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = (T_RPOS_K)m_listData.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetRpos(Key, Data)) return;

	CString csOldName = Data.Name;
	CCMRposItemDlg dlg;
	dlg.SetParamData(Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_listData.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}

void CCMRposListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		UnitChanged();
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
