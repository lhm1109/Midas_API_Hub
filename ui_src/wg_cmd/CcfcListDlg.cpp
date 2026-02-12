// CcfcListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CcfcListDlg.h"
#include "CcfcItemDlg.h"

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
// CCcfcListDlg dialog


CCcfcListDlg::CCcfcListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCcfcListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCcfcListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCcfcListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCcfcListDlg)
	DDX_Control(pDX, IDC_CCFC_LIST, m_List);
	//}}AFX_DATA_MAP
}

void CCcfcListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CCcfcListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_CCFC_K Key, KeyBak;
	T_CCFC_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_CCFC_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetCcfc(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_CCFC_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetCcfc(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_CCFC_MFD):
				// 여기서는 저장만 하고 UR_CCFC_MFS에서 처리한다.
				pViewBuff->GetCcfc(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_CCFC_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetCcfc(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

BEGIN_MESSAGE_MAP(CCcfcListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCcfcListDlg)
	ON_BN_CLICKED(IDC_CCFC_BTN_ADD, OnCcfcBtnAdd)
	ON_BN_CLICKED(IDC_CCFC_BTN_MODIFY, OnCcfcBtnModify)
	ON_BN_CLICKED(IDC_CCFC_BTN_DELETE, OnCcfcBtnDelete)
	ON_BN_CLICKED(IDC_CCFC_BTN_CLOSE, OnCcfcBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_CCFC_LIST, OnDblclkCcfcList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define COLCOUNT 2
/////////////////////////////////////////////////////////////////////////////
// CCcfcListDlg message handlers
BOOL CCcfcListDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	SetHeaderTitle(TRUE);
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCcfcListDlg::SetHeaderTitle(BOOL bInit)
{
	CString aTitle[] = { _LS(IDS_CMD_CCFC_name), _LS(IDS_CMD_CCFC_type) };
//18japan  CString aTitle[] = {_T("Name"), _T("Type")};
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

void CCcfcListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountCcfc();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_CCFC_K Key;
		T_CCFC_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartCcfc();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextCcfc(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_CCFC_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetCcfc(Key, Data);
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

BOOL CCcfcListDlg::InsertItem(T_CCFC_K Key, T_CCFC_D &Data)
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

CString CCcfcListDlg::DataToStr(int i, T_CCFC_K Key, T_CCFC_D &Data)
{
	CString str;
	CString csFuncType[] = { _LS(IDS_CMD_CCFC_constant), _LS(IDS_CMD_CCFC_user) };
//18japan	CString csFuncType[] = {_T("Constant"), _T("User")};

	ASSERT(Data.nFuncType >= 1 && Data.nFuncType <= 2);

	if(i==0) str = Data.strFuncName;
	else if(i==1) str = csFuncType[Data.nFuncType-1];
	else str = _LS(IDS_CMD_CCFC_error);

	return str;
}

BOOL CCcfcListDlg::DeleteItem(T_CCFC_K Key, T_CCFC_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCcfcListDlg::ModifyItem(T_CCFC_K KeyOld, T_CCFC_K Key, T_CCFC_D &Data)
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

void CCcfcListDlg::OnCcfcBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CCcfcItemDlg dlg;
	dlg.DoModal();
}

void CCcfcListDlg::OnCcfcBtnModify() 
{
	// TODO: Add your control notification handler code here
	T_CCFC_K Key;
	T_CCFC_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_Convection_Coefficien));
		return;
	}
 
	Key = (T_CCFC_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetCcfc(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Convection_Coefficient_Function_k), Key);
		AfxMessageBox(msg);
		return;
	}

	CCcfcItemDlg dlg;
	dlg.SetParamData(Data);
//  dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCcfcListDlg::OnCcfcBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_Convection_Coefficien));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelCcfc(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCcfcListDlg::OnCcfcBtnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CCcfcListDlg::OnDblclkCcfcList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnCcfcBtnModify();
	*pResult = 0;
}
