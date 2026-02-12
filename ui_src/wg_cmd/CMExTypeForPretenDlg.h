#if !defined(__CMEXTYPEFORPRETENDLG_H__)
#define __CMEXTYPEFORPRETENDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMExTypeForPretenDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CCMExTypeForPretenDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMExTypeForPretenDlg : public CDialogMove
{
// Construction
public:
	CCMExTypeForPretenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMExTypeForPretenDlg)
	enum { IDD = IDD_PRES_EXLD };
	CListCtrl	m_ExternalList;
	CListCtrl	m_StaticList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMExTypeForPretenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<T_STLD_K, T_STLD_K> m_arStldKeyList;
	CArray<BOOL, BOOL> m_arBExternal;

	void MakeListHeader();
	void MakeItemEx();
	BOOL GetSelectedIndex(BOOL bExternal, int &nIndex);
	void AddList(T_STLD_K key, BOOL bExternal);
	void ChangeValue(T_STLD_K key);
	CString DataToStr(int nIndex, T_STLD_K key);

	// Generated message map functions
	//{{AFX_MSG(CCMExTypeForPretenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSelButton();
	afx_msg void OnCmdUnselButton();
	virtual void OnOK();
	afx_msg void OnCmdRemoveData();
	afx_msg void OnDblclkCmdAllList();
	afx_msg void OnDblclkCmdSelectedList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMEXTYPEFORPRETENDLG_H__)
