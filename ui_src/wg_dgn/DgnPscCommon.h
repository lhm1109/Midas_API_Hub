// DgnPscCommon.h: interface for the CDgnPscCommon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNPSCCOMMON_H__E5183771_6C1E_4BED_96C9_7C96278A0370__INCLUDED_)
#define AFX_DGNPSCCOMMON_H__E5183771_6C1E_4BED_96C9_7C96278A0370__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnStruct.h"
#include "CRCDataCtrl.h"
#include "CRCForceCtrl.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"

#include "..\dgnengine\src\DgnBase\DgnBase_CodeStruct.h"
#include "..\dgnengine\src\DgnBase\DgnBaseShape_Struct.h"
#include "..\dgnengine\src\CVL_DgnCalc_CH\DgnCalc_CH_PSC_Struct.h"

//#include "..\wg_db\DB_ST_DT_SECT.h"
//#include "..\wg_db\DB_ST_DT_ELEM.h"

struct DGN_SPSC_SECT;
struct DGN_RBAR_POSI;
struct DGN_RBAR_CRC;

struct S_PSCD_D;
struct PSC_MATL_ELEM;
struct PSC_TDNA_INFO;
struct PSC_TNDN_PTIF;
struct PSC_SECT_POSI;
struct PSC_RSHR_POSI;
struct PSC_TNDN_POSI;
struct DGN_RPSC_SBAR;
struct DGN_TNDN_CRC;
struct PSC_TNDN_TDNA;
struct PSC_TNDN_PROP;
struct PSC_EINF_POSI;

struct _FYSG_PSC_LRFD_BASE;
struct _FYPC_AASHTO;
struct _FYCM_PSC_LRFD_BASE;
struct _FPMT_PSC_LRFD_BASE;
struct _TBAR_PSC_LRFD_BASE;
struct _BDCW_PSC_LRFD_BASE;
struct _STCM_AASHTO_BASE;
struct PSC_LRFD_STRS_CHK_POSI;
struct _TRAN_PSC_BASE;
struct _CRMS_AASHTO_BASE;
struct PSC_RUS_MEMB_RES_D;

struct PSC_FRCR_D;
struct PSC_FYSG_BASE;
struct PSC_FYCM_BASE;
struct PSC_FPSG_BASE;
struct PSC_FPMS_BASE;
struct PSC_FYPC_D;
struct PSC_CUMS_BASE;
struct PSC_CRMT_BASE;
struct PSC_BDCW_BASE;
struct PSC_FATG_BASE;
struct PSC_TNDN_CALD;
struct PSC_STRD_INFO;
struct PSC_FRCR_TNCD;

struct PSC_STRS_CHK_POSI;
struct PSC_BDCW_SNIP_BASE;
struct PSC_CUMS_SNIP_BASE;
struct PSC_FATI_SNIP_BASE;
struct PSC_DRES_RPT_SNIP;

struct CSG_SECT_POSD;
struct CSG_ULMF_D;
struct CSG_MEMB_POSD_KSCE_LSD;

class CDgn_PSCManager;
class CMembCtrl;

struct _PSC_MATL_ELEM
{
	T_MATD_D MatdD;
	double dfci; // 초기상태 콘크리트 압축강도 
	double dfci_s; 
	// JTG-D62-04.
	double dfck, dftk, dfckp, dftkp, dfcd, dftd;	// 콘크리트.
	double dfskM, dfsdtM, dfsdcM;									// 종방향철근.
	double dfskS, dfsdtS, dfsdcS;									// 전단철근.
	double dfskD, dfsdtD, dfsdcD;									// 경사철근.
	double dfpkV[2], dfpdtV[2], dfpdcV[2];				// 전단강봉 (0=I,1=J).
	// Eurocode2
	// 거더
	double dfcd_PT, dfcd_A;
	double dfyd_PT, dfyd_A; // fcd=fck/Gamma_c
	double dfywd_PT, dfywd_A;
	// 슬래브
	double dfcd_PT_s, dfcd_A_s;  
	double dfyd_PT_s, dfyd_A_s; // fcd=fck/Gamma_c
	double dfywd_PT_s, dfywd_A_s;

	// KSCE-LSd12
	//거더
	double dfcm, dfctm, dfctk; 
	//슬래브
	double dfcm_s, dfctm_s, dfctk_s; 
    // BS
    double dGamma_mc;
    double dGamma_ms;
    double dGamma_mv; //
    double dGamma_mc_s;
    double dGamma_mc_pre_s;
    double dGamma_mc_post_s;

	void Initialize()
	{
		MatdD.Initialize();
		dfci = 0.0;
		dfci_s = 0.0;
		dfck=0.0; dftk=0.0; dfckp=0.0; dftkp=0.0; dfcd=0.0; dftd=0.0;
		dfskM=0.0; dfsdtM=0.0; dfsdcM=0.0;
		dfskS=0.0; dfsdtS=0.0; dfsdcS=0.0;
		dfskD=0.0; dfsdtD=0.0; dfsdcD=0.0;
		for(int i=0; i<2; i++)
		{
			dfpkV[i]=0.0; dfpdtV[i]=0.0; dfpdcV[i]=0.0;
		}
		dfcd_PT = dfcd_A = 0.0;
		dfyd_PT = dfyd_A = 0.0;
		dfywd_PT = dfywd_A = 0.0;
		dfcm = dfctm = dfctk =0.0; 

		dfcd_PT_s = dfcd_A_s = 0.0;
		dfyd_PT_s = dfyd_A_s = 0.0;
		dfywd_PT_s = dfywd_A_s = 0.0;
		dfcm_s = dfctm_s = dfctk_s =0.0; 

        dGamma_mc = 1.0;
        dGamma_ms = 1.0;
        dGamma_mv = 1.0;
        dGamma_mc_s      = 1.0;
        dGamma_mc_pre_s  = 1.0;
        dGamma_mc_post_s = 1.0;

	}
	_PSC_MATL_ELEM() { Initialize();}
};

const int cSECT_INN = 20;	// Inner Maximum Number.
struct _PSC_SECT_POSI
{
	// Change by ZINU.('06.08.28). Add dWt.
	T_SPSC_SECT SpscSect;
	CString strShape;
	BOOL bClosedSect;
	BOOL bCompoSect;
	int  nDgnSectType;
	BOOL bUseAoh;
	double dAg;
	double dAoh, dPh;
	double dPcp, dAcp;                // KSCE03,05
	double dKt, dWt;									// Kt(JSEC02), Wt(JTG D62-04).
	double dpMomPos[4][2];						// Position for Stress (0~4), (0=y,1=z).
	double dpShrPos[3], dpShrThk[3];	// Web (0=Top, 1=Center, 2=Bot).
    double dQ[3];                       // 단면1차모멘트
	double dpFlgThk[2];								// Flange (0=Top, 1=Bot).
	double dTorThk;
	double dbtf;                      // Top flange의 돌출부를 제외한 폭
	int iInnerCellNum;
	CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
	CArray<T_SPSC_COOR,T_SPSC_COOR> arInner[cSECT_INN];
	// Eurocode2-2:05, 개단면일 때 Torsion 검토에 대한 property를 만들기 위해.
	double dLbarThk;
	BOOL bDivSect4Tor;
	int nDivPart;
	double dCutPos[3];
	double dIxx_part[3];
	double dAoh_part[3];
	double dPh_part[3];
	double dPcp_part[3];
	double dAcp_part[3];
	double dyDim_part[3];
	double dzDim_part[3];
	double dBc;  // 합성거더의 슬래브 유효폭
	double dtc;  // 합성거더의 슬래브 두께
	double dHh;  // 합성거더의 슬래브 이격거리.

	double dCutPos_Sl;  // slab
	double dIxx_part_Sl;
	double dAoh_part_Sl;
	double dPh_part_Sl;
	double dPcp_part_Sl;
	double dAcp_part_Sl;
	double dyDim_part_Sl;
	double dzDim_part_Sl;
	int nWebSize;

	void Initialize()
	{
		SpscSect.Initialize();
		strShape=_T("");
		bClosedSect=FALSE;
		bCompoSect=FALSE;
		nDgnSectType=-1;
		bUseAoh = TRUE;
		dAg=0.0;
		dAoh=0.0; dPh=0.0;    
		dKt=0.0; dWt=0.0;
		dPcp=0.0; dAcp=0.0;
		for(int i=0; i<4; i++)
		{
			dpMomPos[i][0]=0.0;
			dpMomPos[i][1]=0.0;
		}
		for(int i=0; i<3; i++)
		{
			dpShrPos[i]=0.0;
			dpShrThk[i]=0.0;
            dQ[i]      =0.0;
		}
		dpFlgThk[0]=0.0; dpFlgThk[1]=0.0;
		dTorThk=0.0;
		dbtf=0.0;
		iInnerCellNum=0;
		arOuter.RemoveAll();
		for(int i=0; i<cSECT_INN; i++)	arInner[i].RemoveAll();
		dLbarThk=0.0;
		bDivSect4Tor=FALSE;
		nDivPart=0;
		for(int i=0; i<3; i++)
		{
			dCutPos[i]   = 0.0;
			dIxx_part[i] = 0.0;
			dAoh_part[i] = 0.0;
			dPh_part[i]  = 0.0;
			dPcp_part[i] = 0.0;
			dAcp_part[i] = 0.0;
			dyDim_part[i]= 0.0;
			dzDim_part[i]= 0.0;
		}
		dBc = 0.0;
		dtc = 0.0;
		dHh = 0.0;

		dCutPos_Sl  = 0.0; 
		dIxx_part_Sl= 0.0;
		dAoh_part_Sl= 0.0;
		dPh_part_Sl = 0.0;
		dPcp_part_Sl= 0.0;
		dAcp_part_Sl= 0.0;
		dyDim_part_Sl = 0.0;
		dzDim_part_Sl = 0.0;
		nWebSize = 0;


	}
	_PSC_SECT_POSI() { Initialize(); }
	_PSC_SECT_POSI(const _PSC_SECT_POSI& rData) { Initialize(); *this = rData; }
	_PSC_SECT_POSI& operator = (const _PSC_SECT_POSI& rData)
	{
		SpscSect=rData.SpscSect;
		strShape=rData.strShape;
		bClosedSect=rData.bClosedSect;
		bCompoSect=rData.bCompoSect;
		nDgnSectType=rData.nDgnSectType;
		bUseAoh = rData.bUseAoh;
		dAg=rData.dAg;
		dAoh=rData.dAoh;
		dPh=rData.dPh;
		dKt=rData.dKt;
		dWt=rData.dWt;
		dPcp=rData.dPcp;
		dAcp=rData.dAcp;
		for(int i=0; i<4; i++)
		{
			dpMomPos[i][0]=rData.dpMomPos[i][0];
			dpMomPos[i][1]=rData.dpMomPos[i][1];
		}
		for(int i=0; i<3; i++)
		{
			dpShrPos[i]=rData.dpShrPos[i];
			dpShrThk[i]=rData.dpShrThk[i];
            dQ[i]      =rData.dQ[i];
		}
		dpFlgThk[0]=rData.dpFlgThk[0]; dpFlgThk[1]=rData.dpFlgThk[1];
		dTorThk=rData.dTorThk;
		dbtf   = rData.dbtf;
		arOuter.Copy(rData.arOuter);
		for(int i=0; i<cSECT_INN; i++)	arInner[i].Copy(rData.arInner[i]);
		dLbarThk = rData.dLbarThk;
		bDivSect4Tor = rData.bDivSect4Tor;
		nDivPart = rData.nDivPart;
		for(int i=0; i<3; i++)
		{
			dCutPos[i]   = rData.dCutPos[i];
			dIxx_part[i] = rData.dIxx_part[i];
			dAoh_part[i] = rData.dAoh_part[i];
			dPh_part[i]  = rData.dPh_part[i];
			dPcp_part[i] = rData.dPcp_part[i];
			dAcp_part[i] = rData.dAcp_part[i];
			dyDim_part[i] = rData.dyDim_part[i];
			dzDim_part[i] = rData.dzDim_part[i];
		}
		dBc = rData.dBc;
		dtc = rData.dtc;
		dHh = rData.dHh;

		dCutPos_Sl   = rData.dCutPos_Sl  ;
		dIxx_part_Sl = rData.dIxx_part_Sl;
		dAoh_part_Sl = rData.dAoh_part_Sl;
		dPh_part_Sl  = rData.dPh_part_Sl ;
		dPcp_part_Sl = rData.dPcp_part_Sl;
		dAcp_part_Sl = rData.dAcp_part_Sl;
		dyDim_part_Sl = rData.dyDim_part_Sl;
		dzDim_part_Sl = rData.dzDim_part_Sl;
		nWebSize     = rData.nWebSize;

		return *this;
	}
};

struct _PSC_SECT_ELEM
{
	_PSC_SECT_POSI SectPosi[2];	// 0=I,1=J.
	void Initialize()
	{
		for(int i=0; i<2; i++)	SectPosi[i].Initialize();
	}
	_PSC_SECT_ELEM() { Initialize(); }
	_PSC_SECT_ELEM(const _PSC_SECT_ELEM& rData) { Initialize(); *this = rData; }
	_PSC_SECT_ELEM& operator = (const _PSC_SECT_ELEM& rData)
	{
		for(int i=0; i<2; i++)	SectPosi[i] = rData.SectPosi[i];
		return *this;
	}
};

struct _PSC_SECG_ELEM
{
	double dBc;  // 합성거더의 슬래브 유효폭
	double dtc;  // 합성거더의 슬래브 두께
	double dHh;  // 합성거더의 슬래브 이격거리.

	T_SPSC_SECT SectPosi[2];	// 0=I,1=J.
	void Initialize()
	{
		dBc = 0.0;
		dtc = 0.0;
		dHh = 0.0;
		for(int i=0; i<2; i++)	SectPosi[i].Initialize();
	}
	_PSC_SECG_ELEM() { Initialize(); }
	_PSC_SECG_ELEM(const _PSC_SECG_ELEM& rData) { Initialize(); *this = rData; }
	_PSC_SECG_ELEM& operator = (const _PSC_SECG_ELEM& rData)
	{
		dBc = rData.dBc;
		dtc = rData.dtc;
		dHh = rData.dHh;
		for(int i=0; i<2; i++)	SectPosi[i] = rData.SectPosi[i];
		return *this;
	}
};

struct _PSC_RBAR_POSI
{
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
	void Initialize()
	{
		arRbarPosi.RemoveAll();
	}
	_PSC_RBAR_POSI() { Initialize(); }
	_PSC_RBAR_POSI(const _PSC_RBAR_POSI& rData) { Initialize(); *this = rData; }
	_PSC_RBAR_POSI& operator = (const _PSC_RBAR_POSI& rData)
	{
		arRbarPosi.Copy(rData.arRbarPosi);
		return *this;
	}
};

struct _PSC_RBAR_ELEM
{
	_PSC_RBAR_POSI RbarPosi[2];	// 0=I,1=J.
	void Initialize()
	{
		for(int i=0; i<2; i++)	RbarPosi[i].Initialize();
	}
	_PSC_RBAR_ELEM() { Initialize(); }
	_PSC_RBAR_ELEM(const _PSC_RBAR_ELEM& rData) { Initialize(); *this = rData; }
	_PSC_RBAR_ELEM& operator = (const _PSC_RBAR_ELEM& rData)
	{
		for(int i=0; i<2; i++)	RbarPosi[i] = rData.RbarPosi[i];
		return *this;
	}
};

struct _PSC_RSHR_POSI
{
	double dfdt, dfdb;
	double dVd, dMd;
	T_RPSC_SBAR RpscSbar;
	void Initialize()
	{
		dfdt=0.0; dfdb=0.0;
		dVd=0.0; dMd=0.0;
		RpscSbar.Initialize();
	}
	_PSC_RSHR_POSI() { Initialize(); }
	_PSC_RSHR_POSI(const _PSC_RSHR_POSI& rData) { Initialize(); *this = rData; }
	_PSC_RSHR_POSI& operator = (const _PSC_RSHR_POSI& rData)
	{
		dfdt=rData.dfdt; dfdb=rData.dfdb;
		dVd=rData.dVd; dMd=rData.dMd;
		RpscSbar=rData.RpscSbar;
		return *this;
	}
};

struct _PSC_RSHR_ELEM
{
	_PSC_RSHR_POSI RshrPosi[2];	// 0=I,1=J.
	void Initialize()
	{
		for(int i=0; i<2; i++)	RshrPosi[i].Initialize();
	}
	_PSC_RSHR_ELEM() { Initialize(); }
	_PSC_RSHR_ELEM(const _PSC_RSHR_ELEM& rData) { Initialize(); *this = rData; }
	_PSC_RSHR_ELEM& operator = (const _PSC_RSHR_ELEM& rData)
	{
		for(int i=0; i<2; i++)	RshrPosi[i] = rData.RshrPosi[i];
		return *this;
	}
};

struct _PSC_TNDN_POSI
{
	int iMaxLoadType;
	int iMaxRelxType;
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
	BOOL bPost;
	BOOL bBonded;
	void Initialize()
	{
		iMaxLoadType=0;
		iMaxRelxType=0;
		arTndnPosi.RemoveAll();
		bPost=TRUE;
		bBonded=TRUE;
	}
	_PSC_TNDN_POSI() {Initialize();}
	_PSC_TNDN_POSI(const _PSC_TNDN_POSI& rData) { Initialize(); *this = rData; }
	_PSC_TNDN_POSI& operator = (const _PSC_TNDN_POSI& rData)
	{
		iMaxLoadType = rData.iMaxLoadType;
		iMaxRelxType = rData.iMaxRelxType;
		arTndnPosi.Copy(rData.arTndnPosi);
		bPost = rData.bPost;
		bBonded = rData.bBonded;
		return *this;
	}
};
struct _PSC_TNDN_ELEM
{
	_PSC_TNDN_POSI TndnPosi[2];	// 0=I,1=J.
	void Initialize()
	{
		for(int i=0; i<2; i++)	TndnPosi[i].Initialize();
	}
	_PSC_TNDN_ELEM() { Initialize(); }
	_PSC_TNDN_ELEM(const _PSC_TNDN_ELEM& rData) { Initialize(); *this = rData; }
	_PSC_TNDN_ELEM& operator = (const _PSC_TNDN_ELEM& rData)
	{
		for(int i=0; i<2; i++)	TndnPosi[i] = rData.TndnPosi[i];
		return *this;
	}
};

struct _PSC_TNDN_TDNA
{
    int iLoadType;	// 0=Pre-tension, 1=Post-tension.
    int iRelxType;	// KSCE-USD03 (0=저릴렉세이션(fpy/fpu>=0.90), 1=응력제거(fpy/fpu>=0.85), 전단강봉(fpy/fpu>=0.80)).
    // JTG-D62-04 (0=Strand, 1=Wire, 2=Steelbar).
    double dfpu, dfpy;
    double dfpk, dfpdt, dfpdc;
    double dFp01, dfp01k; // Eurocode
    double dfpd;

    double dTendonLength; // Tendon 전체길이
    int iBoundaryNum; // Tendon이 걸쳐있는 boundary 개수 
    double dTransBeginLen;   // Transmission length, begin
    double dTransEndLen;     // Transmission length, end
    double dDebondedBeginLen;  // Debonded 시작 길이
    double dDebondedEndLen;    // Debonded 끝 길이

    void Initialize()
    {
        iLoadType=0;
        iRelxType=0;
        dfpu=0.0; dfpy=0.0;
        dfpk=0.0; dfpdt=0.0; dfpdc=0.0;
        dFp01=dfp01k=.0;
        dfpd=0.0;
        dTendonLength = 0.0;
        iBoundaryNum = 0;
        dTransBeginLen = 0.0;
        dTransEndLen = 0.0;
        dDebondedBeginLen = 0.0;
        dDebondedEndLen = 0.0;

    }
    _PSC_TNDN_TDNA() { Initialize(); }
};

struct _PSC_TNDN_TDNT
{
	_PSC_TNDN_TDNA TndnTdna;

	CString TendonTypeName;
	void Initialize()
	{
		TndnTdna.Initialize();
		TendonTypeName.Empty();
	}
	_PSC_TNDN_TDNT() { Initialize(); }
	_PSC_TNDN_TDNT(const _PSC_TNDN_TDNT& rData) { Initialize(); *this = rData; }
	_PSC_TNDN_TDNT& operator = (const _PSC_TNDN_TDNT& rData)
	{
		TndnTdna = rData.TndnTdna;
		TendonTypeName = rData.TendonTypeName;
		return *this;
	}
};

struct _PSC_TNDN_PROP
{
	CString   strTendonTypeName;
	int       nTendonType;          // 1:Internal, 2:External
	double    dTendonArea;
	double    dDuctArea;            // Internal-Pre인 경우는 StrandArea로 사용됨
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Pre-tension, 1=Post-tension  
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	int       nRelaxMethod;   // 0=Magura, 1=CEB-FIP, 2=JTG04 : 6.4.0 추가 , JTG04는 6.6.0추가
	// 3=TB05, 4=INDIA(IRC:18-2000) 7.5.0 추가
	// 5=European : 7.6.0 추가
	double    dRelaxRatio;    // (6.4.0 추가)

	void Initialize()
	{
		strTendonTypeName.Empty();
		nTendonType = 1;
		dTendonArea = 0.0;
		dDuctArea = 0.0;
		dUltimateStrength = 0.0;
		dYieldStrength = 0.0;
		nLoadType = 1;          // post    
		nTypeId=0;
		nBondType = 0;
		nRelaxMethod = 0;
		dRelaxRatio = 0.0;
	}
	_PSC_TNDN_PROP() {Initialize();}

};
struct _PSC_TNDN_PTIF
{
	CArray<_PSC_TNDN_PROP,_PSC_TNDN_PROP&> aTndnProp;

	void Initialize()
	{
		aTndnProp.RemoveAll();
	}
	_PSC_TNDN_PTIF() {Initialize();}
	_PSC_TNDN_PTIF& operator = (const _PSC_TNDN_PTIF& rData)
	{
		aTndnProp.Copy(rData.aTndnProp);
		return *this;
	}
};


struct _DGN_TNDN_RBAR
{
	//※주의 : 여기서 압축은 중립축에 대한 상면or우측을 나타내며 부모멘트의 경우에는 압축이 아닌 인장측이 됨
	double dApttol, dNtodpt, ddpt, dTpttol;// Tension     Tendon의 총 단면적,   단면적의 도심과 중립축과의 거리,   단면의 최상단(우측)에서도 단면적의 도심까지의 거리, 긴장력의 총합 
	double dApctol, dNtodpc, ddpc, dTpctol;// Compression Tendon의 총 단면적,   단면적의 도심과 중립축과의 거리,   단면의 최상단(우측)에서도 단면적의 도심까지의 거리, 긴장력의 총합
	double dAsttol, dNtodst, ddst;// Tension     Rebar 의 총 단면적,   단면적의 도심과 중립축과의 거리,   단면의 최상단(우측)에서도 단면적의 도심까지의 거리
	double dAsctol, dNtodsc, ddsc;// Compression Rebar 의 총 단면적,   단면적의 도심과 중립축과의 거리,   단면의 최상단(우측)에서도 단면적의 도심까지의 거리
	double daTopt, daTopc;
	double daBott, daBotc;
	double ddpt_max, ddpc_max;  // 최외단 텐던위치

	void Initialize()
	{
		dApttol=0.0, dNtodpt=0.0, ddpt=0.0, dTpttol=0.0;
		dApctol=0.0, dNtodpc=0.0, ddpc=0.0, dTpctol=0.0;
		dAsttol=0.0, dNtodst=0.0, ddst=0.0;
		dAsctol=0.0, dNtodsc=0.0, ddsc=0.0;
		daTopt=0.0, daTopc=0.0;
		daBott=0.0, daBotc=0.0;
		ddpt_max=0.0, ddpc_max=0.0;
	}

	_DGN_TNDN_RBAR& operator = (const _DGN_TNDN_RBAR& rData)
	{
		dApttol=rData.dApttol, dNtodpt=rData.dNtodpt, ddpt=rData.ddpt, dTpttol = rData.dTpttol;
		dApctol=rData.dApctol, dNtodpc=rData.dNtodpc, ddpc=rData.ddpc, dTpctol = rData.dTpctol;
		dAsttol=rData.dAsttol, dNtodst=rData.dNtodst, ddst=rData.ddst;
		dAsctol=rData.dAsctol, dNtodsc=rData.dNtodsc, ddsc=rData.ddsc;
		daTopt=rData.daTopt, daTopc=rData.daTopc;  //JTG D62-04
		daBott=rData.daBott, daBotc=rData.daBotc;
		ddpt_max=rData.ddpt_max;
		ddpc_max=rData.ddpc_max;

		return *this;
	}

};

struct _PSC_EXPC_POSI // exposure class
{
	int iExposureClass[2]; // 0=Top,1=Bottom
	// 구조체를 추가하면 작업량이 많이지기 때문에 여기에 추가 
	BOOL   bmb6;       // 러시아. Operation Condition Factor, SNiP - Table 24
	int    nmb6Type;
	double dSigma_by;
	double dmb6;
	double dmb6_s;
	BOOL   bmb10;
	int    nmb10Type;
	double dSeamThick;
	double dRbj;
	double dmb10;

	void Initialize()
	{
		memset(iExposureClass,0,sizeof(iExposureClass));
		bmb6      = FALSE;
		nmb6Type  = 0;
		dSigma_by = 0.0;
		dmb6 = dmb6_s = 0.0;
		bmb10     = FALSE;
		nmb10Type = 0;
		dSeamThick= 0.0;
		dRbj      = 0.0;
		dmb10     = 0.0;
	}
	_PSC_EXPC_POSI() {Initialize();}
	_PSC_EXPC_POSI(const _PSC_EXPC_POSI& rData) {*this = rData;}
	_PSC_EXPC_POSI& operator = (const _PSC_EXPC_POSI& rData)
	{
		Initialize();
		memcpy(iExposureClass,rData.iExposureClass,sizeof(iExposureClass));

		bmb6      = rData.bmb6;
		nmb6Type  = rData.nmb6Type;
		dSigma_by = rData.dSigma_by;
		dmb6      = rData.dmb6;
		dmb6_s    = rData.dmb6_s;
		bmb10     = rData.bmb10;
		nmb10Type = rData.nmb10Type;
		dSeamThick= rData.dSeamThick;
		dRbj      = rData.dRbj;
		dmb10     = rData.dmb10;

		return *this;
	}
};

struct _PSC_EXPC_ELEM
{
	_PSC_EXPC_POSI ExpcPosi[2];	// 0=I,1=J.
	void Initialize()
	{
		for(int i=0; i<2; i++)	ExpcPosi[i].Initialize();
	}
	_PSC_EXPC_ELEM() { Initialize(); }
	_PSC_EXPC_ELEM& operator = (const _PSC_EXPC_ELEM& rData)
	{
		for(int i=0; i<2; i++)	ExpcPosi[i] = rData.ExpcPosi[i];
		return *this;
	}
};

struct _PSC_PSCN_IRS_ELEM  // shear conn
{
	// 0=I,1=J.
	int nFaceState[2];   // 표면상태 0: 매우 매끄러움, 1: 매끄러움, 2:거침, 3:요철 표면
	double dSL[2];    // [No Unit] 전단연결재 기울기
	double dASL[2];      // [Area]   전단연결재 면적
	double dSt[2];       // [Area] 전단연결재 항복강도
	//Interface Shear
	double dAe[2];    // [Area] 전단연결재 기울기
	double dFck[2];      // [STRESS]   전단연결재 면적
	double dFy[2];       // [STRESS] 전단연결재 항복강도

	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			nFaceState[i] = 0;
			dSL[i] = 0.0;  
			dASL[i]= 0.0; 
			dSt[i] = 0.0;  
			dAe[i] = 0.0; 
			dFck[i]= 0.0;
			dFy[i] = 0.0; 
		}

	}
	_PSC_PSCN_IRS_ELEM() { Initialize(); }
	_PSC_PSCN_IRS_ELEM& operator = (const _PSC_PSCN_IRS_ELEM& rData)
	{

		for(int i=0; i<2; i++)
		{
			nFaceState[i] = rData.nFaceState[i];
			dSL[i]      = rData.dSL[i]   ;  
			dASL[i]     = rData.dASL[i]  ;  
			dSt[i]      = rData.dSt[i]   ;  
			dAe[i]      = rData.dAe[i]   ;  
			dFck[i]     = rData.dFck[i]  ;  
			dFy[i]      = rData.dFy[i]   ; 
		}

		return *this;
	}
};

struct _PSC_PSCN_ELEM  // shear conn
{
	// 0=I,1=J.
	int nFaceState[2];   // 표면상태 0: 매우 매끄러움, 1: 매끄러움, 2:거침, 3:요철 표면
	double dAngle[2];    // [Degree] 전단연결재 기울기
	double dAw[2];       // [Area]   전단연결재 면적
	double dfy[2];       // [STRESS] 전단연결재 항복강도
    double dAe[2];       // BS Longitudinal shear
    double ds[2];        // BS Longitudinal shear
    double dLs[2];       // BS Longitudinal shear
	_PSC_PSCN_IRS_ELEM IRS_SConn;

	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			nFaceState[i] = 0;
			dAngle[i]     = 0.0; 
			dAw[i]        = 0.0; 
			dfy[i]        = 0.0; 
            dAe[i]        = 0.0; 
            ds[i]         = 0.0; 
            dLs[i]        = 0.0; 
		}
		IRS_SConn.Initialize();
	}
	_PSC_PSCN_ELEM() { Initialize(); }
	_PSC_PSCN_ELEM& operator = (const _PSC_PSCN_ELEM& rData)
	{

		for(int i=0; i<2; i++)
		{
			nFaceState[i] = rData.nFaceState[i];
			dAngle[i]     = rData.dAngle[i]    ;  
			dAw[i]        = rData.dAw[i]       ;  
			dfy[i]        = rData.dfy[i]       ;  
            dAe[i]        = rData.dAe[i]       ;  
            ds[i]         = rData.ds[i]        ;  
            dLs[i]        = rData.dLs[i]       ;  
		}
		IRS_SConn = rData.IRS_SConn;
		return *this;
	}
};



struct _PSC_PSCS_ELEM  // Interface Shear
{
	// 0=I,1=J.        
	int nFaceState[2];   // 0: Roughened(CIP slab) 1:Monolithically  
											 // 2:Intentionally roughened(LWC) 3:Intentionally roughened(NWC) 
											 // 4:Not intentionally roughened 5: Anchored 
	double dBvi[2];      // [Length] Interface width
	double dAvf[2];      // [Area]   Area of Interface Shear Reinforcement Connector
	double dfy[2];       // [STRESS] Yield Stress of Reinforcement (Shear Connector)
	double dAlphaf[2];   // [DEGREE] 전단연결재 각도, alpha_f

	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			nFaceState[i]   = 0;
			dBvi[i]         = 0.0; 
			dAvf[i]         = 0.0; 
			dfy[i]          = 0.0; 
			dAlphaf[i]      = 90.0;
		}

	}
	_PSC_PSCS_ELEM() { Initialize(); }
	_PSC_PSCS_ELEM& operator = (const _PSC_PSCS_ELEM& rData)
	{

		for(int i=0; i<2; i++)
		{
			nFaceState[i] = rData.nFaceState[i] ;
			dBvi[i]       = rData.dBvi[i]       ;  
			dAvf[i]       = rData.dAvf[i]       ;  
			dfy[i]        = rData.dfy[i]        ;  
			dAlphaf[i]    = rData.dAlphaf[i]    ;
		}

		return *this;
	}
};

struct _PSC_FACK_ELEM  // PSC 피로
{
	// 0=I,1=J.
	double dt0[2];    // [day] 

	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			dt0[i]  = 0.0; 
		}    
	}
	_PSC_FACK_ELEM() { Initialize(); }
	_PSC_FACK_ELEM& operator = (const _PSC_FACK_ELEM& rData)
	{	
		for(int i=0; i<2; i++)
		{
			dt0[i] = rData.dt0[i];
		}

		return *this;
	}
};

struct _PSC_CINF_ELEM
{
    int nClass;

    void Initialize()
    {
        nClass = 1;
    }
    _PSC_CINF_ELEM() { Initialize(); }
    _PSC_CINF_ELEM& operator = (const _PSC_CINF_ELEM& rData)
    {	
        nClass = rData.nClass;

        return *this;
    }
};

struct _PSC_SPBF_POSI
{
    BOOL bFromI;
    double dLengthFromEnd;

    void Initialize()
    {
        bFromI = TRUE;
        dLengthFromEnd = 0.0;
    }
    _PSC_SPBF_POSI() { Initialize(); }
};

struct _PSC_SPBF_ELEM
{
    _PSC_SPBF_POSI PosD[2]; // i,j

    void Initialize()
    {
        for ( int i=0; i<2; ++i )
        {
            PosD[i].Initialize();
        }
    }

    _PSC_SPBF_ELEM() { Initialize(); }
    _PSC_SPBF_ELEM& operator = (const _PSC_SPBF_ELEM& rData)
    {
        for ( int i=0; i<2; ++i )
        {
            PosD[i].Initialize();
        }
        return *this;
    }
};


/////////////////////////////////////////
// Composite Plate Girder
/////////////////////////////////////////
// Material
struct CPG_MATL_ELEM
{
	T_MATD_D MatdD;
	// Steel
	CString strStlMatName;

	// Concrete
	double dfck;
	double dfcm;
	double dfctm;
	double dEcm;

	// classify
	// 1=S235, S275, S355
	// 2=S420, S460
	// 3=???
	int iMatClass;  

	void Initialize()
	{
		strStlMatName.Empty();
		MatdD.Initialize();
		dfck = dfcm = dfctm = dEcm = 0.0;
		iMatClass = 3;
	}

	CPG_MATL_ELEM& operator = (const CPG_MATL_ELEM& rData)
	{
		strStlMatName = rData.strStlMatName;
		MatdD = rData.MatdD;    
		dfck  = rData.dfck;
		dfcm  = rData.dfcm;
		dfctm = rData.dfctm;
		dEcm  = rData.dEcm;
		iMatClass = rData.iMatClass;

		return *this;
	}

};

// Section
struct CPG_SECT_POSI
{	  
	T_SECT_STIFFNESS PureSect;
	T_SECT_STIFFNESS RebarSect;     // Rebar 반영한 환산단면
	T_SECT_STIFFNESS NonConSect;    // 부모멘트경우(콘크리트 무시) 환산단면

	T_SECT_STIFFNESS RbarStiff3n;   // Rebar 반영한 환산단면, 정모멘트, 3n적용
	T_SECT_STIFFNESS NonSlabStiff2; // 부모멘트경우(콘크리트 무시) 환산단면, Rebar 1/3 적용  

	void Initialize()
	{
		PureSect.Initialize();
		RebarSect.Initialize();
		NonConSect.Initialize();
		RbarStiff3n.Initialize();
		NonSlabStiff2.Initialize();
	}
	CPG_SECT_POSI () { Initialize(); }
	CPG_SECT_POSI(const CPG_SECT_POSI& rData) { *this = rData; }
	CPG_SECT_POSI& operator = (const CPG_SECT_POSI& rData)
	{
		PureSect      = rData.PureSect;
		RebarSect     = rData.RebarSect;
		NonConSect    = rData.NonConSect;
		RbarStiff3n   = rData.RbarStiff3n;
		NonSlabStiff2 = rData.NonSlabStiff2;
		return *this;
	}
};

struct CPG_SECT_ELEM
{
	T_SECT_D SectD;
	CPG_SECT_POSI SectPosi[2];  
	T_GLINE_DGN_D GLineDgn[2];
	BOOL bGenSect;
	BOOL bCracked;
	//  double dPt_minI[2]; // y,z
	//  double dPt_minJ[2];

	void Initialize()
	{
		SectD.Initialize();
		for(int i=0; i<2; i++)
		{
			SectPosi[i].Initialize();
			GLineDgn[i].Initialize();
		}
		bGenSect=FALSE;
		bCracked=FALSE;
		//    memset(dPt_minI,0,sizeof(dPt_minI));
		//    memset(dPt_minJ,0,sizeof(dPt_minJ));
	}

	CPG_SECT_ELEM& operator = (const CPG_SECT_ELEM& rData)
	{
		SectD = rData.SectD;
		for(int i=0; i<2; i++)
		{
			SectPosi[i] = rData.SectPosi[i];
			GLineDgn[i] = rData.GLineDgn[i];
		}
		bGenSect = rData.bGenSect;
		bCracked = rData.bCracked;
		//    memcpy(dPt_minI, rData.dPt_minI, sizeof(dPt_minI));
		//    memcpy(dPt_minJ, rData.dPt_minJ, sizeof(dPt_minJ));
		return *this;
	}
};

// Rbar
struct CPG_RBAR_ELEM
{
	_PSC_RBAR_POSI RbarPosi[2];	// 0=I,1=J.
	_PSC_RBAR_POSI RbarPosiCR[2];	// 0=I,1=J. 균열고려.
	void Initialize()
	{
		for(int i=0; i<2; i++)
		{
			RbarPosi[i].Initialize();
			RbarPosiCR[i].Initialize();
		}
	}
	CPG_RBAR_ELEM& operator = (const CPG_RBAR_ELEM& rData)
	{
		for(int i=0; i<2; i++)
		{
			RbarPosi[i]   = rData.RbarPosi[i];
			RbarPosiCR[i] = rData.RbarPosiCR[i];
		}
		return *this;
	}
};

// Longitudinal Stiffener

struct CPG_LSTI_POSI
{
	CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&> arLstiPosi;
	void Initialize()
	{
		arLstiPosi.RemoveAll();
	}
	CPG_LSTI_POSI& operator = (const CPG_LSTI_POSI& rData)
	{
		arLstiPosi.Copy(rData.arLstiPosi);
		return *this;
	}
};

struct CPG_LSTI_ELEM
{
	CPG_LSTI_POSI LstiPosi[2];

	void Initialize()
	{
		LstiPosi[0].Initialize();
		LstiPosi[1].Initialize();
	}

	CPG_LSTI_ELEM& operator = (const CPG_LSTI_ELEM& rData)
	{    
		LstiPosi[0] = rData.LstiPosi[0];    
		LstiPosi[1] = rData.LstiPosi[1];    
		return *this;
	}
};

// Shear Connector
struct CPG_SCON_POSI
{	  
	BOOL   bUseShear;	// Use Shear Connector (default=TRUE)
	int    iNum;      // Number of connectors(transverse)
	double dfu;       // Ultimate tensile strength
	double dDia;      // Diameter        
	double dhsc;      // Overall height  
	double dSt;       // C to C  
	double dSpace;    // Longitudinal spacing 

	//IRC22 Steel COmposite inputs in shear connectors
	double dQr;       // Nominal Fatigue Strength kN/m^2
	double dAst;      // Transverse R/F Ast (mm^2/mm)
	double dPlaneLen; // Length of shear Plane

	void Initialize()
	{		
		bUseShear = TRUE;
		iNum = 0;
		dfu = 0.0;
		dDia = 0.0;
		dhsc = 0.0;
		dSt = 0.0;
		dSpace = 0.0;
		dQr         = 0.0;      
		dAst        = 0.0;    
		dPlaneLen   = 0.0;
	}

	CPG_SCON_POSI& operator = (const CPG_SCON_POSI& rData)
	{
		bUseShear = rData.bUseShear;
		iNum      = rData.iNum;
		dfu       = rData.dfu;
		dDia      = rData.dDia;
		dhsc      = rData.dhsc;
		dSt       = rData.dSt;
		dSpace    = rData.dSpace;
		dQr         = rData.dQr      ;      
		dAst        = rData.dAst     ;    
		dPlaneLen   = rData.dPlaneLen;
		return *this;
	}
};

struct CPG_SCON_ELEM
{
	CPG_SCON_POSI SconPosi[2];

	void Initialize()
	{
		SconPosi[0].Initialize();
		SconPosi[1].Initialize();
	}

	CPG_SCON_ELEM& operator = (const CPG_SCON_ELEM& rData)
	{    
		SconPosi[0] = rData.SconPosi[0];    
		SconPosi[1] = rData.SconPosi[1];    
		return *this;
	}
};

// Transverse Stiffener
struct CPG_TRST_POSI
{  
	BOOL   bUseStif;					 // Use stiffener 
	BOOL   bInterRigidStif;    // Intermediate rigid stiffener
	BOOL   bInterNonRigidStif; // Intermediate non-rigid stiffener 
	int    iStifNum;           // Stiffener 개수(0=One stiffener, 1=Two stiffener) 
	double dht;                // ht 
	double dt;                 // stiffener 두께. 
	double dPitch;             // Rigid striffener pitch 
	double dDistRstif;         // distance from rigid stiffener 
	double dDistNRstif;        // distance from non-rigid stiffener 
	void Initialize()
	{ 
		bUseStif = FALSE; //TRUE; // Modify by GAY. PMS:4216, TRUE->FALSE.
		bInterRigidStif=FALSE;   
		bInterNonRigidStif=FALSE;
		iStifNum = 0;           
		dht = 0.0;             
		dt = 0.0;              
		dPitch = 0.0;          
		dDistRstif = 0.0;      
		dDistNRstif = 0.0;     
	}
};

struct CPG_TRST_ELEM
{
	CPG_TRST_POSI TrstPosi[2];

	void Initialize()
	{
		TrstPosi[0].Initialize();
		TrstPosi[1].Initialize();
	}

	CPG_TRST_ELEM& operator = (const CPG_TRST_ELEM& rData)
	{    
		TrstPosi[0] = rData.TrstPosi[0];    
		TrstPosi[1] = rData.TrstPosi[1];    
		return *this;
	}
};

// SM - Transverse Stiffener
// struct CPG_TSTI_POSI
// {
//   void Initialize()
//   { 
//   }
// 
//   CPG_TSTI_POSI() {Initialize();}
// };

// Transverse stiffener of End-support
struct CPG_TSES_POSI
{
	BOOL   bUseStif; 
	int    iEndpost; // 0=Rigid end post, 1=Non-rigid end post, 2=No end post
	double dht;      // stiffener(h) 
	double dt;       // stiffener(t) 
	double de;       // Rigid end post일 경우 center to center distance between the stiffeners 
	// 주의.
	// de는 Rigid end post일 경우만 boundary 체크함.
	// dht, dt는 No end post일 경우 boundary 체크안함.

	void Initialize()
	{
		bUseStif = FALSE;
		iEndpost = 1;
		dht = 0.0;
		dt = 0.0;
		de = 0.0;
	}  
};

struct CPG_TSES_ELEM
{
	CPG_TSES_POSI TsesPosi[2];

	void Initialize()
	{
		TsesPosi[0].Initialize();
		TsesPosi[1].Initialize();
	}

	CPG_TSES_ELEM& operator = (const CPG_TSES_ELEM& rData)
	{    
		TsesPosi[0] = rData.TsesPosi[0];    
		TsesPosi[1] = rData.TsesPosi[1];    
		return *this;
	}
};

struct CPG_BCDT_POSI
{
	BOOL bUseTfType;  // Use  (default=TRUE)
	int iLoadAppType; // 0=Type(a), 1=Type(b), 2=Type(c)
	double da;        // a, (default=0.0, boundary >0, Unit:Length)
	double dSs;       // Ss, (default=0.0, boundary >0, Unit:Length)
	double dC;        // C, (default=0.0, boundary >0, Unit:Length)
	// dC는 iLoadAppType==2 일 경우만 체크한다. 
	void Initialize()
	{
		iLoadAppType=0;
		da=0.0;
		dSs=0.0;
		dC=0.0;
	}
};

struct CPG_BCDT_ELEM
{
	CPG_BCDT_POSI BcdtPosi[2];

	void Initialize()
	{
		BcdtPosi[0].Initialize();
		BcdtPosi[1].Initialize();
	}

	CPG_BCDT_ELEM& operator = (const CPG_BCDT_ELEM& rData)
	{    
		BcdtPosi[0] = rData.BcdtPosi[0];    
		BcdtPosi[1] = rData.BcdtPosi[1];    
		return *this;
	}
};

struct CPG_LTBD_POSI
{
	BOOL bUseData;  // Use  (default=TRUE)  
	double dl;        // distance between the springs( default=0, boundary >0, Unit:Length) 
	double dCd;       // spring stiffness(default=0, boundary >0, Unit:D_UNITSYS_BASE_UNITFORCE)
	int iAlpha;       // 1994-2,45p, (6.9) k1계산에서 α (default=2, boundary: 2~4, Unit:None)
	double da;        // spacing between the parallel beam (default=0, boundary >0,  Unit:Length)
	void Initialize()
	{
		bUseData=false;
		dl=0.0;
		dCd=0.0;
		iAlpha=0;
		da=0.0;
	}
};

struct CPG_LTBD_ELEM
{
	CPG_LTBD_POSI LtbdPosi[2];

	void Initialize()
	{
		LtbdPosi[0].Initialize();
		LtbdPosi[1].Initialize();
	}

	CPG_LTBD_ELEM& operator = (const CPG_LTBD_ELEM& rData)
	{    
		LtbdPosi[0] = rData.LtbdPosi[0];    
		LtbdPosi[1] = rData.LtbdPosi[1];    
		return *this;
	}
};

struct CPG_FRDT_POSI
{
	BOOL   bUseData;	// Use Data (default=TRUE)
	double dLamda_v2; // factor for the traffic volume ( default=0, boundary >0, Unit:None) 
	double dLamda_v4; // factor for the structural element is loaded by more than one track (default=0, boundary >0, Unit:None)

	void Initialize()
	{
		bUseData= TRUE;
		dLamda_v2 = 0.0;
		dLamda_v4 = 0.0;  
	}
};

struct CPG_FRDT_ELEM
{
	CPG_FRDT_POSI FrdtPosi[2];

	void Initialize()
	{
		FrdtPosi[0].Initialize();
		FrdtPosi[1].Initialize();
	}

	CPG_FRDT_ELEM& operator = (const CPG_FRDT_ELEM& rData)
	{    
		FrdtPosi[0] = rData.FrdtPosi[0];    
		FrdtPosi[1] = rData.FrdtPosi[1];    
		return *this;
	}
};

struct CPG_MEMB_POSI
{  
	double dLy;
	double dLz;
	double dLu;       // 지지길이 
	void Initialize()
	{
		dLy=0.0;
		dLz=0.0;
		dLu=0.0;
	}
};

struct CPG_MEMB_ELEM
{
	CPG_MEMB_POSI MembPosi[2];

	void Initialize()
	{
		MembPosi[0].Initialize();
		MembPosi[1].Initialize();
	}

	CPG_MEMB_ELEM& operator = (const CPG_MEMB_ELEM& rData)
	{    
		MembPosi[0] = rData.MembPosi[0];    
		MembPosi[1] = rData.MembPosi[1];    
		return *this;
	}
};

struct CPG_CSIF_D
{
	T_STAG_K StagK;
	BOOL     bIsStep;
	int      nStepK;
	int      nSerialStepK;

	void Initialize()
	{
		StagK        = 0;
		bIsStep      = FALSE;
		nStepK       = 0;
		nSerialStepK = 0;
	}

	CPG_CSIF_D() {Initialize();}
};

typedef CArray<CPG_CSIF_D,CPG_CSIF_D&> ArCSIFK;

struct T_STIFF_CALC_D
{
	double dArea;
	double dIy;
	double dz; // Bottom에서   

	void Initialize()
	{
		dArea=0.0;
		dIy=0.0;
		dz=0.0;
	}

	T_STIFF_CALC_D() { Initialize(); }
	T_STIFF_CALC_D(const T_STIFF_CALC_D& rData) { *this = rData; }
	T_STIFF_CALC_D& operator=(const T_STIFF_CALC_D& rData) 
	{
		dArea = rData.dArea;
		dIy = rData.dIy;
		dz = rData.dz;

		return *this;
	}

};


enum PG_Stress_Distribution
{
	Internal_Stress_Type1 = 0,
	Internal_Stress_Type2,
	Internal_Stress_Type3,
	Outstand_Stress_Type1,
	Outstand_Stress_Type2,
	Outstand_Stress_Type3,
	Outstand_Stress_Type4,
};

enum PG_Notations_Stiffened_Plate
{
	Stiffened_Plate_inf = 0,
	Stiffened_Plate_sup,
	Stiffened_Plate_sup_dbc,  
};

enum PG_KminType
{
	Kmin_girder_top = 0,
	Kmin_girder_bot,
	Kmin_Slab,
	Kmin_Rbar,
};

enum DGN_Calc_Type
{
    DGN_TYPE_PSC = 0,
    DGN_TYPE_RATING,
};

namespace dgn
{
    namespace lib
    {
        class CNationalAnnexInterface;
    }
}

class CDgnPscCommon  
{
public:
	CDgnPscCommon();
	virtual ~CDgnPscCommon();

	// Member Variables.
protected:
	CDBDoc*				 m_pDoc;
	CCRCDataCtrl*  m_pDataCtrl;
	CCRCForceCtrl* m_pForcCtrl;
	CMembCtrl* m_pMembCtrl;
	
	double m_dZero;
    double m_dRatMax;
	double m_PSC_iITER;
	double m_Iteration;
	double m_dPI;
	int    m_iCode;
	UINT   m_iCivilCode;
	int    m_nNatinoAnnex;
	int    m_nBridgeType;

private:
    UINT   m_unChkElem;
    UINT   m_unChkPos;
	ElemPairK m_EPairK;

protected:

	ArrElemPairKey m_arElemK;
	CArray<T_TDNA_K,T_TDNA_K> m_arTdnaK;
	CMap<T_ELEM_K,T_ELEM_K,double,double&>	m_amXibJTG[2];	// See JTG-D62-04 Table 5.2.1 (0=I,1=J).
	std::map<ElemPairK,_PSC_MATL_ELEM> m_amMatlElem;
	std::map<ElemPairK,_PSC_SECT_ELEM> m_amSectElem[2];	      // 0=Strs, 1=Strn. girder
	std::map<ElemPairK,_PSC_SECT_ELEM> m_amSectElem_Compo[2];	// 0=Strs, 1=Strn. composite section
	std::map<ElemPairK,_PSC_SECG_ELEM> m_amSecgElem;          // girder
	std::map<ElemPairK,_PSC_SECG_ELEM> m_amSecgElem_Compo;    // composite section

	std::map<ElemPairK,_PSC_RBAR_ELEM> m_amRbarElem[2];	// 0=Strs, 1=Strn.
	std::map<ElemPairK,_PSC_RSHR_ELEM> m_amRshrElem[2];	// 0=Strs, 1=Strn.
	std::map<ElemPairK,_PSC_TNDN_ELEM> m_amTndnElem[2];	// 0=Strs, 1=Strn.
	CMap<T_TDNA_K,T_TDNA_K,_PSC_TNDN_TDNA,_PSC_TNDN_TDNA&> m_amTndnTdna;
	std::map<ElemPairK,_PSC_EXPC_ELEM> m_amExpcElem;	// 0=Strs, 1=Strn.
	std::map<ElemPairK,_PSC_PSCN_ELEM> m_amPscnElem;
	std::map<ElemPairK,_PSC_PSCS_ELEM> m_amPscsElem;
	std::map<ElemPairK,_PSC_FACK_ELEM> m_amFackElem;
	std::map<ElemPairK,_PSC_CINF_ELEM> m_amCinfElem;
	std::map<ElemPairK,_PSC_SPBF_ELEM> m_amSpbfElem;
    CMap<T_TDNA_K, T_TDNA_K, _DGN_TNDN_TRAN, _DGN_TNDN_TRAN&> m_amTndnTran;
	std::map<ElemPairK,ElemPairK> m_amElemLp;
    std::map<std::pair<ElemPairK, UINT>, UINT> m_amElemPosLp;
    

	std::map<ElemPairK,_PSC_PSCN_IRS_ELEM> m_amPscnElem_IRS;
	_PSC_TNDN_PTIF m_TndnProp;
	// Add by ZINU.('05.09.05). Compare Result with Design (Option).
	BOOL m_bSameConsiderRbar;
	BOOL m_bChangeLcAxOfTapSect;

	// Composite Plate Girder.
	std::map<ElemPairK,CPG_MATL_ELEM> m_mapCpgMatlElem;
	std::map<ElemPairK,CPG_SECT_ELEM> m_mapCpgSectElem;	
	std::map<ElemPairK,CPG_LSTI_ELEM> m_mapCpgLstiElem;
	std::map<ElemPairK,CPG_RBAR_ELEM> m_mapCpgRbarElem;	
	std::map<ElemPairK,CPG_SCON_ELEM> m_mapCpgSconElem;
	std::map<ElemPairK,CPG_TRST_ELEM> m_mapCpgTrstElem;
	std::map<ElemPairK,CPG_TSES_ELEM> m_mapCpgTsesElem; 
	std::map<ElemPairK,CPG_BCDT_ELEM> m_mapCpgBcdtElem; 
	std::map<ElemPairK,CPG_LTBD_ELEM> m_mapCpgLtbdElem;
	std::map<ElemPairK,CPG_MEMB_ELEM> m_mapCpgMembElem;
	std::map<ElemPairK,CPG_FRDT_ELEM> m_mapCpgFrdtElem;  
	std::map<ElemPairK,CPG_TSES_ELEM> m_mapCpgTsesElemIRC; 
	//CMap<T_SECT_K,T_SECT_K,T_CGTS_SSM_D,T_CGTS_SSM_D&>   m_mapCpgCgtsSect;    

    PSC_TDNA_INFO m_TdnaInfo;

	// Coded by Seungjun for Test. ('20060719)
	BOOL m_bTestMode;

public:
	CArray<T_TDNT_K,T_TDNT_K> m_arTdntK;
	CArray<_PSC_TNDN_TDNT,_PSC_TNDN_TDNT> m_arTndnTdnt;
	//++++++++++++++++++++++++++
	// Text File for Testing.
	CString m_strTestSectFile;
	CString m_strTestDgnFile;
	std::map<ElemPairK, ElemPairK> m_aSelElemK;
	//++++++++++++++++++++++++++

	CMap<CString,LPCTSTR,int,int&> m_mStfnNameIndex;

	// Member Functions.
protected:
	double m_dzDimComp;
	BOOL Make_PscMatlElem(int nDgnType=DGN_TYPE_PSC);
    BOOL Make_PscMatlElem_JTG (ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem);
    BOOL Make_PscMatlElem_EURO(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem);
    BOOL Make_PscMatlElem_IRC (ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem);
    BOOL Make_PscMatlElem_AS  (ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D& MatdD, _PSC_MATL_ELEM& MatlElem);
    BOOL Make_PscMatlElem_KSCE(ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem);
    BOOL Make_PscMatlElem_RUS (ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem);
    BOOL Make_PscMatlElem_BS  (ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem);
    BOOL Make_PscMatlElem_IRS (ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem);
	BOOL Make_PscMatlElem_BRA (ElemPairK ElemK, T_PSCD_D PscdD, T_MATD_D MatdD, _PSC_MATL_ELEM& MatlElem);

	BOOL Make_PscSectElem(BOOL bSaveComposite=FALSE);
	BOOL Make_PscSecgElem(BOOL bSaveComposite=FALSE);
	BOOL Make_PscRbarElem();
	BOOL Make_PscRshrElem();
	BOOL Make_PscTndnElem(int nDgnType=DGN_TYPE_PSC);
	BOOL Make_PscTndnTdna();
	BOOL Make_PscTndnTdnt();
	BOOL Make_PscXib_JTG();
	BOOL Make_PscExpcElem();
	BOOL Make_PscPscnElem();
	BOOL Make_PscPscsElem();
	BOOL Make_PscFackElem();
	BOOL Make_PscPscnElem_IRS();
    BOOL Make_PscCinfElem();
    BOOL Make_PscSpbfElem();
    BOOL Make_PscTdnaDgnInfo();
	// Composite Plate Girder
	BOOL Make_CpgMatlElem(); 
	BOOL Make_CpgSectElem();
	BOOL Make_CpgSecgElem();
	BOOL Make_CpgLstiElem();
	BOOL Make_CpgRbarElem();
	BOOL Make_CpgSconElem();
	BOOL Make_CpgTrstElem();
	BOOL Make_CpgCgtsSect();
	BOOL Make_CpgTsesElem();
	BOOL Make_CpgBcdtElem(); // Buckling coefficients for different types of load application.
	BOOL Make_CpgLtbdElem(); // Lateral Torsional Buckling Data
	BOOL Make_CpgFrdtElem();
	BOOL Make_CpgMembElem();
	BOOL Make_CpgTsesElem_IRC22();

	//
	BOOL Get_ClassifiedMatl(CString& strMatName, int& iMatClass);

	// for sod stiffener
	void ConvertSodSectLongStfn();      // Longitudinal Stiffener
	void ConvertSodStiffenerData(int nStype, T_SECT_SECTBASE_D* pSectD, T_STPO_D& StpoD, T_SECT_SOD_STIFF_D* pStfn, ST_SECT_STIFF_D& rData);
	void ConvertSodStfnShapeData(T_STIFF_SHAPE_D* pShape, ST_STIFF_SHAPE_D& rData);
	void ConvertSodStfnProfileData(int nStype, T_SECT_SECTBASE_D* pSectD, T_STPO_D& StpoD, T_SECT_SOD_STIFF_SUB_D& PartStfn, ST_SECT_STIFF_D& rData);
	void ConvertSodStfnProfileData(int nStype, T_STPO_D& StpoD, double dWidth, T_SECT_SOD_STIFF_SUB_D& PartStfn, CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&>& arData);
	void ConvertSodStfnProfileData(T_STPO_D& StpoD, int nRefPos, int nDeckPos, int nDeckPart, double dWidth, T_SECT_SOD_STIFF_DATA_D* pStfnProf, ST_STIFF_PROFILE_D& rData);
	BOOL IsReplaceStfnIndex(int nStype, int nRefPos, int nDeckPos, int nDeckPart);

public:
	BOOL Make_PscDgnData();
	BOOL Make_PscDgnDataForJTG_TndnStress();
	virtual void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl);
	// Bridge Load Rating.
	BOOL Make_BridgeLoadRatingDgnData();
	// Composite Plate Girder.
	BOOL Make_PlateGirderDgnData();
	//++++++++++++++++++++++++++
	// Text File for Testing.
	void SetDataForTest(int iTestKind);
	void PrtTestForSect();
	//++++++++++++++++++++++++++

	// Only JTG-D62-04.
	BOOL Get_MatlTndn(T_MATL_K MatlK, double& dfpk, double& dfpdt, double& dfpdc);
	BOOL Get_MatlRbar(ElemPairK ElemK, double& dfskM, double& dfsdtM, double& dfsdcM, double& dfskS, double& dfsdtS, double& dfsdcS);
	BOOL Get_MatlConc(ElemPairK ElemK, double& dfck, double& dftk, double& dfckp, double& dftkp, double& dfcd, double& dftd, double& dGc);

	BOOL Get_PscMatlElem(            ElemPairK ElemK, _PSC_MATL_ELEM& MatlElem);
	BOOL Get_PscSectElem(BOOL bStrn, ElemPairK ElemK, _PSC_SECT_ELEM& SectElem);
	BOOL Get_PscSecgElem(            ElemPairK ElemK, _PSC_SECG_ELEM& SecgElem);
	BOOL Get_PscRbarElem(BOOL bStrn, ElemPairK ElemK, _PSC_RBAR_ELEM& RbarElem);
	BOOL Get_PscRshrElem(BOOL bStrn, ElemPairK ElemK, _PSC_RSHR_ELEM& RshrElem);
	BOOL Get_PscTndnElem(BOOL bStrn, ElemPairK ElemK, _PSC_TNDN_ELEM& TndnElem);
	BOOL Get_PscTndnTdna(            T_TDNA_K TdnaK, _PSC_TNDN_TDNA& TndnTdna);
	BOOL Get_Xib_JTG(ElemPairK ElemK, BOOL bPosiI, double& dXib);
	BOOL Get_PscExpcElem(            ElemPairK ElemK, _PSC_EXPC_ELEM& ExpcElem);
	BOOL Get_PscPscnElem(            ElemPairK ElemK, _PSC_PSCN_ELEM& ExpcElem);
	BOOL Get_PscPscsElem(            ElemPairK ElemK, _PSC_PSCS_ELEM& ExpcElem);
	BOOL Get_PscFackElem(            ElemPairK ElemK, _PSC_FACK_ELEM& FackElem);

	BOOL Get_CompoPscSectElem(BOOL bStrn, ElemPairK ElemK, _PSC_SECT_ELEM& SectElem);
	BOOL Get_CompoPscSecgElem(            ElemPairK ElemK, _PSC_SECG_ELEM& SecgElem);
	BOOL Get_PscPscnElem_IRS(             ElemPairK ElemK, _PSC_PSCN_IRS_ELEM& ExpcElem);
    BOOL Get_PscCinfElem(ElemPairK ElemK, _PSC_CINF_ELEM& CinfElem);
    BOOL Get_PscSpbfElem(ElemPairK ElemK, _PSC_SPBF_ELEM& SpbfElem);
    BOOL Get_PscTndnTran(T_TDNA_K TdnaK, _DGN_TNDN_TRAN& TranD);
	// Composite Plate Girder
	//BOOL Get_CheckElemUblmKeyList(CArray<T_ELEM_K,T_ELEM_K> &rData);

	BOOL Get_CpgMatlElem(ElemPairK ElemK, CPG_MATL_ELEM& MatlElem) const; 
	BOOL Get_CpgSectElem(ElemPairK ElemK, CPG_SECT_ELEM& SectElem) const;
	BOOL Get_CpgSecgElem(ElemPairK ElemK, CPG_SECT_ELEM& SecgElem);
	BOOL Get_CpgLstiElem(ElemPairK ElemK, CPG_LSTI_ELEM& LstiElem) const;
	BOOL Get_CpgRbarElem(ElemPairK ElemK, CPG_RBAR_ELEM& RbarElem) const;  
	BOOL Get_CpgSconElem(ElemPairK ElemK, CPG_SCON_ELEM& SconElem) const;
	BOOL Get_CpgTrstElem(ElemPairK ElemK, CPG_TRST_ELEM& TrstElem) const;
	BOOL Get_CpgTsesElem(ElemPairK ElemK, CPG_TSES_ELEM& TsesElem) const;
	BOOL Get_CpgBcdtElem(ElemPairK ElemK, CPG_BCDT_ELEM& BcdtElem) const;
	BOOL Get_CpgLtbdElem(ElemPairK ElemK, CPG_LTBD_ELEM& LtbdElem) const;
	BOOL Get_CpgFrdtElem(ElemPairK ElemK, CPG_FRDT_ELEM& FrdtElem) const;
	BOOL Get_CpgMembElem(ElemPairK ElemK, CPG_MEMB_ELEM& MembElem) const;
	void GetPureSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData);
	void GetNonComSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData);
	void GetRebarSectStiffD(const CPG_SECT_ELEM& SectElemD, BOOL bPosI, T_SECT_STIFFNESS& rData) const;
	BOOL GetGirderStageInfo(ElemPairK ElemK, CPG_CSIF_D& NCStageD) const;
    virtual BOOL MakeConvertCsgMemberForce(BOOL bIsPLCB,
                                   ArrElemPairKey& aUblmElemK,
                                   int nLcomId,
                                   ADGNMEMB_FORCE &aDgnMembFor,
                                   ADGNMEMB_FORCE &aDgnMembFor_LT,
                                   ADGNMEMB_FORCE &aDgnMembFor_Pre,
                                   T_STRB_D &MembForNC,
                                   CArray<T_STRB_D,T_STRB_D&> &aStrbElem,
                                   CArray<T_STRB_D,T_STRB_D&> &aStrbElem_LT,
                                   CArray<T_STRB_D,T_STRB_D&> &aStrbElem_Pre,
                                   CSG_MEMB_POSD_KSCE_LSD &MembLcomD
                                   ) const;

	//
	BOOL Get_MatlCpgConc(ElemPairK ElemK, double& dfck, double& dfcm, double& dfctm, double& dEcm);
	BOOL Get_fcd_4EuroCode(int iNA, BOOL bUser, double dAlpha_cc, double dGamma_c_PT, double dGamma_c_A, double dfck, double& dfcd_PT, double& dfcd_A);
	BOOL Get_fyd_4EuroCode(int iNA, BOOL bUser, double dGamma_s_PT, double dGamma_s_A, double dfyk, double dfywk, double& dfyd_PT, double& dfyd_A, double& dfywd_PT, double& dfywd_A);
	BOOL Get_fpd_4EuroCode(int iNA, BOOL bUser, double dGamma_ps_PT, double dGamma_ps_A, double dfp01k, double& dfpd_PT, double& dfpd_A);
	double Get_Gamma_c_4EuroCode(int iNA, int nType);
	double Get_Gamma_s_4EuroCode(int iNA, int nType);
	double Get_Gamma_ps_4EuroCode(int iNA, int nType);
	double Get_Alpha_cc_4EuroCode(int iNA);
	double Get_Alpha_ct_4EuroCode(int iNA);

	BOOL Get_fcd_4IRC112(int iNA, BOOL bUser, double dAlpha_cc, double dGamma_c_PT, double dGamma_c_A, double dfck, double& dfcd_PT, double& dfcd_A);
	BOOL Get_fyd_4IRC112(int iNA, BOOL bUser, double dGamma_s_PT, double dGamma_s_A, double dfyk, double dfywk, double& dfyd_PT, double& dfyd_A, double& dfywd_PT, double& dfywd_A);
	BOOL Get_fpd_4IRC112(int iNA, BOOL bUser, double dGamma_ps_PT, double dGamma_ps_A, double dfp01k, double& dfpd_PT, double& dfpd_A);
	double Get_Gamma_c_4IRC112( int nType);
	double Get_Gamma_s_4IRC112(int nType);
	double Get_Gamma_ps_4IRC112(int nType);
	double Get_Alpha_cc_4IRC112();
	double Get_Alpha_ct_4IRC112();
	double GetFySteelSectIRC( T_MATL_STEEL& SteelMaterial,double dThk);

	BOOL Get_fcd_4BRA(BOOL bUser, double dGamma_c_N, double dGamma_c_SC, double dfck, double& dfcd_N, double& dfcd_SC);
	BOOL Get_fyd_4BRA(BOOL bUser, double dGamma_s_N, double dGamma_s_SC, double dfyk, double dfywk, double& dfyd_N, double& dfyd_SC, double& dfywd_N, double& dfywd_SC);
	BOOL Get_fpd_4BRA(BOOL bUser, double dGamma_ps_N, double dGamma_ps_SC, double dfp01k, double& dfpd_N, double& dfpd_SC);
	double Get_Gamma_c_4BRA(int nType);
	double Get_Gamma_s_4BRA(int nType);
	double Get_Gamma_ps_4BRA(int nType);
	double Get_Alpha_cc_4BRA();
	double Get_Alpha_ct_4BRA();

    // DgnEngine으로 이동하는 것이 좋을 듯.
    BOOL Get_fcd_4BS(BOOL bChar, BOOL bUser, double dGamma_mc, double dfcu, double& dfcd);
    BOOL Get_fyd_4BS(BOOL bChar, BOOL bUser, double dGamma_ms, double dfy,  double& dfyd);
    //BOOL Get_fysd_4BS(BOOL bChar, BOOL bUser, double dGamma_ms, double dfys,  double& dfysd);
    BOOL Get_fpd_4BS(BOOL bChar, BOOL bUser, double dGamma_ms, double dfpu, double& dfpd);    
    BOOL Get_fcd_SLS_4BS(BOOL bChar, BOOL bUser, double dGamma_mc, double dGamma_mt_pre, double dGamma_mt_post, double dfcu, double& dfcd_c, double& dfcd_t_pre, double& dfcd_t_post);

	BOOL CalcSectPSC_QA(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bDir, BOOL baxis, double dpos, double dlen, double& dCutArea, double& dQ);
	// Tendon과 Rebar의 단면의 도심 정보를 계산하여 넘겨주는 함수
	//   bYaxis     : 축 방향(TRUE:Y, FALSE:Z)
	//   dNeuC      : 상단(우측연단)에서 중립축까지의 거리
	//   SpscSect   : 단면정보
	//   arRbarPosi : Rebar 정보
	//   arTndnPosi : Tendon 정보
	//   rTnRb      : Tendon과 Rebar의 단면의 도심 정보를 넘겨 받을 변수
	//   dfsd       : ??? //JTG D62-04용
	BOOL Get_TndnReBarInfor(BOOL bYaxis, double dNeuC, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, 
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, _DGN_TNDN_RBAR& rTnRb, double dfsd=0.0);   
	// Tendon과 Rebar의 좌표값을 중립축을 원점으로 하여 좌표변환 시키는 함수
	//   bYaxis      : 축 방향(TRUE:Y, FALSE:Z)
	//   dMu         : 모멘트값(+:정모멘트, -:부모멘트)
	//   dNueC       : 압축연단에서 중립축까지의 거리
	//   dCp         : 압축연단에서 도심까지의 거리
	//   SpscSect    : 단면정보
	//   arRbarPosi  : Rebar 정보
	//   arTndnPosi  : Tendon 정보
	//   rarRbarPosi : 좌표 변환된 Rebar 정보를 넘겨 받을 변수
	//   rarTndnPosi : 좌표 변환된 Tendon 정보를 넘겨 받을 변수
	BOOL Get_TndnReBarTransform(BOOL bYaxis, double dMu, double dNueC, double dCp, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& rarRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& rarTndnPosi);
	BOOL Get_TndnReBarTransform(BOOL bYaxis, BOOL bPositive, double dNueC, double dCp, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& rarRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& rarTndnPosi);
	BOOL Get_TndnTheta(double dStreX, double dStreY, double& dSinT, double& dThetaR, double& dThetaD);

	BOOL Get_TndnKindrp(int iKind, double& drp);
	BOOL Get_AASHTOTndnValueOfk(int iKind, double& dk);
	//  BOOL Get_Torsion_Pcp_Acp(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, double& dPcp, double& dAcp);
	BOOL Get_PrincipalExtraRatio(ElemPairK ElemK, double* dAlwStress);
	// For Tendon Stress.
	// wg_db, SectUtil.h로 이동. 
	//BOOL Get_PolygonCuttingLine(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bDir, BOOL baxis, double dpos, double dlen, CArray<T_SPSC_COOR,T_SPSC_COOR>& arGetOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arGetInner);
	BOOL Get_TorsionSectProperty4OpenShape(int nPart, double* dpDivPos, double dLength, double dCover, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, double* dpIxx, double* dpAoh, double* dpPh, double* dpPcp, double* dpAcp, double* dpyDim, double* dpzDim);
	BOOL Get_TorsionSectProperty4OpenShape_Part(BOOL bDirection, double dCutPos, double dLength, double dCover, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter,  
		double& dIxx, double& dAoh, double& dPh, double& dPcp, double& dAcp, double& dyDim, double& dzDim);

	BOOL Get_SectParam4CheckStrength(_PSC_SECT_POSI& SectPosi, double& dbw, double* dShrThk, double* dFlgThk, double dpMomPos[][2]);

	//!/ Test... 작업후 Sect관련 Class 이동. 
	//
	//BOOL Get_EffectiveSectData(T_PART_SSM& PartSSM);    
	BOOL Get_CpgSectProperty(BOOL bStiffener, ElemPairK ElemK, BOOL bPosiI, const T_SECT_SECTBASE_D& SectBase, T_SECT_STIFFNESS& AfterStiff, T_GENL_DATA& GLineData, T_SECT_STIFFNESS* aSpscSect);
	BOOL Get_EdgeData(T_GSEC_POLYGON& GsecPolyD, CArray<T_GSEC_LINE, T_GSEC_LINE&>& aLine);  

	BOOL Get_GenSectLinetypeData(T_SECT_SECTBASE_D& SectBase, T_GENL_DATA& rData);
	BOOL Get_GenLineDataFromGLineDgn(const T_GLINE_DGN_D& GLineDgn, T_GENL_DATA& rData);
	BOOL Get_ConvertSectBase4GenLineData(const T_SECT_SECTBASE_D& SectBaseD, const T_GLINE_DGN_D& GLineDgnD, T_SECT_SECTBASE_D& rData);
	double Calc_StressOfPosition(double dFx, double dMy, double dMz, double dArea, double dIy, double dIz, double dy, double dz);
	BOOL Get_ShapeInfoOfDgnLineData(T_GLINE_DST& rData, double& db, double& dt, BOOL& bStfn);
	BOOL Is_CompStress(double dStr1, double dStr2);
	BOOL Is_DoubleCurvature(double dStr1, double dStr2);
	int Get_StressDistributionType4Internal(double dStr1, double dStr2);
	int Get_StressDistributionType4Outstand(BOOL bSig1IsFree, double dStr1, double dStr2);
	BOOL Get_f1_f2(double dStr1, double dStr2, double& df1, double& df2, BOOL& bIs1_Sig1); // f1이 압축이면서 f1>f2
	double Get_traiangleRatioVal(double dval1, double dVAL1, double dVAL2);
	double Get_yOfStrightLine(double da, double db, double dx);
	BOOL Get_b1b2DbCurvature(double dLength, double df1, double df2, double& db1, double& db2);
	BOOL Get_GirderStiffnessofCompo(BOOL bPosiI, T_SECT_D& SectD, T_SECT_STIFFNESS& rData) const;
	BOOL Get_GirderStiffnessofCompo(BOOL bPosiI, T_SECT_D& SectD, T_SECT_DESIGN& rData) const;
	void Get_SectionSizeOfSect_I(double* aSize, double& dH, double& dtw, double& dB1, double& dtf1, double& dB2, double& dtf2, double& dh);
	double Get_InertiaOfSB(double dh, double db);
	BOOL Get_StiffnessOfEffeSect(CArray<T_STIFF_CALC_D, T_STIFF_CALC_D&>& aStiffCalcD, double& dCzm, double& dIy);
	BOOL Get_StiffnessOfSB(double Bs, double Bc, double tc, int iNg, T_SECT_STIFFNESS& rData);
	BOOL Get_ConvertRbarPoint4Org(double dCym, double dCzm, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi);
	BOOL Get_ConvertRbarCrcToGsecRbar(_DGN_RBAR_CRC& RbarCrc, T_GSEC_RBAR& rData);
	BOOL Get_ConvertPointFromCentroid(double dYbar, double dZbar, T_SECT_SECTBASE_D& SectBaseD, T_GLINE_DGN_D& rData);
	int Get_SectShapeType(T_SECT_D& rData);
	BOOL Get_CompositeSlabSection(ElemPairK ElemK, double& dBc, double& dtc, double& dHh);

	BOOL Get_PscDataPosi(ElemPairK ElemK, int nIJ, PSC_DATA_POSI& InData, BOOL bIsCompo=FALSE);
	BOOL Get_PscTndnPropData(PSC_TNDN_PTIF &TnPropD);
    BOOL Get_PscTdnaInfo(PSC_TDNA_INFO& rData);
    BOOL Get_PscTdnafpsInfo(PSC_TDNA_INFO& rData);

	int GetConvertCodeCvlToEng(int nDgnCode);
	int GetConvertCodeEngToCvl(int nDgnCode);

	int GetConvertCSGCodeCvlToEng(int nDgnCode);
	int GetConvertCSGCodeEngToCvl(int nDgnCode);

	int GetConvertSodCodeCvlToEng(int nDgnCode);
	int GetConvertSodCodeEngToCvl(int nDgnCode);
	//PMS:RATING BSC-20140404
	double MinWithoutZero(const double &dVal1, const double &dVal2, const double &dVal3);

	void Get_rElemK(CArray<T_ELEM_K,T_ELEM_K>& rElemK);

	void ConvertCsgCalcUlmForce(T_STRB_D &MembD, CSG_ULMF_D &rData) const;
	void ConvertCsgCalcUlmForceCb(T_STRB_D &MembD, int nMid, int nCom, CSG_ULMF_D &rData) const;
	BOOL GetUlmMidIndex(T_STRB_D &MembD, int &nMid, int &nCom) const;

	enSupportType GetSupportType(BOOL bSupport, BOOL bInteriorSupport);
	BOOL GetEndElemOfMember(ElemPairK EPairK, ElemPairK& ShowK, ElemPairK& IendK, ElemPairK& JendK) const;

protected:  
	BOOL IsTndnGroupCheck(int nDgnCode);

	// Eurocode2-2:05
	BOOL Get_MatlTndnData4EC2(T_MATL_K MatlK, double& dFp01, double& dfp01k);
	BOOL Get_MatlTndnData4SNiP(int nBridgeType, T_MATL_K MatlK, double dfpy, double& dfpd);
    CString GetRCMatlCodeName(int nDgnCode);
    BOOL Get_Unit4CalcEc(int nDgnCode, T_UNIT_INDEX& UnitIndex);
    double Calc_Ec(int nDgnCode, double dfc);

	BOOL MakeUnbracedLengthMembData();
	BOOL MakeUnbracedLengthPosition();
	BOOL GetCheckElemKey4UnbracedLength(CArray<T_ELEM_K,T_ELEM_K> &aChkElemKey);

	BOOL CalcStiffnessOfStiffener(T_SECT_STIFF_D &Stfn, ST_SECT_STIFF_D &rData);  
    BOOL GetElementList4TendonLp(ArrElemPairKey& aElemKey);
    BOOL GetCheckElemListAppTendonLp(const ArrElemPairKey& aChkElemKey, ArrElemPairKey& aElemKey);
	void ConvertMapToSortedArray(const std::map<ElemPairK, ElemPairK>& mElemPairK, ArrElemPairKey& outArr);


	S_PSCD_D m_SPscdD;  
	void ConvertPscParamData(const T_PSCD_D& PscdD, S_PSCD_D& rData);  
    void ConvertPscParamASPM(const T_ASPM_D& AspmD, S_PSCD_D& rData);
	void ConvertPscCalcData(ElemPairK ElemK, int nIJ, BOOL bCompositeGen, _PSC_MATL_ELEM* pMatlD, const _PSC_SECT_POSI* pSectD, const T_SPSC_SECT* pSectgD, 
		_PSC_RBAR_POSI* pRbarD, const _PSC_RSHR_POSI* pRshrD,  _PSC_TNDN_POSI* pTndnD, const _PSC_EXPC_POSI* pExpcD, _PSC_SPBF_POSI*pSpbf, PSC_DATA_POSI& rData);

	BOOL ConvertPscCalcShearSectData(ElemPairK ElemK, int nIJ, PSC_SECT_POSI& pSectD);
	void ConvertPscCalcDataPreD(ElemPairK ElemK, int nIJ, BOOL bCompositeGen, const _PSC_SECT_POSI* pSectD, const T_SPSC_SECT* pSectgD, PSC_DATA_POSI& rData);

	void ConvertPscCalcMatlData(int iDgnCode, _PSC_MATL_ELEM* pMatlD, PSC_MATL_ELEM& rData);
	BOOL ConvertPscCalcSectData(ElemPairK ElemK, int nIJ, BOOL bCompositeGen, const _PSC_SECT_POSI* pSectD, PSC_SECT_POSI& rData);
	void ConvertPscCalcSectgData(const T_SPSC_SECT* pSectgD, DGN_SPSC_SECT& rData);
	void ConvertSectStiffnessData(T_SECT_STIFFNESS &rStiff, DGN_SECT_STIFFNESS &rData);
    void ConvertSectStiff4Warping(const T_SECT_STIFFNESS &rStiff, DGN_SECT_WARPING_STIFF &rData);
    void ConvertCompSectStiffness4Warping(int nPos, const T_SECT_D& rSectD, CSG_SECT_POSD &rData);
	void ConvertPscCalcRbarData( _PSC_RBAR_POSI* pRbarD, DGN_RBAR_POSI& rData);
	void ConvertPscCalcRshrData(const _PSC_RSHR_POSI* pRshrD, PSC_RSHR_POSI& rData);
	void ConvertPscCalcTndnData( _PSC_TNDN_POSI* pTndnD, PSC_TNDN_POSI& rData);
	void ConvertPscCalcRbarCrcData(const _DGN_RBAR_CRC* pRbarD, DGN_RBAR_CRC& rData);
	void ConvertPscCalcRpscSbarData(const T_RPSC_SBAR* pSbarD, DGN_RPSC_SBAR& rData);
	void ConvertPscCalcTndnCrcData(const _DGN_TNDN_CRC* pTndnD, DGN_TNDN_CRC& rData);
	void ConvertPscCalcTdnaData(const _PSC_TNDN_TDNA* pTdnaD, PSC_TNDN_TDNA& rData);  
    void ConvertPscCalcTndnTranToTdna(const _DGN_TNDN_TRAN* pTran, PSC_TNDN_TDNA& rData);
	void ConvertPscCalcPescData(const _PSC_EXPC_POSI* pExpcD, PSC_DATA_POSI& rData);
	void ConvertPscnData(int nIJ, const _PSC_PSCN_ELEM& PscnElem, PSC_DATA_POSI& rData);
	void ConvertPscsData(int nIJ, const _PSC_PSCS_ELEM& PscsElem, PSC_DATA_POSI& rData);
	void ConvertFackData(int nIJ, const _PSC_FACK_ELEM& FackElem, PSC_DATA_POSI& rData);
	void ConvertPscCalcTndnPropData(const _PSC_TNDN_PROP *pTnptD, PSC_TNDN_PROP &rData);
	void ConvertPscCalcTndnPropInfo(const _PSC_TNDN_PTIF *pTnptInfo, PSC_TNDN_PTIF &rData);
	void ConvertPscnData_IRS(int nIJ, const _PSC_PSCN_IRS_ELEM& PscnElem, PSC_DATA_POSI& rData);
    void ConvertPscCalcInfo(const _PSC_CINF_ELEM* pCinfD, PSC_DATA_POSI& rData);
    void ConvertPscLengthFromEnd(_PSC_SPBF_POSI*pSpbf, PSC_EINF_POSI& rData);
	void ConvertReinforcedSectInfo(int nPos, const T_SECT_D& rSectD, CSG_SECT_POSD& rData);
	void ConvertReinforcedSectRivetProp(const T_SECT_RIVET_STIFF_D& ReinSect, CSG_SECT_POSD& rData);
	void ConvertCorrosionSectInfo(int nPos, const T_SECT_D& rSectD, const T_CRDP_SSM_D& rCrdp, CSG_SECT_POSD& rData);
	void ConvertCorrosionSectBase(int nPos, const T_SECT_D& rSectD, const T_CRDP_SSM_D& rCrdp, CSG_SECT_POSD& rData);

	int     GetConvertSectTypeCvlToEng(int nSectType);
	CString GetConvertSectShapeCvlToEng(CString &strShape);
	BOOL    IsSecTypeI(int nSectType);
	BOOL    IsSectTypeWith2Web(int nSectType);

protected:
	// For Unit.
	double Cng_LengthUnit(T_UNIT_INDEX FromUnit, T_UNIT_INDEX ToUnit, double dLength, int iUp);
	double Cng_ForceUnit(T_UNIT_INDEX FromUnit, T_UNIT_INDEX ToUnit, double dForce);
	double Cng_StressUnit(T_UNIT_INDEX FromUnit, T_UNIT_INDEX ToUnit, double dStress);
    BOOL   IsCheckPosition(int nPos) const;

    void   SetCheckElem(UINT unElem);
    UINT   GetCheckElem() const;
    void   SetCheckPosition(UINT unPos);
    UINT   GetCheckPosition() const;
	UINT   GetCheckPositionIndex(BOOL bPosi) const;
	void   SetCheckElemPairK(ElemPairK EPairK);
	ElemPairK GetCheckElemPairK() const;

 private:
     void CalcLengthFromEnd(double dSpanLength, _PSC_SPBF_POSI& rData);
	 double Get_fctm_EC2(double dfck, double dfcm);
     dgn::lib::CNationalAnnexInterface* m_pNA;
};

#endif // !defined(AFX_DGNPSCCOMMON_H__E5183771_6C1E_4BED_96C9_7C96278A0370__INCLUDED_)
