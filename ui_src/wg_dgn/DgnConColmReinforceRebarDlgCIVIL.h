#if !defined(__DGNCONCOLMREINFORCEREBARDLGCIVIL_H__)
#define __DGNCONCOLMREINFORCEREBARDLGCIVIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMRebarDlg dialog
class CDgnConColmReinforceDlgCIVIL;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnConColmReinforceRebarDlgCIVIL : public CDialogMove
{
// Construction
public:
	CDgnConColmReinforceRebarDlgCIVIL(CWnd* pParent = NULL);   // standard constructor
	
// interface
public:
	
// Dialog Data
	//{{AFX_DATA(CDgnConColmReinforceRebarDlgCIVIL)
	enum { IDD = IDD_DGN_CON_REIN_COLM_REBAR_DLG };

	CDlgTabCtrl	m_ctrlTab;
	//}}AFX_DATA

	enum {DGNCON_BTN_HIDE = 0, DGNCON_BTN_SHOW, DGNCON_BTN_DISABLE, DGNCON_BTN_ENABLE};

	// Implementation
	void SetStatusAddBtn(int nStatus);
	void SetStatusDeleteBtn(int nStatus);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConColmReinforceRebarDlgCIVIL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	virtual void OnCancel();

protected:
	CDgnConColmReinforceDlgCIVIL* m_pColumnDlg;
	
protected:
	void LayoutChildDlg();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnConColmReinforceRebarDlgCIVIL)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnAdd();
	afx_msg void OnDgnDelete();
	

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNCONCOLMREINFORCEREBARDLGCIVIL_H__)
