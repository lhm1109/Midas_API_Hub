#if !defined(__TREESEISCTRL_H__)
#define __TREESEISCTRL_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "..\MIT_frx\MBaseTreeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTreeSeisCtrl window
#include "HeaderPre.h"

#define ID_1ST_RESULT_TEMP    5000001
#define ID_1ST_RESULT_TB_TEMP 5000002
#define ID_2ND_RESULT_TEMP    5000003
#define ID_2ND_RESULT_TB_TEMP 5000004
#define ID_PO_RESULT_TEMP     5000005
#define ID_PO_RESULT_TB_TEMP  5000006
#define ID_PHGT_LST           5000007
#define ID_PHGE_LST           5000008
#define ID_POLC_LST           5000009
#define ID_IEHP_LST           5000010
#define ID_IEHG_LST           5000011
#define ID_FIMP_LST			  5000012
#define ID_FIBR_LST			  5000013
#define ID_FIBW_LST			  5000014
#define ID_FIMP_PO_LST		  5000015
#define ID_FIBR_PO_LST		  5000016
#define ID_FIBW_PO_LST		  5000017


#define MAX_TREE_COUNT 9
class __MY_EXT_CLASS__ CTreeSeisCtrl : public mit::frx::MBaseTreeCtrl, public CDBUpdateConnector
{
	// Construction
public:
	DECLARE_DYNAMIC(CTreeSeisCtrl);
	CTreeSeisCtrl();
	virtual ~CTreeSeisCtrl();
	// Attributes
public:
protected:
	CImageList m_ImageList;
	int m_IMAGE1_SIZE;
	int m_IMAGE2_SIZE;

	// Operations
protected:
	COLORREF m_BkColor;
	CString m_strBkColorName;
	CString m_strBkTitleColorName;

public:
	void SetBkGroundColor(COLORREF BkColor);
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeSeisCtrl)
	//}}AFX_VIRTUAL

	// Implementation
private:
public:
	void DestroyTree();
	void LoadMenu();
	void CreateTree();

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

	void UpdateBuffer(BOOL bAlwaysSet);
	void UpdateBuffer();
protected:
	int m_nMethod;
	HTREEITEM m_hRootTi;

	void SetItemTree(BOOL bInit=TRUE);
	virtual BOOL GetCustomColor(HTREEITEM hItem);

	COLORREF GetBkColor();
	COLORREF GetBkColorTitle();

	// Generated message map functions
	BOOL IsAnalysisEnable();    // TRUE:입력됨, FALSE:미입력
	BOOL IsPOAnalysisEnable();  // TRUE:입력됨, FALSE:미입력 
	BOOL IsMOE2018();           // TRUE:입력됨, FALSE:미입력 
	BOOL IsMOE2019();           // TRUE:입력됨, FALSE:미입력 
	BOOL IsMOE();               // TRUE:입력됨, FALSE:미입력 
	BOOL IsKISTEC2019();
	BOOL IsAIK2021();

protected:
	void ToggleStyle(DWORD lStyleMask );
	//{{AFX_MSG(CTreeSeisCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	BOOL InsertTreeItemPolc(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_POLC_K, T_POLC_K>& aPolcK);
	BOOL InsertTreeItemPhgt(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_PHGT_K, T_PHGT_K>& aPhgtK);
	BOOL InsertTreeItemPhge(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_PHGE_K, T_PHGE_K>& aPhgeK);

	BOOL InsertTreeItemIehp(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_IEHP_K, T_IEHP_K>& aIehpK);
	BOOL InsertTreeItemIehg(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_IEHG_K, T_IEHG_K>& aIehgK);
	BOOL InsertTreeItemFimp(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_FIMP_K, T_FIMP_K>& aFimpK, BOOL bPushover=FALSE);
	BOOL InsertTreeItemFibr(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_FIBR_K, T_FIBR_K>& aFibrK, BOOL bPushover=FALSE);
	BOOL InsertTreeItemFibw(CDBDoc* pDoc, HTREEITEM hTI, CArray<T_FIBW_K, T_FIBW_K>& aFibwK, BOOL bPushover=FALSE);

	BOOL GetPushoverhingetypeString(CDBDoc* pDoc, T_PHGT_K PhgtK, CString& strName);
	BOOL GetInelastichingetypeString(CDBDoc* pDoc, T_IEHP_K IehpK, CString& strName);
	BOOL GetInelasticFiberTypeString(CDBDoc* pDoc, T_FIMP_K FimpK, CString& strName);
	BOOL GetInelastichingeAssignString(CDBDoc* pDoc, T_IEHG_K IehgK, CMap<T_STOR_K, T_STOR_K, CString, LPCTSTR>& mStorK2Name, CMap<T_ELEM_K, T_ELEM_K, T_STOR_K, T_STOR_K>& mWallElemK2StorK, CString& strName);

	HTREEITEM SetMainCtrlItemTree();
	HTREEITEM SetEffeStiffItemTree();
	HTREEITEM SetPropertyItemTree();
	HTREEITEM SetGeneralDgnParamItemTree();
	HTREEITEM SetRcDgnParamItemTree();
	HTREEITEM SetCommonParameItemTree();
	HTREEITEM SetInfillStrutItemTree();
	HTREEITEM SetMasonryInfillStrutTree();
	HTREEITEM SetPreItemTree();
	HTREEITEM Set1stItemTree();
	HTREEITEM Set2ndItemTree(CDBDoc* pDoc,BOOL bInit/*=TRUE*/);
	HTREEITEM SetPBDPOItemTree(CDBDoc* pDoc);
	HTREEITEM SetPBDTHItemTree(CDBDoc* pDoc);
	void SetPreItemTree_KISTEC(HTREEITEM& hRoot);
	void SetPreItemTree_MOE2018(HTREEITEM& hRoot);
	void SetPreItemTree_AIKG2021(HTREEITEM& hRoot);

	void EditProperty_Pushover_PHGE(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Pushover_PHGT(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	void EditProperty_Pushover_POLC(CDBDoc* pDoc, HTREEITEM ht, UINT data, int pos);
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__TREESEISCTRL_H__)
