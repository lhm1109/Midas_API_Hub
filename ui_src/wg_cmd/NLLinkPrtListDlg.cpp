// NLLinkPrtListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "NLLinkPrtListDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "NLLinkPrtItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtListDlg dialog


CNLLinkPrtListDlg::CNLLinkPrtListDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CNLLinkPrtListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNLLinkPrtListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bInitSelect = FALSE;
	strInitName = _T("");
}

void CNLLinkPrtListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNLLinkPrtListDlg)
	DDX_Control(pDX, IDC_CMD_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNLLinkPrtListDlg, CDialogMove)
	//{{AFX_MSG_MAP(CNLLinkPrtListDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_BTN_COPY, OnCmdBtnCopy)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_CLOSE, OnCmdBtnClose)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_LIST, OnDblclkCmdList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define COLCOUNT 4
/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtListDlg implementation functions

void CNLLinkPrtListDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_STLD_name), _LS(IDS_CMD_STLD_APPTYPE),
											_LS(IDS_CMD_STLD_type), _LS(IDS_CMD_STLD_description)};
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
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CNLLinkPrtListDlg::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;
	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountNllp();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_NLLP_K Key;
		T_NLLP_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartNllp();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextNllp(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetNllp(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

void CNLLinkPrtListDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_NLLP_K Key;
	T_NLLP_D Data;
	BOOL bNllpChanged = FALSE;
 
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_NLLP_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetNllp(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_NLLP_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetNllp(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		case(UR_NLLP_MFD):
		case(UR_NLLP_MFS):
			bNllpChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while
	if (bNllpChanged) MakeItemEx();
}

int CNLLinkPrtListDlg::FindInsertionPos(T_NLLP_K Key)
{
	T_NLLP_D nllp;
	m_pDoc->m_pAttrCtrl->GetNllp(Key, nllp);
	int nID = nllp.nSeq;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		m_pDoc->m_pAttrCtrl->GetNllp(m_List.GetItemData(i), nllp);
		if (nllp.nSeq > nID) break;
	}
	return i;
}

BOOL CNLLinkPrtListDlg::InsertItem(T_NLLP_K Key, T_NLLP_D &Data)
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

BOOL CNLLinkPrtListDlg::DeleteItem(T_NLLP_K Key, T_NLLP_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CNLLinkPrtListDlg::ModifyItem(T_NLLP_K KeyOld, T_NLLP_K Key, T_NLLP_D &Data)
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

CString CNLLinkPrtListDlg::DataToStr(int i, T_NLLP_K Key, T_NLLP_D &Data)
{
	CString str;
	CString csType[] = {_LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop1),  // Visco-Elastic Damper
										 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop2),   // Gap
										 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop3),   // Hook
										 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop4),   // Hysteretic System
										 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop5),   // Lead Rubber Bearing Isolator
										 _LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop6),  // Friction Pendulum System Isolator
										_LS(IDS_WG_CMD__ADD2_Nonlinear_Link_Prop12), // Triple Friction Pendulum System Isolator 
										 _STR(_T("Double Friction Pendulum System Isolator"))}; // Double Friction Pendulum System Isolator
	CString csType2[] = {_LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop1),   // Spring
											 _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop2),   // Linear Dashpot
											 _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop3),   // Spring and Linear Dashpot
											 _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop4),   // Spring and Nonlinear Dashpot 1
											 _LS(IDS_WG_CMD__ADD2_Nonlinear_ELEMENT_Link_Prop5)};  // Spring and Nonlinear Dashpot 2
	
	CString csType3[] = {_LS(IDS_CMD_SEIS_PROP_SDVI),   // _T("Viscous Damper / Oil Damper Properties")
											 _LS(IDS_CMD_SEIS_PROP_SDVE),   // _T("Viscoelastic Damper Properties")
											 _LS(IDS_CMD_SEIS_PROP_SDST),   // _T("Steel Damper Properties")
											 _LS(IDS_CMD_SEIS_PROP_SDHY),   // _T("Hysteretic Isolator(MSS) Properties")
											 _LS(IDS_CMD_SEIS_PROP_SDIS),  // _T("Isolator(MSS) Properties")
											_LS(IDS_CMD_SEIS_PROP_ASGB),	// _T("Isolator(GB) Properties")
											_LS(IDS_CMD_SEIS_PROP_DMGB) };  // _T("Damper(GB) Properties")

	CString csAppType[] = {_LS(IDS_CMD0417__Element), _LS(IDS_CMD0417__Force), _LS(IDS_CMD0417__Element_TWO) };   // {_T("Element"), _T("Force"), _T("Element Ⅱ")};
	ASSERT(Data.nType >= 0 && Data.nType <= 7);		

	if(i==0) str = Data.PropName;
	else if(i==1) str = csAppType[Data.nApplicationType];
	else if(i==2) 
	{
		if (Data.nApplicationType == 0)
			str = csType2[Data.nElemType];
		else if(Data.nApplicationType == 1)
			str = csType[Data.nType];
		else if(Data.nApplicationType == 2)
			str = csType3[Data.nSeisType];
		else ASSERT(0);
	}
	else if(i==3) str = Data.Description;
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtListDlg message handlers

BOOL CNLLinkPrtListDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	SetHeaderTitle();
	MakeItemEx();

	if (bInitSelect)
		ListSelect();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNLLinkPrtListDlg::ListSelect()
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

void CNLLinkPrtListDlg::OnCmdBtnAdd() 
{
	CNLLinkPrtItemDlg dlg;
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();
}

void CNLLinkPrtListDlg::OnCmdBtnModify() 
{
	T_NLLP_K Key;
	T_NLLP_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD_No_Selected_Nonlinear_Property));
		return;
	}
 
	Key = (T_NLLP_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetNllp(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD_Nonlinear_Property_Key), Key);
		AfxMessageBox(msg);
		return;
	}

	CNLLinkPrtItemDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CNLLinkPrtListDlg::OnCmdBtnDelete() 
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD_No_Selected_Nonlinear_Property));
		return;
	}
	T_NLLP_K Key = (T_NLLP_K)m_List.GetItemData(iItem);     // nllp의 key를 찾는다.
	T_NLLP_D nllp;
	m_pDoc->m_pAttrCtrl->GetNllp(Key, nllp);   // nllp의 key를 주고 data를 받는다.
	if (!m_pDoc->m_pDataCtrl->DelNllp(nllp.PropName)) return;    // 해당 key의 data를 지운다.

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CNLLinkPrtListDlg::OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	T_NLLP_K Key;
	T_NLLP_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = (T_NLLP_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetNllp(Key, Data)) return;

	CNLLinkPrtItemDlg dlg;
	dlg.SetParamData(Key, Data);
	dlg.SetInitPos(D_INIT_POS_LB);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}

void CNLLinkPrtListDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CNLLinkPrtListDlg::OnCmdBtnClose() 
{
	CDialogMove::OnOK();
}

void CNLLinkPrtListDlg::InitSelect(CString& str)
{
	bInitSelect = TRUE;
	strInitName = str;
}

void CNLLinkPrtListDlg::OnCmdBtnCopy()
{
	// TODO: Add your control notification handler code here
	T_NLLP_K Key;
	T_NLLP_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD_No_Selected_Nonlinear_Property));
		return;
	}

	Key = (T_NLLP_K)m_List.GetItemData(iItem);
	if (!m_pDoc->m_pAttrCtrl->GetNllp(Key, Data)) 
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_NLLP_KEY_NOT_EXIST), Key);  // _T("Error : General Link Property key %d does not exits!")
		AfxMessageBox(msg);
		return;
	}
	T_NLLP_D CopyNllp;
	CopyNllp = Data;

	// 새 이름 찾기
	CString strNameTemp;
	int nCopyNameCount = 1;
	while(TRUE)
	{
		strNameTemp.Format(_T("%s-%d"),Data.PropName,nCopyNameCount++);
		if(!m_pDoc->m_pAttrCtrl->ExistNllp(strNameTemp))
			break;
	}
	CopyNllp.PropName = strNameTemp;
	if(!m_pDoc->m_pDataCtrl->AddNllp(CopyNllp))	return;

	MakeItemEx();
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}