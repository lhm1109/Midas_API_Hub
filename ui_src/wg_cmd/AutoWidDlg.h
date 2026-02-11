//#if !defined(AFX_AUTOWIDDLG_H__B3BEC582_558D_4B2A_BC1D_3D3EFF64CF01__INCLUDED_)
//#define AFX_AUTOWIDDLG_H__B3BEC582_558D_4B2A_BC1D_3D3EFF64CF01__INCLUDED_

#if !defined(AFX_AUTOWIDDLG_H__)
#define AFX_AUTOWIDDLG_H__


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoWidDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
/////////////////////////////////////////////////////////////////////////////
// CAutoWidDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CAutoWidDlg : public CDialogMove
{
// Construction
public:
	CAutoWidDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoWidDlg)
	enum { IDD = IDD_ETC_AUTOWID_DLG };
	CComboBox	m_wndStoryNameCombo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoWidDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	
	// Generated message map functions
	//{{AFX_MSG(CAutoWidDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnSelectTypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	CArray<UINT , UINT> m_aRdoSelectType;
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOWIDDLG_H__B3BEC582_558D_4B2A_BC1D_3D3EFF64CF01__INCLUDED_)


