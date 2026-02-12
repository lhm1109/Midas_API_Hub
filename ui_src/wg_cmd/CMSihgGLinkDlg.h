#if !defined(__CMSihgGLinkDlg_H__)
#define __CMSihgGLinkDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSihgGLinkDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxIehp.h"

#include "CMChildBarBase.h"

//#include "MenuBarChildDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSihgGLinkDlg dialog

class CCMSihgGLinkDlg : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CCMSihgGLinkDlg();
	//DECLARE_DYNCREATE(CCMSihgGLinkDlg)

// Dialog Data
	//{{AFX_DATA(CCMSihgGLinkDlg)
	enum { IDD = IDD_CMD_HINGE_OUTPUT_GLINK };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSihgGLinkDlg)
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
//	virtual void OnInitialUpdate();

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	CListCtrl m_listSel;  // 선택 후보
	CListCtrl m_listData; // 입력한 데이터

// Implementation
protected:	
	// Generated message map functions
	//{{AFX_MSG(CCMSihgGLinkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddBtn();
	afx_msg void OnDeleteBtn();
	afx_msg void OnClose();
	afx_msg void OnSelBtn();
	afx_msg void OnUnselBtn();
	afx_msg void OnClickSelList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void GetElementByKind(CArray<UINT,UINT>& aKey,int nElemType);
	void InitList();
	void MakeListItem();
	void MakeListItem_sel();
	void MakeListItem_data();
	void GetSelectedKey(CArray<T_NLNK_K,T_NLNK_K>& aNlnkK);
	afx_msg void OnNMClickShelllist(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ReverseCheck();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSihgGLinkDlg_H__)
