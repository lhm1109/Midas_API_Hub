#if !defined(__DGNREBARCPGDLG_H__)
#define __DGNREBARCPGDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRebarCpgDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgDlg dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnRebarCpgDlg : public CDialogMove
{
// Construction
public:
	CDgnRebarCpgDlg(CWnd* pParent = NULL);   // standard constructor
	
// interface
public:
	//void SetParamDataPsc(T_RPSC_K RpscK); 

// Dialog Data
	//{{AFX_DATA(CDgnRebarCpgDlg)
	enum { IDD = IDD_DGN_REBAR_CPG };

	CDlgTabCtrl	m_ctrlTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRebarCpgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//T_RPSC_K m_KeyPsc;
	//T_RPSC_K m_KeyComposite; 
	
protected:
	void LayoutChildDlg();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnRebarCpgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNREBARCPGDLG_H__)
