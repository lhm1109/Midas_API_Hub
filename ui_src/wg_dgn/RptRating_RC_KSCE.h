// RptRating_PSC.h: interface for the RptRating_RC_KSCEclass.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptRating_RC_KSCE_H__)
#define AFX_RptRating_RC_KSCE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DB_ST_DN_RESULT_Rating.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_dgn\Dgn_DllManager_Base.h"

#include "..\dgnengine\src\DgnReportBase\XLStruct.h"
#include "..\dgnengine\src\DgnBase\XLProgressStruct.h"
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_Rating_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_KR\DgnExcel_KR_RCSC_XLStruct.h"
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_KR_RCSC_BeamStruct.h"

#include "CRCDataCtrl.h"
#include "HeaderPre.h"

// (CString, CString, CString, D_XL_PROGRESS_INTERFACE*,  void * ,  CArray <RF_RES_RF_KSCE_RC_D, RF_RES_RF_KSCE_RC_D&> , BOOL ) // Rating Report Design Engine DLL Data Type

// Struct for Print KSCE USD 10 , KSCE Rail USD 11
struct  T_RCRatingResD_KSCE_Str_D
{
	DGNEXCEL_KR_RCSC_OPTION OptD;
	_UMD_RC_BEAM_KCI InD;
	_UMD_RC_KR_BEAM_RES ResD;

	T_RCRatingResD_KSCE_Str_D(){Initailize();}
	void Initailize()
	{
		OptD.Initialize();
		InD.Initialize();
		ResD.Initialize();
	}
};

struct  T_RCRatingResD_KSCE_Str_Rpt_D
{
	DGNEXCEL_KR_RCSC_OPTION OptD;
	D_LCOM_INFO    LoadD;
	CArray<DGNEXCEL_KR_BEAM_SECT, DGNEXCEL_KR_BEAM_SECT&> arRptResD;
	CArray<_UMD_RC_BEAM_KCI, _UMD_RC_BEAM_KCI&> arStrInD;
	CArray<_UMD_RC_KR_BEAM_RES, _UMD_RC_KR_BEAM_RES&> arStrResD;

	T_RCRatingResD_KSCE_Str_Rpt_D(){Initailize();}
	void Initailize()
	{
		OptD.Initialize();
		LoadD.Initialize();
		arRptResD.RemoveAll();
		arStrInD.RemoveAll();
		arStrResD.RemoveAll();
	}
};

struct  T_RCRatingResD_KSCE_Rpt_D
{
	D_XL_PROGRESS_INTERFACE* pProgressInte;
	CString szProgramDir;
	CString szProjectDir;
	CString szSaveFileName;
	CArray<RF_RES_RF_KSCE_RC_D, RF_RES_RF_KSCE_RC_D&> aRptData;

	T_RCRatingResD_KSCE_Rpt_D(){Initailize();}
	void Initailize()
	{
		pProgressInte = NULL;
		aRptData.RemoveAll();
	}
};

class __MY_EXT_CLASS__ CRptRating_RC_KSCE : public CDgn_DllManager_Base
{
public:
	CRptRating_RC_KSCE();
	virtual ~CRptRating_RC_KSCE();

	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};
	void Start() {};


protected:
	CDBDoc* m_pDoc;
	BOOL m_bStopExecute;
	CCRCDataCtrl* m_pDataCtrl;
	CCRCForceCtrl* m_pForcCtrl;

public: 

	// KSCE USD 10 , KSCE Rail USD 11
//  BOOL Set_RC_Rating_Report_DLL();
	BOOL Set_RC_Rating_Result_Report_DLL(HINSTANCE hDllCalc);

	BOOL Excute_Print_RCRatinig_KSCE(int iDgnRatingCode, CString strPath, int iPrintOpt, BOOL bSaveImage);
	BOOL Excute_Print_RCRatinig_KSCE_PLATE(int iDgnRatingCode, CString strPath, int iPrintOpt, BOOL bSaveImage);
	int GetCodeConvert2Engine(const UINT &nCode);

	// Make Rpt Result
	BOOL Make_Beam_RptD_RCRating_KSCE      (int iDgnRatingCode, T_RCRatingResD_KSCE_Str_Rpt_D  &rStrRptD, T_RCRatingResD_KSCE_Rpt_D &rRatingRptD);
	BOOL Make_Beam_RptD_RCRating_KSCE_Plate(int iDgnRatingCode, T_RCRatingResD_KSCE_Str_Rpt_D  &rStrRptD, T_RCRatingResD_KSCE_Rpt_D &rRatingRptD);

	BOOL Print_RptD_RCRating_KSCE();
	BOOL Print_RptD_RCRating_KSCE_Strength();
	BOOL Print_RptD_RCRating_KSCE_Rating();

	CStringW GetBridgeLevel(const int &nRank);
	BOOL ConvertToDLL_RptForce(T_RATI_UNITLCOM &Force, RF_RES_KS_FORCE_KSCE_ASD_D &rForce);

	// Excel 
	CString GetSaveFileDirEx();
	CString GetSaveFileNameEx(BOOL bWithoutExtension = FALSE);

protected:
	BOOL ConvertBeamStrengthResToRpt(const T_RCRatingResD_KSCE_Str_D &crStrResD, T_RCRatingResD_KSCE_Str_Rpt_D &rStrRptD);
	BOOL ConvertBeamStrengthResToRpt_Load(const _UMD_RC_KR_BEAM_LOAD &crStrResLoadD, D_LCOM_INFO &rStrRptLoadD);
	BOOL ConvertBeamStrengthResToRpt_Sect(BOOL bPlate, int iDgnCode, T_ELEM_K ElemK, int nIJ, BOOL bPositive, const _UMD_RC_BEAM_KCI &ChkInD, const _UMD_RC_KR_BEAM_RES &ChkResD, DGNEXCEL_KR_BEAM_SECT &ExcelSect);
	BOOL ConvertToDLL_RF(T_ELEM_K ElemK, int nIJ, int nPosiNega, const T_RPAC_D &crRpacD, T_RFRC_BASE& BaseD, RF_RES_RF_KSCE_RC_D& RptD);
	
	double Calc_Beta(int iDgnCode, double dFck);
	double Calc_Alpha(int iDgnCode, double dFck);
	double Get_epsi_alw_min(double dFyr, double dEsr);
	BOOL Get_RebaLayerInfo(_UMD_RC_KR_BEAM_RBAR& rData, double* dAsLayerTop, double* dAsLayerBot);
	BOOL Get_RhoLimit_Data(int iDgnCode, double dFck, double dFyr, double decu, double& dMaxRho, double& dRhob, double& dMinRho1, double& dMinRho2, double& dMinRho, double dEsr, double dDeff, double dDtmax);
	double Get_Rho_AllowRatioRbar_Rect(int iDgnCode, double dFc, double dFyr, double decu, double dEsr, double dDeff, double dDtmax);
	double Get_RebarStrain_Rect(int iDgnCode, double decu, double dBc, double dDt, double dFc, double dFyr, double dAst);
	

};

#include "HeaderPost.h"

#endif // !defined(AFX_RptRating_RC_KSCE_H__)
