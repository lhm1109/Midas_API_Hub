// CMSlabDgnControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSlabDgnControlDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_db\IUsageCounter.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSlabDgnControlDlg dialog

CCMSlabDgnControlDlg::CCMSlabDgnControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSlabDgnControlDlg::IDD, pParent),
	m_bConsiderMxy(TRUE)
{
	//{{AFX_DATA_INIT(CCMSlabDgnControlDlg)
	//}}AFX_DATA_INIT
}

void CCMSlabDgnControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSlabDgnControlDlg)
	DDX_Check(pDX, IDC_ETC_SLAB_DGN_CONTROL_MXY_CHK, m_bConsiderMxy);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSlabDgnControlDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSlabDgnControlDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSlabDgnControlDlg message handlers

BOOL CCMSlabDgnControlDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_SLDC_D SldcD;
	pDoc->m_pAttrCtrl2->GetSldc(SldcD);
	m_bConsiderMxy = SldcD.bConsiderMxy;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSlabDgnControlDlg::OnOK() 
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_SLDC_D SldcD;
	if(!pDoc->m_pAttrCtrl2->GetSldc(SldcD) || SldcD.bConsiderMxy!=m_bConsiderMxy)
	{
		SldcD.bConsiderMxy = m_bConsiderMxy;
		pDoc->m_pDataCtrl->AddSldc(SldcD);
	}
	IUsageCounter::Use(_T("SDGNC"), FALSE);
	CDialogMove::OnOK();
}
