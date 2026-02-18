// CPGUnbraLengDlg.h: interface for the CCPGUnbraLengDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPGUnbraLengDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGUnbraLengDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "CPGUnbraLengTabIDlg.h"
#include "CPGUnbraLengTabJDlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "DgnTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGUnbraLengTabIDlg;
class CCPGUnbraLengTabJDlg;

class __MY_EXT_CLASS__ CCPGUnbraLengDlg : public CMenuBarChildDlg
{
public:
	CCPGUnbraLengDlg(CWnd* pParent = NULL);
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CCPGUnbraLengDlg)
	enum { IDD = IDD_DGN_CPG_UNBR_LENG_DLG };
	
	int		m_nOption;
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_Tab;
	MButton m_Ctrl_BothIJ;
	//}}AFX_DATA

	T_CPUL_TYPE m_I_Data;
	T_CPUL_TYPE m_J_Data;

	BOOL m_bJTabEnable;
		
public:
	void Initial_SelectItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGUnbraLengDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	BOOL Dlg2Data();

	T_CPUL_D m_Data;

	CCPGUnbraLengTabIDlg* m_pSubDlg_I;
	CCPGUnbraLengTabJDlg* m_pSubDlg_J;

	// Generated message map functions
	//{{AFX_MSG(CCPGUnbraLengDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnCPGLoadAppAdd();
	afx_msg void OnDgnCPGLoadAppDel();
	afx_msg void OnBothIJChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGUnbraLengDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
