#if !defined(AFX_MHTPMATERIALDLG_H__DACAD6A1_1DDA_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_MHTPMATERIALDLG_H__DACAD6A1_1DDA_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MhtpMaterialDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxMhtp.h"
/////////////////////////////////////////////////////////////////////////////
// CMhtpMaterialDlg dialog

class CMhtpMaterialDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	void SetHighTemperDefMatHeader();
	CMhtpMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() {};
	void    UpdateMhtpList();
	void    UpdateLList();
	void    UpdateRList();
	void    MakeItemExMatlist();
	BOOL    InsertItemMatlist(T_MHTP_K Key, T_MHTP_D &Data);
	int     FindInsertionPosMatlist(T_MHTP_K nID);
	CString DataToStrMatlist(int i, T_MHTP_K Key, T_MHTP_D &Data);

	void    AddToRList(CArray<T_MATL_K, T_MATL_K> & rToAdd);
	void    RemoveFromRList(CArray<T_MATL_K, T_MATL_K> & rToAdd);

// Dialog Data
	//{{AFX_DATA(CMhtpMaterialDlg)
	enum { IDD = IDD_TM_MHTP_MATERIAL };
	CCobxMhtp	m_CobxMatType;
	CListCtrl	m_wndTDefMat;
	CListCtrl	m_rlist;
	CListCtrl	m_llist;
	//}}AFX_DATA


// Overrides
	virtual	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMhtpMaterialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CMhtpMaterialDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnTmMhatAddbtn();
	afx_msg void OnTmMhatDelbtn();
	afx_msg void OnTmMhatLmovebtn();
	afx_msg void OnTmMhatRmovebtn();
	afx_msg void OnTmMalTypebtn();
	afx_msg void OnDblclkTmMhatLlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTmMhatRlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedTmTdefMatlist(NMHDR* pNMHDR, LRESULT* pResult);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MHTPMATERIALDLG_H__DACAD6A1_1DDA_11D5_880F_00010263A1F4__INCLUDED_)
