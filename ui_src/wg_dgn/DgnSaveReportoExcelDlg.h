#if !defined(AFX_DGNSAVEREPORTOEXCELDLG_H__)
#define AFX_DGNSAVEREPORTOEXCELDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSaveReportoExcelDlg.h : header file
//

#include "wg_dgnRes2.h"

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnSaveReportoExcelDlg dialog

class __MY_EXT_CLASS__ CDgnSaveReportoExcelDlg : public CDialogMove
{
// Construction
public:
	CDgnSaveReportoExcelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnSaveReportoExcelDlg)
	enum { IDD = IDD_DGN_SAVE_REPORT_EXCEL_DLG };
	int		m_nWriteType;
	MButton m_BtnWriteType;
	//}}AFX_DATA

	CString strFileFullName;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSaveReportoExcelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnSaveReportoExcelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSaveExcelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_DGNSAVEREPORTOEXCELDLG_H__)
