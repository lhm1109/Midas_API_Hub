
#if !defined(__DGNTABCTRL_H__)
#define __DGNTABCTRL_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SWTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDGNTabCtrl window
#include "..\wg_base\wg_base_DlgTabCtrl.h"


#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDGNTabCtrl : public CDlgTabCtrl
{


// Construction
public:
	CDGNTabCtrl();

// Attributes
public:

// Operations
public:
	BOOL ShowTab(int nPos);
	int AddTab(CDialog* pDlg, LPCTSTR szTabName, UINT nIDD, BOOL bDelete);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDGNTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowCurrentTab();
	void HideCurrentTab();

public:
	virtual ~CDGNTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDGNTabCtrl)
	afx_msg BOOL OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#include "HeaderPost.h"


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(__DGNTABCTRL_H__)

