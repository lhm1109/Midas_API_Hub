#if !defined(__SeisCommonSechDlg_H__)
#define      __SeisCommonSechDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCommonSechDlg.h : header file

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonSechDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CSeisCommonSechDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CSeisCommonSechDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCommonSechDlg();

public:
	virtual void Execute();
	  
	// Dialog Data
	//{{AFX_DATA(CSeisCommonSechDlg)
	enum { IDD = IDD_TM_SEISCOMMON_SECH };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCommonSechDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

protected:
	CDBDoc* m_pDoc;
	int m_nAddDel; // 0:Add/Replace, 1:Delete
	CEditUnit m_edth0y;
	CTextUnit m_unth0y;
	CEditUnit m_edth0z;
	CTextUnit m_unth0z;

	MComboBox m_cmbCcpt;
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSeisCommonSechDlg)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SeisCommonSechDlg_H__E3A50C03_6049_11D3_8899_0000C0A2329D__INCLUDED_)
