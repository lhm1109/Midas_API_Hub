#if !defined(AFX_STLDTENDONDLG_H__3931C25B_EC39_4735_A648_ACE05E6EA28E__INCLUDED_)
#define AFX_STLDTENDONDLG_H__3931C25B_EC39_4735_A648_ACE05E6EA28E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldTendonDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStldTendonDlg dialog
#include "..\wg_db\wg_db_DBDoc.h"


class CStldTendonDlg : public CMenuBarChildDlg , public CDBUpdateConnector
{
// Construction
public:
	CStldTendonDlg(CWnd* pParent = NULL);   // standard constructor
	~CStldTendonDlg();
	virtual void Execute() {}
	void SetListHeader();
// Dialog Data
	//{{AFX_DATA(CStldTendonDlg)
	enum { IDD = IDD_TM_STLD_ZASSIGN_TENDON };
	CListCtrl	m_wndTendons;
	//}}AFX_DATA
	
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldTendonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldTendonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnTmTendonTypebtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDTENDONDLG_H__3931C25B_EC39_4735_A648_ACE05E6EA28E__INCLUDED_)
