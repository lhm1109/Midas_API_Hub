#if !defined(AFX_HEALDROPLOAD_AIJRC_RUN_H__93F1D52A_7EFA_4623_A453_B83FCB2BD086__INCLUDED_)
#define AFX_HEALDROPLOAD_AIJRC_RUN_H__93F1D52A_7EFA_4623_A453_B83FCB2BD086__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HealDropLoad_AIJRC_Run.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_AIJRC_Run dialog
#include "HeelLoadChildDialog.h"
class CHealDropLoad_AIJRC_Run : public CHeelLoadChildDialog
{
// Construction
public:
	CHealDropLoad_AIJRC_Run(CWnd* pParent = NULL);   // standard constructor
	void End();
	void MakeSpectrumData();
	BOOL ISVALID();

// Dialog Data
	//{{AFX_DATA(CHealDropLoad_AIJRC_Run)
	enum { IDD = IDD_HEELDROPLOAD_AIJRC_RUN };
	double	m_dFs;
	double	m_dTimeStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHealDropLoad_AIJRC_Run)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHealDropLoad_AIJRC_Run)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEALDROPLOAD_AIJRC_RUN_H__93F1D52A_7EFA_4623_A453_B83FCB2BD086__INCLUDED_)
