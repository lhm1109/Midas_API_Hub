#if !defined(__TREEMENUCTRL_H__)
#define __TREEMENUCTRL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BarTreeMenuCtrl.h : header file
//
#include "..\MIT_frx\MBaseTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTreeMenuCtrl window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CTreeMenuCtrl : public mit::frx::MBaseTreeCtrl, public CDBUpdateConnector
{
// Construction
public:
	DECLARE_DYNAMIC(CTreeMenuCtrl);
	CTreeMenuCtrl();
	virtual ~CTreeMenuCtrl();
// Attributes
public:
protected:
	CImageList m_ImageList;
	CString m_strSvgPath;
// Operations
protected:
	COLORREF m_BkColor;
public:
	void SetBkGroundColor(COLORREF BkColor);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeMenuCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DestroyTree();
	void LoadMenu();
	void CreateTree();

	void ResetDgnGeneralItemTree();
	void ResetDgnConcreteItemTree();

	// Add by ZINU.('01.1.5).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();
	void AdjustTreeMenu(int nCode);
	HTREEITEM GetHTreeItemByID(UINT nID);
	HTREEITEM FindNextItem(TV_ITEM* pItem, HTREEITEM hItem);
	BOOL Compare(TV_ITEM* pItem, TV_ITEM& tvTempItem);
	HTREEITEM GetNextItem(HTREEITEM hItem);

	void UpdateBuffer(BOOL bAlwaysSet);
	void SetPscDgnTableItemTree();
	void SetRatingDgnTableItemTree();
	void SetTowerItemTree();

	// Generated message map functions
protected:
	virtual BOOL GetCustomColor(HTREEITEM hItem);
	virtual COLORREF GetBkColor();

	void ToggleStyle(DWORD lStyleMask );
	//{{AFX_MSG(CTreeMenuCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARTREEMENUCTRL_H__88AF9822_4F9B_11D2_A486_00A0249CC658__INCLUDED_)
