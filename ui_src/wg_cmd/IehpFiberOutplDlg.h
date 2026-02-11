#if !defined(__IehpFiberOutplDlg_H__)
#define __IehpFiberOutplDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpFiberOutplDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CIehpFiberOutplDlg : public CDialogMove 
{
	// Construction
public:
	CIehpFiberOutplDlg(int nUseIehc, BOOL bConsOutpl, BOOL bPushover, CWnd* pParent = NULL);   // standard constructor
	~CIehpFiberOutplDlg();

	// Dialog Data
	//{{AFX_DATA(CIehpFiberOutplDlg)
	enum { IDD = IDD_ETC_IEHP_OUTPL_FIBER_DLG };
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpFiberOutplDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIehpFiberOutplDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Implementation
public:
	int  m_nUseIehc     ;
	BOOL m_bConsOutpl   ;
	int m_nAutoUserLocMemb;
	int m_nLocMemb        ;
	CRect  m_rectPosition;
	BOOL m_bPushover;

protected:
	void MoveDlg();
	void CtrlManager();

	CArray<UINT, UINT> m_aCtrl_Point;
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__IehpFiberOutplDlg_H__)
