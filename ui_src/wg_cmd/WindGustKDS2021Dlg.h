#if !defined(AFX_WINDGUSTKDS2021DLG_H__INCLUDED_)
#define AFX_WINDGUSTKDS2021DLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindGustKDS2021Dlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\Wind_KDS41_10_15_2021.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CWindGustKDS2021Dlg dialog

class CWindGustKDS2021Dlg : public CDialogMove
{
// Construction
public:
	CWindGustKDS2021Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWindGustKDS2021Dlg)
	enum { IDD = IDD_ETC_WIND_GUST_KDS2021_DLG	};
	CTextUnit	m_wndBreadthYUnit;
	CTextUnit	m_wndBreadthXUnit;
	CTextUnit	m_wndBldgHeightUnit;
	CEditUnit	m_wndKor2000If;
	CEditUnit	m_wndKor2000Bws;
 	CEditUnit	m_wndTopography;
	CEditUnit	m_wndKdx;
	CEditUnit	m_wndKdy;
	CEditUnit	m_wndKor2000ECcbo;
	CEditUnit	m_edtFrequencyY;
	CEditUnit	m_edtDampingRatio;
	CEditUnit	m_edtFrequencyX;
	CEditUnit	m_edtBreadthY;
	CEditUnit	m_edtBreadthX;
	CEditUnit	m_edtHeight;

	CEditUnit   m_edtTotalMass;
	CEditUnit   m_edtMx;
	CEditUnit   m_edtMy;
	CEditUnit   m_editVibration;
	CTextUnit	m_untTotalMass;
	CTextUnit	m_untMx;
	CTextUnit	m_untMy;
	
	//}}AFX_DATA

// Attribute
public:
	double m_dGustFactorX;
	double m_dGustFactorY;
	// BOOL m_bDirection;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindGustKDS2021Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nCodeType;
	double m_dImportance;
	double m_dHeight;
	double m_dKzt;
	double m_dHzt;
	double m_dSpeed;
	double m_dWidth;
	double m_dBreadth;	
	double m_dFrequencyX;
	double m_dFrequencyY;
	double m_dDamping;
	double m_dTotalMass;
	double m_dMx;
	double m_dMy;
	double m_dVibration;
	int		 m_nExposure;
	int    m_nRigidity;
	BOOL   m_bTopography;
	BOOL   m_bDirection;
	double m_dKdx;
	double m_dKdy;

	double m_dStoryHMax;
	CArray<double, double&> m_StoryHeight;
	CArray<double, double&> m_StoryMass;

protected:
	void InitialData();

	// Generated message map functions
	//{{AFX_MSG(CWindGustKDS2021Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdWindgustCal();
	// afx_msg void OnCmdWindgustGustRdo();
	afx_msg void OnCmdWindgustOk();
	afx_msg void OnCmdWindgustCancel();

	afx_msg void OnChangeVibration();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDGUSTKDS2021DLG_H__INCLUDED_)
