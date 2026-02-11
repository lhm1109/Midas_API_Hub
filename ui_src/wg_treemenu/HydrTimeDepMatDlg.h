#if !defined(AFX_HYDRTIMEDEPMATDLG_H__B113FD44_F486_4181_8A2D_E939E1710A76__INCLUDED_)
#define AFX_HYDRTIMEDEPMATDLG_H__B113FD44_F486_4181_8A2D_E939E1710A76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HydrTimeDepMatDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxTdmt.h"
#include "..\wg_db\wg_db_CobxTdme.h"

/////////////////////////////////////////////////////////////////////////////
// CHydrTimeDepMatDlg dialog
class CDBDoc;
class CHydrTimeDepMatDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	void SetTimeDefMatHeader();
	CHydrTimeDepMatDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	void    UpdateTdmtList();
	void    UpdateLList();
	void    UpdateRList();
	void    MakeItemExMatlist();
	BOOL    InsertItemMatlist(T_TDMT_K Key, T_TDMT_D &Data);
	int     FindInsertionPosMatlist(T_TDMT_K nID);
	CString DataToStrMatlist(int i, T_TDMT_K Key, T_TDMT_D &Data);
	
	void    AddToRList(CArray<T_MATL_K, T_MATL_K> & rToAdd);
	void    RemoveFromRList(CArray<T_MATL_K, T_MATL_K> & rToAdd);

// Dialog Data
	//{{AFX_DATA(CHydrTimeDepMatDlg)
	enum { IDD = IDD_TM_HYDR_TIME_DEP_MATERIAL };
	CCobxTdme	m_CobxTdme;
	CCobxTdmt	m_CobxMatType;
	CListCtrl	m_rlist;
	CListCtrl	m_llist;
	CListCtrl	m_wndTDefMat;
	//}}AFX_DATA


// Overrides
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHydrTimeDepMatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CHydrTimeDepMatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnTmMatTypebtn();
	afx_msg void OnTmHmatAddbtn();
	afx_msg void OnTmHmatDelbtn();
	afx_msg void OnTmHmatLmovebtn();
	afx_msg void OnTmHmatRmovebtn();
	afx_msg void OnTmElasticityTypebtn();
	afx_msg void OnDblclkTmSourceMatTypelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTmSelectedMatTypelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedTmTdefMatlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYDRTIMEDEPMATDLG_H__B113FD44_F486_4181_8A2D_E939E1710A76__INCLUDED_)
