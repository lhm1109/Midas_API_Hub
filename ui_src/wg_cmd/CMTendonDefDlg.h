#if !defined(__CMTENDONDEFDLG_H__)
#define __CMTENDONDEFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTendonDefDlg.h : header file
//


#include "CMDlgBase.h"
#include "BtnSTNew.h"
#include "DragDropListCtrl.h"

//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

#include "HeaderPre.h"      /////////

/////////////////////////////////////////////////////////////////////////////
// CCMTendonDefDlg dialog

class __MY_EXT_CLASS__ CCMTendonDefDlg : public CCMDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CCMTendonDefDlg(CWnd* pParent = NULL);   // standard constructor
	void UpdateTendonList();
	void GenBysuffixButton();
	void UpdateSelectMark(CStringArray& aSelSZ);
	void UpdateListData();
// Dialog Data
	//{{AFX_DATA(CCMTendonDefDlg)
	enum { IDD = IDD_CMD_TENDON_GROUP_DLG };
	mit::frx::MEdit	m_wndGroupName;
	CDragDropListCtrl		m_wndGrupNameList;
	CString	m_strGroupName;
	CString	m_strSuffix;

	mit::frx::MButton	m_btnSortAsc;
	mit::frx::MButton	m_btnSortDsc;
	mit::frx::MButton	m_btnUp;
	mit::frx::MButton	m_btnDown;
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTendonDefDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMTendonDefDlg)
	afx_msg void OnCmdBtnClose();
	afx_msg void OnCmdAddButton();
	afx_msg void OnCmdDeleteButton();
	afx_msg void OnCmdModifyButton();
	afx_msg void OnCmdASCButton();
	afx_msg void OnCmdDESCButton();
	afx_msg void OnCmdUpButton();
	afx_msg void OnCmdDownButton();  
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdGroupNameList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdInvDeleteButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"      /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMTENDONDEFDLG_H__)






