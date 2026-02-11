#if !defined(__TM_STLD_WIND_AREA_ASCE7_GUSTFACTOR_H__)
#define __TM_STLD_WIND_AREA_ASCE7_GUSTFACTOR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldWindPressureAreaDlg_ASCE7_16_GustF.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MEdit.h"
using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_ASCE7_GustF dialog

struct T_ASCE7_GUST_FACTOR_DATA
{
	int nStructureType;    // ASCE7 1:Rigid Structure, 2:Flexible Structure
	int nExposureCategory; // ASCE7 1:A, 2:B, 3:C, 4:D
	double dBasicWindSpeed;
	BOOL   bTopoEffects;  //ASCE7
	double dAverageRoofH; //ASCE7
	double dBreadth;
	double dBreadth_L;
	double dNaturalFreq;
	double dDampingRatio;
	double dKzt; //ASCE7
	double dHzt; //ASCE7
	double dKd;

	void Initialize()
	{
		nStructureType    = 1;
		nExposureCategory = 1;
		dBasicWindSpeed   = 0.0;
		bTopoEffects      = 0.0;
		dAverageRoofH     = 0.0;
		dBreadth          = 0.0;
		dBreadth_L = 0.0;
		dNaturalFreq      = 0.0;
		dDampingRatio    = 0.0;
		dKzt = 0.0;
		dHzt = 0.0;
		dKd = 1.0;
	}
	T_ASCE7_GUST_FACTOR_DATA()
	{
	}
	T_ASCE7_GUST_FACTOR_DATA& operator=(const T_ASCE7_GUST_FACTOR_DATA& src)
	{
		nStructureType    = src.nStructureType;
		nExposureCategory = src.nExposureCategory;
		dBasicWindSpeed   = src.dBasicWindSpeed;
		bTopoEffects      = src.bTopoEffects;
		dAverageRoofH     = src.dAverageRoofH;
		dBreadth          = src.dBreadth;
		dNaturalFreq      = src.dNaturalFreq;
		dDampingRatio     = src.dDampingRatio;

		dKzt = src.dKzt;
		dHzt = src.dHzt;
	}
};

class CStldWindPressureAreaDlg_ASCE7_GustF : public CDialogMove
{
// Construction
public:
	CStldWindPressureAreaDlg_ASCE7_GustF(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_TM_STLD_WINDP_DLG_AREA_ASCE7_16_GUSTF	};

// Attribute
public:
	BOOL SetWindData(const T_WVEP_ASCE7_16* pASCE7, T_WDPR_D* pData);
	void GetDlgInfo(T_WDPR_ASCE7_16& ASCE7);
	double GetCalcGustFactor() { return m_dGustFactor; }
public:

	MEdit		m_edtExposureCate;
	CEditUnit	m_edtBasicWindSpeed; // mile/h
	MEdit		m_edtTopoEffects;
	CEditUnit	m_edtDirectionalFactor;
	CEditUnit	m_edtAverageRoofH; // m
	CTextUnit	m_stcAverageRoofH; // m
	CEditUnit	m_edtBreadth;      // m
	CTextUnit	m_stcBreadth;      // m
	CEditUnit	m_edtBreadth_L;      // m
	CTextUnit	m_stcBreadth_L;      // m
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

	T_ASCE7_GUST_FACTOR_DATA m_Data;
	double m_dGustFactor; // T_ASCE7_GUST_FACTOR_DATA 를 이용하여 구한값.

	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnCmdRdoChanged();
	afx_msg void OnCmdWindgustCal();
	afx_msg void OnCmdWindgustOk();
	afx_msg void OnCmdWindgustCancel();

	DECLARE_MESSAGE_MAP()

private:
	int m_nWindCode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__TM_STLD_WIND_AREA_ASCE7_GUSTFACTOR_H__)
