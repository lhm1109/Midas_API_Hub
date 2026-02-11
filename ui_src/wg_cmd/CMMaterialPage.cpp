// CMMaterialPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMaterialPage.h"

#include "..\wg_base\wg_base_Product.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"

#include "CMImportPropertyDlg.h"
#include "CMPropRenumDlg.h"
#include "CMPropertiesDlg.h"

#include "..\mit_frx\global_Opt4Perf.h"

#ifdef _MVIL
	#include "CMMatPageItemDlg2.h"
	#define CCMMATPAGEDLGCLASS CCMMatPageItemDlg2
#else
	#include "CMMatPageItemDlg.h"
	#define CCMMATPAGEDLGCLASS CCMMatPageItemDlg
#endif

#define COLCOUNT 5

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMaterialPage dialog


CCMMaterialPage::CCMMaterialPage(CDBDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CCMMaterialPage_base(CCMMaterialPage::IDD, pParent)
{
	m_pDoc = pDoc;
	m_pParent = (CCMPropertiesDlg*) pParent;
	//{{AFX_DATA_INIT(CCMMaterialPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMMaterialPage::DoDataExchange(CDataExchange* pDX)
{
	CCMMaterialPage_base::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMaterialPage)
	DDX_Control(pDX, IDC_CMD_MP_MAT_LIST, m_List);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMMaterialPage implementation functions
void CCMMaterialPage::SetHeaderTitle(BOOL bInit)
{
	
	CString aTitle[] = { _LS(IDS_CMD_MATERIAL_id), _LS(IDS_CMD_MATERIAL_name), _LS(IDS_CMD_MATERIAL_type),
											_LS(IDS_CMD_MATERIAL_standard), _LS(IDS_CMD_MATERIAL_db) };
	
	
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
	nColWidth[0] = 50; nColWidth[1] = 50; nColWidth[2] = 60;
	nColWidth[3] = 70; nColWidth[4] = 50;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		if (i == 0) lvcolumn.fmt = LVCFMT_RIGHT;
		else lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		if (bInit)
			m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMaterialPage::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	if(!bUnitChanged)
		m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountMatl();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_MATL_K Key;
		T_MATL_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartMatl();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextMatl(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_MATL_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetMatl(Key,Data);
			for(int i = 0; i < COLCOUNT; i++)
			{
				lvitem.iItem=nCount;
				lvitem.iSubItem=i;
				str = DataToStr(i, Key, Data);

				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT;
				
			  if(!bUnitChanged)
			  {
				  if( i == 0 )
					  m_List.InsertItem(&lvitem);
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

void CCMMaterialPage::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_MATL_K    Key, KeyBak;
	T_MATL_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_MATL_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetMatl(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_MATL_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetMatl(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_MATL_MFD):
				// 여기서는 저장만 하고 UR_MATL_MFS에서 처리한다.
				pViewBuff->GetMatl(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_MATL_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetMatl(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

BOOL CCMMaterialPage::InsertItem(T_MATL_K Key, T_MATL_D &Data)
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

		if( i == 0 ) nItem = m_List.InsertItem(&lvitem);
		else m_List.SetItem(&lvitem);

	  str.ReleaseBuffer();
	}

	// [GEN-1808] 업데이트 후 리스트 스크롤 유지
	m_List.EnsureVisible(m_List.GetItemCount() - 1, TRUE); // Scroll down to the bottom
	m_List.EnsureVisible(nItem, TRUE);// scroll back up just enough to show said item on top

	return TRUE;
}

BOOL CCMMaterialPage::DeleteItem(T_MATL_K Key, T_MATL_D &Data)
{
	CString str;
	str.Format(_T("%6d"), Key);

	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_STRING;
	FindInfo.psz = str.GetBuffer(0);
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	str.ReleaseBuffer();
	return TRUE;
}

BOOL CCMMaterialPage::ModifyItem(T_MATL_K KeyOld, T_MATL_K Key, T_MATL_D &Data)
{
	CString str;
	str.Format(_T("%6d"), KeyOld);

	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_STRING;
	FindInfo.psz = str.GetBuffer(0);
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Key, Data);
			m_List.SetItemText(nItem, i, str);
		}
	}
	str.ReleaseBuffer();
	return TRUE;
}

CString CCMMaterialPage::DataToStr(int i, T_MATL_K Key, T_MATL_D &Data)
{
	CString str;
	if(i==0)str.Format(_T("%6d"),Key);
	else if(i==1)str = Data.Name;
	else if(i==2)
	{
		if (Data.Type == _T("C")) str = _LS(IDS_WG_CMD__ADDD__Concrete);
		else if (Data.Type == _T("S")) str = _LS(IDS_WG_CMD__ADDD__Steel);
		else if (Data.Type == _T("U")) str = _LS(IDS_WG_CMD__ADD2__User_Def_);
		else if (Data.Type == _LS(IDS_WG_CMD__ADDD_MATL_SRC)) str = _LS(IDS_WG_CMD__ADDD_MATL_SRC);
		else if (Data.Type == _T("A")) str = _LS(IDS_WG_CMD__ADDD__Aluminium);
	}
	else if(i==3)
	{
		if (Data.Type == _LS(IDS_WG_CMD__ADDD_MATL_SRC))
		{
			if (Data.bChangeSteel) str = Data.Data1.CodeName;
			else str = Data.Data2.CodeName;
		}
		else str = Data.Data1.CodeName;

		m_pDoc->m_pEditData->ConvertEngMatlToRusMatl(str);
	}
	else if(i==4)
	{
		if (Data.Type == _LS(IDS_WG_CMD__ADDD_MATL_SRC))
		{
			if (Data.bChangeSteel)
			{
				str = Data.Data1.CodeMatlName;
				m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(Data.Data1.CodeName, str);
			}
			else
			{
				str = Data.Data2.CodeMatlName;
				m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(Data.Data2.CodeName, str);
			}
		}
		else
		{
			str = Data.Data1.CodeMatlName;
			m_pDoc->m_pEditData->ConvertEngMatlToRusMatlDBName(Data.Data1.CodeName, str);
		}
	}
	return str;
}


#define CDialog CCMMaterialPage_base
BEGIN_MESSAGE_MAP(CCMMaterialPage, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CCMMaterialPage)
	ON_BN_CLICKED(IDC_CMD_MP_BTN_ADD, OnAdd)
	ON_BN_CLICKED(IDC_CMD_MP_BTN_MOD, OnModify)
	ON_BN_CLICKED(IDC_CMD_MP_BTN_DEL, OnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_MP_MAT_LIST, OnDblClickListItem)
	ON_BN_CLICKED(IDC_CMD_MP_BTN_COPY, OnCopy)
	ON_BN_CLICKED(IDC_CMD_MP_BTN_IMPORT, OnCmdMpBtnImport)
	ON_BN_CLICKED(IDC_CMD_MP_BTN_RENUM, OnCmdMpBtnRenum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMaterialPage message handlers

BOOL CCMMaterialPage::OnInitDialog() 
{
	gGuardSetRedraw_Matl gsRedraw(this);

	CCMMaterialPage_base::OnInitDialog();
	
	// TODO: Add extra initialization here
	// Make Column Header
	SetHeaderTitle(TRUE);
	MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMaterialPage::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMMATPAGEDLGCLASS dlg(m_pDoc);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMMaterialPage::OnModify() 
{
	// TODO: Add your control notification handler code here
	T_MATL_K Key;
	T_MATL_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_material__nChoose_one));
		return;
	}
 
	Key = _tstol(m_List.GetItemText(iItem, 0));
	if (!m_pDoc->m_pAttrCtrl->GetMatl(Key, Data)) return;

	CCMMATPAGEDLGCLASS dlg(m_pDoc);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetModifyData(Key, Data);
	dlg.DoModal();	
}

void CCMMaterialPage::OnDelete() 
{
	// TODO: Add your control notification handler code here
	T_MATL_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_material__nChoose_one));
		return;
	}
	Key = _tstol(m_List.GetItemText(iItem, 0));
	m_pDoc->m_pDataCtrl->DelMatl(Key);

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMMaterialPage::OnCopy() 
{
	// TODO: Add your control notification handler code here
	T_MATL_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_material__nChoose_one));
		return;
	}
	Key = _tstol(m_List.GetItemText(iItem, 0));
	m_pDoc->m_pDataCtrl->CopyMatl(Key);
}

void CCMMaterialPage::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMMaterialPage::OnDblClickListItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_MATL_K Key;
	T_MATL_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return;  // no message and just return
 
	Key = _tstol(m_List.GetItemText(iItem, 0));
	if (!m_pDoc->m_pAttrCtrl->GetMatl(Key, Data)) return;

	CCMMATPAGEDLGCLASS dlg(m_pDoc);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetModifyData(Key, Data);
	dlg.DoModal();	

	*pResult = 0;
}

void CCMMaterialPage::OnCmdMpBtnImport() 
{
#if defined(_MGEN_ORG) || defined(_MGEN_US)
	CFileDialog fd(TRUE, D_MODEL_FILE_EXT, NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES,
		D_MODEL_FILE_FILTER_EX, NULL);
#else
	CFileDialog fd(TRUE, D_MODEL_FILE_EXT, NULL, 
								 OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
								 D_MODEL_FILE_FILTER, NULL);
#endif
	if (fd.DoModal() == IDOK)
	{
		CString csFileName = fd.GetPathName();
		CString strMgbExt = _T(".") + fd.GetFileExt();
		BOOL bIsMgb = _T(".mgb") == strMgbExt;
		CCMImportPropertyDlg dlg;
		dlg.InitPropertyData(csFileName, 0, bIsMgb);
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.DoModal();
	}		
}

//Add by KJH20021030
void CCMMaterialPage::OnCmdMpBtnRenum() 
{
	CCMPropRenumDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.m_nPropNum = 0;
	dlg.DoModal();
}

void CCMMaterialPage::OnActivatePage()
{
	//if(m_pParent) m_pParent->SetShowTip(CString(_T("")));
}
