#if !defined(AFX_CMMVLDSUBITEMDLG_H__8A601086_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMMVLDSUBITEMDLG_H__8A601086_2359_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldSubItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MEdit.h"

using namespace mit::frx;

class CCMMvldItemDlg;
//class CCMMvldItemEuroBSDlg;
class CCMMvldItemEuroBSNewDlg;
class CCMMvldItemRusDlg;
class CCMMvldItemKRLRFD11Dlg;
class CCMMvldItemSouthAfricaDlg;
class CCMMvldItemPolandDlg;
class CCMMvldItemAustraliaDlg;
class CCMMvldItemNewZealandDlg;
class CCMMvldItemBrazilDlg;
/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemDlg dialog

class CCMMvldSubItemDlg : public CDialogMove
{
// Construction
public:
	CCMMvldSubItemDlg(CWnd* pParent = NULL);   // standard constructor
	CCMMvldItemDlg* m_pMvldItemDlg;
	//CCMMvldItemEuroBSDlg* m_pMvldItemEuroBSDlg;
	CCMMvldItemEuroBSNewDlg* m_pMvldItemEuroBSNewDlg;
	CCMMvldItemRusDlg* m_pMvldItemRusDlg;
	CCMMvldItemKRLRFD11Dlg* m_pMvldItemKRLRFDDlg;
	CCMMvldItemSouthAfricaDlg* m_pMvldItemSouthAfricaDlg;
	CCMMvldItemPolandDlg* m_pMvldItemPolandDlg;
	CCMMvldItemAustraliaDlg* m_pMvldItemAustraliaDlg;
	CCMMvldItemNewZealandDlg* m_pMvldItemNewZealandDlg;
	CCMMvldItemBrazilDlg* m_pMvldItemBrazilDlg;

//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLD_BASE &ParamData, int nPos) 
	{ m_Data = ParamData; m_nPos = nPos; m_bModify = TRUE; }

public:
	T_MVLD_BASE   m_Data;
	int           m_nPos;
	int           m_nLoadModel;

//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ErrorCheck();
	BOOL ApplyOrOK();
	void EnableCtrl();
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
	//{{AFX_DATA(CCMMvldSubItemDlg)
	enum { IDD = IDD_CMD_ML_MVLD_SITEM1 };
	CListBox	m_wndUnselList;
	CListBox	m_wndSelList;
	MEdit	m_wndMaxNum;
	MEdit	m_wndMinNum;
	MEdit	m_wndScaleFactor;
	MComboBox	m_wndVClass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldSubItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvldSubItemDlg)
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
