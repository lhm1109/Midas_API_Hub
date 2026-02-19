#if !defined(AFX_CPGResidualStressTabJDlg_H_INCLUDED_)
#define AFX_CPGResidualStressTabJDlg_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGResidualStressTabJDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGResidualStressTabJDlg dialog

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

class __MY_EXT_CLASS__ CCPGResidualStressTabJDlg : public CChildDialog
{
// Construction
public:
	CCPGResidualStressTabJDlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGResidualStressTabJDlg)
	enum { IDD = IDD_DGN_CPG_RESIDUAl_STRESS_TAB_DLG };
	
	int     m_nType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGResidualStressTabJDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCgscData();
	void SetCgscData();
	void SetAllControlEnable(bool enable);

	CCPGResidualStressDlg* m_pParent;
	T_CRST_TYPE m_Data;

protected:

	void InitialDlgData();

	BOOL Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGResidualStressTabJDlg)
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

#endif // !defined(AFX_CPGResidualStressTabJDlg_H_INCLUDED_)
