#if !defined(__CMRatingLoadCombSteelDlg_H__)
#define __CMRatingLoadCombSteelDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLoadCombSteelDlg.h : header file
//


//#include "..\wg_db\wg_db_DlgBase.h"
#include "RatingLoadCombCommonDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombSteelDlg dialog

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
class __MY_EXT_CLASS__ CRatingLoadCombSteelDlg : public CRatingLoadCombCommonDlg
{
// Construction
public:
	CRatingLoadCombSteelDlg(CWnd* pParent = NULL);   // standard constructor
	~CRatingLoadCombSteelDlg();

// Dialog Data
	//{{AFX_DATA(CRatingLoadCombSteelDlg)
	enum { IDD = IDD_DGN_RATING_LOADCOMB_STEEL_DLG };
	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingLoadCombSteelDlg)
	public:
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void UpdateBuffer();
// Implementation
protected:
		
	// Generated message map functions
	//{{AFX_MSG(CRatingLoadCombSteelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint(); 
	afx_msg void OnCopyToLCom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
};
/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMRatingLoadCombSteelDlg_H__)