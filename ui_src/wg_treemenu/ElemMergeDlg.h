#if !defined(__ELEMMERGEDLG_H__)
#define			 __ELEMMERGEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemMergeDlg.h : header file
//

#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CElemMergeDlg dialog

class CElemMergeDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemMergeDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CElemMergeDlg)
	enum { IDD = IDD_TM_ELEM_MERGE };
	CMouseEdit	m_editElement;
	mit::frx::MButton	m_chkRemoveNode;
	int		m_nElemType;
	int		m_nSelectType;
	BOOL	m_bRemoveNode;
	CFormulaEditSpin	m_wndTolerance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemMergeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT,UINT> arForcedGroup;

protected:
	BOOL ExecuteForced();
	void EnableDisableForced();

	// Generated message map functions
	//{{AFX_MSG(CElemMergeDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDeltaposTmMergeTolerSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmElemMergeType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMMERGEDLG_H__B82002C3_8AE0_11D4_A678_00010263A1CE__INCLUDED_)
