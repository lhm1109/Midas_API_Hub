#if !defined(__TREEJUDCTRL_H__)
#define __TREEJUDCTRL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "..\MIT_frx\MBaseTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTreeJUDCtrl window
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CTreeJUDCtrl : public mit::frx::MBaseTreeCtrl, public CDBUpdateConnector
{
// Construction
public:
	DECLARE_DYNAMIC(CTreeJUDCtrl);
	CTreeJUDCtrl();
	virtual ~CTreeJUDCtrl();
// Attributes
public:
protected:
	CImageList m_ImageList;
// Operations
protected:
	COLORREF m_BkColor;
public:
	void SetBkGroundColor(COLORREF BkColor);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeJUDCtrl)
	//}}AFX_VIRTUAL

// Implementation
private:
	void LoadMenu_FoundationModule(mit::frx::MBaseTreeCtrl* ctlr,HTREEITEM hTreeParent, HTREEITEM hInsertAfter = TVI_LAST);
public:
	void DestroyTree();
	void LoadMenu();
	void CreateTree();
	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	void UpdateBuffer(BOOL bAlwaysSet);

protected:
	virtual BOOL GetCustomColor(HTREEITEM hItem);

	COLORREF GetBkColor() const;
	COLORREF GetBkColorTitle() const;

	BOOL IsExistHorLoad();    // TRUE:입력됨, FALSE:미입력
	BOOL IsExistCalcInfo();   // TRUE:입력됨, FALSE:미입력 
	BOOL IsExistPlanAssign(); // TRUE:입력됨, FALSE:미입력
	
	// Generated message map functions
protected:
	void ToggleStyle(DWORD lStyleMask );
	//{{AFX_MSG(CTreeJUDCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEJUDCTRL_H__88AF9822_4F9B_11D2_A486_00A0249CC658__INCLUDED_)
