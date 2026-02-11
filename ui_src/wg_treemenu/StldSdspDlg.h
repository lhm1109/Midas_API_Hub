#if !defined(AFX_STLDSDSPDLG_H__66BE7327_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STLDSDSPDLG_H__66BE7327_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldSdspDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"
/////////////////////////////////////////////////////////////////////////////
// CStldSdspDlg dialog

class CStldSdspDlg : public CMenuBarChildDlg
{
// Construction
public:
	CStldSdspDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
// Dialog Data
	//{{AFX_DATA(CStldSdspDlg)
	enum { IDD = IDD_TM_STLD_SDSP };
	CSelectLC	m_LoadCase;
	int		m_nOption;
	CCobxLdgr m_wndLdgrCobx;
	//}}AFX_DATA
	CEditUnit m_Value[6];
	CTextUnit m_Unit[6];
	mit::frx::MButton m_Flag[6];
	mit::frx::MillustViewer m_wndPicture;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldSdspDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void EnableValueEdit(BOOL bEnable);

	// Generated message map functions
	//{{AFX_MSG(CStldSdspDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickLoadCaseBtn();
	afx_msg void OnToggleFlag();
	afx_msg void OnChangeOption();
	afx_msg void OnTmDefineLdgrBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDSDSPDLG_H__66BE7327_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
