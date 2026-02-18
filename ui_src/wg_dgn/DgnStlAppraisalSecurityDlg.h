#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

class CDgnStlAppraisalSecurityDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlAppraisalSecurityDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CDgnStlAppraisalSecurityDlg)
	enum	{		IDD = IDD_DGN_STL_DSAS_DLG	};
	int		m_nOption;

	//}}AFX_DATA

private:
	CComboBox	m_LevelCombox;
	int			m_nType;
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlAppraisalSecurityDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void Update_InitDataByTaif();
// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnStlAppraisalSecurityDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

