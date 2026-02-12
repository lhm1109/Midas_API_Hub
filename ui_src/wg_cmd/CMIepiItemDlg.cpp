// CMIepiItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMIepiItemDlg.h"

#include "..\wg_base\wg_base_NumericOptimizer.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_EditData.h"

#include "..\wg_common\wg_common_Query.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMIepiItemDlg dialog

//IMPLEMENT_DYNCREATE(CCMIepiItemDlg, CCMChildBarBase)
enum D_SELECT_MODE { SELECT_MODE_FROM_VIEW, SELECT_MODE_IMPORT_IELC };
enum D_OPERATE     { OPERATE_ADD, OPERATE_REPLACE, OPERATE_DELETE };

CCMIepiItemDlg::CCMIepiItemDlg()
	: CCMChildBarBase(CCMIepiItemDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMIepiItemDlg)
	m_pDoc = 0;
	//{{AFX_DATA_INIT(CCMIepiItemDlg)
	m_aCtrlOp.Add(IDC_CMD_OPTION_ADD);
	m_aCtrlOp.Add(IDC_CMD_OPTION_REPLACE);
	m_aCtrlOp.Add(IDC_CMD_OPTION_DELETE);
	m_nSelectMode = SELECT_MODE_IMPORT_IELC;
	//m_bImportSTLoad=TRUE;
}


void CCMIepiItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMIepiItemDlg)
	DDX_Control(pDX, IDC_CMD_IEPI_LC_CMB, m_LoadCase);
	DDX_Radio(pDX, IDC_CMD_IEPI_SELECT_VIEW_RDO, m_nSelectMode);
	//DDX_Check  (pDX, IDC_CMD_IMPORT_STLOAD_CHK, m_bImportSTLoad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMIepiItemDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMIepiItemDlg)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_CMD_IEPI_IMPORT_IELC_RDO, OnSelectModeRdo)
	ON_BN_CLICKED(IDC_CMD_IEPI_SELECT_VIEW_RDO, OnSelectModeRdo)
	ON_BN_CLICKED(IDC_CMD_IEPI_IELC_DLG_BTN, OnIelcDlgBtn)
	//}}AFX_MSG_MAP
	//ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMIepiItemDlg message handlers

BOOL CCMIepiItemDlg::OnInitDialog() 
{
	GetDlgItem(IDC_CMD_CLOSE)->ShowWindow(FALSE);
	GetDlgItem(IDC_CMD_EXECUTE)->ShowWindow(FALSE);
	m_pDoc = CDBDoc::GetDocPoint();
	CCMChildBarBase::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_LoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOp, OPERATE_ADD);
	OnSelectModeRdo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMIepiItemDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CCMIepiItemDlg::Execute() 
{
	UpdateData(TRUE);

	// Static Load Case에 할당된 Ielc를 그대로 적용함
	if(m_nSelectMode==SELECT_MODE_IMPORT_IELC)
	{
		if(DoImportSTLoad())
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);
		return;
	}

	int nOp=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOp, nOp);

	CArray<T_ELEM_K, T_ELEM_K> aKey;
	GetSelectedElemKeyList(aKey);

	if (aKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
		return;
	}

	T_IEPI_D IepiData; 
	IepiData.bIgnore = TRUE;

	BOOL bSuccess=FALSE;
	if (nOp == OPERATE_ADD)       // Add
		bSuccess = m_pDoc->m_pDataCtrl->AddIepi(aKey, IepiData);
	else if (nOp == OPERATE_DELETE)  // Delete
		bSuccess = m_pDoc->m_pDataCtrl->DelIepi(aKey);
	else
		ASSERT(0); // Replace는 없엉

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

BOOL CCMIepiItemDlg::DoImportSTLoad()
{
	unsigned int nLoadCaseType, nLoadCaseKey;
	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	if(nLoadCaseKey==0)
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD_GILC_NO_SELECTED_LOADCASE));
		return FALSE;
	}

	std::vector<T_ELEM_K> vElemK;
	CArray<T_IELC_K,T_IELC_K> aIelcK;
	m_pDoc->m_pAttrCtrl->GetIelcKeyList(aIelcK);
	for(int i=0; i<aIelcK.GetSize(); i++)
	{
		if(aIelcK[i].key.stldid == nLoadCaseKey)
			vElemK.push_back(aIelcK[i].key.entity);
	}

	// Super Transaction
	if(!m_pDoc->m_pUndoCtrl->StartEditDB(_LS(IDS_DB_DATACTRL_Add_IEPI), CMDTYPE_REMOVE_PUSHOVER)) return FALSE;

	int nOp=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOp, nOp);
	T_IEPI_D IepiD;
	IepiD.bIgnore=TRUE;
	if(nOp==OPERATE_REPLACE)
	{
		m_pDoc->m_pEditData->DelAllIepi();
		for(auto IepiK : vElemK)
			m_pDoc->m_pEditData->AddIepi(IepiK, IepiD);
	}
	else if(nOp==OPERATE_ADD)
	{
		for(auto IepiK : vElemK)
			m_pDoc->m_pEditData->AddIepi(IepiK, IepiD);
	}
	else // delete
	{
		for(auto IepiK : vElemK)
			m_pDoc->m_pEditData->DelIepi(IepiK);
	}

	m_pDoc->m_pUndoCtrl->CloseEditDB(FALSE, TRUE, LT_IEPI_CMD);
	// Super Transaction

	return TRUE;
}

void CCMIepiItemDlg::OnSelectModeRdo()
{
	UpdateData();

	GetDlgItem(IDC_CMD_OPTION_REPLACE)->EnableWindow(m_nSelectMode==SELECT_MODE_IMPORT_IELC);

	int nOp=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOp, nOp);
	if(m_nSelectMode==SELECT_MODE_FROM_VIEW)
	{
		// Replace > Add
		if(nOp==OPERATE_REPLACE)
			CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOp, OPERATE_ADD);
	}
}

void CCMIepiItemDlg::OnIelcDlgBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_QUERY_STRUCT_IELC,0));
}