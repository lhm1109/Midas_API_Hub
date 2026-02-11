#if !defined(__TM_STLD_WIND_AREA_KBC2009_GUSTFACTOR_H__)
#define __TM_STLD_WIND_AREA_KBC2009_GUSTFACTOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindGustKBC2009Dlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MEdit.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2009_GustF dialog

struct T_KBC2009_GUST_FACTOR_DATA
{
	int nStructureType;    // KBC2009 1:Rigid Structure, 2:Flexible Structure
	int nExposureCategory; // KBC2009 1:A, 2:B, 3:C, 4:D
	double dBasicWindSpeed;
	double dImportanceFactor;
	BOOL   bTopoEffects;  //KBC2009
	double dAverageRoofH; //KBC2009
	double dBreadth;
	double dNaturalFreq;
	double dDampingRatio;

	double dKzt; //KBC2009
	double dHzt; //KBC2009

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
		
		dKzt = 0.0;
		dHzt = 0.0;
	}
	T_KBC2009_GUST_FACTOR_DATA()
	{
	}
	T_KBC2009_GUST_FACTOR_DATA& operator=(const T_KBC2009_GUST_FACTOR_DATA& src)
	{
		nStructureType    = src.nStructureType;
		nExposureCategory = src.nExposureCategory;
		dBasicWindSpeed   = src.dBasicWindSpeed;
		dImportanceFactor = src.dImportanceFactor;
		bTopoEffects      = src.bTopoEffects;
		dAverageRoofH     = src.dAverageRoofH;
		dBreadth          = src.dBreadth;
		dNaturalFreq      = src.dNaturalFreq;
		dDampingRatio    = src.dDampingRatio;
		
		dKzt = src.dKzt;
		dHzt = src.dHzt;
	}
};

class CStldWindPressureAreaDlg_KBC2009_GustF : public CDialogMove
{
// Construction
public:
	CStldWindPressureAreaDlg_KBC2009_GustF(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_TM_STLD_WINDP_DLG_AREA_KBC2009_GUSTF };

// Attribute
public:
	BOOL SetWindData(const T_WVEP_KBC2016& WvepD, T_WDPR_D* pData);
	BOOL SetWindData(const T_WVEP_KBC2009& WvepD, T_WDPR_D* pData);
	void GetDlgInfo(int& nRigidity, double& dBreath, double& dNatFreq, double& dDampingRatio, double& dGustFac);

public:
	T_KBC2009_GUST_FACTOR_DATA m_Data;  
	double m_dGustFactor; // T_KBC2009_GUST_FACTOR_DATA를 이용하여 구한값.

	double m_dHeight;  //계산하는데 쓰이는 임시 변수.
	double m_dBreadth; //계산하는데 쓰이는 임시 변수.

	BOOL m_bKBC2016;

	MEdit     m_edtExposureCate;
	CEditUnit	m_edtBasicWindSpeed; // m/sec
	//CTextUnit	m_stcBasicWindSpeed; // m/sec
	CEditUnit	m_edtImportanceFactor;
	MEdit     m_edtTopoEffects;
	CEditUnit	m_edtAverageRoofH; // m
	CTextUnit	m_stcAverageRoofH; // m
	CEditUnit	m_edtBreadth;      // m
	CTextUnit	m_stcBreadth;      // m
	CEditUnit	m_edtNaturalFreq;  // Hz
	CTextUnit	m_stcNaturalFreq;  // Hz
	CEditUnit	m_edtDampingRatio;
	CEditUnit	m_edtGustFactor;  

protected:
	void InitControl();
	void ShowHideControls();
	void DisplayGustFactor();
	BOOL Dlg2Data();
	BOOL Data2Dlg();

	double GetrfValue();
	double GetBfValue  ();
	double GetRfValue  ();
	double GetgfValue  ();
	void   GetKztFactor(double& dKzt);
	void   GetrfFactor (double& dAlpha, double& dZb, double& dZg);
	double GetBfFactor ();
	double GetSfValue  ();
	double GetFValue   ();
	double GetVhValue  ();
	void   GetKzrFactor(double& Kzr_low, double& Kzr_high);
	
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

#endif // !defined(AFX_WINDGUSTKBC2009DLG_H__D88A7361_8496_11D4_A678_00010263A1CE__INCLUDED_)
