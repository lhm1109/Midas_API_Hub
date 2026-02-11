#if !defined(__IehpStiffnessOptionDlg_H__)
#define __IehpStiffnessOptionDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpStiffnessOptionDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CIehpStiffnessOptionDlg : public CDialogMove
{
	// Construction
public:
	CIehpStiffnessOptionDlg(BOOL bUseNonlinearShearY, BOOL bUseNonlinearShearz, CWnd* pParent = NULL);   // standard constructor
	~CIehpStiffnessOptionDlg();

	// Dialog Data
	//{{AFX_DATA(CIehpStiffnessOptionDlg)
	enum { IDD = IDD_ETC_IEHP_STIFFNESS_OPTION_DLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpStiffnessOptionDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIehpStiffnessOptionDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//afx_msg void OnInteractionType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Implementation
public:
	BOOL  m_bUseNonlinearShearY;
	BOOL  m_bUseNonlinearShearZ;
	CRect  m_rectPosition;

protected:
	void MoveDlg();

	CArray<UINT, UINT> m_aCtrl_Point;
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__IehpStiffnessOptionDlg_H__)
