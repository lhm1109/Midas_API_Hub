#if !defined(__WINDSELSTORY_H__)
#define __WINDSELSTORY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindSelStory.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_CompFunc.h"
/////////////////////////////////////////////////////////////////////////////
// CWindSelStoryDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CWindSelStoryDlg : public CDialogMove
{
	// Construction
public:
	CWindSelStoryDlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CWindSelStoryDlg)
	enum { IDD = IDD_ETC_WIND_SEL_STORY };
	CListBox	m_lstSel;
	CListBox	m_lstUnSel;
	//}}AFX_DATA
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMElsPlsMatlAddDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
private:
	CArray<T_STOR_K, T_STOR_K> m_aStorK;
	CArray<T_STOR_K, T_STOR_K> m_aSelStorK;
	
public:
	void GetSelStorK(CArray<T_STOR_K, T_STOR_K>& aStorK) { aStorK.Copy(m_aSelStorK); }

private:
	CDBDoc*  m_pDoc;

private:
	void GetSelListStorK(CArray<T_STOR_K, T_STOR_K>& aStorK);
	BOOL GetSelListData(CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&>& aPosIdPair, CStringArray& aName, BOOL bAll);
	BOOL GetUnSelListData(CArray<T_POS_INT_PAIR, T_POS_INT_PAIR&>& aPosIdPair, CStringArray& aName, BOOL bAll);
	void UpdateUnSelList();

protected:
	// Generated message map functions
	//{{AFX_MSG(CCMElsPlsMatlAddDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelectionBtn();
	afx_msg void OnUnselectionBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__WINDSELSTORY_H__)