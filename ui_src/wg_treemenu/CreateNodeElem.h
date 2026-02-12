#if !defined(AFX_CREATENODEELEM_H__869AD1CC_3380_11D3_8899_0000C0A2329D__INCLUDED_)
#define AFX_CREATENODEELEM_H__869AD1CC_3380_11D3_8899_0000C0A2329D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateNodeElem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CreateNodeElemDlg dialog

class CreateNodeElemDlg : public CDialog
{
// Construction
public:
	CreateNodeElemDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CreateNodeElemDlg)
	enum { IDD = IDD_TM_NODEELEM };
	CMouseEdit	m_wndCoord;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CreateNodeElemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CreateNodeElemDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATENODEELEM_H__869AD1CC_3380_11D3_8899_0000C0A2329D__INCLUDED_)
