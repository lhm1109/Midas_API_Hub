#pragma once
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDgnStlPfdlDlg dialog

class CDBDoc;

class CDgnStlPfdlDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlPfdlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnStlPfdlDlg)
	enum { IDD = IDD_DGN_STL_PFDL_DLG };
	int		m_nOption;
	int		m_pfdl;
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
	//{{AFX_VIRTUAL(CDgnStlPfdlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void EnableItems(BOOL bEnable);
private:
	CComboBox   m_lstPfdl;
// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnStlPfdlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnPhibAr();
	afx_msg void OnDgnPhibDel();
	afx_msg void OnSelPfdl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
