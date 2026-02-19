// CPGLateraTorsion_IRC_Dlg.h: interface for the CCPGLateraTorsion_IRC_Dlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPGLATERALTORSION_IRC_DLG_H__)
#define AFX_CPGLATERALTORSION_IRC_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "CPGLateralTorsionTab_IRC_Dlg.h"
#include "CPGLateralTorsionTabJ_IRC_Dlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "DgnTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGLateralTorsionTab_IRC_Dlg;
class CCPGLateralTorsionTabJ_IRC_Dlg;

class __MY_EXT_CLASS__ CCPGLateraTorsion_IRC_Dlg : public CMenuBarChildDlg
{
public:
	CCPGLateraTorsion_IRC_Dlg(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CCPGLateraTorsion_IRC_Dlg)
	enum { IDD = IDD_DGN_CPG_RATER_TORSION_IRC_DLG };

	int		m_nOption;	
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_Tab;
	mit::frx::MButton m_Ctrl_BothIJ;
	//}}AFX_DATA

	T_CGLT_DATA m_I_Data;
	T_CGLT_DATA m_J_Data;

	BOOL m_bJTabEnable;
		
public:
	void Initial_SelectItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGLateraTorsion_IRC_Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	BOOL Dlg2Data();

	CCPGLateralTorsionTab_IRC_Dlg* m_pSubDlg_I;
	CCPGLateralTorsionTabJ_IRC_Dlg* m_pSubDlg_J;

	T_CGLT_D m_Data;

	// Generated message map functions
	//{{AFX_MSG(CCPGLateraTorsion_IRC_Dlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnClose();
// 	afx_msg void OnDgnExecute();
	afx_msg void OnDgnCPGTorsionAdd();
	afx_msg void OnDgnCPGTorsionDel();
	afx_msg void OnBothIJChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGLATERALTORSIONDLG_H__)
