// RCPrintOption.h: interface for the CRCPrintOption class.
// Add by sshan. MNET:2067.('20061220)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCPRINTOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_RCPRINTOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
//#include "..\wg_db\DB_ST_DN.h"

class CDBDoc;

class CRCPrintOption : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CRCPrintOption(CWnd* pParent = NULL);
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CRCPrintOption)
	enum { IDD = IDD_DGN_RC_PRINT_OPTION };
	int		m_nOption;
	int 	m_nMPOption;
	int   m_nMNOption;
	int   m_nSOption;
	int   m_nTOption;
	int   m_nTenOption;
	int   m_nCompOption;
	int   m_nRPOption;
	int   m_nRNOption;

	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCPrintOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void SetControlLocation();
	// Generated message map functions
	//{{AFX_MSG(CRCPrintOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnCRCPorcAr();
	afx_msg void OnDgnCRCPorcDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCPRINTOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
