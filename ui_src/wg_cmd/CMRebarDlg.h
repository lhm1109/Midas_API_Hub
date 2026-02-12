#if !defined(__CMREBARDLG_H__)
#define __CMREBARDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMRebarDlg dialog
class CCMRebarPscDlg;
class CCMRebarCompositeDlg;

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMRebarDlg : public CDialogMove
{
// Construction
public:
	CCMRebarDlg(CWnd* pParent = NULL);   // standard constructor
	
// interface
public:
	void SetParamDataPsc(T_RPSC_K RpscK); 
	void SetParamDataComposite(T_RPSC_K RpscK); 

// Dialog Data
	//{{AFX_DATA(CCMRebarDlg)
	enum { IDD = IDD_CMD_REBAR };

	CDlgTabCtrl	m_ctrlTab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_RPSC_K m_KeyPsc;
	T_RPSC_K m_KeyComposite;
	CCMRebarPscDlg* m_pPscDlg;
	CCMRebarCompositeDlg* m_pCompositeDlg;

protected:
	void LayoutChildDlg();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMRebarDlg)
	virtual BOOL OnInitDialog();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARDLG_H__)
