#if !defined(AFX_DgnStlBoltDeductionPAGE_H__)
#define AFX_DgnStlBoltDeductionPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlBoltDeductionPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnStlBoltDeductionPage dialog
#include "..\wg_base\ChildDialog.h" 

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CDgnStlBoltDeductionDlg;

class __MY_EXT_CLASS__ CDgnStlBoltDeductionPage : public CChildDialog
{
// Construction
public:
	CDgnStlBoltDeductionPage(CWnd* pParent = NULL, int nTab = 0);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnStlBoltDeductionPage)
	enum { IDD = IDD_DGN_STL_BOLT_DEDUCTION_PAGE };
	
	BOOL  m_bTotal;
	CEditUnit m_edtTotal;
	CEditUnit m_edt_TF;
	CEditUnit m_edt_BF;
	CEditUnit m_edt_LW;
	CEditUnit m_edt_RW;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlBoltDeductionPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetDohlData();
	void ControlsEnableDisable();

	CDgnStlBoltDeductionDlg* m_pParent;
	T_STBD_TYPE m_Data;

	int m_nTab;

protected:
	void InitUnit();

	void Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnStlBoltDeductionPage)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnTotalChk();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnStlBoltDeductionPAGE_H__)
