#if !defined(__DGNIMPORTFORCEDLG_H__)
#define __DGNIMPORTFORCEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnImportForceDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnImportForceDlg dialog

class __MY_EXT_CLASS__ CDgnImportForceDlg : public CDialogMove
{
// Construction
public:
	CDgnImportForceDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnConMatDlg)
	enum { IDD = IDD_DGN_IMPORT_FORCE_DLG };

	CString m_strExistFile;
	BOOL m_bImport;
	CButton	m_ImportBtn;
	//}}AFX_DATA
	
protected:
	int m_iDgnCode;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConMatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnImportBtn();
	afx_msg void OnImportCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL ExistImportForceFile();
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNIMPORTFORCEDLG_H__)
