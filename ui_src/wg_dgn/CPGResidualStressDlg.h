// CPGResidualStressDlg.h: interface for the CCPGResidualStressDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPGResidualStressDlg_H__INCLUDED_)
#define AFX_CPGResidualStressDlg_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "DgnTabCtrl.h"
#include "CPGResidualStressTabDlg.h"
#include "CPGResidualStressTabJDlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGResidualStressTabDlg;
class CCPGResidualStressTabJDlg;

using namespace mit::frx;

class __MY_EXT_CLASS__ CCPGResidualStressDlg : public CMenuBarChildDlg
{
public:
	CCPGResidualStressDlg(CWnd* pParent = NULL);
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CCPGResidualStressDlg)
	enum { IDD = IDD_DGN_CPG_RESIDUAl_STRESS_DLG };
	
	int		m_nOption;
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_Tab;
	MButton m_Ctrl_BothIJ;
	//}}AFX_DATA

	T_CRST_TYPE m_I_Data;
	T_CRST_TYPE m_J_Data;

	BOOL m_bJTabEnable;
		
public:
	void Initial_SelectItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGResidualStressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	BOOL Dlg2Data();

	T_CRST_D m_Data;

	CCPGResidualStressTabDlg*  m_pSubDlg_I;
	CCPGResidualStressTabJDlg* m_pSubDlg_J;

	// Generated message map functions
	//{{AFX_MSG(CCPGResidualStressDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnCPGShearAdd();
	afx_msg void OnDgnCPGShearDel();
	afx_msg void OnBothIJChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGResidualStressDlg_H__INCLUDED_)
