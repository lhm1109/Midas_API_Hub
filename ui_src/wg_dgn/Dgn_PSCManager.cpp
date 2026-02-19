// Dgn_PSCManager.cpp: implementation of the CDgn_PSCManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\DgnEngine\Src\DGNe_RUS\DgnCalc_Rus_PSC_Struct.h"
#include "Dgn_PSCManager.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\PCDesign.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_PSCManager::CDgn_PSCManager()
{
    m_pBeforeLoss_Tens = NULL;
    m_pBeforeLoss_Comp = NULL;
    m_pTendonStress = NULL;
    m_pAfterLoss_Tens = NULL;
    m_pAfterLoss_Comp = NULL;
    m_pPrincipalBeforeLoss = NULL;
    m_pPrincipalAfterLoss = NULL;
    m_pStressCheck = NULL;
    m_pFlexureStrength_Pos = NULL;
    m_pFlexureStrength_Neg = NULL;
    m_pTensionRebarReq_Top = NULL;
    m_pTensionRebarReq_Bot = NULL;
    m_pCrackCheck_Top = NULL;
    m_pCrackCheck_Bot = NULL;
    m_pCrackCheck = NULL;
    m_pShearDesign = NULL;
    m_pTorsionDesign = NULL;

    m_pMemberRes = NULL;
    m_pBeforeLoss = NULL;
    m_pTendonStressCheck = NULL;
    m_pAfterLoss = NULL;
    m_pPrincipalStressBeforeLoss = NULL;
    m_pPrincipalStressAfterLoss = NULL;
    m_pFlexureStrength = NULL;
    m_pShearCheck = NULL;
    m_pTorsionCheck = NULL;
    m_pFatigueCheck = NULL;
}

CDgn_PSCManager::~CDgn_PSCManager()
{

}

BOOL CDgn_PSCManager::InitialData(int iDgnType, int iDgnCode)
{
    HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(iDgnType, iDgnCode);
    if ( hDllCalc == NULL ) return FALSE;

    /*
    if(iNationType != DGNENGINE_US_DLL)
    {
        m_pBeforeLoss_Tens = NULL;
        m_pBeforeLoss_Comp = NULL;
        m_pTendonStress = NULL;
        m_pAfterLoss_Tens = NULL;
        m_pAfterLoss_Comp = NULL;
        m_pPrincipalBeforeLoss = NULL;
        m_pPrincipalAfterLoss = NULL;
        m_pFlexureStrength_Pos = NULL;
        m_pFlexureStrength_Neg = NULL;
        m_pTensionRebarReq_Top = NULL;
        m_pTensionRebarReq_Bot = NULL;
        m_pCrackCheck_Top = NULL;
        m_pCrackCheck_Bot = NULL;
        m_pShearDesign = NULL;
        m_pTorsionDesign = NULL;
    }
    else
    {
    */

    if ( CDBLib::IsPscCodeAASHTO(iDgnCode) ||
        CDBLib::IsPscCodeCSA(iDgnCode) )
    {
        m_pBeforeLoss_Tens     = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowbleStress_Conc_BeforeLosses_Tens");
        if ( !m_pBeforeLoss_Tens ) return FALSE;

        m_pBeforeLoss_Comp     = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowbleStress_Conc_BeforeLosses_Comp");
        if ( !m_pBeforeLoss_Comp ) return FALSE;

        m_pAfterLoss_Tens      = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowableStress_Conc_AfterLosses_Tens");
        if ( !m_pAfterLoss_Tens ) return FALSE;

        m_pAfterLoss_Comp      = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowableStress_Conc_AfterLosses_Comp");
        if ( !m_pAfterLoss_Comp ) return FALSE;

        m_pPrincipalBeforeLoss = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowablePrincipalStress_Conc_BeforeLosses");
        if ( !m_pPrincipalBeforeLoss ) return FALSE;

        m_pPrincipalAfterLoss  = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowablePrincipalStress_Conc_AfterLosses");
        if ( !m_pPrincipalAfterLoss ) return FALSE;

        m_pTendonStress        = (PDGNENGINE_FUNC2*)GetProcAddress(hDllCalc, "Check_AllowableStress_Tendon");
        if ( !m_pTendonStress ) return FALSE;

        m_pFlexureStrength_Pos = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_FlexureStrength_Pos");
        if ( !m_pFlexureStrength_Pos ) return FALSE;

        m_pFlexureStrength_Neg = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_FlexureStrength_Neg");
        if ( !m_pFlexureStrength_Neg ) return FALSE;

        m_pTensionRebarReq_Top = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_TensionRebarReq_Top");
        if ( !m_pTensionRebarReq_Top ) return FALSE;

        m_pTensionRebarReq_Bot = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_TensionRebarReq_Bot");
        if ( !m_pTensionRebarReq_Bot ) return FALSE;

        m_pCrackCheck_Top          = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_CrackCheck_Top");
        if ( !m_pCrackCheck_Top ) return FALSE;

        m_pCrackCheck_Bot          = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_CrackCheck_Bot");
        if ( !m_pCrackCheck_Bot ) return FALSE;

        m_pCrackCheck          = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_CrackCheck");
        if ( !m_pCrackCheck ) return FALSE;

        m_pShearDesign         = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_ShearDesign");
        if ( !m_pShearDesign ) return FALSE;

        m_pTorsionDesign       = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_TorsionDesign");
        if ( !m_pTorsionDesign ) return FALSE;

    }
    else if ( CDBLib::IsPscCodeLSD(iDgnCode) || iDgnCode==IRC_112_2011_PSC || iDgnCode==IRS_PSC || iDgnCode == IRC_112_2020_PSC)
    {
        m_pBeforeLoss          = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowbleStress_Conc_BeforeLosses");
        if ( !m_pBeforeLoss ) return FALSE;

        m_pAfterLoss           = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowableStress_Conc_AfterLosses");
        if ( !m_pAfterLoss ) return FALSE;

        m_pTendonStressCheck   = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowableStress_Tendon_Check");
        if ( !m_pTendonStressCheck ) return FALSE;

        m_pPrincipalStressBeforeLoss = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowablePrincipalStress_BeforeLosses");
        if ( !m_pPrincipalStressBeforeLoss ) return FALSE;

        m_pPrincipalStressAfterLoss  = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowablePrincipalStress_AfterLosses");
        if ( !m_pPrincipalStressAfterLoss ) return FALSE;

        //m_pTendonStress        = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowableStress_Tendon"));
        //if(!m_pTendonStress) return FALSE;

        if ( CDBLib::IsPscCodeLSD(iDgnCode) || iDgnCode==IRC_112_2011_PSC || iDgnCode==IRS_PSC || iDgnCode == IRC_112_2020_PSC)
        {
            m_pCrackCheck          = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_CrackCheck");
            if ( !m_pCrackCheck ) return FALSE;
        }

        if ( iDgnCode==EUROCODE2_2_05_PSC || iDgnCode==IRC_112_2011_PSC || iDgnCode==IRS_PSC || iDgnCode == IRC_112_2020_PSC)
        {
            m_pFatigueCheck         = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_FatigueCheck");
            if ( !m_pFatigueCheck ) return FALSE;
        }

        m_pFlexureStrength     = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_FlexureStrength");
        if ( !m_pFlexureStrength ) return FALSE;


        m_pShearCheck         = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_ShearCheck");
        if ( !m_pShearCheck ) return FALSE;

        m_pTorsionCheck       = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_TorsionCheck");
        if ( !m_pTorsionCheck ) return FALSE;
    }
    else if ( iDgnCode==SNiP_20503_84_PSC || iDgnCode==SP_35_13330_11_PSC || iDgnCode==SNiP_20503_84_PSC_MKS || iDgnCode==SP_35_13330_11_PSC_MKS )
    {
        m_pSetMembRes = (PDGNENGINE_SETD*)GetProcAddress(hDllCalc, "Set_PSC_MembData");
        if ( !m_pSetMembRes ) return FALSE;

        m_pMemberRes = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_MemberData");
        if ( !m_pMemberRes ) return FALSE;

        m_pStressCheck = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_StressCheck");
        if ( !m_pStressCheck ) return FALSE;

        m_pTendonStressCheck   = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_AllowableStress_Tendon_Check");
        if ( !m_pTendonStressCheck ) return FALSE;

        m_pCrackCheck          = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_CrackCheck");
        if ( !m_pCrackCheck ) return FALSE;

        m_pFlexureStrength     = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_FlexureStrength");
        if ( !m_pFlexureStrength ) return FALSE;

        m_pShearCheck         = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_ShearCheck");
        if ( !m_pShearCheck ) return FALSE;

        //m_pTorsionCheck       = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_TorsionCheck");
        //if(!m_pTorsionCheck) return FALSE;

        m_pFatigueCheck         = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_FatigueCheck");
        if ( !m_pFatigueCheck ) return FALSE;

    }
    else if ( iDgnCode==AS_5100_5_17_PSC )
    {
        m_pStressCheck = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_StressCheckAS");
        if ( !m_pStressCheck ) return FALSE;

        m_pTendonStress   = (PDGNENGINE_FUNC2*)GetProcAddress(hDllCalc, "Check_AllowableStress_Tendon");
        if ( !m_pTendonStress ) return FALSE;

        m_pTransferStress = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_TransferStressAS");
        if ( !m_pTransferStress ) return FALSE;

        m_pCrackCheck          = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_CrackAS");
        if ( !m_pCrackCheck ) return FALSE;

		m_pFlexureStrength     = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_FlexureStrengthAS");
        if ( !m_pFlexureStrength ) return FALSE;

        m_pShearDesign         = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_ShearDesign");
        if ( !m_pShearDesign ) return FALSE;

        m_pTorsionDesign       = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_TorsionDesign");
        if ( !m_pTorsionDesign ) return FALSE;

        m_pFatigueCheck         = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_FatigueAS");
        if ( !m_pFatigueCheck ) return FALSE;
    }
    else if ( iDgnCode==BS5400_90_PSC || iDgnCode == TMH07_3_1989  || iDgnCode==BD_44_15_PSC || iDgnCode==CS_455_PSC )
    {
        m_pStressCheck = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_StressBS");
        if ( !m_pStressCheck ) return FALSE;

        m_pTendonStressCheck   = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_AllowableStress_Tendon_Check");
        if ( !m_pTendonStressCheck ) return FALSE;

        m_pCrackCheck          = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_CrackBS");
        if ( !m_pCrackCheck ) return FALSE;

        m_pFlexureStrength     = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_FlexureStrength");
        if ( !m_pFlexureStrength ) return FALSE;

        m_pShearDesign         = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_ShearCheck");
        if ( !m_pShearDesign ) return FALSE;

        m_pTorsionDesign       = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Check_TorsionCheck");
        if ( !m_pTorsionDesign ) return FALSE;

    }
    else ASSERT(0);

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_FysgAASHTO(const T_FYSG_D& FysgD, T_FYSG_AASHTO& rData)
{
    for ( int i=0; i<4; i++ )
    {
        rData.FysgBase[i].bCHK  = FysgD.FysgBase[i].bCHK;
        rData.FysgBase[i].StagK = FysgD.FysgBase[i].StagK;
        rData.FysgBase[i].dFT   = FysgD.FysgBase[i].dFT;
        rData.FysgBase[i].dFB   = FysgD.FysgBase[i].dFB;
        rData.FysgBase[i].dFTL  = FysgD.FysgBase[i].dFTL;
        rData.FysgBase[i].dFBL  = FysgD.FysgBase[i].dFBL;
        rData.FysgBase[i].dFTR  = FysgD.FysgBase[i].dFTR;
        rData.FysgBase[i].dFBR  = FysgD.FysgBase[i].dFBR;
        rData.FysgBase[i].dFMAX = FysgD.FysgBase[i].dFMAX;
        rData.FysgBase[i].dALW  = FysgD.FysgBase[i].dALW;
        rData.FysgBase[i].bOK   = FysgD.FysgBase[i].bOK;

        rData.FysgBase[i].dTtfc  = FysgD.FysgBase[i].dTtfc;
        rData.FysgBase[i].dAct   = FysgD.FysgBase[i].dAct;
        rData.FysgBase[i].dTtfs  = FysgD.FysgBase[i].dTtfs;
        rData.FysgBase[i].dAst   = FysgD.FysgBase[i].dAst;
        rData.FysgBase[i].dFs    = FysgD.FysgBase[i].dFs;
        rData.FysgBase[i].dfci   = FysgD.FysgBase[i].dfci;
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypcKSCE_FypcAASHTO(const T_FYPC_KSCE& FypcD, T_FYPC_AASHTO& rData)
{
    rData.bCHK   = FypcD.bCHK;
    rData.dFDL1  = FypcD.dFDL1;
    rData.dFDL2  = FypcD.dFDL2;
    rData.dFLL1  = FypcD.dFLL;
    rData.dAFDL1 = FypcD.dAFDL1;
    rData.dAFDL2 = FypcD.dAFDL2;
    rData.dAFLL1  = FypcD.dAFLL;
    rData.bOK    = FypcD.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_FycmAASHTO(const T_FYCM_D& FycmD, T_FYCM_AASHTO& rData)
{
    for ( int i=0; i<4; i++ )
    {
        rData.FycmBase[i].bCHK  = FycmD.FycmBase[i].bCHK;
        rData.FycmBase[i].LcomK = FycmD.FycmBase[i].LcomK;
        rData.FycmBase[i].iMax  = FycmD.FycmBase[i].iMax;
        rData.FycmBase[i].iKind = FycmD.FycmBase[i].iKind;
        rData.FycmBase[i].dFT   = FycmD.FycmBase[i].dFT;
        rData.FycmBase[i].dFB   = FycmD.FycmBase[i].dFB;
        rData.FycmBase[i].dFTL  = FycmD.FycmBase[i].dFTL;
        rData.FycmBase[i].dFBL  = FycmD.FycmBase[i].dFBL;
        rData.FycmBase[i].dFTR  = FycmD.FycmBase[i].dFTR;
        rData.FycmBase[i].dFBR  = FycmD.FycmBase[i].dFBR;
        rData.FycmBase[i].dFMAX = FycmD.FycmBase[i].dFMAX;
        rData.FycmBase[i].dALW  = FycmD.FycmBase[i].dALW;
        rData.FycmBase[i].bOK   = FycmD.FycmBase[i].bOK;
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysgD_FysgPSC(const T_FYSG_D& FysgD, T_FYSG_PSC& rData)
{
    for ( int i=0; i<4; i++ )
    {
        rData.FysgBase[i].bCHK   = FysgD.FysgBase[i].bCHK;
        rData.FysgBase[i].StageK = FysgD.FysgBase[i].StagK;
        rData.FysgBase[i].dFT    = FysgD.FysgBase[i].dFT;
        rData.FysgBase[i].dFB    = FysgD.FysgBase[i].dFB;
        rData.FysgBase[i].dFTL   = FysgD.FysgBase[i].dFTL;
        rData.FysgBase[i].dFBL   = FysgD.FysgBase[i].dFBL;
        rData.FysgBase[i].dFTR   = FysgD.FysgBase[i].dFTR;
        rData.FysgBase[i].dFBR   = FysgD.FysgBase[i].dFBR;
        rData.FysgBase[i].dFMAX  = FysgD.FysgBase[i].dFMAX;
        rData.FysgBase[i].dALW   = FysgD.FysgBase[i].dALW;
        rData.FysgBase[i].bOK    = FysgD.FysgBase[i].bOK;
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycmD_FycmPSC(const T_FYCM_D& FycmD, T_FYCM_PSC& rData)
{
    for ( int i=0; i<4; i++ )
    {
        rData.FycmBase[i].bCHK  = FycmD.FycmBase[i].bCHK;
        rData.FycmBase[i].LcomK = FycmD.FycmBase[i].LcomK;
        rData.FycmBase[i].iMax  = FycmD.FycmBase[i].iMax;
        rData.FycmBase[i].iKind = FycmD.FycmBase[i].iKind;
        rData.FycmBase[i].dFT   = FycmD.FycmBase[i].dFT;
        rData.FycmBase[i].dFB   = FycmD.FycmBase[i].dFB;
        rData.FycmBase[i].dFTL  = FycmD.FycmBase[i].dFTL;
        rData.FycmBase[i].dFBL  = FycmD.FycmBase[i].dFBL;
        rData.FycmBase[i].dFTR  = FycmD.FycmBase[i].dFTR;
        rData.FycmBase[i].dFBR  = FycmD.FycmBase[i].dFBR;
        rData.FycmBase[i].dFMAX = FycmD.FycmBase[i].dFMAX;
        rData.FycmBase[i].dALW  = FycmD.FycmBase[i].dALW;
        rData.FycmBase[i].bOK   = FycmD.FycmBase[i].bOK;
    }

    return TRUE;
}

/*
BOOL CDgn_PSCManager::ConvertFpmtJtgD_FpsgPSC(const T_FPMT_JTG& InData, T_FPSG_PSC& rData)
{
    for(int i=0; i<4; i++)
    {
      rData.FpsgBase[i].bCHK   = InData.FpmtBase[i].bCHK ;
      rData.FpsgBase[i].StageK = InData.FpmtBase[i].LcomK;
        rData.FpsgBase[i].iMax   = InData.FpmtBase[i].iMax ;
        rData.FpsgBase[i].dFP1   = InData.FpmtBase[i].dFP1 ;
      rData.FpsgBase[i].dFP2   = InData.FpmtBase[i].dFP2 ;
      rData.FpsgBase[i].dFP3   = InData.FpmtBase[i].dFP3 ;
      rData.FpsgBase[i].dFP4   = InData.FpmtBase[i].dFP4 ;
      rData.FpsgBase[i].dFP5   = InData.FpmtBase[i].dFP5 ;
      rData.FpsgBase[i].dFP6   = InData.FpmtBase[i].dFP6 ;
      rData.FpsgBase[i].dFP7   = InData.FpmtBase[i].dFP7 ;
      rData.FpsgBase[i].dFP8   = InData.FpmtBase[i].dFP8 ;
      rData.FpsgBase[i].dFP9   = InData.FpmtBase[i].dFP9 ;
        rData.FpsgBase[i].dFP10  = InData.FpmtBase[i].dFP10;
        rData.FpsgBase[i].dFMAX  = InData.FpmtBase[i].dFMAX;
        rData.FpsgBase[i].dAFP   = InData.FpmtBase[i].dAFP ;
        rData.FpsgBase[i].bOK    = InData.FpmtBase[i].bOK  ;
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpmsJtgD_FpmsPSC(const T_FPMS_JTG& InData, T_FPMS_PSC& rData)
{
    for(int i=0; i<4; i++)
    {
      rData.FpmsBase[i].bCHK  = InData.FpmsBase[i].bCHK ;
      rData.FpmsBase[i].LcomK = InData.FpmsBase[i].LcomK;
        rData.FpmsBase[i].iMax  = InData.FpmsBase[i].iMax ;
        rData.FpmsBase[i].dFP1  = InData.FpmsBase[i].dFP1 ;
      rData.FpmsBase[i].dFP2  = InData.FpmsBase[i].dFP2 ;
      rData.FpmsBase[i].dFP3  = InData.FpmsBase[i].dFP3 ;
      rData.FpmsBase[i].dFP4  = InData.FpmsBase[i].dFP4 ;
      rData.FpmsBase[i].dFP5  = InData.FpmsBase[i].dFP5 ;
      rData.FpmsBase[i].dFP6  = InData.FpmsBase[i].dFP6 ;
      rData.FpmsBase[i].dFP7  = InData.FpmsBase[i].dFP7 ;
      rData.FpmsBase[i].dFP8  = InData.FpmsBase[i].dFP8 ;
      rData.FpmsBase[i].dFP9  = InData.FpmsBase[i].dFP9 ;
        rData.FpmsBase[i].dFP10 = InData.FpmsBase[i].dFP10;
        rData.FpmsBase[i].dFMAX = InData.FpmsBase[i].dFMAX;
        rData.FpmsBase[i].dAFP  = InData.FpmsBase[i].dAFP ;
        rData.FpmsBase[i].bOK   = InData.FpmsBase[i].bOK  ;
    }

    return TRUE;
}
*/

BOOL CDgn_PSCManager::Calc_AllowbleStressBeforeLoss(UINT nType, PSC_DATA_POSI& PosiD, _FYSG_PSC_LRFD_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_FYSG_PSC_LRFD_BASE);

    if ( m_pBeforeLoss_Comp == NULL || m_pBeforeLoss_Tens == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = FALSE;
    if ( nType==0 ) bCheck = (*m_pBeforeLoss_Comp)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    else         bCheck = (*m_pBeforeLoss_Tens)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_AllowbleStressTendonLoss(int iCivilCode, PSC_TNDN_TDNA& PosiD, _FYPC_AASHTO& rData)
{
    size_t TSize_In    = sizeof(PSC_TNDN_TDNA);
    size_t TSize_Res   = sizeof(_FYPC_AASHTO);

    if ( m_pTendonStress == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pTendonStress)(iCivilCode, &PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_AllowbleStressAfterLoss(UINT nType, PSC_DATA_POSI& PosiD, _FYCM_PSC_LRFD_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_FYCM_PSC_LRFD_BASE);

    if ( m_pAfterLoss_Tens == NULL || m_pAfterLoss_Comp == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = FALSE;
    if ( nType==1 ) bCheck = (*m_pAfterLoss_Tens)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    else         bCheck = (*m_pAfterLoss_Comp)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_AllowblePrincipalStressBeforeLoss(UINT nType, PSC_DATA_POSI& PosiD, _FPMT_PSC_LRFD_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_FPMT_PSC_LRFD_BASE);

    if ( m_pPrincipalBeforeLoss == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pPrincipalBeforeLoss)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_AllowblePrincipalStressAfterLoss(UINT nType, PSC_DATA_POSI& PosiD, _FPMT_PSC_LRFD_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_FPMT_PSC_LRFD_BASE);

    if ( m_pPrincipalAfterLoss == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pPrincipalAfterLoss)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_FlexureStrength(UINT nType, PSC_DATA_POSI& PosiD, _BDCY_PSC_LRFD_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_BDCY_PSC_LRFD_BASE);

    if ( m_pFlexureStrength_Pos == NULL || m_pFlexureStrength_Neg == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = FALSE;

    if ( nType==0 ) bCheck = (*m_pFlexureStrength_Pos)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    else         bCheck = (*m_pFlexureStrength_Neg)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_TensionRebarReq(UINT nType, PSC_DATA_POSI& PosiD, _TBAR_PSC_LRFD_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_TBAR_PSC_LRFD_BASE);

    if ( m_pTensionRebarReq_Top == NULL || m_pTensionRebarReq_Bot == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = FALSE;
    if ( nType==0 ) bCheck = (*m_pTensionRebarReq_Top)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    else         bCheck = (*m_pTensionRebarReq_Bot)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_CrackCheck(UINT nType, PSC_DATA_POSI& PosiD, _BDCW_PSC_LRFD_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_BDCW_PSC_LRFD_BASE);

    if ( m_pCrackCheck_Top == NULL || m_pCrackCheck_Bot == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = FALSE;

    if ( nType==0 ) bCheck = (*m_pCrackCheck_Top)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    else bCheck = (*m_pCrackCheck_Bot)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_ShearDesign(UINT nType, PSC_DATA_POSI& PosiD, _CUMS_AASHTO_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_CUMS_AASHTO_BASE);

    if ( m_pShearDesign == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pShearDesign)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_TorsionDesign(UINT nType, PSC_DATA_POSI& PosiD, _STCM_AASHTO_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_STCM_AASHTO_BASE);

    if ( m_pTorsionDesign == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pTorsionDesign)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}



BOOL CDgn_PSCManager::Calc_PscAllowbleStressBeforeLoss(UINT nType, PSC_DATA_POSI& PosiD, PSC_FYSG_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_FYSG_BASE);

    if ( m_pBeforeLoss==NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = FALSE;

    PosiD.CalcD.iFysgCalcType = nType;
    bCheck = (*m_pBeforeLoss)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscAllowbleStressAfterLoss(UINT nType, PSC_DATA_POSI& PosiD, PSC_FYCM_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_FYCM_BASE);

    if ( m_pAfterLoss == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = FALSE;

    PosiD.CalcD.iFycmCalcType = nType;
    bCheck = (*m_pAfterLoss)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscAllowbleStressTendonLoss(PSC_DATA_TNDN& TndnD, PSC_FYPC_D& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_TNDN);
    size_t TSize_Res   = sizeof(PSC_FYPC_D);

    if ( m_pTendonStressCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = FALSE;

    bCheck = (*m_pTendonStressCheck)(&TndnD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscAllowblePrincipalStressBeforeLoss(UINT nType, PSC_DATA_POSI& PosiD, PSC_FPSG_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_FPSG_BASE);

    if ( m_pPrincipalStressBeforeLoss == NULL ) return FALSE;

    PosiD.CalcD.iFpsgCalcType = nType;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pPrincipalStressBeforeLoss)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscAllowblePrincipalStressAfterLoss(UINT nType, PSC_DATA_POSI& PosiD, PSC_FPMS_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_FPMS_BASE);

    if ( m_pPrincipalStressAfterLoss == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pPrincipalStressAfterLoss)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscCrackCheck(UINT nType, PSC_DATA_POSI& PosiD, PSC_BDCW_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_BDCW_BASE);

    if ( m_pCrackCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pCrackCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscFatigeCheck(UINT nType, PSC_DATA_POSI& PosiD, PSC_FATG_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_FATG_BASE);

    if ( m_pFatigueCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pFatigueCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;

}

BOOL CDgn_PSCManager::Calc_PscFlexureStrength(UINT nType, PSC_DATA_POSI& PosiD, PSC_FRCR_D& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_FRCR_D);

    if ( m_pFlexureStrength == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pFlexureStrength)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscShearDesign(UINT nType, PSC_DATA_POSI& PosiD, PSC_CUMS_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_CUMS_BASE);

    if ( m_pShearCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pShearCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscTorsionDesign(UINT nType, PSC_DATA_POSI& PosiD, PSC_CRMT_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_CRMT_BASE);

    if ( m_pTorsionCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pTorsionCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);
    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}


void CDgn_PSCManager::Set_MemberResDataSNiP(PSC_RUS_MEMB_RES_D &rData)
{
    if ( m_pSetMembRes == NULL ) return;

    size_t TSize_Res   = sizeof(PSC_RUS_MEMB_RES_D);

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pSetMembRes)(&rData, TSize_Res, bVersionOK);
}

BOOL CDgn_PSCManager::Calc_MemberResData(PSC_DATA_MEMB &MembD, PSC_RUS_MEMB_RES_D &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_MEMB);
    size_t TSize_Res   = sizeof(PSC_RUS_MEMB_RES_D);

    if ( m_pMemberRes == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pMemberRes)(&MembD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;

}
BOOL CDgn_PSCManager::Calc_SecondGroupLSSNiP(PSC_DATA_POSI &PosiD, PSC_STRS_CHK_POSI &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_STRS_CHK_POSI);

    if ( m_pCrackCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pStressCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;

}

BOOL CDgn_PSCManager::Calc_PscCrackCheckSNiP(PSC_DATA_POSI &PosiD, PSC_BDCW_SNIP_BASE &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_BDCW_SNIP_BASE);

    if ( m_pCrackCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pCrackCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscShearDesignSNiP(PSC_DATA_POSI &PosiD, PSC_CUMS_SNIP_BASE &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_CUMS_SNIP_BASE);

    if ( m_pShearCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pShearCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscFatigueDesignSNiP(PSC_DATA_POSI &PosiD, PSC_FATI_SNIP_BASE &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_FATI_SNIP_BASE);

    if ( m_pFatigueCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pFatigueCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// AS 5100:5

BOOL CDgn_PSCManager::Calc_PscStressCheckAS(PSC_DATA_POSI &PosiD, PSC_LRFD_STRS_CHK_POSI &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_LRFD_STRS_CHK_POSI);

    if ( m_pStressCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pStressCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscTransferStressAS(PSC_DATA_POSI &PosiD, _TRAN_PSC_BASE& rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_TRAN_PSC_BASE);

    if ( m_pTransferStress == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pTransferStress)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscCrackCheckAS(PSC_DATA_POSI &PosiD, _BDCW_PSC_LRFD_BASE &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_BDCW_PSC_LRFD_BASE);

    if ( m_pCrackCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pCrackCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscFlexureStrengthAS(PSC_DATA_POSI &PosiD, _BDCY_PSC_LRFD_BASE &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_BDCY_PSC_LRFD_BASE);

    if ( m_pFlexureStrength == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pFlexureStrength)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscShearTorsionStrengthAS(PSC_DATA_POSI &PosiD, _CUMS_AASHTO_BASE &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_CUMS_AASHTO_BASE);

    if ( m_pShearDesign == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pShearDesign)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscShearTorsionStrengthAS(PSC_DATA_POSI &PosiD, _STCM_AASHTO_BASE &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(_STCM_AASHTO_BASE);

    if ( m_pTorsionDesign == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pTorsionDesign)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

// BS5400:4, BD44/15
BOOL CDgn_PSCManager::Calc_PscStressCheckBS(PSC_DATA_POSI &PosiD, PSC_STRS_CHK_POSI &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_STRS_CHK_POSI);

    if ( m_pStressCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pStressCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscCrackCheckBS(PSC_DATA_POSI &PosiD, PSC_BDCW_BASE &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_BDCW_BASE);

    if ( m_pCrackCheck == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pCrackCheck)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscFlexureStrengthBS(PSC_DATA_POSI &PosiD, PSC_FRCR_D &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_FRCR_D);

    if ( m_pFlexureStrength == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pFlexureStrength)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscShearTorsionStrengthBS(PSC_DATA_POSI &PosiD, PSC_CUMS_BASE &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_CUMS_BASE);

    if ( m_pShearDesign == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pShearDesign)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::Calc_PscShearTorsionStrengthBS(PSC_DATA_POSI &PosiD, PSC_CRMT_BASE &rData)
{
    size_t TSize_In    = sizeof(PSC_DATA_POSI);
    size_t TSize_Res   = sizeof(PSC_CRMT_BASE);

    if ( m_pTorsionDesign == NULL ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pTorsionDesign)(&PosiD, &rData, TSize_In, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_EngineToCVL(const _FYSG_PSC_LRFD_BASE& InData, T_FYSG_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StagK  = InData.StagK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dFT_PS   = InData.dFT_PS;
    rData.dFB_PS   = InData.dFB_PS;
    rData.dFTL_PS  = InData.dFTL_PS;
    rData.dFBL_PS  = InData.dFBL_PS;
    rData.dFTR_PS  = InData.dFTR_PS;
    rData.dFBR_PS  = InData.dFBR_PS;
    rData.dFMAX_PS = InData.dFMAX_PS;

    rData.dALW   = InData.dALW;
    rData.dfci   = InData.dfci;
    rData.dRatio = InData.dRatio;
    rData.bOK    = InData.bOK;
    rData.dTtfc     = InData.dTtfc;
    rData.dAct      = InData.dAct;
    rData.dTtfs     = InData.dTtfs;
    rData.dAst      = InData.dAst;
    rData.dFs       = InData.dFs;
    rData.dfci      = InData.dfci;


    return TRUE;
}
BOOL CDgn_PSCManager::ConvertFysg_EngineToCVL2(const _FYSG_PSC_LRFD_BASE& InData, T_FYSG_AASHTO_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StagK  = InData.StagK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;

    rData.bOK    = InData.bOK;
    rData.dTtfc     = InData.dTtfc;
    rData.dAct      = InData.dAct;
    rData.dTtfs     = InData.dTtfs;
    rData.dAst      = InData.dAst;
    rData.dFs       = InData.dFs;


    return TRUE;
}


BOOL CDgn_PSCManager::ConvertFypc_EngineToCVL(const _FYPC_AASHTO& InData, T_FYPC_AASHTO& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.dFDL1  = InData.dFDL1;
    rData.dFDL2  = InData.dFDL2;
    rData.dFDL3  = InData.dFDL3;
    rData.dFLL1  = InData.dFLL1;
    rData.dFLL2  = InData.dFLL2;
    rData.dAFDL1 = InData.dAFDL1;
    rData.dAFDL2 = InData.dAFDL2;
    rData.dAFDL3 = InData.dAFDL3;
    rData.dAFLL1 = InData.dAFLL1;
    rData.dAFLL2 = InData.dAFLL2;
    rData.bOK    = InData.bOK;
    rData.nLoadType = InData.nLoadType;
    rData.nElemFDL1  = InData.nElemFDL1;
    rData.nElemFDL2  = InData.nElemFDL2;
    rData.nElemFLL1  = InData.nElemFLL1;
    rData.nPartFDL1  = InData.nPartFDL1;
    rData.nPartFDL2  = InData.nPartFDL2;
    rData.nPartFLL1  = InData.nPartFLL1;
    rData.nStage    = InData.nStage;
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_EngineToCVL(const _FYCM_PSC_LRFD_BASE& InData, T_FYCM_AASHTO_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.iMax  = InData.iMax;
    rData.iKind = InData.iKind;
    rData.dFT   = InData.dFT;
    rData.dFB   = InData.dFB;
    rData.dFTL  = InData.dFTL;
    rData.dFBL  = InData.dFBL;
    rData.dFTR  = InData.dFTR;
    rData.dFBR  = InData.dFBR;
    rData.dFMAX = InData.dFMAX;
    rData.dALW  = InData.dALW;
    rData.bOK   = InData.bOK;

    rData.dFT_PS   = InData.dFT_PS;
    rData.dFB_PS   = InData.dFB_PS;
    rData.dFTL_PS  = InData.dFTL_PS;
    rData.dFBL_PS  = InData.dFBL_PS;
    rData.dFTR_PS  = InData.dFTR_PS;
    rData.dFBR_PS  = InData.dFBR_PS;
    rData.dFMAX_PS = InData.dFMAX_PS;
    rData.dRatio= InData.dRatio;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFssg_EngineToCVL(const _FPMT_PSC_LRFD_BASE& InData, T_FPMT_JTG_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.iMax  = InData.iMax;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;
    rData.dfci    = InData.dfci;
    rData.dRatio  = InData.dRatio;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_EngineToCVL(const _FPMT_PSC_LRFD_BASE& InData, T_FPMS_JTG_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.iMax  = InData.iMax;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCVL(const _BDCW_PSC_LRFD_BASE& InData, T_BDCW_AASHTO_BASE& rData)
{
    rData.bCHK   = InData.LInf.bCHK;
    rData.LcomK  = InData.LInf.LcomK;
    rData.iKind  = InData.LInf.nKind;
    rData.iMax   = InData.LInf.nMax;
    rData.dFT    = InData.ForB.dFT;
    rData.dFB    = InData.ForB.dFB;
    rData.dFSS   = InData.dFSS;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.bOK    = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertTbar_EngineToCVL(const _TBAR_PSC_LRFD_BASE& InData, T_TBAR_AASHTO_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.iMax  = InData.iMax;
    rData.iKind = InData.iKind;
    rData.dftop = InData.dftop;
    rData.dfbot = InData.dfbot;
    rData.dREQ  = InData.dREQ;
    rData.dMIN  = InData.dMIN;
    rData.dUSE  = InData.dUSE;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcy_EngineToCVL(const _BDCY_PSC_LRFD_BASE& InData, T_BDCY_AASHTO_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iKind   = InData.iKind;
    rData.iMax    = InData.iMax;
    rData.dMuy    = InData.dMuy;
    rData.drMuy   = InData.drMuy;
    rData.dAps    = InData.dAps;
    rData.ddp     = InData.ddp;
    rData.dAs     = InData.dAs;
    rData.dds     = InData.dds;
    rData.db      = InData.db;
    rData.dc      = InData.dc;
    rData.dfps    = InData.dfps;
    rData.da      = InData.da;
    rData.dfr     = InData.dfr;
    rData.dfcpe   = InData.dfcpe;
    rData.dMdnc   = InData.dMdnc;
    rData.dSc     = InData.dSc;
    rData.dSnc    = InData.dSnc;
    rData.dMny    = InData.dMny;
    rData.dMcry   = InData.dMcry;
    rData.dFacM   = InData.dFacM;
    rData.dFacMcr = InData.dFacMcr;
    rData.dpMn_12Mcr_Rat  = InData.dpMn_12Mcr_Rat;
    rData.dpMn_133Mcr_Rat = InData.dpMn_133Mcr_Rat;
    rData.bOK             = InData.bOK;

    rData.dAp_unbonded  = InData.dAp_unbonded;
    rData.dAp_bonded    = InData.dAp_bonded;
    rData.ddp_unbonded  = InData.ddp_unbonded;
    rData.ddp_bonded    = InData.ddp_bonded;
    rData.dAsc          = InData.dAsc;
    rData.ddsc          = InData.ddsc;
    rData.dbw           = InData.dbw;
    rData.dhf           = InData.dhf;
    rData.dBeta1        = InData.dBeta1;
    rData.dK            = InData.dK;
    rData.dfpe          = InData.dfpe;
    rData.dle           = InData.dle;
    rData.dPhi          = InData.dPhi;

    rData.dfc           = InData.dfc;
    rData.dfci          = InData.dfci;
    rData.dfy           = InData.dfy;
    rData.dfyc          = InData.dfyc;
    rData.dfpu_bonded   = InData.dfpu_bonded;
    rData.dfpu_unbonded = InData.dfpu_unbonded;
    rData.dfpy_bonded   = InData.dfpy_bonded;
    rData.dfpy_unbonded = InData.dfpy_unbonded;

    // CSA
    rData.dMf_Mr_Rat    = InData.dpMn_133Mcr_Rat;
    rData.dMr_12Mcr_Rat =InData.dpMn_12Mcr_Rat;
    rData.dc_d_Rat      = InData.dc_d_rat;
    rData.bNeuTendonRat = InData.bNeuTendonRat;
    rData.dNeuTendonRat = InData.dNeuTendonRat;
    return TRUE;
}


#define DGN_CONV_PRINTUNIT(nForce, nLength, UNIT, value)\
				pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(nForce, nLength, D_UNITSYS_BASE_##UNIT, value)
BOOL CDgn_PSCManager::ConvertBdcy_Unit(int nBase_Force, int nBase_Length, _BDCY_PSC_LRFD_BASE& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    ConvertCurUnit_TndnCalcD(nBase_Force, nBase_Length, InData.TndnCalcD);
    ConvertCurUnit_CalcTndnInfo(nBase_Force, nBase_Length, InData.arCalcTndnInfo);
    ConvertCurUnit_NeutAxisInfo(nBase_Force, nBase_Length, InData.arNeutAxisInfo);

    for ( int i=0; i<3; ++i )
    {
        InData.dForceTP[i] = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dForceTP[i]);
        int nM=i+3;
        InData.dForceTP[nM] = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dForceTP[nM]);
    }

    InData.dMuy           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMuy); // [F]/[L^2]
    InData.dAps           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAps); // [Unit=Area]
    InData.ddp            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddp); // [Unit =Length]
    InData.dAs            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAs); // [Unit=Area]
    InData.dds            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dds); // [Unit=Length]
    InData.db             = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.db); // [Unit=Length]
    InData.dc             = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dc); // [Unit=Length]
    InData.dCc            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dCc); // 압축 콘크리트 Force
    InData.dCp            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dCp); // 압축 Tendon Force
    InData.dCs            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dCs); // 압축 Rebar Force
    InData.dTp            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dTp); // 인장 Tendon Force
    InData.dTs            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dTs); // 인장 Rebar Force
    InData.dMn_cc         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMn_cc); // 내력모멘트-압축콘크리트
    InData.dMn_sc         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMn_sc); // 내력모멘트-압축철근
    InData.dMn_st         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMn_st); // 내력모멘트-인장철근
    InData.dMn_ps         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMn_ps); // 내력모멘트-tendon
    InData.ddr_max        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddr_max); // 인장측 최외단 철근or텐던 위치 
    InData.dTp_bond       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dTp_bond); // 인장 Tendon Force_bond
    InData.dTp_unbond     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dTp_unbond); // 인장 Tendon Force_unbond
    InData.dfps_bond      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfps_bond);
    InData.dfps_unbond    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfps_unbond); // [Unit = stress]
    InData.da             = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.da); // [Unit=Length]
    InData.da_Area        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.da_Area); // 등가응력 깊이 a에 대한 압축콘크리트 단면적  
    InData.da_Cen         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.da_Cen); // 등가응력 깊이 a에서 압축콘크리트 도심까지 거리  
    InData.dfr            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfr);	// [Unit=Stress]
    InData.dfr_sl         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfr_sl);	// [Unit=Stress]
    InData.dEc            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dEc);    // [Unit=Stress]
    InData.dEc_sl         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dEc_sl);    // [Unit=Stress]    
    InData.dfcpe          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfcpe);	// [Unit=Stress]
    InData.dMdnc          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMdnc); //[Unit=Moment]
    InData.dMdncApp       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMdncApp); //[Unit=Moment]
    InData.dSc            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, VOLUME, InData.dSc); //[Unit=Section Modulus] //Length^3
    InData.dScGrTop       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, VOLUME, InData.dScGrTop); //[Unit=Section Modulus]	//Length^3
    InData.dSnc           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, VOLUME, InData.dSnc); //[Unit=Section Modulus]	//Length^3
    InData.drMuy          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.drMuy);	// Factored moment of resistance [Unit=Moment]
    InData.dMny           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMny);	// Nominal moment of resistance [Unit=Moment]
    InData.dMcry          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMcry);	// Cracking moment [Unit=Moment]
    InData.dFacM          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dFacM);	// [Unit=Moment]
    InData.dFacMcr        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dFacMcr);	// [Unit=Moment]
    InData.dMcr_g         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMcr_g);	// [Unit=Moment]
    InData.dMcr_s         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMcr_s);	// [Unit=Moment]
    InData.dMcrCal        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMcrCal);	// [Unit=Moment]
    InData.dAp_unbonded   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAp_unbonded); // [Unit=Area]
    InData.dAp_bonded     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAp_bonded); // [Unit=Area]
    InData.ddp_unbonded   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddp_unbonded); // [Unit=Length]
    InData.ddp_bonded     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddp_bonded); // [Unit=Length]
    InData.dAsc           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAsc); // [Unit=Area]
    InData.ddsc           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddsc); // [Unit=Length]
    InData.dbw            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dbw); // [Unit=Length]
    InData.dhf            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dhf); // [Unit=Length]
    InData.dfpe           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpe); // [Unit=Stress]
    InData.dle            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dle); // [Unit=Length]
    InData.dfc            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfc); // [Unit=Stress]
    InData.dfci           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfci); // [Unit=Stress]
    InData.dfy            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfy); // [Unit=Stress]
    InData.dfyc           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfyc); // [Unit=Stress]
    InData.dfpu_bonded    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dfpu_bonded); // [Unit=Stress]
    InData.dfpu_unbonded  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpu_unbonded); // [Unit=Stress]
    InData.dfpy_bonded    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpy_bonded); // [Unit=Stress]
    InData.dfpy_unbonded  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpy_unbonded); // [Unit=Stress]
    InData.ddc            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddc);
    InData.dde            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dde); // dde, 휨강도에 대한 Rating 계산서 출력용, 
    InData.ddv            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddv); // ddv, 휨강도에 대한 Rating 계산서 출력용
    InData.dNeu_comp      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dNeu_comp); // 압축연단~중립축 거리
    InData.dComp_ten      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dComp_ten); // 압축연단~인장 중심거리
    InData.dB_flange      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dB_flange); // 플랜지 폭

    return TRUE;
}


BOOL CDgn_PSCManager::ConvertCurUnit_TndnCalcD(int nBase_Force, int nBase_Length, PSC_TNDN_CALD& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    InData.dfpy_bond   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpy_bond); // min
    InData.dfpu_bond   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpu_bond); // min
    InData.dAps_bond   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAps_bond);
    InData.dPe_bond    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dPe_bond);
    InData.dfpe_bond   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpe_bond);
    InData.dfpy_unbond = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpy_unbond);
    InData.dfpu_unbond = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpu_unbond);
    InData.dAps_unbond = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAps_unbond);
    InData.dPe_unbond  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dPe_unbond);
    InData.dfpe_unbond = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpe_unbond);
    InData.dPe_total   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dPe_total); // 전체   
    InData.dPe_sum     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dPe_sum); // 인장부분. 
    InData.dAps_sum    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAps_sum);
    InData.dMe_sum     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMe_sum);
    InData.dep_sum     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dep_sum);

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCurUnit_CalcTndnInfo(int nBase_Force, int nBase_Length, CArray<PSC_FRCR_TNCD, PSC_FRCR_TNCD&>& CalcTndnInfo)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    int nSize = CalcTndnInfo.GetSize();
    for ( int i=0; i<nSize; ++i )
    {
        PSC_FRCR_TNCD InData = CalcTndnInfo.GetAt(i);

        InData.dEpi        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dEpi);
        InData.dApsi       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dApsi);
        InData.ddpi        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddpi);
        InData.depi_neu    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.depi_neu);  // 중립축에서 떨어진 거리 
        InData.dfpsi       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpsi);
        InData.dPei        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dPei);
        InData.dMpsi       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMpsi);
        InData.dfps_Apsi   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dfps_Apsi);  // Tp = fps*Aps
        InData.dTp_epi     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dTp_epi);  // Tp*ep
        InData.dPe_epi     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dPe_epi);  // Pe*ep
        InData.dle         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dle);  // 2.0*dLi/(2.0+dNs)
		InData.dLpt        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dLpt);
		InData.dDevLength  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dDevLength);

        CalcTndnInfo.SetAt(i, InData);
    }


    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCurUnit_NeutAxisInfo(int nBase_Force, int nBase_Length, CArray<PSC_FRCR_NEUT_AXIS_D, PSC_FRCR_NEUT_AXIS_D&>& NeutAxisInfo)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    int nSize = NeutAxisInfo.GetSize();
    for ( int i=0; i<nSize; ++i )
    {
        PSC_FRCR_NEUT_AXIS_D InData = NeutAxisInfo.GetAt(i);

        InData.dx       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dx);
        InData.dCompF_c = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dCompF_c); // concrete force
        InData.dCompF_s = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dCompF_s); // compression Rbar
        InData.dTensF_p = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dTensF_p); // Tension tendon
        InData.dTensF_s = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dTensF_s); // Tension Rbar

        NeutAxisInfo.SetAt(i, InData);
    }


    return TRUE;
}



// #define DGN_CUMS_CONV_SIUNIT(UNIT, value)\
// pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_##UNIT, value)

BOOL CDgn_PSCManager::ConvertCums_Unit(int nBase_Force, int nBase_Length, _CUMS_AASHTO_BASE& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    for ( int i=0; i<3; ++i )
    {
        InData.dForceTP[i] = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dForceTP[i]); // Tendon primary force
        int nM=i+3;
        InData.dForceTP[nM] = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dForceTP[nM]);
    }

    InData.dNu           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dNu); //[Unit=Force]
    InData.dVu           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVu); //[Unit=Force]
    InData.dMu           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMu); //[Unit=Moment]
    InData.dVn           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVn); //[Unit=Force]
    InData.dVc           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVc); //[Unit=Force]
    InData.dVs           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVs); //[Unit=Force]	
    InData.dVp           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVp); //[Unit=Force]
    InData.dPhiVn        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dPhiVn); //[Unit=Force]
    InData.dde           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dde); //[Unit=Length]
    InData.ddv           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddv); //[Unit=Length]
    InData.dVc_FactorVp  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVc_FactorVp); //[Unit=Force]

    InData.ddpt          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddpt); //[Unit=Length]
    InData.ddv_Org       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddv_Org);	//[Unit=Length]
    InData.ddv_Cal       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddv_Cal); //[Unit=Length]
    InData.ddst          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddst); //[Unit=Length]
    InData.ddsv          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddsv); //[Unit=Length]
    InData.ddt           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddt); //[Unit=Length]
    InData.dAv_s         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAv_s); //[Unit=Length]
    InData.dA_st         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dA_st); //[Unit=Area]
    InData.dAl           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAl); //[Unit=Area]
    InData.dbv           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dbv); //[Unit=Length]
    InData.ds            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ds); //[Unit=Length]
    InData.dsmax         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dsmax);	//[Unit=Length] 
    InData.dAv_s_min     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAv_s_min);	//[Unit=LENGTH]
    InData.dAv_s_req     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAv_s_req);	//[Unit=LENGTH]
    InData.dAl_min       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAl_min); //[Unit=Area]
    InData.dbv_min       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dbv_min); //[Unit=Length]
    InData.dfc           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfc); // [Unit=Stress]
    InData.dfy           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfy); // [Unit=Stress]
    InData.dfpu          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpu); // [Unit=Stress]
    InData.dfpy          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpy); // [Unit=Stress]
    InData.dvu           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dvu); // [Unit=Stress]
    InData.dfct          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfct); // [Unit=Stress]
    InData.dfpo          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpo); // [Unit=Stress]
    InData.dfpc          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpc); // [Unit=Stress] 
    InData.dMu_lim       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMu_lim); //[Unit=Moment] // fabs(dVu-dVp)*ddv
    InData.ds_xe         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ds_xe); //[Unit=LENGTH]
    InData.dVn1          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVn1); //[Unit=Force]
    InData.dVn2          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVn2); //[Unit=Force] 
    InData.dh            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dh); //[Unit=Length]
    InData.dfcr_cal      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfcr_cal); // [Unit=Stress]
    InData.dfcr_lim      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfcr_lim); // [Unit=Stress]
    InData.dfcr          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfcr); // [Unit=Stress]
    InData.dSz           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dSz); //[Unit=LENGTH]
    InData.dSze_cal      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dSze_cal); //[Unit=LENGTH]
    InData.dSze_lim      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dSze_lim); //[Unit=LENGTH]
    InData.dSze          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dSze); //[Unit=LENGTH]
    InData.dV_rebar_lim  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dV_rebar_lim); //[Unit=Force]
    InData.dVr_lim       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVr_lim); //[Unit=Force]
    InData.dFlt          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dFlt); //[Unit=Force]
    InData.dFlc          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dFlc); //[Unit=Force]
    InData.dFt           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dFt); //[Unit=Force]
    InData.dFc           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dFc); //[Unit=Force] 
    InData.dVf_lim_dist  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVf_lim_dist); //[Unit=Force]
    InData.da_g          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.da_g); //[Unit=LENGTH]
    InData.d0125fck      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.d0125fck); //[Unit=Stress]
    InData.d019SqrtFck   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.d019SqrtFck); //[Unit=Stress]

    InData.dBvi             = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dBvi); //[Unit=LENGTH]   //Interface shear
    InData.dAvf             = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dAvf); //[Unit=LENGTH]   
    InData.dfy_PscsD        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfy_PscsD); //[Unit=Stress] 
    InData.dAcv             = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAcv); //[Unit=Area]   
    InData.dFactor_c        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFactor_c); //[Unit=Stress]
    InData.dFactor_K2       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFactor_K2); //[Unit=Stress]
    InData.dVni_PscsD_Temp1 = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVni_PscsD_Temp1); //[Unit=Force]
    InData.dVni_PscsD_Temp2 = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVni_PscsD_Temp2); //[Unit=Force]
    InData.dVni_PscsD_Temp3 = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVni_PscsD_Temp3); //[Unit=Force]
    InData.dPc_PscsD        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dPc_PscsD); //[Unit=Force]
    InData.dVui_PscsD       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVui_PscsD); //[Unit=Force]
    InData.dVni_PscsD       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVni_PscsD); //[Unit=Force]
    InData.dfck_PscsD       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dfck_PscsD); //[Unit=Force]
    InData.ddv_PscsD        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddv_PscsD); //[Unit=Length]
    InData.dVeff            = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVeff); //[Unit=Length]

	InData.dVui_PscsD_S     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dVui_PscsD_S); 
	InData.dVni_PscsD_S     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dVni_PscsD_S); 

	InData.dVni_PscsD_Rat1     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dVni_PscsD_Rat1);
	InData.dVni_PscsD_Rat2     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dVni_PscsD_Rat2);
	InData.dVni_Conc_PscsD     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dVni_Conc_PscsD);
	InData.dVni_Fric_PscsD     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dVni_Fric_PscsD);
	InData.dVni_Conc_Rat_PscsD     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dVni_Conc_Rat_PscsD);
	InData.dVni_Fric_Rat_PscsD     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dVni_Fric_Rat_PscsD);

    InData.dAct = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAct); //[Unit=AREA]

    return TRUE;
}

// #define DGN_CRMS_CONV_SIUNIT(UNIT, value)\
// pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_##UNIT, value)

BOOL CDgn_PSCManager::ConvertCrms_Unit(int nBase_Force, int nBase_Length, _CRMS_AASHTO_BASE& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    for ( int i=0; i<3; ++i )
    {
        InData.dForceTP[i] = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dForceTP[i]); // Tendon primary force
        int nM=i+3;
        InData.dForceTP[nM] = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dForceTP[nM]);
    }

    InData.dNu          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dNu); //[Unit=Force]
    InData.dVu          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVu); //[Unit=Force]
    InData.dMu          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMu); //[Unit=Moment]
    InData.dTu          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dTu); //[Unit=Moment]
    InData.dVn          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVn); //[Unit=Force]
    InData.dTn          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dTn); //[Unit=Moment]
    InData.dVc          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVc); //[Unit=Force]
    InData.dVs          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVs); //[Unit=Force]	
    InData.dVp          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVp); //[Unit=Force]
    InData.dPhiVn       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dPhiVn); //[Unit=Force]
    InData.dPhi_tTn     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dPhi_tTn); //[Unit=Moment]
    InData.dde          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dde); //[Unit=Length]
    InData.ddv          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddv); //[Unit=Length]
    InData.dAv_s        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAv_s); //[Unit=Length]
    InData.dbv          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dbv); //[Unit=Length]
    InData.dA_st        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dA_st); //[Unit=Area]
    InData.dAl          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAl); //[Unit=Area]
    InData.ds           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ds); //[Unit=Length]
    InData.dsmax        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dsmax); //[Unit=Length]
    InData.dAv_s_min    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAv_s_min); //[Unit=Length]
    InData.dAv_s_req    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAv_s_req); //[Unit=Length]
    InData.dAl_min      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAl_min); //[Unit=Area]
    InData.dbv_min      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dbv_min); //[Unit=Length]
    InData.dAt          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAt); //[Unit=Area]
    InData.dAt_req      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAt_req); //[Unit=Area] 
    InData.dfc          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfc); // [Unit=Stress]
    InData.dfy          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfy); // [Unit=Stress]
    InData.dfpu         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpu); // [Unit=Stress]
    InData.dfpy         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpy); // [Unit=Stress] 
    InData.dvu          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dvu); // [Unit=Stress]
    InData.dfct         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfct); // [Unit=Stress]
    InData.dfpo         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpo); // [Unit=Stress]
    InData.dfpc         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfpc); // [Unit=Stress] 
    InData.dVn1         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVn1); //[Unit=Force]
    InData.dVn2         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVn2); //[Unit=Force] 
    InData.dA_long      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dA_long); //[Unit=Force]  ΣAps·fps+ΣAs·fy
    InData.dA_long_lim  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dA_long_lim); //[Unit=Force]
    InData.dh           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dh); //[Unit=Length]
    InData.dTcr         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dTcr); //[Unit=Moment]
    InData.dCombRat     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dCombRat); // [Unit=Stress]
    InData.dCombRat_Lim = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dCombRat_Lim); // [Unit=Stress]
    InData.dfcr_cal     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfcr_cal); // [Unit=Stress]         
    InData.dfcr_lim     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfcr_lim); // [Unit=Stress]         
    InData.dfcr         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfcr); // [Unit=Stress]             
    InData.dSz          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dSz); //[Unit=LENGTH]              
    InData.dSze_cal     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dSze_cal); //[Unit=LENGTH]       
    InData.dSze_lim     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dSze_lim); //[Unit=LENGTH]       
    InData.dSze         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dSze); //[Unit=LENGTH]       
    InData.dV_rebar_lim = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dV_rebar_lim); //[Unit=Force]     
    InData.dVr_lim      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVr_lim); //[Unit=Force]          
    InData.dFlt         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dFlt); //[Unit=Force]                       
    InData.dFlc         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dFlc); //[Unit=Force]                       
    InData.dFt          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dFt); //[Unit=Force]               
    InData.dFc          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dFc); //[Unit=Force]               
    InData.dVf_lim_dist = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVf_lim_dist); //[Unit=Force]                    
    InData.da_g         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.da_g); //[Unit=LENGTH]  
    InData.dAo          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAo); //[Unit=Area]           
    InData.dt_box       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dt_box); // [Unit=Length]  
    InData.dMu_lim_T    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMu_lim_T); //[Unit=Moment]
    InData.d0125fck     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.d0125fck); //[Unit=Stress]
    InData.dVeff        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVeff); //[Unit=Force]
    InData.dAoh_be      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, VOLUME, InData.dAoh_be);
    InData.dAoh_bv      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, VOLUME, InData.dAoh_bv);

    return TRUE;
}

// #define DGN_BDCW_CONV_SIUNIT(UNIT, value)\
// pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_##UNIT, value)

BOOL CDgn_PSCManager::ConvertBdcw_Unit(int nBase_Force, int nBase_Length, _BDCW_PSC_LRFD_BASE& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    InData.ForB.dFT    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.ForB.dFT);		//[Unit=Stress]
    InData.ForB.dFB    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.ForB.dFB);		//[Unit=Stress]
    InData.dFSS   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFSS);   //[Unit=Stress]	
    InData.ds_use = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ds_use);   //[Unit=Length]
    InData.ds_max = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ds_max);   //[Unit=Length]
    InData.ddc    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddc);   // 인장연단에서 철근도심까지 거리 

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_Unit(int nBase_Force, int nBase_Length, _FYSG_PSC_LRFD_BASE& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    InData.dFT   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFT);
    InData.dFB   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFB);
    InData.dFTL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTL);
    InData.dFBL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBL);
    InData.dFTR  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTR);
    InData.dFBR  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBR);
    InData.dFMAX = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFMAX);
    //
    InData.dFT_PS   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFT_PS);
    InData.dFB_PS   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFB_PS);
    InData.dFTL_PS  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTL_PS);
    InData.dFBL_PS  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBL_PS);
    InData.dFTR_PS  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTR_PS);
    InData.dFBR_PS  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBR_PS);
    InData.dFMAX_PS = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFMAX_PS);
    //
    InData.dFT_SL   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFT_SL);
    InData.dFB_SL   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFB_SL);
    InData.dFTL_SL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTL_SL);
    InData.dFBL_SL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBL_SL);
    InData.dFTR_SL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTR_SL);
    InData.dFBR_SL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBR_SL);
    InData.dFMAX_SL = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFMAX_SL);
    //
    InData.dfci    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfci);
    InData.dALW    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dALW);
    InData.dfcri   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfcri);
    InData.dfcri_s = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfcri_s);
    InData.dTtfc   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dTtfc);
    InData.dAct    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAct);
    InData.dTtfs   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dTtfs);
    InData.dAst    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAst);
    InData.dFs     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFs);

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_Unit(int nBase_Force, int nBase_Length, _FYCM_PSC_LRFD_BASE& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    InData.dFT   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFT);
    InData.dFB   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFB);
    InData.dFTL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTL);
    InData.dFBL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBL);
    InData.dFTR  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTR);
    InData.dFBR  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBR);
    InData.dFMAX = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFMAX);
    //
    InData.dFT_PS   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFT_PS);
    InData.dFB_PS   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFB_PS);
    InData.dFTL_PS  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTL_PS);
    InData.dFBL_PS  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBL_PS);
    InData.dFTR_PS  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTR_PS);
    InData.dFBR_PS  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBR_PS);
    InData.dFMAX_PS = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFMAX_PS);
    //
    InData.dFT_SL   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFT_SL);
    InData.dFB_SL   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFB_SL);
    InData.dFTL_SL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTL_SL);
    InData.dFBL_SL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBL_SL);
    InData.dFTR_SL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFTR_SL);
    InData.dFBR_SL  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFBR_SL);
    InData.dFMAX_SL = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dFMAX_SL);

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertPSCIndata_Unit(int nBase_Force, int nBase_Length, PSC_DATA_POSI& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    //   S_PSCD_D PscdD      : 단위변환이 필요한 변수 없음
    //   PSC_MATL_ELEM MatlD : AASHTO변수만 변환
    //   PSC_SECT_POSI SectD ; AASHTO변수만 변환
    //   DGN_SPSC_SECT SectgD; // Gross Section property
    //   DGN_RBAR_POSI RbarD ;
    //   PSC_RSHR_POSI RshrD ;
    //   PSC_TNDN_POSI TndnD ; // Tendon Data

    ConvertPSCIndata_MatlD_Unit(nBase_Force, nBase_Length, InData.MatlD);
    ConvertPSCIndata_SectD_Unit(nBase_Force, nBase_Length, InData.SectD);
    ConvertPSCIndata_SectgD_Unit(nBase_Force, nBase_Length, InData.SectgD);
    ConvertPSCIndata_SectD_Unit(nBase_Force, nBase_Length, InData.SectPreD);
    ConvertPSCIndata_SectgD_Unit(nBase_Force, nBase_Length, InData.SectgPreD);
    ConvertPSCIndata_RbarD_Unit(nBase_Force, nBase_Length, InData.RbarD);
    ConvertPSCIndata_RshrD_Unit(nBase_Force, nBase_Length, InData.RshrD);
    ConvertPSCIndata_TndnD_Unit(nBase_Force, nBase_Length, InData.TndnD);

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertPSCIndata_MatlD_Unit(int nBase_Force, int nBase_Length, PSC_MATL_ELEM& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    InData.C_fc         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.C_fc);   // MatdD.Data1.Design.C_fc
    InData.C_fci        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.C_fci);   // MatdD.Data1.Design.C_fci
    InData.Elast        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.Elast);   // MatdD.Data1.Analysis.Elast
    InData.B_fy         = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.B_fy);   // MatdD.MainRebarData.B_fy= DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddc   );
    InData.B_Elast      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.B_Elast);   // MatdD.MainRebarData.B_Elast= DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddc   );
    InData.fy_sub       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.fy_sub);
    InData.Elast_sub    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.Elast_sub);
    //Composite MatID
    InData.C_fc_s       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.C_fc_s);
    InData.C_fci_s      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.C_fci_s);
    InData.Elast_s      = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.Elast_s);   // MatdD.Data1.Analysis.Elast
    InData.B_fy_s       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.B_fy_s);   // MatdD.MainRebarData.B_fy= DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddc   );
    InData.B_Elast_s    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.B_Elast_s);   // MatdD.MainRebarData.B_Elast= DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.ddc   );
    InData.fy_sub_s     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.fy_sub_s);
    InData.Elast_sub_s  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.Elast_sub_s);
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertPSCIndata_SectD_Unit(int nBase_Force, int nBase_Length, PSC_SECT_POSI& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    //DGN_SPSC_SECT SpscSect;
    ConvertPSCIndata_SectgD_Unit(nBase_Force, nBase_Length, InData.SpscSect);

    InData.dAg           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAg);
    InData.dAoh          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAoh);  // 비틀림 저항철근의 중심선으로 폐쇄된 면적
    InData.dPh           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dPh);  // 외곽부 폐쇄 횡방향 비틀림 철근의 중심선의 둘레길이.
    InData.dPcp          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dPcp);  // 전단면의 둘레의 길이
    InData.dAcp          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAcp);  // 콘크리트 단면의 바깥 둘레로 둘러싸인 단면적으로, 속빈 단면에서는 속빈 면적을 포함. 
    InData.dKt           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dKt);
    InData.dWt           = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dWt);  // Kt(JSEC02), Wt(JTG D62-04).

    for ( int i=0; i<4; ++i )
    {
        for ( int j=0; j<2; ++j )
        {
            InData.dpMomPos[i][j]= DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dpMomPos[i][j]);  // Position for Stress (0~4), (0=y,1=z).
        }
    }
    for ( int i=0; i<3; ++i )
    {
        InData.dpShrPos[i]   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dpShrPos[i]);
        InData.dpShrThk[i]   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dpShrThk[i]);  // Web (0=Top, 1=Center, 2=Bot). 전단검토 위치 및 두께 
    }

    for ( int i=0; i<2; ++i )
    {
        InData.dpFlgThk[i]   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dpFlgThk[i]);  // Flange (0=Top, 1=Bot).
    }

    InData.dTorThk= DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dTorThk);
    InData.dbw    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dbw);
    InData.dBc    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dBc);
    InData.dtc    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dtc);
    InData.dHh    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dHh);

    InData.dPcp_c = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dPcp_c);

    return TRUE;
}


BOOL CDgn_PSCManager::ConvertPSCIndata_SectgD_Unit(int nBase_Force, int nBase_Length, DGN_SPSC_SECT& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    InData.dyDim = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dyDim);
    InData.dzDim = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dzDim);	// 단면고
    InData.dyCen = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dyCen);
    InData.dzCen = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dzCen);	// 중립축
    InData.dArea = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dArea);  // 단면적
    InData.dAsy  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAsy);
    InData.dAsz  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.dAsz);  // 전단면적
    InData.dIxx  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STIF, InData.dIxx);  // 비틀림강성
    InData.dIyy  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STIF, InData.dIyy);
    InData.dIzz  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STIF, InData.dIzz);  // 단면2차모멘트
    InData.dSlt  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, VOLUME, InData.dSlt);
    InData.dSlb  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, VOLUME, InData.dSlb);  // 강축좌측단면계수(Top,Bot)
    InData.dSrt  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, VOLUME, InData.dSrt);
    InData.dSrb  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, VOLUME, InData.dSrb);  // 강축우측단면계수(Top,Bot)

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertPSCIndata_RbarD_Unit(int nBase_Force, int nBase_Length, DGN_RBAR_POSI& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    int nSize = InData.arRbarPosi.GetSize();
    for ( int i=0; i<nSize; ++i )
    {
        DGN_RBAR_CRC RbarPosi = InData.arRbarPosi.GetAt(i);

        RbarPosi.dArea = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, RbarPosi.dArea);
        RbarPosi.dDia  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, RbarPosi.dDia);
        RbarPosi.dy    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, RbarPosi.dy);
        RbarPosi.dz    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, RbarPosi.dz);

        InData.arRbarPosi.SetAt(i, RbarPosi);
    }

    for ( int i=0; i<2; ++i )
    {
        InData.dsmax[i] = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.dsmax[i]);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertPSCIndata_RshrD_Unit(int nBase_Force, int nBase_Length, PSC_RSHR_POSI& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    InData.dfdt = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfdt);
    InData.dfdb = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, InData.dfdb);
    InData.dVd  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.dVd);
    InData.dMd  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, MOMENT, InData.dMd);

    // DGN_RPSC_SBAR RpscSbar; 
    InData.RpscSbar.dSbarPitch = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.RpscSbar.dSbarPitch);  // 경사인장철근의 간격
    InData.RpscSbar.dSbarAw    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.RpscSbar.dSbarAw);  // 경사인장철근의 배치량
    InData.RpscSbar.dTndnDist  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.RpscSbar.dTndnDist);  // 전단강봉의 간격
    InData.RpscSbar.dTndnAp    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.RpscSbar.dTndnAp);  // 전단강봉의 배치량
    InData.RpscSbar.dTndnPs    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, FORCE, InData.RpscSbar.dTndnPs);  // 전단강봉의 유효인장력
    InData.RpscSbar.dLbarPitch = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.RpscSbar.dLbarPitch);  // 횡방향철근의 간격
    InData.RpscSbar.dLbarAwt   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.RpscSbar.dLbarAwt);  // 횡방향철근의 Pitch당 철근 1개의 단면적
    InData.RpscSbar.dLbarAlt   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.RpscSbar.dLbarAlt);  // 횡뱡향철근의 전단면적
    InData.RpscSbar.dStirPitch = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.RpscSbar.dStirPitch);  // 6.6.0 추가: 전단철근의 간격
    InData.RpscSbar.dStirAw    = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, InData.RpscSbar.dStirAw);  // 6.6.0 추가: 전단철근의 배치량
    InData.RpscSbar.dLbarThick = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, InData.RpscSbar.dLbarThick);  // 6.9.2 추가: 비틀림 스트럽에서 연단까지의 거리(피복두께)

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertPSCIndata_TndnD_Unit(int nBase_Force, int nBase_Length, PSC_TNDN_POSI& InData)
{
    CDBDoc *pDoc = CDBDoc::GetDocPoint();
    if ( pDoc==NULL ) { ASSERT(0); return FALSE; }

    CArray<DGN_TNDN_CRC, DGN_TNDN_CRC&> arTndnPosi;

    int nSize = InData.arTndnPosi.GetSize();
    for ( int i=0; i<nSize; ++i )
    {
        DGN_TNDN_CRC TndnPosi = InData.arTndnPosi.GetAt(i);

        TndnPosi.dEps        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, TndnPosi.dEps);
        TndnPosi.dfpu        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, TndnPosi.dfpu);
        TndnPosi.dfpy        = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, TndnPosi.dfpy);
        TndnPosi.dArea       = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, AREA, TndnPosi.dArea);
        TndnPosi.dy          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, TndnPosi.dy);
        TndnPosi.dz          = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, TndnPosi.dz); // 콘크리트 단면에 대한 도심(0=y,1=z)
        TndnPosi.dStressLL   = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, TndnPosi.dStressLL);
        TndnPosi.dLength     = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, TndnPosi.dLength);
        TndnPosi.dStress[0]  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, TndnPosi.dStress[0]);		// 프리스트레스 분력(0=x,1=y,2=z)
        TndnPosi.dStress[1]  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, TndnPosi.dStress[1]);		// 프리스트레스 분력(0=x,1=y,2=z)
        TndnPosi.dStress[2]  = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, STRESS, TndnPosi.dStress[2]);		// 프리스트레스 분력(0=x,1=y,2=z)
		TndnPosi.dLengthElem = DGN_CONV_PRINTUNIT(nBase_Force, nBase_Length, LENGTH, TndnPosi.dLengthElem);

        InData.arTndnPosi.SetAt(i, TndnPosi);
    }

    return TRUE;
}
#undef DGN_CONV_PRINTUNIT


BOOL CDgn_PSCManager::ConvertCums_EngineToCVL(const _CUMS_AASHTO_BASE& InData, T_CUMS_AASHTO_BASE& rData)
{
    rData.bCHK      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iKind     = InData.iKind;
    rData.iMax      = InData.iMax;
    rData.dNu       = InData.dNu;
    rData.dVu       = InData.dVu;
    rData.dMu       = InData.dMu;
    rData.dVn       = InData.dVn;
    rData.dPhi      = InData.dPhi;
    rData.dPhiVn    = InData.dPhiVn;
    rData.dVc       = InData.dVc;
    rData.dVs       = InData.dVs;
    rData.dVp       = InData.dVp;
    rData.dde       = InData.dde;
    rData.ddv       = InData.ddv;
    rData.dex       = InData.dex;
    rData.dTheta    = InData.dTheta;
    rData.dBeta     = InData.dBeta;
    rData.dAv_s     = InData.dAv_s;
    rData.dA_st     = InData.dA_st;
    rData.dAl       = InData.dAl;
    rData.dbv       = InData.dbv;
    rData.ds        = InData.ds;
    rData.dsmax     = InData.dsmax;
    rData.dAv_s_min = InData.dAv_s_min;
    rData.dAv_s_req = InData.dAv_s_req;
    rData.dAl_min   = InData.dAl_min;
    rData.dbv_min   = InData.dbv_min;
    rData.dfc       = InData.dfc;
    rData.dfy       = InData.dfy;
    rData.dfpu      = InData.dfpu;
    rData.dfpy      = InData.dfpy;
    rData.dvu       = InData.dvu;
    rData.dvu_fc    = InData.dvu_fc;
    rData.dK        = InData.dK;
    rData.dfpo      = InData.dfpo;
    rData.dVn1      = InData.dVn1;
    rData.dVn2      = InData.dVn2;
    rData.bSegmentalConstruction = InData.bSegmentalConstruction;
    rData.dAlpha    = InData.dAlpha;
    rData.dh        = InData.dh;
    rData.bOK       = InData.bOK;

    // CSA
    rData.dFt        = InData.dFt;
    rData.dFc        = InData.dFc;
    rData.dFlt       = InData.dFlt;
    rData.dFlc       = InData.dFlc;
    //Interface shear
    rData.dVui_PscsD  = InData.dVui_PscsD;
    rData.dVni_PscsD  = InData.dVni_PscsD;
    rData.bOK_PscsD   = InData.bOK_PscsD;

    //Interface shear(CSA)
    rData.dVui_PscsD_S = InData.dVui_PscsD_S;
	rData.dVni_PscsD_S = InData.dVni_PscsD_S;
    rData.bOK_PscsD_CSA  = InData.bOK_PscsD;
    rData.bChk_PscD      = InData.bChk_PscD;
    rData.dVni_Conc_PscsD      =  InData.dVni_Conc_PscsD;
    rData.dVni_Fric_PscsD      =  InData.dVni_Fric_PscsD;
    rData.dVni_Conc_Rat_PscsD  =  InData.dVni_Conc_Rat_PscsD;
    rData.dVni_Fric_Rat_PscsD  =  InData.dVni_Fric_Rat_PscsD;
    rData.bOK_Conc_PscsD       =  InData.bOK_Conc_PscsD;
    rData.bOK_Fric_PscsD       =  InData.bOK_Fric_PscsD;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmt_EngineToCVL(const _STCM_AASHTO_BASE& InData, T_CRMT_AASHTO_BASE& rData)
{
    const _CRMS_AASHTO_BASE &TorIn = InData.Tor;
    rData.bCHK      = TorIn.bCHK;
    rData.LcomK     = TorIn.LcomK;
    rData.iKind     = TorIn.iKind;
    rData.iMax      = TorIn.iMax;
    rData.dNu       = TorIn.dNu;
    rData.dVu       = TorIn.dVu;
    rData.dMu       = TorIn.dMu;
    rData.dVn       = TorIn.dVn;
    rData.dTn       = TorIn.dTn;
    rData.dTu       = TorIn.dTu;
    rData.dPhi      = TorIn.dPhi;
    rData.dPhiVn    = TorIn.dPhiVn;
    rData.dPhi_t    = TorIn.dPhi_t;
    rData.dPhi_tTn  = TorIn.dPhi_tTn;
    rData.dVc       = TorIn.dVc;
    rData.dVs       = TorIn.dVs;
    rData.dVp       = TorIn.dVp;
    rData.dde       = TorIn.dde;
    rData.ddv       = TorIn.ddv;
    rData.dex       = TorIn.dex;
    rData.dTheta    = TorIn.dTheta;
    rData.dBeta     = TorIn.dBeta;
    rData.dAv_s     = TorIn.dAv_s;
    rData.dA_st     = TorIn.dA_st;
    rData.dAl       = TorIn.dAl;
    rData.dbv       = TorIn.dbv;
    rData.ds        = TorIn.ds;
    rData.dsmax     = TorIn.dsmax;
    rData.dAv_s_min = TorIn.dAv_s_min;
    rData.dAv_s_req = TorIn.dAv_s_req;
    rData.dAl_min   = TorIn.dAl_min;
    rData.dbv_min   = TorIn.dbv_min;
    rData.dAt       = TorIn.dAt;
    rData.dAt_req   = TorIn.dAt_req;
    rData.dfc       = TorIn.dfc;
    rData.dfy       = TorIn.dfy;
    rData.dfpu      = TorIn.dfpu;
    rData.dfpy      = TorIn.dfpy;
    rData.dvu       = TorIn.dvu;
    rData.dvu_fc    = TorIn.dvu_fc;
    rData.dK        = TorIn.dK;
    rData.dfpo      = TorIn.dfpo;
    rData.dVn1      = TorIn.dVn1;
    rData.dVn2      = TorIn.dVn2;
    rData.bSegmentalConstruction = TorIn.bSegmentalConstruction;
    rData.dAlpha    = TorIn.dAlpha;
    rData.dh        = TorIn.dh;
    rData.dTcr      = TorIn.dTcr;
    rData.bOK       = TorIn.bOK;

    rData.dSig_comb      = TorIn.dCombRat;
    rData.d025phiF       = TorIn.dCombRat_Lim;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertPscDgnBdcr_EngineToCVL(const PSC_DGNR_BDCR& InD, T_PSCD_DGN_B& rData)
{
    //rData.dMd = InD.d
    rData.dMn = InD.dMr;
    rData.dRatio = InD.dMr_Rat;
    int nTndnSize = InD.arTndnDgn.GetSize();
    rData.arTndn.SetSize(nTndnSize);
    for ( int i=0; i<nTndnSize; ++i )
    {
        T_PSCD_TNDN_B& TndnB = rData.arTndn[i];
        ConvertPscDgnTndn_EngineToCVL(InD.arTndnDgn[i], TndnB);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertPscDgnTndn_EngineToCVL(const DGN_TNDN_CRC& InD, T_PSCD_TNDN_B& rData)
{
    rData.TdnaK = InD.KeyTdna;
    rData.dx    = InD.dy;
    rData.dy    = InD.dz;
    return TRUE;
}


//////////////////////////////////////////////////////////////////////////

BOOL CDgn_PSCManager::ConvertFysg_CivilToEngineAASHTO(const T_FYSG_BASE& InData, _FYSG_PSC_LRFD_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StagK  = InData.StagK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dFT_PS   = InData.dFT_PS;
    rData.dFB_PS   = InData.dFB_PS;
    rData.dFTL_PS  = InData.dFTL_PS;
    rData.dFBL_PS  = InData.dFBL_PS;
    rData.dFTR_PS  = InData.dFTR_PS;
    rData.dFBR_PS  = InData.dFBR_PS;
    rData.dFMAX_PS = InData.dFMAX_PS;

    // Check, in Engine
    rData.dALW   = InData.dALW;
    rData.dfci   = InData.dfci;
    rData.dRatio = InData.dRatio;
    rData.bOK    = InData.bOK;
    rData.dTtfc  = InData.dTtfc;
    rData.dAct   = InData.dAct;
    rData.dTtfs  = InData.dTtfs;
    rData.dAst   = InData.dAst;
    rData.dFs    = InData.dFs;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_CivilToEngineAASHTO2(const T_FYSG_AASHTO_BASE& InData, _FYSG_PSC_LRFD_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StagK  = InData.StagK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    // Check, in Engine
    rData.dALW   = InData.dALW;
    rData.bOK    = InData.bOK;
    rData.dTtfc  = InData.dTtfc;
    rData.dAct   = InData.dAct;
    rData.dTtfs  = InData.dTtfs;
    rData.dAst   = InData.dAst;
    rData.dFs    = InData.dFs;
    rData.dfci   = InData.dfci;
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypc_CivilToEngineAASHTO(const T_FYPC_AASHTO& InData, _FYPC_AASHTO& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.nLoadType = InData.nLoadType;
    rData.dFDL1  = InData.dFDL1;
    rData.dFDL2  = InData.dFDL2;
    rData.dFDL3  = InData.dFDL3;
    rData.dFLL1  = InData.dFLL1;
    rData.dFLL2  = InData.dFLL2;
    // Check, in Engine
    rData.dAFDL1 = InData.dAFDL1;
    rData.dAFDL2 = InData.dAFDL2;
    rData.dAFDL3 = InData.dAFDL3;
    rData.dAFLL1 = InData.dAFLL1;
    rData.dAFLL2 = InData.dAFLL2;
    rData.bOK    = InData.bOK;
    rData.nElemFDL1  = InData.nElemFDL1;
    rData.nElemFDL2  = InData.nElemFDL2;
    rData.nElemFLL1  = InData.nElemFLL1;
    rData.nPartFDL1  = InData.nPartFDL1;
    rData.nPartFDL2  = InData.nPartFDL2;
    rData.nPartFLL1  = InData.nPartFLL1;
    rData.nStage    = InData.nStage;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_CivilToEngineAASHTO(const T_FYCM_AASHTO_BASE& InData, _FYCM_PSC_LRFD_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.iKind = InData.iKind;
    rData.iMax  = InData.iMax;
    rData.dFT   = InData.dFT;
    rData.dFB   = InData.dFB;
    rData.dFTL  = InData.dFTL;
    rData.dFBL  = InData.dFBL;
    rData.dFTR  = InData.dFTR;
    rData.dFBR  = InData.dFBR;
    rData.dFMAX = InData.dFMAX;

    rData.dFT_PS   = InData.dFT_PS;
    rData.dFB_PS   = InData.dFB_PS;
    rData.dFTL_PS  = InData.dFTL_PS;
    rData.dFBL_PS  = InData.dFBL_PS;
    rData.dFTR_PS  = InData.dFTR_PS;
    rData.dFBR_PS  = InData.dFBR_PS;
    rData.dFMAX_PS = InData.dFMAX_PS;

    // Check, in Engine
    rData.dALW  = InData.dALW;
    rData.bOK   = InData.bOK;
    rData.dRatio= InData.dRatio;
    rData.bLTerm = InData.bLTerm;


    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFssg_CivilToEngineAASHTO(const T_FPMT_JTG_BASE& InData, _FPMT_PSC_LRFD_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.iMax  = InData.iMax;
    //rData.iKind = InData.iKind ;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    // Check, in Engine
    rData.dAFP  = InData.dAFP;
    rData.dfci    = InData.dfci;
    rData.dRatio  = InData.dRatio;

    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_CivilToEngineAASHTO(const T_FPMS_JTG_BASE& InData, _FPMT_PSC_LRFD_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.iMax  = InData.iMax;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    // Check, in Engine
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_CivilToEngineAASHTO(const T_BDCW_AASHTO_BASE& InData, _BDCW_PSC_LRFD_BASE& rData)
{
    rData.LInf.bCHK   = InData.bCHK;
    rData.LInf.LcomK  = InData.LcomK;
    rData.LInf.nMax   = InData.iMax;
    rData.LInf.nKind  = InData.iKind;
    rData.ForB.dFT    = InData.dFT;
    rData.ForB.dFB    = InData.dFB;
    rData.ForB.dft_DL = InData.dft_DL;
    rData.ForB.dfb_DL = InData.dfb_DL;
    rData.ForB.dft_LL = InData.dft_LL;
    rData.ForB.dfb_LL = InData.dfb_LL;
    rData.ForP2.dFT    = InData.dFT_Slab;
    rData.ForP2.dFB    = InData.dFB_Slab;
    rData.ForP2.dft_DL = InData.dft_DL_Slab;
    rData.ForP2.dfb_DL = InData.dfb_DL_Slab;
    rData.ForP2.dft_LL = InData.dft_LL_Slab;
    rData.ForP2.dfb_LL = InData.dfb_LL_Slab;
    rData.CrPropD.dPu  = InData.dPu;
    rData.CrPropD.dMu  = InData.dMu;
    rData.CrPropD.dPu_st  = InData.dPuShort;
    rData.CrPropD.dMu_st  = InData.dMuShort;
    rData.CrPropD.dPu_lt  = InData.dPuLong;
    rData.CrPropD.dMu_lt  = InData.dMuLong;
    rData.CrPropD.dPu_c = InData.dPu_C;
    rData.CrPropD.dMu_c = InData.dMu_C;
    rData.dMdnc = InData.dMdnc;
    // Check, in Engine
    rData.dFSS   = InData.dFSS;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.bOK    = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertTbar_CivilToEngineAASHTO(const T_TBAR_AASHTO_BASE& InData, _TBAR_PSC_LRFD_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.iMax  = InData.iMax;
    rData.iKind = InData.iKind;
    rData.dftop = InData.dftop;
    rData.dfbot = InData.dfbot;
    // Check, in Engine
    rData.dREQ  = InData.dREQ;
    rData.dMIN  = InData.dMIN;
    rData.dUSE  = InData.dUSE;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcy_CivilToEngineAASHTO(const T_BDCY_AASHTO_BASE& InData, _BDCY_PSC_LRFD_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.dMuy    = InData.dMuy;
    rData.drMuy   = InData.drMuy;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    memcpy(rData.dForceTP, InData.dForceTP, sizeof(rData.dForceTP));
    // Check, in Engine	
    rData.dAps    = InData.dAps;    // [Unit=Area]
    rData.ddp     = InData.ddp;		 // [Unit =Length]
    rData.dAs     = InData.dAs;		 // [Unit=Area]
    rData.dds     = InData.dds;		 // [Unit=Length]
    rData.db      = InData.db;		 // [Unit=Length]
    rData.dc      = InData.dc;		 // [Unit=Length]
    rData.dfps    = InData.dfps; 	 // [Unit = stress]
    rData.da      = InData.da;		 // [Unit=Length]
    rData.dfr     = InData.dfr;		 // [Unit=Stress]
    rData.dfcpe   = InData.dfcpe;	   // [Unit=Stress]
    rData.dMdnc   = InData.dMdnc;    // [Unit=Moment]
    rData.dSc     = InData.dSc;		 // [Unit=Section Modulus] //Length^3
    rData.dSnc    = InData.dSnc;    // [Unit=Section Modulus]	//Length^3
    rData.dMny    = InData.dMny;
    rData.dMcry   = InData.dMcry;
    rData.dFacM   = InData.dFacM;
    rData.dFacMcr = InData.dFacMcr;
    rData.dpMn_12Mcr_Rat  = InData.dpMn_12Mcr_Rat;
    rData.dpMn_133Mcr_Rat = InData.dpMn_133Mcr_Rat;

    rData.dAp_unbonded    = InData.dAp_unbonded;
    rData.dAp_bonded      = InData.dAp_bonded;
    rData.ddp_unbonded    = InData.ddp_unbonded;
    rData.ddp_bonded      = InData.ddp_bonded;
    rData.dAsc            = InData.dAsc;
    rData.ddsc            = InData.ddsc;
    rData.dbw             = InData.dbw;
    rData.dhf             = InData.dhf;
    rData.dBeta1          = InData.dBeta1;
    rData.dK              = InData.dK;
    rData.dfpe            = InData.dfpe;
    rData.dle             = InData.dle;
    rData.dPhi            = InData.dPhi;

    rData.dfc             = InData.dfc;
    rData.dfci            = InData.dfci;
    rData.dfy             = InData.dfy;
    rData.dfyc            = InData.dfyc;
    rData.dfpu_bonded     = InData.dfpu_bonded;
    rData.dfpu_unbonded   = InData.dfpu_unbonded;
    rData.dfpy_bonded     = InData.dfpy_bonded;
    rData.dfpy_unbonded   = InData.dfpy_unbonded;

    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCums_CivilToEngineAASHTO(const T_CUMS_AASHTO_BASE& InData, _CUMS_AASHTO_BASE& rData)
{
    rData.bCHK      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iMax      = InData.iMax;
    rData.iKind     = InData.iKind;
    rData.dNu       = InData.dNu;
    rData.dVu       = InData.dVu;
    rData.dMu       = InData.dMu;
    memcpy(rData.dForceTP, InData.dForceTP, sizeof(rData.dForceTP));
    // Check, in Engine  
    rData.dVn       = InData.dVn;
    rData.dPhi      = InData.dPhi;
    rData.dPhiVn    = InData.dPhiVn;
    rData.dVc       = InData.dVc;
    rData.dVs       = InData.dVs;
    rData.dVp       = InData.dVp;
    rData.dde       = InData.dde;
    rData.ddv       = InData.ddv;
    rData.dex       = InData.dex;
    rData.dTheta    = InData.dTheta;
    rData.dBeta     = InData.dBeta;
    rData.dAv_s     = InData.dAv_s;
    rData.dA_st     = InData.dA_st;
    rData.dAl       = InData.dAl;
    rData.dbv       = InData.dbv;
    rData.ds        = InData.ds;
    rData.dsmax     = InData.dsmax;
    rData.dAv_s_min = InData.dAv_s_min;
    rData.dAv_s_req = InData.dAv_s_req;
    rData.dAl_min   = InData.dAl_min;
    rData.dbv_min   = InData.dbv_min;

    rData.dfc       = InData.dfc;
    rData.dfy       = InData.dfy;
    rData.dfpu      = InData.dfpu;
    rData.dfpy      = InData.dfpy;
    rData.dvu       = InData.dvu;
    rData.dvu_fc    = InData.dvu_fc;
    rData.dK        = InData.dK;
    rData.dfpo      = InData.dfpo;
    rData.dVn1      = InData.dVn1;
    rData.dVn2      = InData.dVn2;
    rData.bSegmentalConstruction = InData.bSegmentalConstruction;

    rData.dAlpha    = InData.dAlpha;
    rData.dh        = InData.dh;
    rData.bOK       = InData.bOK;
    //Interface shear
    rData.dVui_PscsD  = InData.dVui_PscsD;
    rData.dVni_PscsD  = InData.dVni_PscsD;
	rData.dVui_PscsD_S = InData.dVui_PscsD_S;
	rData.dVni_PscsD_S = InData.dVni_PscsD_S;
    rData.bOK_PscsD   = InData.bOK_PscsD;
    rData.dVni_Conc_PscsD     = InData.dVni_Conc_PscsD;
    rData.dVni_Fric_PscsD     = InData.dVni_Fric_PscsD;
    rData.dVni_Conc_Rat_PscsD = InData.dVni_Conc_Rat_PscsD;
    rData.dVni_Fric_Rat_PscsD = InData.dVni_Fric_Rat_PscsD;
    rData.bOK_Conc_PscsD      = InData.bOK_Conc_PscsD;
    rData.bOK_Fric_PscsD      = InData.bOK_Fric_PscsD;
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmt_CivilToEngineAASHTO(const T_CRMT_AASHTO_BASE& InData, _STCM_AASHTO_BASE& rData)
{
    rData.Shr.bCHK      = InData.bCHK;
    rData.Shr.LcomK     = InData.LcomK;
    rData.Shr.iMax      = InData.iMax;
    rData.Shr.iKind     = InData.iKind;
    rData.Shr.dNu       = InData.dNu;
    rData.Shr.dVu       = InData.dVu;
    rData.Shr.dMu       = InData.dMu;
    rData.Shr.dVn       = InData.dVn;
    rData.Shr.dTu       = InData.dTu;
    memcpy(rData.Shr.dForceTP, InData.dForceTP, sizeof(rData.Shr.dForceTP));

    rData.Tor.bCHK      = InData.bCHK;
    rData.Tor.LcomK     = InData.LcomK;
    rData.Tor.iMax      = InData.iMax;
    rData.Tor.iKind     = InData.iKind;
    rData.Tor.dNu       = InData.dNu;
    rData.Tor.dVu       = InData.dVu;
    rData.Tor.dMu       = InData.dMu;
    rData.Tor.dVn       = InData.dVn;
    rData.Tor.dTu       = InData.dTu;
    memcpy(rData.Tor.dForceTP, InData.dForceTP, sizeof(rData.Tor.dForceTP));
    // Check, in Engine
    rData.Tor.dTn       = InData.dTn;
    rData.Tor.dPhi      = InData.dPhi;
    rData.Tor.dPhiVn    = InData.dPhiVn;
    rData.Tor.dPhi_t    = InData.dPhi_t;
    rData.Tor.dPhi_tTn  = InData.dPhi_tTn;
    rData.Tor.dVc       = InData.dVc;
    rData.Tor.dVs       = InData.dVs;
    rData.Tor.dVp       = InData.dVp;
    rData.Tor.dde       = InData.dde;
    rData.Tor.ddv       = InData.ddv;
    rData.Tor.dex       = InData.dex;
    rData.Tor.dTheta    = InData.dTheta;
    rData.Tor.dBeta     = InData.dBeta;
    rData.Tor.dAv_s     = InData.dAv_s;
    rData.Tor.dA_st     = InData.dA_st;
    rData.Tor.dAl       = InData.dAl;
    rData.Tor.dbv       = InData.dbv;
    rData.Tor.ds        = InData.ds;
    rData.Tor.dsmax     = InData.dsmax;
    rData.Tor.dAv_s_min = InData.dAv_s_min;
    rData.Tor.dAv_s_req = InData.dAv_s_req;
    rData.Tor.dAl_min   = InData.dAl_min;
    rData.Tor.dbv_min   = InData.dbv_min;
    rData.Tor.bOK       = InData.bOK;
    rData.Tor.dAt       = InData.dAt;
    rData.Tor.dAt_req   = InData.dAt_req;

    rData.Tor.dfc       = InData.dfc;
    rData.Tor.dfy       = InData.dfy;
    rData.Tor.dfpu      = InData.dfpu;
    rData.Tor.dfpy      = InData.dfpy;
    rData.Tor.dvu       = InData.dvu;
    rData.Tor.dvu_fc    = InData.dvu_fc;
    rData.Tor.dK        = InData.dK;
    rData.Tor.dfpo      = InData.dfpo;
    rData.Tor.dVn1      = InData.dVn1;
    rData.Tor.dVn2      = InData.dVn2;
    rData.Tor.bSegmentalConstruction = InData.bSegmentalConstruction;

    rData.Tor.dAlpha    = InData.dAlpha;
    rData.Tor.dh        = InData.dh;
    rData.Tor.dTcr      = InData.dTcr;
    rData.Tor.bOK       = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertPscDgnBdcr_CivilToEngineAASHTO(const T_PSCD_DGN_B& InD, _BDCY_PSC_LRFD_BASE& rData)
{
    ConvertPscForce_CivilToEngine(InD.ForD, rData.DgnR.ForceD);
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertPscForce_CivilToEngine(const T_PSCD_FORCE& ForD, PSC_LCOM_FOR_B& rData)
{
    for ( int i=0; i<9; ++i )
    {
        rData.dForce[i] = ForD.dForce[i];
    }
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_CivilToEngine(ElemPairK ElemK, const T_FYSG_PSC_BASE& InData, PSC_FYSG_BASE& rData, BOOL bReport)
{
    rData.bCHK   = InData.bCHK;
    rData.StageK = InData.StageK;
    rData.StepK  = InData.StepK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dALW   = InData.dALW;
    rData.dfci   = InData.dfci;
    rData.bOK    = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strStage = Get_SatgeName4PSC(rData.StageK);
    }

    return TRUE;
}


BOOL CDgn_PSCManager::ConvertFysg_CivilToEngine(ElemPairK ElemK, const T_FYSG_PSC_BASE_IRC& InData, PSC_FYSG_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK   = InData.bCHK;
    rData.StageK = InData.StageK;
    rData.StepK  = InData.StepK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dALW   = InData.dALW;
    rData.bOK    = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strStage = Get_SatgeName4PSC(rData.StageK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_CivilToEngine(ElemPairK ElemK, const T_FYSG_PSC_BASE_IRS& InData, PSC_FYSG_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK   = InData.bCHK;
    rData.StageK = InData.StageK;
    rData.StepK  = InData.StepK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dALW   = InData.dALW;
    rData.bOK    = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strStage = Get_SatgeName4PSC(rData.StageK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertFysg_CivilToEngineSNiP(ElemPairK ElemK, const T_FYSG_SNIP_BASE& InData, PSC_FYSG_BASE& rData, BOOL bReport)
{
    rData.bCHK   = InData.bCHK;
    rData.StageK = InData.StageK;
    rData.StepK  = InData.StepK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dALW   = InData.dALW;
    rData.bOK    = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strStage = Get_SatgeName4PSC(rData.StageK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypc_CivilToEngine(T_TDNA_K TdnaK, const T_FYPC_AASHTO& InData, PSC_FYPC_D& rData, BOOL bReport)
{
    rData.bCHK   = InData.bCHK;
    rData.dFDL1  = InData.dFDL1;
    rData.dFDL2  = InData.dFDL2;
    rData.dFDL3  = InData.dFDL3;
    rData.dFLL1  = InData.dFLL1;
    rData.dFLL2  = InData.dFLL2;
    rData.dAFDL1 = InData.dAFDL1;
    rData.dAFDL2 = InData.dAFDL2;
    rData.dAFDL3 = InData.dAFDL3;
    rData.dAFLL1 = InData.dAFLL1;
    rData.dAFLL2 = InData.dAFLL2;
    rData.bOK    = InData.bOK;

    if ( bReport )
    {
        rData.TdnaK = TdnaK;
        CString strTendonName = _T("");
        int nLoadType=0;
        if ( Get_TndnInfo4PSC(TdnaK, strTendonName, nLoadType) )
        {
            rData.nLoadType = nLoadType;
            rData.strTendonName = strTendonName;
        }
    }
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypc_CivilToEngineSNiP(T_TDNA_K TdnaK, const T_FYPC_SNIP& InData, PSC_FYPC_D& rData, BOOL bReport)
{
    rData.bCHK   = InData.bCHK;
    rData.dFDL1  = InData.dFDL1;
    rData.dFDL2  = InData.dFDL2;
    rData.dFDL3  = InData.dFDL3;
    rData.dFLL1  = InData.dFLL1;
    rData.dFLL2  = InData.dFLL2;
    rData.dAFDL1 = InData.dAFDL1;
    rData.dAFDL2 = InData.dAFDL2;
    rData.dAFDL3 = InData.dAFDL3;
    rData.dAFLL1 = InData.dAFLL1;
    rData.dAFLL2 = InData.dAFLL2;
    rData.bOK    = InData.bOK;

    if ( bReport )
    {
        rData.TdnaK = TdnaK;
        CString strTendonName = _T("");
        int nLoadType=0;
        if ( Get_TndnInfo4PSC(TdnaK, strTendonName, nLoadType) )
        {
            rData.nLoadType = nLoadType;
            rData.strTendonName = strTendonName;
        }
    }
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_CivilToEngine(ElemPairK ElemK, const T_FYCM_PSC_BASE& InData, PSC_FYCM_BASE& rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFT     = InData.dFT;
    rData.dFB     = InData.dFB;
    rData.dFTL    = InData.dFTL;
    rData.dFBL    = InData.dFBL;
    rData.dFTR    = InData.dFTR;
    rData.dFBR    = InData.dFBR;
    rData.dFMAX   = InData.dFMAX;
    rData.dALW    = InData.dALW;
    rData.bOK     = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_CivilToEngine(ElemPairK ElemK, const T_FYCM_PSC_BASE_IRC& InData, PSC_FYCM_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFT     = InData.dFT;
    rData.dFB     = InData.dFB;
    rData.dFTL    = InData.dFTL;
    rData.dFBL    = InData.dFBL;
    rData.dFTR    = InData.dFTR;
    rData.dFBR    = InData.dFBR;
    rData.dFMAX   = InData.dFMAX;
    rData.dALW    = InData.dALW;
    rData.bOK     = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_CivilToEngine(ElemPairK ElemK, const T_FYCM_PSC_BASE_IRS& InData, PSC_FYCM_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFT     = InData.dFT;
    rData.dFB     = InData.dFB;
    rData.dFTL    = InData.dFTL;
    rData.dFBL    = InData.dFBL;
    rData.dFTR    = InData.dFTR;
    rData.dFBR    = InData.dFBR;
    rData.dFMAX   = InData.dFMAX;
    rData.dALW    = InData.dALW;
    rData.bOK     = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_CivilToEngineSNiP(ElemPairK ElemK, const T_FYCM_SNIP_BASE& InData, PSC_FYCM_BASE& rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFT     = InData.dFT;
    rData.dFB     = InData.dFB;
    rData.dFTL    = InData.dFTL;
    rData.dFBL    = InData.dFBL;
    rData.dFTR    = InData.dFTR;
    rData.dFBR    = InData.dFBR;
    rData.dFMAX   = InData.dFMAX;
    rData.dALW    = InData.dALW;
    rData.bOK     = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsg_CivilToEngine(ElemPairK ElemK, const T_FPSG_PSC_BASE& InData, PSC_FPSG_BASE& rData, BOOL bReport)
{
    rData.bCHK  = InData.bCHK;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strStage = Get_SatgeName4PSC(rData.StageK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsg_CivilToEngine(ElemPairK ElemK, const T_FPSG_PSC_BASE_IRC& InData, PSC_FPSG_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK  = InData.bCHK;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strStage = Get_SatgeName4PSC(rData.StageK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsg_CivilToEngine(ElemPairK ElemK, const T_FPSG_PSC_BASE_IRS& InData, PSC_FPSG_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK  = InData.bCHK;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strStage = Get_SatgeName4PSC(rData.StageK);
    }

    return TRUE;
}


BOOL CDgn_PSCManager::ConvertFpsg_CivilToEngineSNiP(ElemPairK ElemK, const T_FPSG_SNIP_BASE& InData, PSC_FPSG_BASE& rData, BOOL bReport)
{
    rData.bCHK  = InData.bCHK;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strStage = Get_SatgeName4PSC(rData.StageK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_CivilToEngine(ElemPairK ElemK, const T_FPMS_PSC_BASE& InData, PSC_FPMS_BASE& rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertFpms_CivilToEngine(ElemPairK ElemK, const T_FPMS_PSC_BASE_IRC& InData, PSC_FPMS_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_CivilToEngine(ElemPairK ElemK, const T_FPMS_PSC_BASE_IRS& InData, PSC_FPMS_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_CivilToEngineSNiP(ElemPairK ElemK, const T_FPMS_SNIP_BASE& InData, PSC_FPMS_BASE& rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_CivilToEngine(ElemPairK ElemK, const T_BDCW_PSC_BASE& InData, PSC_BDCW_BASE& rData, BOOL bReport)
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dft    = InData.dft;
    rData.dfb    = InData.dfb;
    rData.dEpsilon_sm_cm = InData.dEpsi_sm_Epsi_cm;
    rData.dFSS   = InData.dFSS;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.dW_rat = InData.dRatio;
    rData.bOK    = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertBdcw_CivilToEngine(ElemPairK ElemK, const T_BDCW_PSC_BASE_IRC& InData, PSC_BDCW_BASE& rData, BOOL bReport)  //IRC
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dft    = InData.dft;
    rData.dfb    = InData.dfb;
    rData.dEpsilon_sm_cm = InData.dEpsi_sm_Epsi_cm;
    rData.dFSS   = InData.dFSS;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.dW_rat = InData.dRatio;
    rData.bOK    = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertBdcw_CivilToEngine(ElemPairK ElemK, const T_BDCW_PSC_BASE_IRS& InData, PSC_BDCW_BASE& rData, BOOL bReport)  //IRC
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dft    = InData.dft;
    rData.dfb    = InData.dfb;
    rData.dEpsilon_sm_cm = InData.dEpsi_sm_Epsi_cm;
    rData.dFSS   = InData.dFSS;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.dW_rat = InData.dRatio;
    rData.bOK    = InData.bOK;

    rData.dSL		= InData.dSL;
    rData.dcnom	= InData.dcnom;
    rData.dDesCW	= InData.dDesCW;
    rData.dEtamm	= InData.dEtamm;
    rData.dEta1	= InData.dEta1;
    rData.dbt		= InData.dbt;
    rData.dadash	= InData.dadash;
    rData.dMg		= InData.dMg;
    rData.dMq		= InData.dMq;
    rData.dEtas	= InData.dEtas;
    rData.dAten	= InData.dAten;
    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertBdcr_CivilToEngine(ElemPairK ElemK, const T_BDCR_PSC_BASE& InData, PSC_FRCR_D& rData, BOOL bReport)
{
    rData.bChk      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iMax      = InData.iMax;
    rData.iKind     = InData.iKind;
    rData.iLSTerm   = InData.iLSTerm;
    rData.dMu       = InData.dMu;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertBdcr_CivilToEngine(ElemPairK ElemK, const T_BDCR_PSC_BASE_IRC& InData, PSC_FRCR_D& rData, BOOL bReport)//IRC
{
    rData.bChk      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iMax      = InData.iMax;
    rData.iKind     = InData.iKind;
    rData.iLSTerm   = InData.iLSTerm;
    rData.dMu       = InData.dMu;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertBdcr_CivilToEngine(ElemPairK ElemK, const T_BDCR_PSC_BASE_IRS& InData, PSC_FRCR_D& rData, BOOL bReport)//IRC
{
    rData.bChk      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iMax      = InData.iMax;
    rData.iKind     = InData.iKind;
    rData.iLSTerm   = InData.iLSTerm;
    rData.dMu       = InData.dMu;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertCums_CivilToEngine(ElemPairK ElemK, const T_CUMS_PSC_BASE& InData, PSC_CUMS_BASE& rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu     = InData.dPu;
    rData.dVu     = InData.dVu;
    rData.dVp     = InData.dVp;
    rData.dMu     = InData.dMu;
    rData.dft     = InData.dft;
    rData.dfb     = InData.dfb;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertCums_CivilToEngine(ElemPairK ElemK, const T_CUMS_PSC_BASE_IRC& InData, PSC_CUMS_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu     = InData.dPu;
    rData.dVu     = InData.dVu;
    rData.dMu     = InData.dMu;
    rData.dft     = InData.dft;
    rData.dfb     = InData.dfb;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertCums_CivilToEngine(ElemPairK ElemK, const T_CUMS_PSC_BASE_IRS& InData, PSC_CUMS_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu     = InData.dPu;
    rData.dVu     = InData.dVu;
    rData.dMu     = InData.dMu;
    rData.dft     = InData.dft;
    rData.dfb     = InData.dfb;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
        rData.CumsIRSData.strM1 = Get_LcomName4PSC(InData.strM1);
        rData.CumsIRSData.N1 = InData.N1;
        rData.CumsIRSData.V1 = InData.V1;
        rData.CumsIRSData.M1 = InData.M1;

        rData.CumsIRSData.strM2 = Get_LcomName4PSC(InData.strM2);
        rData.CumsIRSData.N2 = InData.N2;
        rData.CumsIRSData.V2 = InData.V2;
        rData.CumsIRSData.M2 = InData.M2;

        rData.CumsIRSData.strM3 = Get_LcomName4PSC(InData.strM3);
        rData.CumsIRSData.N3 = InData.N3;
        rData.CumsIRSData.V3 = InData.V3;
        rData.CumsIRSData.M3 = InData.M3;

        rData.CumsIRSData.strM4 = Get_LcomName4PSC(InData.strM4);
        rData.CumsIRSData.N4 = InData.N4;
        rData.CumsIRSData.V4 = InData.V4;
        rData.CumsIRSData.M4 = InData.M4;

        rData.CumsIRSData.dfcp  =InData.dfcp;
        rData.CumsIRSData.db	=InData.db;
        rData.CumsIRSData.dVp	=InData.dVp_Irs;
        rData.CumsIRSData.dd	=InData.dd;
        rData.CumsIRSData.dfpt	=InData.dfpt;
        rData.CumsIRSData.dy	=InData.dy;
        rData.CumsIRSData.dWebPlanThk	=InData.dWebPlanThk;
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertCrmt_CivilToEngine(ElemPairK ElemK, const T_CRMT_PSC_BASE& InData, PSC_CRMT_BASE& rData, BOOL bReport)
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu     = InData.dPu;
    rData.dVu     = InData.dVu;
    rData.dMu     = InData.dMu;
    rData.dTu     = InData.dTu;
    rData.dft     = InData.dft;
    rData.dfb     = InData.dfb;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmt_CivilToEngine(ElemPairK ElemK, const T_CRMT_PSC_BASE_IRC& InData, PSC_CRMT_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu     = InData.dPu;
    rData.dVu     = InData.dVu;
    rData.dMu     = InData.dMu;
    rData.dTu     = InData.dTu;
    rData.dft     = InData.dft;
    rData.dfb     = InData.dfb;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertCrmt_CivilToEngine(ElemPairK ElemK, const T_CRMT_PSC_BASE_IRS& InData, PSC_CRMT_BASE& rData, BOOL bReport)//IRC
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu     = InData.dPu;
    rData.dVu     = InData.dVu;
    rData.dMu     = InData.dMu;
    rData.dTu     = InData.dTu;
    rData.dft     = InData.dft;
    rData.dfb     = InData.dfb;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertFatg_CivilToEngine(ElemPairK ElemK, const T_FATG_PSC_BASE& InData, PSC_FATG_BASE& rData, BOOL bReport)
{
    if ( bReport )
    {
        rData.strLcomName     = Get_LcomName4PSC(InData.LcomK1, TRUE);
        rData.strLcomName_s   = Get_LcomName4PSC(InData.LcomK2, TRUE);
    }

    rData.bCHK     = InData.bCHK;
    rData.iKind    = 11;                   // 11=Persistent & Transient, 12=Accidental
    rData.dSig_max = InData.dSigLcom_max; // [Unit=Stress] 
    rData.dSig_min = InData.dSigLcom_min; // [Unit=Stress] 
    rData.dVu_max  = InData.dVuLcom_max; // [Unit=Force] 
    rData.dVu_min  = InData.dVuLcom_min; // [Unit=Force] 
    rData.dVrdc    = InData.dVc;


    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFatg_CivilToEngine(ElemPairK ElemK, const T_FATG_PSC_BASE_IRC& InData, PSC_FATG_BASE& rData, BOOL bReport)//IRC
{
    if ( bReport )
    {
        rData.strLcomName     = Get_LcomName4PSC(InData.LcomK1, TRUE);
        rData.strLcomName_s   = Get_LcomName4PSC(InData.LcomK2, TRUE);
    }

    rData.bCHK     = InData.bCHK;
    rData.iKind    = 11;                   // 11=Persistent & Transient, 12=Accidental
    rData.dSig_max = InData.dSigLcom_max; // [Unit=Stress] 
    rData.dSig_min = InData.dSigLcom_min; // [Unit=Stress] 
    rData.dVu_max  = InData.dVuLcom_max; // [Unit=Force] 
    rData.dVu_min  = InData.dVuLcom_min; // [Unit=Force] 
    rData.dVrdc    = InData.dVc;


    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFatg_CivilToEngine(ElemPairK ElemK, const T_FATG_PSC_BASE_IRS& InData, PSC_FATG_BASE& rData, BOOL bReport)//IRC
{
    if ( bReport )
    {
        rData.strLcomName     = Get_LcomName4PSC(InData.LcomK1, TRUE);
        rData.strLcomName_s   = Get_LcomName4PSC(InData.LcomK2, TRUE);
    }

    rData.bCHK     = InData.bCHK;
    rData.iKind    = 11;                   // 11=Persistent & Transient, 12=Accidental
    rData.dSig_max = InData.dSigLcom_max; // [Unit=Stress] 
    rData.dSig_min = InData.dSigLcom_min; // [Unit=Stress] 
    rData.dVu_max  = InData.dVuLcom_max; // [Unit=Force] 
    rData.dVu_min  = InData.dVuLcom_min; // [Unit=Force] 
    rData.dVrdc    = InData.dVc;


    return TRUE;
}
// PSC Composite
BOOL CDgn_PSCManager::ConvertFysgComp_CivilToEngine(ElemPairK ElemK, const T_FYSG_PSC_COMP_BASE& InData, PSC_FYSG_BASE& rData, BOOL bReport)
{
    rData.bCHK   = InData.bCHK;
    rData.StageK = InData.StageK;
    rData.StepK  = InData.StepK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dALW   = InData.dALW;
    rData.dfci   = InData.dfci;
    rData.bOK    = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strStage = Get_SatgeName4PSC(rData.StageK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycmComp_CivilToEngine(ElemPairK ElemK, const T_FYCM_PSC_COMP_BASE& InData, PSC_FYCM_BASE& rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFT     = InData.dFT;
    rData.dFB     = InData.dFB;
    rData.dFTL    = InData.dFTL;
    rData.dFBL    = InData.dFBL;
    rData.dFTR    = InData.dFTR;
    rData.dFBR    = InData.dFBR;
    rData.dFMAX   = InData.dFMAX;
    rData.dALW    = InData.dALW;
    rData.bOK     = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsgComp_CivilToEngine(ElemPairK ElemK, const T_FPSG_PSC_COMP_BASE& InData, PSC_FPSG_BASE& rData, BOOL bReport)
{
    rData.bCHK  = InData.bCHK;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strStage = Get_SatgeName4PSC(rData.StageK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpmsComp_CivilToEngine(ElemPairK ElemK, const T_FPMS_PSC_COMP_BASE& InData, PSC_FPMS_BASE& rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcwComp_CivilToEngine(ElemPairK ElemK, const T_BDCW_PSC_COMP_BASE& InData, PSC_BDCW_BASE& rData, BOOL bReport)
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dft    = InData.dft;
    rData.dfb    = InData.dfb;
    rData.dEpsilon_sm_cm = InData.dEpsi_sm_Epsi_cm;
    rData.dFSS   = InData.dFSS;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.dW_rat = InData.dRatio;
    rData.bOK    = InData.bOK;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcrComp_CivilToEngine(ElemPairK ElemK, const T_BDCR_PSC_COMP_BASE& InData, PSC_FRCR_D& rData, BOOL bReport)
{
    rData.bChk      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iMax      = InData.iMax;
    rData.iKind     = InData.iKind;
    rData.iLSTerm   = InData.iLSTerm;
    rData.dMu       = InData.dMu;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCumsComp_CivilToEngine(ElemPairK ElemK, const T_CUMS_PSC_COMP_BASE& InData, PSC_CUMS_BASE& rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu     = InData.dPu;
    rData.dVu     = InData.dVu;
    rData.dMu     = InData.dMu;
    rData.dVp     = InData.dVp;
    rData.dft     = InData.dft;
    rData.dfb     = InData.dfb;

    // 	rData.dMr     = dMr;	
    // 	rData.dz_sh   = dz_sh;	

    rData.BdcwBaseComp.dM_non   = InData.dM_pre;
    rData.BdcwBaseComp.dVc1     = InData.dV_pre;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmtComp_CivilToEngine(ElemPairK ElemK, const T_CRMT_PSC_COMP_BASE& InData, PSC_CRMT_BASE& rData, BOOL bReport)
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu     = InData.dPu;
    rData.dVu     = InData.dVu;
    rData.dMu     = InData.dMu;
    rData.dTu     = InData.dTu;
    rData.dft     = InData.dft;
    rData.dfb     = InData.dfb;

    rData.dM_non   = InData.dM_pre;
    rData.dVc1     = InData.dV_pre;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }

    return TRUE;
}


BOOL CDgn_PSCManager::ConvertBdcw_CivilToEngineSNiP(ElemPairK ElemK, const T_BDCW_SNIP_BASE &InData, PSC_BDCW_SNIP_BASE &rData, BOOL bReport)
{
    rData.bCHK               = InData.bCHK;
    rData.LcomK              = InData.LcomK;
    rData.iKind              = InData.iKind;
    rData.iMax               = InData.iMax;
    rData.iLSTerm            = InData.iLSTerm;
    rData.dPu                = InData.dPu;
    rData.dQu                = InData.dQu;
    rData.dMuy               = InData.dMuy;
    rData.dMuz               = InData.dMuz;
    rData.dTu                = InData.dTu;

    rData.dSigma_top         = InData.dSigma_top;
    rData.dSigma_bot         = InData.dSigma_bot;
    rData.dSigma_bx          = InData.dSigma_bx;
    rData.dSigma_mt          = InData.dSigma_mt;
    rData.dSigma_mc          = InData.dSigma_mc;
    rData.dRb_mc2            = InData.dRb_mc2;
    rData.dSigma_bt          = InData.dSigma_bt;
    rData.dSigma_bt_Alw      = InData.dSigma_bt_Alw;
    rData.dSigma_bc_min      = InData.dSigma_bc_min;
    rData.dSigma_bc_min_Alw  = InData.dSigma_bc_min_Alw;
    rData.dSigma_mt_Alw      = InData.dSigma_mt_Alw;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcr_CivilToEngineSNiP(ElemPairK ElemK, const T_BDCR_SNIP_BASE &InData, PSC_FRCR_D &rData, BOOL bReport)
{
    rData.bChk      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iMax      = InData.iMax;
    rData.iKind     = InData.iKind;
    rData.iLSTerm   = InData.iLSTerm;
    rData.dMu       = InData.dMu;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCums_CivilToEngineSNiP(ElemPairK ElemK, const T_CUMS_SNIP_BASE &InData, PSC_CUMS_SNIP_BASE &rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.LcomQuK = InData.LcomQuK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.bIsSLS  = InData.bIsSLS ? true : false;
    rData.dPu     = InData.dPu;
    rData.dQu     = InData.dQu;
    rData.dQuSLS  = InData.dQuSLS;
    rData.dMu     = InData.dMu;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
        rData.strLcomQuName = Get_LcomName4PSC(rData.LcomQuK);
    }
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmt_CivilToEngineSNiP(ElemPairK ElemK, const T_CRMT_SNIP_BASE &InData, PSC_CUMS_SNIP_BASE &rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.LcomQuK = InData.LcomQuK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.bIsSLS  = InData.bIsSLS ? true : false;
    rData.dPu     = InData.dPu;
    rData.dQu     = InData.dQu;
    rData.dMu     = InData.dMu;
    rData.dTu     = InData.dTu;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
        rData.strLcomQuName = Get_LcomName4PSC(rData.LcomQuK);
    }
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFati_CivilToEngineSNiP(ElemPairK ElemK, const T_FATI_SNIP_BASE &InData, PSC_FATI_SNIP_BASE &rData, BOOL bReport)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.nMax;
    rData.iKind   = InData.nKind;
    rData.iLSTerm = InData.nLSTerm;
    rData.dPu     = InData.dPu;
    rData.dQu     = InData.dQu;
    rData.dMuy    = InData.dMuy;
    rData.dMuz    = InData.dMuz;
    rData.dTu     = InData.dTu;

    rData.dPu_tp   = InData.dPu_tp;
    rData.dMuy_tp  = InData.dMuy_tp;
    rData.dMuz_tp  = InData.dMuz_tp;
    rData.dPu_ts   = InData.dPu_ts;
    rData.dMuy_ts  = InData.dMuy_ts;
    rData.dMuz_ts  = InData.dMuz_ts;
    rData.dPu_per  = InData.dPu_per;
    rData.dMuy_per = InData.dMuy_per;
    rData.dMuz_per = InData.dMuz_per;

    rData.dSigma_pmax  = InData.dSigma_pmax;
    rData.dSigma_pmin  = InData.dSigma_pmin;
    rData.dSigma_bcmax = InData.dSigma_bcmax;
    rData.dSigma_bcmin = InData.dSigma_bcmin;

    // For Report
    if ( bReport )
    {
        rData.iElemK = ElemK.first;
        rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    }
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_CivilToEngineAS(ElemPairK ElemK, const T_FYSG_PSC_AS_BASE &InData, _FYSG_PSC_LRFD_BASE &rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StagK  = InData.StageK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dfci   = InData.dfci;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertTran_CivilToEngineAS(ElemPairK ElemK, const T_TRAN_PSC_AS_BASE& InData, _TRAN_PSC_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StageK = InData.StageK;
    rData.Step   = InData.StepK;
    rData.LcomK  = InData.LcomK;
    rData.dftc   = InData.dftc;
    rData.dfbc   = InData.dfbc;
    rData.dft    = InData.dft;
    rData.dfb    = InData.dfb;
    for ( int i=0; i<4; ++i )
    {
        rData.dStress[i] = InData.dStress[i];
    }
    rData.dAlwStr= InData.dAlwStr;
    rData.dfcp   = InData.dfcp;
    //rData.bRectStress =;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_CivilToEngineAS(ElemPairK ElemK, const T_FYCM_PSC_AS_BASE &InData, _FYCM_PSC_LRFD_BASE &rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.iKind = InData.nKind;
    rData.iMax  = InData.nMax;
    rData.dFT   = InData.dFT;
    rData.dFB   = InData.dFB;
    rData.dFTL  = InData.dFTL;
    rData.dFBL  = InData.dFBL;
    rData.dFTR  = InData.dFTR;
    rData.dFBR  = InData.dFBR;
    rData.dFMAX = InData.dFMAX;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsg_CivilToEngineAS(ElemPairK ElemK, const T_FPMT_JTG_BASE    &InData, _FPMT_PSC_LRFD_BASE &rData)
{
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_CivilToEngineAS(ElemPairK ElemK, const T_FPMS_PSC_BASE    &InData, _FPMT_PSC_LRFD_BASE &rData)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    // For Report
//   if(bReport)
//   {
//     rData.iElemK = ElemK;
//     rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
//   }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypc_CivilToEngineAS(T_TDNA_K TdnaK, const T_FYPC_PSC_AS& InData, _FYPC_AASHTO& rData)
{
    rData.bCHK      = InData.bCHK;
    rData.nLoadType = InData.nLoadType;
    rData.dFDL1	    = InData.dFDL1;
    rData.dFDL2	    = InData.dFDL2;
    rData.dFDL3	    = InData.dFDL3;
    rData.dFLL1	    = InData.dFLL1;
    rData.dFLL2	    = InData.dFLL2;
    rData.dAFDL1	  = InData.dAFDL1;
    rData.dAFDL2	  = InData.dAFDL2;
    rData.dAFDL3	  = InData.dAFDL3;
    rData.dAFLL1	  = InData.dAFLL1;
    rData.dAFLL2	  = InData.dAFLL2;
    rData.bOK       = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_CivilToEngineAS(ElemPairK ElemK, const T_BDCW_PSC_AS_BASE &InData, _BDCW_PSC_LRFD_BASE &rData)
{
    rData.LInf.bCHK   = InData.bCHK;
    rData.LInf.LcomK  = InData.LcomK;
    rData.LInf.nMax   = InData.nMax;
    rData.LInf.nKind  = InData.nKind;
    rData.ForB.dFT    = InData.dftop;
    rData.ForB.dFB    = InData.dfbot;
    rData.ForB.dft_DL = InData.dft;
    rData.ForB.dfb_DL = InData.dfb;
    rData.ForB.dft_LL = InData.dft_LL;
    rData.ForB.dfb_LL = InData.dfb_LL;
    rData.ForP2.dFT   = InData.dftop_sl;
    rData.ForP2.dFB   = InData.dfbot_sl;
    rData.ForP2.dft_DL= InData.dft_DL_sl;
    rData.ForP2.dfb_DL= InData.dfb_DL_sl;
    rData.ForP2.dft_LL= InData.dft_LL_sl;
    rData.ForP2.dfb_LL= InData.dfb_LL_sl;
    rData.dfc_p  = InData.dfcp;
    rData.ForB.dForce[4] = InData.dMu;
    rData.CrPropD.dPu    = InData.ForBe.dFxx;
    rData.CrPropD.dMu    = InData.ForBe.dMuy;
    rData.CrPropD.dPu_st = InData.ForST.dFxx;
    rData.CrPropD.dMu_st = InData.ForST.dMuy;
    rData.CrPropD.dPu_lt = InData.ForLT.dFxx;
    rData.CrPropD.dMu_lt = InData.ForLT.dMuy;
    rData.CrPropD.dPu_c  = InData.ForST.dFxx + InData.ForLT.dFxx;
    rData.CrPropD.dMu_c  = InData.ForST.dMuy + InData.ForLT.dMuy;
    // Check, in Engine
    rData.ds_use = InData.ds;
    rData.ds_max = InData.dsa;
    rData.bOK    = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcr_CivilToEngineAS(ElemPairK ElemK, const T_BDCR_PSC_AS_BASE &InData, _BDCY_PSC_LRFD_BASE &rData)
{
    rData.bCHK    = InData.bCHK;
    rData.dMuy    = InData.dMu;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.nMax;
    rData.iKind   = InData.nKind;
    //memcpy(rData.dForceTP, InData.dForceTP, sizeof(rData.dForceTP));
    // Check, in Engine	
//   rData.drMuy   = InData.dpMn;
//   rData.dAps    = InData.dAps ;    // [Unit=Area]
//   rData.ddp     = InData.ddp  ;		 // [Unit =Length]
//   rData.dAs     = InData.dAs  ;		 // [Unit=Area]
//   rData.dds     = InData.dds  ;		 // [Unit=Length]
//   rData.db      = InData.db   ;		 // [Unit=Length]
//   rData.dc      = InData.dc   ;		 // [Unit=Length]
//   rData.dfps    = InData.dfps ; 	 // [Unit = stress]
//   rData.da      = InData.da   ;		 // [Unit=Length]
//   rData.dfr     = InData.dfr  ;		 // [Unit=Stress]
//   rData.dfcpe   = InData.dfcpe;	   // [Unit=Stress]
//   rData.dMdnc   = InData.dMdnc;    // [Unit=Moment]
//   rData.dSc     = InData.dSc  ;		 // [Unit=Section Modulus] //Length^3
//   rData.dSnc    = InData.dSnc ;    // [Unit=Section Modulus]	//Length^3
//   rData.dMny    = InData.dMny;
//   rData.dMcry   = InData.dMcry;
//   rData.dFacM   = InData.dFacM;
//   rData.dFacMcr = InData.dFacMcr;
//   rData.dpMn_12Mcr_Rat  = InData.dpMn_12Mcr_Rat;
//   rData.dpMn_133Mcr_Rat = InData.dpMn_133Mcr_Rat;		  
// 
//   rData.dAp_unbonded    = InData.dAp_unbonded; 
//   rData.dAp_bonded      = InData.dAp_bonded;
//   rData.ddp_unbonded    = InData.ddp_unbonded;
//   rData.ddp_bonded      = InData.ddp_bonded;
//   rData.dAsc            = InData.dAsc;
//   rData.ddsc            = InData.ddsc;
//   rData.dbw             = InData.dbw;
//   rData.dhf             = InData.dhf;
//   rData.dBeta1          = InData.dBeta1;
//   rData.dK              = InData.dK;
//   rData.dfpe            = InData.dfpe;
//   rData.dle             = InData.dle;
//   rData.dPhi            = InData.dPhi;
// 
//   rData.dfc             = InData.dfc;
//   rData.dfci            = InData.dfci;
//   rData.dfy             = InData.dfy;
//   rData.dfyc            = InData.dfyc;
//   rData.dfpu_bonded     = InData.dfpu_bonded;
//   rData.dfpu_unbonded   = InData.dfpu_unbonded;
//   rData.dfpy_bonded     = InData.dfpy_bonded;
//   rData.dfpy_unbonded   = InData.dfpy_unbonded;

    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCums_CivilToEngineAS(ElemPairK ElemK, const T_SHRR_PSC_AS_BASE &InData, _CUMS_AASHTO_BASE &rData)
{
    rData.bCHK      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iMax      = InData.nMax;
    rData.iKind     = InData.nKind;
    rData.dNu       = InData.dFu;
    rData.dVu       = InData.dVu;
    rData.dMu       = InData.dMu;
    rData.dTu       = InData.dTu;
    memcpy(rData.dForceTP, InData.dForceTP, sizeof(rData.dForceTP));
    // Check, in Engine  
//   rData.dVn       = InData.dVn      ;
//   rData.dPhi      = InData.dPhi     ;
//   rData.dPhiVn    = InData.dPhiVn   ;
//   rData.dVc       = InData.dVc      ;
//   rData.dVs       = InData.dVs      ;
//   rData.dVp       = InData.dVp      ;
//   rData.dde       = InData.dde      ;
//   rData.ddv       = InData.ddv      ;
//   rData.dex       = InData.dex      ;
//   rData.dTheta    = InData.dTheta   ;
//   rData.dBeta     = InData.dBeta    ;
//   rData.dAv_s     = InData.dAv_s    ;
//   rData.dA_st     = InData.dA_st    ;
//   rData.dAl       = InData.dAl      ;
//   rData.dbv       = InData.dbv      ;
//   rData.ds        = InData.ds       ;
//   rData.dsmax     = InData.dsmax    ;
//   rData.dAv_s_min = InData.dAv_s_min;
//   rData.dAv_s_req = InData.dAv_s_req;
//   rData.dAl_min   = InData.dAl_min  ;
//   rData.dbv_min   = InData.dbv_min  ;
// 
//   rData.dfc       = InData.dfc;
//   rData.dfy       = InData.dfy;
//   rData.dfpu      = InData.dfpu;
//   rData.dfpy      = InData.dfpy;
//   rData.dvu       = InData.dvu;
//   rData.dvu_fc    = InData.dvu_fc;
//   rData.dK        = InData.dK;
//   rData.dfpo      = InData.dfpo;
//   rData.dVn1      = InData.dVn1;
//   rData.dVn2      = InData.dVn2;
//   rData.bSegmentalConstruction = InData.bSegmentalConstruction;
// 
//   rData.dAlpha    = InData.dAlpha;
//   rData.dh        = InData.dh;
    rData.bOK       = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmt_CivilToEngineAS(ElemPairK ElemK, const T_TORR_PSC_AS_BASE &InData, _STCM_AASHTO_BASE &rData)
{
    _CUMS_AASHTO_BASE &ShrD = rData.Shr;
    ShrD.bCHK      = InData.bCHK;
    ShrD.LcomK     = InData.LcomK;
    ShrD.iMax      = InData.nMax;
    ShrD.iKind     = InData.nKind;
    ShrD.dNu       = InData.dFu;
    ShrD.dVu       = InData.dVu;
    ShrD.dMu       = InData.dMu;
    ShrD.dTu       = InData.dTu;
    memcpy(ShrD.dForceTP, InData.dForceTP, sizeof(ShrD.dForceTP));

    _CRMS_AASHTO_BASE &TorD = rData.Tor;
    TorD.bCHK      = InData.bCHK;
    TorD.LcomK     = InData.LcomK;
    TorD.iMax      = InData.nMax;
    TorD.iKind     = InData.nKind;
    TorD.dNu       = InData.dFu;
    TorD.dVu       = InData.dVu;
    TorD.dMu       = InData.dMu;
    TorD.dTu       = InData.dTu;
    memcpy(TorD.dForceTP, InData.dForceTP, sizeof(TorD.dForceTP));
    // Check, in Engine
    TorD.dVn       = InData.dpVn;
    //   rData.dTn       = InData.dTn      ;
    //   rData.dPhi      = InData.dPhi     ;
    //   rData.dPhiVn    = InData.dPhiVn   ;
    //   rData.dPhi_t    = InData.dPhi_t   ;
    //   rData.dPhi_tTn  = InData.dPhi_tTn ;
    //   rData.dVc       = InData.dVc      ;
    //   rData.dVs       = InData.dVs      ;
    //   rData.dVp       = InData.dVp      ;
    //   rData.dde       = InData.dde      ;
    //   rData.ddv       = InData.ddv      ;
    //   rData.dex       = InData.dex      ;
    //   rData.dTheta    = InData.dTheta   ;
    //   rData.dBeta     = InData.dBeta    ;
    //   rData.dAv_s     = InData.dAv_s    ;
    //   rData.dA_st     = InData.dA_st    ;
    //   rData.dAl       = InData.dAl      ;
    //   rData.dbv       = InData.dbv      ;	
    //   rData.ds        = InData.ds       ;
    //   rData.dsmax     = InData.dsmax    ;
    //   rData.dAv_s_min = InData.dAv_s_min;
    //   rData.dAv_s_req = InData.dAv_s_req;
    //   rData.dAl_min   = InData.dAl_min  ;
    //   rData.dbv_min   = InData.dbv_min  ;	
    //   rData.bOK       = InData.bOK      ;
    //   rData.dAt       = InData.dAt      ;
    //   rData.dAt_req   = InData.dAt_req  ;
    // 
    //   rData.dfc       = InData.dfc;
    //   rData.dfy       = InData.dfy;
    //   rData.dfpu      = InData.dfpu;
    //   rData.dfpy      = InData.dfpy;
    //   rData.dvu       = InData.dvu;
    //   rData.dvu_fc    = InData.dvu_fc;
    //   rData.dK        = InData.dK;
    //   rData.dfpo      = InData.dfpo;
    //   rData.dVn1      = InData.dVn1;
    //   rData.dVn2      = InData.dVn2;
    //   rData.bSegmentalConstruction = InData.bSegmentalConstruction;
    // 
    //   rData.dAlpha    = InData.dAlpha;
    //   rData.dh        = InData.dh;
    //   rData.dTcr      = InData.dTcr;
    TorD.bOK       = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_CivilToEngineBS(ElemPairK ElemK, const T_FYSG_PSC_BS_BASE& InData, PSC_FYSG_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.iElemK = ElemK.first;
    rData.StageK = InData.StageK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dfci   = InData.dfci;
    rData.dALW   = InData.dALW;
    rData.bOK    = InData.bOK;
    rData.strStage = Get_SatgeName4PSC(rData.StageK);

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_CivilToEngineBS(ElemPairK ElemK, const T_FYCM_PSC_BS_BASE& InData, PSC_FYCM_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.iElemK = ElemK.first;
    rData.LcomK = InData.LcomK;
    rData.iKind = InData.nKind;
    rData.iMax  = InData.nMax;
    rData.dFT   = InData.dFT;
    rData.dFB   = InData.dFB;
    rData.dFTL  = InData.dFTL;
    rData.dFBL  = InData.dFBL;
    rData.dFTR  = InData.dFTR;
    rData.dFBR  = InData.dFBR;
    rData.dFMAX = InData.dFMAX;
    rData.dALW  = InData.dALW;
    rData.bOK   = InData.bOK;
    rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
    rData.dDepthFac = InData.dDepthFac;
    rData.dFlexTensC3 = InData.dFlexTensC3;
    rData.dAddReinStress = InData.dAddReinStress;
    rData.dTensConcArea = InData.dTensConcArea;
    rData.dTensRebarArea = InData.dTensRebarArea;
    rData.bJointExist = InData.bJointExist;
    rData.dJointStressLimit = InData.dJointStressLimit;
	rData.dfsa = InData.dfsa;
	rData.dfs = InData.dfs;
    rData.bJointOK = InData.bJointOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsg_CivilToEngineBS(ElemPairK ElemK, const T_FPSG_PSC_BASE& InData, PSC_FPSG_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.iElemK = ElemK.first;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    // Check, in Engine
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;
    rData.strStage = Get_SatgeName4PSC(rData.StageK);

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_CivilToEngineBS(ElemPairK ElemK, const T_FPMS_PSC_BASE& InData, PSC_FPMS_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.iElemK  = ElemK.first;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;
    rData.strLcomName = Get_LcomName4PSC(rData.LcomK);

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_CivilToEngineBS(ElemPairK ElemK, const T_BDCW_PSC_BS_BASE& InData, PSC_BDCW_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.iElemK = ElemK.first;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    rData.dMg    = InData.dMg;
    rData.dMq    = InData.dMq;
    rData.dfb    = InData.dfb;
    rData.dft    = InData.dft;

    rData.bOK    = InData.bOK;

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertBdcr_CivilToEngineBS(ElemPairK ElemK, const T_BDCR_PSC_BS_BASE& InData, PSC_FRCR_D& rData)
{
    rData.bChk    = InData.bCHK;
    rData.iElemK  = ElemK.first;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.nMax;
    rData.iKind   = InData.nKind;

    rData.dMu    = InData.dMy;
    rData.strLcomName = Get_LcomName4PSC(rData.LcomK);

    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCums_CivilToEngineBS(ElemPairK ElemK, const T_SHRR_PSC_BS_BASE& InData, PSC_CUMS_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.iElemK = ElemK.first;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.nMax;
    rData.iKind   = InData.nKind;

    rData.dPu     = InData.dFx;
    rData.dVu     = InData.dVz;
    rData.dMu     = InData.dMy;
    rData.strLcomName = Get_LcomName4PSC(rData.LcomK);
	for ( int i = 0; i < 6; ++i ) rData.dForceTP[i] = InData.dForceTP[i];

    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmt_CivilToEngineBS(ElemPairK ElemK, const T_TORR_PSC_BS_BASE& InData, PSC_CRMT_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.iElemK = ElemK.first;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.nMax;
    rData.iKind   = InData.nKind;

    rData.dPu     = InData.dFx;
    rData.dVu     = InData.dVz;
    rData.dMu     = InData.dMy;
    rData.dTu     = InData.dT;
    rData.strLcomName = Get_LcomName4PSC(rData.LcomK);

    rData.bOK      = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypc_CivilToEngineBS(T_TDNA_K TdnaK, const T_FYPC_PSC_BS& InData, PSC_FYPC_D& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.dFpu   = InData.dFpu;
    rData.dFDL1  = InData.dFDL1;
    rData.dFDL2  = InData.dFDL2;
    rData.dFLL1  = InData.dFLL1;
    rData.dAFDL1 = InData.dAFDL1;
    rData.bOK    = InData.bOK;
    rData.TdnaK = TdnaK;
    CString strTendonName = _T("");
    int nLoadType=0;
    if ( Get_TndnInfo4PSC(TdnaK, strTendonName, nLoadType) )
    {
        rData.nLoadType = nLoadType;
        rData.strTendonName = strTendonName;
    }

    return TRUE;
}

//
BOOL CDgn_PSCManager::ConvertFysg_EngineToCivil(const PSC_FYSG_BASE& InData, T_FYSG_PSC_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StageK  = InData.StageK;
    rData.StepK  = InData.StepK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dALW   = InData.dALW;
    rData.dRatio = InData.dRatio;
    rData.bOK    = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_EngineToCivil(const PSC_FYSG_BASE& InData, T_FYSG_PSC_BASE_IRC& rData)//IRC
{
    rData.bCHK   = InData.bCHK;
    rData.StageK  = InData.StageK;
    rData.StepK  = InData.StepK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dALW   = InData.dALW;
    rData.bOK    = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_EngineToCivil(const PSC_FYSG_BASE& InData, T_FYSG_PSC_BASE_IRS& rData)//IRC
{
    rData.bCHK   = InData.bCHK;
    rData.StageK  = InData.StageK;
    rData.StepK  = InData.StepK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dALW   = InData.dALW;
    rData.bOK    = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypc_EngineToCivil(const PSC_FYPC_D&    InData, T_FYPC_AASHTO&   rData)
{
    rData.bCHK   = InData.bCHK;
    rData.dFDL1  = InData.dFDL1;
    rData.dFDL2  = InData.dFDL2;
    rData.dFDL3  = InData.dFDL3;
    rData.dFLL1  = InData.dFLL1;
    rData.dFLL2  = InData.dFLL2;
    rData.dAFDL1 = InData.dAFDL1;
    rData.dAFDL2 = InData.dAFDL2;
    rData.dAFDL3 = InData.dAFDL3;
    rData.dAFLL1 = InData.dAFLL1;
    rData.dAFLL2 = InData.dAFLL2;
    rData.bOK    = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_EngineToCivil(const PSC_FYCM_BASE& InData, T_FYCM_PSC_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFT     = InData.dFT;
    rData.dFB     = InData.dFB;
    rData.dFTL    = InData.dFTL;
    rData.dFBL    = InData.dFBL;
    rData.dFTR    = InData.dFTR;
    rData.dFBR    = InData.dFBR;
    rData.dFMAX   = InData.dFMAX;
    rData.dALW    = InData.dALW;
    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_EngineToCivil(const PSC_FYCM_BASE& InData, T_FYCM_PSC_BASE_IRC& rData)//IRC
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFT     = InData.dFT;
    rData.dFB     = InData.dFB;
    rData.dFTL    = InData.dFTL;
    rData.dFBL    = InData.dFBL;
    rData.dFTR    = InData.dFTR;
    rData.dFBR    = InData.dFBR;
    rData.dFMAX   = InData.dFMAX;
    rData.dALW    = InData.dALW;
    rData.bOK     = InData.bOK;

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertFycm_EngineToCivil(const PSC_FYCM_BASE& InData, T_FYCM_PSC_BASE_IRS& rData)//IRS
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFT     = InData.dFT;
    rData.dFB     = InData.dFB;
    rData.dFTL    = InData.dFTL;
    rData.dFBL    = InData.dFBL;
    rData.dFTR    = InData.dFTR;
    rData.dFBR    = InData.dFBR;
    rData.dFMAX   = InData.dFMAX;
    rData.dALW    = InData.dALW;
    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsg_EngineToCivil(const PSC_FPSG_BASE& InData, T_FPSG_PSC_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsg_EngineToCivil(const PSC_FPSG_BASE& InData, T_FPSG_PSC_BASE_IRC& rData)//IRC
{
    rData.bCHK  = InData.bCHK;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsg_EngineToCivil(const PSC_FPSG_BASE& InData, T_FPSG_PSC_BASE_IRS& rData)//IRS
{
    rData.bCHK  = InData.bCHK;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_EngineToCivil(const PSC_FPMS_BASE& InData, T_FPMS_PSC_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_EngineToCivil(const PSC_FPMS_BASE& InData, T_FPMS_PSC_BASE_IRC& rData)//IRC
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_EngineToCivil(const PSC_FPMS_BASE& InData, T_FPMS_PSC_BASE_IRS& rData)//IRS
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCivil(const PSC_BDCW_BASE& InData, BOOL bOk_Cr, T_BDCW_PSC_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFSS   = InData.dFSS;
    rData.dEpsilon_sm = InData.dEpsilon_sm;
    rData.dEpsilon_cm = InData.dEpsilon_cm;
    rData.dEpsi_sm_Epsi_cm = InData.dEpsilon_sm_cm;
    rData.dSr_max = InData.dSr_max;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.dWk    = InData.dWk;
    rData.dWmax  = InData.dWa;
    rData.dRatio = InData.dW_rat;
    rData.bOK    = InData.bOK;
    rData.dCrack_Asmin = InData.dCrack_Asmin;
    rData.dAst         = InData.dAst;
    rData.bOK_Cr       = bOk_Cr;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCivil(const PSC_BDCW_BASE& InData, BOOL bOk_Cr, T_BDCW_PSC_BASE_IRC& rData)  //IRC
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFSS   = InData.dFSS;
    rData.dEpsilon_sm = InData.dEpsilon_sm;
    rData.dEpsilon_cm = InData.dEpsilon_cm;
    rData.dEpsi_sm_Epsi_cm = InData.dEpsilon_sm_cm;
    rData.dSr_max = InData.dSr_max;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.dWk    = InData.dWk;
    rData.dWmax  = InData.dWa;
    rData.dRatio = InData.dW_rat;
    rData.bOK    = InData.bOK;
    rData.dCrack_Asmin = InData.dCrack_Asmin;
    rData.dAst         = InData.dAst;
    rData.bOK_Cr       = bOk_Cr;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCivil(const PSC_BDCW_BASE& InData, BOOL bOk_Cr, T_BDCW_PSC_BASE_IRS& rData)  //IRS
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFSS   = InData.dFSS;
    rData.dEpsilon_sm = InData.dEpsilon_sm;
    rData.dEpsilon_cm = InData.dEpsilon_cm;
    rData.dEpsi_sm_Epsi_cm = InData.dEpsilon_sm_cm;
    rData.dSr_max = InData.dSr_max;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.dWk    = InData.dWk;
    rData.dWmax  = InData.dWa;
    rData.dRatio = InData.dW_rat;
    rData.bOK    = InData.bOK;
    rData.dCrack_Asmin = InData.dCrack_Asmin;
    rData.dAst         = InData.dAst;
    rData.bOK_Cr       = bOk_Cr;

    rData.dSL		= InData.dSL;
    rData.dcnom	= InData.dcnom;
    rData.dDesCW	= InData.dDesCW;
    rData.dEtamm	= InData.dEtamm;
    rData.dEta1	= InData.dEta1;
    rData.dbt		= InData.dbt;
    rData.dadash	= InData.dadash;
    rData.dMg		= InData.dMg;
    rData.dMq		= InData.dMq;
    rData.dEtas	= InData.dEtas;
    rData.dAten	= InData.dAten;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCivil_zero(const PSC_BDCW_BASE& InData, BOOL bOK_Zero, T_BDCW_PSC_BASE& rData)
{
    rData.bCHK   = InData.bCHK_Zero;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    //   rData.dFT    = InData.dFT   ;
    //   rData.dFB    = InData.dFB   ;
    //   rData.dFSS   = InData.dFSS  ;
    //   rData.dEpsilon_sm = InData.dEpsilon_sm;
    //   rData.dEpsilon_cm = InData.dEpsilon_cm;
    //   rData.dEpsi_sm_Epsi_cm = InData.dEpsilon_sm_cm;
    //   rData.dSr_max = InData.dSr_max;
    //   rData.ds_use = InData.ds_use;
    //   rData.ds_max = InData.ds_max;
    //   rData.dWk    = InData.dWk   ;
    //   rData.dWmax  = InData.dWa   ;
    //   rData.dRatio = InData.dW_rat;
    //   rData.bOK    = InData.bOK   ;
    //   rData.dCrack_Asmin = InData.dCrack_Asmin;
    //   rData.dAst         = InData.dAst        ;

    rData.dRatio         = InData.dRat_zero;
    rData.bOk_Zero       = bOK_Zero;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCivil_zero(const PSC_BDCW_BASE& InData, BOOL bOK_Zero, T_BDCW_PSC_BASE_IRC& rData)
{
    rData.bCHK   = InData.bCHK_Zero;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    //   rData.dFT    = InData.dFT   ;
    //   rData.dFB    = InData.dFB   ;
    //   rData.dFSS   = InData.dFSS  ;
    //   rData.dEpsilon_sm = InData.dEpsilon_sm;
    //   rData.dEpsilon_cm = InData.dEpsilon_cm;
    //   rData.dEpsi_sm_Epsi_cm = InData.dEpsilon_sm_cm;
    //   rData.dSr_max = InData.dSr_max;
    //   rData.ds_use = InData.ds_use;
    //   rData.ds_max = InData.ds_max;
    //   rData.dWk    = InData.dWk   ;
    //   rData.dWmax  = InData.dWa   ;
    //   rData.dRatio = InData.dW_rat;
    //   rData.bOK    = InData.bOK   ;
    //   rData.dCrack_Asmin = InData.dCrack_Asmin;
    //   rData.dAst         = InData.dAst        ;

    rData.dRatio         = InData.dRat_zero;
    rData.bOk_Zero       = bOK_Zero;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCivil_zero(const PSC_BDCW_BASE& InData, BOOL bOK_Zero, T_BDCW_PSC_BASE_IRS& rData)
{
    rData.bCHK   = InData.bCHK_Zero;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    //   rData.dFT    = InData.dFT   ;
    //   rData.dFB    = InData.dFB   ;
    //   rData.dFSS   = InData.dFSS  ;
    //   rData.dEpsilon_sm = InData.dEpsilon_sm;
    //   rData.dEpsilon_cm = InData.dEpsilon_cm;
    //   rData.dEpsi_sm_Epsi_cm = InData.dEpsilon_sm_cm;
    //   rData.dSr_max = InData.dSr_max;
    //   rData.ds_use = InData.ds_use;
    //   rData.ds_max = InData.ds_max;
    //   rData.dWk    = InData.dWk   ;
    //   rData.dWmax  = InData.dWa   ;
    //   rData.dRatio = InData.dW_rat;
    //   rData.bOK    = InData.bOK   ;
    //   rData.dCrack_Asmin = InData.dCrack_Asmin;
    //   rData.dAst         = InData.dAst        ;

    rData.dRatio         = InData.dRat_zero;
    rData.bOk_Zero       = bOK_Zero;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCivil_Asmin(const PSC_BDCW_BASE& InData, BOOL bOk_Asmin, T_BDCW_PSC_BASE& rData)
{
    //Asmin(최소철근량 검토 여부)
    rData.bCHK              =InData.bCHK_Asmin;
    rData.LcomK             =InData.LcomK;
    rData.iKind             =InData.iKind;
    rData.iMax              =InData.iMax;
    rData.iLSTerm           =InData.iLSTerm;
    rData.dPu               =0.0;
    rData.dMu               =0.0;
    rData.dFT		            =0.0;
    rData.dFB		            =0.0;
    rData.dft               =InData.dft;
    rData.dfb               =InData.dfb;
    rData.dFSS	            =0.0;
    rData.dEpsilon_sm       =0.0;
    rData.dEpsilon_cm       =0.0;
    rData.dEpsi_sm_Epsi_cm  =0.0;
    rData.dSr_max           =0.0;
    rData.ds_use	          =0.0;
    rData.ds_max	          =0.0;
    rData.dWk               =0.0;
    rData.dWmax             =0.0;
    rData.dRatio            =InData.dRatio_Asmin;
    rData.dCrack_Asmin      =0.0;
    rData.dAst              =InData.dfct_eff;//dfct_eff 값 확인용
    rData.bOK               =InData.bOK_Asmin;
    rData.bOK_Cr            =FALSE;
    rData.bOk_Zero          =FALSE;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcr_EngineToCivil(const PSC_FRCR_D&    InData, T_BDCR_PSC_BASE& rData)
{
    rData.bCHK    = InData.bChk;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;

    rData.dMu     = InData.dMu;
    rData.dMn     = InData.dMn;
    rData.dMn_cal = InData.dMn_cal;
    rData.dphi    = InData.dphi;
    rData.dphiMn  = InData.dphiMn;
    rData.dRatM   = InData.dRatM;
    rData.dc_neu  = InData.dc_neu;
    rData.dalpha  = InData.dalpha;
    rData.dbeta1  = InData.dbeta1;
    rData.da      = InData.da;
    rData.dPe     = InData.dPe;
    rData.dPe_t   = InData.dPe_t;
    rData.dAps    = InData.dAps;
    rData.dAps_t  = InData.dAps_t;
    rData.dAst    = InData.dAst;
    rData.dAsc    = InData.dAsc;
    rData.dRho_pt = InData.dRho_pt;
    rData.dRho_st = InData.dRho_st;
    rData.dRho_sc = InData.dRho_sc;
    rData.dRho_ps = InData.dRho_ps;
    rData.ddpt    = InData.ddpt;
    rData.ddst    = InData.ddst;
    rData.ddsc    = InData.ddsc;
    //
    //rData.dphiMcr    = ;   
    rData.dphiMn_mo  = InData.d12Mcr;
    //rData.dRho_p_max = ;
    //rData.dRho_p_min = ;

    // KSCE-LSD15
    rData.dCmax    = InData.dCmax;
    rData.dAs_max    = InData.dAs_max;
    rData.dAs_min    = InData.dAs_min;

    rData.bOK = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcr_EngineToCivil(const PSC_FRCR_D&    InData, T_BDCR_PSC_BASE_IRC& rData)//IRC
{
    rData.bCHK    = InData.bChk;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;

    rData.dMu     = InData.dMu;
    rData.dMn     = InData.dMn;
    rData.dMn_cal = InData.dMn_cal;
    rData.dphi    = InData.dphi;
    rData.dphiMn  = InData.dphiMn;
    rData.dRatM   = InData.dRatM;
    rData.dc_neu  = InData.dc_neu;
    rData.dalpha  = InData.dalpha;
    rData.dbeta1  = InData.dbeta1;
    rData.da      = InData.da;
    rData.dPe     = InData.dPe;
    rData.dPe_t   = InData.dPe_t;
    rData.dAps    = InData.dAps;
    rData.dAps_t  = InData.dAps_t;
    rData.dAst    = InData.dAst;
    rData.dAsc    = InData.dAsc;
    rData.dRho_pt = InData.dRho_pt;
    rData.dRho_st = InData.dRho_st;
    rData.dRho_sc = InData.dRho_sc;
    rData.dRho_ps = InData.dRho_ps;
    rData.ddpt    = InData.ddpt;
    rData.ddst    = InData.ddst;
    rData.ddsc    = InData.ddsc;
    //
    //rData.dphiMcr    = ;   
    rData.dphiMn_mo  = InData.d12Mcr;
    //rData.dRho_p_max = ;
    //rData.dRho_p_min = ;

    // KSCE-LSD15
    rData.dCmax    = InData.dCmax;
    rData.dAs_max    = InData.dAs_max;
    rData.dAs_min    = InData.dAs_min;

    rData.bOK = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcr_EngineToCivil(const PSC_FRCR_D&    InData, T_BDCR_PSC_BASE_IRS& rData)//IRS
{
    rData.bCHK    = InData.bChk;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;

    rData.dMu     = InData.dMu;
    rData.dMn     = InData.dMn;
    rData.dMn_cal = InData.dMn_cal;
    rData.dphi    = InData.dphi;
    rData.dphiMn  = InData.dphiMn;
    rData.dRatM   = InData.dRatM;
    rData.dc_neu  = InData.dc_neu;
    rData.dalpha  = InData.dalpha;
    rData.dbeta1  = InData.dbeta1;
    rData.da      = InData.da;
    rData.dPe     = InData.dPe;
    rData.dPe_t   = InData.dPe_t;
    rData.dAps    = InData.dAps;
    rData.dAps_t  = InData.dAps_t;
    rData.dAst    = InData.dAst;
    rData.dAsc    = InData.dAsc;
    rData.dRho_pt = InData.dRho_pt;
    rData.dRho_st = InData.dRho_st;
    rData.dRho_sc = InData.dRho_sc;
    rData.dRho_ps = InData.dRho_ps;
    rData.ddpt    = InData.ddpt;
    rData.ddst    = InData.ddst;
    rData.ddsc    = InData.ddsc;
    //
    //rData.dphiMcr    = ;   
    rData.dphiMn_mo  = InData.d12Mcr;
    //rData.dRho_p_max = ;
    //rData.dRho_p_min = ;

    // KSCE-LSD15
    rData.dCmax    = InData.dCmax;
    rData.dAs_max    = InData.dAs_max;
    rData.dAs_min    = InData.dAs_min;

    rData.bOK = InData.bOK;

    return TRUE;
}


BOOL CDgn_PSCManager::ConvertCums_EngineToCivil(const PSC_CUMS_BASE& InData, T_CUMS_PSC_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu       = InData.dPu;
    rData.dVu       = InData.dVu;
    rData.dMu       = InData.dMu;
    rData.dft       = InData.dft;
    rData.dfb       = InData.dfb;
    rData.dVn       = InData.dVn;
    rData.dPhi      = InData.dPhi;
    rData.dPhiVn    = InData.dPhiVn;
    rData.dVc       = InData.dVc;
    rData.dVs       = InData.dVs;
    rData.dVp       = InData.dVp;
    rData.dAv_s     = InData.dAv_s;
    rData.dAv_s_min = InData.dAv_s_min;
    rData.dAv_s_req = InData.dAv_s_req;
    rData.ds        = InData.ds;
    rData.ds_max    = InData.ds_max;
    rData.bOK       = InData.bOK;
    rData.dV_Rd_max = InData.dV_Rd_max;
    rData.dVu_pVn_Rat = InData.dVu_pVn_Rat;

    rData.dRho_w = InData.dRho_w;
    rData.dRho_w_min = InData.dRho_w_min;
    rData.ds_l_max = InData.ds_l_max;

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertCums_EngineToCivil(const PSC_CUMS_BASE& InData, T_CUMS_PSC_BASE_IRC& rData)//IRC
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu       = InData.dPu;
    rData.dVu       = InData.dVu;
    rData.dMu       = InData.dMu;
    rData.dft       = InData.dft;
    rData.dfb       = InData.dfb;
    rData.dVn       = InData.dVn;
    rData.dPhi      = InData.dPhi;
    rData.dPhiVn    = InData.dPhiVn;
    rData.dVc       = InData.dVc;
    rData.dVs       = InData.dVs;
    rData.dVp       = InData.dVp;
    rData.dAv_s     = InData.dAv_s;
    rData.dAv_s_min = InData.dAv_s_min;
    rData.dAv_s_req = InData.dAv_s_req;
    rData.ds        = InData.ds;
    rData.ds_max    = InData.ds_max;
    rData.bOK       = InData.bOK;
    rData.dV_Rd_max = InData.dV_Rd_max;
    rData.dVu_pVn_Rat = InData.dVu_pVn_Rat;

    rData.dRho_w = InData.dRho_w;
    rData.dRho_w_min = InData.dRho_w_min;
    rData.ds_l_max = InData.ds_l_max;

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertCums_EngineToCivil(const PSC_CUMS_BASE& InData, T_CUMS_PSC_BASE_IRS& rData)//IRS
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu       = InData.dPu;
    rData.dVu       = InData.dVu;
    rData.dMu       = InData.dMu;
    rData.dft       = InData.dft;
    rData.dfb       = InData.dfb;
    rData.dVn       = InData.dVn;
    rData.dPhi      = InData.dPhi;
    rData.dPhiVn    = InData.dPhiVn;
    rData.dVc       = InData.dVc;
    rData.dVs       = InData.dVs;
    rData.dVp       = InData.dVp;
    rData.dAv_s     = InData.dAv_s;
    rData.dAv_s_min = InData.dAv_s_min;
    rData.dAv_s_req = InData.dAv_s_req;
    rData.ds        = InData.ds;
    rData.ds_max    = InData.ds_max;
    rData.bOK       = InData.bOK;
    rData.dV_Rd_max = InData.dV_Rd_max;
    rData.dVu_pVn_Rat = InData.dVu_pVn_Rat;

    rData.dRho_w = InData.dRho_w;
    rData.dRho_w_min = InData.dRho_w_min;
    rData.ds_l_max = InData.ds_l_max;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmt_EngineToCivil(const PSC_CRMT_BASE& InData, T_CRMT_PSC_BASE& rData)
{
    rData.bCHK                 = InData.bCHK;
    rData.LcomK                = InData.LcomK;
    rData.iMax                 = InData.iMax;
    rData.iKind                = InData.iKind;
    rData.iLSTerm              = InData.iLSTerm;
    rData.dPu                  = InData.dPu;
    rData.dVu                  = InData.dVu;
    rData.dMu                  = InData.dMu;
    rData.dVn                  = InData.dVn;
    rData.dTn                  = InData.dTn;
    rData.dTu                  = InData.dTu;
    rData.dft                  = InData.dft;
    rData.dfb                  = InData.dfb;
    rData.dPhi                 = InData.dPhi;
    rData.dPhiVn               = InData.dPhiVn;
    rData.dPhi_t               = InData.dPhi_t;
    rData.dPhi_tTn             = InData.dPhi_tTn;
    rData.dAl                  = InData.dAl;
    rData.dAl_req              = InData.dAl_req;
    rData.dAl_min              = InData.dAl_min;
    rData.dAt                  = InData.dAt;
    rData.dAt_req              = InData.dAt_req;
    rData.dTcr                 = InData.dTcr;
    rData.bOK                  = InData.bOK;
    rData.dV_Rd_max            = InData.dV_Rd_max;
    rData.dComb_rat            = InData.dComb_rat;
    rData.ds_t                 = InData.ds_t;
    rData.dst_max_rep          = InData.dst_max_rep;
    rData.drho_st              = InData.dRho_st;
    rData.drho_st_min          = InData.dRho_st_min;
    rData.bPSC_BOX             = InData.bPSC_BOX;
    rData.dV_Rd_c              = InData.dV_Rd_c;
    if ( rData.bPSC_BOX )
    {
        rData.dT_Rd_c = InData.dT_Rd_c;
        rData.dComb_Rd_c_rat       = InData.dComb_Rd_c_rat;
        rData.bComb_Rd_c_rat_check = InData.bComb_Rd_c_rat_check;
    }
    else
    {
        rData.dT_Rd_c = InData.TorPart[1].dT_Rd_c_i;
        rData.dComb_Rd_c_rat       = InData.TorPart[1].dComb_Rd_c_4NotBox;
        rData.bComb_Rd_c_rat_check = InData.TorPart[1].bComb_rat_chk4NotBox;
    }
    rData.dV_Rd_s              = InData.dV_Rd_s;
    rData.dT_Rd_s              = InData.dT_Rd_s;
    rData.dV_Rd_s_rat          = InData.dV_Rd_s_rat;
    rData.dT_Rd_s_rat          = InData.dT_Rd_s_rat;
    rData.dT_Rd_c_rat          = InData.dT_Rd_c_rat;
    rData.dV_Rd_c_rat          = InData.dV_Rd_c_rat;
    rData.dT_Rd_lr             = InData.dT_Rd4;
    rData.dT_Rd_lr_rat         = InData.dT_Rd_lr_rat;


    return TRUE;
}
BOOL CDgn_PSCManager::ConvertCrmt_EngineToCivil(const PSC_CRMT_BASE& InData, T_CRMT_PSC_BASE_IRC& rData)//IRC
{
    rData.bCHK      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iMax      = InData.iMax;
    rData.iKind     = InData.iKind;
    rData.iLSTerm   = InData.iLSTerm;
    rData.dPu       = InData.dPu;
    rData.dVu       = InData.dVu;
    rData.dMu       = InData.dMu;
    rData.dVn       = InData.dVn;
    rData.dTn       = InData.dTn;
    rData.dTu       = InData.dTu;
    rData.dft       = InData.dft;
    rData.dfb       = InData.dfb;
    rData.dPhi      = InData.dPhi;
    rData.dPhiVn    = InData.dPhiVn;
    rData.dPhi_t    = InData.dPhi_t;
    rData.dPhi_tTn  = InData.dPhi_tTn;
    rData.dAl       = InData.dAl;
    rData.dAl_req   = InData.dAl_req;
    rData.dAl_min   = InData.dAl_min;
    rData.dAt       = InData.dAt;
    rData.dAt_req   = InData.dAt_req;
    rData.dTcr      = InData.dTcr;
    rData.bOK       = InData.bOK;
    rData.dV_Rd_max = InData.dV_Rd_max;
    rData.dComb_rat = InData.dComb_rat;
    rData.ds_t      = InData.ds_t;
    rData.dst_max_rep = InData.dst_max_rep;

    return TRUE;
}
BOOL CDgn_PSCManager::ConvertCrmt_EngineToCivil(const PSC_CRMT_BASE& InData, T_CRMT_PSC_BASE_IRS& rData)//IRS
{
    rData.bCHK      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iMax      = InData.iMax;
    rData.iKind     = InData.iKind;
    rData.iLSTerm   = InData.iLSTerm;
    rData.dPu       = InData.dPu;
    rData.dVu       = InData.dVu;
    rData.dMu       = InData.dMu;
    rData.dVn       = InData.dVn;
    rData.dTn       = InData.dTn;
    rData.dTu       = InData.dTu;
    rData.dft       = InData.dft;
    rData.dfb       = InData.dfb;
    rData.dPhi      = InData.dPhi;
    rData.dPhiVn    = InData.dPhiVn;
    rData.dPhi_t    = InData.dPhi_t;
    rData.dPhi_tTn  = InData.dPhi_tTn;
    rData.dAl       = InData.dAl;
    rData.dAl_req   = InData.dAl_req;
    rData.dAl_min   = InData.dAl_min;
    rData.dAt       = InData.dAt;
    rData.dAt_req   = InData.dAt_req;
    rData.dTcr      = InData.dTcr;
    rData.bOK       = InData.bOK;
    rData.dV_Rd_max = InData.dV_Rd_max;
    rData.dComb_rat = InData.dComb_rat;
    rData.ds_t      = InData.ds_t;
    rData.dst_max_rep = InData.dst_max_rep;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFatg_EngineToCivil(const PSC_FATG_BASE& InData, T_FATG_PSC_BASE& rData)
{
    rData.bCHK          = InData.bCHK;
    // 	rData.dSigLcom_max  = InData.dSig_max ; // [Unit=Stress] 
    // 	rData.dSigLcom_min  = InData.dSig_min ; // [Unit=Stress] 
    // 	rData.dVuLcom_max   = InData.dVu_max  ; // [Unit=Force] 
    // 	rData.dVuLcom_min   = InData.dVu_min  ; // [Unit=Force] 


    rData.dStressRatio   = InData.dSigMax_dfcd;
    rData.dStressLimit01 = InData.dLimit01;
    rData.dStressLimit02 = InData.dLimit02;

    //rData.dShearRatio01  = InData.dLimit02; //  σc,max/νfcd,fat
    rData.dShearRatio02   = InData.dRatio_Vu; //  VEd,min/VEd,max 
    rData.dStressLimit03  = InData.dLimit03;

    //rData.dRatio;
    rData.bOK = (InData.bChkFat1 && InData.bChkFat2);

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFatg_EngineToCivil(const PSC_FATG_BASE& InData, T_FATG_PSC_BASE_IRC& rData)//IRC
{
    rData.bCHK          = InData.bCHK;
    // 	rData.dSigLcom_max  = InData.dSig_max ; // [Unit=Stress] 
    // 	rData.dSigLcom_min  = InData.dSig_min ; // [Unit=Stress] 
    // 	rData.dVuLcom_max   = InData.dVu_max  ; // [Unit=Force] 
    // 	rData.dVuLcom_min   = InData.dVu_min  ; // [Unit=Force] 


    rData.dStressRatio   = InData.dSigMax_dfcd;
    rData.dStressLimit01 = InData.dLimit01;
    rData.dStressLimit02 = InData.dLimit02;

    //rData.dShearRatio01  = InData.dLimit02; //  σc,max/νfcd,fat
    rData.dShearRatio02   = InData.dRatio_Vu; //  VEd,min/VEd,max 
    rData.dStressLimit03  = InData.dLimit03;

    //rData.dRatio;
    rData.bOK = (InData.bChkFat1 && InData.bChkFat2);

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFatg_EngineToCivil(const PSC_FATG_BASE& InData, T_FATG_PSC_BASE_IRS& rData)//IRS
{
    rData.bCHK          = InData.bCHK;
    // 	rData.dSigLcom_max  = InData.dSig_max ; // [Unit=Stress] 
    // 	rData.dSigLcom_min  = InData.dSig_min ; // [Unit=Stress] 
    // 	rData.dVuLcom_max   = InData.dVu_max  ; // [Unit=Force] 
    // 	rData.dVuLcom_min   = InData.dVu_min  ; // [Unit=Force] 


    rData.dStressRatio   = InData.dSigMax_dfcd;
    rData.dStressLimit01 = InData.dLimit01;
    rData.dStressLimit02 = InData.dLimit02;

    //rData.dShearRatio01  = InData.dLimit02; //  σc,max/νfcd,fat
    rData.dShearRatio02   = InData.dRatio_Vu; //  VEd,min/VEd,max 
    rData.dStressLimit03  = InData.dLimit03;

    //rData.dRatio;
    rData.bOK = (InData.bChkFat1 && InData.bChkFat2);

    return TRUE;
}

// PSC composite
BOOL CDgn_PSCManager::ConvertFysgComp_EngineToCivil(const PSC_FYSG_BASE& InData, T_FYSG_PSC_COMP_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StageK  = InData.StageK;
    rData.StepK  = InData.StepK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dALW   = InData.dALW;
    rData.dRatio = InData.dRatio;
    rData.bOK    = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycmComp_EngineToCivil(const PSC_FYCM_BASE& InData, T_FYCM_PSC_COMP_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFT     = InData.dFT;
    rData.dFB     = InData.dFB;
    rData.dFTL    = InData.dFTL;
    rData.dFBL    = InData.dFBL;
    rData.dFTR    = InData.dFTR;
    rData.dFBR    = InData.dFBR;
    rData.dFMAX   = InData.dFMAX;
    rData.dALW    = InData.dALW;
    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsgComp_EngineToCivil(const PSC_FPSG_BASE& InData, T_FPSG_PSC_COMP_BASE& rData)
{
    rData.bCHK  = InData.bCHK;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpmsComp_EngineToCivil(const PSC_FPMS_BASE& InData, T_FPMS_PSC_COMP_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    return TRUE;
}


BOOL CDgn_PSCManager::ConvertBdcwComp_EngineToCivil(const PSC_BDCW_BASE& InData, BOOL bOk_Cr, T_BDCW_PSC_COMP_BASE& rData)
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFSS   = InData.dFSS;
    rData.dEpsilon_sm = InData.dEpsilon_sm;
    rData.dEpsilon_cm = InData.dEpsilon_cm;
    rData.dEpsi_sm_Epsi_cm = InData.dEpsilon_sm_cm;
    rData.dSr_max = InData.dSr_max;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.dWk    = InData.dWk;
    rData.dWmax  = InData.dWa;
    rData.dRatio = InData.dW_rat;
    rData.bOK    = InData.bOK;
    rData.dCrack_Asmin = InData.dCrack_Asmin;
    rData.dAst         = InData.dAst;

    rData.bOK_Cr         = bOk_Cr;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcwComp_EngineToCivil_zero(const PSC_BDCW_BASE& InData, BOOL bOk_Zero, T_BDCW_PSC_COMP_BASE& rData)
{
    rData.bCHK   = InData.bCHK_Zero;
    rData.LcomK  = InData.LcomK;
    rData.iMax   = InData.iMax;
    rData.iKind  = InData.iKind;
    rData.iLSTerm= InData.iLSTerm;
    rData.dPu    = InData.dPu;
    rData.dMu    = InData.dMu;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFSS   = InData.dFSS;
    rData.dEpsilon_sm = InData.dEpsilon_sm;
    rData.dEpsilon_cm = InData.dEpsilon_cm;
    rData.dEpsi_sm_Epsi_cm = InData.dEpsilon_sm_cm;
    rData.dSr_max = InData.dSr_max;
    rData.ds_use = InData.ds_use;
    rData.ds_max = InData.ds_max;
    rData.dWk    = InData.dWk;
    rData.dWmax  = InData.dWa;
    rData.dRatio = InData.dRat_zero;
    rData.bOK    = InData.bOK;
    rData.dCrack_Asmin = InData.dCrack_Asmin;
    rData.dAst         = InData.dAst;

    rData.bOk_Zero         = bOk_Zero;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcwComp_EngineToCivil_Asmin(const PSC_BDCW_BASE& InData, BOOL bOk_Asmin, T_BDCW_PSC_COMP_BASE& rData)
{
    //Asmin(최소철근량 검토 여부)
    rData.bCHK              =InData.bCHK_Asmin;
    rData.LcomK             =InData.LcomK;
    rData.iKind             =InData.iKind;
    rData.iMax              =InData.iMax;
    rData.iLSTerm           =InData.iLSTerm;
    rData.dPu               =0.0;
    rData.dMu               =0.0;
    rData.dFT		            =0.0;
    rData.dFB		            =0.0;
    rData.dft               =InData.dft;
    rData.dfb               =InData.dfb;
    rData.dFSS	            =0.0;
    rData.dEpsilon_sm       =0.0;
    rData.dEpsilon_cm       =0.0;
    rData.dEpsi_sm_Epsi_cm  =0.0;
    rData.dSr_max           =0.0;
    rData.ds_use	          =0.0;
    rData.ds_max	          =0.0;
    rData.dWk               =0.0;
    rData.dWmax             =0.0;
    rData.dRatio            =InData.dRatio_Asmin;
    rData.dCrack_Asmin      =0.0;
    rData.dAst              =InData.dfct_eff;//dfct_eff 값 확인용
    rData.bOK               =InData.bOK_Asmin;
    rData.bOK_Cr            =FALSE;
    rData.bOk_Zero          =FALSE;

    return TRUE;
}


BOOL CDgn_PSCManager::ConvertBdcrComp_EngineToCivil(const PSC_FRCR_D&    InData, T_BDCR_PSC_COMP_BASE& rData)
{
    rData.bCHK    = InData.bChk;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;

    rData.dMu     = InData.dMu;
    rData.dMn     = InData.dMn;
    rData.dMn_cal = InData.dMn_cal;
    rData.dphi    = InData.dphi;
    rData.dphiMn  = InData.dphiMn;
    rData.dRatM   = InData.dRatM;
    rData.dc_neu  = InData.dc_neu;
    rData.dalpha  = InData.dalpha;
    rData.dbeta1  = InData.dbeta1;
    rData.da      = InData.da;
    rData.dPe     = InData.dPe;
    rData.dPe_t   = InData.dPe_t;
    rData.dAps    = InData.dAps;
    rData.dAps_t  = InData.dAps_t;
    rData.dAst    = InData.dAst;
    rData.dAsc    = InData.dAsc;
    rData.dRho_pt = InData.dRho_pt;
    rData.dRho_st = InData.dRho_st;
    rData.dRho_sc = InData.dRho_sc;
    rData.dRho_ps = InData.dRho_ps;
    rData.ddpt    = InData.ddpt;
    rData.ddst    = InData.ddst;
    rData.ddsc    = InData.ddsc;
    //
    //rData.dphiMcr    = ;   
    rData.dphiMn_mo  = InData.d12Mcr;
    //rData.dRho_p_max = ;
    //rData.dRho_p_min = ;

    // KSCE-LSD15
    rData.dCmax    = InData.dCmax;
    rData.dAs_max    = InData.dAs_max;
    rData.dAs_min    = InData.dAs_min;
    rData.dz_sh      = InData.dz_sh;
    rData.dCc_gr     = InData.dCc_gr;   // 거더 압축력
    rData.dCc_sl     = InData.dCc_sl;   // 슬래브 압축력
    // EC2 PSC Composite (Interface Shear - Beta)
    rData.dForce_T   = InData.dForce_T; // 전체인장응력(Rebar + Tendon)
    rData.dTst_sl    = InData.dTst_sl;  // 슬래브인장응력(Rebar)

    rData.bOK = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCumsComp_EngineToCivil(const PSC_CUMS_BASE& InData, T_CUMS_PSC_COMP_BASE& rData)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.iLSTerm = InData.iLSTerm;
    rData.dPu       = InData.dPu;
    rData.dVu       = InData.dVu;
    rData.dMu       = InData.dMu;
    rData.dMuMax    = InData.dMr;
    rData.dft       = InData.dft;
    rData.dfb       = InData.dfb;
    rData.dVn       = InData.dVn;
    rData.dPhi      = InData.dPhi;
    rData.dPhiVn    = InData.dPhiVn;
    rData.dVc       = InData.dVc;
    rData.dVs       = InData.dVs;
    rData.dVp       = InData.dVp;
    rData.dAv_s     = InData.dAv_s;
    rData.dAv_s_min = InData.dAv_s_min;
    rData.dAv_s_req = InData.dAv_s_req;
    rData.ds        = InData.ds;
    rData.ds_max    = InData.ds_max;
    rData.bOK       = InData.bOK;
    rData.dV_Rd_max = InData.dV_Rd_max;
    rData.dVu_pVn_Rat = InData.dVu_pVn_Rat;

    rData.dRho_w = InData.dRho_w;
    rData.dRho_w_min = InData.dRho_w_min;
    rData.ds_l_max = InData.ds_l_max;

    rData.dM_pre = InData.BdcwBaseComp.dM_non;
    rData.dV_pre = InData.BdcwBaseComp.dVc1;


    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmtComp_EngineToCivil(const PSC_CRMT_BASE& InData, T_CRMT_PSC_COMP_BASE& rData)
{
    rData.bCHK      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.iMax      = InData.iMax;
    rData.iKind     = InData.iKind;
    rData.iLSTerm   = InData.iLSTerm;
    rData.dPu       = InData.dPu;
    rData.dVu       = InData.dVu;
    rData.dMu       = InData.dMu;
    rData.dVn       = InData.dVn;
    rData.dTn       = InData.dTn;
    rData.dTu       = InData.dTu;
    rData.dft       = InData.dft;
    rData.dfb       = InData.dfb;
    rData.dPhi      = InData.dPhi;
    rData.dPhiVn    = InData.dPhiVn;
    rData.dPhi_t    = InData.dPhi_t;
    rData.dPhi_tTn  = InData.dPhi_tTn;
    rData.dAl       = InData.dAl;
    rData.dAl_req   = InData.dAl_req;
    rData.dAl_min   = InData.dAl_min;
    rData.dAt       = InData.dAt;
    rData.dAt_req   = InData.dAt_req;
    rData.dTcr      = InData.dTcr;
    rData.bOK       = InData.bOK;
    rData.dV_Rd_max = InData.dV_Rd_max;
    rData.dComb_rat = InData.dComb_rat;
    rData.ds_t      = InData.ds_t;
    rData.dst_max_rep = InData.dst_max_rep;
    rData.drho_st      = InData.dRho_st;
    rData.drho_st_min  = InData.dRho_st_min;

    rData.dM_pre = InData.dM_non;
    rData.dV_pre = InData.dVc1;

    rData.bPSC_BOX = InData.bPSC_BOX;
    rData.dV_Rd_c = InData.dV_Rd_c;
    if ( rData.bPSC_BOX )
    {
        rData.dT_Rd_c = InData.dT_Rd_c;
        rData.dComb_Rd_c_rat = InData.dComb_Rd_c_rat;
        rData.bComb_Rd_c_rat_check = InData.bComb_Rd_c_rat_check;
    }
    else
    {
        rData.dT_Rd_c = InData.TorPart[1].dT_Rd_c_i;
        rData.dComb_Rd_c_rat = InData.TorPart[1].dComb_Rd_c_4NotBox;
        rData.bComb_Rd_c_rat_check = InData.TorPart[1].bComb_rat_chk4NotBox;
    }

    rData.dV_Rd_s = InData.dV_Rd_s;
    rData.dT_Rd_s = InData.dT_Rd_s;
    rData.dV_Rd_s_rat = InData.dV_Rd_s_rat;
    rData.dT_Rd_s_rat = InData.dT_Rd_s_rat;
    rData.dT_Rd_c_rat  = InData.dT_Rd_c_rat;
    rData.dV_Rd_c_rat  = InData.dV_Rd_c_rat;
    rData.dT_Rd_lr     = InData.dT_Rd4;
    rData.dT_Rd_lr_rat = InData.dT_Rd_lr_rat;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_EngineToCivilSNiP(const PSC_FYSG_BASE &InData, T_FYSG_SNIP_BASE &rData)
{
    rData.dALW = InData.dALW;
    rData.bOK  = InData.bOK;
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypc_EngineToCivilSNiP(const PSC_FYPC_D &InData, T_FYPC_SNIP &rData)
{
    rData.dAFDL1 = InData.dAFDL1;
    rData.dAFDL2 = InData.dAFDL2;
    rData.dAFDL3 = InData.dAFDL3;
    rData.dAFLL1 = InData.dAFLL1;
    rData.dAFLL2 = InData.dAFLL2;
    rData.bOK  = InData.bOK;
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_EngineToCivilSNiP(const PSC_FYCM_BASE &InData, T_FYCM_SNIP_BASE &rData)
{
    rData.dALW = InData.dALW;
    rData.bOK  = InData.bOK;
    return TRUE;
}
BOOL CDgn_PSCManager::ConvertFpsg_EngineToCivilSNiP(const PSC_FPSG_BASE &InData, T_FPSG_SNIP_BASE &rData)
{
    rData.dAFP = InData.dAFP;
    rData.bOK  = InData.bOK;
    return TRUE;
}
BOOL CDgn_PSCManager::ConvertFpms_EngineToCivilSNiP(const PSC_FPMS_BASE &InData, T_FPMS_SNIP_BASE &rData)
{
    rData.dAFP = InData.dAFP;
    rData.bOK  = InData.bOK;
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCivilSNiP(const PSC_BDCW_SNIP_BASE &InData, T_BDCW_SNIP_BASE &rData)
{
    rData.bCHK               = InData.bCHK;
    rData.LcomK              = InData.LcomK;
    rData.iKind              = InData.iKind;
    rData.iMax               = InData.iMax;
    rData.iLSTerm            = InData.iLSTerm;

    rData.dPu                = InData.dPu;
    rData.dQu                = InData.dQu;
    rData.dMuy               = InData.dMuy;
    rData.dMuz               = InData.dMuz;
    rData.dTu                = InData.dTu;
    rData.dSigma_top         = InData.dSigma_top;
    rData.dSigma_bot         = InData.dSigma_bot;
    rData.dSigma_bx          = InData.dSigma_bx;
    rData.dSigma_mt          = InData.dSigma_mt;
    rData.dSigma_mc          = InData.dSigma_mc;
    rData.dRb_mc2            = InData.dRb_mc2;
    rData.dSigma_bt          = InData.dSigma_bt;
    rData.dSigma_bt_Alw      = InData.dSigma_bt_Alw;
    rData.dSigma_bc_min      = InData.dSigma_bc_min;
    rData.dSigma_bc_min_Alw  = InData.dSigma_bc_min_Alw;
    rData.dSigma_mt_Alw      = InData.dSigma_mt_Alw;
    rData.dAlpha_cr          = InData.dAlpha_cr;
    rData.dDelta_cr          = InData.dDelta;
    rData.dRatW              = InData.dWrat;
    rData.bOK                = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcr_EngineToCivilSNiP(const PSC_FRCR_D         &InData, T_BDCR_SNIP_BASE &rData)
{
    rData.bCHK    = InData.bChk;
    rData.LcomK   = InData.LcomK;
    rData.iKind   = InData.iKind;
    rData.iMax    = InData.iMax;
    rData.iLSTerm = InData.iLSTerm;
    rData.dMu     = InData.dMu;
    rData.dMr     = InData.dphiMn;
    rData.dRatM   = InData.dRatM;
    rData.dXi     = InData.SNiP.dXi;
    rData.dXiy    = InData.SNiP.dXiy;
    rData.bOK     = InData.bOK;
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCums_EngineToCivilSNiP(const PSC_CUMS_SNIP_BASE &InData, T_CUMS_SNIP_BASE &rData)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.LcomQuK = InData.LcomQuK;
    rData.iKind   = InData.iKind;
    rData.iMax    = InData.iMax;
    rData.iLSTerm = InData.iLSTerm;
    rData.bIsSLS  = InData.bIsSLS ? TRUE : FALSE;
    rData.dPu     = InData.dPu;
    rData.dQu     = InData.dQu;
    rData.dQuSLS  = InData.dQuSLS;
    rData.dMu     = InData.dMu;
    rData.dQr     = InData.dQr;
    rData.dMr     = InData.dMrsh;
    rData.dRatQ   = InData.dQ_rat;
    rData.dRatM   = InData.dMsh_rat;
    rData.bOK     = InData.bShrOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmt_EngineToCivilSNiP(const PSC_CUMS_SNIP_BASE &InData, T_CRMT_SNIP_BASE &rData)
{
    rData.bCHK      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.LcomQuK   = InData.LcomQuK;
    rData.iKind     = InData.iKind;
    rData.iMax      = InData.iMax;
    rData.iLSTerm   = InData.iLSTerm;
    rData.bIsSLS    = InData.bIsSLS ? TRUE : FALSE;
    rData.dPu       = InData.dPu;
    rData.dQu       = InData.dQu;
    rData.dQuSLS    = InData.dQuSLS;
    rData.dMu       = InData.dMu;
    rData.dTu       = InData.dTu;
    rData.dTr       = InData.dTr;
    rData.dRatT     = InData.dT_rat;
    rData.dIntRatTM = InData.dTM_Inter_lim;
    rData.dIntRatTQ = InData.dTQ_Inter_lim;
    rData.bOK       = InData.bTorOK;
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFati_EngineToCivilSNiP(const PSC_FATI_SNIP_BASE &InData, T_FATI_SNIP_BASE &rData)
{
    rData.bCHK         = InData.bCHK;
    rData.LcomK        = InData.LcomK;
    rData.nKind        = InData.iKind;
    rData.nMax         = InData.iMax;
    rData.nLSTerm      = InData.iLSTerm;
    rData.dPu          = InData.dPu;
    rData.dQu          = InData.dQu;
    rData.dMuy         = InData.dMuy;
    rData.dMuz         = InData.dMuz;
    rData.dTu          = InData.dTu;

    rData.dPu_tp       = InData.dPu_tp;
    rData.dMuy_tp      = InData.dMuy_tp;
    rData.dMuz_tp      = InData.dMuz_tp;
    rData.dPu_ts       = InData.dPu_ts;
    rData.dMuy_ts      = InData.dMuy_ts;
    rData.dMuz_ts      = InData.dMuz_ts;
    rData.dPu_per      = InData.dPu_per;
    rData.dMuy_per     = InData.dMuy_per;
    rData.dMuz_per     = InData.dMuz_per;

    rData.dSigma_pmax  = InData.dSigma_pmax;
    rData.dSigma_pmin  = InData.dSigma_pmin;
    rData.dm_ap1_Rp    = InData.dm_ap1_Rp;
    rData.dSigma_bcmax = InData.dSigma_bcmax;
    rData.dSigma_bcmin = InData.dSigma_bcmin;
    rData.dm_b1_Rb     = InData.dm_b1_Rb;
    rData.dfbc         = InData.dfbc;
    rData.dffs         = InData.dffs;
    rData.dm_as1_Rs    = InData.dm_as1_Rs;
    rData.dfacs        = InData.dfacs;
    rData.dRatF        = InData.dRatF;
    rData.bOK          = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_EngineToCivilAS(const _FYSG_PSC_LRFD_BASE &InData, T_FYSG_PSC_AS_BASE &rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StageK = InData.StagK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dfci   = InData.dfci;
    rData.dALW   = InData.dALW;
    rData.bOK    = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertTran_EngineToCivilAS(const _TRAN_PSC_BASE &InData, T_TRAN_PSC_AS_BASE &rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StageK = InData.StageK;
    rData.StepK  = InData.Step;
    rData.LcomK  = InData.LcomK;
    rData.dft    = InData.dft;
    rData.dfb    = InData.dfb;
    for ( int i=0; i<4; ++i )
    {
        rData.dStress[i] = InData.dStress[i];
    }
    rData.dfcp   = InData.dfcp;
    rData.dAlwStr= InData.dAlwStr;
    rData.bOK    = InData.bOK;
    //rData.bRectStress = InData.bRectStress;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_EngineToCivilAS(const _FYCM_PSC_LRFD_BASE &InData, T_FYCM_PSC_AS_BASE &rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.nKind = InData.iKind;
    rData.nMax  = InData.iMax;
    rData.dFT   = InData.dFT;
    rData.dFB   = InData.dFB;
    rData.dFTL  = InData.dFTL;
    rData.dFBL  = InData.dFBL;
    rData.dFTR  = InData.dFTR;
    rData.dFBR  = InData.dFBR;
    rData.dFMAX = InData.dFMAX;
    rData.dALW  = InData.dALW;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsg_EngineToCivilAS(const _FPMT_PSC_LRFD_BASE &InData, T_FPMT_JTG_BASE    &rData)
{

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_EngineToCivilAS(const _FPMT_PSC_LRFD_BASE &InData, T_FPMS_PSC_BASE    &rData)
{
    rData.bCHK    = InData.bCHK;
    rData.LcomK   = InData.LcomK;
    rData.iMax    = InData.iMax;
    rData.iKind   = InData.iKind;
    rData.dFP1    = InData.dFP1;
    rData.dFP2    = InData.dFP2;
    rData.dFP3    = InData.dFP3;
    rData.dFP4    = InData.dFP4;
    rData.dFP5    = InData.dFP5;
    rData.dFP6    = InData.dFP6;
    rData.dFP7    = InData.dFP7;
    rData.dFP8    = InData.dFP8;
    rData.dFP9    = InData.dFP9;
    rData.dFP10   = InData.dFP10;
    rData.dFMAX   = InData.dFMAX;
    rData.dAFP    = InData.dAFP;
    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypc_EngineToCivilAS(const _FYPC_AASHTO& InData, T_FYPC_PSC_AS& rData)
{
    rData.bCHK      = InData.bCHK;
    rData.nLoadType = InData.nLoadType;
    rData.dFDL1	    = InData.dFDL1;
    rData.dFDL2	    = InData.dFDL2;
    rData.dFDL3	    = InData.dFDL3;
    rData.dFLL1	    = InData.dFLL1;
    rData.dFLL2	    = InData.dFLL2;
    rData.dAFDL1	  = InData.dAFDL1;
    rData.dAFDL2	  = InData.dAFDL2;
    rData.dAFDL3	  = InData.dAFDL3;
    rData.dAFLL1	  = InData.dAFLL1;
    rData.dAFLL2	  = InData.dAFLL2;
    rData.bOK       = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCivilAS(const _BDCW_PSC_LRFD_BASE &InData, T_BDCW_PSC_AS_BASE &rData)
{
    rData.bCHK   = InData.LInf.bCHK;
    rData.LcomK  = InData.LInf.LcomK;
    rData.nMax   = InData.LInf.nMax;
    rData.nKind  = InData.LInf.nKind;
    rData.dMu    = InData.ForB.dForce[4];
    // Check, in Engine
    rData.dftop    = InData.ForB.dFT;
    rData.dfbot    = InData.ForB.dFB;
    rData.dfta     = InData.dfta;
    rData.dfta2    = InData.dfta2;
    rData.dfs      = InData.dFSS;
    rData.dfsa     = InData.dfs_lim;
    rData.dft      = InData.ForB.dft_DL;
    rData.dfb      = InData.ForB.dfb_DL;
    rData.dft_LL   = InData.ForB.dft_LL;
    rData.dfb_LL   = InData.ForB.dfb_LL;
    rData.dftop_sl = InData.ForP2.dFT;
    rData.dfbot_sl = InData.ForP2.dFB;
    rData.dft_DL_sl= InData.ForP2.dft_DL;
    rData.dfb_DL_sl= InData.ForP2.dfb_DL;
    rData.dft_LL_sl= InData.ForP2.dft_LL;
    rData.dfb_LL_sl= InData.ForP2.dfb_LL;
    rData.dfcp     = InData.dfc_p;
    rData.dfcp_sl  = InData.SlabLC.dfsr;
    rData.dfcp_DL_sl = InData.SlabPL.dfsr;
    rData.dfs_sl   = InData.SlabLC.dfsr_s_lim;
    rData.dfs_DL_sl= InData.SlabPL.dfsr_d_lim;
    rData.bIsChkSlabPL = InData.bIsChkSlabPL;
    rData.ds       = InData.ds_use;
    rData.dsa      = InData.ds_max;
    rData.bChk_fta = InData.bChk_fta;
    rData.bChk_8_6_2_3 = InData.bChk_Crack_8_6_2_3;
    rData.bOK      = InData.bOK;

    rData.ForBe.dFxx = InData.CrPropD.dPu;
    rData.ForBe.dMuy = InData.CrPropD.dMu;
    rData.ForST.dFxx = InData.CrPropD.dPu_st;
    rData.ForST.dMuy = InData.CrPropD.dMu_st;
    rData.ForLT.dFxx = InData.CrPropD.dPu_lt;
    rData.ForLT.dMuy = InData.CrPropD.dMu_lt;
    //InData.CrPropD.dPu_c = InData.CrPropD.dPu_st + InData.CrPropD.dPu_lt;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcr_EngineToCivilAS(const _BDCY_PSC_LRFD_BASE &InData, T_BDCR_PSC_AS_BASE &rData)
{
    rData.bCHK    = InData.bCHK;
    rData.dMu     = InData.dMuy;
    rData.LcomK   = InData.LcomK;
    rData.nMax    = InData.iMax;
    rData.nKind   = InData.iKind;
    rData.dpMn    = InData.drMuy;
    rData.dRatM   = InData.dpMn_133Mcr_Rat;
    rData.dMn_min = InData.dFacMcr;
    rData.dAst    = InData.dAs;
    rData.dAst_min= InData.dAs_min;
    rData.bOK     = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCums_EngineToCivilAS(const _CUMS_AASHTO_BASE   &InData, T_SHRR_PSC_AS_BASE &rData)
{
    rData.bCHK      = InData.bCHK;
    rData.LcomK     = InData.LcomK;
    rData.nMax      = InData.iMax;
    rData.nKind     = InData.iKind;
    memcpy(rData.dForceTP, InData.dForceTP, sizeof(rData.dForceTP));
    rData.dFu       = InData.dNu;
    rData.dVu       = InData.dVu;
    rData.dTu       = InData.dTu;
    rData.dVeq      = InData.dVeq;
    rData.dMu       = InData.dMu;
    rData.dpVn      = InData.dPhiVn;
    rData.dVnc      = InData.dVc;
    rData.dVns      = InData.dVs;
    rData.dRatV     = InData.dV_rat;
    rData.dPv       = InData.dVp;
    rData.dVu_max   = InData.dVn2;
    rData.dVu_min   = InData.dVu_min;;
    rData.dAsv      = InData.dAv_s;
    rData.dAsv_min  = InData.dAv_s_min;
    rData.bOK       = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmt_EngineToCivilAS(const _STCM_AASHTO_BASE   &InData, T_TORR_PSC_AS_BASE &rData)
{
    const _CRMS_AASHTO_BASE &TorIn = InData.Tor;
    rData.bCHK      = TorIn.bCHK;
    rData.LcomK     = TorIn.LcomK;
    rData.nMax      = TorIn.iMax;
    rData.nKind     = TorIn.iKind;
    memcpy(rData.dForceTP, TorIn.dForceTP, sizeof(rData.dForceTP));
    rData.dFu       = TorIn.dNu;
    rData.dVu       = TorIn.dVu;
    rData.dMu       = TorIn.dMu;
    rData.dTu       = TorIn.dTu;
    rData.dpVn      = TorIn.dVn;
    rData.dpTn      = TorIn.dPhi_tTn;
    rData.d025pTcr  = TorIn.dTu_chk;
    rData.dTcr      = TorIn.dTcr;
    rData.dRatT     = TorIn.dT_rat;
    rData.dRatVT    = TorIn.dCombRat;
    rData.dTus      = TorIn.dTn;
    rData.bOK       = TorIn.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFysg_EngineToCivilBS(const PSC_FYSG_BASE &InData, T_FYSG_PSC_BS_BASE &rData)
{
    rData.bCHK   = InData.bCHK;
    rData.StageK = InData.StageK;
    rData.dFT    = InData.dFT;
    rData.dFB    = InData.dFB;
    rData.dFTL   = InData.dFTL;
    rData.dFBL   = InData.dFBL;
    rData.dFTR   = InData.dFTR;
    rData.dFBR   = InData.dFBR;
    rData.dFMAX  = InData.dFMAX;
    rData.dfci   = InData.dfci;
    rData.dALW   = InData.dALW;
    rData.bOK    = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_EngineToCivilBS(const PSC_FYCM_BASE &InData, T_FYCM_PSC_BS_BASE &rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.nKind = InData.iKind;
    rData.nMax  = InData.iMax;
    rData.dFT   = InData.dFT;
    rData.dFB   = InData.dFB;
    rData.dFTL  = InData.dFTL;
    rData.dFBL  = InData.dFBL;
    rData.dFTR  = InData.dFTR;
    rData.dFBR  = InData.dFBR;
    rData.dFMAX = InData.dFMAX;
    rData.dALW  = InData.dALW;
    rData.bOK   = InData.bOK;
	rData.dfsa  = InData.dfsa;
	rData.dfs  = InData.dfs;
    rData.dDepthFac = InData.dDepthFac;
    rData.dFlexTensC3 = InData.dFlexTensC3;
    rData.dAddReinStress = InData.dAddReinStress;
    rData.dTensConcArea = InData.dTensConcArea;
    rData.dTensRebarArea = InData.dTensRebarArea;
    rData.dJointStressLimit = InData.dJointStressLimit;
    rData.bJointExist = InData.bJointExist;
    rData.bJointOK = InData.bJointOK;
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpsg_EngineToCivilBS(const PSC_FPSG_BASE &InData, T_FPSG_PSC_BASE &rData)
{
    rData.bCHK  = InData.bCHK;
    rData.StageK= InData.StageK;
    rData.StepK = InData.StepK;
    rData.iKind = InData.iKind;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFpms_EngineToCivilBS(const PSC_FPMS_BASE &InData, T_FPMS_PSC_BASE &rData)
{
    rData.bCHK  = InData.bCHK;
    rData.LcomK = InData.LcomK;
    rData.iMax  = InData.iMax;
    rData.dFP1  = InData.dFP1;
    rData.dFP2  = InData.dFP2;
    rData.dFP3  = InData.dFP3;
    rData.dFP4  = InData.dFP4;
    rData.dFP5  = InData.dFP5;
    rData.dFP6  = InData.dFP6;
    rData.dFP7  = InData.dFP7;
    rData.dFP8  = InData.dFP8;
    rData.dFP9  = InData.dFP9;
    rData.dFP10 = InData.dFP10;
    rData.dFMAX = InData.dFMAX;
    rData.dAFP  = InData.dAFP;
    rData.bOK   = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypc_EngineToCivilBS(const PSC_FYPC_D &InData, T_FYPC_PSC_BS &rData)
{
    rData.dFpu   = InData.dFpu;
    rData.dAFDL1 = InData.dAFDL1;
    rData.bOK  = InData.bOK;
    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcw_EngineToCivilBS(const PSC_BDCW_BASE &InData, T_BDCW_PSC_BS_BASE &rData)
{
    rData.bCHK = InData.bCHK;

    rData.LcomK = InData.LcomK;
    rData.iMax  = InData.iMax;

    rData.dPu = InData.dPu;
    rData.dMu = InData.dMu;
    rData.dMg = InData.dMg;
    rData.dMq = InData.dMq;
    rData.dfb = InData.dfb;
    rData.dft = InData.dft;
    rData.dEpsilon_s = InData.dEtas;
    rData.dEpsilon_1 = InData.dEta1;
    rData.dEpsilon_m = InData.dEtamm;
    rData.dcnom = InData.dcnom;
    rData.dc_neu = InData.dc_neu;
    rData.dh    = InData.dh;
    rData.dbt   = InData.dbt;
    rData.dap   = InData.dadash;
    rData.dAst  = InData.dAten;
    rData.dCW   = InData.dWk;
    rData.dDgnCW= InData.dWa;
    rData.dRatio= InData.dW_rat;

    rData.bOK = rData.dRatio > 1.0 ? FALSE : TRUE;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertBdcr_EngineToCivilBS(const PSC_FRCR_D &InData, T_BDCR_PSC_BS_BASE &rData)
{
    rData.bCHK   = InData.bChk;
    rData.LcomK  = InData.LcomK;
    rData.nMax   = InData.iMax;
    rData.nKind  = InData.iKind;
    rData.dMu    = InData.dphiMn;
    rData.dMy    = InData.dMu;
    rData.dRatM  = InData.dRatM;
    rData.bOK    = InData.bOK;
    //rData.dMu_min= InData.

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCums_EngineToCivilBS(const PSC_CUMS_BASE &InData, T_SHRR_PSC_BS_BASE &rData)
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.nMax   = InData.iMax;
    rData.nKind  = InData.iKind;

    rData.dVu    = InData.dPhiVn;
    rData.dVmax  = InData.dVMax;
    rData.dRatV  = InData.dVu_pVn_Rat;

    memcpy(rData.dForceTP, InData.dForceTP, sizeof(rData.dForceTP));

    rData.dFx    = InData.dPu;
    rData.dVz    = InData.dVu;
    rData.dMy    = InData.dMu;

    rData.ds     = InData.ds;
    rData.dVc    = InData.dVc;
    rData.dVp    = InData.dVp;
    rData.dMcr   = InData.dMcr;
    rData.dAv_s  = InData.dAv_s;
    rData.dAv_s_req = InData.dAv_s_req;
    rData.dAsv_req  = rData.dAv_s_req*rData.ds;
    rData.dJointVu  = InData.dJointVu;
    rData.bJoint	= InData.bJoint;
    rData.dVmax     = InData.dVMax;
    rData.dAsv      = InData.dAv;
    rData.dAsv_min  = InData.dAv_s_min*rData.ds;
    rData.bUncrack  = InData.bUncrack;
    rData.bOK       = InData.bOK;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertCrmt_EngineToCivilBS(const PSC_CRMT_BASE &InData, T_TORR_PSC_BS_BASE &rData)
{
    rData.bCHK   = InData.bCHK;
    rData.LcomK  = InData.LcomK;
    rData.nMax   = InData.iMax;
    rData.nKind  = InData.iKind;

    rData.dVu    = InData.dPhiVn;
    rData.dTu    = InData.dPhi_tTn;
    rData.dv     = InData.dv_s;
    rData.dv_t   = InData.dv_t;
    rData.dv_tmin= InData.dv_tmin;
    rData.dv_tu  = InData.dv_tu;
    rData.dy1    = InData.dy1;
    rData.bBox   = InData.bPSC_BOX;
    rData.dv_tu_550   = InData.dv_t_chk; // vtu(y1/550)
    rData.dAst_st     = InData.dAst_st;
    rData.dAst_st_req = InData.dTorLinkLimit;
    rData.dAsl_sl     = InData.dAsl_sl;
    rData.dAsl_sl_req = InData.dTorLongLimit;
    rData.dFx       = InData.dPu;
    rData.dVz       = InData.dVu;
    rData.dMy       = InData.dMu;
    rData.dT        = InData.dTu;
    rData.bOK       = InData.bOK;

    return TRUE;
}


//////////////////////////////////////////////////////////////////////////

BOOL CDgn_PSCManager::ConvertFycm_ToEngineAASHTO(const T_FYCM_D& InData, T_FYCM_AASHTO& rData)
{
    for ( int i=0; i<4; i++ )
    {
        rData.FycmBase[i].bCHK  = InData.FycmBase[i].bCHK;
        rData.FycmBase[i].LcomK = InData.FycmBase[i].LcomK;
        rData.FycmBase[i].iMax  = InData.FycmBase[i].iMax;
        rData.FycmBase[i].iKind = InData.FycmBase[i].iKind;
        rData.FycmBase[i].dFT   = InData.FycmBase[i].dFT;
        rData.FycmBase[i].dFB   = InData.FycmBase[i].dFB;
        rData.FycmBase[i].dFTL  = InData.FycmBase[i].dFTL;
        rData.FycmBase[i].dFBL  = InData.FycmBase[i].dFBL;
        rData.FycmBase[i].dFTR  = InData.FycmBase[i].dFTR;
        rData.FycmBase[i].dFBR  = InData.FycmBase[i].dFBR;
        rData.FycmBase[i].dFMAX = InData.FycmBase[i].dFMAX;
        rData.FycmBase[i].dALW  = InData.FycmBase[i].dALW;

        rData.FycmBase[i].dFT_PS   = InData.FycmBase[i].dFT_PS;
        rData.FycmBase[i].dFB_PS   = InData.FycmBase[i].dFB_PS;
        rData.FycmBase[i].dFTL_PS  = InData.FycmBase[i].dFTL_PS;
        rData.FycmBase[i].dFBL_PS  = InData.FycmBase[i].dFBL_PS;
        rData.FycmBase[i].dFTR_PS  = InData.FycmBase[i].dFTR_PS;
        rData.FycmBase[i].dFBR_PS  = InData.FycmBase[i].dFBR_PS;
        rData.FycmBase[i].dFMAX_PS = InData.FycmBase[i].dFMAX_PS;

        rData.FycmBase[i].dRatio   = InData.FycmBase[i].dRatio;
        rData.FycmBase[i].bOK   = InData.FycmBase[i].bOK;
        rData.FycmBase[i].bLTerm = InData.FycmBase[i].bLTerm;
    }

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFycm_ToCivilData(const T_FYCM_AASHTO& InData, T_FYCM_D& rData)
{
    for ( int i=0; i<4; i++ )
    {
        rData.FycmBase[i].bCHK  = InData.FycmBase[i].bCHK;
        rData.FycmBase[i].LcomK = InData.FycmBase[i].LcomK;
        rData.FycmBase[i].iMax  = InData.FycmBase[i].iMax;
        rData.FycmBase[i].iKind = InData.FycmBase[i].iKind;
        rData.FycmBase[i].dFT   = InData.FycmBase[i].dFT;
        rData.FycmBase[i].dFB   = InData.FycmBase[i].dFB;
        rData.FycmBase[i].dFTL  = InData.FycmBase[i].dFTL;
        rData.FycmBase[i].dFBL  = InData.FycmBase[i].dFBL;
        rData.FycmBase[i].dFTR  = InData.FycmBase[i].dFTR;
        rData.FycmBase[i].dFBR  = InData.FycmBase[i].dFBR;
        rData.FycmBase[i].dFMAX = InData.FycmBase[i].dFMAX;
        rData.FycmBase[i].dALW  = InData.FycmBase[i].dALW;

        rData.FycmBase[i].dFT_PS   = InData.FycmBase[i].dFT_PS;
        rData.FycmBase[i].dFB_PS   = InData.FycmBase[i].dFB_PS;
        rData.FycmBase[i].dFTL_PS  = InData.FycmBase[i].dFTL_PS;
        rData.FycmBase[i].dFBL_PS  = InData.FycmBase[i].dFBL_PS;
        rData.FycmBase[i].dFTR_PS  = InData.FycmBase[i].dFTR_PS;
        rData.FycmBase[i].dFBR_PS  = InData.FycmBase[i].dFBR_PS;
        rData.FycmBase[i].dFMAX_PS = InData.FycmBase[i].dFMAX_PS;

        rData.FycmBase[i].dRatio   = InData.FycmBase[i].dRatio;
        rData.FycmBase[i].bOK     = InData.FycmBase[i].bOK;
        rData.FycmBase[i].bLTerm  = InData.FycmBase[i].bLTerm;

    }
    return TRUE;
}

CString CDgn_PSCManager::Get_SatgeName4PSC(int StagK)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    CString strStageName = _T("");

    T_STAG_D StagD;
    if ( pDoc->m_pAttrCtrl->GetStag(StagK, StagD) ) strStageName = StagD.StageName;

    return strStageName;
}

CStringW CDgn_PSCManager::Get_LcomName4PSC(int iDgnLcomK, BOOL bOriginalLcomKey/* = FALSE*/)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    CStringW strLcomName = _T("");
    T_LCOM_D LcomD;
    T_LCOM_K LcomK = 0;

    if ( !bOriginalLcomKey )
    {
        LcomK = 0;
        if ( pDoc->m_pPostCtrl->GetPCDesign()->GetOrgLcomK(iDgnLcomK, LcomK) )
        {
            if ( pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomK, LcomD) ) strLcomName = LcomD.LoadCombName;
        }
    }
    else
    {
        LcomK = iDgnLcomK;
        if ( pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomK, LcomD) ) strLcomName = LcomD.LoadCombName;
    }

    return strLcomName;
}

BOOL CDgn_PSCManager::Get_TndnInfo4PSC(T_TDNA_K TdnaK, CString& strTendonName, int& nLoadType)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    strTendonName = _T("");
    T_TDNA_D TdnaD;
    T_TDNT_D TdntD;
    if ( !pDoc->m_pAttrCtrl->GetTdna(TdnaK, TdnaD) ) return FALSE;
    if ( !pDoc->m_pAttrCtrl->GetTdnt(TdnaD.TendonTypeKey, TdntD) ) return FALSE;

    strTendonName = TdnaD.TendonName;
    nLoadType = TdntD.nLoadType;

    return TRUE;
}

BOOL CDgn_PSCManager::ConvertFypc_CivilToEngine(T_TDNA_K TdnaK, const T_FYPC_IRS& InData, PSC_FYPC_D& rData, BOOL bReport)
{
    rData.bCHK   = InData.bCHK;
    rData.dFDL1  = InData.dFDL1;
    rData.dFDL2  = InData.dFDL2;
    rData.dFDL3  = InData.dFDL3;
    rData.dFLL1  = InData.dFLL1;
    rData.dFLL2  = InData.dFLL2;
    rData.dAFDL1 = InData.dAFDL1;
    rData.dAFDL2 = InData.dAFDL2;
    rData.dAFDL3 = InData.dAFDL3;
    rData.dAFLL1 = InData.dAFLL1;
    rData.dAFLL2 = InData.dAFLL2;
    rData.bOK    = InData.bOK;

    if ( bReport )
    {
        rData.TdnaK = TdnaK;
        CString strTendonName = _T("");
        int nLoadType=0;
        if ( Get_TndnInfo4PSC(TdnaK, strTendonName, nLoadType) )
        {
            rData.nLoadType = nLoadType;
            rData.strTendonName = strTendonName;
        }
    }
    return TRUE;
}