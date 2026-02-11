#if !defined(__SEISCOMMONSERDDLG_H__)
#define      __SEISCOMMONSERDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCommonSerdDlg.h : header file

/////////////////////////////////////////////////////////////////////////////
// CSeisCommonSerdDlg dialog
#include "..\wg_base\wg_base_ControlEx.h"

class CSeisCommonSerdDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CSeisCommonSerdDlg(CWnd* pParent = NULL);   // standard constructor
	~CSeisCommonSerdDlg();

public:
	virtual void Execute();

	// Dialog Data
	//{{AFX_DATA(CSeisCommonSerdDlg)
	enum { IDD = IDD_TM_SEISCOMMON_SERD };
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCommonSerdDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

protected:
	CDBDoc* m_pDoc;
	int m_nAddDel; // 0:Add/Replace, 1:Delete
	int m_nReinforcement; // 0:135, 1:90, 2:other
	
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSeisCommonSerdDlg)
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISCOMMONSERDDLG_H__)
