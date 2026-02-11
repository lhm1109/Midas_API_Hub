#if !defined(AFX_WINDBSFACTOR_H__CD6B2C4B_180E_48FD_B7A1_F599458C7F41__INCLUDED_)
#define AFX_WINDBSFACTOR_H__CD6B2C4B_180E_48FD_B7A1_F599458C7F41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindBSFactor.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CWindBSFactor dialog

class CWindBSFactor : public CDialogMove
{
// Construction
public:
	CWindBSFactor(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CWindBSFactor)
	enum { IDD = IDD_ETC_WIND_BS_WSP_DLG };
	CEditUnit	m_wndBS6399Ss;
	CEditUnit	m_wndBS6399Sp;
	CEditUnit	m_wndBS6399Sd;
	CEditUnit	m_wndBS6399Sa;
	CEditUnit	m_wndBS6399Sh;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindBSFactor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int nProcedure;
	T_WIND_D  m_Data;

protected:
	// Generated message map functions
	//{{AFX_MSG(CWindBSFactor)
	afx_msg void OnCmdBsWspOk();
	afx_msg void OnCmdBsWspCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDBSFACTOR_H__CD6B2C4B_180E_48FD_B7A1_F599458C7F41__INCLUDED_)
