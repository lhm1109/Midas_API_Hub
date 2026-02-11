#if !defined(AFX_CMSINFDLG_H__AAFF4D61_3795_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSINFDLG_H__AAFF4D61_3795_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSinfDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxThfc.h"
#include "..\wg_db\wg_db_SelectLC.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSinfDlg dialog

#include "CMChildBarBase.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCMSinfDlg : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CCMSinfDlg();           // protected constructor used by dynamic creation
//	DECLARE_DYNCREATE(CCMSinfDlg)

//----------------------------------------------------------------
// Overridables
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------
// Implementations
protected:
	void SetListCtrlHeader();
	void MakeItemEx();
	CString DataToStr(int i, int nNo, T_SINF_K key);
	void SetItem(int nIndex, T_SINF_K Key);
	void InsertItem(T_SINF_K Key);
	void DeleteItem(T_SINF_K Key);
	void OnDelKeyPressedInList();
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	virtual void Execute() { OnCmdApply(); };

public:
	CDBDoc* m_pDoc;

protected:
	CArray<UINT, UINT> m_aCtrlOp;

// Form Data
public:
	//{{AFX_DATA(CCMSinfDlg)
	enum { IDD = IDD_DBAR_SINF_DLG };
	CListCtrl	m_List;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSinfDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCMSinfDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCMSinfDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdClose();
	afx_msg void OnClicklist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSINFDLG_H__AAFF4D61_3795_11D4_92DE_0000C0B0E6B3__INCLUDED_)
