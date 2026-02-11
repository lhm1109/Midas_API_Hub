#if !defined(AFX_SEISMASONRYMFACTORDlG_H__INCLUDED_)
#define AFX_SEISMASONRYMFACTORDlG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000 
// SeisMasonryMFactorDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisMasonryMFactorDlg dialog

class CSeisMasonryMFactorDlg : public CMenuBarChildDlg
{
// Construction
public:
	CSeisMasonryMFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CSeisMasonryMFactorDlg)
	enum { IDD = IDD_TM_SEIS_MASONRY_MFACT };
	int		m_nOption;
	int		m_nMembType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisMasonryMFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CEditUnit	m_Value1[3];

// Implementation
protected:
	CDBDoc* m_pDoc;
	void EnableValueEdit(BOOL bEnable);
	void InitCtrl();

	// Generated message map functions
	//{{AFX_MSG(CSeisMasonryMFactorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISMASONRYMFACTORDlG_H__INCLUDED_)
