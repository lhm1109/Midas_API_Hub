#if !defined(__CMRESULTLCTBDLG_H__)
#define __CMRESULTLCTBDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMResultLctbDlg.h : header file
//
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMResultLctbDlg dialog
class CCMResultLctbGrid;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMResultLctbDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMResultLctbDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMResultLctbDlg();  

// Dialog Data
	//{{AFX_DATA(CCMResultLctbDlg)
	enum { IDD = IDD_CMD_RESULT_LCTB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMResultLctbDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

protected:
	CCMResultLctbGrid* m_wndGrid;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMResultLctbDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMRESULTLCTBDLG_H__)
