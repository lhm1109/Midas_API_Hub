#if !defined(__SEISCOMMONPMCMDLG_H__)
#define      __SEISCOMMONPMCMDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCommonPmcmDlg.h : header file

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonPmcmDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"

class CSeisCommonPmcmDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CSeisCommonPmcmDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCommonPmcmDlg();

public:
	virtual void Execute();

	// Dialog Data
	//{{AFX_DATA(CSeisCommonPmcmDlg)
	enum { IDD = IDD_TM_SEISCOMMON_PMCM };
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCommonPmcmDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CDBDoc* m_pDoc;
	int m_nAddDel; // 0:Add/Replace, 1:Delete
	int m_nElemType;	// 0:RC Column, 1:Wall
	int m_nCalcMethod;	// 0: Keep M constant, 1: Keep P constant
	
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSeisCommonPmcmDlg)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISCOMMONPMCMDLG_H__)
