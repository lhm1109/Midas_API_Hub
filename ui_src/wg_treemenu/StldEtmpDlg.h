#if !defined(AFX_STLDETMPDLG_H__5640AF59_F464_4647_AB5B_6342980F4E34__INCLUDED_)
#define AFX_STLDETMPDLG_H__5640AF59_F464_4647_AB5B_6342980F4E34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldEtmpDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditTemp.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
/////////////////////////////////////////////////////////////////////////////
// CStldEtmpDlg dialog

class CStldEtmpDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldEtmpDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void Execute() { OnTmExecute(); }
// Dialog Data
	//{{AFX_DATA(CStldEtmpDlg)
	enum { IDD = IDD_TM_STLD_ETMP };
	CTextUnit	m_wndT1Unit;
	CEditUnit	m_wndT1Edit;
	CTextUnit	m_wndT0Unit;
	CEditTemp	m_wndT0Edit;
	CSelectLC	m_LoadCase;
	int		m_nOption;
	CCobxLdgr m_wndLdgrCobx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldEtmpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aDeleteCtrl;

	// Generated message map functions
	//{{AFX_MSG(CStldEtmpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnChangeOption();
	afx_msg void OnTmProjConfBtn();
	afx_msg void OnTmDefineLdgrBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDETMPDLG_H__5640AF59_F464_4647_AB5B_6342980F4E34__INCLUDED_)
