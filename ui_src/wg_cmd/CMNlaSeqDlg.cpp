// CMNlaSeqDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMNlaSeqDlg.h"
#include "CMNlacDlg.h"
#include "CMNlacDlg_MEC.h"
#include "ExportFunc.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_dbLock\LockOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMNlaSeqDlg dialog

CCMNlaSeqDlg::CCMNlaSeqDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMNlaSeqDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMNlaSeqDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMNlaSeqDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMNlaSeqDlg)
	DDX_Control(pDX, IDC_CMD_NLASEQ_RLIST, m_RightList);
	DDX_Control(pDX, IDC_CMD_NLASEQ_LLIST, m_LeftList);
	//}}AFX_DATA_MAP
}

void CCMNlaSeqDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == nullptr || !IsWindow(hWnd)) return;

	switch (lHint)
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

void CCMNlaSeqDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K Key;
	T_STLD_D Data, DataBak;

	BOOL bUpdate = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_STLD_ADD):
			// do nothing
			break;
		case(UR_STLD_DEL):
			pViewBuff->GetStld(nKey, Key, Data);
			for (int i = m_aSeqData.GetSize() - 1; i >= 0; i--)
			{
				if (m_aSeqData[i].stldKey == Key)
				{
					m_aSeqData.RemoveAt(i);
				}
			}
			for (int i = m_aLcData.GetSize() - 1; i >= 0; i--)
			{
				if (m_aLcData[i].stldKey == Key)
				{
					m_aLcData.RemoveAt(i);
				}
			}
			bUpdate = TRUE;
			break;
		case(UR_STLD_MFD):			
			bUpdate = TRUE;
			break;
		case(UR_STLD_MFS):
			bUpdate = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if(bUpdate)
	{
		InitSeqDataList();

		MakeLeftItemEx();
		MakeRightItemEx();
	}
}


BEGIN_MESSAGE_MAP(CCMNlaSeqDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMNlaSeqDlg)
	ON_BN_CLICKED(IDC_CMD_NLASEQ_NLAC_BTN, OnCmdNlaseqNlacBtn)
	ON_BN_CLICKED(IDC_CMD_NLASEQ_LEFT_BT, OnCmdNlaseqLeftBt)
	ON_BN_CLICKED(IDC_CMD_NLASEQ_RIGHT_BT, OnCmdNlaseqRightBt)
	ON_BN_CLICKED(IDC_CMD_NLASEQ_RLIST_UP, OnCmdNlaseqRlistUp)
	ON_BN_CLICKED(IDC_CMD_NLASEQ_RLIST_DN, OnCmdNlaseqRlistDn)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_NLASEQ_LLIST, OnDblclkCmdNlaseqLlist)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_NLASEQ_RLIST, OnDblclkCmdNlaseqRlist)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMNlaSeqDlg message handlers

BOOL CCMNlaSeqDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	
	// TODO: Add extra initialization here
	SetLeftHeaderTitle();
	SetRightHeaderTitle();

	InitSeqDataList();

	MakeLeftItemEx();
	MakeRightItemEx();

#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_FEM_NL_ANAL))
	{
		GetDlgItem(IDC_CMD_NLASEQ_NLAC_BTN)->EnableWindow(FALSE);
	}
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMNlaSeqDlg::OnCmdNlaseqNlacBtn() 
{
	// TODO: Add your control notification handler code here
#if defined(_MEC)
	CreateOrActivateDlg(m_pDoc, CCMNlacDlg_MEC::IDD);
#else
	CreateOrActivateDlg(m_pDoc, CCMNlacDlg::IDD);
#endif
}

void CCMNlaSeqDlg::OnCmdNlaseqLeftBt() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_RightList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected__nChoose_one_));
		return;
	}

	seqdata lcd;
	lcd.stldKey  = m_aSeqData[iItem].stldKey;
	lcd.stldName = m_aSeqData[iItem].stldName;
	m_aLcData.Add(lcd);
	m_aSeqData.RemoveAt(iItem);

	MakeRightItemEx();
	MakeLeftItemEx();

	int nCount = m_RightList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem < 0) iItem = 0;

	m_RightList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMNlaSeqDlg::OnCmdNlaseqRightBt() 
{
	// TODO: Add your control notification handler code here
//	T_STLD_K Key;
//	T_STLD_D Data;

	int iItem = m_LeftList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_static_load_c_MSG3));
		return;
	}
 
//  Key = (T_STLD_K)m_LeftList.GetItemData(iItem);
//  if (!m_pDoc->m_pAttrCtrl->GetStld(Key, Data)) return;

	seqdata seqd;
	seqd.stldKey	= m_aLcData[iItem].stldKey;
	seqd.stldName = m_aLcData[iItem].stldName;

	if(CheckLDSQData(seqd.stldKey))
	{
		m_aSeqData.Add(seqd);
		m_aLcData.RemoveAt(iItem);

		MakeRightItemEx();
		MakeLeftItemEx();
	}

	int nCount = m_LeftList.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem < 0) iItem = 0;

	m_LeftList.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMNlaSeqDlg::OnOK() 
{
	// TODO: Add extra validation here
	T_LDSQ_D data;
	data.Initialize();

	int nNum = m_aSeqData.GetSize();
	for(int i=0; i < nNum; i++)
	{
		data.aLoadingSequence.Add(m_aSeqData[i].stldKey);
	}

	if(nNum == 0) 
	{
		BOOL bExist = m_pDoc->m_pAttrCtrl->ExistLdsq();
		if(bExist) m_pDoc->m_pDataCtrl->DelLdsq();
	}
	else if(!m_pDoc->m_pDataCtrl->AddLdsq(data)) return;

	CCMDlgBase::OnOK();
}

void CCMNlaSeqDlg::OnCmdNlaseqRlistUp() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_RightList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected__nChoose_one_));
		return;
	}
	if (iItem == 0) return;

	int			tempkey		= m_aSeqData[iItem-1].stldKey;
	CString tempname	= m_aSeqData[iItem-1].stldName;
	m_aSeqData[iItem-1].stldKey		= m_aSeqData[iItem].stldKey;
	m_aSeqData[iItem-1].stldName	= m_aSeqData[iItem].stldName;
	m_aSeqData[iItem].stldKey		= tempkey;
	m_aSeqData[iItem].stldName	= tempname;

	MakeRightItemEx();
	m_RightList.SetItemState(iItem-1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMNlaSeqDlg::OnCmdNlaseqRlistDn() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_RightList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected__nChoose_one_));
		return;
	}
	if (iItem == m_RightList.GetItemCount()-1) return;

	int			tempkey		= m_aSeqData[iItem+1].stldKey;
	CString tempname	= m_aSeqData[iItem+1].stldName;
	m_aSeqData[iItem+1].stldKey		= m_aSeqData[iItem].stldKey;
	m_aSeqData[iItem+1].stldName	= m_aSeqData[iItem].stldName;
	m_aSeqData[iItem].stldKey		= tempkey;
	m_aSeqData[iItem].stldName	= tempname;

	MakeRightItemEx();
	m_RightList.SetItemState(iItem+1, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMNlaSeqDlg::InitSeqDataList()
{
	InitRightItem();
	InitLeftItem();
}
///////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////
// static loadcase list
void CCMNlaSeqDlg::InitLeftItem()
{
	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountStld();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;

	if(nItemCount)
	{
		T_STLD_K Key;
		T_STLD_D Data;
		pos = m_pDoc->m_pAttrCtrl->GetStartStld();
		while(pos !=NULL)
		{
			seqdata lcd;
		  m_pDoc->m_pAttrCtrl->GetNextStld(pos, Key, Data);
			if(!CheckLDSQData(Key)) continue;
			if(Data.LoadCaseType == _T("CS") || Data.LoadCaseType == _T("TCS")) continue;
			lcd.stldKey = Key;
			lcd.stldName = Data.LoadCaseName;
			m_aLcData.Add(lcd);
		}
	}
}

void CCMNlaSeqDlg::SetLeftHeaderTitle()
{
	CString aTitle = _LS(IDS_WG_CMD__ADDD__Load_Case_Name);
	int nColWidth;

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_LeftList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_LeftList.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth = 145;

	// Set Title
	title = aTitle;
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	lvcolumn.iSubItem = 0;
	lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth);
	lvcolumn.pszText = title.GetBuffer(0);
	m_LeftList.InsertColumn(0, &lvcolumn);
	title.ReleaseBuffer();
}

void CCMNlaSeqDlg::MakeLeftItemEx()
{
	m_LeftList.DeleteAllItems();

	int nItemCount = m_aLcData.GetSize();
	if(nItemCount == 0) return;

 	for(int nCount=0; nCount < nItemCount; nCount++)
	{
//		InsertItemLeft(m_aLcData[nCount].stldKey, m_aLcData[nCount].stldName);
		LVITEM lvitem;
		CString str;
		int nItem = m_LeftList.GetItemCount();

		lvitem.iItem = nCount;

		lvitem.iSubItem = 0;
		str = m_aLcData[nCount].stldName;
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		lvitem.mask |= LVIF_PARAM;
		lvitem.lParam = (LPARAM)m_aLcData[nCount].stldKey;
		nItem = m_LeftList.InsertItem(&lvitem);
		str.ReleaseBuffer();
	}
}

BOOL CCMNlaSeqDlg::InsertItemLeft(T_STLD_K Key, CString strLcName)
{
	LVITEM lvitem;
	CString str;
	int nItem = m_LeftList.GetItemCount();

	lvitem.iItem = FindInsertionPosLeft(Key);

	lvitem.iSubItem = 0;
	str = strLcName;
	lvitem.pszText = str.GetBuffer(0);
	lvitem.mask=LVIF_TEXT;

	lvitem.mask |= LVIF_PARAM;
	lvitem.lParam = (LPARAM)Key;
	nItem = m_LeftList.InsertItem(&lvitem);
	str.ReleaseBuffer();

	return TRUE;
}

int CCMNlaSeqDlg::FindInsertionPosLeft(T_STLD_K Key)
{
	T_STLD_D stld;
	m_pDoc->m_pAttrCtrl->GetStld(Key, stld);
	int nID = stld.LoadCaseId;
	int nCount = m_LeftList.GetItemCount();
	int i = 0;
	for (i=0; i < nCount; i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(m_LeftList.GetItemData(i), stld);
		if (stld.LoadCaseId > nID) break;
	}
	return i;
}
///////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////
// sequence
void CCMNlaSeqDlg::InitRightItem()
{
	T_LDSQ_D data;
	T_STLD_K stldKey;
	T_STLD_D stldData;
	data.Initialize();

	m_pDoc->m_pAttrCtrl->GetLdsq(data);
	int nNum = data.aLoadingSequence.GetSize();

	m_aSeqData.RemoveAll();
	for(int i=0; i < nNum; i++)
	{
		seqdata seqd;
		stldKey = data.aLoadingSequence[i];
		m_pDoc->m_pAttrCtrl->GetStld(stldKey, stldData);

		seqd.stldKey	= stldKey;
		seqd.stldName = stldData.LoadCaseName;
		m_aSeqData.Add(seqd);
	}
}

void CCMNlaSeqDlg::SetRightHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load_Case_Name)};
	int nColWidth[2];

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_RightList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_RightList.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 30; nColWidth[1] = 115;

	// Set Title
	for(i=0; i < 2; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_RightList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMNlaSeqDlg::MakeRightItemEx()
{
	m_RightList.DeleteAllItems();

	int nItem;
	int nItemCount = m_aSeqData.GetSize();
	if(nItemCount == 0) return;

	if(nItemCount)
	{
		LVITEM lvitem;
		CString str;

		for(int nCount=0; nCount < nItemCount; nCount++)
		{
			for(int i=0; i < 2; i++)
			{
				lvitem.iItem = nCount;
				lvitem.iSubItem = i;
				str = DataToStr(i, nCount);
				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT;

				if( i == 0 ) 
				{
					lvitem.mask |= LVIF_PARAM;
					lvitem.lParam = (LPARAM)nCount;
					nItem = m_RightList.InsertItem(&lvitem);
				}
				else m_RightList.SetItem(&lvitem);
				str.ReleaseBuffer();
			}
		}
	}
}

CString CCMNlaSeqDlg::DataToStr(int i, int nNum)
{
	CString str;

	if(i==0)
		str.Format(_T("%d"), nNum+1);
	else if(i==1) str.Format(_T("%s"), m_aSeqData[nNum].stldName);
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

BOOL CCMNlaSeqDlg::CheckLDSQData(int key)
{
	int		nNum = m_aSeqData.GetSize();
	BOOL	flg = TRUE;

	for(int i=0; i < nNum; i++)
		if(m_aSeqData[i].stldKey == key) { flg = FALSE; break; }

	return flg;
}
///////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////////////////////////
// util

void CCMNlaSeqDlg::OnDblclkCmdNlaseqLlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdNlaseqRightBt();
	*pResult = 0;
}

void CCMNlaSeqDlg::OnDblclkCmdNlaseqRlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCmdNlaseqLeftBt(); 
	*pResult = 0;
}
