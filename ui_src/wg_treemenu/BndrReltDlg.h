#if !defined(AFX_BndrReltDLG_H__19067F01_6A49_11D4_92DF_0000C0B0E6B3__INCLUDED_)
#define AFX_BndrReltDLG_H__19067F01_6A49_11D4_92DF_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrReltDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"

#include "..\wg_db\wg_db_selectctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CBndrReltDlg dialog
class CBndrReltDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CBndrReltDlg(CWnd* pParent = NULL);   // standard constructor
	~CBndrReltDlg();
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CBndrReltDlg)
	enum { IDD = IDD_TM_BNDR_RELT };

	CCobxBngr	 m_wndGroupCombo;     // Boundary Group Name
	CSelectEdit  m_edtLink;
	int          m_nLinkType;

	CArray<UINT, UINT> m_aCtrlOption;      // Add, Delete
// 	CArray<UINT, UINT> m_aCtrlApplyClose;  // Apply Close

// Overrides
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrReltDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support  

	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;  

	// Generated message map functions
	//{{AFX_MSG(CBndrReltDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnChangeOption();
	afx_msg void OnTmSetfocusLinkEdt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void RemoveNodeSelect();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BndrReltDLG_H__19067F01_6A49_11D4_92DF_0000C0B0E6B3__INCLUDED_)





















