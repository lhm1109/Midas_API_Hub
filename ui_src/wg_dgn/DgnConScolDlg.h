#if !defined(__DGNCONSCOLDLG_H__)
#define __DGNCONSCOLDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConScolDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConScolDlg dialog
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

class CDgnConScolDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CDgnConScolDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	void Update_InitDataByCode();

	// Dialog Data
	//{{AFX_DATA(CDgnConScolDlg)
	enum { IDD = IDD_DGN_CON_SCOL };

	int	m_nOption;		// 0: Add/Replace, 1: Delete
	int m_nType;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConScolDlg)
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
	//{{AFX_MSG(CDgnConScolDlg)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONSCOLDLG_H__)
