#if !defined(AFX_TREETABLECTRL_H__AEB0BA01_A313_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_TREETABLECTRL_H__AEB0BA01_A313_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeTableCtrl.h : header file
//

#include "..\MIT_frx\MBaseTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTreeTableCtrl window

class CTreeTableCtrl : public mit::frx::MBaseTreeCtrl, public CDBUpdateConnector
{
// Construction
public:
	DECLARE_DYNAMIC(CTreeTableCtrl);
	CTreeTableCtrl();
	virtual ~CTreeTableCtrl();

// Attributes
public:
protected:
	HTREEITEM m_hTIStlDgn;
// Operations
protected:
	COLORREF m_BkColor;
public:
	void SetBkGroundColor(COLORREF BkColor);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeTableCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DestroyTree();
	void LoadMenu();
	void CreateTree();
	// Add by ZINU.('01.1.5).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();
	// Add by ZINU.('01.1.6).
	HTREEITEM GetHTreeItemByID(UINT nID);
	HTREEITEM FindNextItem(TV_ITEM* pItem, HTREEITEM hItem);
	BOOL Compare(TV_ITEM* pItem, TV_ITEM& tvTempItem);
	HTREEITEM GetNextItem(HTREEITEM hItem);
	// Add by ZINU.('04.08.20). For PSC Design.
	void UpdateBuffer(BOOL bAlwaysSet);
	void SetPscDgnTableItemTree();
	void SetStoryTable(); //!@#$ MNET:1489 20050823 LBI

	//Add by maxiao(2007-12-07) For civil CHINA RC design
	void SetRcDgnTableItemTree();

	// Add by sshan.('06.06.02). For Bridge Load Rating Design
	void SetBridgeLoadRatingDesignTableItemTree(); 
protected:
	virtual COLORREF GetBkColor();
	CString GetPSCResultTitle(int iDgnCode, int iPSCResType);

private:
    void SetPscDgnTableItemTreeKSCE(T_PSCD_D PscdD, HTREEITEM hItem);
    void SetPscDgnTableItemTreeKSCELSD(T_PSCD_D PscdD, HTREEITEM hItem);
    void SetPscDgnTableItemTreeJSCE(T_PSCD_D PscdD, HTREEITEM hItem);
    void SetPscDgnTableItemTreeJTG(T_PSCD_D PscdD, HTREEITEM hItem);
    void SetPscDgnTableItemTreeTB(T_PSCD_D PscdD, HTREEITEM hItem);
    void SetPscDgnTableItemTreeUS(T_PSCD_D PscdD, HTREEITEM hItem);
    void SetPscDgnTableItemTreeEURO(T_PSCD_D PscdD, HTREEITEM hItem);
		void SetPscDgnTableItemTreeABNT(T_PSCD_D PscdD, HTREEITEM hItem);
    void SetPscDgnTableItemTreeIND(T_PSCD_D PscdD, HTREEITEM hItem);
    void SetPscDgnTableItemTreeAS(T_PSCD_D PscdD, HTREEITEM hItem);
    void SetPscDgnTableItemTreeBS(T_PSCD_D PscdD, HTREEITEM hItem);
    void SetPscDgnTableItemTreeRUS(T_PSCD_D PscdD, HTREEITEM hItem);

	// Generated message map functions
protected:
	void ToggleStyle(DWORD lStyleMask );
	//{{AFX_MSG(CTreeTableCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	CString m_strBkColorName;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREETABLECTRL_H__AEB0BA01_A313_11D3_92DE_0000C0B0E6B3__INCLUDED_)
