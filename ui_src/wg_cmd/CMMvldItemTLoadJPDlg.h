#if !defined(AFX_CMMVLDITEMTLOADJPDLG_H__F8416494_EE4A_4552_9F9C_BCBCCAAB2EE9__INCLUDED_)
#define AFX_CMMVLDITEMTLOADJPDLG_H__F8416494_EE4A_4552_9F9C_BCBCCAAB2EE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemTLoadJPDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemTLoadJPDlg dialog

class CCMMvldItemTLoadJPDlg : public CChildDialog
{
// Construction
public:
	CCMMvldItemTLoadJPDlg(CWnd* pParent = NULL);   // standard constructor

public:
	BOOL GetCurDlgData(T_MVLDjp_D* pData);
	void SetCurDlgData(T_MVLDjp_D* pData);

protected:
	void SetData2Dlg();

protected:
	CDBDoc* m_pDoc;
	T_MVLDjp_D m_Data;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldItemTLoadJPDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_TLOAD_JAPAN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemTLoadJPDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvldItemTLoadJPDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMTLOADJPDLG_H__F8416494_EE4A_4552_9F9C_BCBCCAAB2EE9__INCLUDED_)
