// CMThcrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThcrDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_DBDocExtVar.h"

#include "CMThMainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NCOL    3
/////////////////////////////////////////////////////////////////////////////
// CCMThcrDlg dialog
#define CDialog CCMChildBarBase

CCMThcrDlg::CCMThcrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMThcrDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThcrDlg)
	m_Data.Initialize();
	m_nOutType = 0;

	//}}AFX_DATA_INIT
}
void CCMThcrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThcrDlg)
	DDX_Control(pDX, IDC_CMD_THCR_GROUPNAME_EDT, m_edtName);
	DDX_Control(pDX, IDC_CMD_THCR_MNODE_EDT,     m_edtMasterNode);
    DDX_Control(pDX, IDC_CMD_THCR_SNODE_EDT,     m_edtSelectSubNode);
	DDX_Radio  (pDX, IDC_CMD_THCR_OUT_DISP_RDO,  m_nOutType);
	DDX_Control(pDX, IDC_CMD_THCR_COMP_COMBO,    m_CompCb);
	DDX_Control(pDX, IDC_CMD_THCR_LIST,          m_ctrlList);
	//}}AFX_DATA_MAP
}

void CCMThcrDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch (lHint)
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

void CCMThcrDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;

		switch (nCmd)
		{
		case(UR_THCR_ADD):
		case(UR_THCR_DEL):
			//case(UR_THCR_MFD):
			//case(UR_THCR_MFS):
			bMFD = TRUE;
			break;
		default:
			break;
		}
	}

	if (bMFD)
		MakeItemEx();
}

BOOL CCMThcrDlg::ExternalInit(UINT key)
{
	int nItemCount = m_ctrlList.GetItemCount();
	int i = 0;
	for (i = 0; i < nItemCount; i++)
		if (key == m_ctrlList.GetItemData(i)) break;
	if (i == nItemCount) { ASSERT(0); return FALSE; }

	m_ctrlList.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);


	return TRUE;
}

void CCMThcrDlg::InitCtrl()
{
	m_edtMasterNode.SetAttNodeList();
	m_edtMasterNode.SetLButtonDownNotifyWindow(this);
	m_edtMasterNode.SetEnterNotifyWindow(this);
	m_edtMasterNode.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_edtMasterNode.SetMaxNodeKeyNum(1);
	GotoDlgCtrl(&m_edtMasterNode);

	m_edtSelectSubNode.Connect(SC_ID_NODE, &m_edtSelectSubNode);

	m_CompCb.ResetContent();
	CDlgUtil::CobxAddItem(m_CompCb, _LS(IDS_CMD_GLOBAL_DX), 0);
	CDlgUtil::CobxAddItem(m_CompCb, _LS(IDS_CMD_GLOBAL_DY), 1);
	CDlgUtil::CobxAddItem(m_CompCb, _LS(IDS_CMD_GLOBAL_DZ), 2);
	CDlgUtil::CobxAddItem(m_CompCb, _LS(IDS_CMD_GLOBAL_RX), 3);
	CDlgUtil::CobxAddItem(m_CompCb, _LS(IDS_CMD_GLOBAL_RY), 4);
	CDlgUtil::CobxAddItem(m_CompCb, _LS(IDS_CMD_GLOBAL_RZ), 5);
	m_CompCb.SetCurSel(0);
}

void CCMThcrDlg::MakeListHeader()
{
	CString aTitle[] = {_LS(IDS_CMD_THRD_NAME), _LS(IDS_CMD_MASTERNODE), _LS(IDS_CMD_THRD_TYPE)};
	int nColWidth[] = {55,85,62};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_ctrlList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_ctrlList.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i=0; i<NCOL; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_ctrlList.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMThcrDlg::MakeItemEx()
{
	m_ctrlList.DeleteAllItems();

	T_THCR_D ThcrD;
	CArray<T_THCR_K, T_THCR_K> arKey;
	m_pDoc->m_pAttrCtrl->GetThcrKeyList(arKey);

	int nSize = arKey.GetSize();
	if (nSize == 0)
		return;

	for (int i=0; i<nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetThcr(arKey[i], ThcrD);
		m_ctrlList.InsertItem(i, _T(""));
		SetItem(i, arKey[i], ThcrD);
	}
}

void CCMThcrDlg::SetItem(int nIndex, T_THCR_K ThcrK, T_THCR_D& ThcrD)
{
	LVITEM lvitem;
	CString str;
	for(int i = 0; i < NCOL; i++)
	{
		lvitem.iItem=nIndex;
		lvitem.iSubItem=i;
		str = DataToStr(i, ThcrD);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT ;

		m_ctrlList.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	m_ctrlList.SetItemData(nIndex, (DWORD)ThcrK);
}

CString CCMThcrDlg::DataToStr(int nIndex, T_THCR_D& ThcrD)
{
	CString aType[] = { _LS(IDS_CMD_Disp), _LS(IDS_CMD_Vel), _LS(IDS_CMD_Accel)};
	CString value = _T("");
	switch(nIndex)
	{
	case 0:
		value = ThcrD.GroupName; break;
	case 1:
		value.Format(_T("%d"), ThcrD.MasterNode); break;
	case 2:
		value = aType[ThcrD.nOutType]; break;
	default:
		ASSERT(FALSE);
		break;
	}
	return value;
}

BOOL CCMThcrDlg::Dlg2Data()
{
	UpdateData(TRUE);

	//
	m_edtName.GetWindowText(m_Data.GroupName);
	//
	CString str;
	m_edtMasterNode.GetWindowText(str);
	m_Data.MasterNode = _ttoi(str);
	//
	m_edtSelectSubNode.GetWindowText(str);
	GetNodeList(str, m_Data.aSubNode);
	//
	m_Data.nOutType = m_nOutType;
	//
	m_Data.nComponent = CDlgUtil::CobxGetCurSelItemData(m_CompCb, m_CompCb.GetCurSel());

	return  TRUE;
}

void CCMThcrDlg::Data2Dlg()
{
	//
	m_edtName.SetWindowText(m_Data.GroupName);
	//
	CString str;
	str.Format(_T("%d"), m_Data.MasterNode);
	m_edtMasterNode.SetWindowText(str);
	//
	CNumericOptimizer opt;
	str = opt.OptimizeUnsortedArray(m_Data.aSubNode);
	m_edtSelectSubNode.SetWindowText(str);
	//
	m_nOutType = m_Data.nOutType;
	//
	CDlgUtil::CobxSetCurSelItemData(m_CompCb, m_Data.nComponent);

	UpdateData(FALSE);
}


BOOL CCMThcrDlg::GetSelectedIndex(int &nIndex)
{
	int iItem = m_ctrlList.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return FALSE;

	nIndex = iItem;
	return TRUE;
}


BEGIN_MESSAGE_MAP(CCMThcrDlg, CDialog)
	//{{AFX_MSG_MAP(CCMThcrDlg)
	ON_BN_CLICKED(IDC_CMD_ADD_BUTTON   , OnThcrAdd)
	ON_BN_CLICKED(IDC_CMD_MODIFY_BUTTON, OnThcrModify)
	ON_BN_CLICKED(IDC_CMD_DELETE_BUTTON, OnThcrDelete)
	ON_BN_CLICKED(IDC_CMD_CLOSE        , OnClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_THCR_LIST, OnListItemChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThcrDlg message handlers
BOOL CCMThcrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	InitCtrl();

	MakeListHeader();
	MakeItemEx();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMThcrDlg::OnThcrAdd()
{
	m_Data.Initialize();
	if(!Dlg2Data()) return;

	if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddThcr(m_Data))
	{
		ASSERT(0);
	}
}

void CCMThcrDlg::OnThcrModify()
{
	int nIndex;
	if (!GetSelectedIndex(nIndex))
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_NO_SELECTED_THCR));
		return;
	}

	m_Data.Initialize();
	if(!Dlg2Data()) return;

	T_THCR_K ThcrK;
	T_THCR_D ThcrD;
	ThcrK = m_ctrlList.GetItemData(nIndex);
	m_pDoc->m_pAttrCtrl->GetThcr(ThcrK, ThcrD);

	if(!CDBDoc::GetDocPoint()->m_pDataCtrl->ModifyThcr(ThcrK, ThcrK, m_Data))
	{
		ASSERT(0);
	}
}

void CCMThcrDlg::OnThcrDelete()
{
	int nIndex = -1;
	int nSelCount = m_ctrlList.GetSelectedCount();

	if (nSelCount < 1)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_NO_SELECTED_THCR));
		return;
	}

	T_THCR_K ThcrK;
	T_THCR_D ThcrD;

	POSITION pos = m_ctrlList.GetFirstSelectedItemPosition();
	CArray<T_THCR_K, T_THCR_K> rstrKey;
	while (pos)
	{
		nIndex = m_ctrlList.GetNextSelectedItem(pos);
		ThcrK = m_ctrlList.GetItemData(nIndex);
		m_pDoc->m_pAttrCtrl->GetThcr(ThcrK, ThcrD);
		rstrKey.Add(ThcrK);
	}
	m_pDoc->m_pDataCtrl->DelThcr(rstrKey);
}

void CCMThcrDlg::OnClose()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
	CDBDocExtVar::Instance()->GPS_m_nReturnMenuID = 0;
}


void CCMThcrDlg::OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_THCR_K key;
	T_THCR_D data;
	int nIndex;
	if (!GetSelectedIndex(nIndex))
	{
		//GSaveHistoryFormatNF(_LS(IDS_CMD_NO_SELECTED_THCR));
		return;
	}

	key = m_ctrlList.GetItemData(nIndex);
	pDoc->m_pAttrCtrl->GetThcr(key, data);
	m_Data = data;
	Data2Dlg();

	*pResult = 0;
}
