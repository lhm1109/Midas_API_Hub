#if !defined(AFX_BNDRSTORYDLG_H__EA860063_4090_4AE7_BD57_3BB645F7C313__INCLUDED_)
#define AFX_BNDRSTORYDLG_H__EA860063_4090_4AE7_BD57_3BB645F7C313__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrStoryDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBndrStoryDlg dialog
#include "BndrStoryGroupGrid.h"

class CBndrStoryDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrStoryDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void Execute() {};
//  void SetBndrTypeList();
//  unsigned int GetSourceGroupKey();
//  unsigned int GetTargetGroupKey();
//  void OptionCtrlMan();

// Dialog Data
	//{{AFX_DATA(CBndrStoryDlg)
	enum { IDD = IDD_TM_BNDR_STORY_GROUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrStoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBndrStoryGroupGrid m_grid;

	// Generated message map functions
	//{{AFX_MSG(CBndrStoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRSTORYDLG_H__EA860063_4090_4AE7_BD57_3BB645F7C313__INCLUDED_)
