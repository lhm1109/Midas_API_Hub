// FatigueCheckDlg.h: interface for the CFatigueCheckDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FATIGUECHECKDLG_H__)
#define AFX_FATIGUECHECKDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "DgnTabCtrl.h"
#include "FatigueCheckTabIDlg.h"
#include "FatigueCheckTabJDlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CFatigueCheckTabIDlg;
class CFatigueCheckTabJDlg;

class __MY_EXT_CLASS__ CFatigueCheckDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CFatigueCheckDlg(CWnd* pParent = NULL);
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CFatigueCheckDlg)
	enum { IDD = IDD_DGN_DESIGN_FATI_CHECK_DLG };

	int   m_nOption;
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_ctrlTab;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFatigueCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
		
public:

	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();	

	void InitTab();
	void ControlsEnableDisable();

	void Dlg2Data();

	CDBDoc* m_pDoc;

	T_FACK_D m_Data;

	int m_nDgnCode;

	CFatigueCheckTabIDlg* m_pSubDlg_I;
	CFatigueCheckTabJDlg* m_pSubDlg_J;

// Implementation
	
	// Generated message map functions
	//{{AFX_MSG(CFatigueCheckDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnFatiChkOptionRdo();
	afx_msg void OnDgnFatiChkBothChk();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FATIGUECHECKDLG_H__)
