#if !defined(AFX_TPSGCONVERTDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_)
#define AFX_TPSGCONVERTDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TpsgConvertDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTpsgConvertDlg dialog

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CTpsgConvertDlg : public CDialogMove
{
// Construction
public:
	CTpsgConvertDlg(CWnd* pParent = NULL);   // standard constructor
	void SetSelStrings(CArray<CString,CString&>& aSelStrings);
	CArray<CString,CString&> m_aSelStrings;

// Dialog Data
	//{{AFX_DATA(CTpsgConvertDlg)
	enum { IDD = IDD_TREEMENU_TPSG_CONVERT_DLG };
	MButton	m_chkSuffix;
	CEdit	m_editSuffix;
	int		m_nStartNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTpsgConvertDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTpsgConvertDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTreemenuTpsgUseSuffix();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TPSGCONVERTDLG_H__36077B72_59F2_473B_994A_CF00F5718EEF__INCLUDED_)
