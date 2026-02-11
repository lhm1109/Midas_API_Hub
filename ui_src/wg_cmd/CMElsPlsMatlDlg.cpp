// CMElsPlsMatlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMElsPlsMatlDlg.h"
#include "CMElsplsMatlAddDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMatlDlg dialog


CCMElsPlsMatlDlg::CCMElsPlsMatlDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMElsPlsMatlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMElsPlsMatlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMElsPlsMatlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMElsPlsMatlDlg)
	DDX_Control(pDX, IDC_ELSPLS_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMElsPlsMatlDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMElsPlsMatlDlg)
	ON_BN_CLICKED(IDC_ELSPLS_ADD, OnElsplsAdd)
	ON_BN_CLICKED(IDC_ELSPLS_MOD, OnElsplsMod)
	ON_BN_CLICKED(IDC_ELSPLS_DEL, OnElsplsDel)
	ON_BN_CLICKED(IDC_ELSPLS_CLOSE, OnElsplsClose)
	ON_NOTIFY(NM_DBLCLK, IDC_ELSPLS_LIST, OnDblclkElsplsList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define COLCOUNT 2

/////////////////////////////////////////////////////////////////////////////
void CCMElsPlsMatlDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CCMElsPlsMatlDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_EPMT_K Key, KeyBak;
	T_EPMT_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_EPMT_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetEpmt(nKey, Key, Data);
				InsertItem(Key, Data);
				break;
		case(UR_EPMT_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetEpmt(nKey, Key, Data);
				DeleteItem(Key, Data);
				break;
		case(UR_EPMT_MFD):
				// 여기서는 저장만 하고 UR_EPMT_MFS에서 처리한다.
				pViewBuff->GetEpmt(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_EPMT_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetEpmt(nKey, Key, Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while
}
void CCMElsPlsMatlDlg::SetHeaderTitle(BOOL bInit)
{
	CString aTitle[] = { _LS(IDS_CMD_MATERIAL_name), _LS(IDS_WG_CMD__ADDD__Model) };

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
	nColWidth[0] = 130;
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
void CCMElsPlsMatlDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountEpmt();
	if(nItemCount<1) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_EPMT_K Key;
		T_EPMT_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartEpmt();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextEpmt(pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_EPMT_K), CCompFunc::UINTAsc);

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key = KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetEpmt(Key, Data);
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
BOOL CCMElsPlsMatlDlg::InsertItem(T_EPMT_K Key, T_EPMT_D &Data)
{
	LVITEM lvitem;
	CString str;

	int nItem = FindInsertionPos(Key);  
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
int CCMElsPlsMatlDlg::FindInsertionPos(T_EPMT_K Key)
{
	T_EPMT_D data;
	m_pDoc->m_pAttrCtrl->GetEpmt(Key, data);
	int nID = data.nSeq;
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		m_pDoc->m_pAttrCtrl->GetEpmt(m_List.GetItemData(i), data);
		if (data.nSeq > nID) break;
	}
	return i;
}
BOOL CCMElsPlsMatlDlg::DeleteItem(T_EPMT_K Key, T_EPMT_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);

	return TRUE;
}
BOOL CCMElsPlsMatlDlg::ModifyItem(T_EPMT_K KeyOld, T_EPMT_K Key, T_EPMT_D &Data)
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
CString CCMElsPlsMatlDlg::DataToStr(int i, T_EPMT_K Key, T_EPMT_D &Data)
{
	CString str; str.Empty();
	CString csFuncType[] = {_LS(IDS_CMD_ELSPLS_TRESCA),_LS(IDS_CMD_ELSPLS_VON),
													_LS(IDS_CMD_ELSPLS_MOHR),_LS(IDS_CMD_ELSPLS_DRUCKER),
													_LS(IDS_CMD_ELSPLS_MICRO),_LS(IDS_CMD_ELSPLS_MASONRY),
													_LS(IDS_CMD_ELSPLS_CONCDMG)};

	if(i==0)      str = Data.strMatlName;
	else if(i==1) str = csFuncType[Data.nModelType];
	else ASSERT(0);

	return str;
}
/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMatlDlg message handlers

BOOL CCMElsPlsMatlDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetHeaderTitle(TRUE);
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMElsPlsMatlDlg::OnElsplsAdd() 
{
	CCMElsPlsMatlAddDlg dlg;
	dlg.DoModal();
}

void CCMElsPlsMatlDlg::OnElsplsMod() 
{
	T_EPMT_K Key;
	T_EPMT_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_material_type__nChoos));
		return;
	}

	Key = (T_EPMT_K)m_List.GetItemData(iItem);
	if(!m_pDoc->m_pAttrCtrl->GetEpmt(Key, Data))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Material_type_key__d_does_not_exi), Key);
		AfxMessageBox(msg);
		return;
	}

	CCMElsPlsMatlAddDlg dlg;
	dlg.SetElsPlsData(Key, Data);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CCMElsPlsMatlDlg::OnElsplsDel() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_Material_type__nChoos));
		return;
	}

	CString csName = m_List.GetItemText(iItem, 0);
	if (!m_pDoc->m_pDataCtrl->DelEpmt(csName)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMElsPlsMatlDlg::OnElsplsClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CCMElsPlsMatlDlg::OnDblclkElsplsList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnElsplsMod();
	*pResult = 0;
}

