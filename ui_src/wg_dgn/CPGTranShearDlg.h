// CPGTranShearDlg.h: interface for the CCPGTranShearDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CPGTRANSHEARDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGTRANSHEARDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "DgnTabCtrl.h"
#include "CPGTranShearTabDlg.h"
#include "CPGTranShearTabJDlg.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"
#include "..\MIT_frx\MButton.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGTranShearTabDlg;
class CCPGTranShearTabJDlg;

class __MY_EXT_CLASS__ CCPGTranShearDlg : public CMenuBarChildDlg
{
public:
	CCPGTranShearDlg(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CCPGTranShearDlg)
	enum { IDD = IDD_DGN_CPG_TRAN_SHEAR_DLG };
	
	int		m_nOption;
	BOOL	m_bBothIJ;
	CDlgTabCtrl	m_Tab;
	mit::frx::MButton m_Ctrl_BothIJ;
	//}}AFX_DATA

	T_CGSC_STUD m_I_Data;
	T_CGSC_STUD m_J_Data;

	BOOL m_bJTabEnable;
		
	int	m_nElemType;

public:
	void Initial_SelectItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGTranShearDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	BOOL Dlg2Data();

	T_CGSC_D m_Data;

	CCPGTranShearTabDlg*  m_pSubDlg_I;
	CCPGTranShearTabJDlg* m_pSubDlg_J;

	// Generated message map functions
	//{{AFX_MSG(CCPGTranShearDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnClose();
// 	afx_msg void OnDgnExecute();
	afx_msg void OnDgnCPGShearAdd();
	afx_msg void OnDgnCPGShearDel();
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

#endif // !defined(AFX_CPGTRANSHEARDLG_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
