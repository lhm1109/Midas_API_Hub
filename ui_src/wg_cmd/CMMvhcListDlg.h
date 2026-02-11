#if !defined(__CMMVHCLISTDLG_H__)
#define __CMMVHCLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhcListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvhcListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhcListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMMvhcListDlg(CWnd* pParent = NULL);   // standard constructor
	void SetModeAddOnly() { m_bAddOnly = TRUE; }

//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------------
// Implementation
protected:
	void SetHeaderTitle();
	virtual void MakeItemEx(BOOL bUnitChanged=FALSE);

	int  FindInsertionPos();
	BOOL InsertItem(T_MVHC_K Key, T_MVHC_D &Data);
	BOOL DeleteItem(T_MVHC_K Key, T_MVHC_D &Data);
	BOOL ModifyItem(T_MVHC_K KeyOld, T_MVHC_K Key, T_MVHC_D &Data);
	CString DataToStr(int i, T_MVHC_K Key, T_MVHC_D &Data);

protected:
	CDBDoc* m_pDoc;
	BOOL m_bAddOnly;

public:
	// Dialog Data
	//{{AFX_DATA(CCMMvhcListDlg)
	enum { IDD = IDD_CMD_ML_MVHC_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhcListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvhcListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVHCLISTDLG_H__)
