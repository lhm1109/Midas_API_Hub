#if !defined(__PUSHOVERHINGEASSIGNDLG_H__)
#define __PUSHOVERHINGEASSIGNDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PushOverHingeAssignDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxPhgt.h"
#include "..\wg_db\wg_db_CobxNllp.h"
#include "CMChildBarBase.h"
#include "..\mit_frx\MButton.h"

#include "..\wg_base\MouseEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CPushOverHingeAssignDlg dialog

class CPushOverHingeAssignDlg  : public CCMChildBarBase
{
public:
	CPushOverHingeAssignDlg();           // protected constructor used by dynamic creation
	// DECLARE_DYNCREATE(CPushOverHingeAssignDlg)

// Dialog Data
	//{{AFX_DATA(CPushOverHingeAssignDlg)
	enum { IDD = IDD_PUSHOVER_HINGE_ASSIGN_NEW };
	CBCGPStatic	m_wndHingeTypeInform;
	CCobxPhgt	m_cmbHingeType;
	CMouseEdit	m_edtNlnkInput;
	CBCGPStatic   	m_stcNlnkInput;
	mit::frx::MButton     m_rdoGlink1;
	mit::frx::MButton     m_rdoGlink2;
	CCobxNllp   m_cmbGlink;
	mit::frx::MButton     m_btnGlink;
	int		m_nLocation;
	int		m_nElemType;
	int		m_nCommandType;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPushOverHingeAssignDlg)	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);


public:
	virtual BOOL ExternalInit(UINT key) { ASSERT(0); return TRUE; }
	

// Implementation
protected:
	int m_nHingeType;
	void SetHingeTypeInform(int nHingeType);
	CDBDoc* m_pDoc;
	void GetElementByKind(CArray<UINT,UINT>& aKey,int nElemType);
	void GetPointSpringSupportNode(CArray<UINT,UINT>& aKey);
	// Generated message map functions
	//{{AFX_MSG(CPushOverHingeAssignDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePushoverAssignHingetypeCmb();
	afx_msg void OnPushoverAssignHingetypelistBtn();
	afx_msg void OnPushOverElemType();
	afx_msg void OnPushoverAssignCmd();
	afx_msg void OnPushoverGLinkSelRdo();
	afx_msg void OnPushoverGLinkPropBtn();
	virtual void Execute();
	virtual void OnTmClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL DlgGlinkCtrlChanges();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PUSHOVERHINGEASSIGNDLG_H__)
