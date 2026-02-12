#if !defined(AFX_WINDGUSTKBC2009DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
#define AFX_WINDGUSTKBC2009DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindGustKBC2009Dlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CWindGustKBC2009Dlg dialog

class CWindGustKBC2009Dlg : public CDialogMove
{
// Construction
public:
	CWindGustKBC2009Dlg(int nCodeType=16, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWindGustKBC2009Dlg)
	enum { IDD = IDD_ETC_WIND_GUST_KBC2009_DLG };	
	CTextUnit	m_wndBreadthYUnit;
	CTextUnit	m_wndBreadthXUnit;
	CTextUnit	m_wndBldgHeightUnit;
	CEditUnit	m_wndKor2000If;
	CEditUnit	m_wndKor2000Bws;
 	CEditUnit	m_wndTopography;
	CEditUnit	m_wndKor2000ECcbo;
	CEditUnit	m_edtFrequencyY;
	CEditUnit	m_edtDampingRatio;
	CEditUnit	m_edtFrequencyX;
	CEditUnit	m_edtBreadthY;
	CEditUnit	m_edtBreadthX;
	CEditUnit	m_edtHeight;
	// int				m_GustFactorDirction;
	//}}AFX_DATA

// Attribute
public:
	double m_dGustFactorX;
	double m_dGustFactorY;
	// BOOL m_bDirection;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindGustKBC2009Dlg)
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
	int		 m_nExposure;
	int    m_nRigidity;
	BOOL   m_bTopography;
	BOOL   m_bDirection;

protected:
	void InitialData();
	double GetrfValue();
	double GetBfValue(BOOL bDirection);
	double GetRfValue(BOOL bDirection);
	double GetgfValue(BOOL bDirection);
	void GetKztFactor(double& dKzt);
	void GetrfFactor(double& dAlpha, double& dZb, double& dZg);
	double GetBfFactor(BOOL bDirection);
	double GetSfValue(BOOL bDirection);
	double GetFValue(BOOL bDirection);
	double GetVhValue();
	void GetKzrFactor(double& Kzr_low, double& Kzr_high);

	// Generated message map functions
	//{{AFX_MSG(CWindGustKBC2009Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdWindgustCal();
	// afx_msg void OnCmdWindgustGustRdo();
	afx_msg void OnCmdWindgustOk();
	afx_msg void OnCmdWindgustCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDGUSTKBC2009DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
