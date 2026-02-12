#if !defined(AFX_LDGRCHANGEDLG_H__C82223E8_BEC1_461B_93AC_84A7E7EB458C__INCLUDED_)
#define AFX_LDGRCHANGEDLG_H__C82223E8_BEC1_461B_93AC_84A7E7EB458C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LdgrChangeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLdgrChangeDlg dialog
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\MIT_frx\MCheckListBox.h"

class CLdgrChangeDlg : public CMenuBarChildDlg
{
// Construction
public:
	CLdgrChangeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnTmExecute();  };
	void         SetLoadTypeList();
	unsigned int GetSourceGroupKey();
	unsigned int GetTargetGroupKey();

// Dialog Data
	//{{AFX_DATA(CLdgrChangeDlg)
	enum { IDD = IDD_TM_LOAD_GROUP_CHANGE };
	mit::frx::MCheckListBox	m_wndLoadTypeList;
	CCobxLdgr	m_wndTarGroupComb;
	CCobxLdgr	m_wndSrcGroupComb;
	int		m_nOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLdgrChangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DataExchange(CArray<int, int>& aItemChecked, int nOperation);

	// Generated message map functions
	//{{AFX_MSG(CLdgrChangeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefineGroupButton();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDGRCHANGEDLG_H__C82223E8_BEC1_461B_93AC_84A7E7EB458C__INCLUDED_)
