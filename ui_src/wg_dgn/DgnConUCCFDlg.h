#if !defined(AFX_DGNCONUCCFDLG_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_)
#define AFX_DGNCONUCCFDLG_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConUCCFDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConUCCFDlg dialog
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

class CDgnConUCCFDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CDgnConUCCFDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }
	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Initial_SelectItem();
	void Initial_Data();
	void Update_InitDataByCode();
	
	// Dialog Data
	//{{AFX_DATA(CDgnConUCCFDlg)
	enum { IDD = IDD_DGN_CON_UCCF_DLG };
	int		m_nOption;
	double	m_dGammard;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConUCCFDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnUCCFAr();
	afx_msg void OnDgnUCCFDel();
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	CDBDoc* m_pDoc;
	
	// Generated message map functions
	//{{AFX_MSG(CDgnConUCCFDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONUCCFDLG_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_)
