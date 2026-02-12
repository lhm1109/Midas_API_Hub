#if !defined(__CM_WIND_VELO_ASCE7_16_TOPOGRAPHY_H__)
#define __CM_WIND_VELO_ASCE7_16_TOPOGRAPHY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldWindPressureArea_ASCE7_16_Topography.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CMWindPressureVelo_ASCE7_16Topography dialog
class CCMWindPressureVelo_ASCE7_16Topography : public CDialogMove
{
// Construction
public:
	CCMWindPressureVelo_ASCE7_16Topography(int nCode, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMWindPressureVelo_ASCE7_16Topography)
	enum { IDD = IDD_CMD_DEF_WIND_VELOCITY_ITEM_ASCE7_16_TOPOGRAPY	};
	
	MComboBox	m_cobxHillShape_X;
	MComboBox	m_cobxBLocation_X;
	CEditUnit	m_editHHeight_X;  
	CEditUnit	m_editHillLeng_X;
	CEditUnit	m_editCBDistance_X;

	CTextUnit	m_unitHHeight;
	CTextUnit	m_unitHillLeng;
	CTextUnit	m_unitCBDistance;	
	//}}AFX_DATA

	double GetKzt() { return m_dKzt; }
	void SetData(int nExposure, double dz) { m_nExposure = nExposure; m_dZ = dz; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMWindPressureVelo_ASCE7_16Topography)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void InitControls();
	void InitHillShapeCombo();
	void InitBLocationCombo();
	void Data2Dlg();
	BOOL Dlg2Data();
	void ChangeComboWidth();

protected:
	int     m_nExposure;
	int     m_nHillShape;    // EN_ASCE7_HILL_SHAPE
	int     m_nBldgLocation; // EN_ASCE7_BLDG_LOCATION
	double  m_dHillHeight;
	double  m_dHillLength;
	double  m_dHillDistance;   // crest-building distance
	double  m_dKzt;
	double  m_dZ;

	int m_nCode;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMWindPressureVelo_ASCE7_16Topography)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeCmdAnalHillshapeCombo_X();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CM_WIND_VELO_ASCE7_16_TOPOGRAPHY_H__)
