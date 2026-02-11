#if !defined(AFX_CMDAUTOLOADCOMBCVLHELPDLG_H__E4F1D694_75A2_488E_B911_B8F1439E70EF__INCLUDED_)
#define AFX_CMDAUTOLOADCOMBCVLHELPDLG_H__E4F1D694_75A2_488E_B911_B8F1439E70EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlHelpDlg.h : header file
//
#include "CmdAutoLoadCombCvlHelpGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlHelpDlg dialog

class CCmdAutoLoadCombCvlHelpDlg : public CDialogMove
{
// Construction
public:
	CCmdAutoLoadCombCvlHelpDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdAutoLoadCombCvlHelpDlg();
// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlHelpDlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_CVL_HELP_DLG };
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlHelpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCmdAutoLoadCombCvlHelpGrid *m_pGrid;

protected:
	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlHelpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDAUTOLOADCOMBCVLHELPDLG_H__E4F1D694_75A2_488E_B911_B8F1439E70EF__INCLUDED_)
