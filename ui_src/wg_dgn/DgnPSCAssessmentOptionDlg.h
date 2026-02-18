#if !defined(AFX_DGNSTLRATINGPRINTDLG_H__)
#define AFX_DGNSTLRATINGPRINTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnPSCAssessmentOptionDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnPSCAssessmentOptionDlg dialog

class CDgnPSCAssessmentOptionDlg : public CMenuBarChildDlg
{
// Construction
public:
	CDgnPSCAssessmentOptionDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnPSCAssessmentOptionDlg)
	enum { IDD = IDD_DGN_PSC_ASSESSMENT_OPT_DLG };

	int	  m_nOption;
	int   m_nPart;
	int   m_nClass;
	BOOL  m_bTypeC;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnPSCAssessmentOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void EnableDisableControls();

	// Generated message map functions
	//{{AFX_MSG(CDgnPSCAssessmentOptionDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnExecute();
// 	afx_msg void OnDgnClose();
	afx_msg void OnDgnArDel();
	afx_msg void OnDgnClassRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLRATINGPRINTDLG_H__)
