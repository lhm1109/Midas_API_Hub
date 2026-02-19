#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\mit_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenAssignReinforceSectDlg dialog

class CDBDoc;

class CDgnGenAssignReinforceSectDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CDgnGenAssignReinforceSectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void OnDgnExecute();

	enum 
	{
		IDD = IDD_DGN_GEN_REINFORCE_ARST_DLG
	};
	int		m_nOption;

public:
	void UpdateRfst();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	mit::frx::MComboBox		m_RfstCombox;

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnGenAssignReinforceSectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnArst();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

