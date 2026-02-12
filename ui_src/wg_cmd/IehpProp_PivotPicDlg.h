#if !defined(AFX_IEHPPROP_PIVOTPICDLG_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_)
#define AFX_IEHPPROP_PIVOTPICDLG_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IehpProp_PivotPicDlg.h : header file
//
#include "CMDlgBase.h"
#include "../MIT_frx/MillustViewer.h"
/////////////////////////////////////////////////////////////////////////////
// CIehpProp_PivotPicDlg dialog

using namespace mit::frx;
class CIehpProp_PivotPicDlg : public CCMDlgBase
{
// Construction
public:
	CIehpProp_PivotPicDlg(CWnd* pParent = NULL);   // standard constructor
	~CIehpProp_PivotPicDlg();

public:
																						// Data가 Setting되어 있어야 한다.
// Dialog Data
	//{{AFX_DATA(CIehpProp_PivotPicDlg)
	enum { IDD = IDD_CMD_MULTI_PIVOT_PIC_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpProp_PivotPicDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	void MoveWindow();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CIehpProp_PivotPicDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	MillustViewer m_wndPicture;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEHPPROP_PIVOTPICDLG_H__9CE8CA6D_4909_43EA_BD01_5CCD6113B51E__INCLUDED_)
