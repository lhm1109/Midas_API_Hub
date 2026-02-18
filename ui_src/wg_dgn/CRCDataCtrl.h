// CRCDataCtrl.h: interface for the CCRCDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRCDATACTRL_H__6AC60648_9789_4D89_BE08_66EF09500ECF__INCLUDED_)
#define AFX_CRCDATACTRL_H__6AC60648_9789_4D89_BE08_66EF09500ECF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRCForceCtrl.h"
#include "Dgn_ServiceDef.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
#include "..\wg_db\DB_ST_DN_RESULT_RATING.h"
#include "..\wg_db\DB_ST_DN_RESULT_CH.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\DB_ST_DT_ELEM.h"


class CDesignForceResult;
class CDgnResultFileCtrl;

struct D_XL_PROGRESS_INTERFACE;
struct PSC_DATA_MEMB;
struct PSC_DATA_POSI;

struct _LCOM_FOR_B;
struct _BDCW_PSC_LRFD_BASE;
struct PSC_BDCW_CR_PROP_R;
struct PSC_FRCR_D;
struct _BDCY_PSC_LRFD_BASE;
struct _CUMS_AASHTO_BASE;

struct _DGN_TDGR_CRC
{
    T_TDGR_K DgnTdgrK;
    T_TDGR_K OrgTdgrK;
    T_TDNA_K TdnaK;
    CString strTdgrNa;
    void Initialize()
    {
        DgnTdgrK=0;
        OrgTdgrK=0;
        TdnaK=0;
        strTdgrNa=_T("");
    }
};

struct _DGN_LCBA_CRC
{
    // 1st(0=전사하중시, 1=설계시, 2=온도시, 3=지진시), 2nd(0=I-MAX,1=I-MIN,2=J-MAX,3=J-MIN), 3rd(0=TOP,1=BOT).
    int iLcomLcbaMy[4][4][2];
    int iLcomLcbaFz[4][4];
    int iLcomLcbaMx[4][4];
    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                for(int k=0; k<2; k++) iLcomLcbaMy[i][j][k]=0;
                iLcomLcbaFz[i][j]=0;
                iLcomLcbaMx[i][j]=0;
            }
        }
    }

};

struct _DGN_RBAR_CRC
{
    CString strRbarNa;
    double dArea, dDia;
    double dy, dz;  // 콘크리트 단면에 대한 도심(0=y,1=z)
    int    nPart;
    void Initialize()
    {
        strRbarNa.Empty();
        dArea=0.0; dDia=0.0;
        dy=0.0; dz=0.0;
        nPart =0;
    }
};

struct _DGN_TNDN_CRC
{
    T_MATL_K MatlK;				// Tndt(Property) MatlK.
    CString strMatlName;  // Material Name
    CString strTendonName;
    CString strTndnGrup;
    T_TDNA_K KeyTdna;     // Single일 경우만  사용.
    double dTndnNum;			// Tndn 개수.
    int iTndnType;				// 1:Internal, 2:External
    int iBondType;				// 0:Bonded, 1:Unbonded
    int iLoadType;	      // 0:Pre-tension, 1:Post-tension
    int iRelxType;				// KSCE-USD03 (0=저릴렉세이션(fpy/fpu>=0.90), 1=응력제거(fpy/fpu>=0.85), 전단강봉(fpy/fpu>=0.80)).
    // JTG-D62-04 (0=Strand, 1=Wire, 2=Steelbar).
    double dEps, dfy, dfpu, dfpy;
    double dArea;
    double dy, dz;				// 콘크리트 단면에 대한 도심(0=y,1=z)
    double dzAng;
    double dStressLL;
    double dStress[3];		// 프리스트레스 분력(0=x,1=y,2=z)
    // JTG-D62-04.
    double dfpk, dfpdt, dfpdc;	// 표준값, 인장설계값, 압축설계값.
    double dEffDia;							// 유효직경 de=sqrt(n)*d.
    // Change by Jaeoh. ('06.09.25) 1=Relaxation, 2=Creep/Shrinkage --> 1=Creep/Shrinkage, 2=Relaxation
    double dStrLoss[3][3];			// 프리스트레스 손실응력(0=x,1=y,2=z)(0=Elastic, 1=Creep/Shrinkage, 2=Relaxation)
    //Eurocode2-2:05,
    double dFp01; // 0.1% proof force
    double dfp01k; // fp0.1k, 0.1% proof stress
    double dfpd_PT, dfpd_A; // fpd=fp0.1k/Gamma_s , Eurocode2-2:05
    double dEpsi_pd; // fpd/Ep, Eurocode2-2:05
    double dLength; // Single일 경우만 사용. 
    int iBoundaryNum; // Single일 경우만 사용. 
    double dDuctDia; // duct diameter for unbonded tendon
    BOOL bReverse;
    BOOL bDebond;
    double dTranRatio;
    double dLengthElem; // 누적 요소길이
    BOOL bFrom_I;       // I단 기준

    void Initialize()
    {
        MatlK=0;
        strMatlName.Empty();
        strTendonName.Empty();
        strTndnGrup.Empty();
        KeyTdna=0;
        dTndnNum=0.0;
        iTndnType=0;
        iBondType=0;
        iLoadType=0;
        iRelxType=0;
        dEps=dfy=dfpu=dfpy=0.0;
        dArea=0.0;
        dy=0.0; dz=0.0;
        dzAng = 0.0;
        dfpk=0.0; dfpdt=0.0; dfpdc=0.0;
        dEffDia=0.0;
        dStressLL=0.0;
        memset(dStress,0,sizeof(dStress));
        memset(dStrLoss,0,sizeof(dStrLoss));
        dFp01=dfp01k=0.0;
        dfpd_PT=dfpd_A=0.0;
        dEpsi_pd=0.0;
        dLength = 0.0;
        iBoundaryNum=0;
        dDuctDia=0.0;
        bReverse = FALSE;
        bDebond = FALSE;
        dTranRatio = 0.0;
        dLengthElem = 0.0;
        bFrom_I = TRUE;
    }
    _DGN_TNDN_CRC() {Initialize();}

};

struct _DGN_TNDN_TRAN_PN
{
    double dfps_Lp;

    void Initialize()
    {
        dfps_Lp = 0.0;
    }
    _DGN_TNDN_TRAN_PN() { Initialize(); }
};
struct _DGN_TNDN_TRAN_INF
{
    ElemPairK ElemLptK;  // Transmission(Lpt)
    BOOL     bLptI_End;
    ElemPairK ElemLpK;   // Developement Length (Lp)
    BOOL     bLpI_End;
    double   dfpe_Lpt;
    ArrElemPairKey aElemLptToLp; // Lpt ~ Lp 사이 요소
    //double   dfpe_Lp;    
    _DGN_TNDN_TRAN_PN Pos;
    _DGN_TNDN_TRAN_PN Neg;

    void Initialize()
    {
        ElemLptK.first = 0;
        ElemLptK.second = EN_EL_BEAM;
        bLptI_End = TRUE;
        ElemLpK.first = 0;
        ElemLpK.second = EN_EL_BEAM;
        bLpI_End  = TRUE;
        dfpe_Lpt  = 0.0;
		aElemLptToLp.RemoveAll();
        Pos.Initialize();
        Neg.Initialize();
    }
    _DGN_TNDN_TRAN_INF() { Initialize(); }
	_DGN_TNDN_TRAN_INF(const _DGN_TNDN_TRAN_INF& rData) { *this = rData; }
	_DGN_TNDN_TRAN_INF& operator = (const _DGN_TNDN_TRAN_INF& rData)
	{
		ElemLptK  = rData.ElemLptK;
		bLptI_End = rData.bLptI_End;
		ElemLpK   = rData.ElemLpK;
		bLpI_End  = rData.bLpI_End;
		dfpe_Lpt  = rData.dfpe_Lpt;
		aElemLptToLp.Copy(rData.aElemLptToLp);
		Pos       = rData.Pos;
		Neg       = rData.Neg;
		return *this;
	}

};

struct _DGN_TNDN_TRAN
{
    _DGN_TNDN_TRAN_INF TranB[2]; // Tendon의 끝단에서 정보 

    void Initialize()
    {
        for ( int i=0 ; i<2; ++i )
        {
            TranB[i].Initialize();
        }        
    }
    _DGN_TNDN_TRAN() { Initialize(); }
};

// Composite Plate Girder
// Longitudinal Stiffener
struct CPG_LSTI_DATA
{
    double  dz;         // Top(web시작부분) (default :0, boundary >0 )  
    double  dWidth;     // (default :0, boundary >0 ) Unit:Length
    double  dt;         // (default :0, boundary >0 ) Unit:Length
    double  dArea;
    double	dCzp;
    double  dCzm;
    double  dIyy;
    double	dCyp;
    double  dCym;
    double  dIzz;

    void Initialize()
    {
        dz		= 0.0;
        dWidth	= 0.0;
        dt		= 0.0;
        dArea	= 0.0;
        dCzp	= 0.0;
        dCzm	= 0.0;
        dIyy	= 0.0;
        dCyp	= 0.0;
        dCym	= 0.0;
        dIzz	= 0.0;
    }

    CPG_LSTI_DATA& operator=(const CPG_LSTI_DATA& rD)
    {
        dz		= rD.dz		;
        dWidth	= rD.dWidth	;
        dt		= rD.dt		;
        dArea	= rD.dArea	;
        dCzp	= rD.dCzp	;
        dCzm	= rD.dCzm	;
        dIyy	= rD.dIyy	;
        dCyp	= rD.dCyp	;
        dCym	= rD.dCym	;
        dIzz	= rD.dIzz	;

        return *this;
    }
};

struct CPG_FORC_STAG
{
    _DGN_FORC_CRC MaxForce;
    _DGN_FORC_CRC MinForce;
    void Initialize()
    {
        MaxForce.Initialize();
        MinForce.Initialize();
    }
    CPG_FORC_STAG& operator = (const CPG_FORC_STAG& rData)
    {
        MaxForce=rData.MaxForce;
        MinForce=rData.MinForce;
        return *this;
    }
};

struct BLR_SPAN_D
{
    T_SPAN_K SpanK;     // Span Key.. DB에 있는값. 
    UINT iL_count;      // 몇 번째 span인가. SpanK 수와 iL_Count는 다를 수 있다.
    double dLength;     // 요소길이 
    double dElemI_Len;  // Span 시작점에서(iL_Count에 의한)떨어진 거리.
    double dElemJ_Len;  
    double dElemI_Rat;  // Span 전체길이에 대한 I단까지의 비율 (dElemI_Len/Span길이)
    double dElemJ_Rat;

    void Initialize()
    {
        SpanK = 0;
        iL_count = 0;
        dElemI_Len = 0.0;
        dElemJ_Len = 0.0;
        dElemI_Rat = 0.0;
        dElemJ_Rat = 0.0;
    }
};

struct BLR_FLEX_DATA
{
    double dPhi[4]; // 0=I-Positive, 1=I-Negative, 2=J-Positive, 3=J-Negative
    double dMn[4];
    double dArea_bar[4]; // 철근면적
    double dArea_ten[4]; // Tendon면적
    BOOL bBarRat_Max[4]; // 최대철근비 Check
    BOOL bBarRat_Min[4]; // 최소철근비 Check

    void Initialize()
    {
        for(int i=0; i<4; i++)
        {
            dPhi[i] = 0.0;          
            dMn[i]  = 0.0;
            dArea_bar[i] = 0.0;     
            dArea_ten[i] = 0.0;     
            bBarRat_Max[i] = FALSE; 
            bBarRat_Min[i] = FALSE; 
        }
    }
};

struct DGN_JOINT_D
{
    int iI_joint; // 0=other area, 1=joint
    int iJ_joint;
    int iJointType;
    void Initialize()
    {
        iI_joint = 0;
        iJ_joint = 0;
        iJointType = 0;
    }
    DGN_JOINT_D() {Initialize(); }
};

struct DGN_RENI_BASE
{
    ArrElemPairKey arElemK; // Node에 연결된 Element.
    CArray<UINT,UINT> arIndexIJ; // Element의 node. 0=I, 1=J
    void Initialize()
    {
        arElemK.RemoveAll();
        arIndexIJ.RemoveAll();
    }  
    DGN_RENI_BASE() {Initialize();}
    DGN_RENI_BASE(const DGN_RENI_BASE& rData) {*this=rData;}
    DGN_RENI_BASE& operator = (const DGN_RENI_BASE& rData)
    {
        arElemK.Copy(rData.arElemK);
        arIndexIJ.Copy(rData.arIndexIJ);
        return *this;
    }

};
struct DGN_RENI_D // Rating Element & Node Information
{
    DGN_RENI_BASE ReniBase[2]; // I,J

    void Initialize()
    {
        for(int i=0; i<2; i++) ReniBase[i].Initialize();
    }
    DGN_RENI_D() {Initialize();}  
    DGN_RENI_D(const DGN_RENI_D& rData) {*this=rData;}
    DGN_RENI_D& operator = (const DGN_RENI_D& rData)
    {
        for(int i=0; i<2; i++) ReniBase[i] = rData.ReniBase[i];
        return *this;
    }

};

struct T_ULEN_MEMB_D
{
    ElemPairK ElemI;
    ElemPairK ElemM;
    ElemPairK ElemJ;
    int      nIpos; // 0=I,1=J
    int      nMpos; // 0=I, 1=1/4, 2=2/4, 3=3/4, 4=J
    int      nJpos;
    BOOL     bJsupport;
    double   dLength;
    BOOL     bCantilever;
    ArrElemPairKey aElemK;
    EN_BEAM_DGN_TYPE enBType;

    void Initialize()
    {
        ElemI.first = 0;
        ElemI.second = EN_EL_BEAM;
        ElemM.first = 0; 
        ElemM.second = EN_EL_BEAM;
        ElemJ.first = 0;
        ElemJ.second = EN_EL_BEAM;
        nIpos=nMpos=nJpos=0;
        bJsupport = FALSE;
        dLength=0.0;
        bCantilever=FALSE;
        aElemK.RemoveAll();
        enBType = EN_EL_BEAM;
    }

    T_ULEN_MEMB_D() {Initialize();}
    T_ULEN_MEMB_D(const T_ULEN_MEMB_D& rData) { *this = rData; }
    T_ULEN_MEMB_D& operator=(const T_ULEN_MEMB_D& rData) 
    {
        ElemI   = rData.ElemI;
        ElemM   = rData.ElemM;
        ElemJ   = rData.ElemJ;
        nIpos   = rData.nIpos;
        nMpos   = rData.nMpos;
        nJpos   = rData.nJpos;
		bJsupport = rData.bJsupport;
        dLength = rData.dLength;
        bCantilever = rData.bCantilever;
        aElemK.Copy(rData.aElemK);
        enBType = rData.enBType;

        return *this;
    }
};

struct T_CONC_AGE_D
{
    bool   bTdmeGr;
    UINT   TdmeKGr;
    double dAgeGr;
    
    bool   bTdmeSl; 
    UINT   TdmeKSl;
    double dAgeSl;
    
    void Initialize()
    {
        bTdmeGr = false;
		TdmeKGr = 0;
        dAgeGr = 0.0;
        
        bTdmeSl = false;
		TdmeKSl = 0;
        dAgeSl = 0.0;
    }
    T_CONC_AGE_D() {Initialize();}
};

class CDBDoc;
class CAttrCtrl;
class CAttrCtrl2;
class CUnitCtrl;
class CPostCtrl;
class CMembCtrl;
class CStageInfo;
class CTendonInfo;

class CDgnJTG_D62_04;
class CRptJTG_D62_04;
class CDgnKSCE_USD03;
class CPrtKSCE_USD03;
class CDgnKSCE_USD05;
class CPrtKSCE_USD05;
class CPrtKSCE_USD10;
class CDgnBridgeLoadRating;
class CDgnRating_KSCE_USD05;
class CDgnPlateGirder;
class CDgnPlateGirder_IRC;
//class CDgnAASHTO_LRFD08;
class CDgnPscDataCtrl;
class CDgnCsgDataCtrl;
class CDgnStlRatingDataCtrl;
class CDgnRCRatingDataCtrl;
class CDgnSodDataCtrl;
class CDgnRUSStlCompDataCtrl;
class CDgnPlateGirder_IRC22_15;
class CDgnCsgDataCtrlAS5100_6_2017;


class CCRCDataCtrl
{
public:
    CCRCDataCtrl();  
    virtual ~CCRCDataCtrl();

    // Member Variables.
public:
    CDgnPscDataCtrl* m_pPscDataCtrl;
    //CDgnAASHTO_LRFD08* m_pAASHTO_LRFD08;  
    CDgnCsgDataCtrl *m_pCsgDataCtrl;
    CDgnStlRatingDataCtrl *m_pStlRatingDataCtrl;
    CDgnSodDataCtrl *m_pSodDataCtrl;
    CDgnRUSStlCompDataCtrl *m_pRusStlCompDataCtrl;
    CDgnRCRatingDataCtrl *m_pRCRatingDataCtrl;

protected:
    CCRCForceCtrl m_ForceCtrl;
    CDgnKSCE_USD03* m_pKSCE_USD03;
    CDgnKSCE_USD05* m_pKSCE_USD05;
    CDgnJTG_D62_04* m_pJTG_D62_04;
    CDgnBridgeLoadRating* m_pLoadRating;
    CDgnRating_KSCE_USD05* m_pRatingKS05;
    CDgnPlateGirder* m_pPlateGirder;
    CDgnPlateGirder_IRC* m_pPlateGirder_IRC;
    CDgnPlateGirder_IRC22_15* m_pPlateGirder_IRC22_15;

    T_UNIT_INDEX m_CurrUnit, m_CodeUnit;
    double m_dZero;
    double m_dPi;
    std::map<ElemPairK,_DGN_LCBA_CRC> m_amLcbaD;

    int m_iCivilCode;  
    //
    CDBDoc *m_pDoc;
    CAttrCtrl  *m_pAttrCtrl;
    CAttrCtrl2 *m_pAttrCtrl2;
    CUnitCtrl  *m_pUnitCtrl;
    CMembCtrl* m_pMembCtrl;
    CPostCtrl* m_pPostCtrl;
    CStageInfo *m_pStageInfo;
    CTendonInfo *m_pTendonInfo;

    CCurUnitSaver m_UnitSaver;

public:
    BOOL m_bCreateDgnFile;
    CMap<T_TDNA_K,T_TDNA_K,_DGN_TDGR_CRC,_DGN_TDGR_CRC&> m_amTdnaK;
    CMap<T_TDGR_K,T_TDGR_K,_DGN_TDGR_CRC,_DGN_TDGR_CRC&> m_amTdgrK;

    // Member Functions.
protected:
    // Get Checking Flag.
    BOOL Get_ChkFlag(ElemPairK ElemK, int iPosi, BOOL bIsMom, int nChkType=-1);
    BOOL Is_CalcItem(ElemPairK ElemK, BOOL bIsMom);
    int Get_RelxType(T_TDNT_D& TdntD);
    // Get Section Aw.
    double Get_AwCmpWeb(ElemPairK ElemK, BOOL bPosI);
    //
    ArrElemPairKey m_arCheckElemK;
    std::map<ElemPairK,UINT> m_mapSerialElemK;
    std::map<ElemPairK,DGN_JOINT_D> m_mapJoint; // For AASHTO-LRFD08, 

    std::map<ElemPairK,T_BDCY_JTG> m_mapResultBDCY_JTG;
    CMap<T_TDNA_K, T_TDNA_K&, double, double&> m_mapTotElemLength4Tdna;

    //
    int m_iDgnElemNum;
    int m_iDgnLcomNum;  
    BOOL m_bChangeLcAxOfTapSect;
public:
    void Set_SerialCheckElemData();
    UINT Get_SerialCheckElemData(ElemPairK ElemK);
    int  Get_DgnElemNum();
    int  Get_DgnLcomNum();  
    BOOL SetCheckElementData(ArrElemPairKey& arCheckElemK);
    BOOL GetCheckElementData(ArrElemPairKey& arCheckElemK);

    void SetChangeLocalAxisOfTapSect(BOOL bChageLocalAxis);

public:
    int Get_Code();
    int Get_CivilCode();
    CCRCForceCtrl* Get_ForceCtrlPointer();
    CDgnKSCE_USD03* Get_DgnKscePointer();
    CPrtKSCE_USD03* Get_PrtKscePointer();
    CDgnJTG_D62_04* Get_DgnJtgPointer();
    //CRptJTG_D62_04* Get_RptJtgPointer();
    CDgnKSCE_USD05* Get_DgnKsce05Pointer();
    CPrtKSCE_USD05* Get_PrtKsce05Pointer();
    CPrtKSCE_USD10* Get_PrtKsce10Pointer();
    CDgnBridgeLoadRating* Get_BridgeLoadRatingPointer();
    CDgnRating_KSCE_USD05* Get_RatingKS05Pointer();
    CDgnPlateGirder* Get_PlateGirderPointer();
    CDgnPlateGirder_IRC* Get_PlateGirderPointer_IRC();
    //CDgnAASHTO_LRFD08* Get_AASHTO_LRFD08Pointer();
    //CDgnEurocode2_2_05* Get_Eurocode2_05Pointer();
    CDgnCsgDataCtrl* Get_CsgDataCtrlPointer();
    CDgnStlRatingDataCtrl* Get_RatingCsgDataCtrlPointer();
    CDgnRCRatingDataCtrl* Get_RatingRCDataCtrlPointer();
    CDgnSodDataCtrl* Get_SodDataCtrlPointer();
    CDgnRUSStlCompDataCtrl* Get_RUSStlCompDataCtrlPointer();
    CDgnPlateGirder_IRC22_15* Get_PlateGirderPointer_IRC22_15();
    // Matl.
    BOOL Get_ConMatd(T_MATD_D& mData, BOOL bCompositeSect=FALSE);
    BOOL Get_SrcMatd(T_MATD_D& mData);
    BOOL Get_StlMatd(T_MATD_D& mData);
    BOOL Get_RbarMatl(BOOL bMbar, T_MATD_D& MatlD);  
    // Tdgr.
    BOOL Cal_TdgrDataForDesignAll();
    BOOL Get_CrcSpscTdgr(ElemPairK ElemK, BOOL bPosI, double dMuyMax, double dMuyMin, _DGN_TDGR_CRC& TdgrCrc, T_SPSC_TNDN& SpscTndn);
    BOOL Get_ServMaxMinMoment(ElemPairK ElemK, double& dMuyMaxI, double& dMuyMinI, double& dMuyMaxJ, double& dMuyMinJ);
	BOOL Get_MqIRS(ElemPairK ElemK, double& dMuyMaxI, double& dMuyMinI, double& dMuyMaxJ, double& dMuyMinJ);

    // For Psc Design.
    BOOL Get_CrcElemListForDgn(BOOL bShowMsg, ArrElemPairKey& aElemK, BOOL bSkip2nd=FALSE);
    BOOL Get_ElemListForJtgFypc(ArrElemPairKey& arElemK, ArrElemPairKey& arTnElemK);
    // JTG D62-04, 텐던응력 계산에 필요.
    BOOL Get_ElemListForTendon(ArrElemPairKey& aElemK);
    BOOL Get_LastStepInCurStage(T_STAG_K StagK, int& iFinalStepNo);
    void Initialize();
    BOOL Get_CrcPosc(ElemPairK ElemK, T_POSC_D& PoscD);
    // LcomData For All Elem.
    BOOL Cal_LcomDataForDesignAll();
    BOOL Cal_LcomDataForDesignAll(ArrElemPairKey& aElemK);
    BOOL Cal_LcomDataForDesignAll_for_LRFDPrint();
    BOOL Get_LcomDataMap(CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K>& aLcomK);

    // LcomData For PSC Composite
    BOOL Get_StagePart1Force_PSCComposite(ArrElemPairKey& aElemK);
    BOOL Get_ForceStageMapData_PSCComposite(ElemPairK ElemK, CPG_FORC_STAG& ForceStage);
    // Unbraced Length
    BOOL Get_UnbracedLength();

    // For Design Forces.
    BOOL Get_ForcDataMap(CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&>& aForcD, CMapEx<int, int, _CRC_COMF_K, _CRC_COMF_K&>& mComfKey);
    // For Export and Import.
    BOOL Get_CrcFysg(ElemPairK ElemK, T_FYSG_D& FysgD);	//  1. 가설중 휨 응력도 (FY_SG)
    BOOL Get_CrcFypc(ElemPairK ElemK, T_FYPC_D& FypcD);	//  2. PC강재 최대응력도 (FY_PC)
    BOOL Get_CrcFycm(ElemPairK ElemK, T_FYCM_D& FycmD);	//  3. 합성응력도 (FY_COM)
    BOOL Get_CrcBdcw(ElemPairK ElemK, T_BDCW_D& BdcwD);	//  4. 휨 균열폭 (BND_CW)
    BOOL Get_CrcTbar(ElemPairK ElemK, T_TBAR_D& TbarD);	//  5. 인장철근 (RBAR_T)
    BOOL Get_CrcBdcr(ElemPairK ElemK, T_BDCR_D& BdcrD);	//  6. 휨파괴안전도 (BND_CR)
    BOOL Get_CrcFssg(ElemPairK ElemK, T_FSSG_D& FssgD);	//  7. 가설중 경사 인장 응력도 (FST_SG)
    BOOL Get_CrcFsms(ElemPairK ElemK, T_FSMS_D& FsmsD);	//  8. 설계하중시 경사 인장 응력도 (전단력 최대시) (FST_MS)
    BOOL Get_CrcCwms(ElemPairK ElemK, T_CWMS_D& CwmsD);	//  9. 설계하중시 전단균열 (전단력 최대시) (CW_MS)
    BOOL Get_CrcCums(ElemPairK ElemK, T_CUMS_D& CumsD);	// 10. 종국하중시 웹 또는 플랜지 압괴내력에 대한 검토 (전단력 최대시) (CRU_MS)
    BOOL Get_CrcCrms(ElemPairK ElemK, T_CRMS_D& CrmsD);	// 11. 종국하중시 경사 파괴 내력에 대한 검토 (전단력 최대시) (CR_MS)
    BOOL Get_CrcFsmt(ElemPairK ElemK, T_FSMT_D& FsmtD);	// 12. 설계하중시 경사 인장 응력도 (비틀림 최대시) (FST_MT)
    BOOL Get_CrcCwmt(ElemPairK ElemK, T_CWMT_D& CwmtD);	// 13. 설계하중시 전단균열 (비틀림 최대시) (CW_MT)
    BOOL Get_CrcCumt(ElemPairK ElemK, T_CUMT_D& CumtD);	// 14. 종국하중시 웹 또는 플랜지 압괴내력에 대한 검토 (비틀림 최대시) (CRU_MT)
    BOOL Get_CrcCrmt(ElemPairK ElemK, T_CRMT_D& CrmtD);	// 15. 종국하중시 경사 파괴 내력에 대한 검토 (비틀림 최대시) (CR_MT)
    BOOL Get_CrcFassCmpw(ElemPairK ElemK, T_FASS_CMPW& FassCmpw);  // 24. 설계하중시 평균전단응력도 (전단력 최대시) (BCK_ASS)
    BOOL Get_CrcFmssCmpw(ElemPairK ElemK, T_FMSS_CMPW& FmssCmpw);  // 25. 종국하중시 최대전단응력도 (전단력 최대시) (BCK_MSS) 
    BOOL Get_CrcFprtCmpw(ElemPairK ElemK, T_FPRT_CMPW& FprtCmpw);  // 26. 파형강판 국부좌굴 검토 (BCK_PRT)
    BOOL Get_CrcFwhlCmpw(ElemPairK ElemK, T_FWHL_CMPW& FwhlCmpw);  // 27. 파형강판 전체좌굴 검토 (BCK_WHL) 
    // For Only Export.
    BOOL Get_CrcFylc(ElemPairK ElemK, T_FYLC_D& FylcD);	// 16. 합성응력도 (하중조합별) (FY_LCB)
    BOOL Get_CrcFyst(ElemPairK ElemK, T_FYST_D& FystD);	// 17. 스테이지별 가설중 휨 응력도 (FY_SGT)
    BOOL Get_CrcFsst(ElemPairK ElemK, T_FSST_D& FsstD);	// 18. 스테이지별 가설중 경사 인장 응력도 (FST_SGT)
    BOOL Get_CrcFssm(ElemPairK ElemK, T_FSSM_D& FssmD);	// 19. 스테이지별 가설중 경사 인장 응력도 중간값 (FST_SMT)
    BOOL Get_CrcFtms(ElemPairK ElemK, T_FTMS_D& FtmsD);	// 20. 하중조합별 설계하중시 경사 인장 응력도 계산을 위한 중간값 (전단력최대시) (FST_MST)
    BOOL Get_CrcFtmt(ElemPairK ElemK, T_FTMT_D& FtmtD);	// 21. 하중조합별 설계하중시 경사 인장 응력도 계산을 위한 중간값 (비틀림최대시) (FST_MTT)
    BOOL Get_CrcLcba(ElemPairK ElemK, T_LCBA_D& LcbaD);	// 22. 설계하중시 최대/최소에 대한 하중조합번호 (LCB_ALW)
    BOOL Get_CrcLcbu(ElemPairK ElemK, T_LCBU_D& LcbuD);	// 23. 종국시 최대/최소에 대한 하중조합번호 (LCB_ULT)



    // Add by ZINU.('04.08.16). For PC-Design (KSCE-USD03).
    BOOL Get_KsceFypc(T_TDNA_K TdnaK, T_FYPC_KSCE& FypcKsce, int iCalcType=0);	// 2. 사용하중시 PC강재 최대응력도 (텐던별)
    BOOL Get_KsceFssg(ElemPairK ElemK, T_FPMT_JTG&  FpmtKsce);
    BOOL Get_KsceBdcy(ElemPairK ElemK, T_BDCY_KSCE& BdcrKsce);	// 6-1. 계수하중시 휨강도 (강축)
    BOOL Get_KsceBdcz(ElemPairK ElemK, T_BDCZ_KSCE& BdcrKsce);	// 6-2. 계수하중시 휨강도 (약축)
    BOOL Get_KsceCums(ElemPairK ElemK, T_CUMS_KSCE& CumsKsce);	// 10. 계수하중시 전단강도
    BOOL Get_KsceCrmt(ElemPairK ElemK, T_CRMT_KSCE& CrmtKsce);	// 11. 계수하중시 비틀림강도
    BOOL Get_KsceTndn(ElemPairK ElemK, T_TNDN_KSCE& TndnKsce);

    // JTG D62-04.
    //   Get_CrcFysg()                                      //  1. 가설중 휨 응력도                                                         
    BOOL Get_JtgFypc(T_TDNA_K TdnaK, T_FYPC_JTG& FypcJtg);	//  2. 정상사용극한상태 PC강재 최대응력도 (텐던별)
    BOOL Get_JtgFypc(CMap<T_TDNA_K,T_TDNA_K,T_FYPC_JTG,T_FYPC_JTG>& mapFypcJtg);	//  2. 정상사용극한상태 PC강재 최대응력도 (텐던별)
    BOOL Get_JtgFycm(ElemPairK ElemK, T_FYCM_JTG& FycmJtg, T_DETAIL_STR& RptInfo);      //  3. 정상사용극한상태 합성응력도
    BOOL Get_JtgFpms(ElemPairK ElemK, T_FPMS_JTG& FpmsJtg, T_DETAIL_STR& RptInfo, int iShrTor=3);	//  4. 정상사용극한상태 주응력도 (전단력 최대시)
    BOOL Get_JtgFpmt(ElemPairK ElemK, T_FPMT_JTG& FpmtJtg);	//  5. 정상사용극한상태 주응력도 (비틀림 최대시)
    BOOL Get_JtgBdcw(ElemPairK ElemK, T_BDCW_D& BdcwD, T_DETAIL_STR& RptInfo);      //  6. 정상사용극한상태 휨 균열폭
    BOOL Get_JtgTbar(ElemPairK ElemK, T_TBAR_JTG& TbarJtg, T_DETAIL_STR& RptInfo);  //  7. 정상사용극한상태 인장철근
    BOOL Get_JtgTndn(ElemPairK ElemK, T_TNDN_JTG& TndnJtg, T_DETAIL_STR& RptInfo);  //  8. 정상사용/재하능력극한상태 인장텐던량
    BOOL Get_JtgBdcy(ElemPairK ElemK, T_BDCY_JTG& BdcrJtg, T_DETAIL_STR& RptInfo);	//  9. 재하능력극한상태 휨강도 (강축)
    BOOL Get_JtgCums(ElemPairK ElemK, T_CUMS_JTG& CumsJtg, T_DETAIL_STR& RptInfo);	// 10. 재하능력극한상태 전단강도
    BOOL Get_JtgCrmt(ElemPairK ElemK, T_CRMT_JTG& CrmtJtg, T_DETAIL_STR& RptInfo);	// 11. 재하능력극한상태 비틀림강도  
    BOOL Get_JtgCscs(ElemPairK ElemK, T_CSTCS_CH& CscsJtg, T_DETAIL_STR& RptInfo);	// 12.compressive stress check at service stage.예箕駱炬 add by maxiao 2008.01.11
    BOOL Get_JtgRptOption(ElemPairK ElemK,int iPosi,int PrintItem);
    BOOL Get_JtgRptOption(ElemPairK ElemK,int PrintItem);

    // For AASHTO-LRFD08
    BOOL Get_FypcData(T_TDNA_K TdnaK, T_FYPC_AASHTO& rData);
    BOOL Get_BdcwData(ElemPairK ElemK, T_BDCW_AASHTO& rData);
    BOOL Get_TbarData(ElemPairK ElemK, T_TBAR_AASHTO& rData);
    BOOL Get_BdcyData(ElemPairK ElemK, T_BDCY_AASHTO& rData, T_PSCD_DGN_INFO& rDgn);
    //BOOL Get_BdczData(T_ELEM_K ElemK, T_TBAR_AASHTO& rData);
    BOOL Get_CumsData(ElemPairK ElemK, T_CUMS_AASHTO& rData);
    BOOL Get_CrmtData(ElemPairK ElemK, T_CRMT_AASHTO& rData);
    // For PSC Composite (AASHTO LRFD 12, 14)
    BOOL Get_CrcFysg_Composite(ElemPairK ElemK, T_FYSG_D& FysgD_Gr, T_FYSG_D& FysgD_sl);   //  1. 가설중 휨 응력도 (FY_SG)
    BOOL Get_CrcFycm_Composite(ElemPairK ElemK, T_FYCM_D& FycmD_Gr, T_FYCM_D& FycmD_Sl);   //  3. 합성응력도 (FY_COM)
    BOOL Get_PSCDgnData(ElemPairK ElemK, T_PSCD_DGN_INFO& rData);



    BOOL IsExsitPscDesignData(int iDgnCode, ElemPairK ElemK);
    BOOL IsAdPSCCode();

    // For Eurocode2-2:05, KSCE-LSD15 ~ 
    BOOL Get_CStageResPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC &FysgD, T_FPSG_PSC &FpsgD);
    BOOL Get_FysgDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC& rData);
    BOOL Get_FycmDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC& rData);
    BOOL Get_FpsgDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC& rData);
    BOOL Get_FpmsDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC& rData);
    BOOL Get_BdcwDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC& rData);
    BOOL Get_BdcrDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC& rData);
    BOOL Get_CumsDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CUMS_PSC& rData);
    BOOL Get_CrmtDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CRMT_PSC& rData);

    BOOL Get_FatgDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FATG_PSC& rData);

    BOOL Convert_FysgDataPSC_to_Compo(T_FYSG_PSC& rData, T_FYSG_PSC_COMP& rResData);
    BOOL Convert_FycmDataPSC_to_Compo(T_FYCM_PSC& rData, T_FYCM_PSC_COMP& rResData);
    BOOL Convert_FpsgDataPSC_to_Compo(T_FPSG_PSC& rData, T_FPSG_PSC_COMP& rResData);
    BOOL Convert_FpmsDataPSC_to_Compo(T_FPMS_PSC& rData, T_FPMS_PSC_COMP& rResData);
    BOOL Convert_BdcwDataPSC_to_Compo(T_BDCW_PSC& rData, T_BDCW_PSC_COMP& rResData);
    BOOL Convert_BdcrDataPSC_to_Compo(T_BDCR_PSC& rData, T_BDCR_PSC_COMP& rResData);
    BOOL Convert_CumsDataPSC_to_Compo(T_CUMS_PSC& rData, T_CUMS_PSC_COMP& rResData);
    BOOL Convert_CrmtDataPSC_to_Compo(T_CRMT_PSC& rData, T_CRMT_PSC_COMP& rResData);

    //IRC112
    BOOL Get_CStageResPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_IRC &FysgD, T_FPSG_PSC_IRC &FpsgD);//IRC112
    BOOL Get_FysgDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_IRC& rData);//IRC112
    BOOL Get_FycmDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_IRC& rData);//IRC112
    BOOL Get_FpsgDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC_IRC& rData);//IRC112
    BOOL Get_FpmsDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC_IRC& rData);//IRC112
    BOOL Get_BdcwDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_IRC& rData);//IRC112
    BOOL Get_BdcrDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_IRC& rData);//IRC112
    BOOL Get_CumsDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CUMS_PSC_IRC& rData);//IRC112
    BOOL Get_CrmtDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CRMT_PSC_IRC& rData);//IRC112

    BOOL Get_FatgDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FATG_PSC_IRC& rData);//IRC112
    //Convert to IRC112 Structs
    BOOL Convert_FysgDataPSC_to_IRC(T_FYSG_PSC& rData, T_FYSG_PSC_IRC& rResData);	//IRC112
    BOOL Convert_FycmDataPSC_to_IRC(T_FYCM_PSC& rData, T_FYCM_PSC_IRC& rResData);	//IRC112
    BOOL Convert_FpsgDataPSC_to_IRC(T_FPSG_PSC& rData, T_FPSG_PSC_IRC& rResData);	//IRC112
    BOOL Convert_FpmsDataPSC_to_IRC(T_FPMS_PSC& rData, T_FPMS_PSC_IRC& rResData);	//IRC112
    BOOL Convert_BdcwDataPSC_to_IRC(T_BDCW_PSC& rData, T_BDCW_PSC_IRC& rResData);	//IRC112
    BOOL Convert_BdcrDataPSC_to_IRC(T_BDCR_PSC& rData, T_BDCR_PSC_IRC& rResData);	//IRC112
    BOOL Convert_CumsDataPSC_to_IRC(T_CUMS_PSC& rData, T_CUMS_PSC_IRC& rResData);	//IRC112
    BOOL Convert_CrmtDataPSC_to_IRC(T_CRMT_PSC& rData, T_CRMT_PSC_IRC& rResData);	//IRC112
    //End IRC112
	//IRS
	BOOL Get_CStageResPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_IRS &FysgD, T_FPSG_PSC_IRS &FpsgD);//IRS
	BOOL Get_FysgDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_IRS& rData);//IRS
	BOOL Get_FycmDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_IRS& rData);//IRS
	BOOL Get_FpsgDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC_IRS& rData);//IRS
	BOOL Get_FpmsDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC_IRS& rData);//IRS
	BOOL Get_BdcwDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_IRS& rData);//IRS
	BOOL Get_BdcrDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_IRS& rData);//IRS
	BOOL Get_CumsDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CUMS_PSC_IRS& rData);//IRS
	BOOL Get_CrmtDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CRMT_PSC_IRS& rData);//IRS
	BOOL Get_FatgDataPSC(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FATG_PSC_IRS& rData);//IRS
    BOOL Convert_FypcToIRS(const T_FYPC_AASHTO &rIn, T_FYPC_IRS &rData) { return TRUE; }
	BOOL Get_StrbToForce(T_STRB_D &Strb, _DGN_FORC_CRC &rData);
    // PSC Composite(KSCE-LSD15)
    BOOL Get_PSCCheckResult(ElemPairK ElemK);
    BOOL Get_CStageResPSC_Comp(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_COMP &FysgD, T_FPSG_PSC_COMP &FpsgD);
    BOOL Get_FysgDataPSC_Comp(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_COMP& rData);
    BOOL Get_FycmDataPSC_Comp(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_COMP& rData);
    BOOL Get_FpsgDataPSC_Comp(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC_COMP& rData);
    BOOL Get_FpmsDataPSC_Comp(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC_COMP& rData);
    BOOL Get_BdcwDataPSC_Comp(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_COMP& rData);
    BOOL Get_BdcrDataPSC_Comp(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_COMP& rData);
    BOOL Get_CumsDataPSC_Comp(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CUMS_PSC_COMP& rData);
    BOOL Get_CrmtDataPSC_Comp(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CRMT_PSC_COMP& rData);

    BOOL Calc_CStageResCpt(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_STRS_CPT &StrsD, T_SHRS_CPT &ShrsD, T_FYSG_PSC_COMP &FysgD, T_FPSG_PSC_COMP &FpsgD);
	BOOL Get_BdcwDataPSC_Comp_IRC112(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_COMP& rData);

    // For SNiP 2.05.03-84*, SP 35.13330.2011
    BOOL CheckFirstGroupLS(ElemPairK ElemK, T_BDCR_SNIP &BcdrRes, T_CUMS_SNIP &CumsRes, T_CRMT_SNIP &CrmtRes, T_FATI_SNIP &FatiRes);
    BOOL CheckSecondGroupLS(ElemPairK ElemK, T_FYSG_SNIP &FysgD, T_FYCM_SNIP &FycmD, T_FPSG_SNIP &FpsgD, T_FPMS_SNIP &FpmsD, T_BDCW_SNIP &BdcwD);
    BOOL Get_FypcDataSNiP(T_TDNA_K TdnaK, T_FYPC_SNIP &rData);
    BOOL Get_FysgDataSNiP(ElemPairK ElemK, T_FYSG_SNIP &rData);
    BOOL Get_FycmDataSNiP(ElemPairK ElemK, T_FYCM_SNIP &rData);
    BOOL Get_FpsgDataSNiP(ElemPairK ElemK, T_FPSG_SNIP &rData);
    BOOL Get_FpmsDataSNiP(ElemPairK ElemK, T_FPMS_SNIP &rData);
    BOOL Get_BdcwDataSNiP(ElemPairK ElemK, T_BDCW_SNIP &rData);
    BOOL Get_BdcrDataSNiP(ElemPairK ElemK, T_BDCR_SNIP &rData);
    BOOL Get_CumsDataSNiP(ElemPairK ElemK, T_CUMS_SNIP &rCums, T_CRMT_SNIP &rCrmt); // 비틀림 같이 검토  
    BOOL Get_FatiDataSNiP(ElemPairK ElemK, T_FATI_SNIP &rData);

    BOOL CalcTendonfpsLpPosition();
    BOOL Get_PSCCheckResultAS(ElemPairK ElemK, T_DRST_D &Drst);
    BOOL Get_CStageResPSC_AS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_AS &FysgD, T_FPMT_JTG &FpsgD);
    BOOL Get_TransferPSC_AS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_TRAN_PSC_AS& TranD, T_BDCW_PSC_AS &CrackD);
    BOOL Get_StressResSLSPSC_AS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_AS &FysgD, T_FPMS_PSC &FpsgD);
    BOOL Get_CrackResSLSPSC_AS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_AS &BdcwD);
    BOOL Get_StrengthResULSPSC_AS (ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_AS &BdcrD, T_SHRR_PSC_AS &ShrrD, T_TORR_PSC_AS &TorrD);
    BOOL Get_BdcrDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_AS &rData);
    BOOL Convert_FypcToAS(const T_FYPC_AASHTO &rIn, T_FYPC_PSC_AS &rData);
    BOOL Get_ForceInfo4Crack(int nSize, BOOL bComposite,
        _DGN_FORC_CRC& ForceD, _DGN_FORC_CRC& TnprForD, _DGN_FORC_CRC& FcsDL, _DGN_FORC_CRC& ForSTD,
        _DGN_STRS_CRC& StressD, _DGN_STRS_CRC& StrLTermD, _DGN_STRS_CRC& StrSTermD,
        _DGN_STRS_CRC& StrP2, _DGN_STRS_CRC& StrLTP2, _DGN_STRS_CRC& StrSTP2,
        _LCOM_FOR_B pForGr[2], _LCOM_FOR_B pForSl[2], PSC_BDCW_CR_PROP_R pCrP[2]);
    
    BOOL Get_PSCCheckResultBS(ElemPairK ElemK, T_DRST_D &Drst);
    BOOL Get_CStageResPSC_BS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_BS &FysgD, T_FPSG_PSC &FpsgD);
    BOOL Get_StressResSLSPSC_BS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_BS &FysgD, T_FPMS_PSC &FpsgD);
    BOOL Get_CrackResSLSPSC_BS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_BS &rData);
    BOOL Get_StrengthResULSPSC_BS (ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_BS &BdcrD, T_SHRR_PSC_BS &ShrrD, T_TORR_PSC_BS &TorrD);
    BOOL Get_BdcrDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_BS &rData);
    BOOL Get_FypcDataBS(T_TDNA_K TdnaK, T_FYPC_PSC_BS& rData);

    BOOL Get_CheckForceMu_MaxMin(BOOL bStrengh, ElemPairK ElemK, double* pdMyMax, double* pdMyMin, int* piMaxDgnLcom, int* piMinDgnLcom);
	BOOL Get_CheckForceMu_MaxMin(BOOL bStrengh, ElemPairK ElemK, double* pdMyMax, double* pdMyMin, int* piMaxDgnLcom, int* piMinDgnLcom, double* pdFyMax, double* pdFyMin, double* pdFxMax, double* pdFxMin);//Overloaded to get Fy and Fx at max/min Mu
    BOOL Get_CumsForceFz_MaxMin(ElemPairK ElemK, _DGN_FORC_CRC& FzMaxForceD, _DGN_FORC_CRC& FzMinForceD, _DGN_STRS_CRC& FzMaxStressD, _DGN_STRS_CRC& FzMinStressD, int* piMaxDgnLcom, int* piMinDgnLcom);
    BOOL Get_CrmtForceMx_MaxMin(ElemPairK ElemK, _DGN_FORC_CRC& MxMaxForceD, _DGN_FORC_CRC& FzMaxForceD, _DGN_FORC_CRC& FzMinForceD,
        _DGN_STRS_CRC& MxMaxStressD, _DGN_STRS_CRC& FzMaxStressD, _DGN_STRS_CRC& FzMinStressD, int* piMxMaxDgnLcom, int* piFzMaxDgnLcom, int* piFzMinDgnLcom);

    BOOL Get_CumsForceFz_MaxMin_LSD(ElemPairK ElemK, _DGN_FORC_CRC& FzMaxForceD, _DGN_FORC_CRC& FzMinForceD, _DGN_STRS_CRC& FzMaxStressD, _DGN_STRS_CRC& FzMinStressD, int* piMaxDgnLcom, int* piMinDgnLcom);
    BOOL Get_CumsForce_Mu_MaxMin(ElemPairK ElemK, double* pdMaxMu, double* pdMinMu);

    BOOL Get_CheckSigma_MaxMin_FatgPsc(BOOL bStrengh, ElemPairK ElemK, double* pSigLcom_max_t, double* pSigLcom_min_t, int* pMaxSigLcom_t,
        double* pSigLcom_max_b, double* pSigLcom_min_b, int* pMaxSigLcom_b,
        double* pVuLcom_max,    double* pVuLcom_min,    int* pMaxVuLcom);

    // 시공단계, 시간의존 압축강도..
    BOOL Get_InitCompressStrength(T_TDME_K TdmeK, double dt, double& dfci);
    BOOL Get_Day4fci(ElemPairK ElemK, BOOL bIsCpt, BOOL bIsPart1, T_STAG_K StagK, int nSerialStep, double& dDay);

protected:  
    BOOL MakeJointData4Segment();
    BOOL Make_JointData4Element(ElemPairK ElemK, DGN_JOINT_D& rData);

    double OhzagiY(int nCement, double m);
    BOOL GetCentroidAngleOfSectoffset(int nOffsetPoint, double dLength,
        double dCypI, double dCymI, double dCzpI, double dCzmI,
        double dCypJ, double dCymJ, double dCzpJ, double dCzmJ,
        double &dAngY, double &dAngZ);

public:
    BOOL Get_JointData4Element(ElemPairK ElemK, DGN_JOINT_D& rData);
    BOOL Get_JointData4Segment();

    // To Use at KSCE-USD03.
    // 1. 제일 먼저 Set_UnitIndex()를 호출하자.
    // 2. Get_ConSect()를 호출하고 나서 arInner에 대하여 메모리 할당된 것을 지우자.
    void Set_UnitIndex(T_UNIT_INDEX& CodeUnit);
    void Get_CurrUnitIndex(T_UNIT_INDEX& CodeUnit);
    T_UNIT_INDEX Get_CodeUnitIndex();
    BOOL Get_KscePhi(double& dPhib, double& dPhiv);
    BOOL Get_JtgGamma(double& dgam0);
    BOOL Get_ConMatd(ElemPairK ElemK, T_MATD_D& MatdD);
    BOOL Get_SrcMatd(ElemPairK ElemK, T_MATD_D& MatdD);
    BOOL Get_StlMatd(ElemPairK ElemK, T_MATD_D& MatdD);
    BOOL Get_ConSect(ElemPairK ElemK, BOOL bPosI, double dzDimComp, T_SPSC_SECT& SpscSect, CString& strShape,
        BOOL& bClosedSect, int &nDgnSectType, double& dAg, double& dAoh, double& dPh, double& dKt, double& dWt, double dpMomPos[][2], double* dpShrPos, double* dpShrThk, double* dpQ, double& dTorThk, double* dFlgThk,
        double& dLbarThk, double &dbtf, BOOL& bDivSect4Tor, int& nDivPart, double* dpCutPos,
        CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, int& nWebSize);
    BOOL Get_ConSect_Compo(ElemPairK ElemK, BOOL bPosI, T_SPSC_SECT& SpscSect, CString& strShape,
        BOOL& bClosedSect, int &nDgnSectType, double& dAg, double& dAoh, double& dPh, double& dKt, double& dWt, double dpMomPos[][2], double* dpShrPos, double* dpShrThk, double* dpQ, double& dTorThk, double* dFlgThk,
        double& dLbarThk, BOOL& bDivSect4Tor, int& nDivPart, double* dpCutPos,
        CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, int& nWebSize);

    BOOL Get_ConSecg(ElemPairK ElemK, BOOL bPosI, T_SPSC_SECT& SpscSect);	// Gross Sect Data.
    BOOL Get_ConSecg_Compo(ElemPairK ElemK, BOOL bPosI, T_SPSC_SECT& SpscSect);	// Gross Sect Data.
    BOOL Get_ConRbar(ElemPairK ElemK, BOOL bPosI, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi);
    BOOL Get_ConRshr(ElemPairK ElemK, BOOL bPosI, double& dfdt, double& dfdb, double& dVd, double& dMd, T_RPSC_SBAR& RpscSbar);
    BOOL Get_ConTndn(ElemPairK ElemK, BOOL bPosI, BOOL bIsGrup, double dMuyMax, double dMuyMin, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi);
    BOOL Get_ConTndn(T_TDNA_K TdnaK, int& iLoadType, int& iRelxType, double& dfpu, double& dfpy, double& dTotalTendonLength, int& iBoundaryNum);
    BOOL Get_ConForc(ElemPairK ElemK, BOOL bIncludeTP, ADGNFORCE &arForcLcom);
    BOOL Get_ConForc(ElemPairK ElemK, BOOL bIncludeTP, T_LCOM_K LcomK, _DGN_FORC_CRC& ForcLcom);
    BOOL Get_ConForc(ElemPairK ElemK, BOOL bIncludeTP, T_LCOM_K LcomK, _DGN_FORC_CRC& ForcTnsc, _DGN_CRCK_JTG& CrckForc);
    BOOL Get_ConForc(ElemPairK ElemK, BOOL bIncludeTP, T_LCOM_K LcomK, _DGN_FORC_CRC& ForcLcom, _DGN_FORC_CRC& ForcTnpr);
    BOOL Get_LengthFromAnchorage(ElemPairK ElemK, T_TDNA_K TdnaK, BOOL bPosI, double& dLength, BOOL& bFromI);
    BOOL Get_TransferTendonInfo(T_TDNA_K TdnaK, _DGN_TNDN_TRAN& rData);
    BOOL Get_TransferTendonInfo(T_TDNA_K TdnaK, BOOL bStart, const T_TDNA_D& TdnaD, const T_TDNT_D& TdntD, _DGN_TNDN_TRAN_INF& rData);
    BOOL MakeElemLength4TendonProfile();
	BOOL MakeTendonForceElemToNode(CArray<double, double>& aMatrixData, double& dfx, double& dfy, double& dfz);
    

    // Composite Plate Girder
    BOOL Get_CpgSect(ElemPairK ElemK, int nSectType, BOOL bPosiI, T_SECT_STIFFNESS* SpscSect);
    BOOL Get_CpgRbar(ElemPairK ElemK, BOOL bPosI, BOOL bCrack, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi);
    BOOL Get_CpgRbar4Dgn(ElemPairK ElemK, BOOL bPosI, BOOL bCrack, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi);
    BOOL Get_CpgLsti(ElemPairK ElemK, BOOL bPosI, CArray<CPG_LSTI_DATA,CPG_LSTI_DATA&>& arLstiPosi);

    BOOL Get_Cscs4CPT(CArray<T_STAG_K,T_STAG_K> &aStagK, ElemPairK ElemK, T_CSCS_D &rCscs);
    BOOL Get_SlabElastCscs(ElemPairK ElemK, T_CSCS_D &rCscs, double &dElast);

    // Rating 기능..
    BOOL Get_TndnStress(_TNDN_STRESS& TndnStress);
    //BOOL Calc_TndnStress4Tndn(T_TDNA_K TdnaK, T_FYPC_KSCE& FypcKsce);


    // Bridge Load Rating Design
protected:
    ArrElemPairKey m_arBlrdElemK;
    CArray<T_RRPL_K,T_RRPL_K> m_arBlrdMembK;
    CArray<T_BLRC_K,T_BLRC_K> m_arServBlrcK;
    CArray<T_BLRC_K,T_BLRC_K> m_arStreBlrcK;
    std::map<ElemPairK,BLR_SPAN_D> m_mapElemSpanInfo;
    std::map<ElemPairK,DGN_RENI_D> m_mapReinD;

    void ChangeTemperatureFactor(T_BLRC_D& BlrcD);
    BOOL Make_ConnectedElemInfo4Node();
    BOOL Make_ConnectedElemForceDat4Node();
    BOOL Get_AddForceData4Average(ElemPairK ElemK, CArray<FORCE_BLRD_RCASE,FORCE_BLRD_RCASE>& arAddForce);

    BOOL Get_RatingFlexuralStrengthKR(ElemPairK ElemK, BOOL bCompoSect, T_RAKR_D& RakrD, T_RKMC_CASE& RkmcCaseD);
    BOOL Get_BdcrDataPSC_Rating(ElemPairK ElemK, int nIJ, PSC_DATA_MEMB &MembInD, T_RAKR_D& RakrD, T_RKMC_BASE& RkmcBaseD, PSC_FRCR_D& OutData, BOOL bPrt = FALSE);
    BOOL Get_BdcrDataPSC_Comp_Rating(ElemPairK ElemK, int nIJ, PSC_DATA_MEMB &MembInD, T_RAKR_D& RakrD, T_RKMC_BASE& RkmcBaseD, PSC_FRCR_D& OutData, BOOL bPrt = FALSE);


    BOOL Set_FlexuralStrengthRatingData(ElemPairK CheckElemK, int nPos_Max, int iMn_Method, int iChkPosition, PSC_DATA_POSI InData, _BDCY_PSC_LRFD_BASE BdcyBase);
    BOOL Set_ShearStrengthRatingData(ElemPairK CheckElemK, int nPos,  int iChkPosition, double dMu_Cal, _CUMS_AASHTO_BASE& CumsBase, T_RTSC_BASE& RtscBaseD);

public:

    BOOL Get_RatingFlexuralStrengthKR_Print(ElemPairK ElemK, BOOL bCompoSect, int nIJ, int nPos, PSC_DATA_POSI& InData, T_RAKR_D& RakrD, T_RKMC_CASE& RkmcCaseD, PSC_FRCR_D& OutData);

    BOOL Calc_FlexuralStrengthMethod(int nSurveyMeth, double dpMn, double dElast, T_RKMC_BASE& RkmcBase);
    BOOL Calc_LengthPosition4Span(const ArrElemPairKey& aElemK);
    BOOL Calc_LengthPosition4SpanElem(const ArrElemPairKey& aElemK);
    BOOL Calc_LengthPosition4SpanVBeam(const ArrElemPairKey& aElemK);

    BOOL Cal_RatingCaseDataForAASHTO();
    BOOL Cal_RatingCaseStlDataForAASHTO();
    BOOL Cal_RatingCaseDataForKSCE05(const int &nDgnCode);
    BOOL Cal_RatingCaseDataForKSCE_Plate(const int &nDgnCode);
    //
    CString GetLcomType(const int &iLcomType);
    BOOL ConvertForce2ResStr(_DGN_FORC_CRC &Force, T_RATI_UNITLCOM &rData, const int &nIJ);
    int  GetBridgeLevel(const double &dRatio, const int &nOK);
    //
    BOOL Get_RcplDataRC(T_RRPL_K iRcpl);
    int GetConnectPlateKeyList(CArray<T_ELEM_K, T_ELEM_K>& arElemK, CArray<T_ELEM_K, T_ELEM_K>& arConnectElemK);
    //

    BOOL Get_ElemListForBlrDgn(BOOL bShowMsg, ArrElemPairKey& arElemK);
    BOOL Get_ElemListForBlrDgn_STL(BOOL bShowMsg, ArrElemPairKey& arElemK);
    BOOL Get_ElemListForBlrDgn_RC(BOOL bShowMsg, CArray<T_ELEM_K, T_ELEM_K>& arElemK);
    BOOL Get_ElemListForBlrDgn_RC_Plate(BOOL bShowMsg, CArray<T_MEMB_K,T_MEMB_K>& arMembK);
    void InitialBridgeLoadRating();

    //
    BOOL Calc_StressRatingDataAASHTO();
    BOOL Calc_StressRatingDataAASHTO19(D_XL_PROGRESS_INTERFACE* pProInte);
    BOOL Calc_TendonStressRatingDataAASHTO();
    BOOL Calc_FlexuralStrengthRatingDataAASHTO();
    BOOL Calc_FlexuralStrengthRatingDataAASHTO19(D_XL_PROGRESS_INTERFACE* pProInte);
    BOOL Calc_ShearStrengthRatingDataAASHTO();
    BOOL Calc_ShearStrengthRatingDataAASHTO19(D_XL_PROGRESS_INTERFACE* pProInte);

    BOOL ExecutePSCRatingAASHTO();
    BOOL ExecutePSCRatingAASHTO19();
    BOOL ExecuteRatingKS05(int nCodeNum);
    BOOL ExecuteSTLRating(const int &nCodeNum);
    BOOL ExecuteRCRating(const int &nCodeNum);
    BOOL ExecuteRCPlateRating(const int &nCodeNum);
    BOOL ExecuteBD21PSCBridgeAssessment();

    BOOL Get_RatingStressData(int iIJ, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int iConcurrent, T_BLRC_D& BlrcD, STRESS_BLRD_RCASE& StressRcase, _DGN_STRS_CRC& StrTp, T_RTST_BASE& RtstBase, UINT& nPoint_comp, UINT& nPoint_tens);
    BOOL Get_RatingStressPermitData(BOOL bIsCpt, int iIJ, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int iConcurrent, T_BLRC_D& BlrcD, STRESS_BLRD_RCASE& StressRcase, FORCE_BLRD_RCASE& ForceRcase, T_RTST_PERMIT_BASE& RtstPermitBase, double* dStressDead, double* dStressLive);
    BOOL Get_RatingTendonStressData(ElemPairK ElemK, int iIJ, T_TDNA_K TdnaK, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int iConcurrent, T_BLRC_D& BlrcD, FORCE_BLRD_RCASE& ForceRcase, T_RTTS_BASE& RttsBase);
    BOOL Get_RatingFlexuralStrengthData(int iIJ, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int iConcurrent, T_BLRC_D& BlrcD, FORCE_BLRD_RCASE& ForceRcase, T_RTBC_BASE& RtbcBase);
    BOOL Get_RatingShearStrengthData(int iIJ, BOOL bDCmax, BOOL bDWmax, BOOL bTmax, int iConcurrent, T_BLRC_D& BlrcD, FORCE_BLRD_RCASE& ForceRcase, T_RTSC_BASE& RtscBase);
    BOOL GetBlrCheckGroupKey(ElemPairK ElemK, T_GRUP_K& GroupK);

    BOOL Get_ElemSpanInfo(ElemPairK ElemK, BLR_SPAN_D &rData);

    // KR, Rating, KSCE-USD05
    BOOL Calc_StressRatingDataKS05();
    BOOL Calc_FlexuralStrengthRatingDataKR(const int &nDgnCode);
    BOOL Get_ChkElemFlag4RatingKR(T_RKCE_D& RkceD, int iPosi);
    BOOL Get_ChkElemFlag4RatingKR(T_RROC_D& RrocD, int iPosi);
    UINT Get_ConCurrType(int iRcaseTypeDL, int iRcaseTypeLL);

    // Plate Ratign 처짐 검토
    //   BOOL IsRCPlateRating_Defl();
    //   BOOL Get_DeflectionResult4Plate(T_RRPL_K MembK, int iRcaseNo, double& dDisp);
    BOOL Get_Deflection4Plate(T_RRPL_K MembK, int iRcaseNo, T_NODE_K &MinNodeK, double& dDisp);

    //PMS:RATING BSC-20140329
    BOOL Get_ChkElemFlag4RatingKSCEASD(ElemPairK& ElemK, int iPosi);
    BOOL Get_Deflection4NodeOfElem(ElemPairK ElemK, int iRcaseNo, double& dIdisp, double& dJdisp, const int &nRatingType);

    // BD 4415 and BD 2101 Assessment
    BOOL Cal_RatingCaseDataForBD21();
    BOOL Calc_BD_ULS_FlexuralAssessmentData();
    BOOL Calc_BD_ULS_ShearAssessmentData();
    BOOL Calc_BD_ULS_TorsionAssessmentData();
    BOOL Calc_BD_SLS_AssessmentData();
    BOOL Get_ChkElemFlag4RatingBS(T_ASOP_D& AsopD, int iPosi);

protected:
    BOOL Calc_AdjustmentFactorK(BOOL bAppAFK, BOOL bAutoAFK, double dEpsiC, double dEpsiT, double dKb, double dKuser, double &dKa, double &dK);
    BOOL Get_DeflectionOfNode(ElemPairK ElemK, T_BLRC_D &BlrcD, T_LCOM_D &LcomDE, double &dIdisp, double &dJdisp);
    BOOL Calc_Stress4MVpr(ElemPairK ElemK, int nPos, int nCC, double dEc, STRESS_BLRD_RCASE &StressD, double &dEpsiCal);
    BOOL Calc_StressCpt4MVpr(ElemPairK ElemK, int nPos, int nCC, double dEc, double dEcs, STRESS_BLRD_RCASE &StressD, STRESS_BLRD_RCASE &StressSlabD, double &dEpsiCal);
    double Get_ElemLengthInTendon(T_TDNA_K TdnaK);
    double Get_TotalElemLength4TdnaK(T_TDNA_K TdnaK);

public:
	double Calc_DevelopeLength(T_TDNA_K TdnaK, BOOL bStart, double dh, double dfpe, double dfps, double& dDevL_cal);


    // Composite Plate Girder Design.
protected:
    BOOL m_bChkItem[D_CPG_CHECK_ITEM];
    ArrElemPairKey m_arPgdElemK;  
    std::map<ElemPairK, CPG_FORC_STAG> m_ForceStageMap;
    BOOL m_bTestMode;

    int m_ElemKey_Flex;
    int m_ElemKey_Shear;

public:  
    ArrElemPairKey m_arPgdElemKList;

    std::map<std::pair<EN_BEAM_DGN_TYPE,int>,T_ULEN_MEMB_D> m_mUblenMemb;
    std::map<ElemPairK, std::pair<EN_BEAM_DGN_TYPE, int>>           m_mElemUblmKey;
    ArrElemPairKey                    m_aChkElemUblmKey;

    std::map<ElemPairK, int> m_mNs;
    std::map<T_TDNA_K, int> m_mNs4Td;

    BOOL MakePlateGiderDesignData();
    BOOL MakePlateGirderDesign4Report();
    void InitialPlateGirder();
    BOOL Get_ElemListForPlateGirder(BOOL bShowMsg, ArrElemPairKey& arElemK);
    BOOL Get_ModiCheckElemKey(const ArrElemPairKey& aChkElemK, const ArrElemPairKey& aChkElemUblmK, ArrElemPairKey& raChkElemKey);
    BOOL ExecutePlateGirderDesign();
    BOOL Get_ChkFlagPlateGirder(ElemPairK ElemK, int iPosi);
    BOOL GetCheckElemKey4WebPanel(const ArrElemPairKey& aChkElemK, ArrElemPairKey& raChkElemPanel);

    // Plate Girder Bending Resistance
    BOOL Calc_ClassifyCrossSectionForPGD();

    BOOL Check_CompositePlateGirderDesign();
    BOOL Check_CompositePlateGirderDesign_IRC();
    BOOL Check_CompositePlateGirderDesign_IRC22_15();
    //
    BOOL Calc_BendingResistanceForPGD(BOOL bBendingChk);
    // Plate girder, Resistance to Vertical Shear
    BOOL Calc_VerticalShearResistanceForPGD();
    // Plate girder, resistance to Lateral-Torsional Buckling
    BOOL Calc_LateralTorsionalBucklingResistanceForPGD();
    // Plate girder, Resistance to Transverse Force
    BOOL Calc_TransverseForceResistanceForPGD();
    // Plate Girder, resistance to Longitudinal Shear
    BOOL Calc_LongitudinalShearResistanceForPGD(BOOL bPglsChk);
    // Plate Girder, Resistance to Fatigue
    BOOL Calc_FatigueResistanceForPGD(double dt_Ld);
    //Plate Girder, Beam Stress at the serviceability limit state
    BOOL Calc_BeamStressResistanceForPGD();

    BOOL Get_StagePart1Force(ArrElemPairKey& aElemK);
    double Get_Mc_Ed_EC4PlateGirder(double dMa_Ed, double dMu);

    //void Set_PlateGirderInfo4Check(BOOL bChk, T_ELEM_K ElemK, int nPosi, int iDgnLcomNo, int iOrgLcomNo, int iMax, double* aForce, double dMa_Ed, T_PGDR_D& rData);

    //
    BOOL Get_RigidLength(ElemPairK ElemK, int iPosi, double& dLy, double& dLz, double& dLu);
    BOOL Get_C4valueForMomentType(int iDgnLcomKey, ElemPairK MembK, ArrElemPairKey& arMembElemK, double& dC4, BOOL bCodeUnit=FALSE) const;
    int Get_MomentType4PGD(double dMom) const;
    BOOL Get_PSI_EndMoment56Type(double dM1, double dM2, double& dPsi) const;
    BOOL Get_PSI_EndMoment14Type(ElemPairK MembK, ArrElemPairKey& arMembElemK, CArray<_DGN_FORC_CRC,_DGN_FORC_CRC&>& arForce, double& dM0, double& dPsi, double& dCoeff) const;
    BOOL Get_MomentShape_PSI(ElemPairK MembK, ArrElemPairKey& arMembElemK, CArray<_DGN_FORC_CRC,_DGN_FORC_CRC&>& arForce, int& iMomType, double& dPsi, double& dCoeff) const;
    BOOL Get_CountInflection(int iPre, int iCurr, int& iGetCurr, int& iCount) const;
    double Get_y_Function(double da, double db, double dx1) const;
    BOOL Get_C4ForUFrameModel(int iMomType, double dPsi, double dCoeff, double& dC4) const;
    BOOL Get_PsiDataForInterPolation(int iMomType, double dPsi, int& iPsi1, int& iPsi2, double& dPsiRat) const;
    BOOL Get_PsiDataForInterPolation14(double dPsi, int& iPsi1, int& iPsi2, double& dPsiRat) const;
    BOOL Get_PsiDataForInterPolation56(double dPsi, int& iPsi1, int& iPsi2, double& dPsiRat) const;
    BOOL Get_PsiDataForInterPolation7 (double dPsi, int& iPsi1, int& iPsi2, double& dPsiRat) const;
    //
    BOOL Get_CoeffDataForInterPolation(int iMomType, double dCoeff, int& iCoeff1, int& iCoeff2, double& dCoeffRat) const;
    BOOL Get_CoeffDataForInterPolation12(double dCoeff, int& iCoeff1, int& iCoeff2, double& dCoeffRat) const;
    BOOL Get_CoeffDataForInterPolation34(double dCoeff, int& iCoeff1, int& iCoeff2, double& dCoeffRat) const;
    BOOL Get_CoeffDataForInterPolation7 (double dCoeff, int& iCoeff1, int& iCoeff2, double& dCoeffRat) const;

    // 
    BOOL Print_PlateGiderDesignData();

    BOOL GetPgdElemKeyList(ArrElemPairKey& arPgdElemK);
    BOOL Get_StageForce4CPG(ElemPairK ElemK, CPG_FORC_STAG& ForceStage, BOOL bCodeUnit=FALSE);
    BOOL Get_GirderCSLoadOfLCOM(T_STCT_D &StctD, ElemPairK ElemK, T_STAG_K nGdLastStagK, int nLastStepNo, int nDgnLcomKey, CPG_FORC_STAG& ForceStage);

    // Steel Orthotropic Deck 
    BOOL MakeStlOrthDeckDesignData();  
    void InitialStlOrthDeck();
    BOOL Get_ElemListForStlOrthDeck(BOOL bShowMsg, CArray<T_ELEM_K,T_ELEM_K>& arElemK);
    BOOL ExecuteStlOrthDeckDesign();

    // RUS Steel Composite Design
    BOOL MakeRUSStlCompDesignData();
    void InitialStlCompDesign();
    BOOL Get_ElemListForStlCompDesign(BOOL bShowMsg, ArrElemPairKey& arElemK);
    BOOL ExecuteRUSStlCompDesign();
    BOOL MakeRUSStlCompDesign4PrintReport();
    BOOL ExecuteRUSStlCompPrintReport();


protected:
    BOOL ExecuteCSG_Eurocode4();
    BOOL ExecuteCSG_KSCE_LSD15();
    BOOL ExecuteCSG_KSCE_ASD();
    BOOL ExecuteCSG_IRC22();
    BOOL ExecuteCSG_IRC22_15();

    BOOL GetCheckElemKey4UnbracedLength(ArrElemPairKey& aChkElemKey);
    int  IsCantilever(CArray<T_ELEM_K,T_ELEM_K> &aElemList);
    double GetVu4TpEx(double dVu, double dVu_tp);
    BOOL GetCheckElemKeyIncludedMembEnd(CArray<T_ELEM_K, T_ELEM_K>& aElemKey, CArray<T_ELEM_K, T_ELEM_K>& aChkElemKey);

public:
    BOOL MakeUnbracedLengthMembData();
    BOOL MakeUnbracedLengthMembDataBase(const CArray<T_SPAN_K, T_SPAN_K>& SpanKeyList, const EN_BEAM_DGN_TYPE& enBeamType);
    BOOL MakeUnbracedLengthPosition();
    BOOL GetUbLengthElemInfo(ElemPairK ElemK, DgnBeamPairK& nUblmK, DgnBeamPairK& nCurUblmK, ElemPairK& nElemI, ElemPairK& nElemM, ElemPairK& nElemJ, int &nPosI, int &nPosM, int &nPosJ,
        BOOL &bChkUblm, BOOL &bCantilever, BOOL &bMakeMembFor, BOOL &b2SideBgg, ArrElemPairKey& aUblmElemK);
    BOOL GetUbLengthElemInfo(ElemPairK ElemK, DgnBeamPairK& nUblmK, ElemPairK& nElemI, ElemPairK& nElemM, ElemPairK& nElemJ, int &nPosI, int &nPosM, int &nPosJ, BOOL &bCantilever);
	BOOL MakeNumberofSupportByTendonBetweenAnchorage();
    BOOL CalcCrossGirderLength(T_ELEM_K StaElemK, BOOL bConnNodeJ, double CGVec[3], double& dCGLength, BOOL& bNextFind);

    // Write Design Force & Stress 
protected:
    CDesignForceResult* m_pDesignForceResult;
    CDgnResultFileCtrl* m_pDgnResultFileCtrl;

    T_UNIT_INDEX m_PrtUnit;

public:
    void Set_PrintUnit(int nBase_Force,  int nBase_Length);
    void Get_PrintUnit(int& nBase_Force,  int& nBase_Length);


    CDesignForceResult* GetDesignForceResult(){return m_pDesignForceResult;}
    CDgnResultFileCtrl* GetDgnResultFileCtrl(){return m_pDgnResultFileCtrl;}

    BOOL CheckDesignForceData();
    void ClearAndCloseDesignForceData();
    //
    BOOL CheckDesignLcaseForceData();
    void ClearAndCloseDesignLcaseForceData();  
    //
    BOOL CheckDgnLoadCaseForce();
    void ClearAndCloseDgnLoadCaseForce();  

    BOOL CheckDgnLoadCaseForcePart();
    void ClearAndCloseDgnLoadCaseForcePart();  

public:
    BOOL IsPscCode(int iDgnCode);
    BOOL IsCompositePSCSect(ElemPairK ElemK);
    BOOL IsCompositeGenPSCSect(ElemPairK ElemK);
    BOOL IsPSCSect(ElemPairK ElemK);
    BOOL IsCrackedSectionOptionInSectionManger(ElemPairK ElemK);
    BOOL IsCompositeSect(ElemPairK ElemK);
    BOOL IsCsgSect(int nStype, int nStype2);
    BOOL IsComPSCSect(int nStype, int nStype2);
    BOOL IsRcCode(int iDgnCode);//add by gongxing at 2015-12-18
    double GetDgnMoment(double dMu);
	ElemPairK GetElemPairKey4Elem(T_ELEM_K ElemK);
    ElemPairK GetElemPairKey4VB(T_VBEM_K VbemK);
    void GetElemKeyList4ElemPair(UINT nType, const ArrElemPairKey& aElemPairK, CArray<T_ELEM_K, T_ELEM_K>& aElemK);
    void ConvertElemKeyToElemPairKey(const CArray<T_ELEM_K, T_ELEM_K>& aElemK, ArrElemPairKey& aElemPairK);
    void ConvertElemKeyToElemPairKey(const CArray<T_ELEM_K, T_ELEM_K&>& aElemK, ArrElemPairKey& aElemPairK);
    void ConvertElemVBeamKeyToElemPairKey(EN_BEAM_DGN_TYPE enBType, const CArray<T_ELEM_K, T_ELEM_K&>& aElemK, ArrElemPairKey& aElemPairK);
    SectPairK GetSectPairKey(ElemPairK EPairK);
    double GetLengthOfBeam(ElemPairK EPairK);
    BOOL GetGenKfactor(ElemPairK EPairK, T_KFAC_D& KFacD);
    int  GetMaterialKey4ElemPairK(ElemPairK EPairK);

public:
    int GetBlrdElemList(ArrElemPairKey& raList);

private:
    void ConvertCrackDataRatingAASHTO(const T_RTST_PERMIT_BASE &InData, T_BDCW_AASHTO_BASE& OutData, double* dStressDead, double* dStressLive);
    void CalcStressRatioRatingAASHTO(const _BDCW_PSC_LRFD_BASE &InData, T_RTST_PERMIT_BASE &OutData, double dAllowStressRebar, double dAllowStressTendon);

    void InitializeRatingStressAASHTO(ElemPairK CheckElemK, T_GRUP_K CheckGroupK, T_RTST_CASE &MinRtstD);
    void InitializeRatingStressPermitAASHTO(ElemPairK CheckElemK, T_GRUP_K CheckGroupK, T_RTST_PERMIT_CASE &MinRtstPermitD);

    void SummarizeRatingStressAASHTO(int n, double &dComp_RF, double &dTens_RF, UINT nPoint_comp, UINT nPoint_tens, T_RTST_BASE RtstBaseD, T_RTST_CASE &RtstCaseD, T_RTST_CASE &CompMinRtstD, T_RTST_CASE &TensMinRtstD);
    void SummarizeRatingStressPermitAASHTO(int n, double &dTens_RF, T_RTST_PERMIT_BASE RtstBasePermitD, T_RTST_PERMIT_CASE &RtstCasePermitD, T_RTST_PERMIT_CASE &TensMinRtstPermitD);
    void SummarizeRatingFlexAASHTO(int n, double &dPosI_RF, double &dNegI_RF, double &dPosJ_RF, double &dNegJ_RF, double dK, double dWeight, double dCapacityF, BLR_FLEX_DATA *pFlexRes, T_RTBC_CASE& RtbcCaseD, T_RTBC_BASE &RtbcBaseD, T_RTBC_CASE &PosMin, T_RTBC_CASE &NegMin);
    void SummarizeRatinShearAASHTO(int n, double &dShearI_RF, double &dShearJ_RF,double dVn_Cal, double dK, double dWeight, double dCapacityF, T_RTSC_CASE& RtscCaseD, T_RTSC_BASE &RtscBaseD, T_RTSC_CASE &Min);

    void CalcStressDiagnosticRatingAASHTO(ElemPairK ElemK, int nPos, int nCC, int iDiagMethod, BOOL bAppAFK, BOOL bAutoAFK, STRESS_BLRD_RCASE &StressD, T_RTST_BASE& RtstBase, double dDispC, double dDispT, double &dEpsiC, double dEpsiT, double dKb, double dKuser, double &dKa, double &dK, double dEc, double dEcs);
    void CalcStressDiagnosticRatingCompoAASHTO(ElemPairK ElemK, int nPos, int nCC, int iDiagMethod, BOOL bAppAFK, BOOL bAutoAFK, STRESS_BLRD_RCASE &StressD, STRESS_BLRD_RCASE &StressSlabD, T_RTST_BASE& RtstBase, double dDispC, double dDispT, double &dEpsiC, double dEpsiT, double dKb, double dKuser, double &dKa, double &dK, double dEc, double dEcs);
    int  GetConvertNode2To5(int nIJ);

	double GetInitAgeOfStage(const T_STAG_K& StagK, const ElemPairK& ElemK);
    bool   GetInitAgesOfConcrete(const ElemPairK& ElemK, T_CONC_AGE_D& rData);

};
#endif // !defined(AFX_CRCDATACTRL_H__6AC60648_9789_4D89_BE08_66EF09500ECF__INCLUDED_)