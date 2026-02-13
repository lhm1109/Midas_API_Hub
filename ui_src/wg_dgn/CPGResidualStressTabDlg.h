#if !defined(AFX_CPGResidualStressTabDlg_H_INCLUDED_)
#define AFX_CPGResidualStressTabDlg_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGResidualStressTabDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGResidualStressTabDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGResidualStressDlg;

class __MY_EXT_CLASS__ CCPGResidualStressTabDlg : public CChildDialog
{
// Construction
public:
	CCPGResidualStressTabDlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGResidualStressTabDlg)
	enum { IDD = IDD_DGN_CPG_RESIDUAl_STRESS_TAB_DLG };
	
	int     m_nType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGResidualStressTabDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCgscData();
	void SetCgscData();

	CCPGResidualStressDlg* m_pParent;
	T_CRST_TYPE m_Data;

protected:

	void InitialDlgData();

	BOOL Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGResidualStressTabDlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGResidualStressTabDlg_H_INCLUDED_)
