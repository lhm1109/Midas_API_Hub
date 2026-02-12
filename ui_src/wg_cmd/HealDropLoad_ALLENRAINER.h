#if !defined(AFX_HEALDROPLOAD_ALLENRAINER_H__B2DF6542_2B1E_40D9_8A4C_A13199039049__INCLUDED_)
#define AFX_HEALDROPLOAD_ALLENRAINER_H__B2DF6542_2B1E_40D9_8A4C_A13199039049__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HealDropLoad_ALLENRAINER.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHealDropLoad_ALLENRAINER dialog
#include "HeelLoadChildDialog.h"
class CHealDropLoad_ALLENRAINER : public CHeelLoadChildDialog
{
// Construction
public:
	CHealDropLoad_ALLENRAINER(CWnd* pParent = NULL);   // standard constructor
	void End();
	void MakeSpectrumData();
	BOOL ISVALID();
// Dialog Data
	//{{AFX_DATA(CHealDropLoad_ALLENRAINER)
	enum { IDD = IDD_HEELDROPLOAD_ALLENRAINER_RUN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHealDropLoad_ALLENRAINER)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHealDropLoad_ALLENRAINER)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEALDROPLOAD_ALLENRAINER_H__B2DF6542_2B1E_40D9_8A4C_A13199039049__INCLUDED_)
