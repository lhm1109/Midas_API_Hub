#if !defined(AFX_STTLGROUPDLG_H__93312C02_6EBB_11D4_9AAF_0000C0B9C58C__INCLUDED_)
#define AFX_STTLGROUPDLG_H__93312C02_6EBB_11D4_9AAF_0000C0B9C58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SttlGroupDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CSttlGroupDlg dialog

class CSttlGroupDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CSttlGroupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {};
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CSttlGroupDlg)
	enum { IDD = IDD_TM_STTL_GROUP };
	CTextUnit	m_SetlDispUnit;
	MEdit	m_edtGroupName;
	CMouseEdit	m_edtSetlDisp;
	CSelectEdit	m_edtNodeList;
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
public:
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSttlGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	
	T_SMPT_K m_SmptKey;	
	T_SMPT_D m_SmptData;
	T_SMPT_DT m_SmptNodeData;
	T_SMPT_UNIT m_SmptUnit;

	void InitListTitle();
	void InitSmptData();
	void UpdateUnit(); 
	void ModifyTitle();
	int GetCurrentRow();
	void UpdateBuffer();
	int GetCurrentSmptKey();
	CString GetCurrentSmptKeyName();

	// Generated message map functions
	//{{AFX_MSG(CSttlGroupDlg)
	afx_msg void OnTmSttlGroupClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnTmSttlGroupAdd();
	afx_msg void OnTmSttlGroupModify();
	afx_msg void OnTmSttlGroupDelete();
	afx_msg void OnItemchangedTmSttlGroupList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeTmSttlGroupLoadlistEdit();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STTLGROUPDLG_H__93312C02_6EBB_11D4_9AAF_0000C0B9C58C__INCLUDED_)
