#if !defined(AFX_CMMLTTDLG_H__49F116E2_2582_11D4_92DE_0000C0B0E6B3__INCLUDED_) 
#define AFX_CMMLTTDLG_H__49F116E2_2582_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMlttDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_selectctrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMlttDlg form view

#include "CMChildBarBase.h"

#define D_Mltt_TRUS 0
#define D_Mltt_BEAM 1
#define D_Mltt_PLAT 2
#define D_Mltt_ELNK 3
#define D_Mltt_NLNK 4

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCMMlttDlg : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CCMMlttDlg();           // protected constructor used by dynamic creation
//	DECLARE_DYNCREATE(CCMMlttDlg)

//----------------------------------------------------------------
// Overridables
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	afx_msg void OnTmSetfocusLinkEdt();
//----------------------------------------------------------------
// Implementations
protected:
	void AlignControl();
	void RemoveNodeSelect();
	void SetListCtrlHeader();
	void MakeItemEx();
	CString DataToStr(int i, int nNo, T_MLTT_K key);
	void SetItem(int nIndex, T_MLTT_K Key);
	void InsertItem(T_MLTT_K Key);
	void DeleteItem(T_MLTT_K Key);
	void OnDelKeyPressedInList();
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	void CtrlMan();
	virtual void Execute() { OnCmdApply(); };

public:
	CDBDoc* m_pDoc;

protected:  
	CArray<UINT, UINT> m_aCtrlOp;

// Form Data
public:
	//{{AFX_DATA(CCMMlttDlg)
	enum { IDD = IDD_DBAR_MLTT_DLG };
	CListCtrl	  m_List;
	CSelectEdit   m_edtLink;

	int		m_nEType;
	int		m_nRFType;
	int     m_COLCOUNT;

	int m_nMVCode;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMlttDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCMMlttDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCMMlttDlg)
	afx_msg void OnCmdApply();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdOptionAddDel();
	afx_msg void OnCmdEtype();
	afx_msg void OnClicklist(NMHDR* pNMHDR, LRESULT* pResult);
 	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMLTTDLG_H__49F116E2_2582_11D4_92DE_0000C0B0E6B3__INCLUDED_)
