#if !defined(AFX_SEIS_LOAD_CLASSIFY_METHOD_DLG_H__INCLUDED_)
#define AFX_SEIS_LOAD_CLASSIFY_METHOD_DLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisLoadClassifyMethodDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CSeisLoadClassifyMethodDlg dialog

class CSeisLoadClassifyMethodDlg : public CDialogMove
{
// Construction
public:
	CSeisLoadClassifyMethodDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSeisLoadClassifyMethodDlg)
	enum { IDD = IDD_ETC_SEIS_CLASSIFY_METHOD_DLG };
	CEditUnit	m_edtGroundLevel;
	CEditUnit	m_edtBedrockLevel;
	CEditUnit	m_edtFootingLevel;
	CTextUnit	m_wndGroundUnit;
	CTextUnit	m_wndFootingUnit;
	CTextUnit	m_wndBedrockUnit;
	//}}AFX_DATA

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisLoadClassifyMethodDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nMethod;

protected:
	void InitialData();

	// Generated message map functions
	//{{AFX_MSG(CSeisLoadClassifyMethodDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdClassify();
	afx_msg void OnCmdCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEIS_LOAD_CLASSIFY_METHOD_DLG_H__INCLUDED_)
