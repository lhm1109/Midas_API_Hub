#if !defined(AFX_STLDFSSFDLG_H__5640AF59_F464_4647_AB5B_6342980F4E34__INCLUDED_)
#define AFX_STLDFSSFDLG_H__5640AF59_F464_4647_AB5B_6342980F4E34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldFssfDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditTemp.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldFssfDlg dialog

class CStldFssfDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldFssfDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void Execute() { OnTmExecute(); }
// Dialog Data
	//{{AFX_DATA(CStldFssfDlg)
	enum { IDD = IDD_TM_STLD_FIRE_FSSF };
private:
	MComboBox	m_wndProtection;
	int			m_nOption;
	BOOL		m_bAutoCalc;
	double		m_dUserFactor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldFssfDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CStldFssfDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void IsAutoCalc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDFSSFDLG_H__5640AF59_F464_4647_AB5B_6342980F4E34__INCLUDED_)
