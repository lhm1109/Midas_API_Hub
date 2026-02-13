// CPGDamageEquivalenceDlg.h: interface for the CCPGDamageEquivalenceDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPGDAMAGEEQUIVALENCEDLG_H__)
#define AFX_CPGDAMAGEEQUIVALENCEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "CPGDamageEquivalenceTabDlg.h"
#include "CPGDamageEquivalenceTabJDlg.h"

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

class CCPGDamageEquivalenceTabDlg;
class CCPGDamageEquivalenceTabJDlg;

class __MY_EXT_CLASS__ CCPGDamageEquivalenceDlg : public CMenuBarChildDlg
{
public:
	CCPGDamageEquivalenceDlg(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CCPGDamageEquivalenceDlg)
	enum { IDD = IDD_DGN_CPG_DAMAGE_EQUIVALENCE_DLG };

	int		m_nOption;		
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_Tab;
	mit::frx::MButton m_Ctrl_BothIJ;
	//}}AFX_DATA

	T_CGFR_DATA m_I_Data;
	T_CGFR_DATA m_J_Data;

	BOOL m_bJTabEnable;
	int	m_nElemType;

public:
	void Initial_SelectItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGDamageEquivalenceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	BOOL Dlg2Data();

	CCPGDamageEquivalenceTabDlg* m_pSubDlg_I;
	CCPGDamageEquivalenceTabJDlg* m_pSubDlg_J;

	T_CGFR_D m_Data;

	// Generated message map functions
	//{{AFX_MSG(CCPGDamageEquivalenceDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnClose();
// 	afx_msg void OnDgnExecute();
	afx_msg void OnDgnCPGDamageAdd();
	afx_msg void OnDgnCPGDamageDel();
	afx_msg void OnBothIJChk();
	afx_msg void OnDgnCPGElemTypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void AlignControl();
	void ShowHideControls();

	CArray<UINT, UINT> m_aPositionCtrl, m_aElemTypeCtrl;
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGDAMAGEEQUIVALENCEDLG_H__)
