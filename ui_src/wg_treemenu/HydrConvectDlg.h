#if !defined(AFX_HYDRCONVECTDLG_H__E4CB2AC2_17E4_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_HYDRCONVECTDLG_H__E4CB2AC2_17E4_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HydrConvectDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxEtfc.h"
#include "..\wg_db\wg_db_CobxCcfc.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CHydrConvectDlg dialog

class CHydrConvectDlg : public CMenuBarChildDlg
{
// Construction
public:
	CHydrConvectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);
	void UpDateSelectOption();

public:
	void CheckMode();

// Dialog Data
	//{{AFX_DATA(CHydrConvectDlg)
	enum { IDD = IDD_TM_HYDR_CONVECT };
	CCobxCcfc	m_CobxConv;
	CCobxEtfc	m_CobxFunc;
	mit::frx::MComboBox	m_CobxFace;
	CCobxBngr	m_wndGroupCombo;
	int		m_option;
	int		m_select;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHydrConvectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_group, m_gFace, m_gNPlane, m_gSubSel;

	// Generated message map functions
	//{{AFX_MSG(CHydrConvectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmChangeOption();
	afx_msg void OnTmChangeSelect();
	afx_msg void OnTmHydrFuncBtn();
	afx_msg void OnSelchangeTmHydrFaceCombo();
	afx_msg void OnTmHydrFuncBtn1();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYDRCONVECTDLG_H__E4CB2AC2_17E4_11D5_880F_00010263A1F4__INCLUDED_)
