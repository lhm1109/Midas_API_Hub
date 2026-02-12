#if !defined(__CMDAUTOLOADCOMBSELCVLDLG_H__)
#define __CMDAUTOLOADCOMBSELCVLDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CmdAutoLoadCombSelCvlDlg.h : header file
//
#include "wg_cmd.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombSelCvlDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCmdAutoLoadCombSelCvlDlg : public CDialogMove
{
// Construction
public:
	CCmdAutoLoadCombSelCvlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCmdAutoLoadCombSelCvlDlg)
	enum { IDD = IDD_CMD_LCOMBDEFAULT_SEL_DLG };

	BOOL m_bUlti1;
	BOOL m_bUlti2;
	BOOL m_bUlti3;
	BOOL m_bUlti4;
	BOOL m_bUlti5;
	BOOL m_bExtr1;
	BOOL m_bExtr2;
	BOOL m_bServ1;
	BOOL m_bServ2;
	BOOL m_bServ3;
	BOOL m_bServ4;
	BOOL m_bServ5;
	BOOL m_bFati1;

public:
	void SetDefaultValue();
	void EnableDisableControls();

	int m_nDesignType;
		
		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCmdAutoLoadCombSelCvlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
	//{{AFX_MSG(CCmdAutoLoadCombSelCvlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdAutoCombSelAll();
	afx_msg void OnCmdAutoCombUnSelAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMDAUTOLOADCOMBSELCVLDLG_H__)
