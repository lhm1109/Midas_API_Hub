#if !defined(__CMTENDONCHANGEPROPDLG_H__)
#define __CMTENDONCHANGEPROPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTendonChangePropDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMTendonChangePropDlg dialog
class CDBDoc;
class CCMTendonChangePropDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMTendonChangePropDlg(CWnd* pParent = NULL);   // standard constructor

// interface
	void SetSelectedProfiles(CArray<UINT, UINT> &aSelectedK);

// Dialog Data
	//{{AFX_DATA(CCMTendonChangePropDlg)
	enum { IDD = IDD_CMD_DEF_TENDON_CHANGE_PROP_DLG };
	CComboBox	m_cobxProperty;
	CListBox	m_listSelProfiles;
	CListBox	m_listAllProfiles;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTendonChangePropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	void InitAllList();
	void InitSelList();
	void InitPropCombo();
	void DoSelectItem();
	void DoUnselectItem();
	BOOL DoChangeProperty();

protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aSelectedProfiles;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMTendonChangePropDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdSelButton();
	afx_msg void OnCmdUnselButton();
	afx_msg void OnCmdPropertyButton();
	afx_msg void OnDblclkCmdAllList();
	afx_msg void OnDblclkCmdSelectedList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMTENDONCHANGEPROPDLG_H__)
