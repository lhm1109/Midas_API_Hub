#if !defined(AFX_STLDIFGSDLG_H__8964ED81_C832_11D4_880F_00010263A1F4__INCLUDED_)
#define AFX_STLDIFGSDLG_H__8964ED81_C832_11D4_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldIfgsDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldIfgsDlg dialog

class CStldIfgsDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldIfgsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnTmExecute(); }
// Dialog Data
	//{{AFX_DATA(CStldIfgsDlg)
	enum { IDD = IDD_TM_STLD_IFGS };
	MComboBox	m_direct;
	CTextUnit m_forceunit;
	CEditUnit m_force;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldIfgsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_Group;
	CArray<UINT, UINT> m_aCtrlOption;
	CDBDoc* m_pDoc;

protected:
	// Generated message map functions
	//{{AFX_MSG(CStldIfgsDlg)
	afx_msg void OnTmClose();
	afx_msg void OnTmExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnTmReplace();
	afx_msg void OnTmDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDIFGSDLG_H__8964ED81_C832_11D4_880F_00010263A1F4__INCLUDED_)
