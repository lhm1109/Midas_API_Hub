// CMDgnAnalOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDgnAnalOptionDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_dbLock\LockOption.h"

#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDgnAnalOptionDlg dialog

CCMDgnAnalOptionDlg::CCMDgnAnalOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMDgnAnalOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMDgnAnalOptionDlg)
	m_bConsiderStiff = FALSE;
	m_nStiffType     = 1;     // Plate를 기본값으로
	//m_bRelieveSettle = FALSE; 
	//}}AFX_DATA_INIT
}

void CCMDgnAnalOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDgnAnalOptionDlg)
	DDX_Check(pDX, IDC_ETC_DGNANAL_OPT_CONSIDER_STIFF_CHK, m_bConsiderStiff);
	DDX_Radio(pDX, IDC_ETC_DGNANAL_OPT_STIFF_BEAM_RDO    , m_nStiffType    );
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMDgnAnalOptionDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMDgnAnalOptionDlg)
	ON_BN_CLICKED(IDC_ETC_DGNANAL_OPT_CONSIDER_STIFF_CHK, OnConsiderStiffChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDgnAnalOptionDlg message handlers

BOOL CCMDgnAnalOptionDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_DAOP_D DaopD;
	if(!pDoc->m_pAttrCtrl2->GetDaop(DaopD)) DaopD.Initialize();
	m_bConsiderStiff = DaopD.bConsiderStiff;
	m_nStiffType     = DaopD.nStiffType    ;

	UpdateData(FALSE);

	EnableDisableCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDgnAnalOptionDlg::OnOK() 
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_DAOP_D DaopD;
	if(!pDoc->m_pAttrCtrl2->GetDaop(DaopD) || DaopD.bConsiderStiff!=m_bConsiderStiff || DaopD.nStiffType!=m_nStiffType)
	{
		DaopD.bConsiderStiff =m_bConsiderStiff;
		DaopD.nStiffType     =m_nStiffType;
		pDoc->m_pDataCtrl->AddDaop(DaopD);
	}

	IUsageCounter::Use(_T("DACT"), FALSE);
	CDialogMove::OnOK();
}

void CCMDgnAnalOptionDlg::OnConsiderStiffChk()
{
	UpdateData(TRUE);
	EnableDisableCtrls();
}

void CCMDgnAnalOptionDlg::EnableDisableCtrls()
{
	GetDlgItem(IDC_ETC_DGNANAL_OPT_STIFF_BEAM_RDO )->EnableWindow(m_bConsiderStiff);
	GetDlgItem(IDC_ETC_DGNANAL_OPT_STIFF_PLATE_RDO)->EnableWindow(m_bConsiderStiff);
}