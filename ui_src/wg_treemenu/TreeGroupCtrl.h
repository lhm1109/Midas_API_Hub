#if !defined(AFX_TREEGROUPCTRL_H__A86285A2_973F_40D2_B7AC_8DDC6DB860BF__INCLUDED_)
#define AFX_TREEGROUPCTRL_H__A86285A2_973F_40D2_B7AC_8DDC6DB860BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeGroupCtrl.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MBaseTreeCtrl.h"
#include <set>
#include <map>

class CViewBase;

#define  STRT_GROUP_FLD   0
#define  STRT_GROUP_LST   1
#define  BNDR_GROUP_FLD   2
#define  BNDR_GROUP_LST   3
#define  LOAD_GROUP_FLD   4
#define  LOAD_GROUP_LST   5
#define  TEND_GROUP_FLD   6   //tnsel
#define  TEND_GROUP_LST   7
#define  LDAR_GROUP_FLD   8   // Loading Area Group added by yskim 140228
#define  LDAR_GROUP_LST   9

/////////////////////////////////////////////////////////////////////////////
// CTreeGroupCtrl window

class CTreeGroupCtrl : public mit::frx::MBaseTreeCtrl, public CDBUpdateConnector
{
	enum FieldOrList               // Declare enum type Days
	{
		
	} FLDLST;     

// Construction
public:
	DECLARE_DYNAMIC(CTreeGroupCtrl);
	CTreeGroupCtrl();

protected:
	BOOL      m_bDrag;     // Drag 여부 판단, TRUE==드래깅 중 
	BOOL      m_bDragPoss; // Drag 가능여부 판단, TRUE==드래깅 가능
	HTREEITEM m_hDragItem; // Drag한 tree Item의 handle 값
	
	std::map<unsigned int, HTREEITEM> m_mFld2hItem;
	std::map<unsigned int, std::map<T_KEY, HTREEITEM>> m_mKey2hItem;
	std::map<unsigned int, std::map<HTREEITEM, T_KEY>> m_mhItem2Key;

	HTREEITEM                     m_hRootTi;

	HTREEITEM m_hEditItem;
	CString m_csEditLabel;
public:

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
public:
	// Operations
	void SaveEditItem(HTREEITEM hItem);
	BOOL IsLabelChanged(LPCTSTR lpStr) { return (BOOL)m_csEditLabel.Compare(lpStr); }
	
protected:
	BOOL IsBeam(int key);
	BOOL IsColumn(int key);
	void CreateTree();
	void SetItemTree();
	HTREEITEM InitSubTreeItem(UINT insItem, int nCount, UINT imageID);
	void InitGroupItemTree();
	void InitBoundaryGroupItemTree();
	void InitLoadGroupItemTree();
	void InitTendGroupItemTree(); //tnsel
	void InitLdarGroupItemTree(); //tnsel


	void DestroyTree();
	void ToggleStyle(DWORD lStyleMask );
	BOOL SelectEntity(UINT data,BOOL bReplace);
	void UnselectEntity(UINT data);
	void ActiveEntity(UINT data);
	void InactiveEntity(UINT data);
	BOOL GetSelectedItemKeyList(UINT data,
															CArray<unsigned int,unsigned int> &rNode,
															CArray<unsigned int,unsigned int> &rElem);
	BOOL GetSelectedItemKeyList(UINT data,
															CArray<unsigned int,unsigned int> &rTdna); // tnsel for Tendon, Loading Area Group

	int GetSelectedGrupKeyListWithMultiTreeItemSelect(CArray<T_GRUP_K, T_GRUP_K> &aKey);
	int GetSelectedBngrKeyListWithMultiTreeItemSelect(CArray<T_BNGR_K, T_BNGR_K> &aKey);
	int GetSelectedLdgrKeyListWithMultiTreeItemSelect(CArray<T_LDGR_K, T_LDGR_K> &aKey);
	int GetSelectedTdgrKeyListWithMultiTreeItemSelect(CArray<T_TDGR_K, T_TDGR_K> &aKey); //tnsel
	int GetSelectedLagrKeyListWithMultiTreeItemSelect(CArray<T_LAGR_K, T_LAGR_K> &aKey); //Loading Area Group
	/*
	void AssignAttribute(HTREEITEM ht,
											 CArray<T_NODE_K, T_NODE_K> &aNodeKeyList,
											 CArray<T_ELEM_K, T_ELEM_K> &aElemKeyList,BOOL bPlus);
											 */
	//void AssignAttribute(CViewBase * pView , BOOL bPlus, BOOL bDrag);

	// nOption : (0) Assign(replace) (1) Assign Plus (2) Assign Minus
	void AssignAttribute(CViewBase * pView , int nOption, BOOL bDrag);

	void EndDrag(CPoint point);
	//void DeleteItemEntity(CArray<HTREEITEM, HTREEITEM> &aItem);
	void DeleteItemEntity(UINT data);

	void EndEditLabelStrtGroup(NMHDR* pNMHDR, LRESULT* pResult);
	void EndEditLabelBndrGroup(NMHDR* pNMHDR, LRESULT* pResult); 
	void EndEditLabelLoadGroup(NMHDR* pNMHDR, LRESULT* pResult); 
	void EndEditLabelTendGroup(NMHDR* pNMHDR, LRESULT* pResult); //tnsel
	void EndEditLabelLdarGroup(NMHDR* pNMHDR, LRESULT* pResult); // Loading Area

	int  GetSelectedItems(UINT nWhatKind, CArray<HTREEITEM,HTREEITEM>& arItems);

	BOOL DisplayEntity(UINT data, BOOL bReplace=TRUE);
	void UndisplayEntity(UINT data);
	
	void RenewTreeItem_GRUP(std::set<T_GRUP_K> stKey);
	void RenewTreeItem_BNGR(std::set<T_BNGR_K> stKey);
	void RenewTreeItem_LDGR(std::set<T_LDGR_K> stKey);
	void RenewTreeItem_TDGR(std::set<T_TDGR_K> stKey);
	void RenewTreeItem_LAGR(std::set<T_LAGR_K> stKey);

	void ArrangeOrderTreeItem_GRUP();
	void ArrangeOrderTreeItem_BNGR();
	void ArrangeOrderTreeItem_LDGR();
	void ArrangeOrderTreeItem_TDGR();
	void ArrangeOrderTreeItem_LAGR();

	void DeleteTreeItem_GRUP(std::set<T_GRUP_K> stKey);
	void DeleteTreeItem_BNGR(std::set<T_BNGR_K> stKey);
	void DeleteTreeItem_LDGR(std::set<T_LDGR_K> stKey);
	void DeleteTreeItem_TDGR(std::set<T_TDGR_K> stKey);
	void DeleteTreeItem_LAGR(std::set<T_LAGR_K> stKey);

	void SetTreeImage();
	void SelectEditText();

protected:
	virtual BOOL GetCustomColor(HTREEITEM hItem);
	COLORREF GetBkColorTitle();
	virtual COLORREF GetBkColor();
	
	BOOL IsJUDGroup(UINT data); // for iGen : 2차설계 결과를 위한 Structure Group인가 ?
	BOOL IsJUDGroup(HTREEITEM hItem);

	virtual BOOL IsNeedColorPalette(HTREEITEM hTreeItem);
	virtual COLORREF GetColorPalette(HTREEITEM hTreeItem);
	virtual void OnChangeColorPalette(HTREEITEM hTreeItem, COLORREF clr);

protected:
	CDBDoc* m_pDoc;
	COLORREF m_BkColor;
	CString m_strBkColorName;
	CString m_strBkTitleColorName;
public:
	void SetBkGroundColor(COLORREF BkColor);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeGroupCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeGroupCtrl();
	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeGroupCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult); //
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);  //
	afx_msg void OnStrtGroupNew();
	afx_msg void OnBndrGroupNew();
	afx_msg void OnLoadGroupNew();
	afx_msg void OnTendGroupNew(); //tnsel
	afx_msg void OnLoadGroupRename();
	afx_msg void OnStrtGroupRename();
	afx_msg void OnBndrGroupRename();
	afx_msg void OnTendGroupRename(); //tnsel
	afx_msg void OnBndrGroupNewMulti();
	afx_msg void OnLoadGroupNewMulti();
	afx_msg void OnTendGroupNewMulti(); //tnsel
	afx_msg void OnStrtGroupAssign();
	afx_msg void OnStrtGroupAssignPlus();
	afx_msg void OnStrtGroupAssignMinus();
	afx_msg void OnStrtGroupActive();
	afx_msg void OnTendGroupActive(); //tnsel
	afx_msg void OnStrtGroupDelete();
	afx_msg void OnTendGroupDelete(); //tnsel
	afx_msg void OnStrtGroupInactive();
	afx_msg void OnTendGroupInactive(); //tnsel
	afx_msg void OnStrtGroupSelect();
	afx_msg void OnStrtGroupUnselect();
	afx_msg void OnStrtGroupNewMulti();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnStrtGroupDisplay();
	afx_msg void OnStrtGroupUndisplay();

	//Loading Area Group
	afx_msg void OnLdarGroupNew();
	afx_msg void OnLdarGroupNewMulti();
	afx_msg void OnLdarGroupSelect();
	afx_msg void OnLdarGroupUnselect();
	afx_msg void OnLdarGroupActive();
	afx_msg void OnLdarGroupInactive();
	afx_msg void OnLdarGroupRename();
	afx_msg void OnLdarGroupDisplay();
	afx_msg void OnLdarGroupUndisplay();
	//}}AFX_MSG
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEGROUPCTRL_H__A86285A2_973F_40D2_B7AC_8DDC6DB860BF__INCLUDED_)



