#if !defined(AFX_DGNSAVEREPORTOEXCELWITHEMFDLG_H__)
#define AFX_DGNSAVEREPORTOEXCELWITHEMFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSaveReportoExcelWithEMFDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MButton.h"
#include "wg_dgnres.h"

using namespace mit::frx;

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnSaveReportoExcelWithEMFDlg dialog

class __MY_EXT_CLASS__ CDgnSaveReportoExcelWithEMFDlg : public CDialogMove
{
// Construction
public:
	CDgnSaveReportoExcelWithEMFDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnSaveReportoExcelWithEMFDlg)
	enum { IDD = IDD_DGN_SAVE_REPORT_EXCEL_WITH_EMF_DLG };
	int		m_nWriteType;
	MButton m_BtnWriteType;
	BOOL	m_bSaveEMF;
	MButton m_BtnSaveEMF;
	//}}AFX_DATA

	CString strFileFullName;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSaveReportoExcelWithEMFDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnSaveReportoExcelWithEMFDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSaveExcelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSAVEREPORTOEXCELWITHEMFDLG_H__)
