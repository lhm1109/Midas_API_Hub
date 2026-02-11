





#if !defined(AFX_CMBGENTABLEDLG_H__84EB48C4_45BB_11D4_B00F_0000C0B9C58C__INCLUDED_)
#define AFX_CMBGENTABLEDLG_H__84EB48C4_45BB_11D4_B00F_0000C0B9C58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMBgenTableDlg.h : header file
//
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"  // for T_MAKEBUILDING
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "CMBgenTableGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCMBgenTableDlg dialog

class CCMBgenTableDlg : public CDialogMove
{
// Construction
public:
	CCMBgenTableDlg(CWnd* pParent = NULL);   // standard constructor

	CCMBgenTableGrid m_wndGrid;
// Dialog Data
	//{{AFX_DATA(CCMBgenTableDlg)
	enum { IDD = IDD_ETC_BGEN_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMBgenTableDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CArray<T_MAKEBUILDING, T_MAKEBUILDING&> m_arMakeBuilding;

protected:
	void InitGrid();

	BOOL m_bGridInit;
	// Generated message map functions
	//{{AFX_MSG(CCMBgenTableDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMBGENTABLEDLG_H__84EB48C4_45BB_11D4_B00F_0000C0B9C58C__INCLUDED_)
