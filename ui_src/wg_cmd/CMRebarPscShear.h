#if !defined(__CMREBARPSCSHEAR_H__)
#define __CMREBARPSCSHEAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarPscShear.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

class CCMRebarPscShear_I;
class CCMRebarPscShear_J;
/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscShear dialog
class CCMRebarPscDlg;
class CCMRebarPscShear : public CChildDialog
{
// Construction
public:
	CCMRebarPscShear(CWnd* pParent = NULL);   // standard constructor

// interface
public:
	void SetCurData(T_RPSC_D* pData);
	BOOL GetCurData(T_RPSC_D* pData);

// Dialog Data
	//{{AFX_DATA(CCMRebarPscShear)
	enum { IDD = IDD_CMD_REBAR_PSC_SHEAR };
	CDlgTabCtrl	m_wndTabCtrl;
	CButton	m_chkSameIJ;
	CBCGPStatic	m_wndHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarPscShear)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_RPSC_D m_Data;
	CCMRebarPscDlg* m_pParent;

	CCMRebarPscShear_I* m_pRebarI;
	CCMRebarPscShear_J* m_pRebarJ;

protected:
	void LayoutChildDlg();
	void Data2Dlg();
	BOOL Dlg2Data(T_RPSC_D* pData);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMRebarPscShear)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSameCheck();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARPSCSHEAR_H__)
