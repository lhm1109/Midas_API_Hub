#pragma once

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_MouseEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenDampSpecifySpecialTypeDlg dialog

class CDBDoc;

class CDgnGenDampSpecifySpecialTypeDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
	// Construction
public:
	CDgnGenDampSpecifySpecialTypeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

	// Dialog Data
		//{{AFX_DATA(CDgnGenDampSpecifySpecialTypeDlg)
	enum {
		IDD = IDD_DGN_GEN_DAMP_DSST_DLG
	};
	int		m_nOption;
	int		m_nOptType;
	//}}AFX_DATA

public:
	void Update_InitDataByCode();
	void EnableCtrl(BOOL bEnable);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDgnGenDampSpecifySpecialTypeDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnGenDampSpecifySpecialTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

