// NLLinkPrtListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "DamperGBListDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "DamperGBItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDamperGBListDlg dialog


CDamperGBListDlg::CDamperGBListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDamperGBListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDamperGBListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bInitSelect = FALSE;
	strInitName = _T("");
}

void CDamperGBListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDamperGBListDlg)
	DDX_Control(pDX, IDC_ETC_ANTISUPPORT_GB_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDamperGBListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDamperGBListDlg)
	ON_BN_CLICKED(IDC_CMD_ETC_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_ETC_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_ETC_BTN_COPY, OnCmdBtnCopy)
	ON_BN_CLICKED(IDC_CMD_ETC_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_ETC_BTN_CLOSE, OnCmdBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_ETC_ANTISUPPORT_GB_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define COLCOUNT 4
/////////////////////////////////////////////////////////////////////////////
// CDamperGBListDlg implementation functions

void CDamperGBListDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_STLD_name), _LS(IDS_ETC_SDVP_DEVTYPE), _LS(IDS_ETC_SDVP_DOF), _LS(IDS_CMD_STLD_description)};
	int nColWidth[COLCOUNT];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 60; nColWidth[1] = 120; nColWidth[2] = 120; nColWidth[3] = 120;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = nColWidth[i];
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CDamperGBListDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountDmgb();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_DMGB_K Key;
		T_DMGB_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartDmgb();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextDmgb(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetDmgb(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

void CDamperGBListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_DMGB_K Key;
	T_DMGB_D Data;
	BOOL bDmgbChanged = FALSE;
 
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_DMGB_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetDmgb(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_DMGB_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetDmgb(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_DMGB_MFD):
		case(UR_DMGB_MFS):
			bDmgbChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while
	if (bDmgbChanged) MakeItemEx();
}

int CDamperGBListDlg::FindInsertionPos(T_DMGB_K Key)
{
	T_DMGB_D asgp;
	m_pDoc->m_pAttrCtrl->GetDmgb(Key, asgp);
	int nID = asgp.nSeq;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		m_pDoc->m_pAttrCtrl->GetDmgb(m_List.GetItemData(i), asgp);
		if (asgp.nSeq > nID) break;
	}
	return i;
}

BOOL CDamperGBListDlg::InsertItem(T_DMGB_K Key, T_DMGB_D &Data)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();

	lvitem.iItem = FindInsertionPos(Key);
	for(int i = 0; i < COLCOUNT; i++)
	{
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

BOOL CDamperGBListDlg::DeleteItem(T_DMGB_K Key, T_DMGB_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CDamperGBListDlg::ModifyItem(T_DMGB_K KeyOld, T_DMGB_K Key, T_DMGB_D &Data)
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

CString CDamperGBListDlg::DataToStr(int i, T_DMGB_K Key, T_DMGB_D &Data)
{
	CString str;
	CString csType[] = {_LS(IDS_WG_CMD__ADDD_Damper_BRB), _LS(IDS_WG_CMD__ADDD_Damper_MYD), _LS(IDS_WG_CMD__ADDD_Damper_FD),
						_LS(IDS_WG_CMD__ADDD_Damper_VD),_LS(IDS_WG_CMD__ADDD_Damper_HDRD),_LS(IDS_WG_CMD__ADDD_Damper_VED) };

	CString strDof[] = { _T("Dx"), _T("Dy"), _T("Dz"), _T("Rx"), _T("Ry"), _T("Rz") };

	if(i==0) str = Data.PropName;
	else if(i==1) str = csType[Data.nPropType];	
	else if (i == 2)
	{
		int nDof = 0;
		switch (Data.nPropType)
		{
		case 0: nDof = Data.BRB.nDof;	break;
		case 1: nDof = Data.MYD.nDof;	break;
		case 2: nDof = Data.FD.nDof;	break;
		case 3: nDof = Data.VFD.nDof;	break;
		case 4: nDof = Data.HDRD.nDof;	break;
		case 5: nDof = Data.VED.nDof;	break;
		default:
			break;
		}
		str = strDof[nDof];
	}
	else if (i == 3) str = Data.Description;
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// CDamperGBListDlg message handlers

BOOL CDamperGBListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	SetWindowText(_LS(IDS_CMD_DMGB_LIST));
	SetHeaderTitle();
	MakeItemEx();

	if (bInitSelect)
		ListSelect();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDamperGBListDlg::ListSelect()
{
	int nSize = m_List.GetItemCount();

	CString str;
	int nIndex;
	for (nIndex=0; nIndex<nSize; nIndex++)
	{
		str = m_List.GetItemText(nIndex, 0);

		if (str.Compare(strInitName) == 0)
			break;
	}
	m_List.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CDamperGBListDlg::OnCmdBtnAdd() 
{
	CDamperGBItemDlg dlg;
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
}

void CDamperGBListDlg::OnCmdBtnModify() 
{
	T_DMGB_K Key;
	T_DMGB_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD_No_Selected_Damper));
		return;
	}
 
	Key = (T_DMGB_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetDmgb(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD_Damper_Key), Key);
		AfxMessageBox(msg);
		return;
	}

	CDamperGBItemDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CDamperGBListDlg::OnCmdBtnDelete() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD_No_Selected_Damper));
		return;
	}
	T_DMGB_K Key = (T_DMGB_K)m_List.GetItemData(iItem);     // asgp의 key를 찾는다.
	T_DMGB_D asgp;
	m_pDoc->m_pAttrCtrl->GetDmgb(Key, asgp);   // asgp의 key를 주고 data를 받는다.
	if (!m_pDoc->m_pDataCtrl->DelDmgb(Key)) return;    // 해당 key의 data를 지운다.

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CDamperGBListDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	T_DMGB_K Key;
	T_DMGB_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = (T_DMGB_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetDmgb(Key, Data)) return;

	CDamperGBItemDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}

void CDamperGBListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDamperGBListDlg::OnCmdBtnClose() 
{
	CDialogMove::OnOK();
}

void CDamperGBListDlg::InitSelect(CString& str)
{
	bInitSelect = TRUE;
	strInitName = str;
}

void CDamperGBListDlg::OnCmdBtnCopy()
{
	// TODO: Add your control notification handler code here
	T_DMGB_K Key;
	T_DMGB_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD_No_Selected_Damper));
		return;
	}

	Key = (T_DMGB_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetDmgb(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_DMGB_KEY_NOT_EXIST), Key);  // _T("Error : General Link Property key %d does not exits!")
		AfxMessageBox(msg);
		return;
	}
	T_DMGB_D CopyDmgb;
	CopyDmgb = Data;

	// 새 이름 찾기
	CString strNameTemp;
	int nCopyNameCount = 1;
	while(TRUE)
	{
		strNameTemp.Format(_T("%s-%d"),Data.PropName,nCopyNameCount++);
		if(!m_pDoc->m_pAttrCtrl->ExistDmgb(strNameTemp))
			break;
	}
	CopyDmgb.PropName = strNameTemp;
	if(!m_pDoc->m_pDataCtrl->AddDmgb(CopyDmgb))	return;

	MakeItemEx();
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}