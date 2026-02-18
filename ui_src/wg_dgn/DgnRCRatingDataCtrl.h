// DgnSteelRatingDlg.h: interface for the CDgnSteelRatingDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnRCRatingDataCtrl_H__)
#define AFX_DgnRCRatingDataCtrl_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"
#include "..\wg_base\MSExcel.h"


// Design Engine
#include "..\dgnengine\src\DgnBase\DgnBaseUnit_Struct.h"
#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_RC_BeamStruct.h"
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_KR_RCSC_BeamStruct.h"

class CDgn_RatingManager;
class CDgnDataCtrl;

typedef BOOL (PDGNCALC7)(void*, void*, void*, void*, void*, void*, void*);  // for KSCE_USD10_RC_RATING & KSCE_RAIL_USD11_RC_RATING

class CDgnRCRatingDataCtrl  : public CDgnPscCommon
{
	// Member Function
public:
	// Common
	CDgnRCRatingDataCtrl();
	virtual ~CDgnRCRatingDataCtrl();

	virtual void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);
	void SetDataCtrlPointer4Rating(int nCodeRC, CCRCDataCtrl* pDataCtrl);

public:
	BOOL InitialData(int nDgnCode);

	BOOL MakeRCRatingDesign();
	BOOL ConvertRCRatingInputData();
	BOOL ConvertRCRatingResultData();
	BOOL ExcuteRCRatingDesign(int nType);

	// RC Beam
	BOOL MakeRCRatingBeamDesign();
	BOOL ConvertRCRatingBeamInputData(T_ELEM_K ElemK, _UMD_RC_BEAM_KCI &rData, int nIJ = 0);
	BOOL ConvertRCRatingBeamResultData();
	BOOL ExcuteRCBeamRatingDesign();

	// RC Beam Report
	BOOL Get_RCRatingBeamStrength_Print(_UMD_RC_BEAM_KCI &InData4Rpt,  _UMD_RC_KR_BEAM_RES &DgnEngineResD);
	void Set_CMSExcel_KSCE_USD(CMSExcel* pXL, CString strBaseBook, int nCodeRC);

	// RC Plate
	BOOL MakeRCRatingPlateDesign();
	BOOL ConvertRCRatingPlateInputData(T_RRPL_D &RrplD, _UMD_RC_BEAM_KCI &rData);
	BOOL ConvertRCRatingPlateResultData();
	BOOL ExcuteRCPlateRatingDesign();
	BOOL Set_InputDataByLcom(T_MEMB_K MemK, int iLcomNo);
	BOOL Make_FORCE_RCASE_Plate(T_RRPL_D &RrplD, BOOL bAllDL,  FORCE_RCASE_KR &ForceRcase);

	// Common
	BOOL ConvertDesignCode(_UMD_RC_BEAM_KCI &rData);
	BOOL ConvertCivilDgnCode2DgnCodeStr(int nCivilDgnCode, CString &strDgnCode, int &nEngineDgnCode);

	// RC Beam
	BOOL Get_RCRatingBeamOptionD(_UMD_RC_KR_BEAM_OPTION &DgnOptD);

	// SectD
	BOOL Get_RCRatingBeamSectD(T_ELEM_K ElemK, _UMD_RC_KR_BEAM_SECT &DgnSectD);
	BOOL ConvertRCRatingBeamSectD(const T_SECT_D &SectD, _UMD_RC_KR_BEAM_SECT &DgnSectD); // Get SectD 
	BOOL Get_RCRatingPlateSectD(T_RRPL_D &RrplD, _UMD_RC_KR_BEAM_SECT &DgnSectD);

	// RebarD
	BOOL Get_RCRatingBeamRebarD(T_ELEM_K ElemK, const _UMD_RC_KR_BEAM_SECT &DgnSectD, _UMD_RC_KR_BEAM_RBAR &DgnRebarD, int nPosIJ = 0);
	BOOL ConvertRCRatingBeamRebarD(const T_RCHK_D &RebarD, const _UMD_RC_KR_BEAM_SECT &DgnSectD,  _UMD_RC_KR_BEAM_RBAR &DgnRebarD, int nPosIJ = 0); // Get Rebar Placing Data
	BOOL Get_RCRatingPlateRebarD(T_RRPL_D &RrplD, const _UMD_RC_KR_BEAM_SECT &DgnSectD, _UMD_RC_KR_BEAM_RBAR &DgnRebarD);

	// MatlD
	BOOL Get_RCRatingBeamMatlD(T_ELEM_K ElemK, _UMD_RC_KR_BEAM_MATL &DgnMatlD);
	BOOL ConvertRCRatingBeamMatlD(const T_SECT_D &SectD, _UMD_RC_KR_BEAM_SECT &DgnSectD);  // Get Material Data
	BOOL Get_RCRatingPlateMatlD(T_RRPL_D &RrplD, _UMD_RC_KR_BEAM_MATL &DgnMatlD);

	BOOL Get_RCRatingBeamLcomD(const FORCE_RCASE_KR &DBForceD, _UMD_RC_KR_BEAM_LOAD &Force4DgnEngine, int nPosIJ = 0, BOOL bUseOnlyML=FALSE);
	BOOL Get_RCRatingBeamLcomD(const RATING_FORCE &DBForceD, _UMD_RC_KR_BEAM_LOAD &Force4DgnEngine);
	BOOL ConvertForce2Engine(FORCE_RCASE_KR &Force, RATING_FORCE &rData, const int &nIJ );
//
	BOOL GetRatingRank(BOOL bPositiveM, const double &dMu, const double &dMn, const double &dP0a, double &dRatMuMn, int &nSafetyRank);
	BOOL Calc_RatingResult(int nSurveyMeth, double dpCapacity,T_RFRC_BASE& RfrcBaseD);

// RC Beam Sub
	void Print_Rating_Title(CString strTitle, CMSExcel* pXL);
	void Print_ElementIJ_KSCE_USD(T_ELEM_K ElemK, BOOL bPosiI, BOOL bPositive, CMSExcel* pXL);

	// RC Plate

	protected:


private:
	// Common
	CString GetMaxMinLcomType(const int &iLcomType);

	// RC Beam


	// RC Plate


	// Member Variables
public:
	// Common
	CDgn_RatingManager *m_pRatingManager;  
	CDgnDataCtrl*	m_pDgnDataCtrl;

	// RC Beam


	// RC Plate


protected:
	// Common
		int m_nDgnCode;
		int m_nMembType; // 0 : Beam, 1 : Plate

		// For Excel Report
		CString m_strBaseBook;
		CMSExcel* m_pXL; 

	// RC Beam
		PDGNCALC7 *m_pDgnCalc; // for KSCE_USD10_RC_RATING & KSCE_RAIL_USD11_RC_RATING

	// RC Plate



private:
	// Common


	// RC Beam




	// RC Plate


};




#endif // !defined(AFX_DgnRCRatingDataCtrl_H__)
