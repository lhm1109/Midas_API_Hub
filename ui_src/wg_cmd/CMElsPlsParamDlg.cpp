// CMElsPlsParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMElsPlsParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsParamDlg dialog


CCMElsPlsParamDlg::CCMElsPlsParamDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMElsPlsParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMElsPlsParamDlg)
	m_strMu = _T("");
	//}}AFX_DATA_INIT
	m_Data.Initialize();
}


void CCMElsPlsParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMElsPlsParamDlg)
	DDX_Text(pDX, IDC_ELSPLS_MU_EDIT, m_strMu);
	//}}AFX_DATA_MAP
	UINT nCValue[17] = { IDC_ELSPLS_C1_EDIT,  IDC_ELSPLS_C2_EDIT,  IDC_ELSPLS_C3_EDIT,
											 IDC_ELSPLS_C4_EDIT,  IDC_ELSPLS_C5_EDIT,  IDC_ELSPLS_C6_EDIT,
											 IDC_ELSPLS_C7_EDIT,  IDC_ELSPLS_C8_EDIT,  IDC_ELSPLS_C9_EDIT,
											 IDC_ELSPLS_C10_EDIT, IDC_ELSPLS_C11_EDIT, IDC_ELSPLS_C12_EDIT,
											 IDC_ELSPLS_C13_EDIT, IDC_ELSPLS_C14_EDIT, IDC_ELSPLS_C15_EDIT,
											 IDC_ELSPLS_C16_EDIT, IDC_ELSPLS_C17_EDIT, };

	for(int i=0; i<17; i++) DDX_Text(pDX, nCValue[i], m_dCValue[i]);
}

BEGIN_MESSAGE_MAP(CCMElsPlsParamDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMElsPlsParamDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsParamDlg message handlers

BOOL CCMElsPlsParamDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	for(int i=0; i<17; i++) m_dCValue[i] = m_Data.MICROPL.dParamC[i];
	m_strMu.Format(_T("%g"), m_Data.MICROPL.dMu);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMElsPlsParamDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CString str; str.Empty();
	m_dMu = _tstof(m_strMu);

	CDialogMove::OnOK();
}
