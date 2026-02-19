#if !defined(__DGNSTLENVELOPEDLG_H__)
#define __DGNSTLENVELOPEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlEnvelopeDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\MIT_frx\MButton.h"
#include "DgnStlCodeKRDlg.h"

#include "DgnStlEnvelopeDlg.h"
#include "HeaderPre.h"

using namespace mit::frx;
class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlEnvelopeDlg dialog
class __MY_EXT_CLASS__ CDgnStlEnvelopeDlg : public CDialogMove
{
// Construction
public:
	CDgnStlEnvelopeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnStlEnvelopeDlg)
	enum { IDD = IDD_DGN_STL_ENVELOPE_DLG };

	MButton	m_chkPmax;
	MButton	m_chkPmin;
	MButton	m_chkVymax;
	MButton	m_chkVymin;
	MButton	m_chkVzmax;
	MButton	m_chkVzmin;
	MButton	m_chkTmax;
	MButton	m_chkTmin;
	MButton	m_chkMymax;
	MButton	m_chkMymin;
	MButton	m_chkMzmax;
	MButton	m_chkMzmin;

	MButton	m_btnSelectAll;
	MButton	m_btnUnSelectAll;
	//}}AFX_DATA
	
public:

	T_ENVELOPE_DATA m_EnvelopeData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlEnvelopeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlEnvelopeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();	
	afx_msg void OnDgnEnvelopeBtn();
	afx_msg void OnDgnSelectAllBtn();
	afx_msg void OnDgnUnSelectAllBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

#endif 