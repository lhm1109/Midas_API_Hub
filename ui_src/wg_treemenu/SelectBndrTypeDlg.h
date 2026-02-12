#if !defined(AFX_SELECTBNDRTYPEDLG_H__ACE7F49E_6946_4F65_B2CD_AE6A93648AC9__INCLUDED_)
#define AFX_SELECTBNDRTYPEDLG_H__ACE7F49E_6946_4F65_B2CD_AE6A93648AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectBndrTypeDlg.h : header file
//
#include "..\MIT_frx\MCheckListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectBndrTypeDlg dialog

class CSelectBndrTypeDlg : public CDialogMove
{
// Construction
public:
	CArray<int,int> m_TypeArray;
	CSelectBndrTypeDlg(CWnd* pParent = NULL);   // standard constructor
	void GetTypes(CArray<int, int>& Types);
// Dialog Data
	//{{AFX_DATA(CSelectBndrTypeDlg)
	enum { IDD = IDD_TM_SEL_BNDR_TYPE };
	mit::frx::MCheckListBox	m_wndBndrTypeList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectBndrTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DataExchange(CArray<int, int>& aItemChecked, int nOperation);

	// Generated message map functions
	//{{AFX_MSG(CSelectBndrTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTBNDRTYPEDLG_H__ACE7F49E_6946_4F65_B2CD_AE6A93648AC9__INCLUDED_)
