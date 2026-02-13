#if !defined(AFX_DGNCONCVLRSMODFACTORDLG_H__01BFEEE8_2420_4E09_8FAB_FB09D14475E9__INCLUDED_)
#define AFX_DGNCONCVLRSMODFACTORDLG_H__01BFEEE8_2420_4E09_8FAB_FB09D14475E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCvlRSModFactorDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConCvlRSModFactorDlg dialog

class CDgnConCvlRSModFactorDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConCvlRSModFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }
// Dialog Data
	//{{AFX_DATA(CDgnConCvlRSModFactorDlg)
	enum { IDD = IDD_DGN_CON_CVL_RS_MOD_FACTOR_DLG };

	int		m_nOption;
	double	m_dFx;
	double	m_dFy;
	double	m_dFz;
	double	m_dMx;
	double	m_dMy;
	double	m_dMz;	
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCvlRSModFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConCvlRSModFactorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnAddReplaceRdo();
	afx_msg void OnDgnDelRdo();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnClose();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONCVLRSMODFACTORDLG_H__01BFEEE8_2420_4E09_8FAB_FB09D14475E9__INCLUDED_)
