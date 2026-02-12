// CMThatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThatDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThatDlg dialog


CCMThatDlg::CCMThatDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThatDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThatDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCMThatDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThatDlg)
	DDX_Control(pDX, IDC_THAT_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_INC_EDT, m_edtInc);
	DDX_Control(pDX, IDC_CMD_END_EDT, m_edtEnd);
	DDX_Control(pDX, IDC_CMD_START_EDT, m_edtStart);
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cmbThlc);
	DDX_Control(pDX, IDC_CMD_THAT_START, m_StartUnit);
	DDX_Control(pDX, IDC_CMD_THAT_END, m_EndUnit);
	DDX_Control(pDX, IDC_CMD_THAT_INC, m_IncUnit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThatDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThatDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_THAT_LIST, OnItemchangedThatList)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_DELETE, OnCmdBtnDelete)
	ON_BN_CLICKED(IDC_CMD_BTN_MODIFY, OnCmdBtnModify)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThatDlg message handlers

BOOL CCMThatDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();
	
	m_cmbThlc.SetLoadType(D_SELECTLOAD_HISTORY);
	m_StartUnit.SetUnitType(D_UNITSYS_BASE_TIME);
	m_EndUnit.SetUnitType(D_UNITSYS_BASE_TIME);
	m_IncUnit.SetUnitType(D_UNITSYS_BASE_TIME);

	MakeListHeader();
	MakeItemEx();
	
	/*
	if (m_bModify)
	{
		int nCount = m_List.GetItemCount();
		for (int i=0; i<nCount; i++)
			if (m_Key == m_List.GetItemData(i)) break;
		if (i != nCount) 
		{
			m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
			Data2Dlg(m_Key, m_Data);
		}
	}
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThatDlg::MakeListHeader()
{
	CString aTitle[] = {_LS(IDS_CMD_THAT_LC), _LS(IDS_CMD_THAT_START), 
											_LS(IDS_CMD_THAT_END), _LS(IDS_CMD_THAT_INCREMENT)};
	int nColWidth[] = {70,50, 50, 50};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i=0; i<4; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMThatDlg::MakeItemEx()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	m_List.DeleteAllItems();

	T_THAT_K rKey;
	T_THAT_D rData;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartThat();
	int i = 0;
	while(pos)
	{
		pDoc->m_pAttrCtrl->GetNextThat(pos, rKey, rData);
		m_List.InsertItem(i, _T(""));
		SetItem(i, rKey, rData);
		i++;
	}
}

void CCMThatDlg::SetItem(int nIndex, T_THAT_K key, T_THAT_D& rData)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < 4; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, key, rData);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_List.SetItem(&lvitem);
	  str.ReleaseBuffer();
	}
	m_List.SetItemData(nIndex, key);
}

CString CCMThatDlg::DataToStr(int nIndex, T_THAT_K key, T_THAT_D& rData)
{
	CString str;
	str = _T("");

	switch(nIndex)
	{
	case 0:
		{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		T_THIS_D ThisD;
		pDoc->m_pAttrCtrl->GetThis(key, ThisD);
		str = ThisD.LoadCaseName;
		break;
		}
	case 1:
		str.Format(_T("%g"), rData.dStartTime);
		break;
	case 2:
		str.Format(_T("%g"), rData.dEndTime);
		break;
	case 3:
		str.Format(_T("%g"), rData.dIncrement);
		break;
	}
	return str;
}

void CCMThatDlg::Data2Dlg(T_THAT_K& rKey, T_THAT_D& rData)
{
	m_edtStart.SetEditUnit(rData.dStartTime);
	m_edtEnd.SetEditUnit(rData.dEndTime);
	m_edtInc.SetEditUnit(rData.dIncrement);
	m_cmbThlc.ChangeSelect(D_LOADCASE_HISTORY, rKey);
}

void CCMThatDlg::Dlg2Data(T_THAT_K& rKey, T_THAT_D& rData)
{
	rData.Initialize();
	rData.dStartTime = m_edtStart.GetEditValue();
	rData.dEndTime = m_edtEnd.GetEditValue();
	rData.dIncrement = m_edtInc.GetEditValue();

	UINT nLType,nLKey,nMnMxType;
	if(!m_cmbThlc.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return;
	}
	rKey = nLKey;
}

void CCMThatDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
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

void CCMThatDlg::UpdateBuffer()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_THAT_ADD):
		case(UR_THAT_DEL):
		case(UR_THAT_MFS):
		case(UR_THAT_MFD):
			bChanged = TRUE;
			break;
		default:
			break;
		}
		if (bChanged)
			break;
	} // end of while

	if (bChanged) MakeItemEx(); 
}

void CCMThatDlg::OnItemchangedThatList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_THAT_K key;
	T_THAT_D data;
	int nIndex;

	if (!GetSelectedData(nIndex))
		return;
	
	key = m_List.GetItemData(nIndex);
	pDoc->m_pAttrCtrl->GetThat(key, data);
	Data2Dlg(key, data);
	
	*pResult = 0;
}

void CCMThatDlg::OnCmdBtnAdd() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_THAT_K rKey;
	T_THAT_D rData;
	Dlg2Data(rKey, rData);
	
	pDoc->m_pDataCtrl->AddThat(rKey, rData);
}

void CCMThatDlg::OnCmdBtnDelete() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_THAT_K key;
	int nIndex;

	if (!GetSelectedData(nIndex))
		return;
	
	key = m_List.GetItemData(nIndex);
	pDoc->m_pDataCtrl->DelThat(key);
}

void CCMThatDlg::OnCmdBtnModify() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_THAT_K rKeyOld, rKeyNew;
	T_THAT_D rData;
	int nIndex;

	if (!GetSelectedData(nIndex))
		return;
	
	rKeyOld = m_List.GetItemData(nIndex);
	Dlg2Data(rKeyNew, rData);

	pDoc->m_pDataCtrl->ModifyThat(rKeyOld, rKeyNew, rData);
}

BOOL CCMThatDlg::GetSelectedData(int &nIndex)
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}

void CCMThatDlg::OnCmdClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

void CCMThatDlg::OnCmdLoadcaseBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_HISTORY_LOADCASE, 0));
}

BOOL CCMThatDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) { ASSERT(0); return FALSE; }

	T_THAT_D data;
	if (!pDoc->m_pAttrCtrl->GetThat(key, data))
		data.Initialize();

	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i=0; i<nCount; i++)
		if (key == m_List.GetItemData(i)) break;
	if (i != nCount) 
	{
		m_List.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
		Data2Dlg(key, data);
	}

	return TRUE;
}