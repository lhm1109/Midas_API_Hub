// SODPosiDgnOutDlg.h: interface for the CSODPosiDgnOutDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__SODPOSIDGNOUTDLG_H__)
#define __SODPOSIDGNOUTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnDlgBase.h"
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN.h"

class CDBDoc;

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class __MY_EXT_CLASS__ CSODPosiDgnOutDlg : public CMenuBarChildDlg
{
public:
	CSODPosiDgnOutDlg(CWnd* pParent = NULL);
	
	// Dialog Data
	//{{AFX_DATA(CSODPosiDgnOutDlg)
	enum { IDD = IDD_DGN_STL_ORTHO_DECK_POSI_DGN_OUT_DLG };
		
	int		m_nOption;
	int		m_iPosi;
	//}}AFX_DATA
	
		
public:
	void Initial_SelectItem();
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSODPosiDgnOutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	CDBDoc* m_pDoc;
	
	BOOL Dlg2Data();
	
	T_SDDO_D m_Data;
	
	// Generated message map functions
	//{{AFX_MSG(CSODPosiDgnOutDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnClose();
// 	afx_msg void OnDgnExecute();
	afx_msg void OnDgnSODPosiDgnAdd();
	afx_msg void OnDgnSODPosiDgnDel();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SODPOSIDGNOUTDLG_H__)
