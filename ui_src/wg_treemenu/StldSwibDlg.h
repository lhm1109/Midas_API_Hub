#if !defined(AFX_STLDSWIBDLG_H__FF460457_431A_471B_8A36_582FB9517137__INCLUDED_)
#define AFX_STLDSWIBDLG_H__FF460457_431A_471B_8A36_582FB9517137__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldSwibDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStldSwibDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

class CStldSwibDlg : public CMenuBarChildDlg
{
// Construction
public:
	void CtrlMan();
	CStldSwibDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldSwibDlg();
	virtual void Execute() { OnTmExecute(); }
										 // T_LDGR_K *
	void GetCurGroupKey(void* pLGKey);

// Dialog Data
	//{{AFX_DATA(CStldSwibDlg)
	enum { IDD = IDD_TM_STLD_SWIB };
	CMouseEdit	m_wnd2ndNode;
	CMouseEdit	m_wnd1stNode;
	CSelectLC	m_LoadCase;
	CCobxLdgr	m_wndLdgrCobx;
	int		m_nOption;
	BOOL	m_b2ndNode;
	CString	m_strXFactor;
	CString	m_strYFactor;
	CString	m_strZFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldSwibDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldSwibDlg)
	afx_msg void OnTmBtnLc();
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnTmOptionRadio();
	afx_msg void OnTm2ndNodeCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnTmExecute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMouseEditEnter  (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDSWIBDLG_H__FF460457_431A_471B_8A36_582FB9517137__INCLUDED_)
