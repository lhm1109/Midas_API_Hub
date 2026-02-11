#if !defined(__CMEXLDDLG_H__)
#define __CMEXLDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExtTypeLoadDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CExtTypeLoadDlg dialog

#include "HeaderPre.h"
class CDBDoc;
class __MY_EXT_CLASS__ CExtTypeLoadDlg : public CDialogMove
{
// Construction
public:
	CExtTypeLoadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExtTypeLoadDlg)
	enum { IDD = IDD_PRES_EXLD };
	CListBox	m_listSelProfiles;
	CListBox	m_listAllProfiles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtTypeLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExtTypeLoadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdRemoveData();
	afx_msg void OnCmdSelButton();
	afx_msg void OnCmdUnselButton();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// User Defined Variables and Fucntions
protected:
	CDBDoc* m_pDoc;

	void Data2Dlg();
	BOOL Dlg2Data();
	void InitAllList();
	void InitSelList();

protected:
	CArray<UINT, UINT> m_aAllExld;
	CMapEx<UINT, UINT, int, int> m_mSelExld;  // T_STLD_K, 0
	CMapEx<UINT, UINT, int, int> m_mAllExld;  // T_STLD_K, 0
	T_EXLD_D m_ExldD;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMEXLDDLG_H__)
