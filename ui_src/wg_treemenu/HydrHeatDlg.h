#if !defined(AFX_HYDRHEATDLG_H__E4CB2AC1_17E4_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_HYDRHEATDLG_H__E4CB2AC1_17E4_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HydrHeatDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxHsfc.h"
/////////////////////////////////////////////////////////////////////////////
// CHydrHeatDlg dialog

class CHydrHeatDlg : public CMenuBarChildDlg
{
// Construction
public:
	CHydrHeatDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CHydrHeatDlg)
	enum { IDD = IDD_TM_HYDR_HEAT };
	CCobxHsfc	m_CobxHeatFunc;
	int		m_option;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHydrHeatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_group;

	// Generated message map functions
	//{{AFX_MSG(CHydrHeatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmChangeOption();
	afx_msg void OnTmHydrHeatBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYDRHEATDLG_H__E4CB2AC1_17E4_11D5_880F_00010263A1F4__INCLUDED_)
