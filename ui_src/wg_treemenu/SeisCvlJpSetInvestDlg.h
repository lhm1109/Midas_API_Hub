#if !defined(__SEIS_CVL_JP_SET_INVEST_DLG_H__)
#define __SEIS_CVL_JP_SET_INVEST_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCvlJpSetInvestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "BaseDockingView.h"

class CSeisCvlJpSetInvestDlg : public MBaseDockingView
{
protected:
	CSeisCvlJpSetInvestDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSeisCvlJpSetInvestDlg)
	virtual ~CSeisCvlJpSetInvestDlg();

public:
	//{{AFX_DATA(CSeisCvlJpSetInvestDlg)
	enum { IDD = IDD_TM_SEIS_CVL_JP };

	//}}AFX_DATA

	virtual BOOL ShowChildDlgByID(int nID);
	virtual void OnInitialUpdate();

private:
	void AddChildDlgs();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlJpSetInvestDlg)
	//virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetInvestDlg)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_INVEST_DLG_H__)