#if !defined(AFX_STLDTENDONPROP_H__497ACAA1_A07C_4772_8BCC_24B2E1DDB45D__INCLUDED_)
#define AFX_STLDTENDONPROP_H__497ACAA1_A07C_4772_8BCC_24B2E1DDB45D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldTendonProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStldTendonProp dialog
#include "..\wg_db\wg_db_DBDoc.h"

class CStldTendonProp : public CMenuBarChildDlg , public CDBUpdateConnector
{
// Construction
public:
	CStldTendonProp(CWnd* pParent = NULL);   // standard constructor
	~CStldTendonProp();
	virtual void Execute() {}

	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Dialog Data
	//{{AFX_DATA(CStldTendonProp)
	enum { IDD = IDD_TM_STLD_ZASSIGN_TENDON_PROP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldTendonProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldTendonProp)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDTENDONPROP_H__497ACAA1_A07C_4772_8BCC_24B2E1DDB45D__INCLUDED_)
