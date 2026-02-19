#if !defined(__DGNREBARPSCSHEAR_H__)
#define __DGNREBARPSCSHEAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRebarPscShear.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\ChildDialog.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CDgnRebarPscShear_I;
class CDgnRebarPscShear_J;
/////////////////////////////////////////////////////////////////////////////
// CDgnRebarPscShear dialog
class CDgnRebarPscDlg;
class CDgnRebarPscShear : public CChildDialog
{
// Construction
public:
	CDgnRebarPscShear(CWnd* pParent = NULL);   // standard constructor

// interface
public:
	void SetCurData(T_RPSC_D* pData);
	BOOL GetCurData(T_RPSC_D* pData);

// Dialog Data
	//{{AFX_DATA(CDgnRebarPscShear)
	enum { IDD = IDD_DGN_REBAR_PSC_SHEAR };
	CDlgTabCtrl	m_wndTabCtrl;
	MButton	m_chkSameIJ;
	CBCGPStatic	m_wndHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRebarPscShear)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_RPSC_D m_Data;
	CDgnRebarPscDlg* m_pParent;

	CDgnRebarPscShear_I* m_pRebarI;
	CDgnRebarPscShear_J* m_pRebarJ;

protected:
	void LayoutChildDlg();
	void Data2Dlg();
	BOOL Dlg2Data(T_RPSC_D* pData);

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnRebarPscShear)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSameCheck();
	afx_msg void OnSelchangeDgnTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNREBARPSCSHEAR_H__)
