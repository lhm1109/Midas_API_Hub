#if !defined(AFX_CMMVLDSUBITEMDLG_H__8A601086_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMMVLDSUBITEMDLG_H__8A601086_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldSubItemCHDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

class CCMMvldItemCHDlg;
/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemCHDlg dialog

class CCMMvldSubItemCHDlg : public CDialogMove
{
// Construction
public:
	CCMMvldSubItemCHDlg(CWnd* pParent = NULL);   // standard constructor
	CCMMvldItemCHDlg* m_pMvldItemDlg;

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLDch_BASE &ParamData, int nPos) 
	{ m_Data = ParamData; m_nPos = nPos; m_bModify = TRUE; }

public:
	T_MVLDch_BASE   m_Data;
	int           m_nPos;

//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ApplyOrOK();
	void FillVClassList();
	CString GetLaneName(UINT nLaneKey);

protected:
	CDBDoc*       m_pDoc;
	BOOL          m_bModify;
	BOOL          m_bSurface;

	CArray<UINT, UINT> m_aLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldSubItemCHDlg)
	enum { IDD = IDD_CMD_ML_MVLD_SITEM_CHINA };
	CListBox	m_wndUnselList;
	CListBox	m_wndSelList;
	MEdit	m_wndMaxNum;
	MEdit	m_wndMinNum;
	MEdit	m_wndScaleFactor;
	MComboBox	m_wndVClass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldSubItemCHDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvldSubItemCHDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdSelList();
	afx_msg void OnDblclkCmdUnselList();
	afx_msg void OnCmdBtnVclass();
	afx_msg void OnCmdApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDSUBITEMDLG_H__8A601086_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_)
