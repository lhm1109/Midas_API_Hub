#if !defined(__PUSHOVERHINGEASSIGNDLGOLD_H__)
#define __PUSHOVERHINGEASSIGNDLGOLD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PushOverHingeAssignDlgOld.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxHngt.h"
#include "CMChildBarBase.h"

/////////////////////////////////////////////////////////////////////////////
// CPushOverHingeAssignDlgOld dialog

class CPushOverHingeAssignDlgOld  : public CCMChildBarBase
{
public:
	CPushOverHingeAssignDlgOld();           // protected constructor used by dynamic creation
	// DECLARE_DYNCREATE(CPushOverHingeAssignDlgOld)

// Dialog Data
	//{{AFX_DATA(CPushOverHingeAssignDlgOld)
	enum { IDD = IDD_PUSHOVER_HINGE_ASSIGN_OLD };
	CBCGPStatic	m_wndHingeTypeInform;
	CCobxHngt	m_cmbHingeType;
	int		m_nLocation;
	int		m_nElemType;
	int		m_nCommandType;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPushOverHingeAssignDlgOld)	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	virtual BOOL ExternalInit(UINT key) { ASSERT(0); return TRUE; }
	void Execute() {};

// Implementation
protected:
	int m_nHingeType;
	void SetHingeTypeInform(int nHingeType);
	CDBDoc* m_pDoc;
	void GetElementByKind(CArray<UINT,UINT>& aKey,int nElemType);
	// Generated message map functions
	//{{AFX_MSG(CPushOverHingeAssignDlgOld)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnClose();
	afx_msg void OnSelchangePushoverAssignHingetypeCmb();
	afx_msg void OnPushoverAssignHingetypelistBtn();
	afx_msg void OnPushOverElemType();
	afx_msg void OnPushoverAssignCmd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PUSHOVERHINGEASSIGNDLGOLD_H__)
