#if !defined(__CMStageAdditionalConsDlg_H__)
#define __CMStageAdditionalConsDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//

//#include "wg_cmdres.h"
#include "CMDlgBarBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStageAdditionalBuckDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "HeaderPre.h"

class CCMStageAdditionalConsDlg : public CDialogMove, public CDBUpdateConnector
{

public:
	void CtrlMan();
	BOOL UpdateAdditionalAnalList();
	CCMStageAdditionalConsDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	// Dialog Data
		//{{AFX_DATA(CCMStageConsDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_ADDITIONAL_CONS_DLG };
	CListCtrl	m_wndStageList;

	//}}AFX_DATA
private:
	void LayoutCtrl(void);

	//CString GetLoadCasesString(const CArray<T_STLD_K, T_STLD_K>& aLoadCases);
	//CString GetBucklingStatusString(const T_CSPA_D& CspaD);

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CCMStageConsDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStageConsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdDelete1();
	afx_msg void OnCmdModify1();
	afx_msg void OnDblclkStageList(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnCmdShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#include "HeaderPost.h"

#endif