#if !defined(__IehpMphiLocDlg_H__)
#define __IehpMphiLocDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpMphiLocDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CIehpMphiLocDlg : public CDialogMove 
{
// Construction
public:
	CIehpMphiLocDlg(int nAutoUserLocMemb, int nLocMemb, CWnd* pParent = NULL);   // standard constructor
	~CIehpMphiLocDlg();
	
	// Dialog Data
	//{{AFX_DATA(CIehpMphiLocDlg)
	enum { IDD = IDD_ETC_IEHP_MPHI_LOC_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpMphiLocDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIehpMphiLocDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void CtrlManager();
	//afx_msg void OnInteractionType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
// Implementation
public:
	int m_nAutoUserLocMemb;
	int m_nLocMemb        ;
	CRect  m_rectPosition;

protected:
	void MoveDlg();

	CArray<UINT, UINT> m_aCtrl_Point;
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__IehpMphiLocDlg_H__)
