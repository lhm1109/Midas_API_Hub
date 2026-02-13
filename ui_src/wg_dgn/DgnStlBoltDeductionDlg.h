// DgnStlBoltDeductionDlg.h: interface for the CDgnStlBoltDeductionDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnStlBoltDeductionDLG_H__)
#define AFX_DgnStlBoltDeductionDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "DgnStlBoltDeductionPage.h"

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "DgnTabCtrl.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CDgnStlBoltDeductionPage;

class __MY_EXT_CLASS__ CDgnStlBoltDeductionDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
public:
	CDgnStlBoltDeductionDlg(CWnd* pParent = NULL);
	//virtual void Execute() {}

	// Dialog Data
		//{{AFX_DATA(CDgnStlBoltDeductionDlg)
	enum { IDD = IDD_DGN_STL_BOLT_DEDUCTION_DLG };

	int		m_nOption;
	BOOL  m_bBoth;
	CDlgTabCtrl	m_Tab;
	//}}AFX_DATA

	T_STBD_TYPE m_Data_I;
	T_STBD_TYPE m_Data_J;

	int      m_nDgnCode;
	CString  m_strDgnCode;

public:
	void Initial_SelectItem();
	void SetDgnCode(int nDgnCode);
	int  GetDgnCode();
	void SubDlgCtrlEnableDisable();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnStlBoltDeductionDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	// Implementation
protected:
	CDBDoc* m_pDoc;

	BOOL Dlg2Data();

	T_STBD_D m_Data;

	CDgnStlBoltDeductionPage* m_pSubDlg_I;
	CDgnStlBoltDeductionPage* m_pSubDlg_J;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlBoltDeductionDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	//afx_msg void OnDgnClose();
	//afx_msg void OnDgnExecute();
	afx_msg void OnDgnAddDel();
	afx_msg void OnBothIJChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnStlBoltDeductionDLG_H__)
