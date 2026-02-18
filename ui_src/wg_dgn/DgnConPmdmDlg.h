#if !defined(__DGNCONPMDMDLG_H__)
#define __DGNCONPMDMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConPmdmDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConPmdmDlg dialog
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

class CDgnConPmdmDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConPmdmDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	void Update_InitDataByCode();

// Dialog Data
	//{{AFX_DATA(CDgnConPmdmDlg)
	enum { IDD = IDD_DGN_CON_PMDM };

	int	m_nOption;		// 0: Add/Replace, 1: Delete
	int m_nElemType;	// 0: RC Column, 1: Wall
	int m_nCalcMethod;	// 0: Keep M constant, 1: Keep P constant
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConPmdmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConPmdmDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONPMDMDLG_H__)
