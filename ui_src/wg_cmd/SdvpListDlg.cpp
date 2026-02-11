// SdvpListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SdvpListDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "CMSdviDlg.h"
#include "CMSdveDlg.h"
#include "CMSdstDlg.h"
#include "CMSdhyDlg.h"
#include "CMSdisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSdvpListDlg dialog

CSdvpListDlg::CSdvpListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSdvpListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSdvpListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strInitSelect = _T("");
	m_bInitSelect = FALSE;
	//m_aSdvpData.RemoveAll();  
}


void CSdvpListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSdvpListDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}

#define COLCOUNT 4

/////////////////////////////////////////////////////////////////////////////
// CSdvpListDlg implementation functions
void CSdvpListDlg::SetHeaderTitle()
{
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	CString aTitle[COLCOUNT];
	int aColWidth[COLCOUNT];
	GetHeaderTitleWidth(aTitle, aColWidth);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(aColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CSdvpListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

BEGIN_MESSAGE_MAP(CSdvpListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSdvpListDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD   , OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)	
	ON_BN_CLICKED(IDC_CMD_BTN_COPY  , OnCmdBtnCopy)	
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE , OnCmdBtnClose)	
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSdvpListDlg message handlers

BOOL CSdvpListDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetTitleText();
	SetHeaderTitle();
	MakeItemEx();	

	if (m_bInitSelect)
		SelectItem(m_strInitSelect);

	return TRUE;
}

void CSdvpListDlg::SelectItem(CString str)
{
	int nSize = m_List.GetItemCount();
	for (int i=0; i<nSize; i++)
	{
		if (str.Compare(m_List.GetItemText(i, 0)) == 0)
		{
			m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
			break;
		}
	}
}

void CSdvpListDlg::OnCmdBtnAdd()    { OnBtnAdd(); }
void CSdvpListDlg::OnCmdBtnModify() { OnBtnModify(); }
void CSdvpListDlg::OnCmdBtnCopy()   { OnBtnCopy(); }
void CSdvpListDlg::OnCmdBtnDelete() { OnBtnDel(); }
void CSdvpListDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) { OnDblClk(pNMHDR,pResult); }
void CSdvpListDlg::OnCmdBtnClose()
{
	CDialogMove::OnOK();
}
void CSdvpListDlg::SetInitSelect(CString strInit)
{
	m_bInitSelect = TRUE;
	m_strInitSelect = strInit;
}

void CSdviListDlg::SetTitleText()
{
	this->SetWindowText(_LS(IDS_CMD_DEFINE_PROP_SDVI));
}
void CSdviListDlg::OnBtnAdd()
{
	CCMSdviDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();		
}
void CSdviListDlg::OnBtnModify()
{
	T_SDVI_K Key;
	T_SDVI_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDVI));
		return;
	}

	Key = (T_SDVI_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdvi(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_SDVI_KEY_NOT_EXIST), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMSdviDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
void CSdviListDlg::OnBtnDel()
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDVI));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	T_SDVI_K SdviK = m_pDoc->m_pAttrCtrl2->GetSdviKey(csName);
	if (!m_pDoc->m_pDataCtrl->DelSdvi(SdviK)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
void CSdviListDlg::OnBtnCopy()
{
	T_SDVI_K Key;
	T_SDVI_D Data;
	
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDVI));
		return;
	}
	
	Key = (T_SDVI_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdvi(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_SDVI_KEY_NOT_EXIST), Key);
		AfxMessageBox(msg);
		return;
	}
	T_SDVI_D CopySdvi;
	CopySdvi = Data;
	
	 // 새 이름 찾기
	CString strNameTemp;
	 int nCopyNameCount = 1;
	 while(TRUE)
	 {
		 strNameTemp.Format(_T("%s-%d"),Data.strName,nCopyNameCount++);
		 T_SDVI_K SdviK = m_pDoc->m_pAttrCtrl2->GetSdviKey(strNameTemp);
		 if(!m_pDoc->m_pAttrCtrl2->ExistSdvi(SdviK))
			 break;
	 }
	CopySdvi.strName = strNameTemp;
	if(!m_pDoc->m_pDataCtrl->AddSdvi(CopySdvi))	return;
	
	MakeItemEx();
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}
void CSdviListDlg::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	T_SDVI_K Key;
	T_SDVI_D Data;
	
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
	
	Key = (T_SDVI_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdvi(Key, Data)) return;
	
	CCMSdviDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
	
	 m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}
void CSdviListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountSdvi();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;  
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SDVI_K Key;
		T_SDVI_D Data;

		nCount = 0;
		CArray<T_SDVI_K, T_SDVI_K> aSdviKeyList;
		aSdviKeyList.RemoveAll();
		m_pDoc->m_pAttrCtrl2->GetSdviKeyList(aSdviKeyList);
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key = aSdviKeyList.GetAt(nCount);
			m_pDoc->m_pAttrCtrl2->GetSdvi(Key,Data);
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
void CSdviListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SDVI_K SdviKey, SdviK_bak;
	T_SDVI_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SDVI_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetSdvi(nKey, SdviKey, Data);
			InsertItem(SdviKey, Data);
			break;
		case(UR_SDVI_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetSdvi(nKey, SdviKey, Data);
			DeleteItem(SdviKey, Data);
			break;
		case(UR_SDVI_MFD):
			// 여기서는 저장만 하고 UR_SDVI_MFS에서 처리한다.
			pViewBuff->GetSdvi(nKey, SdviKey, Data);
			SdviK_bak = SdviKey;
			DataBak = Data;
			bMFD = TRUE;
			break;
		case(UR_SDVI_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetSdvi(nKey, SdviKey, Data);
			ModifyItem(SdviKey, SdviK_bak, DataBak);
			// 플래그 수정
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}
BOOL CSdviListDlg::InsertItem(T_SDVI_K Key, T_SDVI_D &Data)
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
BOOL CSdviListDlg::DeleteItem(T_SDVI_K Key, T_SDVI_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}
BOOL CSdviListDlg::ModifyItem(T_SDVI_K KeyOld, T_SDVI_K Key, T_SDVI_D &Data)
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
CString CSdviListDlg::DataToStr(int i, T_SDVI_K Key, T_SDVI_D &Data)
{
	CString str;

	CString csDamperType[]  = {_LS(IDS_CMD_SDVP_DAMPER_SINGLE), _LS(IDS_CMD_SDVP_DAMPER_KELVIN), _LS(IDS_CMD_SDVP_DAMPER_MAXWEL) };
	CString csDashpotType[] = {_LS(IDS_CMD_SDVP_DASHPOT_LELA), _LS(IDS_CMD_SDVP_DASHPOT_EBIL), _LS(IDS_CMD_SDVP_DASHPOT_EXFN)};
	ASSERT(Data.nDamperType >= 0 && Data.nDamperType <= 2);		
	ASSERT(Data.nDashpotType >= 0 && Data.nDashpotType <= 2);		

	if(i==0) str = Data.strName;
	else if(i==1) str = csDamperType[Data.nDamperType];
	else if(i==2) str = csDashpotType[Data.nDashpotType];
	else if(i==3) str = Data.strDesc;  
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}
void CSdviListDlg::GetHeaderTitleWidth(CString* aTitle, int* aColWidth)
{
	aTitle[0] = _LS(IDS_WG_CMD__ADDD__Name)       ;
	aTitle[1] = _LS(IDS_CMD_SDVP_DAMPER_TYPE)     ;
	aTitle[2] = _LS(IDS_CMD_SDVP_DASHPOT_TYPE)    ;
	aTitle[3] = _LS(IDS_WG_CMD__ADDD__Description);
	
	aColWidth[0] = 90;
	aColWidth[1] = 140;
	aColWidth[2] = 160;
	aColWidth[3] = 100;
}

void CSdveListDlg::SetTitleText()
{
	this->SetWindowText(_LS(IDS_CMD_DEFINE_PROP_SDVE));
}
void CSdveListDlg::OnBtnAdd()
{
	CCMSdveDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();		
}
void CSdveListDlg::OnBtnModify()
{
	T_SDVE_K Key;
	T_SDVE_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDVE));
		return;
	}

	Key = (T_SDVE_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdve(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_SDVE_KEY_NOT_EXIST), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMSdveDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
void CSdveListDlg::OnBtnDel()
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDVE));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	T_SDVE_K SdveK = m_pDoc->m_pAttrCtrl2->GetSdveKey(csName);
	if (!m_pDoc->m_pDataCtrl->DelSdve(SdveK)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
void CSdveListDlg::OnBtnCopy()
{
	T_SDVE_K Key;
	T_SDVE_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDVE));
		return;
	}

	Key = (T_SDVE_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdve(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_SDVE_KEY_NOT_EXIST), Key);
		AfxMessageBox(msg);
		return;
	}
	T_SDVE_D CopySdve;
	CopySdve = Data;

	// 새 이름 찾기
	CString strNameTemp;
	int nCopyNameCount = 1;
	while(TRUE)
	{
		strNameTemp.Format(_T("%s-%d"),Data.strName,nCopyNameCount++);
		T_SDVE_K SdveK = m_pDoc->m_pAttrCtrl2->GetSdveKey(strNameTemp);
		if(!m_pDoc->m_pAttrCtrl2->ExistSdve(SdveK))
			break;
	}
	CopySdve.strName = strNameTemp;
	if(!m_pDoc->m_pDataCtrl->AddSdve(CopySdve))	return;

	MakeItemEx();
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}
void CSdveListDlg::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	T_SDVE_K Key;
	T_SDVE_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	Key = (T_SDVE_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdve(Key, Data)) return;

	CCMSdveDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}
void CSdveListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountSdve();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;  
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SDVE_K Key;
		T_SDVE_D Data;

		nCount = 0;
		CArray<T_SDVE_K, T_SDVE_K> aSdveKeyList;
		aSdveKeyList.RemoveAll();
		m_pDoc->m_pAttrCtrl2->GetSdveKeyList(aSdveKeyList);
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key = aSdveKeyList.GetAt(nCount);
			m_pDoc->m_pAttrCtrl2->GetSdve(Key,Data);
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
void CSdveListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SDVE_K SdveKey, SdveK_bak;
	T_SDVE_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SDVE_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetSdve(nKey, SdveKey, Data);
			InsertItem(SdveKey, Data);
			break;
		case(UR_SDVE_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetSdve(nKey, SdveKey, Data);
			DeleteItem(SdveKey, Data);
			break;
		case(UR_SDVE_MFD):
			// 여기서는 저장만 하고 UR_SDVE_MFS에서 처리한다.
			pViewBuff->GetSdve(nKey, SdveKey, Data);
			SdveK_bak = SdveKey;
			DataBak = Data;
			bMFD = TRUE;
			break;
		case(UR_SDVE_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetSdve(nKey, SdveKey, Data);
			ModifyItem(SdveKey, SdveK_bak, DataBak);
			// 플래그 수정
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}
BOOL CSdveListDlg::InsertItem(T_SDVE_K Key, T_SDVE_D &Data)
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
BOOL CSdveListDlg::DeleteItem(T_SDVE_K Key, T_SDVE_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}
BOOL CSdveListDlg::ModifyItem(T_SDVE_K KeyOld, T_SDVE_K Key, T_SDVE_D &Data)
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
CString CSdveListDlg::DataToStr(int i, T_SDVE_K Key, T_SDVE_D &Data)
{
	CString str;
	CString csMType[]  = { _LS(IDS_ETC_SDVE_MTYPE_GR100), _LS(IDS_ETC_SDVE_MTYPE_GR300), _LS(IDS_ETC_SDVE_MTYPE_SR05),
												 _LS(IDS_ETC_SDVE_MTYPE_GR400), _LS(IDS_ETC_SDVE_MTYPE_ISD111), _LS(IDS_ETC_SDVE_MTYPE_ISD111H), _LS(IDS_ETC_SDVE_MTYPE_CST), _LS(IDS_ETC_SDVE_MTYPE_TRC) };
	CString csDOF[] = { _T("Dx"), _T("Dy"), _T("Dz") };
	ASSERT(Data.nMatlType==D_SDVE_GR100 || Data.nMatlType==D_SDVE_GR300 || Data.nMatlType==D_SDVE_SR05 || 
				 Data.nMatlType==D_SDVE_GR400 || Data.nMatlType==D_SDVE_ISD111 || Data.nMatlType==D_SDVE_ISD111H || Data.nMatlType==D_SDVE_CST || Data.nMatlType==D_SDVE_TRC);

	if(i==0) str = Data.strName;
	else if(i==1) str = csMType[Data.nMatlType-1];
	else if(i==2) str = csDOF[Data.nDir-1];
	else if(i==3) str = Data.strDesc;  
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}
void CSdveListDlg::GetHeaderTitleWidth(CString* aTitle, int* aColWidth)
{
	aTitle[0] = _LS(IDS_WG_CMD__ADDD__Name);
	aTitle[1] = _LS(IDS_ETC_SDVP_MATLTYPE) ;
	aTitle[2] = _LS(IDS_ETC_SDVP_DOF)      ;
	aTitle[3] = _LS(IDS_WG_CMD__ADDD__Description);

	aColWidth[0] = 90;
	aColWidth[1] = 160;
	aColWidth[2] = 100;
	aColWidth[3] = 140;
}

void CSdstListDlg::SetTitleText()
{
	this->SetWindowText(_LS(IDS_CMD_DEFINE_PROP_SDST));
}
void CSdstListDlg::OnBtnAdd()
{
	CCMSdstDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();		
}
void CSdstListDlg::OnBtnModify()
{
	T_SDST_K Key;
	T_SDST_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDST));
		return;
	}

	Key = (T_SDST_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdst(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_SDST_KEY_NOT_EXIST), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMSdstDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
void CSdstListDlg::OnBtnDel()
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDST));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	T_SDST_K SdstK = m_pDoc->m_pAttrCtrl2->GetSdstKey(csName);
	if (!m_pDoc->m_pDataCtrl->DelSdst(SdstK)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
void CSdstListDlg::OnBtnCopy()
{
	T_SDST_K Key;
	T_SDST_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDST));
		return;
	}

	Key = (T_SDST_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdst(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_SDST_KEY_NOT_EXIST), Key);
		AfxMessageBox(msg);
		return;
	}
	T_SDST_D CopySdst;
	CopySdst = Data;

	// 새 이름 찾기
	CString strNameTemp;
	int nCopyNameCount = 1;
	while(TRUE)
	{
		strNameTemp.Format(_T("%s-%d"),Data.strName,nCopyNameCount++);
		T_SDST_K SdstK = m_pDoc->m_pAttrCtrl2->GetSdstKey(strNameTemp);
		if(!m_pDoc->m_pAttrCtrl2->ExistSdst(SdstK))
			break;
	}
	CopySdst.strName = strNameTemp;
	if(!m_pDoc->m_pDataCtrl->AddSdst(CopySdst))	return;

	MakeItemEx();
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}
void CSdstListDlg::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	T_SDST_K Key;
	T_SDST_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	Key = (T_SDST_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdst(Key, Data)) return;

	CCMSdstDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}
void CSdstListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountSdst();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;  
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SDST_K Key;
		T_SDST_D Data;

		nCount = 0;
		CArray<T_SDST_K, T_SDST_K> aSdstKeyList;
		aSdstKeyList.RemoveAll();
		m_pDoc->m_pAttrCtrl2->GetSdstKeyList(aSdstKeyList);
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key = aSdstKeyList.GetAt(nCount);
			m_pDoc->m_pAttrCtrl2->GetSdst(Key,Data);
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
void CSdstListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SDST_K SdstKey, SdstK_bak;
	T_SDST_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SDST_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetSdst(nKey, SdstKey, Data);
			InsertItem(SdstKey, Data);
			break;
		case(UR_SDST_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetSdst(nKey, SdstKey, Data);
			DeleteItem(SdstKey, Data);
			break;
		case(UR_SDST_MFD):
			// 여기서는 저장만 하고 UR_SDST_MFS에서 처리한다.
			pViewBuff->GetSdst(nKey, SdstKey, Data);
			SdstK_bak = SdstKey;
			DataBak = Data;
			bMFD = TRUE;
			break;
		case(UR_SDST_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetSdst(nKey, SdstKey, Data);
			ModifyItem(SdstKey, SdstK_bak, DataBak);
			// 플래그 수정
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}
BOOL CSdstListDlg::InsertItem(T_SDST_K Key, T_SDST_D &Data)
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
BOOL CSdstListDlg::DeleteItem(T_SDST_K Key, T_SDST_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}
BOOL CSdstListDlg::ModifyItem(T_SDST_K KeyOld, T_SDST_K Key, T_SDST_D &Data)
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
CString CSdstListDlg::DataToStr(int i, T_SDST_K Key, T_SDST_D &Data)
{
	CString str;

	CString csHysModel[]  = { _LS(IDS_ETC_SDST_HYS_BL2), _LS(IDS_ETC_SDST_HYS_LY2), _LS(IDS_ETC_SDST_HYS_LY3), _LS(IDS_ETC_SDST_HYS_IK2) };
	CString csDOF[] = { _T("Dx"), _T("Dy"), _T("Dz") };
	ASSERT(Data.nHysModel==D_SDST_HYS_BL2 || Data.nHysModel==D_SDST_HYS_LY2 || Data.nHysModel==D_SDST_HYS_LY3 || Data.nHysModel==D_SDST_HYS_IK2);

	if(i==0) str = Data.strName;
	else if(i==1) str = csHysModel[Data.GetHysIdx()];
	else if(i==2) str = csDOF[Data.nDir-1];
	else if(i==3) str = Data.strDesc;  
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}
void CSdstListDlg::GetHeaderTitleWidth(CString* aTitle, int* aColWidth)
{
	aTitle[0] = _LS(IDS_WG_CMD__ADDD__Name);
	aTitle[1] = _LS(IDS_ETC_SDVP_HYSMODEL) ;
	aTitle[2] = _LS(IDS_ETC_SDVP_DOF)      ;
	aTitle[3] = _LS(IDS_WG_CMD__ADDD__Description);

	aColWidth[0] = 90;
	aColWidth[1] = 160;
	aColWidth[2] = 100;
	aColWidth[3] = 140;
}

void CSdhyListDlg::SetTitleText()
{
	this->SetWindowText(_LS(IDS_CMD_DEFINE_PROP_SDHY));
}
void CSdhyListDlg::OnBtnAdd()
{
	CCMSdhyDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();		
}
void CSdhyListDlg::OnBtnModify()
{
	T_SDHY_K Key;
	T_SDHY_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDHY));
		return;
	}

	Key = (T_SDHY_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdhy(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_SDHY_KEY_NOT_EXIST), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMSdhyDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
void CSdhyListDlg::OnBtnDel()
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDHY));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	T_SDHY_K SdhyK = m_pDoc->m_pAttrCtrl2->GetSdhyKey(csName);
	if (!m_pDoc->m_pDataCtrl->DelSdhy(SdhyK)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
void CSdhyListDlg::OnBtnCopy()
{
	T_SDHY_K Key;
	T_SDHY_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDHY));
		return;
	}

	Key = (T_SDHY_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdhy(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_SDHY_KEY_NOT_EXIST), Key);
		AfxMessageBox(msg);
		return;
	}
	T_SDHY_D CopySdhy;
	CopySdhy = Data;

	// 새 이름 찾기
	CString strNameTemp;
	int nCopyNameCount = 1;
	while(TRUE)
	{
		strNameTemp.Format(_T("%s-%d"),Data.strName,nCopyNameCount++);
		T_SDHY_K SdhyK = m_pDoc->m_pAttrCtrl2->GetSdhyKey(strNameTemp);
		if(!m_pDoc->m_pAttrCtrl2->ExistSdhy(SdhyK))
			break;
	}
	CopySdhy.strName = strNameTemp;
	if(!m_pDoc->m_pDataCtrl->AddSdhy(CopySdhy))	return;

	MakeItemEx();
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}
void CSdhyListDlg::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	T_SDHY_K Key;
	T_SDHY_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	Key = (T_SDHY_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdhy(Key, Data)) return;

	CCMSdhyDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}
void CSdhyListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountSdhy();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;  
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SDHY_K Key;
		T_SDHY_D Data;

		nCount = 0;
		CArray<T_SDHY_K, T_SDHY_K> aSdhyKeyList;
		aSdhyKeyList.RemoveAll();
		m_pDoc->m_pAttrCtrl2->GetSdhyKeyList(aSdhyKeyList);
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key = aSdhyKeyList.GetAt(nCount);
			m_pDoc->m_pAttrCtrl2->GetSdhy(Key,Data);
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
void CSdhyListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SDHY_K SdhyKey, SdhyK_bak;
	T_SDHY_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SDHY_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetSdhy(nKey, SdhyKey, Data);
			InsertItem(SdhyKey, Data);
			break;
		case(UR_SDHY_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetSdhy(nKey, SdhyKey, Data);
			DeleteItem(SdhyKey, Data);
			break;
		case(UR_SDHY_MFD):
			// 여기서는 저장만 하고 UR_SDHY_MFS에서 처리한다.
			pViewBuff->GetSdhy(nKey, SdhyKey, Data);
			SdhyK_bak = SdhyKey;
			DataBak = Data;
			bMFD = TRUE;
			break;
		case(UR_SDHY_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetSdhy(nKey, SdhyKey, Data);
			ModifyItem(SdhyKey, SdhyK_bak, DataBak);
			// 플래그 수정
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}
BOOL CSdhyListDlg::InsertItem(T_SDHY_K Key, T_SDHY_D &Data)
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
BOOL CSdhyListDlg::DeleteItem(T_SDHY_K Key, T_SDHY_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}
BOOL CSdhyListDlg::ModifyItem(T_SDHY_K KeyOld, T_SDHY_K Key, T_SDHY_D &Data)
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
CString CSdhyListDlg::DataToStr(int i, T_SDHY_K Key, T_SDHY_D &Data)
{
	CString str;

	CString csHysModel[]  = { _LS(IDS_ETC_SDHY_HYS_DBM), _LS(IDS_ETC_SDHY_HYS_NTM), _LS(IDS_ETC_SDHY_HYS_MRO) };
	ASSERT(Data.nHysModel==D_SDHY_HYS_DEGRBL || Data.nHysModel==D_SDHY_HYS_NORTRI || Data.nHysModel==D_SDHY_HYS_MODIRO);		

	if(i==0) str = Data.strName;
	else if(i==1) str = csHysModel[Data.GetHysIdx()];
	else if(i==2) str.Format(_T("%d"),Data.nNumMSS);
	else if(i==3) str = Data.strDesc;  
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}
void CSdhyListDlg::GetHeaderTitleWidth(CString* aTitle, int* aColWidth)
{
	aTitle[0] = _LS(IDS_WG_CMD__ADDD__Name);
	aTitle[1] = _LS(IDS_ETC_SDVP_HYSMODEL) ;
	aTitle[2] = _LS(IDS_ETC_SDVP_NUMMSS)   ;
	aTitle[3] = _LS(IDS_WG_CMD__ADDD__Description);

	aColWidth[0] = 90;
	aColWidth[1] = 160;
	aColWidth[2] = 100;
	aColWidth[3] = 140;
}

void CSdisListDlg::SetTitleText()
{
	this->SetWindowText(_LS(IDS_CMD_DEFINE_PROP_SDIS));
}
void CSdisListDlg::OnBtnAdd()
{
	CCMSdisDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();		
}
void CSdisListDlg::OnBtnModify()
{
	T_SDIS_K Key;
	T_SDIS_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDIS));
		return;
	}

	Key = (T_SDIS_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdis(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_SDIS_KEY_NOT_EXIST), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMSdisDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
void CSdisListDlg::OnBtnDel()
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDIS));
		return;
	}
	CString csName = m_List.GetItemText(iItem, 0);
	T_SDIS_K SdisK = m_pDoc->m_pAttrCtrl2->GetSdisKey(csName);
	if (!m_pDoc->m_pDataCtrl->DelSdis(SdisK)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}
void CSdisListDlg::OnBtnCopy()
{
	T_SDIS_K Key;
	T_SDIS_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_CMD_NO_SELECTED_SDIS));
		return;
	}

	Key = (T_SDIS_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdis(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_SDIS_KEY_NOT_EXIST), Key);
		AfxMessageBox(msg);
		return;
	}
	T_SDIS_D CopySdis;
	CopySdis = Data;

	// 새 이름 찾기
	CString strNameTemp;
	int nCopyNameCount = 1;
	while(TRUE)
	{
		strNameTemp.Format(_T("%s-%d"),Data.strName,nCopyNameCount++);
		T_SDIS_K SdisK = m_pDoc->m_pAttrCtrl2->GetSdisKey(strNameTemp);
		if(!m_pDoc->m_pAttrCtrl2->ExistSdis(SdisK))
			break;
	}
	CopySdis.strName = strNameTemp;
	if(!m_pDoc->m_pDataCtrl->AddSdis(CopySdis))	return;

	MakeItemEx();
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}
void CSdisListDlg::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	T_SDIS_K Key;
	T_SDIS_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return

	Key = (T_SDIS_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl2->GetSdis(Key, Data)) return;

	CCMSdisDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	*pResult = 0;
}
void CSdisListDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountSdis();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;  
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SDIS_K Key;
		T_SDIS_D Data;

		nCount = 0;
		CArray<T_SDIS_K, T_SDIS_K> aSdisKeyList;
		aSdisKeyList.RemoveAll();
		m_pDoc->m_pAttrCtrl2->GetSdisKeyList(aSdisKeyList);
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
		{
			Key = aSdisKeyList.GetAt(nCount);
			m_pDoc->m_pAttrCtrl2->GetSdis(Key,Data);
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
void CSdisListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SDIS_K SdisKey, SdisK_bak;
	T_SDIS_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_SDIS_ADD):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			pViewBuff->GetSdis(nKey, SdisKey, Data);
			InsertItem(SdisKey, Data);
			break;
		case(UR_SDIS_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
			pViewBuff->GetSdis(nKey, SdisKey, Data);
			DeleteItem(SdisKey, Data);
			break;
		case(UR_SDIS_MFD):
			// 여기서는 저장만 하고 UR_SDIS_MFS에서 처리한다.
			pViewBuff->GetSdis(nKey, SdisKey, Data);
			SdisK_bak = SdisKey;
			DataBak = Data;
			bMFD = TRUE;
			break;
		case(UR_SDIS_MFS):
			// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
			ASSERT(bMFD);
			pViewBuff->GetSdis(nKey, SdisKey, Data);
			ModifyItem(SdisKey, SdisK_bak, DataBak);
			// 플래그 수정
			bMFD = FALSE;
			break;
		default:
			break;
		}
	} // end of while
}
BOOL CSdisListDlg::InsertItem(T_SDIS_K Key, T_SDIS_D &Data)
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
BOOL CSdisListDlg::DeleteItem(T_SDIS_K Key, T_SDIS_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}
BOOL CSdisListDlg::ModifyItem(T_SDIS_K KeyOld, T_SDIS_K Key, T_SDIS_D &Data)
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
CString CSdisListDlg::DataToStr(int i, T_SDIS_K Key, T_SDIS_D &Data)
{
	CString str;
	CString csDevType[]  = { _LS(IDS_ETC_SDIS_LIST_LRB), _LS(IDS_ETC_SDIS_LIST_NRB), _T(""), _LS(IDS_ETC_SDIS_LIST_SLD) };
	ASSERT(Data.nDeviceType>= D_SDIS_DEV_LRB && Data.nDeviceType <= D_SDIS_DEV_SLD);		
 
	if(i==0) str = Data.strName;
	else if(i==1) str = csDevType[Data.GetDvtIdx()];
	else if(i==2) str.Format(_T("%d"),Data.nNumMSS);
	else if(i==3) str = Data.strDesc;  
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}
void CSdisListDlg::GetHeaderTitleWidth(CString* aTitle, int* aColWidth)
{
	aTitle[0] = _LS(IDS_WG_CMD__ADDD__Name);
	aTitle[1] = _LS(IDS_ETC_SDVP_DEVTYPE)  ;
	aTitle[2] = _LS(IDS_ETC_SDVP_NUMMSS)   ;
	aTitle[3] = _LS(IDS_WG_CMD__ADDD__Description);
	
	aColWidth[0] = 90;
	aColWidth[1] = 180;
	aColWidth[2] = 100;
	aColWidth[3] = 120;
}
