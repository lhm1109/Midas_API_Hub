#if !defined(AFX_BNDRDRLSDLG_H__D5822E25_3602_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDRDRLSDLG_H__D5822E25_3602_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrDrlsDlg.h : header file
//
#include "..\wg_db\wg_db_CobxBngr.h"
/////////////////////////////////////////////////////////////////////////////
// CBndrDrlsDlg dialog

class CBndrDrlsDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CBndrDrlsDlg(CWnd* pParent = NULL);   // standard constructor
//----------------------------------------------------------------
// Overridables
public:
	virtual void Execute() { OnTmExecute(); }
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------
// Implementations
protected:
	void SetListCtrlHeader();
	void MakeItemEx();
	CString DataToStr(int i, int nNo, T_DRLS_K key);
	void SetItem(int nIndex, T_DRLS_K Key);
	void InsertItem(T_DRLS_K Key);
	void DeleteItem(T_DRLS_K Key);
	void OnDelKeyPressedInList();
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);

protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aCtrlOp;

// Dialog Data
	//{{AFX_DATA(CBndrDrlsDlg)
	enum { IDD = IDD_TM_BNDR_DRLS };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrDrlsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBndrDrlsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnTmExecute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRDRLSDLG_H__D5822E25_3602_11D4_92DE_0000C0B0E6B3__INCLUDED_)
