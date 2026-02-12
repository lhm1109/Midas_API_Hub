#if !defined(AFX_BNDRSPRSDLG_H__D14EA0E1_6C58_11D4_9AAF_0000C0B9C58C__INCLUDED_)
#define AFX_BNDRSPRSDLG_H__D14EA0E1_6C58_11D4_9AAF_0000C0B9C58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrSprsDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\MIT_frx\MButton.h"
/////////////////////////////////////////////////////////////////////////////
// CBndrSprsDlg dialog

class CBndrSprsDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CBndrSprsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CBndrSprsDlg)
	enum { IDD = IDD_TM_BNDR_SPRS };
	mit::frx::MButton	m_cboOption;
	mit::frx::MComboBox	m_cboSprsKey;
	CEditUnit	m_SDxEdit;
	CEditUnit	m_SDyEdit;
	CEditUnit	m_SDzEdit;
	CEditUnit	m_SRxEdit;
	CEditUnit	m_SRyEdit;
	CEditUnit	m_SRzEdit;
	CTextUnit	m_SRzUnit;
	CTextUnit	m_SRyUnit;
	CTextUnit	m_SRxUnit;
	CTextUnit	m_SDzUnit;
	CTextUnit	m_SDyUnit;
	CTextUnit	m_SDxUnit;
	CEditUnit m_MassEdit;
	CEditUnit m_DampEdit;
	CCobxBngr	m_wndGroupCombo;
	int		m_nOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrSprsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT,UINT> m_arSpring;
	CArray<UINT,UINT> m_arSpringUnit;
	
	T_GSPR_K m_GsprKey;
	CArray<T_GSPR_K, T_GSPR_K> m_arGsprKey;
	T_GSPR_D m_GsprData;
	CArray<T_GSPR_D, T_GSPR_D> m_arGsprData;

	T_GSTP_K m_GstpKey;
	CArray<T_GSTP_K, T_GSTP_K> m_arGstpKey;
	T_GSTP_D m_GstpData;
	CArray<T_GSTP_D, T_GSTP_D> m_arGstpData;

	CDBDoc* m_pDoc;

	void DeleteItem(T_GSTP_K Key, T_GSTP_D Data); 
	void InsertItem(T_GSTP_K Key, T_GSTP_D Data);
	void ModifyItem(CString NewTypeName, CString OldTypeName); 

	// Generated message map functions
	//{{AFX_MSG(CBndrSprsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTmSprsStypeCbo();
	afx_msg void OnTmSprsStypeBtn();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnChangeOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRSPRSDLG_H__D14EA0E1_6C58_11D4_9AAF_0000C0B9C58C__INCLUDED_)
