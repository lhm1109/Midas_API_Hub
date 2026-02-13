// DgnPlateGirder.h: interface for the CDgnPlateGirder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_DGNPLATEGIRDER_H__INCLUDED_)
#define AFX_DGNPLATEGIRDER_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"
#include "DgnSectUtil.h"
#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
#include "..\wg_db\DB_Type.h"
#include "..\wg_dgn\IDgnDataBase.h"
#include "..\wg_dgn\IDgnCodeCheck.h"
#include <concurrent_unordered_map.h>
#include <thread>
#include <tuple>

//////////////////////////////////////////////////////////////////////////
// Structure Define Start
//////////////////////////////////////////////////////////////////////////

namespace dgn
{
	namespace def
	{
		class CRptSettings;
	}
}

struct CLASS_BASE_UNIT
{
	int iClass;
	int iWebDeter;
	double dPsi;	
	double dk_sig;
	double dRho	;
	double dbeff;
	double dbe1;
	double dbe2;

	void Initialize()
	{
		iClass		= 0;
		iWebDeter	= 0;
		dPsi		= 0.0;	
		dk_sig		= 0.0;	
		dRho		= 0.0;	
		dbeff		= 0.0;	
		dbe1		= 0.0;
		dbe2		= 0.0;
	}

	CLASS_BASE_UNIT() { Initialize(); }
};

struct T_CLASS_IN_UNIT_D
{
	int		nType;		 // 0 : Outstand Flange, 1 : internal compression part
	double dStressOuter; // Outer Position stress in plate
	double dStressInter; // Inner Position stress in plate
	double dc;			// Width
	double dt;			// Thickness

	void Initialize()
	{
		nType			= 0;
		dStressOuter	= 0.0;
		dStressInter	= 0.0;	
		dc				= 0.0;	
		dt				= 0.0;	
	}

	T_CLASS_IN_UNIT_D(){ Initialize(); }

	T_CLASS_IN_UNIT_D(const T_CLASS_IN_UNIT_D& rData) {*this = rData; }
	T_CLASS_IN_UNIT_D& operator=(const T_CLASS_IN_UNIT_D& rData) 
	{
		nType			= rData.nType;
		dStressOuter	= rData.dStressOuter;
		dStressInter	= rData.dStressInter;	
		dc				= rData.dc			;	
		dt				= rData.dt			;	
		return *this;
	}
};

struct T_CLASS_IN_D
{
	CArray<T_CLASS_IN_UNIT_D, T_CLASS_IN_UNIT_D&> aTopFlangeD;
	CArray<T_CLASS_IN_UNIT_D, T_CLASS_IN_UNIT_D&> aBotFlangeD;
	CArray<T_CLASS_IN_UNIT_D, T_CLASS_IN_UNIT_D&> aWebD;

	void Initialize()
	{
		aTopFlangeD.RemoveAll();
		aBotFlangeD.RemoveAll();	
		aWebD.RemoveAll();
	}

	T_CLASS_IN_D(){ Initialize(); }

	T_CLASS_IN_D(const T_CLASS_IN_D& rData) {*this = rData; }
	T_CLASS_IN_D& operator=(const T_CLASS_IN_D& rData) 
	{
		aTopFlangeD.Copy(rData.aTopFlangeD);
		aBotFlangeD.Copy(rData.aBotFlangeD);
		aWebD.Copy(rData.aWebD);
		return *this;
	}
};

struct T_VERTICAL_SHAER_RES_UNIT_D
{
	BOOL bIsShearBuck, bEta3Exceed05, bCombinedEffe, bStiffened;
	int nShearBuckCount, iLongStiffSize, nLSsize_cr, iLamda_w_scope, iLamda_w_Type, iEndpost;

	double dM_Ed, dMf_Rd, dMpl_Rd, dVpl_Rd, dVbw_Rd, dVbf_Rd, dBar_Eta1, dBar_Eta1_Lim, dBar_Eta3, dCombinedEffe, dIsl_org;
	double dIsl, dXw, dMax_Bar_Lamda_w, dc, dbf, dtf, dVb_Rd_org, dVb_Rd_Lim, dRigidPitch;
	double dai_cr, dhi_cr, dk_tau_si_cr, dk_tau_sti_cr, dk_tau_st_lti_cr, dIsli_cr, dDist4Shr;
	double dMax_Lamda_wi, dk_tau_p, dk_tau_st_p, dk_tau_st_Limit_p, dLamda_w_part, dLamda_w_cr, Vb_Rd_Limit, dMf_Rd_factor, dFactor_fr;  

	void Initialize()
	{
		bIsShearBuck = bEta3Exceed05 = bCombinedEffe = bStiffened =  FALSE;
		nShearBuckCount = iLongStiffSize = nLSsize_cr = iLamda_w_scope = iLamda_w_Type = iEndpost = 0;
		dM_Ed= dMf_Rd= dMpl_Rd= dVpl_Rd= dVbw_Rd= dVbf_Rd= dBar_Eta1= dBar_Eta1_Lim= dBar_Eta3= dCombinedEffe= dIsl_org = 0.0;
		dIsl= dXw= dMax_Bar_Lamda_w= dc= dbf= dtf= dVb_Rd_org= dVb_Rd_Lim= dRigidPitch = 0.0;
		dai_cr= dhi_cr= dk_tau_si_cr= dk_tau_sti_cr= dk_tau_st_lti_cr= dIsli_cr = dDist4Shr = 0.0;
		dMax_Lamda_wi= dk_tau_p= dk_tau_st_p= dk_tau_st_Limit_p= dLamda_w_part= dLamda_w_cr= Vb_Rd_Limit= dMf_Rd_factor= dFactor_fr = 0.0;
	}

	T_VERTICAL_SHAER_RES_UNIT_D(){ Initialize(); }
	T_VERTICAL_SHAER_RES_UNIT_D(const T_VERTICAL_SHAER_RES_UNIT_D& rData) {*this = rData; }
	T_VERTICAL_SHAER_RES_UNIT_D& operator=(const T_VERTICAL_SHAER_RES_UNIT_D& rD) 
	{
		bIsShearBuck = rD.bIsShearBuck, bEta3Exceed05 = rD.bEta3Exceed05, bCombinedEffe = rD.bCombinedEffe, bStiffened = rD.bStiffened;
		nShearBuckCount = rD.nShearBuckCount, iLongStiffSize = rD.iLongStiffSize, nLSsize_cr = rD.nLSsize_cr;
		iLamda_w_scope = rD.iLamda_w_scope, iLamda_w_Type = rD.iLamda_w_Type, iEndpost = rD.iEndpost;

		dM_Ed = rD.dM_Ed, dMf_Rd = rD.dMf_Rd, dMpl_Rd = rD.dMpl_Rd, dVpl_Rd = rD.dVpl_Rd, dVbw_Rd = rD.dVbw_Rd;
		dVbf_Rd = rD.dVbf_Rd, dBar_Eta1 = rD.dBar_Eta1, dBar_Eta1_Lim = rD.dBar_Eta1_Lim, dBar_Eta3 = rD.dBar_Eta3;
		dCombinedEffe = rD.dCombinedEffe, dIsl_org = rD.dIsl_org;
		dIsl = rD.dIsl, dXw = rD.dXw, dMax_Bar_Lamda_w = rD.dMax_Bar_Lamda_w, dc = rD.dc, dbf = rD.dbf, dtf = rD.dtf;
		dVb_Rd_org = rD.dVb_Rd_org, dVb_Rd_Lim = rD.dVb_Rd_Lim, dRigidPitch = rD.dRigidPitch;
		dai_cr = rD.dai_cr, dhi_cr = rD.dhi_cr, dk_tau_si_cr = rD.dk_tau_si_cr, dk_tau_sti_cr = rD.dk_tau_sti_cr;
		dk_tau_st_lti_cr = rD.dk_tau_st_lti_cr, dIsli_cr = rD.dIsli_cr, dDist4Shr = rD.dDist4Shr;
		dMax_Lamda_wi = rD.dMax_Lamda_wi, dk_tau_p = rD.dk_tau_p, dk_tau_st_p = rD.dk_tau_st_p, dk_tau_st_Limit_p = rD.dk_tau_st_Limit_p;
		dLamda_w_part = rD.dLamda_w_part, dLamda_w_cr = rD.dLamda_w_cr, Vb_Rd_Limit = rD.Vb_Rd_Limit, dMf_Rd_factor = rD.dMf_Rd_factor;
		dFactor_fr = rD.dFactor_fr; 

		return *this;
	}
};



struct T_VERTICAL_SHAER_RES_D
{
	double dVbw_Rd;
	double dVbf_Rd;
	T_VERTICAL_SHAER_RES_UNIT_D ShearResD[2]; // 0 : Left Web,  1 : Right Web

	void Initialize()
	{
		dVbw_Rd = 0.0;
		dVbf_Rd	= 0.0;
		for(int i = 0; i < 2; ++i){ShearResD[i].Initialize();}
	}

	T_VERTICAL_SHAER_RES_D(){ Initialize(); }
	T_VERTICAL_SHAER_RES_D(const T_VERTICAL_SHAER_RES_D& rData) {*this = rData; }
	T_VERTICAL_SHAER_RES_D& operator=(const T_VERTICAL_SHAER_RES_D& rD) 
	{
		dVbw_Rd = rD.dVbw_Rd;
		dVbf_Rd = rD.dVbf_Rd;
		for(int i = 0; i < 2; ++i){ShearResD[i] = rD.ShearResD[i];}
		return *this;
	}
};

struct T_PLATE_GIRDER_PARAM_D
{
	bool bCheckStrength[6];
	bool bCheckService[2];
	
	UINT   unChkElem;
	UINT   unChkPos;

	double dt_Ld;
	double dk1;
	double dk2;
	double dk3;
	double dks;

	bool bIsIDesignModule;

	int iLcomNum;

	BOOL bChkIJ[2];
};

struct T_PLATE_GIRDER_CALC_D
{
	ElemPairK ElemK;
	double dHt[2]; // H_total
	double dZbar_sb[2]; // Slab 도심 
	double dCzm_sb[2]; // Slab
	double dCzp_sb[2];
	double dArea_sb[2];
	double dBc_sb[2];
	double aY_sb[2][4]; // x1,x2..
	double aZ_sb[2][4]; // y1,y2..
	BOOL bChkData[9]; // CALC_DATA_TYPE

	CPG_MATL_ELEM MatlElem;
	CPG_SECT_ELEM SectElem;
	CPG_SCON_ELEM SconElem;
	CPG_LSTI_ELEM LstiElem;
	CPG_TRST_ELEM TrstElem;
	CPG_TSES_ELEM TsesElem;
	CPG_MEMB_ELEM MembElem;
	CPG_BCDT_ELEM BcdtElem;
	CPG_FRDT_ELEM FrdtElem;
	CPG_RBAR_ELEM RbarElem;
	CPG_LTBD_ELEM LtbdElem;

	T_GENL_DATA GLineData;
	T_GLINE_DGN_D GLineDgn;
	CArray<_DGN_RBAR_CRC, _DGN_RBAR_CRC&> arRbarPosi;
	CArray<CPG_LSTI_DATA, CPG_LSTI_DATA&> arLstiPosi;

	CPG_TRST_POSI TrstPosi;
	CPG_TSES_POSI TsesPosi;
	CPG_SCON_POSI SconPosi;
	CPG_LTBD_POSI LtbdPosi;
	CPG_MEMB_POSI MembPosi;
	CPG_BCDT_POSI BcdtPosi;
	CPG_FRDT_POSI FrdtPosi;

	T_PGBR_LCOM PgbrLcomD;
	T_PRVS_D PrvsD;
	T_PGLS_D PglsD;
	T_PGLS_BASE PglsBase;
	T_PGBR_BASE PgbrBase;
	T_PGBR_DETAIL PgbrDetail;

	CArray<int, int&> aWebLineId;
	CArray<int, int&> aTopFlgLineId;
	CArray<int, int&> aBotFlgLineId;
	CArray<int, int&> aEffLineIx;

	T_CLAS_BASE ClassBase;
	T_DLINE_CALC_D DLineCalcD;

	T_SECT_STIFFNESS tEffeStlSect;
	T_SECT_STIFFNESS tEffeComSect;
	T_SECT_SECTBASE_D SectBaseD;

	BOOL bIsGenSect;
	BOOL bTapered;
	BOOL bPosiI;
	UINT iPos;
	double dPlasticNA;

	void Init()
	{
		ElemK = {0u, 0U};
		MatlElem.Initialize();
		SectElem.Initialize();
		SconElem.Initialize();
		LstiElem.Initialize();
		TrstElem.Initialize();
		TsesElem.Initialize();
		MembElem.Initialize();
		BcdtElem.Initialize();
		FrdtElem.Initialize();
		RbarElem.Initialize();
		LtbdElem.Initialize();
		PgbrLcomD.Initialize();
		PrvsD.Initialize();
		PglsD.Initialize();
		aWebLineId.RemoveAll();
		aTopFlgLineId.RemoveAll();
		aBotFlgLineId.RemoveAll();
		aEffLineIx.RemoveAll();
		ClassBase.Initialize();
		DLineCalcD.Initialize();
		PglsBase.Initialize();
		PgbrBase.Initialize();
		PgbrDetail.Initialize();		
		tEffeStlSect.Initialize();
		tEffeComSect.Initialize();
		GLineData.Initialize();
		GLineDgn.Initialize();
		arRbarPosi.RemoveAll();
		arLstiPosi.RemoveAll();
		bIsGenSect = FALSE;
		bTapered = FALSE;
		bPosiI = TRUE;
		iPos = 0;
		SectBaseD.Initialize();
		TrstPosi.Initialize();
		TsesPosi.Initialize();
		SconPosi.Initialize();
		LtbdPosi.Initialize();
		MembPosi.Initialize();
		BcdtPosi.Initialize();
		FrdtPosi.Initialize();
		dPlasticNA = 0.0;
	}
	T_PLATE_GIRDER_CALC_D() { Init(); }
};


struct T_PLATE_GIRDER_LCOM_RES_D
{
	bool bULSLcom;
	bool bSLSLcom;
	int iDgnLcomNo;

	T_PGBR_LCOM PgbrLcomD;
	T_PRVS_D PrvsD;
	T_PLTB_D PltbD;
	T_PRTF_D PrtfD;
	T_PGLS_D PglsD;
	T_PGRF_D PgrfD;
	T_PGBS_D PgbsD;
	T_PLSS_D PlssD;
	T_RHOC_LCOM RhoC;
	
	T_PLATE_GIRDER_LCOM_RES_D() { Init(); }
	T_PLATE_GIRDER_LCOM_RES_D& operator=(const T_PLATE_GIRDER_LCOM_RES_D& rData)
	{
		bULSLcom = rData.bULSLcom;
		bSLSLcom = rData.bSLSLcom;
		iDgnLcomNo= rData.iDgnLcomNo;
		PgbrLcomD = rData.PgbrLcomD;
		PrvsD = rData.PrvsD;
		PltbD = rData.PltbD;
		PrtfD = rData.PrtfD;
		PglsD = rData.PglsD;
		PgrfD = rData.PgrfD;
		PgbsD = rData.PgbsD;
		PlssD = rData.PlssD;
		RhoC = rData.RhoC;

		return *this;
	}

	void Init()
	{
		bULSLcom = false;
		bSLSLcom = false;
		iDgnLcomNo = 0;

		PgbrLcomD.Initialize();
		PrvsD.Initialize();
		PltbD.Initialize();
		PrtfD.Initialize();
		PglsD.Initialize();
		PgrfD.Initialize();
		PgbsD.Initialize();
		PlssD.Initialize();
		RhoC.Initialize();
	}
};

struct T_PLATE_GIRDER_RES_D
{
	T_PGBR_LCOM PgbrPosiRat;
	T_PGBR_LCOM PgbrNegaRat;
	T_RHOC_LCOM RhocPosiRat;
	T_RHOC_LCOM RhocNegaRat;
	T_PRVS_D PrvsRat;
	T_PLTB_D PltbRat;
	T_PRTF_D PrtfRat;
	T_PGLS_D PglsRat;
	T_PGRF_D PgrfRat;
	T_PGBS_D PgbsRat;
	T_PLSS_D PlssRat;

	std::vector<T_PLATE_GIRDER_LCOM_RES_D> svLcomRes;
	double dtau_Ed_max[2];

	T_PLATE_GIRDER_RES_D() { Init(); }
	void Init()
	{
		PgbrPosiRat.Initialize();
		PgbrNegaRat.Initialize();
		RhocPosiRat.Initialize();
		RhocNegaRat.Initialize();
		PrvsRat.Initialize();
		PltbRat.Initialize();
		PrtfRat.Initialize();
		PglsRat.Initialize();
		PgrfRat.Initialize();
		PgbsRat.Initialize();
		PlssRat.Initialize();
		svLcomRes.clear();
		dtau_Ed_max[0] = 0.0;
		dtau_Ed_max[1] = 0.0;
	}
	T_PLATE_GIRDER_RES_D& operator=(const T_PLATE_GIRDER_RES_D& rData)
	{
		PgbrPosiRat	= rData.PgbrPosiRat;
		PgbrNegaRat	= rData.PgbrNegaRat;
		RhocPosiRat	= rData.RhocPosiRat;
		PrvsRat = rData.PrvsRat;
		PltbRat = rData.PltbRat;
		PrtfRat = rData.PrtfRat;
		PglsRat = rData.PglsRat;
		PgrfRat = rData.PgrfRat;
		PgbsRat = rData.PgbsRat;
		PlssRat = rData.PlssRat;
		svLcomRes = rData.svLcomRes;
		dtau_Ed_max[0] = rData.dtau_Ed_max[0];
		dtau_Ed_max[1] = rData.dtau_Ed_max[1];
		
		return *this;
	}
};

enum PGCHK_TYPE
{
	REBAR = 0,
	SHEAR_CONNECTOR,
	LONG_STIFF,
	TRANS_STIFF,
	END_SUPPORT,
	LTBD,
	MEMB,
	BUCK_COEFF,
	LAMDA,
	NUM,
};

//////////////////////////////////////////////////////////////////////////
// Structure Define End
//////////////////////////////////////////////////////////////////////////

class CMembCtrl;
class CAnalysisResult;
class CDgnSectUtil;

namespace dgn
{
    namespace def
    {
        class IResult;
        class CDgnRptCriticalInfo;
    }
}


struct elempair_hash
{
	size_t operator()(
		const std::pair<ElemPairK, UINT>& k
		) const noexcept
	{
		// 1단계: 내부 ElemPairK 해시
		size_t h1 = std::hash<unsigned int>{}(k.first.first);
		size_t h2 = std::hash<UINT>{}(k.first.second);

		size_t h_elem = h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));

		// 2단계: 바깥 pair의 second
		size_t h3 = std::hash<UINT>{}(k.second);

		// 3단계: 전체 결합
		return h_elem ^ (h3 + 0x9e3779b9 + (h_elem << 6) + (h_elem >> 2));
	}
};

class CDgnPlateGirder : public CDgnPscCommon, public IDgnDataBase, public IDgnCodeCheck
{
// Member Functions.
public:
	CDgnPlateGirder();
	virtual ~CDgnPlateGirder();
	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);  

protected:
	T_UNIT_INDEX m_CodeUnit;
	double m_dPi;
	// Partial Factor
	double m_dGamma_C;    // Concrete, 
	double m_dGamma_S;    // Reinforcing steel
	double m_dGamma_M0;   // Structural steel
	double m_dGamma_M1;   // Structural steel
	double m_dGamma_V;    // Shear resistance of a headed stud
	double m_dGamma_Ff;   // Equivalent constant amplitude stress range
	double m_dGamma_Mf;   // Fatigue strength
	double m_dGamma_Mfs;  // Fatigue strength of studs in shear
	double m_dGamma_M_ser;

	double m_dAlpha; // For Concrete Compression Strength
	double m_dPlasticNA;  

	double m_dZero;
	BOOL m_bTestMode;
	BOOL m_bPrintMode;

	CDgnSectUtil *m_pDgnSectUtil;
	T_CPGD_D m_CpgdD;	
    int m_DgnLcomNo;
	size_t m_nThread;

	Concurrency::concurrent_unordered_map<ElemPairK, T_PLATE_GIRDER_RES_D*, EPairk_hash> m_svRes;
	Concurrency::concurrent_unordered_map<std::thread::id, std::tuple<ElemPairK, UINT, UINT>> m_ElemToLcom;//멀티쓰레드로 돌아갈때만 해당 맵을 사용한다. elem, posi, lcom
	Concurrency::concurrent_unordered_map<ElemPairK, _DGN_FORC_CRC_ELEM, EPairk_hash> m_mForce;
	Concurrency::concurrent_unordered_map<ElemPairK, _DGN_STRS_CRC_ELEM, EPairk_hash> m_mStress;
	Concurrency::concurrent_unordered_map<ElemPairK, _DGN_STRS_CRC_ELEM, EPairk_hash> m_mStressP2;
	Concurrency::concurrent_unordered_map<std::pair<ElemPairK, UINT>, CPG_FORC_STAG, elempair_hash> m_mStageForce; //elem, lcom
	Concurrency::concurrent_unordered_map<ElemPairK, _DGN_SCFP_PART_ELEM, EPairk_hash> m_mScfpCr;
	Concurrency::concurrent_unordered_map<ElemPairK, _DGN_SCFP_PART_ELEM, EPairk_hash> m_mScfpSh;
	Concurrency::concurrent_unordered_map<ElemPairK, _DGN_SCFP_PART_ELEM, EPairk_hash> m_mScfpTe;

	UINT GetIDgnCheckPosition(const std::thread::id& id) const;
	UINT GetIDgnLcomNo(const std::thread::id& id) const;
	ElemPairK GetIDgnCheckElem(const std::thread::id& id) const;

	virtual bool InitializeRpt() override;
	virtual bool FinalizeRpt() override;

public:
	BOOL Make_ClassSection(ElemPairK ElemK, T_PGBS_D& PgbsD, T_PGBR_LCOM& PgbrLcomD);
	BOOL Make_ClassInD(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D& ClassInD);
	BOOL Make_ClassInD_I(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D& ClassInD);
	BOOL Make_ClassInD_Box(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D& ClassInD);
	BOOL Make_ClassInD_Tub(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D& ClassInD);
	BOOL Get_BendingResistanceForPlateGirder(ElemPairK ElemK, T_PGBR_LCOM& PgbrLcomD);
	BOOL Get_VerticalShearResistanceForPlateGirder(ElemPairK ElemK, T_PRVS_D& PrvsD);
	BOOL Get_LateralTorsionalBucklingResistanceForPlateGirder(ElemPairK ElemK, T_PLTB_D& PltbD);
	BOOL Get_TransverseForceResistanceForPlateGirder(ElemPairK ElemK, T_PRTF_D& PrtfD);
	BOOL Get_LongitudinalShearResistanceForPlateGirder(ElemPairK ElemK, T_PGLS_D& PglsD);
	BOOL Get_FatigueResistanceForPlateGirder(ElemPairK ElemK, T_PGRF_D& PgrfD, double dt_Ld);
	BOOL Get_BeamStressResistanceForPlateGirder(ElemPairK ElemK, T_PGBS_D& PgbsD);

	// For Design Result Print
	BOOL Get_PlateGirderDgnResult(BOOL* abChkItem);
	BOOL Get_BendingResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGBR_BASE& PgbrGd, const T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);
	BOOL Get_VerticalShearResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PRVS_BASE& PrvsGd, const T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	BOOL Get_LateralTorsionalBucklingResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PLTB_BASE& PltbGd, const T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail);
	BOOL Get_TransverseForceResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail);
	BOOL Get_LongitudinalShearResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGLS_BASE& PglsGd, const T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail);
	BOOL Get_FatigueResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGRF_BASE& PgrfBase, T_PGRF_DETAIL& PgrfDetail);
	BOOL Get_BeamStressDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail);
	BOOL Get_LongitudinalShearResistanceServiceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PLSS_BASE& PlssGd, const T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail);

	void Set_PrintMode(BOOL bPrint);
	BOOL IsPrintMode() const;
	// Calculation
	//BOOL Get_BendingResistance(BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CPG_SCON_ELEM& SconElem, T_PGBR_BASE& PgbrBase);
	
protected:
	BOOL Get_DesignResultExist(BOOL* abChkItem);
	void InitPartialFactor();
	BOOL Get_ClassifyClassSection(T_PLATE_GIRDER_CALC_D& param, double aGirderForce[6], double aCptForce[6]);
	BOOL Get_SectionClass(T_PLATE_GIRDER_CALC_D& param, BOOL bChkRestrained, const T_CLASS_IN_D& crClassInD);
	BOOL Calc_OutstandClassUnit(const T_CLASS_IN_UNIT_D& ClassInD, double dfy, double dfy1, int& iClass,CLASS_BASE_UNIT& ClassBase);
	BOOL Get_InternalWebClassClassification(bool bPosiI,int iBotClass, double aForce[6], double dStress[4], CPG_MATL_ELEM& MatlElem, const T_SECT_D& crSectD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, int& iClass, T_CLAS_BASE& ClasBase);
	BOOL Calc_InternalPlateClassUnit(T_PLATE_GIRDER_CALC_D& param, const T_CLASS_IN_UNIT_D& ClassInD, double dfy, double dfy1, int& iClass, CLASS_BASE_UNIT& ClassBase, bool bWeb = true, bool bTop = true);
	BOOL Calc_ClassifyClassSection4GenSect(BOOL bChkRestrained, double dfy, double dfy1, double* aGirderForce, double* dForce, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, T_PLATE_GIRDER_CALC_D& param);
	BOOL Calc_PartLineClassSection4GenSect(double dfy, double dfy1, int iBotClass, double* aGirderForce, double* dForce, double dPlasticNA, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, T_DLINE_PART_D& DLinePartD);
	BOOL Calc_SubLineClassSection4GenSect(int nPartType, double dfy, double dfy1, int iBotClass, double* aGirderForce, double* dForce, double dPlasticNA, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, const T_GENL_DATA& GLineData, const T_GLINE_DST& LineDst, T_DGNLINE_LBD& DLineLbd, int& iClass);
	BOOL Is_Sig1IsFree4Outstand(BOOL bIs1_Sig1);	
	double Get_OutstandBucklingfactor(int iType, double dPsi);  
	double Get_InternalBucklingfactor(double dPsi);
	int Get_PartClass(int iStreType, double dct, double dEpsi, double dAlpha, double dk);
	int Get_InternalPartClass(BOOL bChkPlastic, int iStreType, double dct, double dEpsi, double dAlpha, double dPsi);
	int Get_WebPlaticClass(double dct, double dEpsi, double dAlpha);
	int Get_WebElasticClass(double dAlpha, double dct, double dEpsi, double dPsi);
	BOOL Get_ReductionFactorRho(BOOL bInternal, double dBar_b, double dt, double dLamda_p, double dPsi, double& dRho);
	double Get_Lamda_p(double db, double dt, double dEpsilon, double dk_sig);
	BOOL Get_EffectiveSectionProperty(BOOL bPosiI, double dMy, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, T_CLAS_BASE& ClassD, T_SECT_STIFFNESS& EffeStlSect, T_SECT_STIFFNESS& EffeComSect);	
	//

	double Get_PlasticNA(BOOL bPositive, T_PLATE_GIRDER_CALC_D& param, BOOL bNeglectWeb = FALSE);
	double Get_PlasticNA4GenSect(BOOL bPositive, T_PLATE_GIRDER_CALC_D& param, T_SECT_SECTBASE_D& SectBaseD, const T_GLINE_DGN_D& GLineDgn);
	BOOL Get_EffeSectPropAfterDB(BOOL bIsBefore, BOOL bPositiveM, double dfy1, double dfy2, double* aForceGd, double* aForce, double da, T_PLATE_GIRDER_CALC_D& param, T_SECT_STIFFNESS& EffeStiff, T_DGNLINE_LBD_BR& RhoC, BOOL bCompoStiffData = FALSE);
	BOOL Get_EffeSectPropBeforeDB(BOOL bIsBefore, BOOL bPositiveM, T_PLATE_GIRDER_CALC_D& param, double dfy1, double dfy2, double* aForceGd, double* aForce, double da, const CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&>& aStiffness, T_SECT_STIFFNESS& EffeStiff, T_DGNLINE_LBD_BR& RhoC);
	BOOL Get_EffectiveCzmData4Sect(bool bPosiI, T_CLAS_BASE& ClassD, BOOL bPositive, T_SECT_D& SectD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, T_DGNLINE_LBD& LineLBD, double& dA_eff, double& dCzm_eff, double& dIy_eff);
	BOOL Get_EffectiveSectionProperty4DB(BOOL bIsBefore, BOOL bPositiveM, double* aForceGd, double* aForce, double da, T_PLATE_GIRDER_CALC_D& param, T_SECT_STIFFNESS& EffeStlSect, T_SECT_STIFFNESS& EffeComSect, T_DGNLINE_LBD_BR& RhocBefore, T_DGNLINE_LBD_BR& RhocAfter, BOOL bCompoStiffData = FALSE);
	BOOL Get_LineDgnInfo4SectDB(BOOL bIsBefore, BOOL bPositiveM, bool bPosiI, T_CLAS_BASE& ClassD, double dfy1, double dfy2, double dEs, double dPoisson, double* aForceGd, double* aForce, T_SECT_D& SectD, double da, const CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&>& aStiffness, CArray<CPG_LSTI_DATA, CPG_LSTI_DATA&>& arLstiPosi, CArray<T_GLINE_DGN_B, T_GLINE_DGN_B&>& aLineDgnBase, T_DGNLINE_LBD& LineLBD);
	BOOL Get_LineInfo4SectDB(bool bPosiI, T_SECT_D& SectD, const T_SECT_STIFFNESS& AppStiffD, CArray<CPG_LSTI_DATA, CPG_LSTI_DATA&>& arLstiPosi, CArray<T_GLINE_DGN_B, T_GLINE_DGN_B&>& aLineDgnBase);
	BOOL Get_ConvertPoint4Centroid(double dCym, double dCzm, CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&>& aLineDgnBase);
	//
	BOOL Get_StiffnessOfEffeSectCompo(BOOL bPositive, T_SECT_INFO_D& SectInfoD, T_SECT_STIFFNESS& EffeStiff);	
	BOOL Calc_BendingResistance(T_PLATE_GIRDER_CALC_D& param, T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);
	BOOL Get_BetaReductionFactor(double dXpl, double dh, double& dBeta);
	BOOL Calc_NonLinearResistance(ElemPairK ElemK, BOOL bPosiI, T_PGBR_BASE& PgbrBase);
	BOOL Calc_PlasticResistance(T_PLATE_GIRDER_CALC_D& param, T_PGBR_BASE& PgbrBase, double dPlasticNA, double& dMpl_Rd, BOOL bNeglectWeb = FALSE);
	BOOL Calc_ElasticResistance(BOOL bPositive, double aForceGd[6], double aForce[6], T_PLATE_GIRDER_CALC_D& param, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);
	double Calc_kmin(BOOL bPositive, double dk_rbar, double dk_slab, double dk_top, double dk_bot, int &nKminType);	
	BOOL Calc_FatigueResistance(T_PLATE_GIRDER_CALC_D& param, double dt_Ld, T_PGRF_BASE& PgrfBase);
	BOOL Calc_LongitudinalShearResistance(BOOL bSLS, T_PLATE_GIRDER_CALC_D& param, double dks, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail);
	BOOL Calc_TransverseForceResistance(T_PLATE_GIRDER_CALC_D& param, T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail);
	BOOL Calc_LateralTorsionalBucklingResistance(T_PLATE_GIRDER_CALC_D& param, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail);
	BOOL Calc_VerticalShearResistance(T_PLATE_GIRDER_CALC_D& param, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	BOOL Calc_StressCheck4SLS(T_PLATE_GIRDER_CALC_D& param, double dk1, double dk2, double dk3, const _DGN_FORC_CRC& ForceConst, T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail);
	BOOL Calc_LongitudinalShearResistance4SLS(T_PLATE_GIRDER_CALC_D& param, double dks, T_PLSS_BASE& PlssGd, T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail);

	// Gen. Section
	BOOL Calc_BendingResistance4GenSect(T_PLATE_GIRDER_CALC_D& param, T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);
	BOOL Calc_VerticalShearResistance4GenSect(T_PLATE_GIRDER_CALC_D& param, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	BOOL Calc_LongitudinalShearResistance4GenSect(BOOL bSLS, T_PLATE_GIRDER_CALC_D& param, double dks, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail);
	BOOL Get_PlasticMoment4GenSect(BOOL bPositive, T_PLATE_GIRDER_CALC_D& param, const T_GLINE_DGN_D& GLineDgn, double dPlasticNA, double& dMpl_Rd, T_PGBR_DETAIL& PgbrDetail);

	BOOL Get_ElasticResistance4GenSect(BOOL bPositive, double aForceGd[6], double aForce[6], T_PLATE_GIRDER_CALC_D& param, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);
	BOOL Get_Mf_Rd_PlasticMoment4GenSect(BOOL bPositive, T_PLATE_GIRDER_CALC_D& param, T_SECT_SECTBASE_D& SectBaseD, double& dMf_Rd);
	
	BOOL Get_PlasticMomentOfDividedPart(double dfy1, double dfy2, double dCzm_NAz, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLineData, double& dMpl, double& dNgir);
	BOOL Get_PlasticMomentOfHoriDividedLine(double dfy1, double dfy2, double dCzm_NAz, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLineData, const CArray<double, double&>& aCutTopThick, double& dMpl_top, double& dMpl_bot, double& dNgir_top, double& dNgir_bot);
	double Get_Mel_RdOfElastResistance(double dfy, double dFx, double dMy, double dA, double dIy, double dz);
	BOOL Get_EffectivePropertyForClass4(T_PLATE_GIRDER_CALC_D& param, BOOL bPositive, T_GENL_DATA& EffGLineD, T_SECT_STIFFNESS& GirderEffStiff, T_SECT_STIFFNESS& CompoEffStiff);
	double Get_Epsilon(double dfy);
	BOOL Get_IslOfLongiStiff(int iType, double dh, double dt, double dtw, double dEpsi, double& dIsl);
	BOOL Get_IslOfLongiStiff(const CPG_LSTI_DATA& InD, double dtw, double dEpsi, double& dIsl);
	BOOL Get_IslOfLongStfn4GS(double dEpsi, T_GLINE_STIFFENER& StfnD, double dtw, double& dIsl);
	BOOL Get_SlendernessParamLamda(int iType, int iLongStiffSize, double dhw, double da, double dt, double dIsl, double dEpsi, double& dk_tau, double& dk_tau_st, double& dk_tau_st_Limit, double& dLamda);
	BOOL Get_ShearBucklingCoeff(int iLongStiffSize, double dhw, double da, double dt, double dIsl, double& dk_tau, double& dk_tau_st, double& dk_tau_st_Limit);
	BOOL Get_ContributionFromWebFactor(int iEndpostType, double dEta, double dLamda, double& dXw, int& iLamda_w_scope);
	BOOL Get_bf4c_DB(bool bPosiI, T_SECT_D& SectD, bool bTopF, int nWebPart, double dEpsilon, double& dbf); // nWebPart : 0 : Left Web,  1 : Right Web
	//
	double Get_ImperfectionFactor(int iCase);
	int Get_BucklingCurveForCrossSection(double dh, double db, BOOL bIsSectI = TRUE);
	BOOL Get_NonDimensionalSlenderness(double dL, double db, double dfy, double dEs, double dm, double dAwc, double dAf, double& dLamda);
	double Get_PHIform(double &dM1, double &dM2, double dNu);
	BOOL Get_m_Value(double dNu, double dPHI, double dgamma, double& dm1, double& dm2, double& dm_min, double& dm);
	double Get_PHI_LT(double dAlpha, double dLamda_LT);
	double Get_X_LT(double dPHI_LT, double dLamda_LT, double& dX_LT_org);
	double Get_Lamda3_DesignLifeInYears(int iYears); 
	BOOL Get_Eta4SteelMaterial(int iMatClass, double dgam0, double dgam1, double& dEta);
	double Get_Eta4SteelGrade(int nMatClass, double dFy1);
	double Get_Zj_Second_Value(double dH, double dHw, double dtw, double dB1, double dtf1, double dB2, double dtf2, double dCzm, double dAa, double dIay);
	double Get_ShearCenter_H(double dh, double dt1, double db1, double dt2, double db2);
	double Get_fy4Thick(double dfy, double dfy1, double dt);
	BOOL Get_WidthThickRatio4Web(BOOL bStiffened, double deta, double dhw, double dtw, double dEpsi, double dk_tau, double& dCheck_lim, BOOL& bCheck);
	double Get_Vbw_Rd(double dXw, double dfyw, double dAvw, double dGamma_M1);
	double Get_Vbw_Rd_Limit(double dEta, double dfyw, double dAvw, double dGamma_M1);
	double Get_tau_b_Rd(double dX, double dfyf, double dGamma_M1);
	double Get_ReductonFactorOfFlange4N_Ed(double dGamma_M2, double dfy_f1, double dfy_f2, double dN_Ed, double dAf1, double dAf2);
	int Get_Lamda_w_Type4Shear(BOOL bLongStiff, CPG_TRST_POSI& TrstPosi);
	double Get_Alpha4Lamda_w(double da, double dhw);
	BOOL IsMaterialS420orS460(double dfy);
	double Get_RatioOfInteractionMV(double deta1, double dBendingRat, double deta3);
	BOOL GetStressInfo(bool bPosiI, const T_SECT_D& crSectD, double aGirderForce[6], double aCptForce[6], T_SECT_STIFFNESS& GdStiffD, T_SECT_STIFFNESS& StiffD, T_STRESS_INFO_I& rData);
	double Get_InertiaOfStiffener(double db, double dt);
	BOOL Get_Nc_fofSlab(T_PLATE_GIRDER_CALC_D& param, double dfcd, double dXpl, double dHt, double& dNc_f);

	BOOL Get_StressOfPosition(double aGirderForce[6], double aCptForce[6], T_SECT_STIFFNESS& GdStiffD, T_SECT_STIFFNESS& StiffD, double aStress[4]);
    BOOL Get_ConstraintStress(int nPos, const _DGN_SCFP_PART_LCASE& Cr, const _DGN_SCFP_PART_LCASE& Sh, const _DGN_SCFP_PART_LCASE& Te, CPG_STRESS_CONS &rData);
    BOOL Get_ConstraintStressLCase(int nPos, const _DGN_SCFP_PART_LCASE& Cons, CPG_STRESS_CONS &rData);
    BOOL Calc_ConstraintStress4Pos(int nPos, int nPart, const _DGN_SCFP_PART_LCASE& Cons, double adStr[4]);
    BOOL SetPartStress(int nPos, const _DGN_STRS_CRC& P1, const _DGN_STRS_CRC& P2, T_PGBS_BASE& PgbsB, T_PGBS_DETAIL& Det);

	// General Section
public:
	BOOL Get_CalcSectInfo4EffeSect(T_PLATE_GIRDER_CALC_D& param, double dfy1, double dfy2, double da, double* dForce, double dCzm_gap, double dStiff[3], T_DLINE_CALC_D& DlineInfo);
	BOOL Get_CalcSectPartLine4EffeSect(double dfy1, double dfy2, double dE, double dPoisson, double da, double* dForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, T_DLINE_PART_D& DlinePart);  
	BOOL Get_LocalBucklingDataOfSubPanel(double dfy1, double dfy2, double dE, double dPoisson, double da, double* dForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_DST& LineDst, 
																			 T_DGNLINE_LBD& LineLBD);		
	BOOL Get_EffectiveSectData(BOOL bAfter, double* dForce, T_PLATE_GIRDER_CALC_D& param, T_DLINE_CALC_D& DLineInfo, T_GENL_DATA& EffGLineD, T_GLINE_DGN_D& EffGLineDgn);
	BOOL Get_EffectiveSectInfo4PartLine(T_PLATE_GIRDER_CALC_D& param, double* aForce, double dCzm_gap, double dStiff[3], const T_GLINE_PART_DGN& PartDgnD, const T_DLINE_PART_D& DLinePart, T_GENL_DATA& EffGLineD);
	BOOL Get_EffectiveSectInfo4PlateElements(T_PLATE_GIRDER_CALC_D& param, int iClass, double* aForce, double dCzm_gap, double dStiff[3], T_GLINE_DST& GLineDST, T_DGNLINE_LBD& LineLBD, T_GENL_DATA& EffGLineD);		
	BOOL Get_EffectivePanelInformation(T_PLATE_GIRDER_CALC_D& param, int iClass, BOOL bInternal, int iStressType, double* aForce, double dCzm_gap, double dStiff[3], const T_GLINE_BASE& LineBaseD, const T_GLINE_DGN_B& LineDgnB, T_GENL_DATA& EffLineD);
	
	BOOL Get_ConvertVertexId(int vs, int v1, int v2, int& vc1, int& vc2);
	BOOL Get_ConvertSectStiffenerD(const T_DGN_STL_STIFFENER_PART& InD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi);
	//
	BOOL Get_EffectiveSect4PartLine(double dfy1, double dfy2, double* dForce, double dArea, double dIy, double dIz , const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, T_GENL_DATA& EffGLineD, T_GLINE_PART_DGN& EffPartDgnD);  
	BOOL Get_EffectiveWidth4Internal(double dPsi, double db, double dRho, double& dbeff, double& dbe1, double& dbe2);
	BOOL Get_EffectiveWidth4Outstand(int nType, double dPsi, double dc, double dRho, double& dbeff);
	BOOL Get_EffectiveSection4StiffenedPlateElements(double dfy1, double dfy2, double* dForce, double dArea, double dIy, double dIz, const T_GENL_DATA& GLineData, const T_GLINE_DST& LineDst, T_GENL_DATA& EffGLineD, T_GLINE_DST& EffLineDst);  
	BOOL Get_EffectivePanelInfo(BOOL bInternal, int iStressType, BOOL b1stIdx, int ve, double* dForce, double dStiff[3], double dbe1, double dbe2, 
															 const T_GENL_DATA& GLineData, const T_GLINE_BASE& LineBaseD, T_GENL_DATA& EffLineD, T_GLINE_BASE& EffLineBase);
	void Set_CalcSubPanel(BOOL bGrossWidth, int iNtype, double dbb, double dti, T_SUB_PANEL_BASE& rData);
	int Get_StressType4EffSect(BOOL bCompStr, BOOL bDbCurve, BOOL bIs1_Sig1);
	BOOL Get_CompWidthOfDoubleCurvatrue(double* dForce, double dCzm_gap, double dArea, double dIy, double dIz, const T_GENL_DATA& GLineData, const T_GLINE_BASE& LineBaseD, double& dbc);
	BOOL Get_CompWidthOfDoubleCurvatrue(T_GLINE_DGN_B& LineDgnB, double& dbc);
	BOOL Get_ElasticCriticalPlateBucklingData(double dfy, double da, double dE, double dPoisson, double* aForce, T_DGNLINE_LBD& DLineLbd);
	BOOL Get_ConvertCompPanelDgnLineLBD(const T_DGNLINE_LBD& DLineLbd, T_DGNLINE_LBD& CompDLineLbd);
	BOOL Get_ColumnBucklingData(double dfy, double da, double dE, double dPoisson, BOOL bIsOpenStfn, double* aForce, T_DGNLINE_LBD& DLineLbd);
	BOOL Get_Sigma_cr_p_4Stiffener(double da, double dE, double dPoisson, double* aForce, const T_DGNLINE_LBD& CompDLineLbd, T_DGNLINE_LBD& DLineLbd);
	BOOL Get_Sigma_cr_p_4MultifleStiffener(double da, double dE, double dPoisson, double* aForce, T_DGNLINE_LBD& DLineLbd);
	double Get_k_sigma_p(double dalpha, double dgamma, double dPsi, double ddelta, double& dalpha_lim);
	double Get_Ip_4BendingOfPlate(double db, double dt, double dPoisson);
	double Get_Width4StiffenedPlates(int iNtype, double db, double dPsi, BOOL bSup=TRUE);
	int Get_StreeTypeOfStiffenedPlates(double df1, double df2, int& iStrType2);
	BOOL Get_AreaAndEffArea4Line(const T_DGNLINE_LBD& DgnLineLBD, double& dAc, double& dAc_eff_loc);  
	BOOL Get_MomentofAreaOfFlange(int iChkPartIx, double dCzm_gap, T_SECT_STIFFNESS& StiffD, const T_GLINE_DGN_D& GLineDgn, double& dQflg);
	int Get_StiffenerSize(const T_GLINE_DST& GLineDST);
	//	
	double Get_Sigma_E(double dEs, double db, double dt, double dPoisson);
	double Get_Lamda_p4PlateTypeBehaviour(double dBeta_A_c, double dfy, double dSigma_cr_p);
	double Get_Sigma_cr_c4ColumnTypeBehaviour(double dEs, double dt, double dnu, double da);
	double Get_Sigma_cr_sl4ColumnTypeBehaviour(double dEs, double dI_sl, double dA_sl, double da);
	double Get_Sigma_cr_sl4SimplifiedModel(double dE, double dPoisson, double dt, double db, double db1, double db2, double dAsl, double dIsl, double da, double& da_c); // Annex A.2.2
	double Get_xi(double dSigma_cr_p, double dSigma_cr_c);
	double Get_Rho_c_FinalReductionFactor(double dRho, double dXc, double dxi);

	int Get_ConnectedVertexId(int v1, int v2, int v3, int v4);
	double Get_Psi(double dSig1, double dSig2);
	BOOL Get_Psi(double dStr1, double dStr2, double& dSig1, double& dSig2, double& dPsi);
	BOOL Get_PointCoor4beff(double dbe, double dx1, double dy1, double dx2, double dy2, double& dx, double& dy);  
	BOOL Get_CompLengthOfPlastic(BOOL bPositive, double dy1, double dz1, double dy2, double dz2, double& dLen_c, double& dLen_t);

	void SetPgbrBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PGBR_BASE& rPgbrGd, T_PGBR_BASE& rData);
	void SetPrvsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PRVS_BASE& rPrvsGd, T_PRVS_BASE& rData);
	void SetPltbBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, double dV1, double dV2, double dM1, double dM2, double dC4, T_PLTB_BASE& rPltbGd, T_PLTB_BASE& rData);
	void SetPrtfBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForce, T_PRTF_BASE& rData);
	void SetPglsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PGLS_BASE& rPglsGd, T_PGLS_BASE& rData);
	void SetPgfrBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForce, T_PGRF_BASE& rData);
	void SetPgbsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, int iLcomType, double* aForceGd, double* aForce, T_PGBS_BASE& rData);
	void SetPlssBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, int iLcomType, double* aForceGd, double* aForce, T_PLSS_BASE& rPlssGd, T_PLSS_BASE& rData);
	// 845 추가
	BOOL SetRhocBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, const T_PGBR_DETAIL& PgbrDetail, T_RHOC_BASE& Rho_C);
	BOOL Get_MaterialData(ElemPairK ElemK, double& dfy, double& dfy1, double& dEs, double& dfck, double& dEcm, double& dfsk, double& dErs);
	BOOL Get_GLineDgnPartData(const T_GLINE_DGN_D& DLineInfo, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aTopFlangePart, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aBotFlangePart, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aWebPart);
	BOOL Get_DLinePartData(const T_DLINE_CALC_D& DLineInfo, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aTopFlangePart, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aBotFlangePart, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aWebPart);
	BOOL Get_LineEndStress4LineDgnBase(const CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&>& aDgnBaseD, double& dStr1, double& dStr2);    
	BOOL Get_ForceLineGS(double dfy1, double dfy2, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLine, double& dTotalForce);
	BOOL Get_TopBotForceLineGS(double dfy1, double dfy2, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLine, const CArray<double, double&>& aTopThick, double& dTopForce, double& dBotForce);
	BOOL Get_PartLineId(int nPartId, const T_GLINE_DGN_D& GLineDgnD, CArray<int,int&>& aWebLineId);  // nPartId, 0=Top, 1=Bottom, 2=Web

protected:
	BOOL Get_CompositeGeneralSectionInfo(T_PLATE_GIRDER_CALC_D& param, BOOL bChkXY=FALSE);
	void Set_LineDgnBase(int iLineType, double db, double dt, double dy1, double dz1, double dy2, double dz2, double df1, double df2,
											 BOOL bCompStr, BOOL bDbCurve, BOOL bIs1_sig1, double dy0, double dz0, T_GLINE_DGN_B& LineDgnBase);	

	BOOL Calc_Vbw_Rd(T_PLATE_GIRDER_CALC_D& param, double& dVbw_Rd, T_VERTICAL_SHAER_RES_D& ResD);
	BOOL Calc_Vbf_Rd(T_PLATE_GIRDER_CALC_D& param, T_SECT_D& SectD, T_PRVS_BASE& PrvsBase, double& dVbf_Rd, T_VERTICAL_SHAER_RES_D& ResD);
	BOOL Calc_Vb_Rd(T_PLATE_GIRDER_CALC_D& param, T_SECT_D& SectD, T_PRVS_BASE& PrvsBase, double& dVb_Rd, T_VERTICAL_SHAER_RES_D& ResD);
	BOOL Calc_Lambda_w(T_PLATE_GIRDER_CALC_D& param, double& Lambda_w, T_VERTICAL_SHAER_RES_UNIT_D& ResD);

	void Set_Calc_Vbw_Param(double dAlpha, double dk_tau, double dk_tau_st, double dk_tau_st_limit, double dIsl, double dhw, double dt, double dLamda_w, double dLamda_w_cr, int iLamda_w_Type, T_PRVS_Vbw_Rd_Param& rData);
	void Set_Calc_Vbw_Param2(int iLamda_w_scope, double dXw, double dVbw_Rd, double dV_Rd, double dVb_Rd_lim, double dV_Edi, double deta3, T_PRVS_Vbw_Rd_Param& rData);
	void Set_Calc_Vbw_SubPanel_Param(int nLongStfnSize, double dhw, double dt, double dAlpha, double dk_tau, double dk_tau_st, double dk_tau_st_lt, double dIsl, double dLamda_w, T_PRVS_Vbw_Rd_Sub_Panel& rData);
	void Set_Calc_FlangeShearstress_Param(double dtau_Ed, double dtau_b_Rd, double dV_Edi, double dQi, double dIyyi, double dbf, T_PRVS_Vbw_Rd_Param& rData);  
	
	double Get_ShearStress(double dV, double dQ, double dIyy, double dbw);
	double Get_Q_FirstMomentOfGirderCentroid(double* aSize, double dZcen_g, double dZcen_c);
	BOOL Get_ThicknessOfWeb4GenSect(const T_GLINE_DGN_D& GLineDgn, double& dbw, double& dAweb);
	BOOL Get_ThicknessOfFlange4GenSect(const T_GLINE_DGN_D& GLineDgn, double& dt_top, double& dt_bot);
	
	BOOL IsValueZero(double dValue);
	BOOL Get_SectPropOfDLinePart(CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aDLinePart, double& dArea, double& dt);
	BOOL Get_SectPropOfLineLBD(CArray<T_DGNLINE_LBD,T_DGNLINE_LBD&>& aDLineLBD, double& dArea, double& dt);

	// Refactoring Methods
protected:
	// Get.. Function
	void GetPureSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData);
	void GetNonComSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData);
	void GetRebarSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData) const;
	double GetOutStandFlgLineLengD(BOOL bTop, BOOL bLeft, bool bPosiI, const T_SECT_D& crSectD);
	BOOL GetCentFlgLinePosD(bool bPosiI, BOOL bTop, BOOL bLeft, const T_SECT_D& crSectD, const T_SECT_STIFFNESS& crStiffD, 
									double& dy1, double& dz1, double& dy2, double& dz2);
	BOOL GetCentWebLinePosD(bool bPosiI, BOOL bLeft, const T_SECT_D& crSectD, const T_SECT_STIFFNESS& crStiffD, 
									double& dy1, double& dz1, double& dy2, double& dz2);
	double GetPlasticNA();
	void SetPlasticNA(const double& dPlasticNA);
	BOOL Get_PlasticNAEff(BOOL bPositive, T_PLATE_GIRDER_CALC_D& param, double dPlasticNAxis, double& dEffPlasticNA, double& dEffeLen, double& dHoleLen);
	void Convert_VerbShearResD(const T_VERTICAL_SHAER_RES_UNIT_D& InD, T_PRVS_BASE_UNIT& rData);
	BOOL IsPositiveMy(double dTopStress, double dBotStress);
	// 임시 계산서 생성. 
	BOOL Calc_PlateGirderElem(ElemPairK ElemK, const T_PLATE_GIRDER_PARAM_D& ParamD, T_PLATE_GIRDER_RES_D& ResD);
	BOOL Calc_PlateGirderElem_IDesign(ElemPairK ElemK, const T_PLATE_GIRDER_PARAM_D& ParamD, T_PLATE_GIRDER_RES_D& ResD);
	BOOL Set_CalcDataCpgElem(ElemPairK ElemK, T_PLATE_GIRDER_CALC_D& param);
	BOOL Set_CalcDataByPos(T_PLATE_GIRDER_CALC_D& param, int iPos);

	double Get_Height4Gen(const T_PLATE_GIRDER_CALC_D& param);
	double Get_ZcentroidofSlab(const T_PLATE_GIRDER_CALC_D& param);
	double Get_CzmofSlab(const T_PLATE_GIRDER_CALC_D& param);
	double Get_CzpofSlab(const T_PLATE_GIRDER_CALC_D& param);
	double Get_AreaofSlab(const T_PLATE_GIRDER_CALC_D& param);
	double Get_BcofSlab(const T_PLATE_GIRDER_CALC_D& param);

public:
	BOOL Execute_Detail_SteelPlateGirder(ArrElemPairKey& arElemK, BOOL* bPgdChk);
	
protected:
	// Detail
	CString m_strUnitMM, m_strUnitMM2, m_strUnitMM3, m_strUnitMM4;
	CString m_strUnitForce;
	CString m_strUnitForce2;
	CString m_strUnitMoment;
	CString m_strUnitStress;  
	
	CAnalysisResult* m_pAnalysisResult;
	
	BOOL Detail_BendingResistance(ElemPairK ElemK, BOOL bPosiI);
	BOOL Detail_VerticalShearResistance(ElemPairK ElemK, BOOL bPosiI);
	BOOL Detail_LateralTorsionalBucklingResistance(ElemPairK ElemK, BOOL bPosiI);
	BOOL Detail_TransverseForceResistance(ElemPairK ElemK, BOOL bPosiI);
	BOOL Detail_LongitudinalShearResistance(ElemPairK ElemK, BOOL bPosiI);
	BOOL Detail_FatigueResistance(ElemPairK ElemK, BOOL bPosiI);
	//
	BOOL Detail_Header(ElemPairK ElemK);
	BOOL Detail_SectionProperty_Material(ElemPairK ElemK, BOOL bPosiI);
	//
	//BOOL Detail_PlasticResistanceMoment_Mpl_Rd(T_PGBR_BASE& PgbrBase);


	CString Get_String_Format(double dValue, int iNum=16);
	
private:
	void Print_EffectivePolyAndLineData(T_GSEC_POLYGON& PolyD, CArray<T_GSEC_LINE, T_GSEC_LINE&>& aEffLine);
	int Get_SubPannelCalcType(int nLongStfnSize);

protected:
    void SetDgnLcomNo(int nDgnLcomNo);
    int  GetDgnLcomNo() const;
	int  GetDgnCurrentLcomIndex(int nLcom);

    T_STCT_D m_StctD;

    T_UNIT_INDEX GetCodeUnit() const ;

public:
	void SetCheckNode(int nIJ);

private:
	void SetScfpForce4Lcom(const _DGN_FORC_CRC& ForD, const _DGN_SCFP_PART_LCASE& StrD);

	int GetCheckNode();
	_DGN_FORC_CRC& GetScfpForce4Lcom();
	_DGN_SCFP_PART_LCASE& GetScfpStress4Lcom();
	double CalcStressInterpolate(int nIJ, const T_SECT_STIFFNESS& StiffD, double dy, double dz);

	_DGN_FORC_CRC m_ScfpForce;
	_DGN_SCFP_PART_LCASE m_ScfpStress;
	int m_nIJ;

private:
    void SetBendingResistanceLcomResult(const dgn::def::IResult* pResult, T_PGBR_LCOM& rData);
    void SetShearResistanceLcomResult(const dgn::def::IResult* pResult, T_PRVS_D& rData);
    void SetLateralTorsionalBucklingLcomResult(const dgn::def::IResult* pResult, T_PLTB_D& rData);
    void SetTransverseForceLcomResult(const dgn::def::IResult* pResult, T_PRTF_D& rData);
    void SetFatigueResistanceLcomResult(const dgn::def::IResult* pResult, T_PGRF_D& rData);
    void SetLongitudinalShearResistanceLcomResult(const dgn::def::IResult* pResult, T_PGLS_D& rData);
    void SetStressLcomResult(const dgn::def::IResult* pResult, T_PGBS_D& rData);
	void SetLongitudinalShearResistanceSLSLcomResult(const dgn::def::IResult* pResult, T_PLSS_D& rData);
	void SetRhocAndEffectiveStiffnessParam(const dgn::def::IResult* pResult, T_RHOC_LCOM& rData);
    
    //T_PGBR_LCOM* GetBendingResistanceLcomResult();
    //T_PRVS_D* GetShearResistanceLcomResult();
    //T_PLTB_D* GetLateralTorsionalBucklingLcomResult();
    //T_PRTF_D* GetTransverseForceLcomResult();
    //T_PGRF_D* GetFatigueResistanceLcomResult();
    //T_PGLS_D* GetLongitudinalShearResistanceLcomResult();
    //T_PGBS_D* GetStressLcomResult();
    //T_PLSS_D* GetLongitudinalShearSLSLcomResult();
	//T_RHOC_LCOM* GetRhocAndEffectiveStiffnessParam();

	void ConvertBendingRes(const T_PGBR_BASE &rIn, T_PGBR_BASE &rOut);
	void ConvertVerticalShearRes(const T_PRVS_BASE &rIn, T_PRVS_BASE &rOut);
	void ConvertLTBRes(const T_PLTB_BASE &rIn, T_PLTB_BASE &rOut);
	void ConvertTransverseForceRes(const T_PRTF_BASE &rIn, T_PRTF_BASE &rOut);
	void ConvertFatigueRes(const T_PGRF_BASE &rIn, T_PGRF_BASE &rOut);
	void ConvertLongitudinalShearRes(const T_PGLS_BASE &rIn, T_PGLS_BASE &rOut);
	void ConvertStressLcomRes(const T_PGBS_BASE &rIn, T_PGBS_BASE &rOut);
	void ConvertLongitudinalShearSLSRes(const T_PLSS_BASE &rIn, T_PLSS_BASE &rOut);
	void ConvertRhocAndEffectiveStiffness(const T_RHOC_BASE &rIn, T_RHOC_BASE &rOut);

// 	T_PGBR_LCOM*  m_pCurPgbrLcom;
// 	T_PRVS_D*    m_pCurPrvs;
// 	T_PLTB_D*    m_pCurPltb;
// 	T_PRTF_D*    m_pCurPrtf;
// 	T_PGRF_D*    m_pCurPgrf;
// 	T_PGLS_D*    m_pCurPgls;
// 	T_PGBS_D*    m_pCurPgbs;
// 	T_PLSS_D*    m_pCurPlss;
// 	T_RHOC_LCOM* m_pCurRhocLcom;

    //////////////////////////////////////////////////////////////////////////
    // 통합엔진..
 protected:
     virtual bool SetResult(const dgn::def::IResult* pResult) override;
     virtual bool GetReportInput(dgn::link::MItemInputList& vMembItemInput, dgn::link::MembInfoList& vMembInfo) override final;

public:
    virtual dgn::def::enDgnCode GetDgnCode() const override;
    virtual dgn::def::enDgn GetDgnType() const override;
    virtual dgn::def::enMemb GetDgnMembType() const override;

    virtual UINT GetLcomSize() override;

    virtual UINT GetDgnElemNo() const override final;
    virtual UINT GetDgnPosiNo() const override final;

    virtual std::wstring GetDgnCodeName() override final;
    virtual std::wstring GetDgnElemName() override final;
    virtual std::wstring GetDgnPosiName() override final;
    virtual std::wstring GetDgnLcomName() override final;
    virtual std::wstring GetDgnPosiName(int nPos) override final;
    virtual std::wstring GetDgnLcomName(CString strLcom) override final;
    virtual void GetDgnCurrentUnit(std::wstring& ForCU, std::wstring& LenCU)  override final;
	virtual void GetRptSettings(const CString& strMembTypeName, dgn::def::CRptSettings& rSettings) override final;

	virtual bool GetDesignCode(dgn::def::CDesignCode* pData) const override final;
    virtual bool GetMemberInfo(dgn::def::CMemberInfo* pData) const override final;
    virtual bool GetMemberInfoArray(dgn::def::CMemberInfoArray* pData) const override final;
    virtual bool GetSection(dgn::def::CSection* pData) const override final;
    virtual bool GetMaterial(dgn::def::CMaterial* pData) const override final;
    virtual bool GetSectionProperty(dgn::def::CSectionProperty* pData) const override final;
    virtual bool GetMaterialSteel(dgn::def::CMaterialSteel* pData) const override final;
    virtual bool GetEffectiveLength(dgn::def::CEffectiveLength* pData) const override final;
    virtual bool GetSteelGeneralSectParam(dgn::def::CSteelGeneralSectParam* pData) const override final;
    virtual bool GetBendingCoefficient(dgn::def::CBendingCoefficient* pData) const override final;
    virtual bool GetShearCoefficient(dgn::def::CShearCoefficient* pData) const override final;
    virtual bool GetSafetyFactor(dgn::def::CSafetyFactor* pData) const override final;
    virtual bool GetSeismicProperty(dgn::def::CSeismicProperty* pData) const override final;
    virtual bool GetUnbracedLength(dgn::def::CUnbracedLength* pData) const override final;
    virtual bool GetUnbracedForce(dgn::def::CForceUbln* pData) const override final;
    virtual bool GetLcomInfo(dgn::def::CLcomInfo* pData) const override final;
    virtual bool GetForce1D(dgn::def::CForce1D* pData) const override final;
    virtual bool GetPartialFactor(dgn::def::CPartialFactor* pData) const override final;
    virtual bool GetSectionShape(dgn::def::CSectionShape* pData) const override final;
    virtual bool GetMembLength(dgn::def::CMembLength* pData) const override final;
    virtual bool GetStressCheck(dgn::def::CStressCheck* pData) const override final;
    virtual bool GetMomentLoadType(dgn::def::CMomentLoadType* pData) const override final;
	virtual bool GetTorsionDesign(dgn::def::CTorsionDesign* pData) const override final;
	virtual bool GetDgnSteelOptions(dgn::def::CDgnSteelOptions* pData) const override final;
	virtual bool GetImperfectionFactor(dgn::def::CImperfectionFactor* pData) const override final;

    virtual bool UpdateShearData(const int& nDir, std::wstring& strLcb, std::wstring& strPosi, dgn::def::CDgnInput* pInp) override final;

public:
    virtual bool GetSteelCompositeSection(dgn::def::CSteelCompositeSection* pData) const override final;
    virtual bool GetMaterialConcrete(dgn::def::CMaterialConcrete* pData) const override final;
    virtual bool GetMaterialSteelHybrid(dgn::def::CMaterialSteelHybrid* pData) const override final;
    virtual bool GetForce1DArray(dgn::def::CForce1DArray* pData) const;
	virtual bool GetForce1DMember(dgn::def::CForce1DMember* pData) const;
    virtual bool GetLoadApplicationType(dgn::def::CLoadApplicationType* pData) const;
	virtual bool GetLateralTorsionalBucklingParam(dgn::def::CLateralTorsionalBuckling* pData) const;
    virtual bool GetShearConnector(dgn::def::CShearConnector* pData) const;
	virtual bool GetSteelCompositeRebar(dgn::def::CSectionRebarCoord* pData) const;
	virtual bool GetLongitudinalStiffener(dgn::def::CLongitudinalStiffener* pData) const;
	virtual bool GetTransverseStiffener(dgn::def::CTransverseStiffener* pData) const;
	virtual bool GetDamageEquivalenceFactor(dgn::def::CDamageEquivalenceFactorDef* pData) const;
	virtual bool GetDamageEquivalenceFactorMember(dgn::def::CDamageEquivalenceFactorMembDef* pData) const;
	virtual bool GetStressLimitationFactor(dgn::def::CStressLimitationFactorDef* pData) const;
	virtual bool GetStressSteelComposite(dgn::def::CStressSteelCompositeDef* pData) const;
	virtual bool GetConstrainStressComposite(dgn::def::CConstrainStressComposite* pData) const;
	virtual bool GetMemoizationKey(dgn::def::CMemoizationKey* pData) const;
	virtual bool GetCompositeSectionProperty(dgn::def::CCompositeSectionProperty* pData) const override final;

private:
	bool GetLongitudinalStiffenerType1(dgn::def::CLongitudinalStiffener* pData) const;
	bool GetLongitudinalStiffenerType2(dgn::def::CLongitudinalStiffener* pData) const;
};

#endif // !defined(AFX_DGNPLATEGIRDER_H__INCLUDED_)



