// CMThicknessPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThicknessPage.h"

#include "..\wg_base\wg_base_Product.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "CMThkPageItemDlg.h"
#include "CMImportPropertyDlg.h"
#include "CMPropRenumDlg.h"
#include "CMPropertiesDlg.h"

#define COLCOUNT 5
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThicknessPage dialog


CCMThicknessPage::CCMThicknessPage(CDBDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CCMThicknessPage_base(CCMThicknessPage::IDD, pParent)
{
	m_pDoc = pDoc;
	m_pParent = (CCMPropertiesDlg*) pParent;
	//{{AFX_DATA_INIT(CCMThicknessPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMThicknessPage::DoDataExchange(CDataExchange* pDX)
{
	CCMThicknessPage_base::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThicknessPage)
	DDX_Control(pDX, IDC_CMD_TP_THK_LIST, m_List);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMThicknessPage implementation functions
void CCMThicknessPage::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	int aUnitIndex[] = { 0, 0, 0, D_UNITSYS_BASE_LENGTH, 0};
	CString csUnit;
	CString aTitle[] = { _LS(IDS_CMD_THICKNESS_id), _LS(IDS_CMD_THICKNESS_name), _LS(IDS_CMD_THICKNESS_type),
		_LS(IDS_CMD_THICKNESS_thickness), _LS(IDS_CMD_OFFSET) };
#ifdef _MGEN
	int nColWidth[] = {40,70,80,94,60};
#else
	int nColWidth[] = {40,0,80,100,60};
#endif

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

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		CUnitCtrl::GetUnitSystem(aUnitIndex[i], csUnit);

		if (csUnit != _T(""))
			title += CString(_T("("))+csUnit+CString(_T(")"));
		// 제일 처음 시작일 때면 Column 추가
		// 아니면 Column Title만 변경
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		if (bInit)
			m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMThicknessPage::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	if(!bUnitChanged)
		m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountThik();
	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_THIK_K Key;
		T_THIK_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartThik();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextThik(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_THIK_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetThik(Key,Data);

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

void CCMThicknessPage::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_THIK_K    Key, KeyBak;
	T_THIK_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_THIK_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetThik(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_THIK_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetThik(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_THIK_MFD):
				// 여기서는 저장만 하고 UR_THIK_MFS에서 처리한다.
				pViewBuff->GetThik(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_THIK_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetThik(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}

void CCMThicknessPage::UnitChanged()
{
	T_UNIT_SYSTEM UnitSystem;
	//m_pDoc->m_pUnitCtrl->GetUnitSystemThik(UnitSystem);
	SetHeaderTitle(UnitSystem);
	MakeItemEx(TRUE);
}


BOOL CCMThicknessPage::InsertItem(T_THIK_K Key, T_THIK_D &Data)
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

BOOL CCMThicknessPage::DeleteItem(T_THIK_K Key, T_THIK_D &Data)
{
	CString str;
	str.Format(_T("%4d"), Key);

	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_STRING;
	FindInfo.psz = str.GetBuffer(0);
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	str.ReleaseBuffer();
	return TRUE;
}

BOOL CCMThicknessPage::ModifyItem(T_THIK_K KeyOld, T_THIK_K Key, T_THIK_D &Data)
{
	CString str;
	str.Format(_T("%4d"), KeyOld);

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

CString CCMThicknessPage::DataToStr(int i, T_THIK_K Key, T_THIK_D &Data)
{
	CString str;

	if(i==0)str.Format(_T("%4d"),Key);
	else if(i==1)
	{
		str = Data.Value.strName;
	}
	else if(i==2)
	{
		if (Data.nType == 0) str = _LS(IDS_WG_CMD__ADDD__Value);
		else  if (Data.nType == 1) str = _LS(IDS_WG_CMD__ADDD__Stiffened);
		else str = _LS(IDS_WG_CMD__ADDD__StlWall);
	}
	else if(i==3)
	{
		double dThik;
		if (Data.nType == 2) 
		{
			str.Format(_T("%f(%f)"), Data.StlWall.ThickCon,Data.StlWall.ThickSteel);
		}
		else
		{
			m_pDoc->m_pAttrCtrl->GetThikForThikName(Key, dThik);
			str.Format(_T("%f"), dThik);
		}
	}
	else if(i==4)
	{
		if(Data.nType==0)
		{
			str = (Data.Value.bOffset) ? _LS(IDS_CMD_YES) : _LS(IDS_CMD_NO);
		}
		else str = _LS(IDS_CMD_NO);
	}

	return str;
}

#define CDialog CCMThicknessPage_base
BEGIN_MESSAGE_MAP(CCMThicknessPage, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CCMThicknessPage)
	ON_BN_CLICKED(IDC_CMD_TP_BTN_ADD, OnAdd)
	ON_BN_CLICKED(IDC_CMD_TP_BTN_MOD, OnModify)
	ON_BN_CLICKED(IDC_CMD_TP_BTN_DEL, OnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_TP_THK_LIST, OnDblClickListItem)
	ON_BN_CLICKED(IDC_CMD_TP_BTN_COPY, OnCopy)
	ON_BN_CLICKED(IDC_CMD_MP_BTN_IMPORT, OnCmdMpBtnImport)
	ON_BN_CLICKED(IDC_CMD_TP_BTN_RENUM, OnCmdBtnRenum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThicknessPage message handlers

BOOL CCMThicknessPage::OnInitDialog() 
{
	CCMThicknessPage_base::OnInitDialog();
	
	// TODO: Add extra initialization here
	T_UNIT_SYSTEM UnitSystem;
	//m_pDoc->m_pUnitCtrl->GetUnitSystemThik(UnitSystem);
	SetHeaderTitle(UnitSystem, TRUE);
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThicknessPage::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMThkPageItemDlg dlg(m_pDoc);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.DoModal();
}

void CCMThicknessPage::OnModify() 
{
	// TODO: Add your control notification handler code here
	T_THIK_K Key;
	T_THIK_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_thickness__nChoose_on));
		return;
	}
 
	Key = _tstol(m_List.GetItemText(iItem, 0));
	if (!m_pDoc->m_pAttrCtrl->GetThik(Key, Data)) return;

	CCMThkPageItemDlg dlg(m_pDoc);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetModifyData(Key, Data);
	dlg.DoModal();	
}

void CCMThicknessPage::OnDelete() 
{
	// TODO: Add your control notification handler code here
	T_THIK_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_thickness__nChoose_on));
		return;
	}
	Key = _tstol(m_List.GetItemText(iItem, 0));
	m_pDoc->m_pDataCtrl->DelThik(Key);

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMThicknessPage::OnCopy() 
{
	// TODO: Add your control notification handler code here
	T_THIK_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_thickness__nChoose_on));
		return;
	}
	Key = _tstol(m_List.GetItemText(iItem, 0));
	m_pDoc->m_pDataCtrl->CopyThik(Key);	
}

void CCMThicknessPage::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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
	default:
		//ASSERT(FALSE);
		break;
	}
}


void CCMThicknessPage::OnDblClickListItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_THIK_K Key;
	T_THIK_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = _tstol(m_List.GetItemText(iItem, 0));
	if (!m_pDoc->m_pAttrCtrl->GetThik(Key, Data)) return;

	CCMThkPageItemDlg dlg(m_pDoc);
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetModifyData(Key, Data);
	dlg.DoModal();	
	
	*pResult = 0;
}

void CCMThicknessPage::OnCmdMpBtnImport() 
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
		dlg.InitPropertyData(csFileName, 2, bIsMgb);
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.DoModal();
	}
}

void CCMThicknessPage::OnCmdBtnRenum() 
{
	CCMPropRenumDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.m_nPropNum = 2;
	dlg.DoModal();
}

void CCMThicknessPage::OnActivatePage()
{
	//if(m_pParent) m_pParent->SetShowTip(CString(_T("")));
}