// DgnEngineProgressDlg.h: interface for the CDgnEngineProgressDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNENGINEPROGRESSDLG_H__71E9834F_B774_4E90_9B03_42A574D8E201__INCLUDED_)
#define AFX_DGNENGINEPROGRESSDLG_H__71E9834F_B774_4E90_9B03_42A574D8E201__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DgnProgressDlg.h"
#include "..\..\dgnengine\src\DgnBase\XLProgressStruct.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnEngineProgressDlg : public CDgnProgressDlg
{
// Construction
public:
	CDgnEngineProgressDlg(CWnd* pParent = NULL);   // standard constructor

protected:
	D_XL_PROGRESS_INTERFACE m_ProgressInte;
	CString  m_strTitle_Pre;
	CString	 m_strMessage_Pre;
	int		 m_nPercent_Pre;

public:
	D_XL_PROGRESS_INTERFACE* Get_InterfacePoint();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnEngineProgressDlg)
	//	
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnEngineProgressDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
#endif // !defined(AFX_DGNENGINEPROGRESSDLG_H__71E9834F_B774_4E90_9B03_42A574D8E201__INCLUDED_)
