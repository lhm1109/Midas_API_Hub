#if !defined(AFX_CMMLSRDLG_H__B921B17F_F98B_4CD6_B02C_630CA6DC09F4__INCLUDED_)
#define AFX_CMMLSRDLG_H__B921B17F_F98B_4CD6_B02C_630CA6DC09F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMlsrDlg.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxThfc.h"
#include "..\wg_db\wg_db_SelectLC.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMlsrDlg dialog

#include "CMChildBarBase.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCMMlsrDlg : public CCMChildBarBase, public CDBUpdateConnector
{
public:
	CCMMlsrDlg();           // protected constructor used by dynamic creation
//	DECLARE_DYNCREATE(CCMMlsrDlg)

//----------------------------------------------------------------
// Overridables
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------
// Implementations
protected:
	void SetListCtrlHeader();
	void MakeItemEx();
	CString DataToStr(int i, int nNo, T_MLSR_K key);
	void SetItem(int nIndex, T_MLSR_K Key);
	void InsertItem(T_MLSR_K Key);
	void DeleteItem(T_MLSR_K Key);
	void OnDelKeyPressedInList();
	void GetSelectedItemList(CListCtrl* pList, CArray<int, int> &aSelItem);
	virtual void Execute() { OnCmdApply(); };

public:
	CDBDoc* m_pDoc;

protected:
	CArray<UINT, UINT> m_aCtrlOp;
// Dialog Data
public:
	//{{AFX_DATA(CCMMlsrDlg)
	enum { IDD = IDD_DBAR_MLSR_DLG };
 	CListCtrl	m_List;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMlsrDlg)
 	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCMMlsrDlg();

	// Generated message map functions
	//{{AFX_MSG(CCMMlsrDlg)	
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdClose();	
	afx_msg void OnClicklist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMLSRDLG_H__B921B17F_F98B_4CD6_B02C_630CA6DC09F4__INCLUDED_)
