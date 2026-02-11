// BndrReltDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrReltDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\ViewBuff.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrReltDlg dialog

CBndrReltDlg::CBndrReltDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrReltDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrReltDlg)
	//}}AFX_DATA_INIT

	// Add, Delete
	m_aCtrlOption.Add(IDC_TM_OPT_ADD);
	m_aCtrlOption.Add(IDC_TM_OPT_DELETE);

// 	m_aCtrlApplyClose.Add(IDC_TM_EXECUTE);
// 	m_aCtrlApplyClose.Add(IDC_TM_CLOSE);

	m_nLinkType = 0;
}

CBndrReltDlg::~CBndrReltDlg()
{  

}

void CBndrReltDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrReltDlg)
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Control(pDX, IDC_TM_BNDR_RELT_LINK_NODE_EDT, m_edtLink);

	DDX_Radio(pDX, IDC_TM_BNDR_RELT_UNLOAD_RDO, m_nLinkType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrReltDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrReltDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE,    OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE,      OnTmClose)
	ON_BN_CLICKED(IDC_TM_OPT_ADD,    OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)

	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_EN_SETFOCUS(IDC_TM_BNDR_RELT_LINK_NODE_EDT, OnTmSetfocusLinkEdt)
	//}}AFX_MSG_MAP
// 	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
// 	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrReltDlg message handlers

BOOL CBndrReltDlg::OnInitDialog()  
{
	m_pDoc = CDBDoc::GetDocPoint();

	CMenuBarChildDlg::OnInitDialog();

	int nOption = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, nOption);

	RemoveNodeSelect();

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CBndrReltDlg::Execute()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// 	CArray<T_NODE_K, T_NODE_K> aSelKey;
	// 	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	//	T_ELNK_K key;
	T_ELNK_D data;
	data.Initialize();
	CArray<int, int> aNodes;

	CArray<T_NODE_K, T_NODE_K> aNodeK;
	CString str;

	m_edtLink.GetWindowText(str);
	GetNodeList(str, aNodeK);

	if (aNodeK.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_nodes_));
		return;
	}

	CArray<T_ELNK_K, T_ELNK_K> aElnkK;
	CArray<T_NLNK_K, T_NLNK_K> aNlnkK;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl2->GetLinkList(aNodeK, aElnkK, aNlnkK);

	if (aElnkK.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_T("There is no Elastic Link."));
		return;
	}

	CArray<T_RELT_D, T_RELT_D&> rData;
	CArray<T_RELT_D, T_RELT_D&> rModData;
	T_RELT_D Data;
	T_ELNK_D ElnkData;
	T_BNGR_K BngrK;
	m_wndGroupCombo.GetSelectedBngr(BngrK);
	for (int i = 0; i < aElnkK.GetSize(); i++)
	{
		pDoc->m_pAttrCtrl->GetElnk(aElnkK[i], ElnkData);
		if (ElnkData.nSubOption != 6) continue;

		Data.nStatus = m_nLinkType;
		Data.GroupKey = BngrK;
		Data.ElnkKey = aElnkK[i];

		rData.Add(Data);
	}

	if (rData.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_T("There is no Data."));
		return;
	}

	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);

	BOOL bSuccess = FALSE;
	if (nOption == 0) // Add or 
		bSuccess = m_pDoc->m_pDataCtrl->AddRelt(rData);
	else if (nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelRelt(rData);

	if (bSuccess)	m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}
void CBndrReltDlg::OnTmExecute() 
{
	Execute();
}

void CBndrReltDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CBndrReltDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nCheck);
	if (nCheck == 0)  // add
	{

	}

	if(nCheck == 1) //Delete
	{
		this->m_wndGroupCombo.ShowHideAll(TRUE);
	}
	else
	{
		this->m_wndGroupCombo.ShowHideAll(FALSE);
	}
}


void CBndrReltDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);
}

void CBndrReltDlg::OnTmSetfocusLinkEdt()
{
	// text preserve
	CString strPreviousText = _T("");
	m_edtLink.GetWindowText(strPreviousText);
	m_edtLink.Connect(SC_ID_NODE, &m_edtLink);
	m_edtLink.SetWindowText(strPreviousText);

	// model select
	CString strBlank = _T("");
	m_edtLink.SelectByStr(strBlank);
	m_edtLink.SelectByStr(strPreviousText);

}

void CBndrReltDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		//UpdateBuffer();
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


void CBndrReltDlg::UpdateBuffer()
{

	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount ==0 ) return;

	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	BOOL bIsTdmtCommand = FALSE;
	BOOL bIsTmatCommand = FALSE;
	BOOL bIsMatlCommand = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

// 		switch(nCmd)
// 		{
// 		case(UR_MLFC_ADD):
// 		case(UR_MLFC_DEL):
// 		case(UR_MLFC_MFD):
// 		case(UR_MLFC_MFS):
// 			RestoreMlfcCbx();
// 			break;
// 		case(UR_RLFC_ADD):
// 		case(UR_RLFC_DEL):
// 		case(UR_RLFC_MFD):
// 		case(UR_RLFC_MFS):
// 			RestoreMlfcCbx();
// 			break;
// 		default:
// 			break;
// 		}
	}
}

void CBndrReltDlg::RemoveNodeSelect()
{
	CString strPreviousText = _T("");
	m_edtLink.GetWindowText(strPreviousText);
	m_edtLink.Connect(SC_ID_NODE, &m_edtLink);

	CString strBlank = _T("");
	m_edtLink.SelectByStr(strBlank);

	m_edtLink.SetWindowText(strPreviousText);
	m_edtLink.Disconnect();
}