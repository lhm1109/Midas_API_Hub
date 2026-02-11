#if !defined(AFX_TREEGROUPDLG_H__C1C53E42_E2BA_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_TREEGROUPDLG_H__C1C53E42_E2BA_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeGroupDlg.h : header file
//

#include "..\wg_base\wg_base_ColorTreeCtrl.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "BaseTreeCtrl.h"

class CGroupTreeCtrl : public CBaseTreeCtrl
{
// Construction
public:
	CGroupTreeCtrl() {}
	virtual ~CGroupTreeCtrl() {}
		
// Overridables
public:
	// 원래처럼 동작하려면 FALSE리턴하면 된다.
	virtual BOOL ProcessReturnKey(CEdit* pEdit);
	virtual BOOL ProcessEscapeKey(CEdit* pEdit);

// Operations
	void SaveEditItem(HTREEITEM hItem);
	BOOL IsLabelChanged(LPCTSTR lpStr) { return (BOOL)m_csEditLabel.Compare(lpStr); }
	HTREEITEM  GetNextItem(HTREEITEM hItem, UINT nCode);
	HTREEITEM  GetNextItem(HTREEITEM hItem);
	HTREEITEM  FindNextItem(TV_ITEM* pItem, HTREEITEM hItem);
	BOOL       Compare(TV_ITEM* pItem, TV_ITEM& tvTempItem);

	int        GetSelectedItemList(CArray<HTREEITEM, HTREEITEM> &aItem);

// Implement
protected:
	HTREEITEM m_hEditItem;
	CString   m_csEditLabel;
};

/////////////////////////////////////////////////////////////////////////////
// CTreeGroupDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTreeGroupDlg : public CFormViewInBar, public CDBUpdateConnector
{
public:
	CTreeGroupDlg();           // protected constructor used by dynamic creation
	virtual ~CTreeGroupDlg();
	virtual void Execute() {}
	DECLARE_DYNCREATE(CTreeGroupDlg)

// Form Data
public:
	//{{AFX_DATA(CTreeGroupDlg)
	enum { IDD = IDD_TM_GROUP };
	CTrackLookButton	m_btnAsnPlus;
	CTrackLookButton	m_btnInactive;
	CTrackLookButton	m_btnActive;
	CTrackLookButton	m_btnDelete;
	CTrackLookButton	m_btnUnselect;
	CTrackLookButton	m_btnSelect;
	CTrackLookButton	m_btnAssign;
	CTrackLookButton	m_btnRename;
	CTrackLookButton	m_btnNew;
	//}}AFX_DATA
	CGroupTreeCtrl m_wndTree;
	//CDispTreeCtrl	m_wndTree;
	//CBaseTreeCtrl m_wndTree;

// Attributes
public:
	CLabelEdit    m_wndLabelEdit;
	CToolTipCtrlEx  m_ToolTipCtrl;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeGroupDlg)
	public:
	virtual void OnInitialUpdate();
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Implementation
protected:
	BOOL m_bInitialized;
	HTREEITEM m_hEdit;

	COleDropTarget m_OleDropTarget;
	HTREEITEM m_hItemDragged;

protected:
	void ResizeTreeCtrl(WINDOWPOS FAR* lpwndpos);
	BOOL SetItemTree();
	void InsertGroupItem(T_GRUP_K Key, T_GRUP_D& Data);
	void DeleteGroupItem(T_GRUP_K Key, T_GRUP_D& Data);
	BOOL FindGroupItem(T_GRUP_K Key, HTREEITEM* phItem);

	int  GetSelectedGroupKeyList(CArray<T_GRUP_K, T_GRUP_K> &aKey);
	void GetSelAndUnselGroupKeyList(CArray<T_GRUP_K, T_GRUP_K> &aSelKey,
																	CArray<T_GRUP_K, T_GRUP_K> &aUnselKey);
	void SelectItemListByKey(CArray<T_GRUP_K, T_GRUP_K> &aSelKey);


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTreeGroupDlg)
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnEndlabeleditTmGroupTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeleditTmGroupTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmGroupRename();
	afx_msg void OnTmGroupNew();
	afx_msg void OnTmGroupAssign();
	afx_msg void OnTmGroupDelete();
	afx_msg void OnTmGroupSelect();
	afx_msg void OnDblclkTmGroupTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmGroupUnselect();
	afx_msg void OnBegindragTmGroupTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmGroupActive();
	afx_msg void OnTmGroupInactive();
	afx_msg void OnTmGroupAsnplus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEGROUPDLG_H__C1C53E42_E2BA_11D3_92DE_0000C0B0E6B3__INCLUDED_)
