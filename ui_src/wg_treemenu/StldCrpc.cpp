// StldCrpc.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldCrpc.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldCrpc dialog
#define CDialog CMenuBarChildDlg

CStldCrpc::CStldCrpc(CWnd* pParent /*=NULL*/)
	: CDialog(CStldCrpc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldCrpc)
	m_strCreep = _T("0");
	m_nOption = 0;
	//}}AFX_DATA_INIT
}


void CStldCrpc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldCrpc)
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_CobxLdgr);
	DDX_Text(pDX, IDC_TM_CREEP_EDIT, m_strCreep);
	DDX_Radio(pDX, IDC_TM_SD_OPT_ADD, m_nOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldCrpc, CDialog)
	//{{AFX_MSG_MAP(CStldCrpc)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_SD_OPT_ADD, OnTmSdOptRadio)
	ON_BN_CLICKED(IDC_TM_SD_OPT_DELETE, OnTmSdOptRadio)
	ON_BN_CLICKED(IDC_TM_SD_OPT_REPLACE, OnTmSdOptRadio)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldCrpc message handlers
BOOL CStldCrpc::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_nOption = 0;
	UpdateData(FALSE);

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldCrpc::OnTmDefineLdgrBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_DEF_LOAD_GROUP,0));
}

void CStldCrpc::OnTmExecute() 
{
	UpdateData(TRUE);
	//T_LDGR_K LdgrK;
	T_CRPC_D CrpcD;
	
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	//T_ELEM_D ElemData;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);

	if(aSelKey.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_T("No selected elements exist!"));
		return ;
	}
	
	
	/*
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
		if (!CDBDoc::GetDocPoint()->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aSelKey.RemoveAt(i);
	}
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
		return;
	}
	*/
	 
	T_LDGR_K LdgrK;
	m_CobxLdgr.GetSelectedLdgr(LdgrK);
	CrpcD.GroupKey = LdgrK;
	if(!CStrParser::GetFloatNumber(m_strCreep,CrpcD.dCreep))
	{
		GSaveHistoryFormatNF(_T("Parameter Error!"));
		return;
	}
	
	BOOL bSuccess;
	if(m_nOption == 0)
		bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddCrpc(aSelKey,CrpcD,FALSE);
	else if(m_nOption ==1)
		bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddCrpc(aSelKey,CrpcD,TRUE);
	else
		bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->DelCrpc(aSelKey,CrpcD.GroupKey);

	if (bSuccess) CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
}

void CStldCrpc::OnTmClose() 
{
	CloseDlg();
}

void CStldCrpc::OnTmSdOptRadio() 
{
	UpdateData();
	
	CArray<UINT,UINT> Ctrls;
	Ctrls.Add(IDC_TM_CREEP_EDIT);

	if(m_nOption == 0 || m_nOption == 1)
		CDlgUtil::CtrlEnableDisable(this,Ctrls,TRUE);
	else
		CDlgUtil::CtrlEnableDisable(this,Ctrls,FALSE);
}
