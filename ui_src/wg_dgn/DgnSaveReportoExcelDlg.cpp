// DgnSaveReportoExcelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSaveReportoExcelDlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\MSOfficeMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSaveReportoExcelDlg dialog


CDgnSaveReportoExcelDlg::CDgnSaveReportoExcelDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSaveReportoExcelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSaveReportoExcelDlg)
	m_nWriteType = 0;
	strFileFullName = _T("");
	//}}AFX_DATA_INIT
}


void CDgnSaveReportoExcelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSaveReportoExcelDlg)	
	DDX_Control(pDX, IDC_DGN_SAVE_EXCEL_OPTION_NEW_RIDIO, m_BtnWriteType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnSaveReportoExcelDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSaveReportoExcelDlg)
	ON_BN_CLICKED(IDC_DGN_SAVE_EXCEL_BTN, OnDgnSaveExcelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSaveReportoExcelDlg message handlers

BOOL CDgnSaveReportoExcelDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_BtnWriteType.SetCheck(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSaveReportoExcelDlg::OnDgnSaveExcelBtn() 
{
	CString strExt    = CMSOfficeMgr::GetExtensionNameExcel();
	CString strDotExt = _T(".") + strExt;
	CString strFileFormat = _T("");
	strFileFormat.Format(_T("EXCEL File(*.%s)|*.%s|All File(*.*)|*.*||"), strExt, strExt);

	CString csFileName = CDBLib::MakeFileNameWithNewExt(strDotExt);
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
		
		strFileFullName = fd.GetPathName();

		CDialogMove::OnOK();
	}
}
