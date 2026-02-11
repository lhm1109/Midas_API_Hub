#if !defined(AFX_HEALDROPLOAD_AIJRC_WALK_H__AB99398D_D295_498A_9463_90315B9AF8F6__INCLUDED_)
#define AFX_HEALDROPLOAD_AIJRC_WALK_H__AB99398D_D295_498A_9463_90315B9AF8F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HealDropLoad_AIJRC_walk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// HealDropLoad_AIJRC_walk dialog
#include "HeelLoadChildDialog.h"
class HealDropLoad_AIJRC_walk : public CHeelLoadChildDialog
{
// Construction
public:
	HealDropLoad_AIJRC_walk(CWnd* pParent = NULL);   // standard constructor
	void End();
	void MakeSpectrumData();
	BOOL ISVALID();
// Dialog Data
	//{{AFX_DATA(HealDropLoad_AIJRC_walk)
	enum { IDD = IDD_HEELDROPLOAD_AIJRC_WALK };
	double	m_dFs;
	double	m_dTimeStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HealDropLoad_AIJRC_walk)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(HealDropLoad_AIJRC_walk)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEALDROPLOAD_AIJRC_WALK_H__AB99398D_D295_498A_9463_90315B9AF8F6__INCLUDED_)
