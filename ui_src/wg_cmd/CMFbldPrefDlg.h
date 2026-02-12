#if !defined(AFX_CMFBLDPREFDLG_H__6E84FE84_9743_44B9_B91F_59795F67AA93__INCLUDED_)
#define AFX_CMFBLDPREFDLG_H__6E84FE84_9743_44B9_B91F_59795F67AA93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMFbldPrefDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMFbldPrefDlg dialog
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\DB_ST_DT_PREF.h"
#include "..\wg_base\wg_base_NotifyListCtrl.h"
#include "..\wg_base\wg_base_InternationalDlg.h"

class CCMFbldPrefDlg : public CInternationalDlg
{
// Construction
	int m_nX, m_nY;
	T_PREFERENCE m_Preference;
	double m_CurLoadValue;
public:
	CCMFbldPrefDlg(CWnd* pParent = NULL);   // standard constructor
	int MyDoModal(int XPos, int YPos);
	void MakeListHeader();
	void ShowLoadData();
	double GetCurLoadValue();
	void SetCurLoadValue();

	LRESULT OnListCtrlKeyUp(WPARAM wParam, LPARAM lParam);
// Dialog Data
	//{{AFX_DATA(CCMFbldPrefDlg)
	enum { IDD = IDD_CMD_FBLD_PREF_DLG };
	CNotifyListCtrl	m_wndFLoadList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMFbldPrefDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMFbldPrefDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickCmdFloadList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedCmdFloadList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkCmdFloadList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMFBLDPREFDLG_H__6E84FE84_9743_44B9_B91F_59795F67AA93__INCLUDED_)
