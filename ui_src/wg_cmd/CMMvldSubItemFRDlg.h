#if !defined(AFX_CMMVLDSUBITEMFRDLG_H__INCLUDED_)
#define AFX_CMMVLDSUBITEMFRDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldSubItemFRDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CCMMvldItemFRDlg;
/////////////////////////////////////////////////////////////////////////////
// CCMMvldSubItemFRDlg dialog

class CCMMvldSubItemFRDlg : public CDialogMove
{
// Construction
public:
	CCMMvldSubItemFRDlg(CWnd* pParent = NULL);   // standard constructor
	CCMMvldItemFRDlg* m_pMvldItemFRDlg;
//----------------------------------------------------------------------
// Interface
public:
	void SetParamData(T_MVLDfr_BASE &ParamData, int nPos) 
	{ m_Data = ParamData; m_nPos = nPos; m_bModify = TRUE; }

public:
	T_MVLDfr_BASE   m_Data;
	int             m_nPos;
	int             m_nLoadModel; //0:Load system A  1:Load system Bc  2:Load system Bt  3: Military Load  4:Pedestrian
	CArray<UINT, UINT> m_aCenfEffType;

//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL ErrorCheck();
	BOOL ApplyOrOK();

	void AlignControl();
	void ResizeDialog();
	void ShowHIdeCtrl();
	void InitCombo();
	CString GetLaneName(UINT nLaneKey);
	int     GetLaneType(UINT nLaneKey);

protected:
	CDBDoc*       m_pDoc;
	BOOL          m_bModify;
	BOOL          m_bSurface;

	CArray<UINT, UINT> m_aVehLaneList;
	CArray<BOOL, BOOL> m_aSelFlag;
	CArray<BOOL, BOOL> m_aUnSelFlag;

	CArray<UINT, UINT> m_aVehLaneList2;
	CArray<BOOL, BOOL> m_aSelFlag2;
	CArray<BOOL, BOOL> m_aUnSelFlag2;

	CArray<UINT, UINT> m_aCtrlVehicle;
	CArray<UINT, UINT> m_aCtrlCentEff;
	CArray<UINT, UINT> m_aCtrlSidewalk;
	CArray<UINT, UINT> m_aCtrlOkNg;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldSubItemFRDlg)
	enum { IDD = IDD_CMD_ML_MVLD_SITEM_FRANCE };
	CListBox	m_wndUnselList;  // Vehicle
	CListBox	m_wndSelList;
	MComboBox	m_wndVeh;

	CListBox	m_wndUnselList2; // Sidewalk
	CListBox	m_wndSelList2;
	MComboBox	m_wndVeh2;

	//CComboBox	m_wndVehBc;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldSubItemFRDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvldSubItemFRDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnDblclkCmdSelList();
	afx_msg void OnDblclkCmdUnselList();
	afx_msg void OnCmbVeh();

	afx_msg void OnCmdBtnAdd2();
	afx_msg void OnCmdBtnDelete2();
	afx_msg void OnDblclkCmdSelList2();
	afx_msg void OnDblclkCmdUnselList2();
	afx_msg void OnCmbVeh2();

	//afx_msg void OnCmdLoadEffRdo();

	//afx_msg void OnCmbVehBc();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDSUBITEMFRDLG_H__INCLUDED_)
