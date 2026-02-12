#if !defined(__CMDLGBASE_H__)
#define __CMDLGBASE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDlgBase.h : header file
//

#include "..\wg_base\wg_base_ModelessMoveDlg.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCMDlgBase dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMDlgBase : public CModelessMoveDlg //CChildDialogMove
{
// Construction
public:
	CCMDlgBase(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMDlgBase();

// Attributes
public:
	CDBDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CCMDlgBase)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDlgBase)
	public:
	virtual BOOL Create( UINT nIDTemplate, CWnd* pParentWnd = NULL, int nOption = 0);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMDlgBase)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateDlg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDLGBASE_H__)
