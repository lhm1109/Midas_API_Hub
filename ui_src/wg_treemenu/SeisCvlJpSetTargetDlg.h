#if !defined(__SEIS_CVL_JP_SET_TARGET_DLG_H__)
#define __SEIS_CVL_JP_SET_TARGET_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCvlJpSetTargetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetTargetDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "BaseDockingView.h"

class CSeisCvlJpSetTargetDlg : public MBaseDockingView
{
private:
	CDBDoc* m_pDoc;

protected:
	CSeisCvlJpSetTargetDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSeisCvlJpSetTargetDlg)
	virtual ~CSeisCvlJpSetTargetDlg();

public:
	//{{AFX_DATA(CSeisCvlJpSetTargetDlg)
	enum { IDD = IDD_TM_SIGR };
	
	//}}AFX_DATA

	virtual BOOL ShowChildDlgByID(int nID);
	virtual void OnInitialUpdate();
	
private:
	void AddChildDlgs();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlJpSetTargetDlg)
	//virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlJpSetTargetDlg)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL ExistElementAtSigrUser(const T_SIGR_K& sigrK, const T_KEY_LIST& elemKeys);
	BOOL ExistElementAtSigrUpper(const T_SIGR_K& sigrK, const T_KEY_LIST& elemKeys);
	BOOL ExistElementAtSigrLower(const T_SIGR_K& sigrK, const T_KEY_LIST& elemKeys);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEIS_CVL_JP_SET_TARGET_DLG_H__)