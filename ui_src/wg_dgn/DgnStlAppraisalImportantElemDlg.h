#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnStlAppraisalImportantElemDlg dialog

class CDBDoc;

class CDgnStlAppraisalImportantElemDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlAppraisalImportantElemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CDgnStlAppraisalImportantElemDlg)
	enum {IDD = IDD_DGN_STL_IMEL_DLG};
	int		m_nOption;

	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlAppraisalImportantElemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnStlAppraisalImportantElemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnImelAr();
	afx_msg void OnDgnImelDel();
	afx_msg void OnDgnImelSel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

