#if !defined(__CMDAUTOLOADCOMBCVLEUROROADDLG_H__)
#define __CMDAUTOLOADCOMBCVLEUROROADDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlEuroRoadDlg.h : header file
//


#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlEuroRoadDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCmdAutoLoadCombCvlEuroRoadDlg : public CDialogMove
{
// Construction
public:
	CCmdAutoLoadCombCvlEuroRoadDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdAutoLoadCombCvlEuroRoadDlg();   // standard deconstruction

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlEuroRoadDlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_EURO_ROAD_DLG };

	CListBox m_lstChra;
	CListBox m_lstFreq;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlEuroRoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CDBDoc* m_pDoc;

	CArray<T_MVLD_K, T_MVLD_K> m_aMvldChar;
	CArray<T_MVLD_K, T_MVLD_K> m_aMvldFreq;

	CArray<BOOL, BOOL> m_aSelFlag;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlEuroRoadDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdEuroRoadAddBtn();
	afx_msg void OnCmdEuroRoadDelBtn();
	afx_msg void OnCmdEuroRoadDblclk();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBCVLEUROROADDLG_H__)
