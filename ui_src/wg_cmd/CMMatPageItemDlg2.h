//
// Old Version 
// See CMMatPageItemDlg.h
//
#if !defined(AFX_CMMATPAGEITEMDLG2_H__6D164E03_070B_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMMATPAGEITEMDLG2_H__6D164E03_070B_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMatPageItemDlg2.h : header file
//

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMatPageItemDlg2 dialog

class CCMMatPageItemDlg2 : public CDialog
{
// Construction
public:
	CCMMatPageItemDlg2(CDBDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Attributes
public:

// Operations
public:
	void SetModifyData(T_MATL_K key, T_MATL_D& data);

// Dialog Data
	//{{AFX_DATA(CCMMatPageItemDlg2)
	enum { IDD = IDD_CMD_MP_ITEM_DLG2 };
	CTextUnit	m_wndUnitT;
	CTextUnit	m_wndUnitP;
	CTextUnit	m_wndUnitD;
	CTextUnit	m_wndUnitE;
	CEditUnit	m_wndSteelT;
	CEditUnit	m_wndSteelP;
	CEditUnit	m_wndSteelD;
	CEditUnit	m_wndSteelE;
	CCobxAutoFill	m_wndSteelName;
	CComboBox	m_wndSteelCode;
	CEdit	m_wndName;
	CFormulaEdit m_wndID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMatPageItemDlg2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InitDlgCtrls();
	void SetUnitType();
	void ShowDataToDlg();
	void SetNameCombo(CComboBox* pCombo, CString &csType, CString &csCode);

	void GetSteelData(CString csCode, CString csName, T_MATL_ANALYSIS &AnalData);
	void SetAnalData(int nTypeIndex, T_MATL_ANALYSIS &AnalData);
	BOOL GetAnalData(int nTypeIndex, T_MATL_ANALYSIS &AnalData);

	BOOL Dlg2Data();

protected:
	CDBDoc*   m_pDoc;
	BOOL      m_bModify;
	T_MATL_K  m_Key, m_OldKey;
	T_MATL_D  m_Data;

	CArray<UINT, UINT> m_aCtrlSteelAnal;

	// Generated message map functions
	//{{AFX_MSG(CCMMatPageItemDlg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeSteelName();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnChangeName();
	afx_msg void OnChangeID();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMATPAGEITEMDLG2_H__6D164E03_070B_11D4_92DE_0000C0B0E6B3__INCLUDED_)
