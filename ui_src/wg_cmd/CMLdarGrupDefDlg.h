#if !defined(AFX_CMLDARGRUPDEFDLG_H__INCLUDED_)
#define AFX_CMLDARGRUPDEFDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLdarGrupDefDlg.h : header file
//

#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "BtnSTNew.h"
#include "DragDropListCtrl.h"
#include "..\MIT_frx\MEdit.h"
#include "..\MIT_frx\MButton.h"
/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCMLdarGrupDefDlg dialog

using namespace mit::frx;
class __MY_EXT_CLASS__ CCMLdarGrupDefDlg : public CCMDlgBase
{
// Construction
public:
	CCMLdarGrupDefDlg(CWnd* pParent = NULL);   // standard constructor
	void UpdateLagrList();
	void GenBysuffixButton();
	void UpdateSelectMark(CStringArray& aSelSZ);
	void UpdateListData();

// Dialog Data
	enum { IDD = IDD_CMD_LDAR_GROUP_DLG };
	MEdit	m_wndGroupName;
	CDragDropListCtrl	m_wndGrupNameList;
	CString	m_strGroupName;
	CString	m_strSuffix;

	mit::frx::MButton	m_btnSortAsc;
	mit::frx::MButton	m_btnSortDsc;
	mit::frx::MButton	m_btnUp;
	mit::frx::MButton	m_btnDown;

	
protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	
	virtual BOOL OnInitDialog();

	afx_msg void OnCmdBtnClose();
	afx_msg void OnCmdAddButton();
	afx_msg void OnCmdDeleteButton();
	afx_msg void OnCmdModifyButton();
	afx_msg void OnCmdASCButton();
	afx_msg void OnCmdDESCButton();
	afx_msg void OnCmdUpButton();
	afx_msg void OnCmdDownButton();
	afx_msg void OnSelchangeCmdGroupNameList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdInvDeleteButton();

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"      /////////
/////////////////////////////////////

#endif
