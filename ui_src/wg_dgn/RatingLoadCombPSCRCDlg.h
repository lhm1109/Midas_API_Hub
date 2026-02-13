#if !defined(__CMRatingLoadCombPSCRCDlg_H__)
#define __CMRatingLoadCombPSCRCDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RatingLoadCombPSCRCDlg.h : header file
//


//#include "..\wg_db\wg_db_DlgBase.h"
#include "RatingLoadCombCommonDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombPSCRCDlg dialog

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
class __MY_EXT_CLASS__ CRatingLoadCombPSCRCDlg : public CRatingLoadCombCommonDlg
{
// Construction
public:
	CRatingLoadCombPSCRCDlg(CWnd* pParent = NULL);   // standard constructor
	~CRatingLoadCombPSCRCDlg();

// Dialog Data
	//{{AFX_DATA(CRatingLoadCombPSCRCDlg)
	enum { IDD = IDD_DGN_RATING_LOADCOMB_PSC_DLG };
	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRatingLoadCombPSCRCDlg)
	public:
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void UpdateBuffer();
// Implementation
protected:
	 
	// Generated message map functions
	//{{AFX_MSG(CRatingLoadCombPSCRCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint(); 
	afx_msg void OnCopyToLCom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	int m_nDlgType;
	void SetDlgType(int nType) {m_nDlgType = nType;}// 0:psc 1:RC

};
/////////////////////////////////////
#include "HeaderPost.h"     /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMRatingLoadCombPSCRCDlg_H__)