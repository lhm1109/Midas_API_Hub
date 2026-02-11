#if !defined(AFX_CMTHKITEMCALCDLG_H__6D3506A1_67B9_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMTHKITEMCALCDLG_H__6D3506A1_67B9_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThkItemCalcDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMThkItemCalcDlg dialog

class CCMThkItemCalcDlg : public CDialogMove
{
// Construction
public:
	CCMThkItemCalcDlg(CDBDoc* pDoc, T_THIK_D* thik, CWnd* pParent = NULL);   // standard constructor

public:
	CDBDoc*		m_pDoc;
	T_THIK_D*	m_pThikData;

public:
	void	MakeUnitSystem();
	void	Data2Dlg();

// Dialog Data
	//{{AFX_DATA(CCMThkItemCalcDlg)
	enum { IDD = IDD_CMD_TP_CALC_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThkItemCalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBCGPStatic   m_wndTitle[9];
	CEditUnit	m_wndEdit[9];
	CTextUnit m_wndUnit[9];

	// Generated message map functions
	//{{AFX_MSG(CCMThkItemCalcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdTpCalcBtnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHKITEMCALCDLG_H__6D3506A1_67B9_11D5_880F_00010263A1F4__INCLUDED_)
