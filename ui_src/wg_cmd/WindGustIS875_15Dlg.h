#if !defined(AFX_WindGustIS875_15Dlg_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
#define AFX_WindGustIS875_15Dlg_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindGustIS875_15Dlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CWindGustIS875_15Dlg dialog

class CWindGustIS875_15Dlg : public CDialogMove
{
// Construction
public:
	CWindGustIS875_15Dlg(int nCodeType=16, CWnd* pParent = NULL);   // standard constructor
	void SetData2Dlg(T_WIND_IS875_2015& data, BOOL bModify);
	BOOL SetDlg2Data(T_WIND_IS875_2015& data);
	int    iStrKind;
	double dBeta;
	double dAlongWind;
	double dAcrossWind;
	int   iFactk;
	double dCfs;
	double dMc;
	CString m_GustFactor;
	BOOL bIsUSerDefGustFact;
// Dialog Data
	//{{AFX_DATA(CWindGustIS875_15Dlg)
	enum { IDD = IDD_ETC_WIND_GUST_IS875_DLG };	
	MComboBox	m_CboStruKind;
	MComboBox	m_CboFactK;
	MComboBox	m_CboFactMc;
	CEditUnit	m_editbeta;
	CEditUnit	m_editalong;
	CEditUnit	m_editacross;
	CEditUnit	m_editcfs;
	MButton	    m_chkUsrDefGustFact;
	

	
	// int				m_GustFactorDirction;
	//}}AFX_DATA

// Attribute
public:
	double m_dGustFactorX;
	double m_dGustFactorY;
	double m_dCFS;
	// BOOL m_bDirection;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindGustIS875_15Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	double m_dHXCH2019;
	double m_dHYCH2019;
	double m_dBXCH2019;
	double m_dBYCH2019;	
	int m_nNXCH2019;
	int m_nNYCH2019;	
	int m_nMtdXCH2019;
	int m_nMtdYCH2019;
	BOOL m_bInit;
	BOOL m_bModify;
	T_WIND_IS875_2015  m_Data;
// Implementation
public:
	

protected:
	void InitDefaultData();
	void InitStrCategoryCombo();
	void InitFactKCombo();
	void InitFactMCCombo();
	void InitBldgPeriod();
	// Generated message map functions
	//{{AFX_MSG(CWindGustIS875_15Dlg)
	virtual BOOL OnInitDialog();
	BOOL Dlg2Data();
	// afx_msg void OnCmdWindgustGustRdo();
	afx_msg void OnCmdWindgustOk();
	afx_msg void OnCmdWindgustCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeEtcWindGustIs875Strcatcombo();
	afx_msg void OnCbnSelchangeEtcWindGustIs875ComboFactk();
	afx_msg void OnCbnSelchangeEtcWindGustIs875ComboFactkmc();
	afx_msg void OnBnClickedCmdWindgustOk();
	afx_msg void OnBnClickedCmdWindgustCancel();
	afx_msg void OnBnClickedEtcWindGustIs875Butautocalc();
	afx_msg void OnBnClickedEtcWindGustIs875Userdef();
	afx_msg void OnBnClickedEtcWindGustIs875ChkUserdefGf();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WindGustIS875_15Dlg_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
