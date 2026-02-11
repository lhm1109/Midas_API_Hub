#if !defined(AFX_CMTHRDDLG_H__10BB2142_19DD_4D0C_8FD4_1FDE8F57606B__INCLUDED_)
#define AFX_CMTHRDDLG_H__10BB2142_19DD_4D0C_8FD4_1FDE8F57606B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "CMChildBarBase.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMThrdDlg dialog

class CCMThrdDlg : public CCMChildBarBase, public CDBUpdateConnector
{
// Construction
public:
	CCMThrdDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void Execute(){}
	virtual BOOL ExternalInit(UINT key);
	void DlgInit(int nFuncType, int nComboIndex);
// Dialog Data
	//{{AFX_DATA(CCMThrdDlg)
	enum { IDD = IDD_DBAR_THRD_DLG };
	CListCtrl	m_ctrlList;
	MComboBox	m_ctrlFunction;
	int		m_nFunctionType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void MakeListHeader();
	void MakeItemEx();
	void SetItem(int nIndex, T_THRD_K ThrdK, T_THRD_D& ThrdD);
	CString DataToStr(int nIndex, T_THRD_D& ThrdD);
	CString GetGraphTitle(int nType);
	CString GetStepTitle(int nType);
	BOOL GetSelectedIndex(int &nIndex);
	void MakeMenuCombo(int nType);

	// Generated message map functions
	//{{AFX_MSG(CCMThrdDlg)
	afx_msg void OnCmdChangeCombo();
	afx_msg void OnCmdThrdSetfunc();
	afx_msg void OnBarClose();
	afx_msg void OnModify();
	afx_msg void OnBack();
	afx_msg void OnDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkCmdThrdFuncList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHRDDLG_H__10BB2142_19DD_4D0C_8FD4_1FDE8F57606B__INCLUDED_)
