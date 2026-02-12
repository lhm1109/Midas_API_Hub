#if !defined(__CMDAUTOLOADCOMBCVLIRS_DRL_DLG_H__)
#define __CMDAUTOLOADCOMBCVLIRS_DRL_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlIRS_Drl_Dlg.h : header file
//


#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRS_DRL_Dlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCmdAutoLoadCombCvlIRS_DRL_Dlg : public CDialogMove
{
// Construction
public:
	CCmdAutoLoadCombCvlIRS_DRL_Dlg(CWnd* pParent = NULL);   // standard constructor
  ~CCmdAutoLoadCombCvlIRS_DRL_Dlg();   // standard deconstruction

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlIRS_DRL_Dlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_IRS_DLG };

  CListBox m_lstChra;//ult
  CListBox m_lstFreq;//serv
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlIRS_DRL_Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
  CDBDoc* m_pDoc;

	CArray<T_STLD_K, T_STLD_K>  m_aMvldChar;
	CArray<T_STLD_K, T_STLD_K>  m_aMvldFreq;

	CArray<BOOL, BOOL> m_aSelFlag;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlIRS_DRL_Dlg)
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

#endif // !defined(__CmdAutoLoadCombCvlIRS_Drl_Dlg_H__)
