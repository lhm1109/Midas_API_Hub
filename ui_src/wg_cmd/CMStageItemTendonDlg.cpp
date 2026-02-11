// CMStageItemTendonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageItemTendonDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_DlgUtil.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMStageItemTendonDlg dialog
#define CDialog CCMStageItemBase

CCMStageItemTendonDlg::CCMStageItemTendonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCMStageItemTendonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStageItemTendonDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMStageItemTendonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStageItemTendonDlg)
	DDX_Control(pDX, IDC_CMD_DEACTIVE_GROUP_LIST, m_DeactiveGroupList);
	DDX_Control(pDX, IDC_CMD_ACTIVE_GROUP_LIST, m_ActiveGroupList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMStageItemTendonDlg, CDialog)
	//{{AFX_MSG_MAP(CCMStageItemTendonDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageItemTendonDlg message handlers


BOOL CCMStageItemTendonDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetListCtrlHeader();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCMStageItemTendonDlg::SetCurStageData(T_STAG_D* pStageD)
{
	this->m_pCurStageD = pStageD;
}

void CCMStageItemTendonDlg::UpdateGroupList() // Update Tendon List
{
	;
}

void CCMStageItemTendonDlg::SetListCtrlHeader()
{
	CStringArray Htitles;
	CArray<float,float> HRatios;
	Htitles.Add(_LS(IDS_CMD0417__Name));
	HRatios.Add(1);
	CDlgUtil::_SetListCtrlHeader(&m_ActiveGroupList,Htitles,&HRatios,NULL);
	CDlgUtil::_SetListCtrlHeader(&m_DeactiveGroupList,Htitles,&HRatios,NULL);
}

void CCMStageItemTendonDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	;
}

void CCMStageItemTendonDlg::UpdateBuffer()
{
	;
}

void CCMStageItemTendonDlg::ResetParam()
{
	;
}

