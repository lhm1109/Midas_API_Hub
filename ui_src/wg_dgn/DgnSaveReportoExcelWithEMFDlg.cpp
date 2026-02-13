// DgnSaveReportoExcelWithEMFDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSaveReportoExcelWithEMFDlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\MSOfficeMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSaveReportoExcelWithEMFDlg dialog


CDgnSaveReportoExcelWithEMFDlg::CDgnSaveReportoExcelWithEMFDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSaveReportoExcelWithEMFDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSaveReportoExcelWithEMFDlg)
	m_nWriteType = 0;
	strFileFullName = _T("");
	m_bSaveEMF = FALSE;
	//}}AFX_DATA_INIT
}


void CDgnSaveReportoExcelWithEMFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);	
	//{{AFX_DATA_MAP(CDgnSaveReportoExcelWithEMFDlg)
	DDX_Control(pDX, IDC_DGN_SAVE_EXCEL_OPTION_NEW_RIDIO, m_BtnWriteType);	
	DDX_Control(pDX, IDC_DGN_SAVE_EMF_CHECK, m_BtnSaveEMF);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnSaveReportoExcelWithEMFDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSaveReportoExcelWithEMFDlg)
	ON_BN_CLICKED(IDC_DGN_SAVE_EXCEL_BTN, OnDgnSaveExcelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSaveReportoExcelWithEMFDlg message handlers
BOOL CDgnSaveReportoExcelWithEMFDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_BtnWriteType.SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSaveReportoExcelWithEMFDlg::OnDgnSaveExcelBtn() 
{
	// TODO: Add your control notification handler code here
	CString strExt    = CMSOfficeMgr::GetExtensionNameExcel();
	CString strDotExt = _T(".") + strExt;
	CString strFileFormat = _T("");
	strFileFormat.Format(_T("EXCEL File(*.%s)|*.%s|All File(*.*)|*.*||"), strExt, strExt);

	CString csFileName = CDBLib::MakeFileNameWithNewExt(strDotExt, TRUE);
	LPCTSTR szFileName;
	if (csFileName.IsEmpty()) szFileName = NULL;
	else szFileName = csFileName;

	CFileDialog fd(FALSE, strDotExt, szFileName,
								 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES, 
								 strFileFormat, NULL);
	if (fd.DoModal() == IDOK)
	{
		//AfxGetMainWnd()->Invalidate(FALSE);
		//AfxGetMainWnd()->UpdateWindow();
		//Invalidate(FALSE);
		//UpdateWindow();

		if(m_BtnWriteType.GetCheck())
			m_nWriteType = 0;
		else
			m_nWriteType = 1;

		m_bSaveEMF = m_BtnSaveEMF.GetCheck();
		
		strFileFullName = fd.GetPathName();

		CDialogMove::OnOK();
	}
}
