// CMStageInfoDlg.cpp : implementation file
//
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageInfoDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
//#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMStageInfoDlg dialog
#define CDialog CChildDialogMove


CCMStageInfoDlg::CCMStageInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMStageInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageInfoDlg)
	//}}AFX_DATA_INIT
}


void CCMStageInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageInfoDlg)
	DDX_Control(pDX, IDC_CMD_NODE_LIST, m_wndNodeList);
	DDX_Control(pDX, IDC_CMD_LDGR_LIST, m_wndLdgrList);
	DDX_Control(pDX, IDC_CMD_BNGR_LIST, m_wndBngrList);
	DDX_Control(pDX, IDC_CMD_GROUP_LIST, m_wndGrupList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMStageInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CCMStageInfoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageInfoDlg message handlers

BOOL CCMStageInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	MakeNodeList();
	MakeGroupList();
	MakeBngrList();
	MakeLdgrList();
	//MakeTendonList();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStageInfoDlg::SetCurStageData(int nStageId, T_STAG_D& CurStagData)
{
	m_nStageId      = nStageId   ;
	m_TempStagData  = CurStagData;
}

void CCMStageInfoDlg::MakeNodeList()
{
	
	CArray<T_NODE_K, T_NODE_K> _NodeList;

	if(m_nStageId < 0) return;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStagActiveList(m_nStageId,m_TempStagData,
																												&_NodeList,NULL,NULL,NULL);
	this->m_wndNodeList.ResetContent();
	CString StrTemp;
	int nItem = _NodeList.GetSize();
	for(int i = 0 ; i < nItem ; i++)
	{
		StrTemp.Format(_T("%d"),_NodeList[i]);
		m_wndNodeList.AddString(StrTemp);
	}
}

void CCMStageInfoDlg::MakeGroupList()
{
	
	CArray<T_ELEM_K, T_ELEM_K> _ElemList;
	if(m_nStageId < 0) return;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStagActiveList(m_nStageId,m_TempStagData,NULL,
																												&_ElemList,NULL,NULL);
	m_wndGrupList.ResetContent();
	CString StrTemp;
	int nItem = _ElemList.GetSize();
	for(int i = 0; i < nItem ; i++)
	{
		StrTemp.Format(_T("%d"),_ElemList[i]);
		m_wndGrupList.AddString(StrTemp);
	}
}

void CCMStageInfoDlg::MakeBngrList()
{
	CArray<T_BNGR_K, T_BNGR_K> _BngrList;
	if(m_nStageId < 0) return;

	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStagActiveList(m_nStageId, m_TempStagData,NULL,
																												NULL,&_BngrList,NULL);
	m_wndBngrList.ResetContent();
	CString StrTemp;
	T_BNGR_D BngrD;

	int nItem = _BngrList.GetSize();
	for( int i= 0; i < nItem ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetBngr(_BngrList[i],BngrD))
		{
			m_wndBngrList.AddString(BngrD.GroupName);
		
		}
	}
}

void CCMStageInfoDlg::MakeLdgrList()
{
	CArray<T_LDGR_K, T_LDGR_K> _LdgrList;
	if(m_nStageId < 0) return;
	
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetStagActiveList(m_nStageId,m_TempStagData,NULL,
																												NULL,NULL,&_LdgrList);
	m_wndLdgrList.ResetContent();
	CString StrTemp;
	T_LDGR_D LdgrD;

	int nItem = _LdgrList.GetSize();
	for( int i = 0; i < nItem ; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetLdgr(_LdgrList[i],LdgrD))
		{
			m_wndLdgrList.AddString(LdgrD.GroupName);
		}
	}
}

//void CCMStageInfoDlg::MakeTendonList()
//{
	
	//CArray<CString, CString&> rStrGrupList;
	//CDBDoc::GetDocPoint()->m_pAttrCtrl->GetGrupList(rStrGrupList);
	
//}
