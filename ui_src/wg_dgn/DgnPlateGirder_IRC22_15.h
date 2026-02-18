// DgnPlateGirder_IRC22_15.h: interface for the CDgnPlateGirder_IRC22_15 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_DgnPlateGirder_IRC22_15_H__INCLUDED_)
#define AFX_DgnPlateGirder_IRC22_15_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"
#include "DgnSectUtil.h"
#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
#include "..\dgnengine\src\DgnCalcBase\\DgnCalcBase_CSG_Struct.h"

//////////////////////////////////////////////////////////////////////////
// Structure Define Start
//////////////////////////////////////////////////////////////////////////

struct CLASS_BASE_UNIT_IRC_15
{
	int iClass;
	int iWebDeter;
	double dPsi;	
	double dk_sig;
	double dRho	;
	double dbeff;
	double dbe1;
	double dbe2;
	double dBTR;
	double dEpsilon;
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
		dBTR	  =0.0;
		dEpsilon=0.0;
	}

	CLASS_BASE_UNIT_IRC_15() { Initialize(); }
};

struct T_CLASS_IN_UNIT_D_IRC_15
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

	T_CLASS_IN_UNIT_D_IRC_15(){ Initialize(); }

	T_CLASS_IN_UNIT_D_IRC_15(const T_CLASS_IN_UNIT_D_IRC_15& rData) {*this = rData; }
	T_CLASS_IN_UNIT_D_IRC_15& operator=(const T_CLASS_IN_UNIT_D_IRC_15& rData) 
	{
		nType			= rData.nType;
		dStressOuter	= rData.dStressOuter;
		dStressInter	= rData.dStressInter;	
		dc				= rData.dc			;	
		dt				= rData.dt			;	
		return *this;
	}
};

struct T_CLASS_IN_D_IRC_15
{
	CArray<T_CLASS_IN_UNIT_D_IRC_15, T_CLASS_IN_UNIT_D_IRC_15&> aTopFlangeD;
	CArray<T_CLASS_IN_UNIT_D_IRC_15, T_CLASS_IN_UNIT_D_IRC_15&> aBotFlangeD;
	CArray<T_CLASS_IN_UNIT_D_IRC_15, T_CLASS_IN_UNIT_D_IRC_15&> aWebD;

	void Initialize()
	{
		aTopFlangeD.RemoveAll();
		aBotFlangeD.RemoveAll();	
		aWebD.RemoveAll();
	}

	T_CLASS_IN_D_IRC_15(){ Initialize(); }

	T_CLASS_IN_D_IRC_15(const T_CLASS_IN_D_IRC_15& rData) {*this = rData; }
	T_CLASS_IN_D_IRC_15& operator=(const T_CLASS_IN_D_IRC_15& rData) 
	{
		aTopFlangeD.Copy(rData.aTopFlangeD);
		aBotFlangeD.Copy(rData.aBotFlangeD);
		aWebD.Copy(rData.aWebD);
		return *this;
	}
};

struct T_VERTICAL_SHAER_RES_UNIT_D_IRC_15
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

	T_VERTICAL_SHAER_RES_UNIT_D_IRC_15(){ Initialize(); }
	T_VERTICAL_SHAER_RES_UNIT_D_IRC_15(const T_VERTICAL_SHAER_RES_UNIT_D_IRC_15& rData) {*this = rData; }
	T_VERTICAL_SHAER_RES_UNIT_D_IRC_15& operator=(const T_VERTICAL_SHAER_RES_UNIT_D_IRC_15& rD) 
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



struct T_VERTICAL_SHAER_RES_D_IRC_15
{
	double dVbw_Rd;
	double dVbf_Rd;
	T_VERTICAL_SHAER_RES_UNIT_D_IRC_15 ShearResD[2]; // 0 : Left Web,  1 : Right Web

	void Initialize()
	{
		dVbw_Rd = 0.0;
		dVbf_Rd	= 0.0;
		for(int i = 0; i < 2; ++i){ShearResD[i].Initialize();}
	}

	T_VERTICAL_SHAER_RES_D_IRC_15(){ Initialize(); }
	T_VERTICAL_SHAER_RES_D_IRC_15(const T_VERTICAL_SHAER_RES_D_IRC_15& rData) {*this = rData; }
	T_VERTICAL_SHAER_RES_D_IRC_15& operator=(const T_VERTICAL_SHAER_RES_D_IRC_15& rD) 
	{
		dVbw_Rd = rD.dVbw_Rd;
		dVbf_Rd = rD.dVbf_Rd;
		for(int i = 0; i < 2; ++i){ShearResD[i] = rD.ShearResD[i];}
		return *this;
	}
};

struct T_LCIX_STEP
{
    T_LCIX_STEP() { lcix = step = 0; }
    T_LCIX_STEP(int lcix, int step) { this->lcix = lcix; this->step = step; }
    int lcix;
    int step;
};

union T_ELPT_K
{
    unsigned int keymap;
    struct
    {
        unsigned int elem : 20;   // 0-1048576
        unsigned int part :  6;   // 0-64
        unsigned int optn :  6;   // 0-64 (0, 1, 2)
    }key;
};
//////////////////////////////////////////////////////////////////////////
// Structure Define End
//////////////////////////////////////////////////////////////////////////

class CMembCtrl;
class CAnalysisResult;
class CDgnSectUtil;

class CDgnPlateGirder_IRC22_15 : public CDgnPscCommon
{
// Member Functions.
public:
	CDgnPlateGirder_IRC22_15();
	virtual ~CDgnPlateGirder_IRC22_15();
	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);  

protected:

	T_UNIT_INDEX m_CodeUnit;
	double m_dPi;
	// Partial Factor
	double m_dGamma_C;    // Concrete, 
	double m_dGamma_C_Acc;    // Concrete Accidental, 
	double m_dGamma_S;    // Reinforcing steel
	double m_dGamma_M0;   // Structural steel
	double m_dGamma_M1;   // Structural steel
	double m_dGamma_V;    // Shear resistance of a headed stud
	double m_dGamma_Fft;   // Equivalent constant amplitude stress range
	double m_dGamma_Mft;   // Fatigue strength


	double m_dGamma_Mfs;  // Fatigue strength of studs in shear
	double m_dGamma_M_ser;

	double m_dAlpha; // For Concrete Compression Strength
	double m_dEs;
	double m_dPoisson;

	double m_dHt[2]; // H_total
	double m_dZbar_sb[2]; // Slab 도심 
	double m_dCzm_sb[2]; // Slab
	double m_dCzp_sb[2];
	double m_dArea_sb[2];
	double m_dBc_sb[2];
	double m_aY_sb[2][4]; // x1,x2..
	double m_aZ_sb[2][4]; // y1,y2..
	double m_dPlasticNA;  

	T_SECT_STIFFNESS m_EffeStlSect;
	T_SECT_STIFFNESS m_EffeComSect;

	double m_dZero;
	BOOL m_bTestMode;
	BOOL m_bPrintMode;
	BOOL m_bIsRolledSection;//=True - Rolled section / =False - Welded Section
	int m_iClassSect;
	int    m_iType;
	double m_dH			;
	double m_dTw	;
	double m_dTw_t;
	double m_dB1	;
	double m_dtf1	;
	double m_dB2	;
	double m_dtf2	;
	double m_dHw	;	
	double m_dArea;
	double m_dPu,m_dMuy,m_dMuz,m_dRyy,m_dRzz;
	double m_dfy,m_dZbar,m_dYbar;
	double m_dBeffFactor;
	double m_dMfRd,m_dHighShearCond1,m_dHighDhearCond2,m_dBetaFactHighShear;
	double m_dpgls_shear;
	double dIRCL,m_dTotalLength;
	//IRC22 15 report related variables
	double m_dBc;
	double m_dtc	;
	double m_dHh	;
	double m_ddc;
	double m_dfck;
	double m_dfyk;
	double m_dBeffLength;
	double m_ddc1;
	double m_dZp,m_dhw,m_dHt_sectProp;
	double m_dTF1,m_dTF2;
	double m_dfcds_bs,m_dfcds_acc,m_dfyd_steel;
	double m_ddccase3;
	DOUBLE m_dEc,m_dFyk;


	BOOL bOuter,bissimplysup;
	CMap<T_COMF_KEY, T_COMF_KEY, T_CLAS_BASE, T_CLAS_BASE&> m_ClassSectMap;
	CMap<T_COMF_KEY, T_COMF_KEY, T_PGBR_BASE, T_PGBR_BASE&> m_LcomPgbrMap;  
	CMap<T_COMF_KEY, T_COMF_KEY, CPG_SECT_EFFE, CPG_SECT_EFFE&> m_EffeSectMap;  
	CMap<T_COMF_KEY, T_COMF_KEY, T_PGLS_BASE, T_PGLS_BASE&> m_LcomPglsMap;  
	CMap<T_COMF_KEY, T_COMF_KEY, T_PGBR_IRC_BASE, T_PGBR_IRC_BASE&> m_LcomPgbr_IRC_Map;  
	CMap<T_SECT_K,T_SECT_K,CSG_TRST_SECT,CSG_TRST_SECT&> m_mCsgTrst;

	T_CLAS_BASE m_ClassBase;
	T_CLAS_BASE m_BeforeClassBase;

	CMembCtrl* m_pMembCtrl;
	CDgnSectUtil *m_pDgnSectUtil;
	T_CPGD_D m_CpgdD;

	CArray<int,int&> m_aWebLineId;
	CArray<int,int&> m_aTopFlgLineId;
	CArray<int,int&> m_aBotFlgLineId;

public:
	
	BOOL Make_ClassSection(ElemPairK ElemK, T_PGBS_D& PgbsD, T_PGBR_LCOM& PgbrLcomD);
	BOOL Make_ClassInD(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D_IRC_15& ClassInD);
	BOOL Make_ClassInD_I(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D_IRC_15& ClassInD);
	BOOL Make_ClassInD_Box(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D_IRC_15& ClassInD);
	BOOL Make_ClassInD_Tub(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_SECT_ELEM& rSectD, T_CLASS_IN_D_IRC_15& ClassInD);
	BOOL Get_StageForce(CArray<T_ELEM_K,T_ELEM_K>& aElemK);
	BOOL Get_BendingResistanceForPlateGirder(ElemPairK ElemK, T_PGBR_LCOM& PgbrLcomD);
	BOOL Get_VerticalShearResistanceForPlateGirder(ElemPairK ElemK, T_PRVS_D& PrvsD);
	BOOL Get_LateralTorsionalBucklingResistanceForPlateGirder(ElemPairK ElemK, T_PLTB_D& PltbD);
	BOOL Get_TransverseForceResistanceForPlateGirder(ElemPairK ElemK, T_PRTF_D& PrtfD);
	BOOL Get_LongitudinalShearResistanceForPlateGirder(ElemPairK ElemK, T_PGLS_D& PglsD);
	BOOL Get_FatigueResistanceForPlateGirder(ElemPairK ElemK, T_PGRF_D& PgrfD, double dt_Ld);
	BOOL Get_BeamStressResistanceForPlateGirder(ElemPairK ElemK, T_PGBS_D& PgbsD);
	BOOL Get_BendingResistanceForPlateGirder(ElemPairK ElemK, T_PGBR_IRC_LCOM& PgbrLcomD);
	double Calc_HTR(); // web width-to-thickness ratio
	double Calc_BTR(); // flenge width-to-thickness ratio
	  double    Calc_Class_General();
		double    Calc_Class_General(int &iClassF,int &iClassW);
		double    Calc_Class_General(int &iClassF,int &iClassW,double &dFy,double &dBTR);
		double   Calc_Class_P_SR();
		double Calc_BTR_HC1();
		double Calc_BTR_HC2();
		double Calc_BTR_HC3();
	// For Design Result Print
	BOOL Get_PlateGirderDgnResult(BOOL* abChkItem);
	BOOL Get_BendingResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGBR_BASE& PgbrGd, const T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);
	BOOL Get_VerticalShearResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PRVS_BASE& PrvsGd, const T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	BOOL Get_LateralTorsionalBucklingResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PLTB_BASE& PltbGd, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail,BOOL bspcl=FALSE);
	BOOL Get_TransverseForceResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail);
	BOOL Get_LongitudinalShearResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGLS_BASE& PglsGd, const T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail);
	BOOL Get_FatigueResistanceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGRF_BASE& PgrfBase, T_PGRF_DETAIL& PgrfDetail);
	BOOL Get_BeamStressDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail);
	BOOL Get_LongitudinalShearResistanceServiceDetailResult4PlateGirder(ElemPairK ElemK, BOOL bPosiI, const T_PLSS_BASE& PlssGd, const T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail);
	BOOL Get_SavedResult4Design(ElemPairK ElemK, BOOL bPosiI, const T_PGBR_BASE& PgbrGd,T_PGBR_IRC_BASE& PgbrIRCGd);
	BOOL Get_Elastic_Steel_Results(double bf_top,double tw,double bf_bottom,double tf_top, double dw,double tf_bottom,T_PG_IRC22_15_ELP& SectProp );
	BOOL Get_Plastic_Steel_Results(double bf_top,double tw,double bf_bottom,double tf_top, double dw,double tf_bottom,T_PG_IRC22_15_ELP& SectProp );
	BOOL Get_Short_Term_Composite_Results(double Equ_width_steel_trans,double bf_top,double tw,double bf_bottom,double dtc,double tf_top, double dw,double tf_bottom,double dModular_ratio_Trans,double slab_width,T_PG_IRC22_15_ELP& SectProp );
	BOOL Get_Transverse_Stiffener_Sect_Prop(double dL[5],double dD[5],double dYy[5],double& dA1,double& dA2,double& dA3,double& dIyy1,double& dIyy2,double& dIyy3);
	double Get_ddcfromybar(double bf_top,double tw,double bf_bottom,double tf_top, double dw,double tf_bottom,T_PG_IRC22_15_ELP& SectProp );

	BOOL Get_Long_Term_Composite_Results(double Equ_width_steel_pre,double bf_top,double tw,double bf_bottom,double dtc,double tf_top, double dw,double tf_bottom,double dModular_ratio_per,double slab_width,T_PG_IRC22_15_ELP& SectProp );

	BOOL Get_Cracked_Results(double bf_top,double tw,double bf_bottom,double dtc,double tf_top, double dw,double tf_bottom,double dModular_ratio_Trans,double slab_width,double dTopDia,double dTopNo,double dBotNo,double dBotSpac, double dTopSpac, double dTopCover,double dHh,double d_girder,double dBotDia,T_PG_IRC22_15_ELP& SectProp );
	BOOL Set_Forc24(int iPosiNo, _DGN_FORC_CRC& ForcCrc2, _DGN_FORC_CRC& ForcCrc4);
	BOOL Cng_DgnForcCrc(_DGN_FORC_CRC& ForcCrc);

	void Set_PrintMode(BOOL bPrint);
	BOOL IsPrintMode();
	double interpolate(double x1, double x2, double p1, double p2, double x);
	// Calculation
	//BOOL Get_BendingResistance(BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CPG_SCON_ELEM& SconElem, T_PGBR_BASE& PgbrBase);

	
protected:
	BOOL Get_DesignResultExist(BOOL* abChkItem);
	void InitPartialFactor();
	BOOL Get_ClassifyClassSection(BOOL bPosiI, double aGirderForce[6], double aCptForce[6], CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CPG_SCON_ELEM& SconElem, 
																const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, T_DLINE_CALC_D& DLineCalcD, T_CLAS_BASE& ClassD, ElemPairK ElemK);
	BOOL Get_SectionClass(bool bPosiI,BOOL bChkRestrained, const T_SECT_D& SectD, const T_CLASS_IN_D_IRC_15& crClassInD, const T_MATL_DESIGN& DgnMatD, T_CLAS_BASE& ClassD);
	BOOL Calc_OutstandClassUnit(const T_CLASS_IN_UNIT_D_IRC_15& ClassInD, double dfy, double dfy1, int& iClass,CLASS_BASE_UNIT_IRC_15& ClassBase);
	BOOL Get_InternalWebClassClassification(bool bPosiI,int iBotClass, double aForce[6], double dStress[4], CPG_MATL_ELEM& MatlElem, const T_SECT_D& crSectD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, int& iClass, T_CLAS_BASE& ClasBase);
	BOOL Calc_InternalPlateClassUnit(bool bPosiI, const T_SECT_D& SectD,const T_CLASS_IN_UNIT_D_IRC_15& ClassInD, double dfy, double dfy1,int& iClass, CLASS_BASE_UNIT_IRC_15& ClassBase, bool bWeb = true);
	BOOL Calc_ClassifyClassSection4GenSect(BOOL bChkRestrained, double dfy, double dfy1,double dfy3, double* aGirderForce, double* dForce, double dPlasticNA, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, T_DLINE_CALC_D& DLineCalcD);
	BOOL Calc_PartLineClassSection4GenSect(double dfy, double dfy1,double dfy3, int iBotClass, double* aGirderForce, double* dForce, double dPlasticNA, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, T_DLINE_PART_D& DLinePartD);
	BOOL Calc_SubLineClassSection4GenSect(int nPartType, double dfy, double dfy1,double dfy3, int iBotClass, double* aGirderForce, double* dForce, double dPlasticNA, T_SECT_STIFFNESS& GirStiffD, T_SECT_STIFFNESS& CptStiffD, T_SECT_STIFFNESS& OrgStiffD, const T_GENL_DATA& GLineData, const T_GLINE_DST& LineDst, T_DGNLINE_LBD& DLineLbd, int& iClass);
	BOOL Is_Sig1IsFree4Outstand(BOOL bIs1_Sig1);
	BOOL Get_SectionClass(bool bPosiI,BOOL bChkRestrained, const T_SECT_D& SectD, const T_CLASS_IN_D_IRC_15& crClassInD, const T_MATD_D& DgnMatD, T_CLAS_BASE& ClassD);

	void ConvertCsgCalcCgtsData(T_CGTS_SSM_D *pCgtsSSM, CSG_TRST_SECT &rData);
	void ConvertCsgCalcCgtsPosData(T_CGTS_SUB_SSM &CgtsSub, CSG_TRST_POSD &rData);

	double Get_OutstandBucklingfactor(int iType, double dPsi);  
	double Get_InternalBucklingfactor(double dPsi);
	int Get_PartClass(int iStreType, double dct, double dEpsi, double dAlpha, double dk);
	int Get_InternalPartClass(BOOL bChkPlastic, int iStreType, double dct, double dEpsi, double dAlpha, double dPsi);
	int Get_WebPlaticClass(double dct, double dEpsi, double dAlpha);
	int Get_WebElasticClass(double dAlpha, double dct, double dEpsi, double dPsi);
	BOOL Get_ReductionFactorRho(BOOL bInternal, double dBar_b, double dt, double dLamda_p, double dPsi, double& dRho);
	double Get_Lamda_p(double db, double dt, double dEpsilon, double dk_sig);

	BOOL Get_PlasticNA_inclSectclass(BOOL bPosiI, BOOL bPositive,double dL, CPG_MATL_ELEM& MatlElem, const T_SECT_D& crSectD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, double& dPlasticNA, int iSectClass, BOOL bNeglectWeb=FALSE);
	BOOL Get_PlasticNA(BOOL bPosiI, BOOL bPositive,double dL, CPG_MATL_ELEM& MatlElem, const T_SECT_D& crSectD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, double& dPlasticNA, BOOL bNeglectWeb=FALSE);
	BOOL Get_PlasticNA4GenSect(BOOL bPosiI, BOOL bPositive, CPG_MATL_ELEM& MatlElem, T_SECT_SECTBASE_D& SectBaseD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, double& dPlasticNA);
	BOOL Get_PlasticNAEff(BOOL bPosiI, BOOL bPositive, CPG_MATL_ELEM& MatlElem, const T_SECT_D& crSectD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, double dPlasticNAxis, double& dEffPlasticNA, double& dEffeLen, double& dHoleLen);
	BOOL Get_EffectiveSectionProperty(BOOL bPosiI, double dMy, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, T_CLAS_BASE& ClassD, T_SECT_STIFFNESS& EffeStlSect, T_SECT_STIFFNESS& EffeComSect);
	BOOL Get_EffectiveSectionProperty4DB(BOOL bPosiI, double* aForce, double da, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,
																			 T_CLAS_BASE& ClassD, T_SECT_STIFFNESS& EffeStlSect, T_SECT_STIFFNESS& EffeComSect, T_DGNLINE_LBD_BR& RhocBefore, T_DGNLINE_LBD_BR& RhocAfter, BOOL bCompoStiffData=FALSE);
	//
	BOOL Get_EffeSectPropBeforeDB(bool bPosiI, double dfy1, double dfy2,double dfy3, double* aForce, T_SECT_D& SectD, double da, T_SECT_STIFFNESS& GirderStiff, 
									CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, T_SECT_STIFFNESS& EffeStiff, T_DGNLINE_LBD_BR& RhoC);
	BOOL Get_EffeSectPropAfterDB(BOOL bPosiI, double dfy1, double dfy2,double dfy3, double* aForce, double da, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, T_SECT_STIFFNESS& EffeStiff, T_DGNLINE_LBD_BR& RhoC, BOOL bCompoStiffData=FALSE);
	BOOL Get_EffectiveCzmData4Sect(bool bPosiI, BOOL bPositive, T_SECT_D& SectD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, T_DGNLINE_LBD& LineLBD, double& dA_eff, double& dCzm_eff, double& dIy_eff);
	BOOL Get_LineDgnInfo4SectDB(bool bPosiI, double dfy1, double dfy2,double dfy3, double dEs, double dPoisson, double* aForce, T_SECT_D& SectD, double da, T_SECT_STIFFNESS& AppStiff, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&>& aLineDgnBase, T_DGNLINE_LBD& LineLBD);
	BOOL Get_LineInfo4SectDB(bool bPosiI, T_SECT_D& SectD, T_SECT_STIFFNESS& AppStiffD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&>& aLineDgnBase);
	BOOL Get_ConvertPoint4Centroid(double dCym, double dCzm, CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&>& aLineDgnBase);
	//
	BOOL Get_StiffnessOfEffeSectCompo(T_SECT_INFO_D& SectInfoD, T_SECT_STIFFNESS& EffeStiff);
	
	BOOL Calc_BendingResistance(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,
															CPG_SCON_ELEM& SconElem, CPG_TRST_POSI& TrstPosi, T_CLAS_BASE& ClassD, T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);
	BOOL Get_BetaReductionFactor(double dXpl, double dh, double& dBeta);
	BOOL Calc_NonLinearResistance(ElemPairK ElemK, BOOL bPosiI, T_PGBR_BASE& PgbrBase);
	BOOL Calc_PlasticResistance(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, T_PGBR_BASE& PgbrBase, double dPlasticNA, double& dMpl_Rd, BOOL bNeglectWeb=FALSE);
	BOOL Calc_ElasticResistance(ElemPairK ElemK, BOOL bPosiI, BOOL bPositive, double aForceGd[6], double aForce[6], CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,
															CPG_SCON_ELEM& SconElem, CPG_TRST_POSI& TrstPosi, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);

	double Calc_kmin(BOOL bPositive, double dk_rbar, double dk_slab, double dk_top, double dk_bot);

	BOOL Calc_VerticalShearResistance(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
																		CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,                                 
																		T_CLAS_BASE& ClassD, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail,T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);

	BOOL Calc_LateralTorsionalBucklingResistance(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
																							CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,
																							CPG_LTBD_POSI& LtbdPosi, CPG_MEMB_POSI& MembPosi,
																							T_CLAS_BASE& ClassD, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail);
	BOOL Calc_LateralTorsionalBucklingResistance_Neg(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
																							CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,
																							CPG_LTBD_POSI& LtbdPosi, CPG_MEMB_POSI& MembPosi,
																							T_CLAS_BASE& ClassD, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail,double dmaed);

	BOOL Calc_TransverseForceResistance(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
																			CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, CPG_BCDT_POSI& BcdtPosi,
																			T_CLAS_BASE& ClassD, T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail,CPG_SCON_POSI& SconPosi);

	BOOL Calc_LongitudinalShearResistance(ElemPairK ElemK, BOOL bPosiI, BOOL bSLS, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CPG_SCON_POSI& SconPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
																		CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,                                 
																		T_CLAS_BASE& ClassD, double dks, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail);

	BOOL Calc_FatigueResistance(ElemPairK ElemK, BOOL bPosiI, double dt_Ld, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CPG_SCON_POSI& SconPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
															CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, CPG_FRDT_POSI& FrdtPosi,
															T_CLAS_BASE& ClassD, T_PGRF_BASE& PgrfBase, T_PGRF_DETAIL& PgrfDetail);

	BOOL Calc_StressCheck4SLS(ElemPairK ElemK, BOOL bPosiI, BOOL bIsGenSect, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
														const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, double dk1, double dk2, double dk3, T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail);
	
	BOOL Calc_LongitudinalShearResistance4SLS(ElemPairK ElemK, BOOL bPosiI, BOOL bIsGenSect, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CPG_SCON_POSI& SconPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
																						CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,                                 
																						T_CLAS_BASE& ClassD, double dks, T_PLSS_BASE& PlssGd, T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail);
	
	// Gen. Section
	BOOL Calc_BendingResistance4GenSect(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CPG_TRST_POSI& TrstPosi,
																			T_CLAS_BASE& ClassD, const T_GENL_DATA& GLineData, T_GLINE_DGN_D& GLineDgn, T_PGBR_BASE& PgbrGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);

	BOOL Calc_VerticalShearResistance4GenSect(ElemPairK ElemK, BOOL bPosiI, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,
																						T_CLAS_BASE& ClassD, const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);

	BOOL Calc_LongitudinalShearResistance4GenSect(ElemPairK ElemK, BOOL bPosiI, BOOL bSLS, CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem, CPG_SCON_POSI& SconPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
																								CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi,                                 
																								T_CLAS_BASE& ClassD, double dks, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail);

	BOOL Get_PlasticMoment4GenSect(BOOL bPosiI, BOOL bPositive, CPG_MATL_ELEM& MatlElem, T_SECT_SECTBASE_D& SectBaseD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, double dPlasticNA, double& dMpl_Rd, T_PGBR_DETAIL& PgbrDetail);
	BOOL Get_Mf_Rd_PlasticMoment4GenSect(BOOL bPosiI, BOOL bPositive, CPG_MATL_ELEM& MatlElem, T_SECT_SECTBASE_D& SectBaseD, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, const T_GENL_DATA& GLineData, T_GLINE_DGN_D& GLineDgn, double& dMf_Rd);
	BOOL Get_PlasticMomentOfDividedPart(double dfy1, double dfy2, double dfy3,double dCzm_NAz, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLineData, double& dMpl, double& dNgir);
	BOOL Get_PlasticMomentOfHoriDividedLine(double dfy1, double dfy2,double dfy3, double dCzm_NAz, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLineData, const CArray<double, double&>& aCutTopThick, double& dMpl_top, double& dMpl_bot, double& dNgir_top, double& dNgir_bot);
	BOOL Get_ElasticResistance4GenSect(ElemPairK ElemK, BOOL bPosiI, BOOL bPositive, double aForceGd[6], double aForce[6], CPG_MATL_ELEM& MatlElem, CPG_SECT_ELEM& SectElem,
																		 CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CPG_TRST_POSI& TrstPosi, const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);
	double Get_Mel_RdOfElastResistance(double dfy, double dFx, double dMy, double dA, double dIy, double dz);

	BOOL Get_EffectivePropertyForClass4(ElemPairK ElemK, BOOL bPosiI, BOOL bPositive, const T_SECT_SECTBASE_D& SectBase, T_GENL_DATA& EffGLineD, T_SECT_STIFFNESS& GirderEffStiff, T_SECT_STIFFNESS& CompoEffStiff);

	double Get_Epsilon(double dfy);
	BOOL Get_IslOfLongiStiff(int iType, double dh, double dt, double dtw, double dEpsi, double& dIsl);
	BOOL Get_IslOfLongiStiff(const CPG_LSTI_DATA& InD, double dtw, double dEpsi, double& dIsl);
	BOOL Get_IslOfLongStfn4GS(double dEpsi, T_GLINE_STIFFENER& StfnD, double dtw, double& dIsl);
	BOOL Get_SlendernessParamLamda(int iType, int iLongStiffSize, double dhw, double da, double dt, double dIsl, double dEpsi, double& dk_tau, double& dk_tau_st, double& dk_tau_st_Limit, double& dLamda);
	BOOL Get_ShearBucklingCoeff(int iLongStiffSize, double dhw, double da, double dt, double dIsl, double& dk_tau, double& dk_tau_st, double& dk_tau_st_Limit);
	BOOL Get_ContributionFromWebFactor(int iEndpostType, double dEta, double dLamda, double& dXw, int& iLamda_w_scope,double dfyw);
	BOOL Get_bf4c_DB(bool bPosiI, T_SECT_D& SectD, bool bTopF, int nWebPart, double dEpsilon, double& dbf); // nWebPart : 0 : Left Web,  1 : Right Web
	//
	double Get_ImperfectionFactor(int iCase);
	int Get_BucklingCurveForCrossSection(double dh, double db, BOOL bIsSectI = TRUE);
	BOOL Get_NonDimensionalSlenderness(double dL, double db, double dfy, double dEs, double dm, double dAwc, double dAf, double& dLamda);
	double Get_PHIform(double dM1, double dM2, double dNu);
	BOOL Get_m_Value(double dNu, double dPHI, double dgamma, double& dm1, double& dm2, double& dm_min, double& dm);
	double Get_PHI_LT(double dAlpha, double dLamda_LT);
	double Get_X_LT(double dPHI_LT, double dLamda_LT, double& dX_LT_org);
	double Get_Lamda3_DesignLifeInYears(int iYears); 
	BOOL Get_Eta4SteelMaterial(int iMatClass, double dgam0, double dgam1, double& dEta);
	double Get_Eta4SteelGrade(int nMatClass, double dFy1);
	double Get_Zj_Second_Value(double dH, double dHw, double dtw, double dB1, double dtf1, double dB2, double dtf2, double dCzm, double dAa, double dIay);
	double Get_ShearCenter_H(double dh, double dt1, double db1, double dt2, double db2);
	double Get_fy4Thick(double dfy, double dfy1,double dfy2, double dt);
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

	BOOL Get_StressOfPosition(double aGirderForce[6], double aCptForce[6], T_SECT_STIFFNESS& GdStiffD, T_SECT_STIFFNESS& StiffD, double aStress[4]);

	BOOL Get_CalcIRCData(ElemPairK ElemK, BOOL bPosiI,int iDgnSelLcomNo,T_PGBR_IRC_BASE& retPgbr_IRC);
	// General Section
public:
	BOOL Get_CalcSectInfo4EffeSect(double dfy1, double dfy2,double dfy3, double da, double* dForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_DGN_D& GLineDgn, T_DLINE_CALC_D& DlineInfo);
	BOOL Get_CalcSectPartLine4EffeSect(double dfy1, double dfy2,double dfy3, double dE, double dPoisson, double da, double* dForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, T_DLINE_PART_D& DlinePart);  
	BOOL Get_LocalBucklingDataOfSubPanel(double dfy1, double dfy2,double dfy3, double dE, double dPoisson, double da, double* dForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_DST& LineDst, 
																			 T_DGNLINE_LBD& LineLBD);
	BOOL Get_EffectiveSectData(BOOL bPosiI, BOOL bAfter, double dfy1, double dfy2,double dfy3, double da, double* dForce, CPG_SECT_ELEM& SectElem, const T_GENL_DATA& GLineData, T_DLINE_CALC_D& DLineInfo, T_GENL_DATA& EffGLineD, T_GLINE_DGN_D& EffGLineDgn);
	BOOL Get_EffectiveSectInfo4PartLine(double* aForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, const T_DLINE_PART_D& DLinePart, T_GENL_DATA& EffGLineD);  
	BOOL Get_EffectiveSectInfo4PlateElements(int iClass, double* aForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, T_GLINE_DST& GLineDST, T_DGNLINE_LBD& LineLBD, T_GENL_DATA& EffGLineD);  
	BOOL Get_EffectivePanelInformation(int iClass, BOOL bInternal, int iStressType, double* aForce, double dCzm_gap, double dStiff[3], const T_GENL_DATA& GLineData, const T_GLINE_BASE& LineBaseD, const T_GLINE_DGN_B& LineDgnB,
																		 T_GENL_DATA& EffLineD);
	BOOL Get_ConvertVertexId(int vs, int v1, int v2, int& vc1, int& vc2);
	BOOL Get_ConvertSectStiffenerD(const T_DGN_STL_STIFFENER_PART& InD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi);
	//
	
	BOOL Get_EffectiveSect4PartLine(double dfy1, double dfy2, double dfy3,double* dForce, double dArea, double dIy, double dIz , const T_GENL_DATA& GLineData, const T_GLINE_PART_DGN& PartDgnD, T_GENL_DATA& EffGLineD, T_GLINE_PART_DGN& EffPartDgnD);  
	BOOL Get_EffectiveWidth4Internal(double dPsi, double db, double dRho, double& dbeff, double& dbe1, double& dbe2);
	BOOL Get_EffectiveWidth4Outstand(int nType, double dPsi, double dc, double dRho, double& dbeff);
	BOOL Get_EffectiveSection4StiffenedPlateElements(double dfy1, double dfy2, double dfy3,double* dForce, double dArea, double dIy, double dIz, const T_GENL_DATA& GLineData, const T_GLINE_DST& LineDst, T_GENL_DATA& EffGLineD, T_GLINE_DST& EffLineDst);  
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
	BOOL Get_Nc_fofSlab(BOOL bPoisI, double dfcd, double dXpl, double dHt, T_SECT_SECTBASE_D& SectBaseD, const T_GLINE_DGN_D& GLineDgn, double& dNc_f);
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

	void SetPgbrBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PGBR_BASE& rPgbrGd, T_PGBR_BASE& rData , T_PGBR_IRC_BASE& rPgbrIRCGd, T_PGBR_IRC_BASE& rDataIRC);
	void SetPrvsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PRVS_BASE& rPrvsGd, T_PRVS_BASE& rData);
	void SetPltbBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, double dV1, double dV2, double dM1, double dM2, double dC4, T_PLTB_BASE& rPltbGd, T_PLTB_BASE& rData);
	void SetPrtfBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForce, T_PRTF_BASE& rData);
	void SetPglsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PGLS_BASE& rPglsGd, T_PGLS_BASE& rData);
	void SetPgfrBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForce, T_PGRF_BASE& rData);
	void SetPgbsBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, int iLcomType, double* aForceGd, double* aForce, T_PGBS_BASE& rData);
	void SetPlssBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, int iLcomType, double* aForceGd, double* aForce, T_PLSS_BASE& rPlssGd, T_PLSS_BASE& rData);
	//void SetPgIRCData(BOOL bChk, T_ELEM_K ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, double* aForceGd, double* aForce, T_PGBR_BASE& rPgbrGd, T_PGBR_BASE& rData,T_PG_IRC22_15& rPgIRCdata);

	// 845 추가
	BOOL SetRhocBaseData(BOOL bChk, ElemPairK ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMaxMinType, const T_PGBR_DETAIL& PgbrDetail, T_RHOC_BASE& Rho_C);
	BOOL Get_MaterialData(ElemPairK ElemK, double& dfy, double& dfy1, double& dEs, double& dfck, double& dEcm, double& dfsk, double& dErs);
	BOOL Get_GLineDgnPartData(const T_GLINE_DGN_D& DLineInfo, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aTopFlangePart, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aBotFlangePart, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aWebPart);
	BOOL Get_DLinePartData(const T_DLINE_CALC_D& DLineInfo, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aTopFlangePart, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aBotFlangePart, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aWebPart);
	BOOL Get_LineEndStress4LineDgnBase(const CArray<T_GLINE_DGN_B,T_GLINE_DGN_B&>& aDgnBaseD, double& dStr1, double& dStr2);    
	BOOL Get_ForceLineGS(double dfy1, double dfy2,double dfy3, const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLine, double& dTotalForce);
	BOOL Get_TopBotForceLineGS(double dfy1, double dfy2, double dfy3,const T_GSEC_POLYGON& PolyD, const CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLine, const CArray<double, double&>& aTopThick, double& dTopForce, double& dBotForce);
	BOOL Get_PartLineId(int nPartId, const T_GLINE_DGN_D& GLineDgnD, CArray<int,int&>& aWebLineId);  // nPartId, 0=Top, 1=Bottom, 2=Web

	//IRC22 15 functions
	BOOL SetSectionPropData(ElemPairK ElemK,const T_SECT_D SectD,CPG_SECT_ELEM& SectElem,T_MATD_D& MatdD);
	BOOL CalEtaLambda(double dfck,double& dEta,double& dLambda);

protected:

	CArray<int, int&> m_aEffLineIx;
	T_PGBR_BASE m_PgbrBase;
	T_PGBR_DETAIL m_PgbrDetail;
	T_PGLS_BASE m_PglsBase;
	T_DLINE_CALC_D m_DLineCalcD;
	T_PGBR_IRC_BASE m_Pgbr_IRC_Base;
 
	BOOL Get_CompositeGeneralSectionInfo(BOOL bPosiI, const T_SECT_D& SectD, double& dZbar_sb, double& dCzm_sb, double& dCzp_sb, double& dHt, double& dArea_sb, double& dBc_sb, double* aYcr=NULL, double* aZcr=NULL);
	double Get_Height4Gen(BOOL bPosiI);
	double Get_ZcentroidofSlab(BOOL bPosiI);
	double Get_CzmofSlab(BOOL bPosiI);
	double Get_CzpofSlab(BOOL bPosiI);
	double Get_AreaofSlab(BOOL bPosiI);
	double Get_BcofSlab(BOOL bPosiI);
	void Set_LineDgnBase(int iLineType, double db, double dt, double dy1, double dz1, double dy2, double dz2, double df1, double df2,
											 BOOL bCompStr, BOOL bDbCurve, BOOL bIs1_sig1, double dy0, double dz0, T_GLINE_DGN_B& LineDgnBase);
	
	BOOL Calc_Vb_Rd(bool bPosiI,CPG_MATL_ELEM& MatlElem, T_SECT_D& SectD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,  
					CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, T_PRVS_BASE& PrvsBase, T_CLAS_BASE& ClassD, double& dVb_Rd, 
					T_VERTICAL_SHAER_RES_D_IRC_15& ResD);
	BOOL Calc_Vbw_Rd(bool bPosiI,CPG_MATL_ELEM& MatlElem, T_SECT_D& SectD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi,  
					CPG_TRST_POSI& TrstPosi, CPG_TSES_POSI& TsesPosi, double& dVbw_Rd, T_VERTICAL_SHAER_RES_D_IRC_15& ResD);
	BOOL Calc_Vbf_Rd(bool bPosiI,T_MATD_D& MatdD, T_SECT_D& SectD, T_PRVS_BASE& PrvsBase, T_CLAS_BASE& ClassD, double& dVbf_Rd, 
					T_VERTICAL_SHAER_RES_D_IRC_15& ResD);
	BOOL Calc_Lambda_w(bool bPosiI, CPG_MATL_ELEM& MatdD, T_SECT_D& SectD, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi, CPG_TRST_POSI& TrstPosi, 
					CPG_TSES_POSI& TsesPosi, double& Lambda_w, T_VERTICAL_SHAER_RES_UNIT_D_IRC_15& ResD);
	
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
	int	 GetRealSectType(const T_SECT_D& SectD);
	void GetPureSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData);
	void GetNonComSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData);
	void GetRebarSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData);

	double GetOutStandFlgLineLengD(BOOL bTop, BOOL bLeft, bool bPosiI, const T_SECT_D& crSectD);
	BOOL GetCentFlgLinePosD(bool bPosiI, BOOL bTop, BOOL bLeft, const T_SECT_D& crSectD, const T_SECT_STIFFNESS& crStiffD, 
									double& dy1, double& dz1, double& dy2, double& dz2);
	BOOL GetCentWebLinePosD(bool bPosiI, BOOL bLeft, const T_SECT_D& crSectD, const T_SECT_STIFFNESS& crStiffD, 
									double& dy1, double& dz1, double& dy2, double& dz2);
	double GetPlasticNA();
	void SetPlasticNA(const double& dPlasticNA);

	void Convert_VerbShearResD(const T_VERTICAL_SHAER_RES_UNIT_D_IRC_15& InD, T_PRVS_BASE_UNIT& rData);
	// 임시 계산서 생성. 
public:
	BOOL Execute_Detail_SteelPlateGirder(ArrElemPairKey& arElemK, BOOL* bPgdChk);
	void   Calc_r1r2(double& dr1, double& dr2);
	void   Calc_StressWeb_H(double& dStrTop, double& dStrBot);
	// Member Variable
	double	m_MaxFxx, m_MaxFyy, m_MaxFzz, m_MaxBMx, m_MaxBMy, m_MaxBMz, m_MaxDsp;
	double	m_Fxx[5], m_FxxD[5], m_FxxL[5], m_FxxR[5], 
		m_Fyy[5], m_FyyD[5], m_FyyL[5], m_FyyR[5], 
		m_Fzz[5], m_FzzD[5], m_FzzL[5], m_FzzR[5],
		m_BMx[5], m_BMxD[5], m_BMxL[5], m_BMxR[5],
		m_BMy[5], m_BMyD[5], m_BMyL[5], m_BMyR[5],
		m_BMz[5], m_BMzD[5], m_BMzL[5], m_BMzR[5];
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

    CStringArray        m_aPartName;
    CArray<int, int>    m_aPartData;
    CStringArray        m_aLoadName;
    CArray<UINT, UINT>  m_aLoadData;
    CArray<UINT, UINT>  m_aLkindData;
    CArray<int, int>    m_aMinMaxData;
    CStringArray        m_aItemName;
    CArray<int, int>    m_aItemData;

    CArray<T_FSUM_K, T_FSUM_K> m_aKey;
    CArray<T_FSUM_D, T_FSUM_D> m_aData;
    CArray<UINT, UINT> m_aLoad;   // Load Combination or Load Key
    CArray<UINT, UINT> m_aLkind;  // m_aLoad, m_aLkind pair, load type
    CArray<int, int>   m_aMinMax;   // -1:min, 0:normal, 1:max  
    CArray<T_LCIX_STEP, T_LCIX_STEP&> m_aLoadStep;
    CArray<double, double> m_aValue[2];
    void GetShear_Rslt_FSUM(ElemPairK ElemK,T_LCOM_K nStrnKey);
    // 단면력 집계표에서는 Stage나 Step이 존재하는 LC는 모두 빼준다.
    // 즉 CS, PO, NL는 빠지게 된다.
    void GetAllLoadcase(CStringArray &aLoadName, CArray<UINT, UINT> &aLoadData, 
        CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData);
    //void GetAllCsld(BOOL bStageMode, BOOL bFinalStage, CStringArray &aLoadName, 
    //   CArray<UINT, UINT> &aLoadData, CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData);
    void GetAllStld(BOOL bStageMode, BOOL bFinalStage, CStringArray &aLoadName, 
        CArray<UINT, UINT> &aLoadData, CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData);
    void GetAllSplc(CStringArray &aLoadName, CArray<UINT, UINT> &aLoadData, 
        CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData);
    void GetAllEslc(CStringArray &aLoadName, CArray<UINT, UINT> &aLoadData, 
        CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData);
    void GetAllThis(CStringArray &aLoadName, CArray<UINT, UINT> &aLoadData, 
        CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData);
    void GetAllMvld(CStringArray &aLoadName, CArray<UINT, UINT> &aLoadData, 
        CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData);
    void GetAllSmlc(CStringArray &aLoadName, CArray<UINT, UINT> &aLoadData, 
        CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData);
    void GetAllGilc(CStringArray &aLoadName, CArray<UINT, UINT> &aLoadData, 
        CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData);
    void GetAllLcom(BOOL bStageMode, BOOL bFinalStage, CStringArray &aLoadName, 
        CArray<UINT, UINT> &aLoadData, CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData);
    void GetAllEnvl(BOOL bStageMode, BOOL bFinalStage, CStringArray &aLoadName, 
        CArray<UINT, UINT> &aLoadData, CArray<UINT, UINT> &aLkindData, CArray<int, int> &aMinMaxData); 
	
private:
	void Print_EffectivePolyAndLineData(T_GSEC_POLYGON& PolyD, CArray<T_GSEC_LINE, T_GSEC_LINE&>& aEffLine);
	int Get_SubPannelCalcType(int nLongStfnSize);
	void		Get_StaticForceMoment(int ElemNo, int LcaseNo, T_LCOM_D_UL& LcomUlData);
};

#endif // !defined(AFX_DgnPlateGirder_IRC22_15_H__INCLUDED_)



