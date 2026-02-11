// CMDropPanelPageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDropPanelPageDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "CMDropPanelItemDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT 1

/////////////////////////////////////////////////////////////////////////////
// CCMDropPanelPageDlg dialog
CCMDropPanelPageDlg::CCMDropPanelPageDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMDropPanelPageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMDropPanelPageDlg)
	//}}AFX_DATA_INIT
}

// void CCMSectionPage::DoDataExchange(CDataExchange* pDX)
// {
//   CChildDialog::DoDataExchange(pDX);
//   //{{AFX_DATA_MAP(CCMSectionPage)
//   //}}AFX_DATA_MAP
//   DDX_Control(pDX, IDC_CMD_SP_SEC_LIST, m_List);
// }

void CCMDropPanelPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDropPanelPageDlg)
	DDX_Control(pDX, IDC_CMD_PAGE_LIST, m_List);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMDropPanelPageDlg implementation
BEGIN_MESSAGE_MAP(CCMDropPanelPageDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMDropPanelPageDlg)
	ON_BN_CLICKED(IDC_CMD_PAGE_ADD_BTN,     OnBtnAdd)
	ON_BN_CLICKED(IDC_CMD_PAGE_MODIFY_BTN,  OnBtnModify)
	ON_BN_CLICKED(IDC_CMD_PAGE_DELETE_BTN,  OnBtnDelete)
	ON_BN_CLICKED(IDC_CMD_PAGE_CLOSE_BTN,   OnBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_PAGE_LIST, OnDblclkList)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMDropPanelPageDlg message handlers
BOOL CCMDropPanelPageDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();

	SetHeaderTitle(TRUE);
	MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDropPanelPageDlg::SetHeaderTitle(BOOL bInit)
{
	CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Name) };
#ifdef _CH
	aTitle[0] = _LS(IDS_AMR_NAME);
#endif
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
	nColWidth[0] = 200;

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
			m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}
void CCMDropPanelPageDlg::MakeItemEx(BOOL bUnitChanged/* = FALSE*/)
{
	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountDpan();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_DPAN_K Key;
		T_DPAN_D Data;

		int* KeyBuf=new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl2->GetStartDpan();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl2->GetNextDpan(pos, Key, Data);
			KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_DPAN_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl2->GetDpan(Key,Data);
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
						m_List.InsertItem(&lvitem);
					}
					else
						m_List.SetItem(&lvitem);
				}
				else
				{
					m_List.SetItemText(lvitem.iItem,lvitem.iSubItem,lvitem.pszText);
				}
				str.ReleaseBuffer();
			}
		}
		delete []KeyBuf;
	}
}

CString CCMDropPanelPageDlg::DataToStr(int i, T_DPAN_K Key, T_DPAN_D &Data)
{
	CString str;
	if     (i==0) str = Data.strName;
	else if(i==1) str = Data.strDecs;
	else if(i==2) str.Format(_T("%g"), Data.dB1);
	else if(i==3) str.Format(_T("%g"), Data.dB2);
	else if(i==4) str.Format(_T("%g"), Data.dH1);
	else if(i==5) str.Format(_T("%g"), Data.dH2);
	else { ASSERT(0); str.Empty(); }

	return str;
}
void CCMDropPanelPageDlg::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here

	CCMDropPanelItemDlg dlg;
	
	dlg.SetInitPos(D_INIT_POS_RT, 5, 50);
	dlg.DoModal();
}

void CCMDropPanelPageDlg::OnBtnModify() 
{	
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);

	T_DPAN_K Key;
	T_DPAN_D Data;

	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_PUSHOVER_No_selected_Hinge_Data_Type__nChoose_o));
		return;
	}

	Key = (T_DPAN_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetDpan(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_PUSHOVER_Hinge_Data_Type_key__d_does_not_exits_), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMDropPanelItemDlg dlg;
	dlg.m_bModify = TRUE;
	dlg.SetData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT, 5, 50);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMDropPanelPageDlg::OnBtnDelete() 
{
//   // TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_PUSHOVER_No_selected_Hinge_Data_Type__nChoose_o));
		return;
	}

	T_DPAN_K DpanK;
	DpanK = m_List.GetItemData(iItem);
	if (!m_pDoc->m_pDataCtrl->DelDpan(DpanK)) return;
	

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMDropPanelPageDlg::OnBtnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMDropPanelPageDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnBtnModify();
	*pResult = 0;
}

void CCMDropPanelPageDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
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
	case D_UPDATE_EXE_START:
		DestroyWindow(); return;
	case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}


void CCMDropPanelPageDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();


	BOOL bMFD = FALSE;
	T_DPAN_K Key, KeyBak;
	T_DPAN_D Data, DataBak;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_DPAN_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetDpan(nKey, Key, Data);
			InsertItem(Key, Data);
			break;
		case(UR_DPAN_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetDpan(nKey, Key, Data);
			DeleteItem(Key, Data);
			break;
		case(UR_DPAN_MFD):
			// 여기서는 저장만 하고 UR_DPAN_MFS에서 처리한다.
			pViewBuff->GetDpan(nKey, Key, Data);
			KeyBak = Key;
			DataBak = Data;
			bMFD = TRUE;
			break;
		case(UR_DPAN_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetDpan(nKey, Key, Data);
			ModifyItem(Key, KeyBak, DataBak);
			// 플래그 수정
			bMFD = FALSE;
			break;
		default:
			break;
		}
	}

}

void CCMDropPanelPageDlg::UnitChanged()
{
	// unit 변경과 관련 없다.
	return;
}

BOOL CCMDropPanelPageDlg::InsertItem(T_DPAN_K Key, T_DPAN_D &Data)
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

BOOL CCMDropPanelPageDlg::DeleteItem(T_DPAN_K Key, T_DPAN_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMDropPanelPageDlg::ModifyItem(T_DPAN_K KeyOld, T_DPAN_K Key, T_DPAN_D &Data)
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

BOOL CCMDropPanelPageDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::OnNotify(wParam, lParam, pResult);
}