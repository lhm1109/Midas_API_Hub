#if !defined(AFX_BNDRCGLPDLG_H__5B75A7CA_5A1C_46CB_A130_2DD072F1CB3C__INCLUDED_)
#define AFX_BNDRCGLPDLG_H__5B75A7CA_5A1C_46CB_A130_2DD072F1CB3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrCglpDlg.h : header file
//

#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\wg_db\wg_db_CobxNllp.h"

#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CBndrCglpDlg dialog

class CBndrCglpDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrCglpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnApply(); };
	BOOL ExternalInit(UINT key);

	void DisplayCurNllp();
	BOOL IsValidLongNumber(CString value, int &nErrCode);
	BOOL InvalidLongNumber(int nErrCode);	
	BOOL IsNumeric(CString str, int& ecode);
	BOOL ValidateError(LPCTSTR lpszError);

// Dialog Data
	//{{AFX_DATA(CBndrCglpDlg)
	enum { IDD = IDD_TM_BNDR_CGLP };
	CCobxBngr m_wndGroupCombo;
	int m_nOption;
	int m_nSelectOpt;
	CMouseEdit m_wnd2Node;
	MEdit m_wndGlinkEdt;
	CCobxNllp m_wndNllpCobx;
	MEdit m_wndNllp;
	//}}AFX_DATA

	CArray<UINT, UINT> m_aCtrlGenLkProp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrCglpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	// formouse edit....
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBndrCglpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefineGroupButton();
	afx_msg void OnSelchangeNlLinkPropCombo();
	afx_msg void OnNlLinkPropButton();
	afx_msg void OnOption();	
	afx_msg void OnSelOption();
	afx_msg void OnClose();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRCGLPDLG_H__5B75A7CA_5A1C_46CB_A130_2DD072F1CB3C__INCLUDED_)
