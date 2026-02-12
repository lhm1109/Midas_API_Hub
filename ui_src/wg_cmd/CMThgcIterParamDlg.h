#if !defined(__CMThgcIterParamDlg_H__)
#define __CMThgcIterParamDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThgcIterParamDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThgcIterParamDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMThgcIterParamDlg : public CDialogMove
{
// Construction
public:
	CCMThgcIterParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMThgcIterParamDlg)
	enum { IDD = IDD_ETC_THGC_ITER_PARAM_DLG };
	//}}AFX_DATA

	MButton   m_chkPermitFail;
	CEditUnit m_edtSubstep;
	CFormulaEditSpin m_edtMaxIter;
	MButton   m_chkDispNorm;
	MButton   m_chkForcNorm;
	MButton   m_chkEngyNorm;
	CEditUnit m_edtDispNorm;
	CEditUnit m_edtForcNorm;
	CEditUnit m_edtEngyNorm;

	MButton   m_chkUseLineSearch;
	CEditUnit m_edtLineSearchNum;
public:
	void SetThgc(T_THGC_D& ThgcD) { m_ThgcD = ThgcD;}
	T_THGC_D GetThgc() { return m_ThgcD;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThgcIterParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_THGC_D m_ThgcD;
	CArray<UINT, UINT> m_aLineSearch;

	void Data2Dlg();
	void Dlg2Data();

	// Generated message map functions
	//{{AFX_MSG(CCMThgcIterParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnChkCriteria();
	afx_msg void OnUseLineSearch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMThgcIterParamDlg_H__)
