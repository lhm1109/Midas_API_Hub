#if !defined(AFX_SELECTLOADTYPEDLG_H__12F7998D_A895_4EC9_94DC_A7C17FEF07DF__INCLUDED_)
#define AFX_SELECTLOADTYPEDLG_H__12F7998D_A895_4EC9_94DC_A7C17FEF07DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectLoadTypeDlg.h : header file
//
#include "..\MIT_frx\MCheckListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectLoadTypeDlg dialog

class CSelectLoadTypeDlg : public CDialogMove
{
// Construction
public:
	CSelectLoadTypeDlg(CWnd* pParent = NULL);   // standard constructor
	CArray<int,int> m_TypeArray;
	void GetTypes(CArray<int, int>& Types);

// Dialog Data
	//{{AFX_DATA(CSelectLoadTypeDlg)
	enum { IDD = IDD_TM_SEL_LOAD_TYPE };
	mit::frx::MCheckListBox	m_wndLoadTypeList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectLoadTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DataExchange(CArray<int, int>& aItemChecked, int nOperation);

	// Generated message map functions
	//{{AFX_MSG(CSelectLoadTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTLOADTYPEDLG_H__12F7998D_A895_4EC9_94DC_A7C17FEF07DF__INCLUDED_)
