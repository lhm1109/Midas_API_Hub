#if !defined(AFX_CMBNDRGRUPDEFDLG_H__B3FF4377_0F91_45CE_A841_D2241069AE1E__INCLUDED_)
#define AFX_CMBNDRGRUPDEFDLG_H__B3FF4377_0F91_45CE_A841_D2241069AE1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMBndrGrupDefDlg.h : header file
//

#include "CMDlgBase.h"
//#include "..\wg_db\wg_db_DlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "BtnSTNew.h"
#include "DragDropListCtrl.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"

//#include "CMLoadCaseGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseDlg dialog

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCMBndrGrupDefDlg dialog
 
class __MY_EXT_CLASS__ CCMBndrGrupDefDlg : public CCMDlgBase
{
// Construction
public:
	CCMBndrGrupDefDlg(CWnd* pParent = NULL);   // standard constructor
	void UpdateBngrList();
	void GenBysuffixButton();
	void UpdateSelectMark(CStringArray& aSelSZ);
	void UpdateListData();
// Dialog Data
	//{{AFX_DATA(CCMBndrGrupDefDlg)
	enum { IDD = IDD_CMD_BNDR_GROUP_DLG };
	mit::frx::MEdit	m_wndGroupName;
	CDragDropListCtrl	m_wndGrupNameList;
	CString	m_strGroupName;
	CString	m_strSuffix;

	mit::frx::MButton	m_btnSortAsc;
	mit::frx::MButton	m_btnSortDsc;
	mit::frx::MButton	m_btnUp;
	mit::frx::MButton	m_btnDown;

	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMBndrGrupDefDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMBndrGrupDefDlg)
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

#endif // !defined(AFX_CMBNDRGRUPDEFDLG_H__B3FF4377_0F91_45CE_A841_D2241069AE1E__INCLUDED_)
