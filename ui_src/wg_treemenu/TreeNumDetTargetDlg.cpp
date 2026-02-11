#include "stdafx.h"
#include "wg_treemenu.h"
#include "TreeNumDetTargetDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTreeNumDetTargetDlg::CTreeNumDetTargetDlg(DlgNumDet* pDlgData,CWnd* pParent /* = NULL */)
	: m_pDlgData(pDlgData),CDialogMove(CTreeNumDetTargetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTreeNumDetTargetDlg)
	m_DpEd = _T("");
	m_bOkApply = FALSE;
	m_bExponent = FALSE;
	//}}AFX_DATA_INIT
}

CTreeNumDetTargetDlg::CTreeNumDetTargetDlg(UINT nIDTemplate,CWnd* pParent /*= NULL*/)
	: CDialogMove(nIDTemplate, pParent)
{
	m_DpEd = _T("");
	m_bOkApply = FALSE;
	m_bExponent = FALSE;
}


void CTreeNumDetTargetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreeNumDetTargetDlg)
	DDX_Text(pDX, IDC_TM_NUMDET_DPED, m_DpEd);
	DDX_Check(pDX, IDOK, m_bOkApply);
	DDX_Check(pDX, IDC_TM_NUMDET_EXPONENT, m_bExponent);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTreeNumDetTargetDlg, CDialogMove)
	//{{AFX_MSG_MAP(CTreeNumDetTargetDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeNumDetTargetDlg message handlers

BOOL CTreeNumDetTargetDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here

	m_DpEd.Format(_T("%d"), m_pDlgData->num_DecimalPoint);
	m_bExponent = m_pDlgData->num_ExponentFlag;
	//m_bOkApply = m_pView->m_pI_GPSModel->m_bApplyOk;

	UpdateData(FALSE);
	return TRUE;
}

void CTreeNumDetTargetDlg::OnOK()
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_pDlgData->num_ExponentFlag          = m_bExponent;
	_stscanf(m_DpEd.operator LPCTSTR(), _T("%d"), &m_pDlgData->num_DecimalPoint);
	CDialogMove::OnOK();
}

//BOOL CTreeNumDetTargetDlg::PreTranslateMessage(MSG* pMsg)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	if ((pMsg->message==WM_KEYDOWN) && (pMsg->wParam==13)) {
//		if ((GetDlgItem(IDOK)->m_hWnd!=pMsg->hwnd) && (GetDlgItem(IDCANCEL)->m_hWnd!=pMsg->hwnd))
//			pMsg->wParam = 9;
//	}
//
//	return CDialogMove::PreTranslateMessage(pMsg);
//}
