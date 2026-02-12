#if !defined(__NODEMERGEDLG_H__)
#define      __NODEMERGEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeMergeDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CNodeMergeDlg dialog
class  _NodeMerge
{
public:
	int m_nOption;  // All, selection, number
	int m_nStartNum;
	int m_nEndNum;
	CArray<UINT, UINT> m_KeyList;
	double m_nTolerance;
	BOOL m_bRemoveMergedNodes;
};


class CNodeMergeDlg : public CMenuBarChildDlg
{
// Construction
public:
	CNodeMergeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	BOOL GetParameter(_NodeMerge& NM);
	BOOL MergeNode(_NodeMerge& NM);

	CArray<UINT,UINT> m_NumberCtrl;

// Dialog Data
	//{{AFX_DATA(CNodeMergeDlg)
	enum { IDD = IDD_TM_NODE_MERGE };
	CTextUnit	m_wndToleranceUnit;
	CEditUnit	m_wndToleranceEdit;
	int		m_nOption;
	BOOL	m_bRemoveMergedNodes;
	UINT	m_nStartNum;
	UINT	m_nEndNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeMergeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNodeMergeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmOptionAll();
	afx_msg void OnTmOptionNumber();
	afx_msg void OnTmOptionSelection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODEMERGEDLG_H__7C2CE184_9EBD_11D3_8899_0000C0A2329D__INCLUDED_)
