#if !defined(__CMDAUTOLOADCOMBCVLIRC_SPLVEHDLG_H__)
#define __CMDAUTOLOADCOMBCVLIRC_SPLVEHDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombCvlIRC_SPLVEHDlg.h : header file
//


#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlIRC_SPLVEHDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCmdAutoLoadCombCvlIRC_SPLVEHDlg : public CDialogMove
{
// Construction
public:
	CCmdAutoLoadCombCvlIRC_SPLVEHDlg(CWnd* pParent = NULL);   // standard constructor
	~CCmdAutoLoadCombCvlIRC_SPLVEHDlg();   // standard deconstruction

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombCvlIRC_SPLVEHDlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_IRC_SPVEH_DLG };

	CListBox m_lstChra;
	CListBox m_lstFreq;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombCvlIRC_SPLVEHDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CDBDoc* m_pDoc;

	CArray<T_MVLDid_K, T_MVLDid_K> m_aMvldSPV;
	CArray<T_MVLDid_K, T_MVLDid_K> m_aMvldSPVSel;
    CArray<T_MVLDid_K, T_MVLDid_K> m_aMvldSPVTmp;
    CArray<T_MVLDid_K, T_MVLDid_K> m_aMvldSPVSelTmp;
	CArray<BOOL, BOOL> m_aSelFlag;

	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombCvlIRC_SPLVEHDlg)
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
