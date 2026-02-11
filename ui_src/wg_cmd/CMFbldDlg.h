#if !defined(__CMFBLDDLG_H__)
#define __CMFBLDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMFbldDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

#include "FbldGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMFbldDlg dialog
#include "HeaderPre.h"

#define FBLD_NUM 8

class __MY_EXT_CLASS__ CCMFbldDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMFbldDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMFbldDlg)
	enum { IDD = IDD_CMD_FBLD_DLG };
	CString	m_csName;
	CString	m_csDesc;
	//}}AFX_DATA
	CSelectLC	m_wndLC[FBLD_NUM];
	CEditUnit	m_wndLoad[FBLD_NUM];
	CTextUnit	m_wndUnit[FBLD_NUM];
	MButton   m_wndSbw[FBLD_NUM];
	
public:
	void OnChangeCurrentFbld(T_FBLD_K nKey);
	void UpdateBuffer();
	void UnitChanged();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMFbldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
protected:
	BOOL GetFbldData(T_FBLD_D& data);

protected:
	CDBDoc* m_pDoc;
	CFbldGrid m_wndGrid;
	CArray<CWnd*, CWnd*> m_WndArr;

	// Generated message map functions
	//{{AFX_MSG(CCMFbldDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgClose();
	afx_msg void OnCmdFbldAdd();
	afx_msg void OnCmdFbldDelete();
	afx_msg void OnCmdFbldModify();
	afx_msg void OnChangeLoadCase();
	afx_msg void OnCmdLoadcaseBtn();
	afx_msg void OnCmdLtypeButton1();
	afx_msg void OnCmdLtypeButton2();
	afx_msg void OnCmdLtypeButton3();
	afx_msg void OnCmdLtypeButton4();
	afx_msg void OnCmdLtypeButton5();
	afx_msg void OnCmdLtypeButton6();
	afx_msg void OnCmdLtypeButton7();
	afx_msg void OnCmdLtypeButton8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMFBLDDLG_H__)
