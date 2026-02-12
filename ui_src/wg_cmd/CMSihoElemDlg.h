#if !defined(__CMSihoElemDlg_H__)
#define __CMSihoElemDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSihoElemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxIehp.h"

#include "CMChildBarBase.h"

//#include "MenuBarChildDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSihoElemDlg dialog

class CCMSihoElemDlg : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CCMSihoElemDlg();
	//DECLARE_DYNCREATE(CCMSihoElemDlg)

	enum ELEM_TYPE { ET_ALL=0, ET_TRUSS, ET_BEAM, ET_WALL };
// Dialog Data
	//{{AFX_DATA(CCMSihoElemDlg)
	enum { IDD = IDD_CMD_HINGE_OUTPUT_ELEM };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSihoElemDlg)
public:
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
//	virtual void OnInitialUpdate();

public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	int m_nAddDelete;
	int m_nElemType;
	BOOL m_bFiberSectionResult;
	CListCtrl m_list;

// Implementation
protected:	
	// Generated message map functions
	//{{AFX_MSG(CCMSihoElemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddBtn();
	afx_msg void OnDeleteBtn();
	afx_msg void OnClose();
	afx_msg void OnClickSelList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnElemTypeRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void GetElementByKind(CArray<UINT,UINT>& aKey,int nElemType);
	void InitList();
	void MakeListItem();

	BOOL m_bExistFiber;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSihoElemDlg_H__)
