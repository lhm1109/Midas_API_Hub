#include "stdafx.h"

#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"


#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\PostCtrl.h"

#include "DgnPscCommon.h"
#include "CRCForceCtrl.h"
#include "DgnDataCtrl.h"
#include "DgnCsgDataCtrl.h"
#include "DgnCsgDataCtrlCS457_R1.h"
#include "DgnCsgDataCtrlNR_GN_CIV_025_06.h"
#include "DgnForceCtrl.h"

#include "DgnStlRatingDataCtrlNR_GN_CIV_025.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnStlRatingDataCtrlNR_GN_CIV_025::CDgnStlRatingDataCtrlNR_GN_CIV_025() : CDgnStlRatingDataCtrl()
{
}

CDgnStlRatingDataCtrlNR_GN_CIV_025::~CDgnStlRatingDataCtrlNR_GN_CIV_025()
{
}

BOOL CDgnStlRatingDataCtrlNR_GN_CIV_025::CheckStrengthRCaseAssessment(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMB_RESULT_D& MembResD, CSG_CHECK_RES_CS457& ResD,
    T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if (!MembResD.pNRGN) return FALSE;

    CSG_MEMBPOS_RES_NR_GN_CIV_025_D TMembRes = *MembResD.pNRGN;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pNRGN = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_STRN_RES_CS457 StrnD;
    CSG_CHECK_RES_NR_GN_CIV_025 TRes;
    TRes.pStrnD = &StrnD;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pNRGN = &TRes;

    MembLcomD.aChkFor[0].nLcomType = 3; // 강합성 들어오면 수정 필요

    m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    //CSG_MEMB_RES_AASHTO_LRFD_STR_D &StrnRes = TRes.StrengthD;

    CheckFlexuralStrengthRCase(ElemK, nPos, nMovType, MembLcomD, *MembResD.pNRGN, TRes, UlsB);

    CheckShearStrengthRCase(ElemK, nPos, nMovType, MembLcomD, *MembResD.pNRGN, TRes, UlsB);

	CheckTransverseStiffenerStrengthRCase(ElemK, nPos, nMovType, MembLcomD, *MembResD.pNRGN, TRes, UlsB);

	CheckFastenerRCase(ElemK, nPos, nMovType, MembLcomD, *MembResD.pNRGN, TRes, UlsB);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrlNR_GN_CIV_025::CheckServiceRCaseAssessment(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD &MembLcomD, CSG_MEMB_RESULT_D& MembResD, CSG_CHECK_RES_CS457& ResD,
    T_ASSL_CSG_BS_BASE& AsslB)    
{
    if (!MembResD.pNRGN) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = *MembResD.pCS457;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_SERV_RES_CS457 ServR;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pServD = &ServR;

    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;

    MembLcomD.aChkFor[0].nLcomType = 1;

    m_pCSGDataCtrl->Check_Service(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    //BOOL bPositiveM = IsPositiveM(MembLcomD.aChkFor[0].ForLC.My());
    //int nPN = bPositiveM ? 0 : 1;

    //AsslB.dsig_top[0] = ServR.SigTop[0].dSigma;
    //AsslB.dsig_top[1] = ServR.SigTop[1].dSigma;
    //AsslB.dsig_bot[0] = ServR.SigBottom[0].dSigma;
    //AsslB.dsig_bot[1] = ServR.SigBottom[1].dSigma;
    //AsslB.dsig_t = ServR.dSigma_t;
    //AsslB.dsig_c = ServR.dSigma_c;
    //AsslB.dsig_c_lim = ServR.dSig_c_lim;
    //AsslB.dsig_t_lim = ServR.dSig_t_lim;
    //
    //AsslB.dfr = ServR.dfr;
    //AsslB.dfr_lim = ServR.dfr_lim;
    //AsslB.nChkRebar = ServR.nChkRebar;
    //
    //AsslB.dft_s = ServR.dft_s;
    //AsslB.dft_s_lim = ServR.dft_s_lim;
    //
    //AsslB.RFcom.dRA = AsslB.ComD.dFc * ServR.dSig_c_lim;
    //AsslB.RFten.dRA = AsslB.ComD.dFc * ServR.dSig_t_lim;
    //if ( ServR.bTopComp )
    //{
    //    const CSG_WEB_PANEL_STRESS_CS457& Sig = ServR.bIsCompLeftCr ? ServR.SigTop[0] : ServR.SigTop[1];
    //    AsslB.RFcom.dSA  = Sig.dSigma;
    //    AsslB.RFcom.dSd  = Sig.dSigma_gr + Sig.dSigma_lt;
    //    AsslB.RFcom.dSst = Sig.dSigma_sv;
    //    AsslB.RFcom.dS   = Sig.dSigma_mv;
    //    //
    //    const CSG_WEB_PANEL_STRESS_CS457& SigB = ServR.bIsTensLeftCr ? ServR.SigBottom[0] : ServR.SigBottom[1];
    //    AsslB.RFten.dSA  = SigB.dSigma;
    //    AsslB.RFten.dSd  = SigB.dSigma_gr + SigB.dSigma_lt;
    //    AsslB.RFten.dSst = SigB.dSigma_sv;
    //    AsslB.RFten.dS   = SigB.dSigma_mv;
    //}
    //else
    //{
    //    const CSG_WEB_PANEL_STRESS_CS457& SigB = ServR.bIsCompLeftCr ? ServR.SigBottom[0] : ServR.SigBottom[1];
    //    AsslB.RFcom.dSA  = SigB.dSigma;
    //    AsslB.RFcom.dSd  = SigB.dSigma_gr + SigB.dSigma_lt;
    //    AsslB.RFcom.dSst = SigB.dSigma_sv;
    //    AsslB.RFcom.dS   = SigB.dSigma_mv;
    //    //
    //    const CSG_WEB_PANEL_STRESS_CS457& SigT = ServR.bIsTensLeftCr ? ServR.SigTop[0] : ServR.SigTop[1];
    //    AsslB.RFten.dSA  = SigT.dSigma;
    //    AsslB.RFten.dSd  = SigT.dSigma_gr + SigT.dSigma_lt;
    //    AsslB.RFten.dSst = SigT.dSigma_sv;
    //    AsslB.RFten.dS   = SigT.dSigma_mv;
    //}
    //AsslB.nSectClass = TMembRes.MembR[nPos].SClassD[nPN].enClass;
    //
    //
    //CalcReserveFactor(nMovType, AsslB.RFcom);
    //CalcReserveFactor(nMovType, AsslB.RFten);
    //
    //AsslB.dA = min(AsslB.RFcom.dA, AsslB.RFten.dA);
    //
    //AsslB.ComD.bTopFlangeWithLS    = ServR.bIsTopFlangeWithLS ? TRUE : FALSE;
    //AsslB.ComD.bBottomFlangeWithLS = ServR.bIsBotFlangeWithLS ? TRUE : FALSE;
    //AsslB.ComD.bWebWithLS          = ServR.bIsWebWithLS ? TRUE : FALSE;        
    
    return TRUE;
}

BOOL CDgnStlRatingDataCtrlNR_GN_CIV_025::CheckShearConnectorRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD,
    T_ASLS_CSG_BS_BASE& AslsB)
{
    return TRUE;
}

BOOL CDgnStlRatingDataCtrlNR_GN_CIV_025::CheckFlexuralStrengthRCase(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if (UlsB.pAsfcB == NULL) return FALSE;
    if (ResD.pStrnD == NULL) return FALSE;

    T_ASFC_CSG_BS_BASE& AsfcB = *UlsB.pAsfcB;

    CSG_FLEX_RES_CS457& FlexD = ResD.pStrnD->FlexD;
    CSG_RAIL_ASSESS_RAT_RES_D& AssRatD = FlexD.RatRes;

    AsfcB.dR = FlexD.dM_R;
    AsfcB.dRA = AsfcB.ComD.dFc * FlexD.dM_D;

    T_ASCA_RAT_RAIL_BS_B& RatD = AsfcB.RatPa;
    RatD.dSBSU = AssRatD.dSBSU;
    RatD.dDBSU = AssRatD.dDBSU;
    RatD.dSRA = static_cast<double>(AssRatD.nSRA);
    RatD.dDRA = static_cast<double>(AssRatD.nDRA);
    RatD.dSUforRA = AssRatD.dSUforRA;
    RatD.dDUforRA = AssRatD.dDUforRA;
    RatD.nSAC = ConvertAssessedCategory(AssRatD.nSAC);
    RatD.nDAC = ConvertAssessedCategory(AssRatD.nDAC);


    //AsfcB.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);
    return TRUE;
}

BOOL CDgnStlRatingDataCtrlNR_GN_CIV_025::CheckShearStrengthRCase(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if (UlsB.pAsscB == NULL) return FALSE;
    if (ResD.pStrnD == NULL) return FALSE;

    T_ASSC_CSG_BS_BASE& AsscB = *UlsB.pAsscB;

    CSG_SHEAR_RES_CS457& ShearD = ResD.pStrnD->ShearD;
    CSG_RAIL_ASSESS_RAT_RES_D& AssRatD = ShearD.RatRes;


    AsscB.dR = ShearD.dV_D;
    AsscB.dRA = AsscB.ComD.dFc * ShearD.dV_D;

    T_ASCA_RAT_RAIL_BS_B& RatD = AsscB.RatPa;
    RatD.dSBSU = AssRatD.dSBSU;
    RatD.dDBSU = AssRatD.dDBSU;
    RatD.dSRA = static_cast<double>(AssRatD.nSRA);
    RatD.dDRA = static_cast<double>(AssRatD.nDRA);
    RatD.dSUforRA = AssRatD.dSUforRA;
    RatD.dDUforRA = AssRatD.dDUforRA;
    RatD.nSAC = ConvertAssessedCategory(AssRatD.nSAC);
    RatD.nDAC = ConvertAssessedCategory(AssRatD.nDAC);

    //AsscB.ComD.SetStiffener(FlexD.bIsTopFlangeWithLS, FlexD.bIsBotFlangeWithLS, FlexD.bIsCompFlangeWithLS, FlexD.bIsWebWithLS);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrlNR_GN_CIV_025::CheckTransverseStiffenerStrengthRCase(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if (UlsB.pAsbsB == NULL) return FALSE;
    if (ResD.pStrnD == NULL) return FALSE;

    T_ASBS_SG_BS_BASE& TranStB = *UlsB.pAsbsB;

    if (MembLcomD.Memb.ChIf.enMembType != EN_Chk_Memb_Main_Girder)
    {
        TranStB.ComD.bChk = FALSE;
    }

    CSG_BEARING_TRAN_STIFF_RES_D& TStiffD = ResD.pStrnD->TranStiffD;
    CSG_RAIL_ASSESS_RAT_RES_D& WebRatD = TStiffD.WebRatR;
    CSG_RAIL_ASSESS_RAT_RES_D& StiffRatD = TStiffD.StiffRatR;
    CSG_RAIL_ASSESS_RAT_RES_D& BuckRatD = TStiffD.BuckRatR;

    // Web
    T_ASCA_RAT_STR_BS_B& YWeb = TranStB.YieldingWeb;
    YWeb.dSigmaR  = TStiffD.dSigma_Dw;
    YWeb.dSigmaRA = TranStB.ComD.dFc*TStiffD.dSigma_Dw;
    YWeb.dSigmaD  = TStiffD.dSigma_es2_DL;
    YWeb.dSigmaSt = TStiffD.dSigma_es2_SLL;
    YWeb.dSigmaDy = TStiffD.dSigma_es2_DLL;

    T_ASCA_RAT_RAIL_BS_B& WebD = YWeb.RatPa;
    WebD.dSBSU = WebRatD.dSBSU;
    WebD.dDBSU = WebRatD.dDBSU;
    WebD.dSRA = static_cast<double>(WebRatD.nSRA);
    WebD.dDRA = static_cast<double>(WebRatD.nDRA);
    WebD.dSUforRA = WebRatD.dSUforRA;
    WebD.dDUforRA = WebRatD.dDUforRA;
    WebD.nSAC = ConvertAssessedCategory(WebRatD.nSAC);
    WebD.nDAC = ConvertAssessedCategory(WebRatD.nDAC);

	// Stiffener
    T_ASCA_RAT_STR_BS_B& YStiff = TranStB.YieldingStiffener;
    YStiff.dSigmaR  = TStiffD.dSigma_Dst;
    YStiff.dSigmaRA = TranStB.ComD.dFc*TStiffD.dSigma_Dst;
    YStiff.dSigmaD  = TStiffD.dSigma_st_DL;
    YStiff.dSigmaSt = TStiffD.dSigma_st_SLL;
    YStiff.dSigmaDy = TStiffD.dSigma_st_DLL;

    T_ASCA_RAT_RAIL_BS_B& StfnD = YStiff.RatPa;
    StfnD.dSBSU = StiffRatD.dSBSU;
    StfnD.dDBSU = StiffRatD.dDBSU;
    StfnD.dSRA = static_cast<double>(StiffRatD.nSRA);
    StfnD.dDRA = static_cast<double>(StiffRatD.nDRA);
    StfnD.dSUforRA = StiffRatD.dSUforRA;
    StfnD.dDUforRA = StiffRatD.dDUforRA;
    StfnD.nSAC = ConvertAssessedCategory(StiffRatD.nSAC);
    StfnD.nDAC = ConvertAssessedCategory(StiffRatD.nDAC);

	// Buckling
    T_ASCA_RAT_NON_BS_B& BuckSt = TranStB.BucklingEffStiffener;
    BuckSt.dValR  = TStiffD.dBuckAllow;
    BuckSt.dValRA = TranStB.ComD.dFc*TStiffD.dBuckAllow;
    BuckSt.dValD  = TStiffD.dBuckRat_DL;
    BuckSt.dValSt = TStiffD.dBuckRat_SLL;
    BuckSt.dValDy = TStiffD.dBuckRat_DLL;

    T_ASCA_RAT_RAIL_BS_B& BuckD = BuckSt.RatPa;
    BuckD.dSBSU = BuckRatD.dSBSU;
    BuckD.dDBSU = BuckRatD.dDBSU;
    BuckD.dSRA = static_cast<double>(BuckRatD.nSRA);
    BuckD.dDRA = static_cast<double>(BuckRatD.nDRA);
    BuckD.dSUforRA = BuckRatD.dSUforRA;
    BuckD.dDUforRA = BuckRatD.dDUforRA;
    BuckD.nSAC = ConvertAssessedCategory(BuckRatD.nSAC);
    BuckD.nDAC = ConvertAssessedCategory(BuckRatD.nDAC);

    return TRUE;
}

BOOL CDgnStlRatingDataCtrlNR_GN_CIV_025::CheckFastenerRCase(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB)
{
    if (UlsB.pAssfB == NULL) return FALSE;
    if (ResD.pStrnD == NULL) return FALSE;

    T_ASSF_SG_BS_BASE& AssfB = *UlsB.pAssfB;

    CSG_STRENGH_FASTENER_RES& FastResD = ResD.pStrnD->FastenerD;
    CSG_RAIL_ASSESS_RAT_RES_D& AssfRatD = FastResD.RatRes;


    T_ASCA_RAT_STR_BS_B& Fastener = AssfB.Fastener;
    Fastener.dSigmaR  = FastResD.dSigma_alw;
    Fastener.dSigmaRA = AssfB.ComD.dFc * FastResD.dSigma_alw;
    Fastener.dSigmaD  = FastResD.dTau_DL;
    Fastener.dSigmaSt = FastResD.dTau_SLL;
    Fastener.dSigmaDy = FastResD.dTau_DLL;

    T_ASCA_RAT_RAIL_BS_B& RatD = Fastener.RatPa;
    RatD.dSBSU = AssfRatD.dSBSU;
    RatD.dDBSU = AssfRatD.dDBSU;
    RatD.dSRA = static_cast<double>(AssfRatD.nSRA);
    RatD.dDRA = static_cast<double>(AssfRatD.nDRA);
    RatD.dSUforRA = AssfRatD.dSUforRA;
    RatD.dDUforRA = AssfRatD.dDUforRA;
    RatD.nSAC = ConvertAssessedCategory(AssfRatD.nSAC);
    RatD.nDAC = ConvertAssessedCategory(AssfRatD.nDAC);

    return TRUE;
}

int CDgnStlRatingDataCtrlNR_GN_CIV_025::ConvertAssessedCategory(enRailAssessedCategory AssCat)
{
    switch (AssCat)
    {
    case EN_Rail_Assess_Cat_A1:
        return D_NR_RAIL_ASSESSED_CATEGORY_A1;
    case EN_Rail_Assess_Cat_A2:
        return D_NR_RAIL_ASSESSED_CATEGORY_A2;
    case EN_Rail_Assess_Cat_A3:
        return D_NR_RAIL_ASSESSED_CATEGORY_A3;
    case EN_Rail_Assess_Cat_B:
        return D_NR_RAIL_ASSESSED_CATEGORY_B;
    case EN_Rail_Assess_Cat_C:
        return D_NR_RAIL_ASSESSED_CATEGORY_C;
    case EN_Rail_Assess_Cat_D:
        return D_NR_RAIL_ASSESSED_CATEGORY_D;
    case EN_Rail_Assess_Cat_E:
        return D_NR_RAIL_ASSESSED_CATEGORY_E;
    case EN_Rail_Assess_Cat_F:
        return D_NR_RAIL_ASSESSED_CATEGORY_F;
    default: ASSERT(0); break;
    }
    return D_NR_RAIL_ASSESSED_CATEGORY_A1;
}
