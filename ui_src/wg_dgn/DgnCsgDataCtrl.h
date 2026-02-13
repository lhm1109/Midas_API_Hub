// DgnPlateGirder.h: interface for the CDgnPlateGirder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCsgDataCtrl_H__INCLUDED_)
#define AFX_DGNCsgDataCtrl_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\wg_db\DB_ST_DN_RESULT_CSG.h"

#include "DgnCsgDataBase.h"
#include <concurrent_unordered_map.h>
#include <thread>
#include <tuple>

#include "HeaderPre.h"

class CPlateGirderDesign;

struct ElemPairKHash
{
    size_t operator()(const ElemPairK& k) const noexcept
    {
        size_t h1 = std::hash<unsigned int>{}(k.first);
        size_t h2 = std::hash<UINT>{}(k.second);

        // hash combine
        return h1 ^ (h2 << 1);
    }
};

struct CSG_FATI_CR_RSLT 
{
    double dLoadTopRatF1[2];
    double dLoadTopRatF2[2];
    double dLoadBotRatF1[2];
    double dLoadBotRatF2[2];
    double dSCPitchF1[2];
    double dSCPitchF2[2];
    double dSCFatigueRatF1[2];
    double dSCFatigueRatF2[2];

    T_CSCR_BASE  SCresD[2];
    T_CSCR_BASE  SCresF1[2];
    T_CSCR_BASE  SCresF2[2];

    T_CSFC_LOAD  LoadTR_F1[2];
    T_CSFC_LOAD  LoadBR_F1[2];
    T_CSFC_LOAD  LoadTR_F2[2];
    T_CSFC_LOAD  LoadBR_F2[2];

    T_CSFC_LOAD  SCLres_F1[2];
    T_CSFC_LOAD  SCLres_F2[2];

    BOOL bFatigueI;
    BOOL bChkFatigueII_IJ[2];
    BOOL bChkFatigueSC_II_IJ[2];

    void Initialize()
    {
        double dMinVal = -1.0*pow(10,21);
        for (int i=0; i<2; ++i)
        {
            dLoadTopRatF1[i]   = dMinVal;
            dLoadTopRatF2[i]   = dMinVal;
            dLoadBotRatF1[i]   = dMinVal;
            dLoadBotRatF2[i]   = dMinVal;
            dSCPitchF1[i]      = dMinVal;
            dSCPitchF2[i]      = dMinVal;
            dSCFatigueRatF1[i] = dMinVal;
            dSCFatigueRatF2[i] = dMinVal;
            SCresD[i].Initialize();
            SCresF1[i].Initialize();
            SCresF2[i].Initialize();
            LoadTR_F1[i].Initialize();
            LoadBR_F1[i].Initialize();
            LoadTR_F2[i].Initialize();
            LoadBR_F2[i].Initialize();
            SCLres_F1[i].Initialize();
            SCLres_F2[i].Initialize();
            bChkFatigueII_IJ[i]    = FALSE;
            bChkFatigueSC_II_IJ[i] = FALSE;
        }
        bFatigueI = FALSE;
    }

    CSG_FATI_CR_RSLT() {Initialize();}

};

struct CSG_INTERIOR_SUPPORT
{
    bool bSupport[2];
    void Initialize()
    {
        bSupport[0] = false;
        bSupport[1] = false;
    }
    CSG_INTERIOR_SUPPORT() { Initialize(); }
};

struct T_COMPOSITE_STEEL_GIRDER_LCOM_RES_D
{
	bool bULSLcom;
	bool bSLSLcom;
	int  nDgnLcomNo;

	T_ASFC_CSG_BS_CASE AsfcD;
	T_ASSC_CSG_BS_CASE AsscD;
	T_ASTC_CSG_BS_CASE AstcD;
	T_ASCB_CSG_BS_CASE AscbD;
	T_ASSL_CSG_BS_CASE AsslD;
	T_ASLS_CSG_BS_CASE AslsD;
	T_ASFG_CSG_BS_D    AsfgD;
	T_ASFS_CSG_BS_D    AsfsD;
	T_ASWY_CSG_BS_D    AswyD;
	T_ASWB_CSG_BS_D    AswbD;
	T_ASWS_CSG_BS_D    AswsD;
	T_ASSB_CSG_BS_D    AssbD;
	T_ASLS_CSG_BS_CASE AslsSLS;
	T_ASFC_CSG_BS_D    CSfcD;
	T_ASSC_CSG_BS_CASE CSscD;
	T_ASCB_CSG_BS_CASE CscbD;

	void Init()
	{
		bULSLcom = false;
		bSLSLcom = false;
		nDgnLcomNo = 0;
		AsfcD.Initialize();
		AsscD.Initialize();
		AstcD.Initialize();
		AscbD.Initialize();
		AsslD.Initialize();
		AslsD.Initialize();
		AsfgD.Initialize();
		AsfsD.Initialize();
		AswyD.Initialize();
		AswbD.Initialize();
		AswsD.Initialize();
		AssbD.Initialize();
		AslsSLS.Initialize();
		CSfcD.Initialize();
		CSscD.Initialize();
		CscbD.Initialize();
	}

	T_COMPOSITE_STEEL_GIRDER_LCOM_RES_D() { Init(); }
	T_COMPOSITE_STEEL_GIRDER_LCOM_RES_D& operator=(const T_COMPOSITE_STEEL_GIRDER_LCOM_RES_D& rData)
	{
		bULSLcom   = rData.bULSLcom;
		bSLSLcom   = rData.bSLSLcom;
		nDgnLcomNo = rData.nDgnLcomNo;
		AsfcD = rData.AsfcD;
		AsscD = rData.AsscD;
		AstcD = rData.AstcD;
		AscbD = rData.AscbD;
		AsslD = rData.AsslD;
		AslsD = rData.AslsD;
		AsfgD = rData.AsfgD;
		AsfsD = rData.AsfsD;
		AswyD = rData.AswyD;
		AswbD = rData.AswbD;
		AswsD = rData.AswsD;
		AssbD = rData.AssbD;
		AslsSLS = rData.AslsSLS;
		CSfcD = rData.CSfcD;
		CSscD = rData.CSscD;
		CscbD = rData.CscbD;

		return *this;
	}
};

struct T_COMPOSITE_STEEL_GIRDER_RES_D
{
	T_ASFC_CSG_BS_D    AsfcD;
	T_ASSC_CSG_BS_CASE AsscD;
	T_ASTC_CSG_BS_CASE AstcD;
	T_ASCB_CSG_BS_CASE AscbD;
	T_ASSL_CSG_BS_D    AsslD;
	T_ASLS_CSG_BS_CASE AslsD;
	T_ASFG_CSG_BS_D    AsfgD;
	T_ASFS_CSG_BS_D    AsfsD;
	T_ASWY_CSG_BS_D    AswyD;
	T_ASWB_CSG_BS_D    AswbD;
	T_ASWS_CSG_BS_D    AswsD;
	T_ASSB_CSG_BS_D    AssbD;
	T_ASLS_CSG_BS_CASE AslsSLS;
	T_ACSF_CSG_BS_D    CSfcD;
	T_ACSV_CSG_BS_D    CSscD;
	T_ASCB_CSG_BS_CASE CscbD;

	std::vector<T_COMPOSITE_STEEL_GIRDER_LCOM_RES_D> vLcomRes;

	void Init()
	{
		AsfcD.Initialize();
		AsscD.Initialize();
		AstcD.Initialize();
		AscbD.Initialize();
		AsslD.Initialize();
		AslsD.Initialize();
		AsfgD.Initialize();
		AsfsD.Initialize();
		AswyD.Initialize();
		AswbD.Initialize();
		AswsD.Initialize();
		AssbD.Initialize();
		AslsSLS.Initialize();
		CSfcD.Initialize();
		CSscD.Initialize();
		CscbD.Initialize();
		vLcomRes.clear();
	}

	T_COMPOSITE_STEEL_GIRDER_RES_D() { Init(); }
	T_COMPOSITE_STEEL_GIRDER_RES_D& operator=(const T_COMPOSITE_STEEL_GIRDER_RES_D& rData)
	{
		AsfcD = rData.AsfcD;
		AsscD = rData.AsscD;
		AstcD = rData.AstcD;
		AscbD = rData.AscbD;
		AsslD = rData.AsslD;
		AslsD = rData.AslsD;
		AsfgD = rData.AsfgD;
		AsfsD = rData.AsfsD;
		AswyD = rData.AswyD;
		AswbD = rData.AswbD;
		AswsD = rData.AswsD;
		AssbD = rData.AssbD;
		AslsSLS = rData.AslsSLS;
		CSfcD = rData.CSfcD;
		CSscD = rData.CSscD;
		CscbD = rData.CscbD;
		vLcomRes = rData.vLcomRes;

		return *this;
	}
};

class CDgn_CSGManager;
struct CSG_MEMB_RES_KSCE_LSD12_D;
struct CSG_MEMB_POSD_KSCE_LSD;
struct CSG_MEMB_RES_AASHTO_LRFD_D;
struct CSG_MEMB_RESULT_D;
struct CSG_DATA_POSD;
struct S_CSGD_D;
struct CSG_MEMB_ULEN;
struct CSG_ULMS_D;
struct CSG_CHECK_RESULT_CODES;
struct STL_KR_RES_D;
struct CSG_FORCE_COMP;
struct CSG_FORCE_D;
struct CSG_ULMF_D;
struct CSG_MEMB_FOR_D;
struct CSG_CHECK_RESULT_CODES;
struct CSG_CHK_RES_DTR_PARAM;
struct CSG_CHECK_RESULT_CODES;
struct CSG_MATL_ELEM;
struct T_DGN_MATL_STEEL;
struct CSG_SECT_POSD;
struct CSG_TRST_SECT;
struct CSG_TRST_POSD;
struct CSG_TSES_POSD;
struct CSG_LCOM_MV_IN;
struct CSG_CHECK_RESULT_CODES;
struct CSG_SECT_ELEM;
struct CSG_CPSC_ELEM;
struct CSG_CPFP_ELEM;
struct CSG_CPCB_ELEM;
struct CSG_DOHL_ELEM;
struct CSG_EBSL_ELEM;
struct CSG_LTBD_ELEM;
struct CSG_DWSP_ELEM;
typedef CArray<CSG_ULMF_D, CSG_ULMF_D&> CSGULMFOR_ARRAY;
namespace dgn
{
	namespace def
	{
		struct TStiffenerMaterial;		
	}
}

class __MY_EXT_CLASS__ CDgnCsgDataCtrl : public CDgnCsgDataBase
{
    // Member Functions.
public:
    CDgnCsgDataCtrl();
    virtual ~CDgnCsgDataCtrl();

    virtual void SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl);
    //void SetDesignCode(int nDgnCode);  

public:
    BOOL InitialData(int nDgnCode);
    void SetCsgDgnCode(int nDgnCode);
    void SetChkCsgDgnElem(ArrElemPairKey& raElemK);
    BOOL Make_CSGDgnData();
    BOOL Make_CSGDgnData4Rating();

    // KSCE_LSD12
    void SetMemberResData(CSG_MEMB_RES_KSCE_LSD12_D &rData);
    BOOL MakeMemberResData(T_ELEM_K ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RES_KSCE_LSD12_D &rData);

    // KSSC_2014
    void SetMemberResDataKSSC2014(CSG_MEMB_RES_AASHTO_LRFD_D &rData);
    BOOL MakeMemberResDataKSSC2014(T_ELEM_K ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RES_AASHTO_LRFD_D &rData);

    // AASHTO LRFD 07/12
    void SetMemberResDataAASHTO(CSG_MEMB_RES_AASHTO_LRFD_D &rData);
    BOOL MakeMemberResDataAASHTO(T_ELEM_K ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RES_AASHTO_LRFD_D &rData);

    void SetMemberResultData(CSG_MEMB_RESULT_D &rData);
    virtual BOOL MakeMemberResultData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &CsgMembD, CSG_MEMB_RESULT_D &rData);

    BOOL Check_CompositeSteelGirder();

    BOOL Get_CsgMembInData(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD &rData);
    BOOL Get_CsgPosData(ElemPairK ElemK, SectPairK SectK, int nPos, CSG_DATA_POSD &rData);
    void ConvertCGSGlobalData(T_CPGD_D &CpgdD, S_CSGD_D &rData);
    BOOL Get_UnbracedLengthData(ElemPairK ElemK, BOOL bKfac, T_KFAC_D &KFacD, CSG_MEMB_ULEN &rData);
    BOOL Get_CsgUlmSection(ElemPairK ElemI, ElemPairK ElemM, ElemPairK ElemJ, int nPosI, int nPosM, int nPosJ, CSG_ULMS_D &rData);
	BOOL Get_CsgUlmSection(ElemPairK ElemK, CSG_MEMB_POSD_KSCE_LSD& rData);
    BOOL Get_CsgUlmsData4MyMax(ElemPairK ElemK, int nPos, CSG_ULMS_D& rData);
    BOOL GetCsgSectionSupport(ElemPairK EPairK, CSG_MEMB_POSD_KSCE_LSD& rData);

    virtual BOOL Check_Constructibility(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);
    virtual BOOL Check_Strength        (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);
    virtual BOOL Check_Service         (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);
    virtual BOOL Check_Fatigue         (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR, CSG_FATI_CR_RSLT &FatiR);
    virtual BOOL Check_ShearConnector  (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);
    virtual BOOL Check_Stiffener       (ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);
    virtual BOOL Check_BearingStiffener(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes, CSG_CHK_RES_DTR_PARAM &DtrR);

    virtual void GetMy4Strength(CSG_CHECK_RESULT_CODES &ChkRes, double &dMuy);

    // KSCE_LSD12
    //BOOL CheckMembRes();
    BOOL Check_ConstructionStage(ElemPairK ElemK, int nLcomK4CS, ArrElemPairKey& aUblmElemK, T_CSLF_CPT &Cslf, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMB_RESULT_D &rMembR, CPG_CSIF_D& NCStageD);
    BOOL Check_ConstructionNoStage(ElemPairK ElemK, int nLcomK4CS, ArrElemPairKey& aUblmElemK, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, ADGNMEMB_FORCE &aMembForPre, CSG_MEMB_RESULT_D &rMembR);
    BOOL Check_Strength_ASD(T_ELEM_K ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD &InD, CSG_MEMB_RES_KSCE_LSD12_D &MembRes, STL_KR_RES_D &rData);

    // KSSC_2014

    // AASHTO LRFD 07/12
    BOOL CheckSupprotNode(int nNode);
    BOOL CheckSupprotNodeElem(ElemPairK ElemK,  int* nNode, BOOL bCheckJ=FALSE);
    BOOL CheckInteriorSupportNodeElem(ElemPairK ElemK, bool* bIJ);

    void ConvertCSGDCOMF(CSG_FORCE_COMP &InD, T_CSGD_COMF &rData);
    void ConvertCSGDForce(CSG_FORCE_D &InD, T_CSGD_FORCE &rData);
    void ConvertCSGEngToUlmForce(CSG_ULMF_D &InD, T_CSGD_ULMF &rData);
    void ConvertCSGEngToUblmForce(BOOL bConsStage, CSG_MEMB_FOR_D &InD, T_CSGD_ULMF &rData);
    void ConvertCsgEngToUblMembForce(CSGULMFOR_ARRAY& aUlmf, T_CSGF_MEMB& rData);
    void ConvertCsgEngToDgnForce(CSG_MEMB_POSD_KSCE_LSD& LcomFor, T_CSGF_MEMB& rData);

    void ConvertCsgUlmfCvlToEng(T_CSGD_ULMF &For, CSG_ULMF_D &rData);
    void ConvertCsgComfCvlToEng(T_CSGD_COMF &For, CSG_FORCE_COMP &rData);
    void ConvertCsgForceCvlToEng( T_CSGD_FORCE &For, CSG_FORCE_D &rData);
    void ConvertCsgUlmForce(int nPos, CSG_FORCE_COMP& For, CSGULMFOR_ARRAY& raData);    
    void ConvertCsgUlm13Force(int nPos13, CSG_FORCE_COMP& For, CSGULMFOR_ARRAY& raData);
    void ConvertCsgUlmMyMaxForce(CSG_FORCE_COMP& For, CSGULMFOR_ARRAY& raData);

    // CSA-S6-10,14

    void ConvertCsgCalcUlmForcePos3(int nPosI, int nPosM, int nPosJ, _DGN_FORC_CRC &ForI, _DGN_FORC_CRC &ForM, _DGN_FORC_CRC &ForJ, CSG_ULMF_D &rData);
    void ConvertCsgCalcUblMForce(int nPos, T_STRB_D &ForD, CSG_FORCE_D &rData) const;
    void ConvertCsgCalcUblMForce4Pos(int nPos, T_STRB_D& ForD, CSG_FORCE_D& rData) const;
    //void ConvertCsgCalcUlmForce(T_STRB_D &MembD, CSG_ULMF_D &rData);
    //void ConvertCsgCalcUlmForceCb(T_STRB_D &MembD, int nMid, int nCom, CSG_ULMF_D &rData);
    void ConvertCsgCalcChkForce(int nPos, _DGN_FORC_CRC &ForNC, _DGN_FORC_CRC &ForLT, _DGN_FORC_CRC &ForST, _DGN_FORC_CRC &ForLC, CSG_FORCE_COMP &rData);
    void ConvertCsgCalcForce(int nPos, _DGN_FORC_CRC &For, CSG_FORCE_D &rData);
    void ConvertCsgCalcForce(int nPos, _DGN_FORC_CRC_EX &For, CSG_FORCE_D &rData);
    void ConvertCsgCalcForce2(int nPos, CSG_FORCE_D &For, _DGN_FORC_CRC &rData);
    //BOOL GetUlmMidIndex(T_STRB_D &MembD, int &nMid, int &nCom);

    BOOL GetDeadLoadCSMaxFactor(int &nLcomK, double &dFactor);
    BOOL GetEractionCSMaxFactor(double &dERFactor1, double &dERFactor2, double &dERFactor3);
    double GetLinkForce(int nOrgLcomNo, int nMaxMinType, int nIJ, int elnod);
    int GetElasticLinkKey4Node(int elnod);
    int GetGeneralLinkKey4Node(int elnod);
    int GetRigidLinkKey4Node(int elnod);

    virtual void ClassNewMembResult(CSG_MEMB_RESULT_D &MembR);
    virtual void ClassNewDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);
    virtual void InitMembResult(CSG_MEMB_RESULT_D &MembR);
    virtual void InitDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);

    void ClassDelDgnResult(CSG_CHECK_RESULT_CODES &ChkResD);

    BOOL Make_ElemSpanMapKey();
    BOOL Make_ElemSpanMapKey_Sub();
    BOOL Make_ElemSpanData();
	BOOL Make_ElemSpanDataBase(const CArray<T_SPAN_K,T_SPAN_K>& aSpanKeyList, const EN_BEAM_DGN_TYPE& enBeamType);

    bool GetLongitudinalStiffenerType1(int nSectType, const ST_SECT_SECTBASE_D& stSect, const dgn::def::TStiffenerMaterial& stMatl, dgn::def::CLongitudinalStiffener* pData);
    bool GetLongitudinalStiffenerType2(int nSectType, const ST_SECT_SECTBASE_D& stSect, const dgn::def::TStiffenerMaterial& stMatl, dgn::def::CLongitudinalStiffener* pData);

    BOOL Get_CsgSectData4Elem(ElemPairK ElemK, BOOL bLongTermSectProp, CSG_SECT_ELEM& SectElem);

    BOOL GetReactionNodeOfElem(T_LCOM_D LcomD, ElemPairK ElemPairK, int nPosIJ, int nMaxMinType, T_REAC_D& rReacD);

protected:

    BOOL Make_CsgMatlDgnData();
    BOOL Make_CsgSectDgnData();
    BOOL Make_CsgTrstDgnData(); // Transverse stiffener
    BOOL Make_CsgCrdpData();    // Corrosion Depth
    BOOL Make_CpgSconElem(); //PMS:LRFD12 BSC-20140205
    BOOL Make_CpgCpfpElem(); //PMS:LRFD12 BSC-20140205
    BOOL Make_CpgCpcbElem(); //PMS:LRFD12 BSC-20140205
    BOOL Make_CpgDohlElem(); // Deck Overhang Loads
    BOOL Make_CpgEbslElem();
    BOOL Make_CpgLtbdElem();
    BOOL Make_CpgDwspElem();
    BOOL Make_CsgUfraElem();
    BOOL Make_CsgMdfsElem();
    BOOL Make_CsgLshrElem();


    BOOL Check_CSG_Fatigue(T_CPGD_D &CpgdD,
                           T_STCT_D &StctD,
                           BOOL bIsPLCB,
                           ElemPairK ElemK,
                           BOOL bChkIJ[],
                           CPG_CSIF_D &NCStageD,
                           ArrElemPairKey& aUblmElemK,
                           CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                           CSG_MEMB_RESULT_D &MembRes,
                           ADGNFORCE &aForceLcom,
                           ADGNFORCE &aForceSTerm,
                           ADGNMEMB_FORCE &aDgnMembFor,
                           ADGNMEMB_FORCE &aDgnMembFor_LT,
                           ADGNMEMB_FORCE &aDgnMembFor_Pre,
                           CArray<T_STRB_D,T_STRB_D&> &aStrbElem,
                           CArray<T_STRB_D,T_STRB_D&> &aStrbElem_LT,
                           CArray<T_STRB_D,T_STRB_D&> &aStrbElem_Pre,
                           CSG_CHECK_RESULT_CODES &ChkResD,
                           T_CSFC_D &CsfcResD,
                           T_CSCR_D &CscrResD,
                           T_CPFC_D &CpfcResD
                           );

    virtual BOOL FindFatiCrRes4Lcom(int nFLoadType, int nIJ, BOOL *pbChkIJ, 
                                    int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo,
                                    CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                    CSG_MEMB_RESULT_D &MembRes,
                                    int *pnFatigueII,
                                    int *pnLoadTop, int *pnLoadBot,
                                    double *pdLoadTopRat, double *pdLoadBotRat,
                                    CSG_CHK_RES_DTR_PARAM &DtrR,
                                    CSG_CHECK_RESULT_CODES &ChkResD,
                                    T_CSCH_REF &RsltRef,
                                    CSG_FATI_CR_RSLT &FRes);

    virtual BOOL FindFatiSCCrRes4Lcom(ElemPairK ElemK, int nIJ, BOOL *pbChkIJ,
                                      int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo, int nMaxMinType,
                                      CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                      CSG_MEMB_RESULT_D &MembRes,
                                      int *pnSCcount,
                                      double *pdSCmaxRat,
                                      CSG_CHK_RES_DTR_PARAM &DtrR,
                                      CSG_CHECK_RESULT_CODES &ChkResD,
                                      T_CSCH_REF &RsltRef);

    virtual BOOL FindFatiSCLoadCrRes4Lcom(ElemPairK ElemK, int nIJ, BOOL *pbChkIJ,
                                          int nFatiLcomType, int nDgnLcomId, int nOrgLcomNo, int nMaxMinType,
                                          CSG_MEMB_POSD_KSCE_LSD &MembLcomD,
                                          CSG_MEMB_RESULT_D &MembRes,
                                          int *pnFatigueII_SC,
                                          int *pnSCFati,
                                          double *pdSCFatigueRat,
                                          CSG_CHK_RES_DTR_PARAM &DtrR,
                                          CSG_CHECK_RESULT_CODES &ChkMaResD,
                                          CSG_CHECK_RESULT_CODES &ChkResD,
                                          T_CSCH_REF &RsltRef,
                                          CSG_FATI_CR_RSLT &FRes);

    virtual void DecisionFatigueRes(CSG_FATI_CR_RSLT &FRes, T_CSFC_D &CsfcResD, T_CSCR_D &CscrResD);

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
                                   ) const override;

    BOOL Check_CriticalResultOfSpan();
	BOOL Check_CriticalResultOfSpanBase(const CArray<T_SPAN_K, T_SPAN_K>& aSpanKeyList, const EN_BEAM_DGN_TYPE& enBeamType);
    void Get_CriticalCsbr(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nMpcount, int &nMncount, ElemPairK &nMpme, ElemPairK &nMnme, int &nPosMp, int &nPosMn, double &dMpmr, double &dMnmr);
    void Get_CriticalCsvr(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nVcount, ElemPairK &nVme, int &nPosV, double &dVmr);
    void Get_CriticalCsac(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nSpcount, int &nSncount, ElemPairK &nSpme, ElemPairK &nSnme, int &nPosSp, int &nPosSn, double &dSpmr, double &dSnm);
    void Get_CriticalCsfc(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nFpcount, int &nFncount, ElemPairK &nFpme, ElemPairK &nFnme, int &nPosFp, int &nPosFn, double &dFpmr, double &dFnmr);
    void Get_CriticalCscr(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nSCcount, ElemPairK &nSCme, int &nPosSC, double &dSCmr); // Shear connector
    void Get_CriticalCssl(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nLScount, ElemPairK &nLSme, int &nPosLS, double &dLSmr); // Longitudinal stiffener
    void Get_CriticalScbr(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nMpcount, int &nMncount, ElemPairK &nMpme, ElemPairK &nMnme, int &nPosMp, int &nPosMn, double &dMpmr, double &dMnmr);
    void Get_CriticalScvr(CPlateGirderDesign *pGirder, ElemPairK ElemK, int &nVcount, ElemPairK &nVme, int &nPosV, double &dVmr);

    //BOOL CalcStiffnessOfStiffener(T_SECT_STIFF_D &Stfn, ST_SECT_STIFF_D &rData);
    BOOL GetEndElemOfSpan(T_SPAN_K SpanK, T_ELEM_K &nElemI, T_ELEM_K &nElemJ);
    BOOL GetEndElemOfSpan4Stage(T_STAG_K StagK, T_SPAN_K SpanK, T_ELEM_K &nElemI, T_ELEM_K &nElemM, T_ELEM_K &nElemJ);

    virtual void SetScbrBase(BOOL bChk, int nLcomK4CS, int nStageK, int nStepK, T_CSCH_REF &rData);
    virtual void SetScvrBase(BOOL bChk, int nLcomK4CS, int nStageK, int nStepK, T_CSCH_REF &rData);
    virtual void SetCsbrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
    virtual void SetCsvrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
    virtual void SetCsslBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
    virtual void SetCsacBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
    virtual void SetCsfcBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
    virtual void SetCscrBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
    virtual void SetBstfBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
    virtual void SetCpbsBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
    //

    virtual void SetCsfcLoadBase(int nType, BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nTopBot, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);
    virtual void SetCsfcFlexBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);
    virtual void SetCsfcShearBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);

    // CSA
    virtual void SetCpfcStudBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
    virtual void SetCpfcSconBase(BOOL bChk, int nDgnLcomNo, int nOrgLcomNo, int nMaxMinType, T_CSCH_REF &rData);
    int  GetSectType(T_SECT_D &rData);


protected:
	// 통합엔진
	virtual UINT GetDgnElemNo() const override final;
	virtual UINT GetDgnPosiNo() const override final;
	virtual T_UNIT_INDEX GetCodeUnit() const override final;
	virtual int  GetDgnLcomNo() const override final;

	virtual bool SetResult(const dgn::def::IResult* pResult) override final;
	virtual bool GetReportInput(dgn::link::MItemInputList& vMembItemInput, dgn::link::MembInfoList& vMembInfo) override final;

	int GetDgnCurrentLcomIndex(int nLcom);
	void SetLcomInfoResultStruct(const CSG_MEMB_POSD_KSCE_LSD& InD, T_COMPOSITE_STEEL_GIRDER_LCOM_RES_D& LcomR);

private:
	void SetDgnLcomNo(int nLcomNo);


protected:	
    void ConvertCsgCalcData(T_SECT_K SectK, int nPos, CPG_SECT_ELEM *pSect, CPG_RBAR_ELEM *pRbar, CSG_DATA_POSD &rData);
    BOOL ConvertCsgCalcMatlData(CPG_MATL_ELEM *pMatlD, CSG_MATL_ELEM &rData);
    void ConvertCsgCalcMatlStlDgn(T_MATD_ALL &MatlDgn, T_DGN_MATL_STEEL &rData);
    void ConvertCsgCalcMatlStlPart(T_MATL_ANALYSIS &AnalMatl, T_MATL_DESIGN &DgnMatl, T_DGN_MATL_STEEL &rData);

    BOOL ConvertCsgSectPolygonData(int nPos, T_SECT_D *pSectD, ST_SECT_SECTBASE_D &rData);
    void ConvertCsgOrgSectData(int nPos, T_SECT_D *pSectD, CSG_SECT_POSD &rData);
    void ConvertCsgSectBaseData(T_SECT_SECTBASE_D *pSect, ST_SECT_SECTBASE_D &rData);
    void ConvertCsgStiffenerData(double dHw, T_SECT_STIFF_D *pStfn, ST_SECT_STIFF_D &rData);
    void ConvertCsgStfnShapeData(T_STIFF_SHAPE_D *pShape, ST_STIFF_SHAPE_D &rData);
    void ConvertCsgStfnProfileData(CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> &aStfnPro, CArray<ST_STIFF_PROFILE_D,ST_STIFF_PROFILE_D&> &arData);
    void ConvertCsgStfnProfileData(T_STIFF_PROFILE_D *pStfnProf, ST_STIFF_PROFILE_D &rData);
	void ConvertCsgStfnProfileDataBottom2Top(double dHw, CArray<ST_STIFF_PROFILE_D, ST_STIFF_PROFILE_D&> &arData);
    void ConvertCsgCalcSectData(CPG_SECT_POSI *pSectD, CSG_SECT_POSD &rData);        
    void ConvertCsgCalcCgtsData(T_CGTS_SSM_D *pCgtsSSM, CSG_TRST_SECT &rData);
    void ConvertCsgCalcCgtsPosData(T_CGTS_SUB_SSM &CgtsSub, CSG_TRST_POSD &rData);
    void ConvertCsgCalcTrstData(CPG_TRST_POSI *pTrstD, CSG_TRST_POSD &rData);
    void ConvertCsgCalcTsesData(CPG_TSES_POSI *pTsesD, CSG_TSES_POSD& rData);
    void ConvertCsgCalcCordData(int nPos, T_CRDP_SSM_D* pCrdp, ST_SECT_SECTBASE_D& rData);
    void ConvertCsgCalcCordPosData(T_CRDP_SUB_SSM& CrdpSub, ST_CORR_DEPTH_B& rData);
    
    void ConvertCsgCalcChkForce4Stage(int nPos, _DGN_FORC_CRC &ForNC, CSG_FORCE_COMP &rData); 
    void ConvertCsgCalcMVLcaseData(double dW, int nLane, CSG_LCOM_MV_IN &rData);


    // Result
    virtual void ConvertCsgScbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData);
    virtual void ConvertCsgScvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData);
    virtual void ConvertCsgCsbrRes(BOOL bPositive, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_F, T_CSCH_REF &rData);
    virtual void ConvertCsgCsvrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_S, T_CSCH_REF &rData);
    virtual void ConvertCsgCsacRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_Ser, T_CSCH_REF &rData);
    virtual void ConvertCsgFatiLoadRes(int nType, int nPart, CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
    virtual void ConvertCsgFatiFlexRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
    virtual void ConvertCsgFatiShearRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
    virtual void ConvertCsgFatiStudRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData);
    virtual void ConvertCsgFatiSconRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData);
    virtual void ConvertCsgCscrRes(CSG_MEMB_RESULT_D &MembR, CSG_CHECK_RESULT_CODES &ChkRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData); 
    virtual void ConvertCsgCscrMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio_p, T_CSCH_REF &rData);
    virtual void ConvertCsgCsslMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, double dRatio, T_CSCH_REF &rData);
    virtual void ConvertCsgCsslMembNoLcomRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSBR_D &CsbrResD, T_CSCH_REF &rData);
    virtual void ConvertCsgCptsMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData);
    virtual void ConvertCsgCptsResLcom(CSG_CHECK_RESULT_CODES &ChkRes, T_CSCH_REF &rData);
    virtual void ConvertCsgBstfMembRes(CSG_MEMB_RESULT_D &MembR, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCH_REF &rData);
    
    void ConvertCsgCscrMembRes(int nPos, CSG_MEMB_RES_KSCE_LSD12_D &MembRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSCR_BASE &rData);
    void ConvertCsgCsslMembRes(int nPos, T_CSBR_D &CsbrResD, CSG_MEMB_RES_KSCE_LSD12_D &MembRes, CSG_MEMB_POSD_KSCE_LSD &LcomResD, T_CSSL_BASE &rData);

	void WriteResultData();

    // Result\AASHTO
    BOOL Check_LcomMaxMinType(UINT nLcomK);	

    BOOL IsCSACode(int nDgnCode);
    BOOL IsAASHTOCode(int nDgnCode);
    BOOL IsLSDCode(int nDgnCode);

    BOOL GetForceByERLoad(_DGN_FORC_CRC &Er_I, _DGN_FORC_CRC &Er_M, _DGN_FORC_CRC &Er_J, _DGN_FORC_CRC &Force_I, _DGN_FORC_CRC &Force_M, _DGN_FORC_CRC &Force_J, double dFacER);
    int  GetLcomTypeIndex4MaxMin();

    virtual BOOL IsChkFatigue2(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes);
    virtual BOOL IsChkFatigueCr(int nFatiLcomType, int nFatigueType2);
    virtual BOOL IsSkipChkScon(int nFatiLcomType, CSG_CHECK_RESULT_CODES &ChkRes);
    virtual BOOL IsChkFatiLoadStud();
    virtual BOOL IsChkFatiTranFlex();
    virtual BOOL IsBeforeSgldDL();
    virtual BOOL IsAppErLoad();
    virtual BOOL IsSCMembChk();


    //BOOL GetERMembForce(T_STCT_D StctD, T_STAG_K StageK, int nSerialStepK, CArray<T_ELEM_K,T_ELEM_K&> &aUblmElemK, T_STRB_D& MembForNC_ER1, T_STRB_D& MembForNC_ER2, T_STRB_D& MembForNC_ER3);

	virtual bool GetUnbracedLength(dgn::def::CUnbracedLength* pData) const override final;

protected:
	void CalcCriticalLcomResult();

	void SetBendingResistance(const dgn::def::IResult* pResult, T_ASFC_CSG_BS_CASE& rData);
	void SetShearResistance(const dgn::def::IResult* pResult, T_ASSC_CSG_BS_CASE& rData);
	void SetCombinedMVResistance(const dgn::def::IResult* pResult, T_ASCB_CSG_BS_CASE& rData);
	void SetLongitudinalShearResistance(const dgn::def::IResult* pResult, T_ASLS_CSG_BS_CASE& rData);
	void SetFlangeWithStiffener(const dgn::def::IResult* pResult, T_ASFG_CSG_BS_D& rData);
	void SetFlangeStiffenerWithStiffener(const dgn::def::IResult* pResult, T_ASFS_CSG_BS_D& rData);
	void SetWebYieldingWithStiffener(const dgn::def::IResult* pResult, T_ASWY_CSG_BS_D& rData);
	void SetWebBucklingWithStiffener(const dgn::def::IResult* pResult, T_ASWB_CSG_BS_D& rData);
	void SetWebStiffenerWithStiffener(const dgn::def::IResult* pResult, T_ASWS_CSG_BS_D& rData);
	void SetSLSStress(const dgn::def::IResult* pResult, T_ASSL_CSG_BS_CASE& rData);
	void SetCSBendingResistance(const dgn::def::IResult* pResult, T_ASFC_CSG_BS_D& rData);
	void SetCSShearResistance(const dgn::def::IResult* pResult, T_ASSC_CSG_BS_CASE& rData);
	void SetCSCombinedMVResistance(const dgn::def::IResult* pResult, T_ASCB_CSG_BS_CASE& rData);

	
protected:
	bool IsCSChecking();

protected:
	Concurrency::concurrent_unordered_map<ElemPairK, T_COMPOSITE_STEEL_GIRDER_RES_D*, ElemPairKHash> m_vRes;
	Concurrency::concurrent_unordered_map<std::thread::id, std::tuple<UINT, UINT, UINT>> m_ElemToLcom;//멀티쓰레드로 돌아갈때만 해당 맵을 사용한다. elem, posi, lcom




protected:
    int m_nDgnCode;

    CDgn_CSGManager *m_pCsgManager;

    std::map<ElemPairK,T_SPAN_K> m_mElemSpanK;
    std::map<ElemPairK,T_SPAN_K> m_mElemSubSpanK;
    std::map<ElemPairK,T_SPAN_BASE> m_mElemSpanD; 
    std::map<ElemPairK,double> m_mElemSpanLength;
    std::map<ElemPairK,CSG_INTERIOR_SUPPORT> m_mInteriorSupport;

public:
    int m_nChkLcomSizeUlmf;

private:
	T_ASFC_CSG_BS_BASE m_AsfcB;

private:
	int m_nDgnLcomNo;
};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNCsgDataCtrl_H__INCLUDED_)
