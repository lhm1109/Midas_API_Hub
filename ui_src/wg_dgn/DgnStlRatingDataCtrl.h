// DgnStlRatingDataCtrl.h: interface for the CDgnStlRatingDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnStlRatingDataCtrl_H__INCLUDED_)
#define AFX_DgnStlRatingDataCtrl_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"
#include "Dgn_RatingManager.h"

//#include "..\wg_db\DB_ST_DN_RESULT_CSG.h"
#include "..\dgndb\src\CVL_UmdDataBase\DB_ST_SSM.H"
#include "..\dgndb\src\CVL_UmdDataBase\DB_ST_STL.H"

#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"

class CDgn_RatingManager;
class CDgnCsgDataCtrl;

//PMS:RATING BSC-20140329 구조체 위치 변경할것..
struct RATING_STRP_ELEM
{
	T_DGN_STRP_SSM_D Strp[2]; 
	
	RATING_STRP_ELEM() {}
	RATING_STRP_ELEM(const RATING_STRP_ELEM& rData) { *this = rData; }
	RATING_STRP_ELEM& operator=(const RATING_STRP_ELEM& rData)
	{
		Strp[0] = rData.Strp[0];
		Strp[1] = rData.Strp[1];
		return *this;
	}
	void Initialize()
	{
		Strp[0].Initialize();
		Strp[1].Initialize();
	}
};

struct RATING_GRPD_ELEM
{
	T_DGN_GRPD_SSM_D Grpd[2]; 
	
	RATING_GRPD_ELEM() {}
	RATING_GRPD_ELEM(const RATING_GRPD_ELEM& rData) { *this = rData; }
	RATING_GRPD_ELEM& operator=(const RATING_GRPD_ELEM& rData)
	{
		Grpd[0] = rData.Grpd[0];
		Grpd[1] = rData.Grpd[1];
		return *this;
	}
	void Initialize()
	{
		Grpd[0].Initialize();
		Grpd[1].Initialize();
	}
};

struct RATING_RROS_ELEM
{
	double dDefI;          
	double dDefJ;
	double dStrainI;
	double dStrainJ;
	double dImpactFactor;
	double dImpactFactor_J;    // 해외 에서는 Kb Factor 로도 쓰임
	double dAddReponceF_I;    // 추가응답보정계수
	double dAddReponceF_J;    // 추가응답보정계수

	RATING_RROS_ELEM() {}
	RATING_RROS_ELEM(const RATING_RROS_ELEM& rData) { *this = rData; }
	RATING_RROS_ELEM& operator=(const RATING_RROS_ELEM& rData)
	{
		dDefI    = rData.dDefI;         
		dDefJ    = rData.dDefJ;
		dStrainI = rData.dStrainI;
		dStrainJ = rData.dStrainJ;
		dImpactFactor   = rData.dImpactFactor;
		dImpactFactor_J = rData.dImpactFactor_J;
		dAddReponceF_I  = rData.dAddReponceF_I;
		dAddReponceF_J  = rData.dAddReponceF_J;
		return *this;
	}
	void Initialize()
	{
		dDefI = 0.0;        
		dDefJ = 0.0;        
		dStrainI = 0.0;        
		dStrainJ = 0.0;        
		dImpactFactor = 0.0;   
		dImpactFactor_J  = 0.0; 
		dAddReponceF_I   = 0.0; 
		dAddReponceF_J   = 0.0; 
	}
};

struct RATING_STFN_ELEM
{
	T_DGN_STFN_SSM_D Stfn[2]; 
	
	RATING_STFN_ELEM() {}
	RATING_STFN_ELEM(const RATING_STFN_ELEM& rData) { *this = rData; }
	RATING_STFN_ELEM& operator=(const RATING_STFN_ELEM& rData)
	{
		Stfn[0] = rData.Stfn[0];
		Stfn[1] = rData.Stfn[1];
		return *this;
	}
	void Initialize()
	{
		Stfn[0].Initialize();
		Stfn[1].Initialize();
	}
};

class CDgnStlRatingDataCtrl : public CDgnPscCommon
{
// Member Functions.
public:
	CDgnStlRatingDataCtrl();
	virtual ~CDgnStlRatingDataCtrl();

	virtual void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);
	//virtual void SetCodeUnit();

public:
	BOOL InitialData(int nDgnCode);
	BOOL Check_KSCE_ASD_Rating();
	BOOL Check_Rating(UMD_STL_KR_D &InD, STL_KR_UMDC_STLOPTION &OptD, STL_KR_RES_D &rData);
	BOOL Check_Rating(UMD_STL_KR_D &InD, STL_KR_UMDC_STLOPTION &OptD, S_KR_LSD_RES_D &rData);
	BOOL CalcStress(double dMLy, double dMDy, double dMGy, STL_KR_SECT &SectD, double& dFlTT, double& dFlTC, double& dFlBT, double& dFlBC, double& dFdTT, double& dFdTC, double& dFdBT, double& dFdBC, double &dFgTT, double &dFgTC, double &dFgBT, double &dFgBC);
	BOOL CalcStressCompo(double dMLy, double dMDy, double dMGy, CSG_SECT_POSD &SectD, double& dFlTT, double& dFlTC, double& dFlBT, double& dFlBC, double& dFdTT, double& dFdTC, double& dFdBT, double& dFdBC, double &dFgTT, double &dFgTC, double &dFgBT, double &dFgBC);

	BOOL Check_AASHTO_LRFR();
    BOOL Check_AASHTO_LRFR19();
    BOOL Check_CS454CSGBridgeAssessment();

protected:
	BOOL Check_KSCE_ASD(T_RPAS_D &RpasD,
											T_ELEM_K ElemK, int IJ, BOOL bCheckIJ,
											CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
											UMD_STL_KR_D &InD,
											STL_KR_UMDC_STLOPTION &InStlOpt,
											FORCE_LRKR_ELEM  &Force,
											FORCE_LRKR_ELEM  &Force_Str,
											CSG_MEMB_RES_KSCE_LSD12_D &MembRes, 
											STL_KR_RES_D &resD,
											T_RFKR_CASE &TensMinRfkrD,
											T_RFKR_CASE &CompMinRfkrD);
	
	BOOL Check_KSCE_LSD(T_RPAS_D &RpasD,
											T_ELEM_K ElemK, int IJ, BOOL bCheckIJ,
											RATING_RROS_ELEM &Rros,
											CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
											UMD_STL_KR_D &InD,
											STL_KR_UMDC_STLOPTION &InStlOpt,
											FORCE_LRKR_ELEM  &Force,
											FORCE_LRKR_ELEM  &Force_Str,
											CSG_MEMB_RESULT_D &MembRes, 
											S_KR_LSD_RES_D &resD,
											T_RFKR_CASE &TensMinRfkrD,
											T_RFKR_CASE &CompMinRfkrD);


	//Make Func
public:
	BOOL Make_RatingDgnData();
	BOOL Make_RatingMatlDgnData();
	BOOL Make_RatingSectDgnData();
	BOOL Make_UstlSMStrpData();   
	BOOL Make_UstlSMGrpdData();   
	BOOL Make_UstlSMStfnData();   
	BOOL Make_RatingProsData();

	// Etc
	int Get_ShapeNoBySectType(CString strSectType);
	BOOL Get_UnbracedLengthData(ElemPairK ElemK, BOOL bKfac, T_KFAC_D &KFacD, STL_KR_MEMBEROPTION &rData);

	BOOL Get_RatingStrengthLCaseForce(int iIJ, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, T_RLCS_D &RlcsD, _DGN_FORC_CRC &ForceNC, FORCE_BLRD_RCASE &ForceRcase, T_RCST_BASE &RcstB);
	BOOL Get_RatingStrengthData(int iIJ, int nConcurrent, T_RLCS_D &RlcsD, FORCE_BLRD_RCASE &ForceRcase, T_RCST_BASE &RcstB);  

	BOOL Get_RatingServiceLCaseForce(int iIJ, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, T_RLCS_D &RlcsD, FORCE_BLRD_RCASE &ForceRcase, T_RCSS_BASE &RcstB);
	BOOL Get_RatingServiceData(int iIJ, int nConcurrent, T_RLCS_D &RlcsD, FORCE_BLRD_RCASE &ForceRcase, T_RCSS_BASE &RcstB);  

	BOOL Get_RatingFatigueLCaseForce(int iIJ, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, T_RLCS_D &RlcsD, FORCE_BLRD_RCASE &ForceRcase, T_RCFA_BASE &RcfaB);
	BOOL Get_RatingFatigueData(int iIJ, int nConcurrent, T_RLCS_D &RlcsD, FORCE_BLRD_RCASE &ForceRcase, T_RCFA_BASE &RcfaB);  
	
	BOOL Get_LoadCaseMaxMinId(BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int &nI,int &nj, int &nk);
	void SetRcaseStrengthResult(ElemPairK CheckElemK, UINT nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDCmax, BOOL bDWmax, BOOL bTmax,
															UINT nConCurr, BOOL bSpanInfo, T_SPAN_K SpanKey, UINT nSpanPos, double dRel_Locat, double dPhi_CF, double dPhi_SF,
															T_RCST_BASE &RcstB);
	void SetRcaseServiceResult(ElemPairK CheckElemK, UINT nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDCmax, BOOL bDWmax, BOOL bTmax,
														 UINT nConCurr, BOOL bSpanInfo, T_SPAN_K SpanKey, UINT nSpanPos, double dRel_Locat, double dPhi_CF, double dPhi_SF,
														 T_RCSS_BASE &RcssB);
	void SetRcaseFatiResult(ElemPairK CheckElemK, UINT nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDCmax, BOOL bDWmax, BOOL bTmax,
													UINT nConCurr, BOOL bSpanInfo, T_SPAN_K SpanKey, UINT nSpanPos, double dRel_Locat, double dPhi_CF, double dPhi_SF,
												 T_RCFA_BASE &RcfaB);

	BOOL MakeReactionRCase(const T_ALCS_D& AlcsD, ElemPairK EPairK, int nPosIJ, int nMaxMinType,
		T_REAC_D& DLRe, T_REAC_D& SLLRe, T_REAC_D& DLLRe, CSG_MEMB_POSD_KSCE_LSD& MembLcomD);

	BOOL ConvertToWgUMD_Polygon(T_GSEC_POLYGON& InData, DGN_GSEC_POLYGON& OutData);
	BOOL ConvertToWgUMD_GsecLdgn(T_GSEC_LDGN& LdgnD, STL_KR_GSEC_LDGN& rData);
	BOOL ConvertToWgUMD_GsecShape(T_GSEC_LSHAP& LshapeD, STL_KR_GSEC_LSHAP& rData);
	BOOL ConvertToWgUMD_GsecLine(T_GSEC_LINE& LineD, DGN_GSEC_LINE& rData);
	BOOL ConvertForce2Engine(FORCE_LRKR_ELEM &Force, STL_KR_LOAD &rForce, const int &nIJ);
	BOOL ConvertCsgForce2Engine(ElemPairK ElemK, FORCE_LRKR_ELEM &Force, CSG_MEMB_POSD_KSCE_LSD &rMembLcomD, const int &nIJ);
	BOOL ConvertForce2EnginePRT(_DGN_FORC_CRC &Force, STL_KR_UNITLCOM &rData, const int &nIJ);
	void ConvertRatForceCvlToEng(T_RATI_UNITLCOM &For, STL_KR_UNITLCOM &rData);

	void AddForceData(const _DGN_FORC_CRC& Force, _DGN_FORC_CRC& rData);

	BOOL IsCompoElem(T_ELEM_K ElemK);

	int GetConvertStlRatingCodeCvlToEng(int nDgnCode);
	int GetConvertStlRatingCodeEngToCvl(int nDgnCode);

	void ConvertRatingGlobalData(T_RPAS_D &RpasD, S_CSGD_D &rData);
    void ConvertRatingGlobalData(T_RPSS_D &RpssD, S_CSGD_D &rData);

protected:

	int m_nDgnCode;

	CMap<T_ELEM_K,T_ELEM_K,T_ELEM_K,T_ELEM_K&> m_mCompoElemK; 

	T_RPAS_D *m_pRpasD;

	BOOL m_bbAppAFK;
	BOOL m_bAutoAFK;
	
	double m_dEpsiC;
	double m_dEpsiT;
	double m_dDispC;
	double m_dDispT;
	double m_dKa;
	double m_dKb;
	double m_dK;

	double m_dWeight;

    BOOL CheckStrengthRatingcaseAASHTO12(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
                                                    CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                                    CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
                                                    T_RLCS_D &RlcsD, double dCapacityF, double dCapacityFsh, double dVuFactor, T_RCST_BASE &RcstB,
                                                    double dPositive_RF[2], double dNegative_RF[2], double dPositive_CADE[2], double dNegative_CADE[2], T_RCFC_D &RcfcD,
                                                    double dVn_RF[2], double dVn_CADE[2], T_RCSC_D &RcscD);


	BOOL CheckServicethRatingcaseAASHTO12(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
													 CSG_MEMB_POSD_KSCE_LSD &MembLcomD, 
													 CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
													 T_RLCS_D &RlcsD, double dCapacityF,
													 T_RCSS_BASE &RcssB,
													 double &dCompI_RF, double &dTensI_RF, double &dCompJ_RF, double &dTensJ_RF,  
													 double &dCompI_CADE,  double &dCompJ_CADE,  double &dTensI_CADE,  double &dTensJ_CADE,
													 T_RCGS_D &RcgsD);

	BOOL CheckFatigueRatingcaseAASHTO12(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
												 CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
												 T_RLCS_D &RlcsD, double dCapacityF, double dCapaN,
												 double dStrDelMV_PR[2], double dStrDelMV_AD[2],
												 T_RCFA_BASE &RcfaB,
												 double &dTopI_RF, double &dBotI_RF, double &dTopJ_RF, double &dBotJ_RF,  
												 double &dFTopI_CADE,  double &dFBotI_CADE,  double &dFTopJ_CADE,    double &dFBotJ_CADE,
												 T_RCFA_D &RcfaD);

    BOOL CheckInfiniteFatigueRatingAASHTO(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
                                                CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
                                                T_RLCS_D &RlcsD, double dCapacityF, double dCapaN[4],
                                                double dStrDelMV_PR[4], double dStrDelMV_A4[2],
                                                T_RCFA_INF_BASE &RcfaB,
                                                double dStressI_RF[4], double dStressJ_RF[4], double dStressI_CADE[4], double dStressJ_CADE[4],
                                                T_RCFA_INF_LIFE_D &RcfaD);

    BOOL CheckFiniteFatigueRatingAASHTO(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
                                                CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
                                                T_RLCS_D &RlcsD, double dCapacityF, double dCapaN[4],
                                                double dStrDelMV_PR[4], double dStrDelMV_A4[2],
                                                T_RCFA_FIN_BASE &RcfaB,
                                                double dStressI_RF[4], double dStressJ_RF[4], double dStressI_CADE[4], double dStressJ_CADE[4],
                                                T_RCFA_FIN_LIFE_D &RcfaD);

    BOOL CheckFiniteFatigueRatingForEach(ElemPairK ElemK, int nPos, T_GRUP_K CheckGroupK, T_RLCS_K RlcsK, BOOL bDC_Max, BOOL bDW_Max, BOOL bTemp_Max, int nConcurr,
                                                CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                                T_RLCS_D &RlcsD, double dCapacityF, double dCapaN[4],
                                                double dStrDelMV_PR[4], double dStrDelMV_A4[2],
                                                T_RCFA_FIN_BASE &RcfaB,
                                                double dStressI_RF[4], double dStressJ_RF[4], double dStressI_CADE[4], double dStressJ_CADE[4],
                                                T_RCFA_FIN_LIFE_D &RcfaD, int iLifeType, T_RATING_FIN_FATI_LIFE &FinLife);


    double CalcDelFmax(int iFatiType, int iTruckType, double dDelfeff);

    void GetFatigueParamINF(T_ELEM_K ElemK, int nPos, T_RCFA_INF_BASE &RcfaD);
    void GetFatigueParamFIN(T_ELEM_K ElemK, int nPos, T_RCFA_FIN_BASE &RcfaD);


    double CalcRpFact(double dSpanLength, double dADTT_P, double dNumberLane);

    void Convert_Finite_Fatigue_Value(T_RPAS_D RpasD, T_RCFA_FIN_BASE &RcfaB);

	BOOL Check_RFminbyRatingCase();

    BOOL Check_RFminbyRatingCase19();

	double CalcTensionRF(double dCa, double dDe, double dMV);
	double CalcCompressionRF(double dCa, double dDe, double dMV);


    // CS454
    virtual BOOL CheckStrengthRCaseAssessment(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMB_RESULT_D& MembResD, CSG_CHECK_RES_CS457& ResD,
        T_ASSM_ULS_CSG_BS_BASE& UlsB);
    virtual BOOL CheckServiceRCaseAssessment(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMB_RESULT_D& MembResD, CSG_CHECK_RES_CS457& ResD,
        T_ASSL_CSG_BS_BASE& AsslB);
    virtual BOOL CheckShearConnectorRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD,
        T_ASLS_CSG_BS_BASE& AslsB);
    //
    BOOL CheckFlexuralStrengthRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);
    BOOL CheckShearStrengthRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);
    BOOL CheckCombinedMomentShearRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);
    BOOL CheckTorsionalStrengthRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);
    BOOL CheckLongitudinalShearRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);
    BOOL CheckFlangeWithLStiffenerBoxShapeRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);
    BOOL CheckWebWithLStiffenerBoxShapeRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);
    BOOL CheckSlabWithLStiffenerBoxShapeRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);


	// KSCE-LSD15
	BOOL CheckStrengthRcaseLSD15(ElemPairK ElemK,
															 T_RPAS_D &RpasD,    
															 RATING_RROS_ELEM &Rros,
															 int nPos, BOOL bCurved,
															 CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
															 UMD_STL_KR_D &InD,
															 CSG_MEMB_RESULT_D &MembRes,
															 T_RLCS_D &RlcsD,
															 FORCE_RCASE_KR  &Force,
															 FORCE_RCASE_KR  &Force_Str,
															 T_RFKR_BASE &RFB);

	BOOL CheckNonCptStrengthRcaseLSD15(ElemPairK ElemK,
																		 T_RPAS_D &RpasD,    
																		 RATING_RROS_ELEM &Rros,
																		 int nPos, BOOL bCurved,
																		 CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
																		 UMD_STL_KR_D &InD,
																		 STL_KR_UMDC_STLOPTION &InStlOpt,
																		 CSG_MEMB_RESULT_D &MembRes,
																		 S_KR_LSD_RES_D &resD,
																		 T_RLCS_D &RlcsD,
																		 FORCE_RCASE_KR  &Force,
																		 FORCE_RCASE_KR  &Force_Str,
																		 T_RFKR_BASE &RFB);

	BOOL Calc_RatingData(ElemPairK ElemK,
											 T_RPAS_D &RpasD,  
											 RATING_RROS_ELEM &Rros,
											 int nPos,                       
											 T_RFKR_BASE &RFB);

	BOOL IsSkipRatingCase(BOOL bDCmax, BOOL bDWmax, BOOL bTmax, T_RLCS_D &RlcsD);

	BOOL Calc_Stress4MVpr(ElemPairK ElemK, int nPos,
												CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMBPOS_RES_AASHTO_LRFD_D &MembResAASHTO,
												double &dEpsiCal);
	BOOL Calc_AdjustmentFactorK(double dEpsiC, double dEpsiT, double dKb, double dKuser, double &dKa, double &dK);
	BOOL Get_DeflectionOfNode(T_ELEM_K ElemK, T_RLCS_D &RlcsD, T_LCOM_D &LcomDE, double &dIdisp, double &dJdisp);
	BOOL MakeLcomData4RCase(T_RLCS_D &RlcsD, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, T_LCOM_D &LcomD);

	void ConvertRatStlCalcUlmForce(int nPosI, int nPosM, int nPosJ, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr,
																 FORCE_BLRD_RCASE &ForceRcaseI,
																 FORCE_BLRD_RCASE &ForceRcaseM,
																 FORCE_BLRD_RCASE &ForceRcaseJ,
																 CSG_ULMF_D &rData);
	void ConvertRatStlCalcChkForce(int nPos, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr, _DGN_FORC_CRC &ForceNC, FORCE_BLRD_RCASE &ForceRcase, CSG_FORCE_COMP &rData);
    void ConvertRatStlCalcChkForceEX(int nPos, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr, _DGN_FORC_CRC_EX &ForceNC, FORCE_BLRD_RCASE &ForceRcase, CSG_FORCE_COMP &rData);
	void ConvertRatStlCalcMVChkForce(int nPos, int nMvType, int nConcurr, FORCE_BLRD_RCASE &ForceRcase, CSG_FORCE_COMP &rData);
	void ConvertForceDgnToCsgDB(int nPos, _DGN_FORC_CRC &DgnFor, T_CSGD_FORCE &rData);
	void ConvertRatForceEngToCvl(STL_KR_UNITLCOM &For, T_RATI_UNITLCOM &rData);
    void ConvertRatForceToDgnForce(BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr, FORCE_BLRD_RCASE &ForceRcase,
        _DGN_FORC_CRC_EX& ForLT, _DGN_FORC_CRC_EX& ForST, _DGN_FORC_CRC_EX& ForLC, _DGN_FORC_CRC_EX& ForPV, _DGN_FORC_CRC_EX& ForSV);


	BOOL GetRCaseUlmForceSum(BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr, FORCE_BLRD_RCASE &ForceRcase, _DGN_FORC_CRC &rData);
    BOOL MakeConverMemberForce(T_STCT_D& StctD, int nRCase, T_STAG_K GirderLastStagK, int nStepNo, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr,
		ArrElemPairKey& aUblmElemK,
        T_STRB_D& MembForNC,
        CSG_MEMB_POSD_KSCE_LSD& MembLcomD);
    BOOL MakePanelForce(T_STCT_D &StctD, int nRCase, ElemPairK ElemK, T_STAG_K GirderLastStagK, int nStepNo, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr, _DGN_FORC_CRC_EX &ForceNC, CSG_MEMB_POSD_KSCE_LSD& MembLcomD);
    BOOL MakePanelForceMax(T_STCT_D &StctD, int nRCase, ElemPairK ElemK, T_STAG_K GirderLastStagK, int nStepNo, BOOL bIsPLCB, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int nConcurr, CSG_MEMB_POSD_KSCE_LSD& MembLcomD);
	void SetRccrBaseData(ElemPairK ElemK, UINT nPos, T_GRUP_K nGrupKey, T_RLCS_K RlcsK, BOOL bDCmax, BOOL bDWmax, BOOL bTEmax, int nConcurr, T_RCCR_BASE &rData);
	int  GetPosNegIndex(BOOL bPositive, CSG_MEMB_RES_AASHTO_LRFD_STR_D &rData);

	double Calc_1PlusImpF(double dImpFactor);

	BOOL   Calc_CbFactor(T_SECT_D &SectD, double dMy1, double dMy2, double& dCb);
	BOOL   Calc_Pl_Ph_SectForce(T_SECT_D &SectD, double dMy1, double dMy2, double& dPl, double& dPh);
	double Get_SectForce(CString strShape, double aSize[8], double dA, double dIy, double dIz, double dCzp, double dCzm, double dMy);
	double Calc_StressOfPosition(double dA, double dIy, double dIz, double dy, double dz, double dFx, double dMy, double dMz);
	double Cal_Cb(bool bSingleCurve, double dPl, double dPh);

    BOOL   CheckSummaryReserveFactorCS454(T_ASRS_CSG_BS_D& rData);
    BOOL   CheckLSSummaryReserveFactorCS454(T_ASRS_LS_CSG_BS_D& rData);

protected:
	BOOL   IsCriticalValue(double dCurV, double dCriV);

private:
	void SetCSGDataCtrl(int nDgnCode);

    BOOL Get_ChkElemFlag4RatingCS454(BOOL bKey, T_SRAS_D& SrasD, int nPosi);
    BOOL Get_ChkElemFlag4RatingCS454(T_SRAS_D& SrasD, int nPosi);
    double GetDeadLoadFactor4CS454(const T_ALCS_D& rData);
    int  GetIndex4MomentRes(double dMy, int nPos);
    BOOL IsPositiveM(double dMy);
    
    
public:

	CDgn_RatingManager *m_pRatingManager;  
	CDgnDataCtrl*	m_pDgnDataCtrl;
	CDgnCsgDataCtrl *m_pCSGDataCtrl;

	CMap<T_MATL_K,T_MATL_K, STL_KR_MATL,      STL_KR_MATL&>      m_mRatingMatl;
	CMap<T_SECT_K,T_SECT_K, STL_KR_SECT,      STL_KR_SECT&>      m_mRatingSect;
	CMap<T_ELEM_K,T_ELEM_K, RATING_STRP_ELEM, RATING_STRP_ELEM&> m_mRatingStrp;
	CMap<T_ELEM_K,T_ELEM_K, RATING_GRPD_ELEM, RATING_GRPD_ELEM&> m_mRatingGrpd;
	CMap<T_ELEM_K,T_ELEM_K, RATING_STFN_ELEM, RATING_STFN_ELEM&> m_mRatingStfn;
	CMap<T_ELEM_K,T_ELEM_K, RATING_RROS_ELEM, RATING_RROS_ELEM&> m_mRatingRRos;
};

#endif // !defined(AFX_DgnStlRatingDataCtrl_H__INCLUDED_)
