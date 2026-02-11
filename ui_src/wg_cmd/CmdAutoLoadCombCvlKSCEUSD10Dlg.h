#if !defined(__CMDAUTOLOADCOMBCVLKSCEUSD10DLG_H__)
#define __CMDAUTOLOADCOMBCVLKSCEUSD10DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlKSCEUSD10Dlg.h : header file
//
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_db\wg_db_EditUnit.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlKSCEUSD10Dlg dialog

class CCmdAutoLoadCombCvlDlg;
class CCmdAutoLoadCombCvlKSCEUSD10Dlg : public CChildDialog
{
	// Construction
public:
	CCmdAutoLoadCombCvlKSCEUSD10Dlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlKSCEUSD10Dlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_KSCE_USD_10_DLG };

	BOOL   	  m_bKSCEGravityLoadChk;
	CEditUnit m_edtKSCEEffLoadFactor;

	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlKSCEUSD10Dlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
public:
	void GetKSCEUSD10LoadFactor();
	void OnCmdKSCEUSD10EnableDisable();

protected:
	CDBDoc* m_pDoc;
	CString m_strCodeName;

	CCmdAutoLoadCombCvlDlg* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlKSCEUSD10Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdKSCEUSD10GRACHK();


	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLKSCEUSD10DLG_H__)
