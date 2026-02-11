#if !defined(__CMSlabDgnControlDlg_H__)
#define			 __CMSlabDgnControlDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSlabDgnControlDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSlabDgnControlDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMSlabDgnControlDlg : public CDialogMove
{
// Construction
public:
	CCMSlabDgnControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSlabDgnControlDlg() {};
// Dialog Data
	//{{AFX_DATA(CCMSlabDgnControlDlg)
	enum { IDD = IDD_ETC_SLAB_DGN_CONTROL_DLG};

	BOOL m_bConsiderMxy; // Consider Mxy (Torsional moment about x-y plane)
	//}}AFX_DATA
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSlabDgnControlDlg)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CCMSlabDgnControlDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSlabDgnControlDlg_H__DE9A3B81_5F27_11D4_9AAF_0000C0B9C58C__INCLUDED_)
