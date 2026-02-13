#if !defined(AFX_DgnStlPositionJPDlg_H__)
#define AFX_DgnStlPositionJPDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlPositionJPDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnStlPositionJPDlg dialog

class CDgnStlPositionJPDlg : public CMenuBarChildDlg
{
	// Construction
public:
	CDgnStlPositionJPDlg(CWnd* pParent = NULL);   // standard constructor
	//virtual void Execute() {}

	// Dialog Data
		//{{AFX_DATA(CDgnStlPositionJPDlg)
	enum { IDD = IDD_DGN_STL_POSITION_JP_DLG };

	int	m_nOption;
	int m_nPart;
	int m_iChkMethod;
	int m_iChkAxial;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlPositionJPDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void EnableDisableControls();

	// Generated message map functions
	//{{AFX_MSG(CDgnStlPositionJPDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnExecute();
// 	afx_msg void OnDgnClose();
	afx_msg void OnDgnArDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnStlPositionJPDlg_H__)
