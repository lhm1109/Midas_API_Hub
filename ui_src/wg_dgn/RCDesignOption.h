// RCDesignOption.h: interface for the CRCDesignOption class.
// Add by sshan. MNET:2607.('20061220)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RCDESIGNOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_RCDESIGNOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

class CDBDoc;

class CRCDesignOption : public CMenuBarChildDlg
{
public:
	CRCDesignOption(CWnd* pParent = NULL);
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CRCDesignOption)
	enum { IDD = IDD_DGN_RC_DESIGN_OPTION };
	int		m_nOption;
	int		m_nMOption;
	int		m_nSOption;
	int		m_nAOption;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCDesignOption)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CRCDesignOption)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnCRCDorcAr();
	afx_msg void OnDgnCRCDorcDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCDESIGNOPTION_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
