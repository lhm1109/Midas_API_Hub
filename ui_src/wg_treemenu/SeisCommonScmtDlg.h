#if !defined(__SEISCOMMONSCMTDLG_H__)
#define      __SEISCOMMONSCMTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCommonScmtDlg.h : header file

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonScmtDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CSeisCommonScmtDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CSeisCommonScmtDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCommonScmtDlg();

public:
	virtual void Execute();

	// Dialog Data
	//{{AFX_DATA(CSeisCommonScmtDlg)
	enum { IDD = IDD_TM_SEISCOMMON_SCMT };
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCommonScmtDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

protected:
	CDBDoc* m_pDoc;
	int m_nAddDel; // 0:Add/Replace, 1:Delete
	int m_nCompType; // 0:Pri., 1:Sec., 2:None

	MComboBox m_cmbCcpt;
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSeisCommonScmtDlg)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISCOMMONSCMTDLG_H__)
