#if !defined(AFX_HYDRMATERIALDLG_H__DACAD6A1_1DDA_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_HYDRMATERIALDLG_H__DACAD6A1_1DDA_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HydrMaterialDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxHmtp.h"
/////////////////////////////////////////////////////////////////////////////
// CHydrMaterialDlg dialog

class CHydrMaterialDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CHydrMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {};

public:
	// left list
	void	InitLRList();
	void	InitMatList();

	// mat list
	void	MakeItemExMatlist();
	BOOL	InsertItemMatlist(T_HMAT_K Key, T_HMAT_D &Data);
	int		FindInsertionPosMatlist(T_HMAT_K nID);
	CString	DataToStrMatlist(int i, T_HMAT_K Key, T_HMAT_D &Data);
	void	SetHeaderTitleMatlist();

	void	InitHmatData();

// Dialog Data
	//{{AFX_DATA(CHydrMaterialDlg)
	enum { IDD = IDD_TM_HYDR_MATERIAL };
	CCobxHmtp	m_CobxMatType;
	CListCtrl	m_matlist;
	CListCtrl	m_rlist;
	CListCtrl	m_llist;
	//}}AFX_DATA


// Overrides
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHydrMaterialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CHydrMaterialDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnTmHmatRmovebtn();
	afx_msg void OnDblclkTmHmatLlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmHmatLmovebtn();
	afx_msg void OnDblclkTmHmatRlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTmHmatAddbtn();
	afx_msg void OnTmHmatDelbtn();
	afx_msg void OnTmHmatTypebtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYDRMATERIALDLG_H__DACAD6A1_1DDA_11D5_880F_00010263A1F4__INCLUDED_)
