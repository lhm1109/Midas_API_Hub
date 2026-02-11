#if !defined(__CMDAUTOLOADCOMBCVLCSAEPDLG_H__)
#define __CMDAUTOLOADCOMBCVLCSAEPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlCSAEPDlg.h : header file
//


#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlCSAEPDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCmdAutoLoadCombCvlCSAEPDlg : public CDialogMove
{
// Construction
public:
	CCmdAutoLoadCombCvlCSAEPDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdAutoLoadCombCvlCSAEPDlg();   // standard deconstruction

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlCSAEPDlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_CSA_EP_DLG };

	CListBox m_lstEp;
	CListBox m_lstEp1;
	CListBox m_lstEp2;
	CListBox m_lstEp3;
	CListBox m_lstEp4;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlCSAEPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CDBDoc* m_pDoc;

	CArray<T_STLD_K, T_STLD_K> m_aStldK; // Load Case

	CArray<T_STLD_K, T_STLD_K> m_aStldEp1;
	CArray<T_STLD_K, T_STLD_K> m_aStldEp2;
	CArray<T_STLD_K, T_STLD_K> m_aStldEp3;
	CArray<T_STLD_K, T_STLD_K> m_aStldEp4;

	CArray<BOOL, BOOL> m_aSelFlag;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlCSAEPDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdCSAEP1AddBtn();
	afx_msg void OnCmdCSAEP1DelBtn();
	afx_msg void OnCmdCSAEP2AddBtn();
	afx_msg void OnCmdCSAEP2DelBtn();
	afx_msg void OnCmdCSAEP3AddBtn();
	afx_msg void OnCmdCSAEP3DelBtn();
	afx_msg void OnCmdCSAEP4AddBtn();
	afx_msg void OnCmdCSAEP4DelBtn();
	afx_msg void OnCmdCSAEP1Dblclk();
	afx_msg void OnCmdCSAEP2Dblclk();
	afx_msg void OnCmdCSAEP3Dblclk();
	afx_msg void OnCmdCSAEP4Dblclk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLCSAEPDLG_H__)
