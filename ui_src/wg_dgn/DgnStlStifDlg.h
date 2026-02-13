#if !defined(AFX_DGNSTLSTIFDLG_H__B2B06621_7F16_11D4_8895_0000C0F30D4D__INCLUDED_)
#define AFX_DGNSTLSTIFDLG_H__B2B06621_7F16_11D4_8895_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlStifDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\SpinBtnExCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlStifDlg dialog

class CDgnStlStifDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlStifDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnStlStifDlg)
	enum { IDD = IDD_DGN_STL_STIF_DLG };
	CFormulaEditSpin	m_WSpin;
	CFormulaEditSpin	m_FSpin;
	CEditUnit	m_Width;
	CEditUnit	m_Thick;
	CEditUnit	m_Space;
	CEditUnit	m_B;
	CEditUnit	m_Cw;
	CEditUnit	m_H;
	CEditUnit	m_Tf2;
	CEditUnit	m_Tf;	
	CEditUnit	m_Tw;
	CTextUnit	m_TwUnit;
	CTextUnit	m_Tf2Unit;
	CTextUnit	m_TfUnit;
	CTextUnit	m_HUnit;
	CTextUnit	m_CwUnit;
	CTextUnit	m_BUnit;
	CTextUnit	m_WUnit;
	CTextUnit	m_TUnit;
	CTextUnit	m_SpaceUnit;
	MComboBox	m_PropNo;
	int		m_nOption;
	//}}AFX_DATA

public:
	BOOL m_bUpdate;
	int m_nFSpinNo,m_nWSpinNo,m_ProNo;
	CMap<int,int,int,int> m_PropData;

public:
	void Initial_PropCombo();
	void Initial_Unit();
	void Initial_Data();
	void Initial_ExistData(T_STFN_D Data);
	void Initial_WithoutData();
	void Initial_SelectItem();

	void Selected_Element(int PropNo);
	void Change_Section4Property();
	void Define_PropertyCombo();
	BOOL ErrorCheck();

	virtual void OnUpdate(CView* pView, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	// Add by ZINU.('01.1.3).
	void Update_InitDataByCode();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlStifDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlStifDlg)
	virtual void Execute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnStiffboxSect();
	afx_msg void OnDgnStiffboxAr();
	afx_msg void OnDgnStiffboxDel();
	afx_msg void OnSelchangeDgnStiffboxPropno();
	afx_msg void OnDeltaposDgnStiffboxWspin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposDgnStiffboxFspin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLSTIFDLG_H__B2B06621_7F16_11D4_8895_0000C0F30D4D__INCLUDED_)
