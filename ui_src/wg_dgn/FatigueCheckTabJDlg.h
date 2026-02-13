#if !defined(AFX_FATIGUECHECKTABJDLG_H__)
#define AFX_FATIGUECHECKTABJDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FatigueCheckTabIDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFatigueCheckTabJDlg dialog
#include "..\wg_base\ChildDialog.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CFatigueCheckDlg;

class __MY_EXT_CLASS__ CFatigueCheckTabJDlg : public CChildDialog
{
	// Construction
public:
	CFatigueCheckTabJDlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CFatigueCheckTabJDlg)
	enum { IDD = IDD_DGN_DESIGN_FATI_CHECK_TAB_DLG };
	
	CEditUnit m_edtt0;
	//}}AFX_DATA
	
	
	// Overrides
	//{{AFX_VIRTUAL(CFatigueCheckTabJDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
public:
	void GetData(T_FACK_D &FackD);
	void InitUnit();
	void ControlsEnableDisable();
	
	void Dlg2Data();
	
	CFatigueCheckDlg* m_pParent;
	
	T_FACK_D m_Data;
		
	// Generated message map functions
	//{{AFX_MSG(CFatigueCheckTabJDlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FATIGUECHECKTABJDLG_H__)
