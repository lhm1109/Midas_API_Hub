#if !defined(AFX_CMSTRTGRUPDEFDLG_H__D7C8A41F_E68C_4985_8BDC_66FC4B1DD77A__INCLUDED_)
#define AFX_CMSTRTGRUPDEFDLG_H__D7C8A41F_E68C_4985_8BDC_66FC4B1DD77A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStrtGrupDefDlg.h : header file
//
#include "CMDlgBase.h"
#include "BtnSTNew.h"
#include "DragDropListCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStrtGrupDefDlg dialog
/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
class __MY_EXT_CLASS__ CCMStrtGrupDefDlg : public CCMDlgBase
{
// Construction
public:
	CCMStrtGrupDefDlg(CWnd* pParent = NULL);   // standard constructor
	void UpdateStgrList();
	void GenBysuffixButton(); 
	void UpdateSelectMark(CStringArray& aSelSZ);
	void UpdateListData();
// Dialog Data
	//{{AFX_DATA(CCMStrtGrupDefDlg)
	enum { IDD = IDD_CMD_STRT_GROUP_DLG };
	mit::frx::MEdit	m_wndGroupName;
	CDragDropListCtrl	m_wndGrupNameList;
	CString	m_strGroupName;
	CString	m_strSuffix;
	CBitmap m_bitmap;  

	mit::frx::MButton	m_btnSortAsc;
	mit::frx::MButton	m_btnSortDsc;
	mit::frx::MButton	m_btnUp;
	mit::frx::MButton	m_btnDown;
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStrtGrupDefDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStrtGrupDefDlg)
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
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTRTGRUPDEFDLG_H__D7C8A41F_E68C_4985_8BDC_66FC4B1DD77A__INCLUDED_)
