#if !defined(AFX_CMGRIDP1LOADFACTORDLG_H__060C0141_D171_4E2D_8422_759F3D00CA55__INCLUDED_)
#define AFX_CMGRIDP1LOADFACTORDLG_H__060C0141_D171_4E2D_8422_759F3D00CA55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridP1LoadFactorDlg.h : header file
//

#include "..\wg_db\wg_db_CobxP1lt.h"

/////////////////////////////////////////////////////////////////////////////
// CCMGridP1LoadFactorDlg dialog
#include "CMChildBarBase.h"
class CCMGridP1LoadFactorDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMGridP1LoadFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CCMGridP1LoadFactorDlg)
	enum { IDD = IDD_CMD_GRID_P1LA_DLG };
	CCobxP1lt	m_comboLoadType;
	CEdit	m_wndLoadItem;
	int		m_nFactorType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridP1LoadFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	BOOL m_bModify;
	void Data2Dlg(T_P1LA_D* pData);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMGridP1LoadFactorDlg)
	virtual BOOL OnInitDialog();
	virtual void OnTmExecute();
	virtual void OnTmClose();
	afx_msg void OnCmdMoreButton();
	afx_msg void OnSelchangeLoadtypeCombo();
	afx_msg void OnTmConsAddDelRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMGRIDP1LOADFACTORDLG_H__060C0141_D171_4E2D_8422_759F3D00CA55__INCLUDED_)
