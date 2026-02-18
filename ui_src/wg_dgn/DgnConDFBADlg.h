#if !defined(AFX_DGNCONDFBADLG_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_)
#define AFX_DGNCONDFBADLG_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConDFBADlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConDFBADlg dialog
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

class CDgnConDFBADlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConDFBADlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Initial_SelectItem();
	void Initial_Data();
	void Update_InitDataByCode();

// Dialog Data
	//{{AFX_DATA(CDgnConDFBADlg)
	enum { IDD = IDD_DGN_CON_DFBA_DLG };
	int		m_nOption;
	int		m_nForceType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConDFBADlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnDFBAAr();
	afx_msg void OnDgnDFBADel();
	afx_msg void OnDgnDFBAEqui();
	afx_msg void OnDgnDFBAEach();

	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConDFBADlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONDFBADLG_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_)
