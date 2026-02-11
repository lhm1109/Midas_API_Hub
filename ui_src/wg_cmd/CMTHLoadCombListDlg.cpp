#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTHLoadCombListDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "CMTHLoadCombItemDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCOUNT 2

CCMTHLoadCombListDlg::CCMTHLoadCombListDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMTHLoadCombListDlg::IDD, pParent)
{
}

void CCMTHLoadCombListDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
}

BEGIN_MESSAGE_MAP(CCMTHLoadCombListDlg, CCMDlgBase)
	ON_BN_CLICKED(IDC_CMD_ADD        , OnBtnAdd     )
	ON_BN_CLICKED(IDC_CMD_MODIFY     , OnBtnModify  )
	ON_BN_CLICKED(IDC_CMD_DELETE     , OnBtnDelete  )
	ON_BN_CLICKED(IDC_CMD_CLOSE      , OnBtnClose   )
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkList )	
END_MESSAGE_MAP()


BOOL CCMTHLoadCombListDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	
	SetHeaderTitle(TRUE);
	MakeItemEx();

	return TRUE;
}

void CCMTHLoadCombListDlg::SetHeaderTitle(BOOL bInit)
{
	CString aTitle[] = { _LS(IDS_CMD_LCOM_NAME), _LS(IDS_WG_CMD__ADDD__Type) };

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
	nColWidth[0] = 170;
	nColWidth[1] = 120;

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

void CCMTHLoadCombListDlg::MakeItemEx()
{
	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountThlc();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_THLC_K Key;
		T_THLC_D Data;

		int* KeyBuf=new int[nItemCount];
		nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartThlc();
		while(pos !=NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextThlc(pos, Key, Data);
			KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_THLC_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetThlc(Key,Data);
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
CString CCMTHLoadCombListDlg::DataToStr(int i, T_THLC_K Key, T_THLC_D &Data)
{
	CString str;
	if     (i==0) str = Data.strName;
	else if(i==1)
	{
		if(Data.nType==D_THLC_TYPE_ENV) str = _LS(IDS_DB_DT_ENVL);
		else str = _LS(IDS_CMD_AVERAGE);
	}
	else { ASSERT(0); str.Empty(); }

	return str;
}

void CCMTHLoadCombListDlg::OnBtnAdd() 
{
	CCMTHLoadCombItemDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT, 5, 50);
	dlg.DoModal();
}

void CCMTHLoadCombListDlg::OnBtnModify() 
{	
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);

	T_THLC_K Key;
	T_THLC_D Data;

	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_THLC_No_selected_data));
		return;
	}

	Key = (T_THLC_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetThlc(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_THLC_Key_does_not_exist), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMTHLoadCombItemDlg dlg;
	dlg.m_bModify = TRUE;
	dlg.SetData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT, 5, 50);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMTHLoadCombListDlg::OnBtnDelete() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_THLC_No_selected_data));
		return;
	}

	T_THLC_K ThlcK;
	ThlcK = m_List.GetItemData(iItem);
	if (!m_pDoc->m_pDataCtrl->DelThlc(ThlcK)) return;


	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMTHLoadCombListDlg::OnBtnClose() 
{
	DestroyWindow();
}

void CCMTHLoadCombListDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnBtnModify();	
	*pResult = 0;
}

void CCMTHLoadCombListDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START:
		DestroyWindow(); return;
	case D_UPDATE_EXE_END:
		break;
	default:
		break;
	}
}


void CCMTHLoadCombListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_THLC_K Key=0;
	T_THLC_D Data, DataBak;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_THLC_ADD):
		case(UR_THLC_MFD):
		case(UR_THLC_MFS):
			m_List.DeleteAllItems();
			MakeItemEx();
			break;
		case(UR_THLC_DEL):
			pViewBuff->GetThlc(nKey, Key, Data);
			DeleteItem(Key, Data);
			break;
		default:
			break;
		}
	}

}

BOOL CCMTHLoadCombListDlg::InsertItem(T_THLC_K Key, T_THLC_D &Data)
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

BOOL CCMTHLoadCombListDlg::DeleteItem(T_THLC_K Key, T_THLC_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CCMTHLoadCombListDlg::ModifyItem(T_THLC_K KeyOld, T_THLC_K Key, T_THLC_D &Data)
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