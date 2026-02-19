#pragma once
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnStlPfmpDlg dialog

class CDBDoc;

class CDgnCfPfmpDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnCfPfmpDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnStlPfmpDlg)
	enum { IDD = IDD_DGN_STL_PFMP_DLG };
	int		m_nOption;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

	void SectPropertyTest();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlPfmpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void EnableItems(BOOL bEnable);
// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnStlPfmpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnPfmpPl();
	afx_msg void OnDgnPfmpNp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
