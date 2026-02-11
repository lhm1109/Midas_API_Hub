#if !defined(__CMThgcIncrStepDlg_H__)
#define __CMThgcIncrStepDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThgcIncrStepDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThgcIncrStepDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMThgcIncrStepDlg : public CDialogMove
{
// Construction
public:
	CCMThgcIncrStepDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMThgcIncrStepDlg)
	enum { IDD = IDD_ETC_THGC_INC_STEP_DLG };
	CFormulaEditSpin m_edtIncStep;
	CEditUnit m_edtUserInputStep;
	//}}AFX_DATA

public:
	void SetThgc(T_THGC_D& ThgcD) { m_ThgcD = ThgcD;}
	T_THGC_D GetThgc() { return m_ThgcD;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThgcIncrStepDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_THGC_D m_ThgcD;
	void Data2Dlg();
	void Dlg2Data();

	// Generated message map functions
	//{{AFX_MSG(CCMThgcIncrStepDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnResultOutputRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CArray<UINT,UINT> m_aCtrl_ResultOutput;
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMThgcIncrStepDlg_H__)
