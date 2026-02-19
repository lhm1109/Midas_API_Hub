#if !defined(AFX_DgnConEBarDesignMethodWallIDDlg_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_)
#define AFX_DgnConEBarDesignMethodWallIDDlg_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConBndrElemWallIDDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDgnConEBarDesignMethodWallIDDlg dialog
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
class CDgnConEBarDesignMethodWallIDDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConEBarDesignMethodWallIDDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Initial_SelectItem();
	void Initial_Data();

// Dialog Data
	enum { IDD = IDD_DGN_CON_EBAR_METHOD_WALLID_DLG };
	int		m_nOption;
	BOOL  m_bApplyToSameWallId;
	int   m_nEBarMethod;
	int   m_nBEEBarMethod;
	
	T_EBMW_D m_CurData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConEBarDesignMethodWallIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnBEMWAr();
	afx_msg void OnDgnBEMWDel();
	afx_msg void OnDgnBEMWDbm();
	afx_msg void OnDgnBEMWSbm();
	afx_msg void OnDgnBEMWChk();
	afx_msg void OnDgnBEMWBotStorChk();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void EnableControl();
	void EnableByCode();
	void Dlg2Data();
	void Data2Dlg();
	// Generated message map functions
	//{{AFX_MSG(CDgnConEBarDesignMethodWallIDDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnConBndrElemWallIDDlg_H__EBB78788_E40F_4AC6_9946_3A9DF18D4D0C__INCLUDED_)
