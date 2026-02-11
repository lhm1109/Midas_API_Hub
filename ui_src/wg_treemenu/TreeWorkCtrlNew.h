#pragma once
#include "TreeWorkDefine.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MBaseTreeCtrl.h"

class CTreeWorkCtrlNew : public mit::frx::MBaseTreeCtrl, public CDBUpdateConnector
{
	// Construction
public:
	DECLARE_DYNAMIC(CTreeWorkCtrlNew);
	CTreeWorkCtrlNew();
	virtual ~CTreeWorkCtrlNew();

public:
	void SetBkGroundColor(COLORREF BkColor);

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

protected:
	virtual BOOL GetCustomColor(HTREEITEM hItem);

	int GetSelectedItemList(CArray<HTREEITEM, HTREEITEM>& aTreeItem);
	BOOL GetSelectedItemKeyList(UINT data, CArray<T_NODE_K, T_NODE_K>& rNode, CArray<T_ELEM_K, T_ELEM_K>& rElem);

	COLORREF GetBkColor();
	COLORREF GetBkColorTitle();

	HTREEITEM GetPreviousItemToInsertAfter(UINT insItem);
	HTREEITEM SetSubTreeItem(UINT insItem);

	BOOL SelectEntity(UINT data, BOOL bReplace = TRUE);
	void UnselectEntity(UINT data);
	void ActiveEntity(UINT data);
	void ActiveEntityPlus(UINT data);
	void InactiveEntity(UINT data);
	void DeleteItemEntity(CArray<HTREEITEM, HTREEITEM>& aItem);

	void InitTree();
	void InsertTreeItemByLSTKey(HTREEITEM parent, int nLST, UINT Key, CString str, CString svg);
	void DeleteTreeItemByLSTKey(int nLST, UINT Key);
	HTREEITEM GetTreeItemByLSTKey(int nLST, UINT Key);

	// nCmd == UR_XXXX_XXX, Key = m_vbuf->m_xxxx의 Key
	void SetNode(int nCmd, UINT Key);
	void SetElem(int nCmd, UINT Key);
	void SetSect(int nCmd, UINT Key);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnWorktreeActive();
	afx_msg void OnWorktreeDelete();
	afx_msg void OnWorktreeInactive();
	afx_msg void OnWorktreeSelect();
	afx_msg void OnWorktreeUnselect();
	afx_msg void OnWorktreeUnselectall();
	afx_msg void OnWorktreeActiveall();
	afx_msg void OnWorktreeAssign();
	afx_msg void OnWorktreeProperties();
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd* pWnd);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnWorktreeAddmatl();
	afx_msg void OnWorktreeAddsect();
	afx_msg void OnWorktreeAddthik();
	afx_msg void OnWorktreeSelectplus();
	afx_msg void OnWorktreeDisplay();
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnWorktreeDisappear();
	afx_msg void OnWorktreeDeleteUnusMatl();
	afx_msg void OnWorktreeDeleteUnusSect();
	afx_msg void OnWorktreeDeleteUnusThik();
	afx_msg void OnWorktreeActivePlus();
	afx_msg void OnWorktreeTables();
	afx_msg void OnWorktreeEnable();
	afx_msg void OnWorktreeDisable();
	afx_msg void OnWorktreePSCRebar();
	afx_msg void OnWorktreeExpandAll();
	afx_msg void OnWorktreeExpand1LEVEL();
	afx_msg void OnWorktreeExpand2LEVEL();
	afx_msg void OnWorktreePhgePropTable();
	afx_msg void OnWorktreePhgeAssignTable();
	afx_msg void OnWorktreePhgtDefine();
	afx_msg void OnWorktreeAddmado();
	afx_msg void OnWorktreeAddsbdo();

	DECLARE_MESSAGE_MAP()

protected:
	CDBDoc* m_pDoc;
	BOOL      m_bDrag;     // Drag 여부 판단, TRUE==드래깅 중 
	BOOL      m_bDragPoss; // Drag 가능여부 판단, TRUE==드래깅 가능
	HTREEITEM m_hDragItem; // Drag한 tree Item의 handle 값

	HTREEITEM m_hRootTi;
	std::map<int, HTREEITEM> m_mFld2TreeItem; // Key:WORK_XXXX_FLD, Value:HTREEITEM
	std::map<int, std::map<int, HTREEITEM>> m_mMvcd2FldTreeItem; // Key:D_MOVE_CODE_XXXX, Value:{Key:WORK_XXXX_FLD, Value:HTREEITEM}. Moving 관련 데이터 전용
	std::map<int, std::map<UINT, HTREEITEM>> m_mLst2KeyTreeItem; // Key:WORK_XXXX_LST, Value:{Key:DB Key, Value:HTREEITEM}
	std::map<int, std::map<HTREEITEM, UINT>> m_mLst2TreeItemKey; // Key:WORK_XXXX_LST, Value:{Key:DB Key, Value:HTREEITEM}

	CStringArray m_aMvcdStr;
	CArray<UINT, UINT> m_aMvcdData;

	COLORREF m_BkColor;
	CString m_strBkColorName;
	CString m_strBkTitleColorName;
};