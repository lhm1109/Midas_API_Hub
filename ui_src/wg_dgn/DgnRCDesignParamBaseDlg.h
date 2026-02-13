#if !defined(AFX_DGNRCDESIGNPARAMBASEDLG_H__A5FA86BC_EA80_42CC_B555_B733E29842FF__INCLUDED_)
#define AFX_DGNRCDESIGNPARAMBASEDLG_H__A5FA86BC_EA80_42CC_B555_B733E29842FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRCDesignParamBaseDlg.h : header file
//
#include "DgnDlgBase.h"

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\DB_ST_DN.h"
//#include "DgnTabCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// DgnRCDesignParamBaseDlg dialog
// add by maxiao 2006.08.08  RC Design input params main dialog

class DgnRCDesignParamMatDlg;
class DgnRCDesignParamParamDlg;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ DgnRCDesignParamBaseDlg :public CDialogMove//: public CDgnDlgBase, public CDBUpdateConnector//public CDialog
{
// Construction
public:
	DgnRCDesignParamBaseDlg(CWnd* pParent = NULL);   // standard constructor
	~DgnRCDesignParamBaseDlg();   // standard Destructor

// Dialog Data
	//{{AFX_DATA(DgnRCDesignParamBaseDlg)
	enum { IDD = IDD_DGN_RC_DESIGN_PARAM_BASE };
		// NOTE: the ClassWizard will add data members here
	CButton	m_btnAdd;
	CButton m_btnMod;
	CButton	m_btnDel;
	CDlgTabCtrl	m_ctrlTab;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DgnRCDesignParamBaseDlg)
	public:
	//virtual BOOL DestroyWindow();//add by maxiao
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaveOrUpdate();

protected:  
	DgnRCDesignParamMatDlg* m_pMatDlg;
 	DgnRCDesignParamParamDlg* m_pParamDlg;
	void LayoutChildDlg();//add by maxiao
	// Generated message map functions
	//{{AFX_MSG(DgnRCDesignParamBaseDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnClose();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);

	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNRCDESIGNPARAMBASEDLG_H__A5FA86BC_EA80_42CC_B555_B733E29842FF__INCLUDED_)
