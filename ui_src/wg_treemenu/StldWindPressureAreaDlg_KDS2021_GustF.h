#if !defined(__TM_STLD_WIND_AREA_KDS2021_GUSTFACTOR_H__)
#define __TM_STLD_WIND_AREA_KDS2021_GUSTFACTOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindGustKDS2021Dlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KDS2021_GustF dialog

struct T_KDS2021_GUST_FACTOR_DATA
{
	int nStructureType;    // KDS2021 1:Rigid Structure, 2:Flexible Structure
	int nExposureCategory; // KDS2021 1:A, 2:B, 3:C, 4:D
	double dBasicWindSpeed;
	double dImportanceFactor;
	BOOL   bTopoEffects;  //KDS2021
	double dAverageRoofH; //KDS2021
	double dBreadth;
	double dNaturalFreq;
	double dDampingRatio;
	double dTotalMass;
	double d1stMx;
	double dVibration;
	double dKzt; //KDS2021
	double dHzt; //KDS2021
	double dKd;

	void Initialize()
	{
		nStructureType    = 1;
		nExposureCategory = 1;
		dBasicWindSpeed   = 0.0;
		dImportanceFactor = 0.0;
		bTopoEffects      = 0.0;
		dAverageRoofH     = 0.0;
		dBreadth          = 0.0;
		dNaturalFreq      = 0.0;
		dDampingRatio    = 0.0;
		dTotalMass		 = 0.0;
		d1stMx           = 0.0;
		dVibration       = 0.5;
		dKzt = 0.0;
		dHzt = 0.0;
		dKd = 1.0;
	}
	T_KDS2021_GUST_FACTOR_DATA()
	{
	}
	T_KDS2021_GUST_FACTOR_DATA& operator=(const T_KDS2021_GUST_FACTOR_DATA& src)
	{
		nStructureType    = src.nStructureType;
		nExposureCategory = src.nExposureCategory;
		dBasicWindSpeed   = src.dBasicWindSpeed;
		dImportanceFactor = src.dImportanceFactor;
		bTopoEffects      = src.bTopoEffects;
		dAverageRoofH     = src.dAverageRoofH;
		dBreadth          = src.dBreadth;
		dNaturalFreq      = src.dNaturalFreq;
		dDampingRatio     = src.dDampingRatio;
		dTotalMass		  = src.dTotalMass;
		d1stMx			  = src.d1stMx;
		dVibration		  = src.dVibration;

		dKzt = src.dKzt;
		dHzt = src.dHzt;
	}
};

class CStldWindPressureAreaDlg_KDS2021_GustF : public CDialogMove
{
// Construction
public:
	CStldWindPressureAreaDlg_KDS2021_GustF(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_TM_STLD_WINDP_DLG_AREA_KDS2021_GUSTF };

// Attribute
public:
	BOOL SetWindData(const T_WVEP_KDS2021& WvepD, T_WDPR_D* pData);
	void GetDlgInfo(T_WDPR_KDS2021& KDS2021);

public:
	T_KDS2021_GUST_FACTOR_DATA m_Data;  
	double m_dGustFactor; // T_KDS2021_GUST_FACTOR_DATA를 이용하여 구한값.

	BOOL m_bKBC2016;

	CEdit     m_edtExposureCate;
	CEditUnit	m_edtBasicWindSpeed; // m/sec
// 	CTextUnit	m_stcBasicWindSpeed; // m/sec
	CEditUnit	m_edtImportanceFactor;
	CEdit     m_edtTopoEffects;
	CEditUnit	m_edtDirectionalFactor;
	CEditUnit	m_edtAverageRoofH; // m
	CTextUnit	m_stcAverageRoofH; // m
	CEditUnit	m_edtBreadth;      // m
	CTextUnit	m_stcBreadth;      // m
	CEditUnit	m_edtNaturalFreq;  // Hz
	CTextUnit	m_stcNaturalFreq;  // Hz
	CEditUnit	m_edtDampingRatio;
	CEditUnit	m_edtGustFactor;  

	CEditUnit	m_edtTotalMass;  // Kg/m
	CTextUnit	m_stcTotalMass;  // Kg/m
	CEditUnit	m_edt1stMx;  // Kg/m
	CTextUnit	m_stc1stMx;  // Kg/m
	CEditUnit	m_editVibration;  

protected:
	void InitControl();
	void ShowHideControls();
	void DisplayGustFactor();
	BOOL Dlg2Data();
	BOOL Data2Dlg();

// 	double GetrfValue();
// 	double GetBfValue  ();
// 	double GetRfValue  ();
// 	double GetgfValue  ();
// 	void   GetKztFactor(double& dKzt);
// 	void   GetrfFactor (double& dAlpha, double& dZb, double& dZg);
// 	double GetBfFactor ();
// 	double GetSfValue  ();
// 	double GetFValue   ();
// 	double GetVhValue  ();
// 	void   GetKzrFactor(double& Kzr_low, double& Kzr_high);
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnCmdRdoChanged();
	afx_msg void OnCmdWindgustCal();
	afx_msg void OnCmdWindgustOk();
	afx_msg void OnCmdWindgustCancel();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDGUSTKDS2021DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
