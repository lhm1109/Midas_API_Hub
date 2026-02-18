#if !defined(AFX_DgnConFpswElemWallIDDlg_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_)
#define AFX_DgnConFpswElemWallIDDlg_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConFpswElemWallIDDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConFpswElemWallIDDlg dialog
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
class CDgnConFpswElemWallIDDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConFpswElemWallIDDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Initial_SelectItem();
	void Initial_Unit();
	void Initial_Data();

// Dialog Data
	//{{AFX_DATA(CDgnConFpswElemWallIDDlg)
	enum { IDD = IDD_DGN_CON_FPSW_ELEM_WALLID_DLG };
	int		m_nOption;
	BOOL    m_bAutoCheck;
	CEditUnit m_edtT1;
	CTextUnit m_untT1;


	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConFpswElemWallIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnFPSWAr();
	afx_msg void OnDgnFPSWDel();

	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void EnableControl();
	BOOL GetEnableCtrl();

	// Generated message map functions
	//{{AFX_MSG(CDgnConFpswElemWallIDDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnConFpswElemWallIDDlg_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_)
