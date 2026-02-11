#if !defined(AFX_CMMVLDITEMYLOADGRID_H__DF3BF774_5255_43D5_89C4_39B172F3C4D7__INCLUDED_)
#define AFX_CMMVLDITEMYLOADGRID_H__DF3BF774_5255_43D5_89C4_39B172F3C4D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemYLoadGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemYLoadGrid dialog

class CCMMvldItemYLoadGrid : public CDialog
{
// Construction
public:
	CCMMvldItemYLoadGrid(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMMvldItemYLoadGrid)
	enum { IDD = IDD_CMD_ML_LLAN_INFO_JAPAN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemYLoadGrid)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemYLoadGrid)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMYLOADGRID_H__DF3BF774_5255_43D5_89C4_39B172F3C4D7__INCLUDED_)
