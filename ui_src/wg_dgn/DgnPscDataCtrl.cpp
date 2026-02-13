// DgnPscDataCtrl.cpp: implementation of the CDgnPscDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "..\DgnEngine\Src\DGNe_RUS\DgnCalc_Rus_PSC_Struct.h"
#include "DgnPscDataCtrl.h"

#include "CRCForceCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "Dgn_PSCManager.h"
#include "..\..\dgnengine\src\DgnBase/KSCEEnumDef.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnPscDataCtrl::CDgnPscDataCtrl()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_pPscManager = new CDgn_PSCManager;
}

CDgnPscDataCtrl::~CDgnPscDataCtrl()
{
	if(!m_pPscManager) 
	{
		delete m_pPscManager;
		m_pPscManager = NULL;
	}
}


void CDgnPscDataCtrl::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl, int iDgnCode)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

	m_iDgnCode = iDgnCode;
	// Set Code Unit.
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit;

	switch (m_iDgnCode)
	{
	case EUROCODE2_2_05_PSC:
	case IRC_112_2011_PSC:
	case IRC_112_2020_PSC:
	case AS_5100_5_17_PSC:
	case TMH07_3_1989:
	case BS5400_90_PSC:
    case BD_44_15_PSC:
    case CS_455_PSC:
	case CSA_S6_10:
	case CSA_S6_14:
	case CSA_S6_19:
	case KDS_24_14_21_2022_PSC:
	case KSCE_LSD15_PSC:
	case SNiP_20503_84_PSC:
	case SP_35_13330_11_PSC:
	case KSCE_LSD15_RATING:
    case CS_454_20_PSC_RATING:
	case IRS_PSC:
	case ABNT_NBR_6118_2023_PSC:
		{
			CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
			CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
			m_pDataCtrl->Set_UnitIndex(CodeUnit);
			break;
		}
	case SNiP_20503_84_PSC_MKS:
	case SP_35_13330_11_PSC_MKS:
		{
			CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_CM;
			CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_KG;
			m_pDataCtrl->Set_UnitIndex(CodeUnit);
			break;
		}
	case AASHTO_LRFD08:
	case AASHTO_LRFD12:
	case AASHTO_LRFD14:
	case AASHTO_LRFD16:
    case AASHTO_LRFD17:
    case AASHTO_LRFD20:
	case AASHTO_LRFD24:
	case AASHTO_LRFD12_PSC_RATING:
    case AASHTO_LRFD19_PSC_RATING:
	case AREMA_23:
		{
			CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
			CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_KIP;
			//m_pDataCtrl->Get_CurrUnitIndex(CodeUnit);
			m_pDataCtrl->Set_UnitIndex(CodeUnit);
			break;
		}
	default: ASSERT(0); break;      
	}

    switch ( m_iDgnCode )
    {
    case AASHTO_LRFD12_PSC_RATING:
    case AASHTO_LRFD19_PSC_RATING:
    case KSCE_LSD15_RATING:
    case CS_454_20_PSC_RATING:
        Make_BridgeLoadRatingDgnData();
        break;
    default:
        Make_PscDgnData();
        break;
    }

	InitialData(iDgnCode);
}

BOOL CDgnPscDataCtrl::InitialData(int iDgnCode)
{
//	if(!m_pPscManager->InitialData(DGNE_CODETYPE_PSC, iDgnCode)) return FALSE;

	T_PSCD_D PscdD; PscdD.Initialize();
    T_BLRS_D BlrsD; BlrsD.Initialize();
    T_RAKR_D RakrD; RakrD.Initialize();

    switch ( iDgnCode )
    {
    case AASHTO_LRFD12_PSC_RATING:
        m_pDoc->m_pAttrCtrl->GetBlrs(BlrsD);
        PscdD.iDgnCode          = AASHTO_LRFD12;
        PscdD.iTndnKind         = BlrsD.iTendonType;
        PscdD.ifpsMethod        = BlrsD.iMn_Method;
        PscdD.iConstructionType = (BlrsD.iVn_Method==1)? 0 : 1; // Non-Segmental Bridge 로만 적용- 왜??
        if ( !m_pPscManager->InitialData(DGNE_CODETYPE_PSC, PscdD.iDgnCode) ) return FALSE;
        ConvertPscParamData(PscdD, m_SPscdD);
        break;
    case AASHTO_LRFD19_PSC_RATING:
        m_pDoc->m_pAttrCtrl->GetBlrs(BlrsD);
        PscdD.iDgnCode          = AASHTO_LRFD17;
        PscdD.iTndnKind         = BlrsD.iTendonType;
        PscdD.ifpsMethod        = BlrsD.iMn_Method;
        PscdD.iConstructionType = (BlrsD.iVn_Method==1)? 0 : 1; // Non-Segmental Bridge 로만 적용- 왜??
        if ( !m_pPscManager->InitialData(DGNE_CODETYPE_PSC, PscdD.iDgnCode) ) return FALSE;
        ConvertPscParamData(PscdD, m_SPscdD);
        break;
    case KSCE_LSD15_RATING:
        m_pDoc->m_pAttrCtrl2->GetRakr(RakrD);
        PscdD.iDgnCode          = KSCE_LSD15_PSC;
        PscdD.iTndnKind         = RakrD.iTndnKind;
        PscdD.nCalcCcType       = RakrD.nCalcCcType;
        PscdD.bRefRebar = RakrD.bRefRebar;
        PscdD.bUserInputData = RakrD.bUserInputData;
        for ( int i = 0; i < 4; ++i )
        {
            PscdD.dPhi_c[i]  = RakrD.dPhi_c[i];
            PscdD.dPhi_s[i]  = RakrD.dPhi_s[i];
            PscdD.dPhi_ps[i] = RakrD.dPhi_ps[i];
        }
        if ( !m_pPscManager->InitialData(DGNE_CODETYPE_PSC, PscdD.iDgnCode) ) return FALSE;
        ConvertPscParamData(PscdD, m_SPscdD);
        break;
    case CS_454_20_PSC_RATING:
        T_ASPM_D AspmD; AspmD.Initialize();
        m_pDoc->m_pAttrCtrl->GetAspm(AspmD);
        PscdD.iDgnCode       = CS_455_PSC; //BD_44_15_PSC;
        //PscdD.iTndnKind      = AspmD.iTndnKind;
        PscdD.nCalcCcType    = 1;
        PscdD.bUserInputData = AspmD.bUserInput;
        if ( !m_pPscManager->InitialData(DGNE_CODETYPE_PSC, PscdD.iDgnCode) ) return FALSE;
        ConvertPscParamData(PscdD, m_SPscdD);
        ConvertPscParamASPM(AspmD, m_SPscdD);
        if ( !m_pPscManager->InitialData(DGNE_CODETYPE_PSC, PscdD.iDgnCode) ) return FALSE;
        break;
    default:
        m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
        if ( !m_pPscManager->InitialData(DGNE_CODETYPE_PSC, iDgnCode) ) { ASSERT(0); return FALSE; }
        ConvertPscParamData(PscdD, m_SPscdD);
        break;
    }

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FysgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();			

	PSC_FYSG_BASE OutData;	
	BOOL bCalcOK=TRUE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
	int i=0, j=0;
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		for(j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FysgBase[kk].bCHK==FALSE)  continue;

            InData.dDuration = rData.FysgBase[kk].dDuration;
			InData.CalcD.iFysgCalcType = j; // 0=Compression, 1=Tension    
			m_pPscManager->ConvertFysg_CivilToEngine(ElemK, rData.FysgBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowbleStressBeforeLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFysg_EngineToCivil(OutData, rData.FysgBase[kk]);
		}
	}   
	return bCalcOK;    
}

BOOL CDgnPscDataCtrl::Get_FypcData(T_TDNA_K TdnaK, T_FYPC_AASHTO& rData)
{
	_PSC_TNDN_TDNA TndnTdna;
	if(!Get_PscTndnTdna(TdnaK, TndnTdna)) return FALSE;

	PSC_TNDN_TDNA InTdnaD;      
	ConvertPscCalcTdnaData(&TndnTdna, InTdnaD);
	PSC_DATA_TNDN InData;
	InData.PscdD = m_SPscdD;
	InData.TdnaD = InTdnaD;

	PSC_FYPC_D OutData;

	m_pPscManager->ConvertFypc_CivilToEngine(TdnaK, rData, OutData);
	BOOL bOK = m_pPscManager->Calc_PscAllowbleStressTendonLoss(InData, OutData);
	m_pPscManager->ConvertFypc_EngineToCivil(OutData, rData);

	if(!bOK) return FALSE;

	return TRUE; 
}

BOOL CDgnPscDataCtrl::Get_FypcDataBS(T_TDNA_K TdnaK, T_FYPC_PSC_BS& rData)
{
    _PSC_TNDN_TDNA TndnTdna;
    if ( !Get_PscTndnTdna(TdnaK, TndnTdna) ) return FALSE;

    PSC_TNDN_TDNA InTdnaD;
    ConvertPscCalcTdnaData(&TndnTdna, InTdnaD);
    PSC_DATA_TNDN InData;
    InData.PscdD = m_SPscdD;
    InData.TdnaD = InTdnaD;

    PSC_FYPC_D OutData;
    rData.dFpu = InData.TdnaD.dfpu;
    m_pPscManager->ConvertFypc_CivilToEngineBS(TdnaK, rData, OutData);
    BOOL bOK = m_pPscManager->Calc_PscAllowbleStressTendonLoss(InData, OutData);
    m_pPscManager->ConvertFypc_EngineToCivilBS(OutData, rData);

    if ( !bOK ) return FALSE;

    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FycmData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC& rData)
{
	PSC_FYCM_BASE OutData;

	BOOL bCalcOK=TRUE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	int i=0, j=0;
	BOOL bPreTndn[2]={TRUE,TRUE};
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{    
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FycmBase[i*2+j].bCHK==FALSE)  continue;
			//double dalComf, dalTenf;

			InData.CalcD.iFycmCalcType = j; // 0=Compression, 1=Tension
			m_pPscManager->ConvertFycm_CivilToEngine(ElemK, rData.FycmBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowbleStressAfterLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFycm_EngineToCivil(OutData, rData.FycmBase[kk]);      
		}
	}

	return bCalcOK;  
}

BOOL CDgnPscDataCtrl::Get_FpsgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	PSC_FPSG_BASE OutData;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpsgBase[kk].bCHK==FALSE)  continue;

            InData.dDuration = rData.FpsgBase[kk].dDuration;
			InData.CalcD.iFpsgCalcType = j;
			m_pPscManager->ConvertFpsg_CivilToEngine(ElemK, rData.FpsgBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowblePrincipalStressBeforeLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFpsg_EngineToCivil(OutData, rData.FpsgBase[kk]);      
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC& rData)
{
	PSC_FPMS_BASE OutData;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpmsBase[kk].bCHK==FALSE)  continue;

			InData.CalcD.iFpmsCalcType = j;
			m_pPscManager->ConvertFpms_CivilToEngine(ElemK, rData.FpmsBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowblePrincipalStressAfterLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFpms_EngineToCivil(OutData, rData.FpmsBase[kk]);      
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcwData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC& rData, PSC_BDCW_D &rRes)
{
	BOOL bCalcOK=TRUE;

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);    
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<2 ; j++) // 0=Top, 1=Bot
		{    
			BOOL bOK_Zero = TRUE;
			BOOL bOk_Cr   = TRUE;
			BOOL bOk_Asmin= TRUE;

			int kk = i*2+j;      
			PSC_BDCW_BASE OutData = rRes.BdcwBase[kk]; 
			int nType = j;
			InData.CalcD.iBdcwCalcType = nType;
			m_pPscManager->ConvertBdcw_CivilToEngine(ElemK, rData.BdcwBase[kk], OutData);

			if(m_iDgnCode == KSCE_LSD15_PSC || m_iDgnCode == KDS_24_14_21_2022_PSC )
			{
				int iExposureClass = (j==0)? InData.iExposureClass[0] : InData.iExposureClass[1];
				int iLcomType = OutData.iKind;
				BOOL bPost    = InData.TndnD.bPost;
				BOOL bBonded  = InData.TndnD.bBonded;

				bOk_Cr = Check_Wmax_4Crack_Grade(iExposureClass, iLcomType, bPost,  bBonded, bOK_Zero);
				//if(!bOk_Cr && !bOK_Zero) continue;
			}

			if (rRes.BdcwBase[kk].bIsCalcEffProp==TRUE)
			{
				OutData.bIsCalcEffProp = rRes.BdcwBase[kk].bIsCalcEffProp;
				OutData.dc_neu         = rRes.BdcwBase[kk].dc_neu;
				OutData.dArea_cr       = rRes.BdcwBase[kk].dArea_cr;
				OutData.dZcen_cr       = rRes.BdcwBase[kk].dZcen_cr;
				OutData.dIy_cr         = rRes.BdcwBase[kk].dIy_cr;
				OutData.dArea_s_cr     = rRes.BdcwBase[kk].dArea_s_cr;
				OutData.dZcen_s_cr     = rRes.BdcwBase[kk].dZcen_s_cr;
				OutData.dIy_s_cr       = rRes.BdcwBase[kk].dIy_s_cr;
				OutData.dAc_eff        = rRes.BdcwBase[kk].dAc_eff;
			}

			BOOL bCalcType = m_pPscManager->Calc_PscCrackCheck(nType, InData, OutData);

            if ( m_iDgnCode == EUROCODE2_2_05_PSC ) bOk_Cr = OutData.bCHK_Crack; //DB 추가 관련으로 임시로 작업

			m_pPscManager->ConvertBdcw_EngineToCivil(OutData, bOk_Cr, rData.BdcwBase[kk]);
			m_pPscManager->ConvertBdcw_EngineToCivil_zero(OutData, bOK_Zero, rData.BdcwBaseZero[kk]);
			m_pPscManager->ConvertBdcw_EngineToCivil_Asmin(OutData, bOk_Asmin, rData.BdcwBaseAsmin[kk]);

			rRes.BdcwBase[kk] = OutData;
		}
	}


	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcrData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC& rData)
{
	PSC_FRCR_D OutData;

	BOOL bCalcOK=TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.BdcrBase[kk].bCHK==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);

			int nType = j;
			InData.CalcD.iBdcrCalcType = nType;
			m_pPscManager->ConvertBdcr_CivilToEngine(ElemK, rData.BdcrBase[kk], OutData);
			if(!m_pPscManager->Calc_PscFlexureStrength(nType, InData, OutData)) continue;  
			m_pPscManager->ConvertBdcr_EngineToCivil(OutData, rData.BdcrBase[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, double adMr[4], T_CUMS_PSC& rData)
{
	PSC_CUMS_BASE OutData;

	BOOL bCalcOK = TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];  

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.CumsBase[kk].bCHK==FALSE) continue;

			OutData.dMr = adMr[kk];
			int nType = j;
			m_pPscManager->ConvertCums_CivilToEngine(ElemK, rData.CumsBase[kk], OutData);
			if(!m_pPscManager->Calc_PscShearDesign(nType, InData, OutData)) continue;      
			m_pPscManager->ConvertCums_EngineToCivil(OutData, rData.CumsBase[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_FatgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FATG_PSC& rData)
{
	PSC_FATG_BASE OutData;

	BOOL bCalcOK = TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];  

		for(int j=0 ; j<2 ; j++) //Top, Bottom
		{
			int kk = i*2+j;
			if(rData.FatgBase[kk].bCHK==FALSE) continue;

			//OutData.dMr = adMr[kk];
			int nType = j;
			m_pPscManager->ConvertFatg_CivilToEngine(ElemK, rData.FatgBase[kk], OutData); // kk: 0=I_top 1=I_Bot 2=J_top 3=J_Bot
			if(!m_pPscManager->Calc_PscFatigeCheck(nType, InData, OutData)) continue;      
			m_pPscManager->ConvertFatg_EngineToCivil(OutData, rData.FatgBase[kk]);      
		}
	}

	return bCalcOK;
}



BOOL CDgnPscDataCtrl::Get_CrmtData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CRMT_PSC& rData)
{
	PSC_CRMT_BASE OutData;

	BOOL bCalcOK = TRUE;  

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];  

		for(int j=0 ; j<3 ; j++) //Max, Min
		{
			int kk = i*3+j;
			if(rData.CrmtBase[kk].bCHK==FALSE) continue;

			int nType = j;
			m_pPscManager->ConvertCrmt_CivilToEngine(ElemK, rData.CrmtBase[kk], OutData);
			if(!m_pPscManager->Calc_PscTorsionDesign(nType, InData, OutData)) continue;    
			m_pPscManager->ConvertCrmt_EngineToCivil(OutData, rData.CrmtBase[kk]);
		}
	}
	return bCalcOK;
}
//////////////////////////////////////////////////////////////////
//IRC112 START
/////////////////////////////////////////////////////////////////
BOOL CDgnPscDataCtrl::Get_FysgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_IRC& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	PSC_FYSG_BASE OutData;	

	BOOL bCalcOK=TRUE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
	int i=0, j=0;
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		for(j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FysgBase[kk].bCHK==FALSE)  continue;

			T_STAG_K StagK = rData.FysgBase[kk].StageK;
			T_STAG_D StagD; StagD.Initialize();
			if(m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD))
			{
				T_ELFM_D DayActive; DayActive.Initialize();
				pDoc->m_pPostCtrl->GetElfmDgn(StagK, ElemK, DayActive);  // Stage의 시작과 끝에서 재료 날짜.			
				InData.dDuration = StagD.dDuration + DayActive.dStartMatAge;
			}

			InData.CalcD.iFysgCalcType = j; // 0=Compression, 1=Tension    
			m_pPscManager->ConvertFysg_CivilToEngine(ElemK, rData.FysgBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowbleStressBeforeLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFysg_EngineToCivil(OutData, rData.FysgBase[kk]);
		}
	}   
	return bCalcOK;    
}


BOOL CDgnPscDataCtrl::Get_FycmData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_IRC& rData)
{
	PSC_FYCM_BASE OutData;

	BOOL bCalcOK=TRUE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	int i=0, j=0;
	BOOL bPreTndn[2]={TRUE,TRUE};
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{    
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FycmBase[i*2+j].bCHK==FALSE)  continue;
			//double dalComf, dalTenf;

			InData.CalcD.iFycmCalcType = j; // 0=Compression, 1=Tension
			m_pPscManager->ConvertFycm_CivilToEngine(ElemK, rData.FycmBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowbleStressAfterLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFycm_EngineToCivil(OutData, rData.FycmBase[kk]);      
		}
	}

	return bCalcOK;  
}

BOOL CDgnPscDataCtrl::Get_FpsgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC_IRC& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	PSC_FPSG_BASE OutData;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpsgBase[kk].bCHK==FALSE)  continue;

			T_STAG_K StagK = rData.FpsgBase[kk].StageK;
			T_STAG_D StagD; StagD.Initialize();
			if(m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD))
			{
				T_ELFM_D DayActive; DayActive.Initialize();
				pDoc->m_pPostCtrl->GetElfmDgn(StagK, ElemK, DayActive);  // Stage의 시작과 끝에서 재료 날짜.			
				InData.dDuration = StagD.dDuration + DayActive.dStartMatAge;
			}

			InData.CalcD.iFpsgCalcType = j;
			m_pPscManager->ConvertFpsg_CivilToEngine(ElemK, rData.FpsgBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowblePrincipalStressBeforeLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFpsg_EngineToCivil(OutData, rData.FpsgBase[kk]);      
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC_IRC& rData)
{
	PSC_FPMS_BASE OutData;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpmsBase[kk].bCHK==FALSE)  continue;

			InData.CalcD.iFpmsCalcType = j;
			m_pPscManager->ConvertFpms_CivilToEngine(ElemK, rData.FpmsBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowblePrincipalStressAfterLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFpms_EngineToCivil(OutData, rData.FpmsBase[kk]);      
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcwData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_IRC& rData, PSC_BDCW_D &rRes)
{
	PSC_BDCW_BASE OutData; 

	BOOL bCalcOK=TRUE;

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);    
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<2 ; j++) // 0=Top, 1=Bot
		{    
			BOOL bOK_Zero = TRUE;
			BOOL bOk_Cr   = TRUE;

			int kk = i*2+j;      
			int nType = j;
			InData.CalcD.iBdcwCalcType = nType;
			m_pPscManager->ConvertBdcw_CivilToEngine(ElemK, rData.BdcwBase[kk], OutData);

			if(m_iDgnCode == KSCE_LSD15_PSC || m_iDgnCode == KDS_24_14_21_2022_PSC )
			{
				int iExposureClass = (j==0)? InData.iExposureClass[0] : InData.iExposureClass[1];
				int iLcomType = OutData.iKind;
				BOOL bPost    = InData.TndnD.bPost;
				BOOL bBonded  = InData.TndnD.bBonded;

				bOk_Cr = Check_Wmax_4Crack_Grade(iExposureClass, iLcomType, bPost,  bBonded, bOK_Zero);
				//if(!bOk_Cr && !bOK_Zero) continue;
			}

			BOOL bCalcType = m_pPscManager->Calc_PscCrackCheck(nType, InData, OutData);

			m_pPscManager->ConvertBdcw_EngineToCivil(OutData, bOk_Cr, rData.BdcwBase[kk]);
			m_pPscManager->ConvertBdcw_EngineToCivil_zero(OutData, bOK_Zero, rData.BdcwBaseZero[kk]);

			rRes.BdcwBase[kk] = OutData;
		}
	}


	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcrData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_IRC& rData)
{
	PSC_FRCR_D OutData;

	BOOL bCalcOK=TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.BdcrBase[kk].bCHK==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);

			int nType = j;
			InData.CalcD.iBdcrCalcType = nType;
			m_pPscManager->ConvertBdcr_CivilToEngine(ElemK, rData.BdcrBase[kk], OutData);
			if(!m_pPscManager->Calc_PscFlexureStrength(nType, InData, OutData)) continue;  
			m_pPscManager->ConvertBdcr_EngineToCivil(OutData, rData.BdcrBase[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, double adMr[4], T_CUMS_PSC_IRC& rData)
{
	PSC_CUMS_BASE OutData;

	BOOL bCalcOK = TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];  

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.CumsBase[kk].bCHK==FALSE) continue;

			OutData.dMr = adMr[kk];
			int nType = j;
			m_pPscManager->ConvertCums_CivilToEngine(ElemK, rData.CumsBase[kk], OutData);
			if(!m_pPscManager->Calc_PscShearDesign(nType, InData, OutData)) continue;      
			m_pPscManager->ConvertCums_EngineToCivil(OutData, rData.CumsBase[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_FatgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FATG_PSC_IRC& rData)
{
	PSC_FATG_BASE OutData;

	BOOL bCalcOK = TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];  

		for(int j=0 ; j<2 ; j++) //Top, Bottom
		{
			int kk = i*2+j;
			if(rData.FatgBase[kk].bCHK==FALSE) continue;

			//OutData.dMr = adMr[kk];
			int nType = j;
			m_pPscManager->ConvertFatg_CivilToEngine(ElemK, rData.FatgBase[kk], OutData); // kk: 0=I_top 1=I_Bot 2=J_top 3=J_Bot
			if(!m_pPscManager->Calc_PscFatigeCheck(nType, InData, OutData)) continue;      
			m_pPscManager->ConvertFatg_EngineToCivil(OutData, rData.FatgBase[kk]);      
		}
	}

	return bCalcOK;
}



BOOL CDgnPscDataCtrl::Get_CrmtData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CRMT_PSC_IRC& rData)
{
	PSC_CRMT_BASE OutData;

	BOOL bCalcOK = TRUE;  

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];  

		for(int j=0 ; j<3 ; j++) //Max, Min
		{
			int kk = i*3+j;
			if(rData.CrmtBase[kk].bCHK==FALSE) continue;

			int nType = j;
			m_pPscManager->ConvertCrmt_CivilToEngine(ElemK, rData.CrmtBase[kk], OutData);
			if(!m_pPscManager->Calc_PscTorsionDesign(nType, InData, OutData)) continue;    
			m_pPscManager->ConvertCrmt_EngineToCivil(OutData, rData.CrmtBase[kk]);
		}
	}
	return bCalcOK;
}





///////////////////////////////////////////////////////////////
//IRC112 END
//////////////////////////////////////////////////////////////
// PSC Composite

BOOL CDgnPscDataCtrl::Get_FysgData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_COMP& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	PSC_FYSG_BASE OutData;	  

	BOOL bCalcOK=TRUE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
	int i=0, j=0;
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;    

		for(j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FysgGrBase[kk].bCHK==FALSE)  continue;

			// 거더
			InData.nGirder = PSC_DATA_POSI::Girder;
            InData.dDuration = rData.FysgGrBase[kk].dDuration;
            int nType = j; // 0=Compression, 1=Tension    
			InData.CalcD.iFysgCalcType = nType;
			m_pPscManager->ConvertFysgComp_CivilToEngine(ElemK, rData.FysgGrBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowbleStressBeforeLoss(nType, InData, OutData)) continue;
			m_pPscManager->ConvertFysgComp_EngineToCivil(OutData, rData.FysgGrBase[kk]);

			if(rData.FysgSlBase[kk].bCHK==FALSE)  continue;

			// 슬래브
			InData.nGirder = PSC_DATA_POSI::Slab;;
            InData.dDuration = rData.FysgSlBase[kk].dDuration;
			m_pPscManager->ConvertFysgComp_CivilToEngine(ElemK, rData.FysgSlBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowbleStressBeforeLoss(nType, InData, OutData)) continue;
			m_pPscManager->ConvertFysgComp_EngineToCivil(OutData, rData.FysgSlBase[kk]);

		}
	}   

	return bCalcOK;    
}


BOOL CDgnPscDataCtrl::Get_FycmData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_COMP& rData)
{
	PSC_FYCM_BASE OutData;  

	BOOL bCalcOK=TRUE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	int i=0, j=0;
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{    
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		// 거더
		InData.nGirder = PSC_DATA_POSI::Girder;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FycmGrBase[i*2+j].bCHK==FALSE)  continue;

			int nType = j; // 0=Compression, 1=Tension
			InData.CalcD.iFycmCalcType = nType;
			m_pPscManager->ConvertFycmComp_CivilToEngine(ElemK, rData.FycmGrBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowbleStressAfterLoss(nType, InData, OutData)) continue;
			m_pPscManager->ConvertFycmComp_EngineToCivil(OutData, rData.FycmGrBase[kk]);      


		}

		// 슬래브
		InData.nGirder = PSC_DATA_POSI::Slab;;
		for(j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FycmSlBase[i*2+j].bCHK==FALSE)  continue;

			int nType = j; // 0=Compression, 1=Tension
			InData.CalcD.iFycmCalcType = nType;
			m_pPscManager->ConvertFycmComp_CivilToEngine(ElemK, rData.FycmSlBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowbleStressAfterLoss(nType, InData, OutData)) continue;
			m_pPscManager->ConvertFycmComp_EngineToCivil(OutData, rData.FycmSlBase[kk]);      
		}

	}

	return bCalcOK;  
}

BOOL CDgnPscDataCtrl::Get_FpsgData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC_COMP& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	PSC_FPSG_BASE OutData;  

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		PSC_DATA_POSI &InData = MembInD.Pos[i];    
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		// 거더 
		InData.nGirder = PSC_DATA_POSI::Girder;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpsgGrBase[kk].bCHK==FALSE)  continue;
            
			InData.CalcD.iFpsgCalcType = j;
			m_pPscManager->ConvertFpsgComp_CivilToEngine(ElemK, rData.FpsgGrBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowblePrincipalStressBeforeLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFpsgComp_EngineToCivil(OutData, rData.FpsgGrBase[kk]);      
		}

		// 슬래브
		InData.nGirder = PSC_DATA_POSI::Slab;;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpsgSlBase[kk].bCHK==FALSE)  continue;

			InData.CalcD.iFpsgCalcType = j;
			m_pPscManager->ConvertFpsgComp_CivilToEngine(ElemK, rData.FpsgSlBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowblePrincipalStressBeforeLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFpsgComp_EngineToCivil(OutData, rData.FpsgSlBase[kk]);      
		}

	}

	return TRUE;
}


BOOL CDgnPscDataCtrl::Get_FpmsData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC_COMP& rData)
{
	PSC_DATA_POSI InData;
	PSC_FPMS_BASE OutData;  

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		ConvertPscCalcData(ElemK, i, bCompositeGen, &MatlElem, NULL, NULL, NULL, NULL, NULL, NULL, NULL, InData);

		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		// 거더
		InData.nGirder = PSC_DATA_POSI::Girder;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpmsGrBase[kk].bCHK==FALSE)  continue;      
			InData.CalcD.iFpmsCalcType = j;
			m_pPscManager->ConvertFpmsComp_CivilToEngine(ElemK, rData.FpmsGrBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowblePrincipalStressAfterLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFpmsComp_EngineToCivil(OutData, rData.FpmsGrBase[kk]);      
		}

		// 슬래브
		//     InData.nGirder = PSC_DATA_POSI::Slab;;
		//     for(j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		//     {
		//       int kk = i*2+j;
		//       if(rData.FpmsSlBase[kk].bCHK==FALSE)  continue;
		//       InData.CalcD.iFpmsCalcType = j;
		//       m_pPscManager->ConvertFpmsComp_CivilToEngine(ElemK, rData.FpmsSlBase[kk], OutData);
		//       if(!m_pPscManager->Calc_PscAllowblePrincipalStressAfterLoss(j, InData, OutData)) continue;
		//       m_pPscManager->ConvertFpmsComp_EngineToCivil(OutData, rData.FpmsSlBase[kk]);      
		//     }
	}  

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcwData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_COMP& rData, PSC_BDCW_D &rRes)
{
	PSC_BDCW_BASE OutData;

	BOOL bCalcOK=TRUE;

	int i,j;  

	for(i=0 ; i<2 ; i++)  // 0=I, 1=J
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);    

		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(j=0 ; j<2 ; j++) // 0=Top, 1=Bot
		{
			BOOL bOK_Zero = TRUE;
			BOOL bOk_Cr   = TRUE;
			BOOL bOk_Asmin= TRUE;

			int kk = i*2+j;      
			InData.CalcD.iBdcwCalcType = j;
			m_pPscManager->ConvertBdcwComp_CivilToEngine(ElemK, rData.BdcwBase[kk], OutData);

			if(m_iDgnCode == KSCE_LSD15_PSC || m_iDgnCode == KDS_24_14_21_2022_PSC )
			{
				int iExposureClass = (j==0)? InData.iExposureClass[0] : InData.iExposureClass[1];
				int iLcomType = OutData.iKind;
				BOOL bPost    = InData.TndnD.bPost;
				BOOL bBonded  = InData.TndnD.bBonded;

				bOk_Cr = Check_Wmax_4Crack_Grade(iExposureClass, iLcomType, bPost,  bBonded, bOK_Zero);
				//if(!bOk_Cr && !bOK_Zero) continue;
			}

			BOOL bCalcType = m_pPscManager->Calc_PscCrackCheck(j, InData, OutData);

            if ( m_iDgnCode == EUROCODE2_2_05_PSC ) bOk_Cr = OutData.bCHK_Crack; //DB 추가 관련으로 임시로 작업

			m_pPscManager->ConvertBdcwComp_EngineToCivil(OutData, bOk_Cr, rData.BdcwBase[kk]);
			m_pPscManager->ConvertBdcwComp_EngineToCivil_zero(OutData, bOK_Zero, rData.BdcwBaseZero[kk]);
			m_pPscManager->ConvertBdcwComp_EngineToCivil_Asmin(OutData, bOk_Asmin, rData.BdcwBaseAsmin[kk]);

			rRes.BdcwBase[kk] = OutData;
		}
	}

	return TRUE;
}



BOOL CDgnPscDataCtrl::Get_BdcrData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_COMP& rData)
{
	PSC_FRCR_D OutData;

	BOOL bCalcOK = TRUE;

	BOOL bI=FALSE,bJ=FALSE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.BdcrBase[kk].bCHK==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);

			InData.CalcD.iBdcrCalcType = j;
			m_pPscManager->ConvertBdcrComp_CivilToEngine(ElemK, rData.BdcrBase[kk], OutData);
			if(!m_pPscManager->Calc_PscFlexureStrength(j, InData, OutData)) continue;  
			m_pPscManager->ConvertBdcrComp_EngineToCivil(OutData, rData.BdcrBase[kk]);      
		}
	}

	return bCalcOK;
}


BOOL CDgnPscDataCtrl::Get_CumsData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, double adMr[4],  double adZ_sh[4],  
																						 double adCc_gr[4], double adCc_sl[4], double adForce_T[4], double adTst_sl[4], T_CUMS_PSC_COMP& rData)
{
	PSC_CUMS_BASE OutData;

	BOOL bCalcOK = TRUE;

	BOOL bI=FALSE,bJ=FALSE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.CumsBase[kk].bCHK==FALSE) continue;

			int nType = j;
			OutData.dMr   = adMr[kk];
			OutData.dz_sh = adZ_sh[kk];
			OutData.BdcwBaseComp.dCc_gr   = adCc_gr[kk]; 
			OutData.BdcwBaseComp.dCc_sl   = adCc_sl[kk];
			OutData.BdcwBaseComp.dForce_T = adForce_T[kk];
			OutData.BdcwBaseComp.dTst_sl  = adTst_sl[kk];

			m_pPscManager->ConvertCumsComp_CivilToEngine(ElemK, rData.CumsBase[kk], OutData);
			if(!m_pPscManager->Calc_PscShearDesign(nType, InData, OutData)) continue;      
			m_pPscManager->ConvertCumsComp_EngineToCivil(OutData, rData.CumsBase[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CrmtData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, double adZ_sh[6], double adCc_gr[6], double adCc_sl[6], T_CRMT_PSC_COMP& rData)
{
	PSC_CRMT_BASE OutData;

	BOOL bCalcOK = FALSE;

	BOOL bI=FALSE,bJ=FALSE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<3 ; j++) //Max, Min
		{
			int kk = i*3+j;
			if(rData.CrmtBase[kk].bCHK==FALSE) continue;

			OutData.dz_sh = adZ_sh[kk];
//       OutData.BdcwBaseComp.dCc_gr = adCc_gr[kk]; 
//       OutData.BdcwBaseComp.dCc_sl = adCc_sl[kk];  

			m_pPscManager->ConvertCrmtComp_CivilToEngine(ElemK, rData.CrmtBase[kk], OutData);
			if(!m_pPscManager->Calc_PscTorsionDesign(j, InData, OutData)) continue;    
			m_pPscManager->ConvertCrmtComp_EngineToCivil(OutData, rData.CrmtBase[kk]);
            bCalcOK = TRUE;
		}
	}
	return bCalcOK;
}


BOOL CDgnPscDataCtrl::Get_FysgData(ElemPairK ElemK, T_FYSG_D& rData)
{	
	PSC_DATA_POSI InData;
	_FYSG_PSC_LRFD_BASE OutData; OutData.Initialize();

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK=FALSE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
	int i=0, j=0;
	BOOL bPreTndn[2]={TRUE,TRUE};
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

		// Pre, Post 가 같이 있을 경우 Pre, Post 결정은 넓이로 한다. 
		double dApPre =0.0;
		double dApPost=0.0;
		for(j=0; j<arTndnPosi.GetSize(); j++)
		{
			_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(j);
			if(Tndn.iLoadType==0)      dApPre  += Tndn.dArea;
			else if(Tndn.iLoadType==1) dApPost += Tndn.dArea;
		}
		
		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], NULL, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, NULL, InData);
		
		if(dApPre<dApPost) bPreTndn[i] = FALSE;

		for(j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
		int kk = i*2+j;
		if(rData.FysgBase[kk].bCHK==FALSE)  continue;

		int nType = j; // 0=Compression, 1=Tension      
		m_pPscManager->ConvertFysg_CivilToEngineAASHTO(rData.FysgBase[kk], OutData);
		if(!m_pPscManager->Calc_AllowbleStressBeforeLoss(nType, InData, OutData)) continue;
		m_pPscManager->ConvertFysg_EngineToCVL(OutData, rData.FysgBase[kk]);
		}
	}   
	return bCalcOK;  
}

BOOL CDgnPscDataCtrl::Get_FysgData(ElemPairK ElemK, T_FYSG_AASHTO& rData, _FYSG_AASHTO* pRes, int nPrtUnit)
{
	PSC_DATA_POSI InData;
	_FYSG_PSC_LRFD_BASE OutData; OutData.Initialize();

	BOOL bCalcOK=TRUE;

	// Get Crack Width.
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

	int i,j;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}

	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

	for(i=0 ; i<2 ; i++)  // 0=I, 1=J
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);    

		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];

		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
		double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1]; 
		//
		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, NULL, InData);

		for(j=0 ; j<2 ; j++) // 0=Top, 1=Bot
		{
			int kk = i*2+j;

			int nType = j;
			m_pPscManager->ConvertFysg_CivilToEngineAASHTO2(rData.FysgBase[kk], OutData);

//       if(bCalcType)
//       {
//         if(bCalcType==2)
//         {
//           OutData.bOK=TRUE;
//            OutData.dFSS = 0.0;
//            OutData.ds_max = 0.0;
//            OutData.ds_use = 0.0;
//         }
//       }

			// outdata의 단위 변환
			if(nPrtUnit==1) 
			{
				T_UNIT_INDEX PrtUnit;
				m_pDataCtrl->Get_PrintUnit(PrtUnit.nBase_Force, PrtUnit.nBase_Length);

				m_pPscManager->ConvertFysg_Unit(PrtUnit.nBase_Force, PrtUnit.nBase_Length, OutData);
			} 

			m_pPscManager->ConvertFysg_EngineToCVL2(OutData, rData.FysgBase[kk]);

			if(pRes) pRes->FysgBase[kk] = OutData;

			//if(!Get_CrackCheck(bTopCalc, PscdD.iCrackWidth[j], MatdD, dbw, dShrThk, dpMomPos, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dMu, dftop, dfbot, dMu_tp, dfs, dw, dwa)) continue;
			//FysgD.FysgBase[i*2+j].dFRT = (dfs);
			//FysgD.FysgBase[i*2+j].dWC  = dw; 
			//FysgD.FysgBase[i*2+j].dAWC = dwa; 
		}
	}
	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_FycmData(ElemPairK ElemK, T_FYCM_AASHTO& rData, _FYCM_AASHTO* pRes, int nPrtUnit)
{
	PSC_DATA_POSI InData;
	_FYCM_PSC_LRFD_BASE OutData; OutData.Initialize();

	BOOL bCalcOK=TRUE;

	// Get Crack Width.
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

	int i,j;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}

	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

	for(i=0 ; i<2 ; i++)  // 0=I, 1=J
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);    

		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];

		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
		double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1]; 
		//
		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, NULL, InData);

		for(j=0 ; j<2 ; j++) // 0=Top, 1=Bot
		{
			int kk = i*2+j;

			int nType = j;
			m_pPscManager->ConvertFycm_CivilToEngineAASHTO(rData.FycmBase[kk], OutData);

			if(nPrtUnit==1) 
			{
				T_UNIT_INDEX PrtUnit;
				m_pDataCtrl->Get_PrintUnit(PrtUnit.nBase_Force, PrtUnit.nBase_Length);

				m_pPscManager->ConvertFycm_Unit(PrtUnit.nBase_Force, PrtUnit.nBase_Length, OutData);
			} 

			m_pPscManager->ConvertFycm_EngineToCVL(OutData, rData.FycmBase[kk]);

			if(pRes) pRes->FycmBase[kk] = OutData;

			//if(!Get_CrackCheck(bTopCalc, PscdD.iCrackWidth[j], MatdD, dbw, dShrThk, dpMomPos, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dMu, dftop, dfbot, dMu_tp, dfs, dw, dwa)) continue;
			//FycmD.FycmBase[i*2+j].dFRT = (dfs);
			//FycmD.FycmBase[i*2+j].dWC  = dw; 
			//FycmD.FycmBase[i*2+j].dAWC = dwa; 
		}
	}
	return bCalcOK;
}


BOOL CDgnPscDataCtrl::Get_FysgData(int nGirder, ElemPairK ElemK, T_FYSG_D& rData)
{	
	PSC_DATA_POSI InData;
	_FYSG_PSC_LRFD_BASE OutData; OutData.Initialize();

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	if(bComposite) //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	}
	else ASSERT(0);
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK=FALSE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
	int i=0, j=0;
	BOOL bPreTndn[2]={TRUE,TRUE};
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

		// Pre, Post 가 같이 있을 경우 Pre, Post 결정은 넓이로 한다. 
		double dApPre =0.0;
		double dApPost=0.0;
		for(j=0; j<arTndnPosi.GetSize(); j++)
		{
			_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(j);
			if(Tndn.iLoadType==0)      dApPre  += Tndn.dArea;
			else if(Tndn.iLoadType==1) dApPost += Tndn.dArea;
		}

		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], NULL, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, NULL, InData);

		if(dApPre<dApPost) bPreTndn[i] = FALSE;

		for(j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FysgBase[kk].bCHK==FALSE)  continue;
            InData.nGirder = (PSC_DATA_POSI::enType)nGirder;
			int nType = j; // 0=Compression, 1=Tension      
			m_pPscManager->ConvertFysg_CivilToEngineAASHTO(rData.FysgBase[kk], OutData);
			if(!m_pPscManager->Calc_AllowbleStressBeforeLoss(nType, InData, OutData)) continue;
			m_pPscManager->ConvertFysg_EngineToCVL(OutData, rData.FysgBase[kk]);
		}
	}   
	return bCalcOK;  
}


BOOL CDgnPscDataCtrl::Get_FypcData2(T_TDNA_K TdnaK, T_FYPC_AASHTO& rData)
{
	_PSC_TNDN_TDNA TndnTdna;
	if(!Get_PscTndnTdna(TdnaK, TndnTdna)) return FALSE;

	PSC_TNDN_TDNA InData;
	_FYPC_AASHTO OutData; OutData.Initialize();

	ConvertPscCalcTdnaData(&TndnTdna, InData);

	int iCivilCode = GetConvertCodeCvlToEng(m_pDataCtrl->Get_CivilCode());

	m_pPscManager->ConvertFypc_CivilToEngineAASHTO(rData, OutData);
	BOOL bOK = m_pPscManager->Calc_AllowbleStressTendonLoss(iCivilCode, InData, OutData);
	m_pPscManager->ConvertFypc_EngineToCVL(OutData, rData);

	if(!bOK) return FALSE;

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FycmData(ElemPairK ElemK, T_FYCM_AASHTO& rData)
{
	PSC_DATA_POSI InData;
	_FYCM_PSC_LRFD_BASE OutData; OutData.Initialize();

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK=FALSE;
	
	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	int i=0, j=0;
	BOOL bPreTndn[2]={TRUE,TRUE};
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

		// Pre, Post 가 같이 있을 경우 Pre, Post 결정은 넓이로 한다. 
		double dApPre =0.0;
		double dApPost=0.0;
		for(j=0; j<arTndnPosi.GetSize(); j++)
		{
			_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(j);
			if(Tndn.iLoadType==0)      dApPre  += Tndn.dArea;
			else if(Tndn.iLoadType==1) dApPost += Tndn.dArea;
		}    
		if(dApPre<dApPost) bPreTndn[i] = FALSE;

		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], NULL, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, NULL, InData);
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FycmBase[i*2+j].bCHK==FALSE)  continue;
			//double dalComf, dalTenf;
			
			int nType = j; // 0=Compression, 1=Tension
			m_pPscManager->ConvertFycm_CivilToEngineAASHTO(rData.FycmBase[kk], OutData);
			if(!m_pPscManager->Calc_AllowbleStressAfterLoss(nType, InData, OutData)) continue;
			m_pPscManager->ConvertFycm_EngineToCVL(OutData, rData.FycmBase[kk]);

			//if(!Get_AfterAllowbleStressofConc(bPreTndn[i], MatlElem.MatdD, SpscSect, arRbarPosi, arTndnPosi, FycmD.FycmBase[i*2+j], dalComf, dalTenf)) continue;
			//double dExtraRatio  = m_pForcCtrl->Get_ExtraRatio(FycmD.FycmBase[i*2+j].LcomK);

			// Set Data. CodeUnit -> CurrUnit.
			//FycmD.FycmBase[i*2+j].dALW = (-1)*dExtraRatio*(j==0 ? dalComf : dalTenf);
			
		}
	}

	return bCalcOK;  
}

BOOL CDgnPscDataCtrl::Get_FycmData(int nGirder, ElemPairK ElemK, T_FYCM_AASHTO& rData)
{
	PSC_DATA_POSI InData;
	_FYCM_PSC_LRFD_BASE OutData; OutData.Initialize();

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	if(bComposite) //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	}
	else ASSERT(0);
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK=FALSE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	int i=0, j=0;
	BOOL bPreTndn[2]={TRUE,TRUE};
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

		// Pre, Post 가 같이 있을 경우 Pre, Post 결정은 넓이로 한다. 
		double dApPre =0.0;
		double dApPost=0.0;
		for(j=0; j<arTndnPosi.GetSize(); j++)
		{
			_DGN_TNDN_CRC Tndn = arTndnPosi.GetAt(j);
			if(Tndn.iLoadType==0)      dApPre  += Tndn.dArea;
			else if(Tndn.iLoadType==1) dApPost += Tndn.dArea;
		}    
		if(dApPre<dApPost) bPreTndn[i] = FALSE;

		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], NULL, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, NULL, InData);
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FycmBase[i*2+j].bCHK==FALSE)  continue;
			//double dalComf, dalTenf;

			int nType = j; // 0=Compression, 1=Tension
			InData.nGirder = (PSC_DATA_POSI::enType)nGirder; // Girder : 0 Slab : 1
			m_pPscManager->ConvertFycm_CivilToEngineAASHTO(rData.FycmBase[kk], OutData);
			if(!m_pPscManager->Calc_AllowbleStressAfterLoss(nType, InData, OutData)) continue;
			m_pPscManager->ConvertFycm_EngineToCVL(OutData, rData.FycmBase[kk]);

			//if(!Get_AfterAllowbleStressofConc(bPreTndn[i], MatlElem.MatdD, SpscSect, arRbarPosi, arTndnPosi, FycmD.FycmBase[i*2+j], dalComf, dalTenf)) continue;
			//double dExtraRatio  = m_pForcCtrl->Get_ExtraRatio(FycmD.FycmBase[i*2+j].LcomK);

			// Set Data. CodeUnit -> CurrUnit.
			//FycmD.FycmBase[i*2+j].dALW = (-1)*dExtraRatio*(j==0 ? dalComf : dalTenf);

		}
	}

	return bCalcOK;  
}

BOOL CDgnPscDataCtrl::Get_PSCDgnData(ElemPairK ElemK, T_PSCD_DGN_INFO& rData)
{
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FssgData(ElemPairK ElemK, T_FPMT_JTG& rData)
{
	PSC_DATA_POSI InData;
	_FPMT_PSC_LRFD_BASE OutData; OutData.Initialize();

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, NULL, NULL, NULL, NULL, NULL, NULL, NULL, InData);

		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpmtBase[kk].bCHK==FALSE)  continue;

			int nType = j;
			m_pPscManager->ConvertFssg_CivilToEngineAASHTO(rData.FpmtBase[kk], OutData);
			if(!m_pPscManager->Calc_AllowblePrincipalStressBeforeLoss(nType, InData, OutData)) continue;
			m_pPscManager->ConvertFssg_EngineToCVL(OutData, rData.FpmtBase[kk]);
			
			//FsmtD.FsmtBase[i*2+j].dAFS  = -dAFS;
			//FsmtD.FsmtBase[i*2+j].dAFST = -dAFST;
		}
	}
	
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsData(ElemPairK ElemK, T_FPMS_JTG& rData)
{
	PSC_DATA_POSI InData;
	_FPMT_PSC_LRFD_BASE OutData; OutData.Initialize();

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
 
	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, NULL, NULL, NULL, NULL, NULL, NULL, NULL, InData);

		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpmsBase[kk].bCHK==FALSE)  continue;
			int nType = j;
			m_pPscManager->ConvertFpms_CivilToEngineAASHTO(rData.FpmsBase[kk], OutData);
			if(!m_pPscManager->Calc_AllowblePrincipalStressAfterLoss(nType, InData, OutData)) continue;
			m_pPscManager->ConvertFpms_EngineToCVL(OutData, rData.FpmsBase[kk]);
			
			//FsmtD.FsmtBase[i*2+j].dAFS  = -dAFS;
			//FsmtD.FsmtBase[i*2+j].dAFST = -dAFST;
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcwData(ElemPairK ElemK, T_BDCW_AASHTO& rData, _BDCW_AASHTO* pRes, int nPrtUnit)
{
	PSC_DATA_POSI InData;
	_BDCW_PSC_LRFD_BASE OutData; OutData.Initialize();

    if ( m_iDgnCode == AASHTO_LRFD19_PSC_RATING ) InData.bRating = true;

	BOOL bCalcOK=TRUE;
	
	// Get Crack Width.
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

	int i,j;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	
	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);    

		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];

		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
		double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1]; 
		//
		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, NULL, InData);

        if ( bComposite )
        {
            // 합성전 단면
            _PSC_SECT_ELEM SectElemPre;
            Get_PscSectElem(TRUE, ElemK, SectElemPre); // 합성전 환산단면
            _PSC_SECG_ELEM SecgElemPre;
            Get_PscSecgElem(ElemK, SecgElemPre);        // 합성전 순단면

            ConvertPscCalcSectData(ElemK, i, FALSE, &SectElemPre.SectPosi[i], InData.SectPreD);
            ConvertPscCalcSectgData(&SecgElemPre.SectPosi[i], InData.SectgPreD);
        }

		for(j=0 ; j<2 ; j++) // 0=Top, 1=Bot
		{
			int kk = i*2+j;
//      _DGN_FORC_CRC ForcLcomNTP;
			// FALSE : Except Tendon Primary
//       if(!m_pDataCtrl->Get_ConForc(ElemK, FALSE, rData.BdcwBase[kk].LcomK, ForcLcomNTP)) continue;
//       double dMu_tp = ForcLcomNTP.dMuy[i];
//       _DGN_FORC_CRC ForcLcom;
//       if(!m_pDataCtrl->Get_ConForc(ElemK, TRUE, rData.BdcwBase[kk].LcomK, ForcLcom)) continue;
//       BOOL   bTopCalc = j==0 ? TRUE : FALSE;
//       double dMu = ForcLcom.dMuy[i];
//       double dftop = -1.0*(rData.BdcwBase[kk].dFT);
// 	    double dfbot = -1.0*(rData.BdcwBase[kk].dFB);
//       double dfs=0.0, dw=0.0, dwa=0.0;
			
			int nType = j;
			m_pPscManager->ConvertBdcw_CivilToEngineAASHTO(rData.BdcwBase[kk], OutData);
			
			BOOL bCalcType = m_pPscManager->Calc_CrackCheck(nType, InData, OutData);
			if(bCalcType)
			{
				if(bCalcType==2)
				{
					OutData.bOK=TRUE;
					OutData.dFSS = 0.0;
					OutData.ds_max = 0.0;
					OutData.ds_use = 0.0;
				}
			}

			// outdata의 단위 변환
			if(nPrtUnit==1) 
			{
				T_UNIT_INDEX PrtUnit;
				m_pDataCtrl->Get_PrintUnit(PrtUnit.nBase_Force, PrtUnit.nBase_Length);

				m_pPscManager->ConvertBdcw_Unit(PrtUnit.nBase_Force, PrtUnit.nBase_Length, OutData);
			} 
		 
			m_pPscManager->ConvertBdcw_EngineToCVL(OutData, rData.BdcwBase[kk]);

			if(pRes) pRes->BdcwBase[kk] = OutData;

			//if(!Get_CrackCheck(bTopCalc, PscdD.iCrackWidth[j], MatdD, dbw, dShrThk, dpMomPos, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dMu, dftop, dfbot, dMu_tp, dfs, dw, dwa)) continue;
			//BdcwD.BdcwBase[i*2+j].dFRT = (dfs);
			//BdcwD.BdcwBase[i*2+j].dWC  = dw; 
			//BdcwD.BdcwBase[i*2+j].dAWC = dwa; 
		}
	}
	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_TbarData(ElemPairK ElemK, T_TBAR_AASHTO& rData)
{
	PSC_DATA_POSI InData;
	_TBAR_PSC_LRFD_BASE OutData; OutData.Initialize();

	BOOL bCalcOK=TRUE;
	  
	/*
	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	CArray<_DGN_FORC_CRC,_DGN_FORC_CRC> aForceLcom;
	CArray<_DGN_STRS_ADD,_DGN_STRS_ADD> aStressAdd;
	//!/ Tendon 포함이 맞는지 확인
	if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aForceLcom, aStressAdd, iLcomNum))	return FALSE;
	ASSERT(aForceLcom.GetSize()==aStressAdd.GetSize());
	if(m_pForcCtrl->Get_LcomNumForStrs()==0)	return FALSE;

	CArray<_DGN_STRS_CRC,_DGN_STRS_CRC> aStressLcom;
	CArray<_DGN_STRS_CRC,_DGN_STRS_CRC> aStressTnpr;
	if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aStressLcom, aStressTnpr, iLcomNum)) return FALSE;	
	*/

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	
	_PSC_SECT_ELEM SectElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	}

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	
	BOOL bI=FALSE,bJ=FALSE;
	//int iSta = (bI ? 0 : 1);	// I.
	//int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		
		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], NULL, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, NULL, InData);
		for(int j=0; j<2; j++)
		{
			int kk = i*2+j;
			if(!rData.TbarBase[kk].bCHK) continue;    
			/*
			int iDgnLcomNo = rData.TbarBase[kk].LcomK;
			if(iDgnLcomNo==0) continue;
			int m = iDgnLcomNo-1;
			_DGN_STRS_CRC StressCrc = aStressLcom.GetAt(m);
			_DGN_STRS_CRC StrsMax, StrsMin;
			StrsMax.Initialize();
			StrsMin.Initialize();
			if(!m_pDataCtrl->Get_StrsByForcFromCurrToCode(ElemK, iDgnLcomNo, StressCrc, StrsMax, StrsMin))	continue;
			double dftop = -StrsMin.dStop[i];
			double dfbot = -StrsMin.dSbot[i];
			*/

			int nType = j; // 0=Top, 1=Bottom
			m_pPscManager->ConvertTbar_CivilToEngineAASHTO(rData.TbarBase[kk], OutData);
			if(!m_pPscManager->Calc_TensionRebarReq(nType, InData, OutData)) continue;
			m_pPscManager->ConvertTbar_EngineToCVL(OutData, rData.TbarBase[kk]);
		}    
	}
	
	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcyData(ElemPairK ElemK, T_BDCY_AASHTO& rData, T_PSCD_DGN_INFO& rDgnR, _BDCY_AASHTO* pRes, int nPrtUnit, BOOL IsRating)
{
	PSC_DATA_POSI InData; InData.Initialize();
	_BDCY_PSC_LRFD_BASE OutData; OutData.Initialize();

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}

	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
    
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// rp
	// PS강재의 종류(0=저릴랙세이션(0.28), 1=응력제거강재(0.4), 2=강봉(0.55))
	//int iTndnKind = PscdD.iTndnKind;
	//double drp = 0.0;    
	//Get_TndnKindrp(iTndnKind, drp);

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);  

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	// Pre-Combined Composite Bridge
	BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb(); 

	int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
	ADGNFORCE aPreForceLcom;
	if (bIsPLCB==TRUE)
	{
		m_pForcCtrl->Get_BeforeForce4PreCompo(ElemK, aPreForceLcom, nLcomNum);
	}  

    T_STAG_K nGdLastStagK=0;
    int nLastStepNo=0;
    m_pForcCtrl->GetGirderLastStageKey(ElemK, nGdLastStagK, nLastStepNo);

	BOOL bI=FALSE,bJ=FALSE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);

		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		
		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		// Section

		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
		double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];    
		
		//Phi
		//double dPhib, dPhiv;
		//BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);

		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, &SpbfElem.PosD[i], InData);
		//khd SectPre 단면을 가져와서 저장
		if (bComposite==TRUE)
		{
			if(bCompositeGen) 
			{
				if(!ConvertPscCalcShearSectData(ElemK, i, InData.SectD))
				{
					// Get Elem.
					T_ELEM_D ElemD; ElemD.Initialize();
					if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first,ElemD)) {return FALSE;}	

					T_SECT_K SectK = ElemD.elpro;

					CString strText; strText.Format(_LS(IDS_DGN_PSC_COMPOSITE_SECTION_CHECK), SectK);
					AfxMessageBox(strText); // _T("Please Check Composite Section for PSC Design!")
					return FALSE;
				}
			}

			InData.bComposite = bComposite;
			InData.bGenComposite = bCompositeGen;
			// 합성전 단면
			_PSC_SECT_ELEM SectElemPre;
			Get_PscSectElem(TRUE, ElemK, SectElemPre); // 합성전 환산단면 정보 추가
			_PSC_SECG_ELEM SecgElemPre;
			Get_PscSecgElem(ElemK, SecgElemPre);        // 합성전 순단면 정보 추가
			_PSC_PSCN_ELEM PscnElem;
			//Get_PscPscnElem(ElemK, PscnElem);    // 전단연결재
			//_PSC_FACK_ELEM FackElem;
			//Get_PscFackElem(ElemK, FackElem);    // EC PSC 피로

			ConvertPscCalcSectData(ElemK, i, bCompositeGen, &SectElemPre.SectPosi[i], InData.SectPreD);
			ConvertPscCalcSectgData(&SecgElemPre.SectPosi[i], InData.SectgPreD);
			//ConvertPscnData(i, PscnElem, InData);
			//ConvertFackData(i, FackElem, InData);

		}
	
		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.BdcyBase[kk].bCHK==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);
			
			int ff = kk/2;
			if (bIsPLCB==TRUE)
			{
				// Rating 경우 dMdnc에 값을 할당되어 들어옴

				if (IsRating==FALSE)
				{
					int nLcomK = rData.BdcyBase[kk].LcomK-1;
					if (nLcomK<aPreForceLcom.GetSize()) rData.BdcyBase[kk].dMdnc = aPreForceLcom[nLcomK].dMuy[ff];
				}
			}
			else
			{
                // 시공중 거더 Force
                _DGN_FORC_CRC FcsDL;
                int nForceBeforeType=1;
                int nDgnLcomNo = rData.BdcyBase[kk].LcomK;
                if (!m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(IsRating, StctD, ElemK, nGdLastStagK, nLastStepNo,  nDgnLcomNo, nForceBeforeType, FcsDL, 1.0, TRUE))
                {
                    CString strSgldName = _LS(IDS_DB_SGLD_DL);	// DL.
                    T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);

                    double dFactor = 1.0;
                    _DGN_FORC_CRC ForcMinTemp;
                    m_pForcCtrl->Get_CompositeForceBySgld(ElemK, SgldK, nGdLastStagK, nLastStepNo, dFactor, FcsDL, ForcMinTemp);
                }

				rData.BdcyBase[kk].dMdnc = FcsDL.dMuy[ff]; //Mdnc 값
			}
			

			int nType = j;
			m_pPscManager->ConvertBdcy_CivilToEngineAASHTO(rData.BdcyBase[kk], OutData);
            m_pPscManager->ConvertPscDgnBdcr_CivilToEngineAASHTO(rDgnR.Bend[kk], OutData);
			if(!m_pPscManager->Calc_FlexureStrength(nType, InData, OutData)) continue;
			
			//m_pPscManager->Calc_FlexureStrengthComp_12(nType, InData, OutData)

			// outdata의 단위 변환
			if(nPrtUnit==1) 
			{
				T_UNIT_INDEX PrtUnit;
				m_pDataCtrl->Get_PrintUnit(PrtUnit.nBase_Force, PrtUnit.nBase_Length);

				m_pPscManager->ConvertBdcy_Unit(PrtUnit.nBase_Force, PrtUnit.nBase_Length, OutData);
			}
			
			m_pPscManager->ConvertBdcy_EngineToCVL(OutData, rData.BdcyBase[kk]);
            m_pPscManager->ConvertPscDgnBdcr_EngineToCVL(OutData.DgnR.BdcrD, rDgnR.Bend[kk]);

			if(pRes) pRes->BdcyBase[kk] = OutData;
			
			//if(!Get_FlexureStrength(TRUE, bPosiI, bIter, bPositive, dPhib, MatlElem, SpscSect, arOuter, arpInner, SpscgSect, 
			//                        drp, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcrKsce.BdcyBase[k])) continue;
		}
	}
	
	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsData(ElemPairK ElemK, T_CUMS_AASHTO& rData, _CUMS_AASHTO* pRes, int nPrtUnit)
{
	PSC_DATA_POSI InData;
	
	BOOL bCalcOK = TRUE;

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
 
	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	T_MATD_D MatdD = MatlElem.MatdD;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	BOOL bI=FALSE,bJ=FALSE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);

		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
	
		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
		double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];    

		//Shear&Torsion
		T_RPSC_SBAR RpscSbar;  RpscSbar = RshrElem.RshrPosi[i].RpscSbar;
		double dfdt = RshrElem.RshrPosi[i].dfdt;
		double dfdb = RshrElem.RshrPosi[i].dfdb;
		double dVd  = RshrElem.RshrPosi[i].dVd;
		double dMd  = RshrElem.RshrPosi[i].dMd;    
		
		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], &RshrElem.RshrPosi[i], &TndnElem.TndnPosi[i], NULL, &SpbfElem.PosD[i], InData);
		//khd SectPre 단면을 가져와서 저장
		if (bComposite==TRUE)
		{
			if(bCompositeGen) 
			{
				if(!ConvertPscCalcShearSectData(ElemK, i, InData.SectD))
				{
					// Get Elem.
					T_ELEM_D ElemD; ElemD.Initialize();
					if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first,ElemD)) {return FALSE;}	

					T_SECT_K SectK = ElemD.elpro;

					CString strText; strText.Format(_LS(IDS_DGN_PSC_COMPOSITE_SECTION_CHECK), SectK);
					AfxMessageBox(strText); // _T("Please Check Composite Section for PSC Design!")
					return FALSE;
				}
			}

			InData.bComposite = bComposite;
			InData.bGenComposite = bCompositeGen;
			// 합성전 단면
			_PSC_SECT_ELEM SectElemPre;
			Get_PscSectElem(TRUE, ElemK, SectElemPre); // 합성전 환산단면 정보 추가
			_PSC_SECG_ELEM SecgElemPre;
			Get_PscSecgElem(ElemK, SecgElemPre);        // 합성전 순단면 정보 추가
			_PSC_PSCN_ELEM PscnElem;
			//Get_PscPscnElem(ElemK, PscnElem);    // 전단연결재
			//_PSC_FACK_ELEM FackElem;
			//Get_PscFackElem(ElemK, FackElem);    // EC PSC 피로

			ConvertPscCalcSectData(ElemK, i, bCompositeGen, &SectElemPre.SectPosi[i], InData.SectPreD);
			ConvertPscCalcSectgData(&SecgElemPre.SectPosi[i], InData.SectgPreD);
			//ConvertPscnData(i, PscnElem, InData);
			//ConvertFackData(i, FackElem, InData);

		}
		_PSC_PSCS_ELEM PscsElem;
		if(bComposite)
		{
			// Interface Shear 정보
			if(!Get_PscPscsElem(ElemK, PscsElem)) bCalcOK=FALSE;
			if(bComposite) ConvertPscsData(i, PscsElem, InData);
		}

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.CumsBase[kk].bCHK==FALSE) continue;
			
            _CUMS_AASHTO_BASE OutData;
			int nType = j;
			m_pPscManager->ConvertCums_CivilToEngineAASHTO(rData.CumsBase[kk], OutData);
			if(!m_pPscManager->Calc_ShearDesign(nType, InData, OutData)) continue;  
			
			// outdata의 단위 변환
			if(nPrtUnit==1) 
			{
				T_UNIT_INDEX PrtUnit;
				m_pDataCtrl->Get_PrintUnit(PrtUnit.nBase_Force, PrtUnit.nBase_Length);

				m_pPscManager->ConvertCums_Unit(PrtUnit.nBase_Force, PrtUnit.nBase_Length, OutData);
			}

			m_pPscManager->ConvertCums_EngineToCVL(OutData, rData.CumsBase[kk]);
			if(pRes) pRes->CumsBase[kk] = OutData;
			
			//if(!Get_ShearDesign(bPosiI, dPhiv, MatdD, SpscSect, SpscgSect, dbw, dfdt, dfdb, dVd, dMd, arRbarPosi, arTndnPosi, RpscSbar, CumsKsce.CumsBase[k])) continue;      
		}
	}
	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CrmtData(ElemPairK ElemK, T_CRMT_AASHTO& rData, _CRMS_AASHTO* pRes, int nPrtUnit)
{
	PSC_DATA_POSI InData;

	BOOL bCalcOK = TRUE;
	
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}

	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
	
	BOOL bI=FALSE,bJ=FALSE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
	
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		// Section g
		T_SPSC_SECT SpscgSect; SpscgSect = SecgElem.SectPosi[i];
		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
		double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[i].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[i].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[i].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[i].dpMomPos[3][1];    

		//Shear&Torsion
		T_RPSC_SBAR RpscSbar;  RpscSbar = RshrElem.RshrPosi[i].RpscSbar;
		double dfdt = RshrElem.RshrPosi[i].dfdt;
		double dfdb = RshrElem.RshrPosi[i].dfdb;
		double dVd  = RshrElem.RshrPosi[i].dVd;
		double dMd  = RshrElem.RshrPosi[i].dMd;    
		//Phi
		//double dPhib, dPhiv;
		//BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);

		BOOL bSolidSect;
		if(!SectElem.SectPosi[i].bClosedSect) bSolidSect=TRUE;    // bClosedSect -> Box형
		else                                  bSolidSect=FALSE;
		double dAoh = SectElem.SectPosi[i].dAoh;
		double dPh  = SectElem.SectPosi[i].dPh;
		double dKt  = SectElem.SectPosi[i].dKt;    
		double dTorThk = SectElem.SectPosi[i].dTorThk;
		double dPcp = SectElem.SectPosi[i].dPcp;
		double dAcp = SectElem.SectPosi[i].dAcp;

	  ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], &RshrElem.RshrPosi[i], &TndnElem.TndnPosi[i], NULL, &SpbfElem.PosD[i], InData);

		for(int j=0 ; j<3 ; j++) //Vmax, Vmin, Tmax
		{
			int kk = i*3+j;
			if(rData.CrmtBase[kk].bCHK==FALSE) continue;

            _STCM_AASHTO_BASE OutData;
			int nType = j;
			m_pPscManager->ConvertCrmt_CivilToEngineAASHTO(rData.CrmtBase[kk], OutData);
			if(!m_pPscManager->Calc_TorsionDesign(nType, InData, OutData)) continue;  
			
			// outdata의 단위 변환
			if(nPrtUnit==1) 
			{
				T_UNIT_INDEX PrtUnit;
				m_pDataCtrl->Get_PrintUnit(PrtUnit.nBase_Force, PrtUnit.nBase_Length);

				m_pPscManager->ConvertCrms_Unit(PrtUnit.nBase_Force, PrtUnit.nBase_Length, OutData.Tor);
			}

			m_pPscManager->ConvertCrmt_EngineToCVL(OutData, rData.CrmtBase[kk]);
			if(pRes) pRes->CrmsBase[kk] = OutData.Tor;

			//if(!Get_TorsionDesign(TRUE, bPosiI, dPhiv, MatdD, SpscSect, SpscgSect, bSolidSect, dAoh, dPh, dbw, dTorThk, dPcp, dAcp, dfdt, dfdb, dVd, dMd, arTndnPosi, arRbarPosi, RpscSbar, CrmtKsce.CrmtBase[k])) continue;
		}
	}
	return bCalcOK;

}



//////////////////////////////////////////////////////////////////////////
// SNiP, SP35
void CDgnPscDataCtrl::SetMemberResDataSNiP(PSC_RUS_MEMB_RES_D &rData)
{
	m_pMembResSNiP = &rData;
	m_pPscManager->Set_MemberResDataSNiP(rData);
}

BOOL CDgnPscDataCtrl::MakeMemberResDataSNiP(ElemPairK ElemK, PSC_DATA_MEMB &PscMembD, PSC_RUS_MEMB_RES_D &rData)
{
	m_pPscManager->Calc_MemberResData(PscMembD, rData);
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FirstGroupLS(ElemPairK ElemK, PSC_DATA_MEMB &PscMembD, PSC_CUMS_SNIP_BASE *pCumsD)
{	 
	for(int i=0; i<2; ++i)
	{
		if (pCumsD[i].bCHK==FALSE) continue;

		if(!m_pPscManager->Calc_PscShearDesignSNiP(PscMembD.Pos[i], pCumsD[i])) continue;      
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FatiDataSNiP(ElemPairK ElemK, PSC_DATA_MEMB &PscMembD, PSC_FATI_SNIP_D &rData)
{
	for(int i=0; i<2; ++i)
	{
		if (rData.FatiBase[i].bCHK==FALSE) continue;

		if(!m_pPscManager->Calc_PscFatigueDesignSNiP(PscMembD.Pos[i], rData.FatiBase[i])) continue;      
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_SecondGroupLS(ElemPairK ElemK, PSC_DATA_MEMB &PscMembD, T_FYSG_SNIP &FysgD, T_FYCM_SNIP &FycmD, T_FPSG_SNIP &FpsgD, T_FPMS_SNIP &FpmsD)
{
	BOOL bCalcOK=TRUE;

	int i=0, j=0;
	for(i=0 ; i<2 ; ++i)  // 0=I, 1=J.
	{
		PSC_STRS_CHK_POSI OutData;  
		for(int j=0 ; j<2 ; ++j)  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
		{
			int kk = i*2+j;
			if(FysgD.FysgBase[kk].bCHK==TRUE) m_pPscManager->ConvertFysg_CivilToEngineSNiP(ElemK, FysgD.FysgBase[kk], OutData.Fysg[j]);
			if(FycmD.FycmBase[kk].bCHK==TRUE) m_pPscManager->ConvertFycm_CivilToEngineSNiP(ElemK, FycmD.FycmBase[kk], OutData.Fycm[j]);
			if(FpsgD.FpsgBase[kk].bCHK==TRUE) m_pPscManager->ConvertFpsg_CivilToEngineSNiP(ElemK, FpsgD.FpsgBase[kk], OutData.Fpsg[j]);
			if(FpmsD.FpmsBase[kk].bCHK==TRUE) m_pPscManager->ConvertFpms_CivilToEngineSNiP(ElemK, FpmsD.FpmsBase[kk], OutData.Fpms[j]);      
		}

		if(!m_pPscManager->Calc_SecondGroupLSSNiP(PscMembD.Pos[i], OutData)) continue;

		for(j=0 ; j<2 ; ++j)  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
		{
			int kk = i*2+j;
			if(FysgD.FysgBase[kk].bCHK==TRUE) m_pPscManager->ConvertFysg_EngineToCivilSNiP(OutData.Fysg[j], FysgD.FysgBase[kk]);
			if(FycmD.FycmBase[kk].bCHK==TRUE) m_pPscManager->ConvertFycm_EngineToCivilSNiP(OutData.Fycm[j], FycmD.FycmBase[kk]);
			if(FpsgD.FpsgBase[kk].bCHK==TRUE) m_pPscManager->ConvertFpsg_EngineToCivilSNiP(OutData.Fpsg[j], FpsgD.FpsgBase[kk]);
			if(FpmsD.FpmsBase[kk].bCHK==TRUE) m_pPscManager->ConvertFpms_EngineToCivilSNiP(OutData.Fpms[j], FpmsD.FpmsBase[kk]);      
		}
	}

	return bCalcOK;  
}

BOOL CDgnPscDataCtrl::Get_BdcwDataSNiP(ElemPairK ElemK, PSC_DATA_MEMB &PscMembD, PSC_BDCW_SNIP_BASE *pBdcwD)
{
	for(int i=0; i<2; ++i)
	{
		if (pBdcwD[i].bCHK==FALSE) continue;

		if(!m_pPscManager->Calc_PscCrackCheckSNiP(PscMembD.Pos[i], pBdcwD[i])) continue;      
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FypcDataSNiP(T_TDNA_K TdnaK, T_FYPC_SNIP &rData)
{
	_PSC_TNDN_TDNA TndnTdna;
	if(!Get_PscTndnTdna(TdnaK, TndnTdna)) return FALSE;

	PSC_TNDN_TDNA InTdnaD;      
	ConvertPscCalcTdnaData(&TndnTdna, InTdnaD);
	PSC_DATA_TNDN InData;
	InData.PscdD = m_SPscdD;
	InData.TdnaD = InTdnaD;

	PSC_FYPC_D OutData;

	m_pPscManager->ConvertFypc_CivilToEngineSNiP(TdnaK, rData, OutData);
	BOOL bOK = m_pPscManager->Calc_PscAllowbleStressTendonLoss(InData, OutData);
	m_pPscManager->ConvertFypc_EngineToCivilSNiP(OutData, rData);  

	if(!bOK) return FALSE;

	return TRUE;   

}

BOOL CDgnPscDataCtrl::Get_BdcwDataSNiP(ElemPairK ElemK, T_BDCW_SNIP &rData)
{
	PSC_DATA_POSI      InData;
	PSC_BDCW_SNIP_BASE OutData;

	BOOL bCalcOK=TRUE;

	// Get Crack Width.
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem))
	{
		return FALSE;
	}

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	for (int i=0 ; i<2 ; ++i)  // 0=I, 1=J
	{
		InData.Initialize();
		BOOL bPosiI = (i==0);;    
		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];

		double dShrThk[3]={0.0};
		double dbw = 0.0;
		double dFlgThk[2]={0.0}; 
		double dpMomPos[2][2]={0.0};
		Get_SectParam4CheckStrength(SectElem.SectPosi[i], dbw, dShrThk, dFlgThk, dpMomPos);
		//
		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], &ExpcElem.ExpcPosi[i], NULL, InData);

		for (int j=0 ; j<2 ; ++j) // 0=Top, 1=Bot
		{
			int kk    = i*2+j;      
			int nType = j;
			InData.CalcD.iBdcwCalcType = nType;
			m_pPscManager->ConvertBdcw_CivilToEngineSNiP(ElemK, rData.BdcwBase[kk], OutData);      
			BOOL bCalcType = m_pPscManager->Calc_PscCrackCheckSNiP(InData, OutData);

			m_pPscManager->ConvertBdcw_EngineToCivilSNiP(OutData, rData.BdcwBase[kk]);
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcrDataSNiP(ElemPairK ElemK, T_BDCR_SNIP &rData)
{
	PSC_DATA_POSI InData;
	PSC_FRCR_D OutData;

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

	// rp
	// PS강재의 종류(0=저릴랙세이션(0.28), 1=응력제거강재(0.4), 2=강봉(0.55))
	//int iTndnKind = PscdD.iTndnKind;
	//double drp = 0.0;    
	//Get_TndnKindrp(iTndnKind, drp);

	BOOL bI=FALSE,bJ=FALSE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=0; i<2; ++i)
	{
		InData.Initialize();
		BOOL bPosiI = (i==0);

		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		// Section

		// bw
		double dShrThk[3]={0.0};
		double dbw = 0.0;
		double dFlgThk[2]={0.0}; 
		double dpMomPos[2][2]={0.0};
		Get_SectParam4CheckStrength(SectElem.SectPosi[i], dbw, dShrThk, dFlgThk, dpMomPos);

		//Phi
		//double dPhib, dPhiv;
		//BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);

		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, NULL, InData);
		InData.iPosition = i; // I /J

		for(int j=0 ; j<2 ; ++j) //Max, Min
		{
			int kk = i*2+j;
			// [11/7/2013 joahn], 휨검토를 입력하지 않았더라도 비틀림에 사용될 수 있기 대문에 계산은 해야함.
			//if(rData.BdcrBase[kk].bCHK==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);

			int nType = j;
			InData.CalcD.iBdcrCalcType = nType;
			m_pPscManager->ConvertBdcr_CivilToEngineSNiP(ElemK, rData.BdcrBase[kk], OutData);
			if(!m_pPscManager->Calc_PscFlexureStrength(nType, InData, OutData)) continue;  
			m_pPscManager->ConvertBdcr_EngineToCivilSNiP(OutData, rData.BdcrBase[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsDataSNiP(ElemPairK ElemK, T_CUMS_SNIP &rData)
{
	PSC_DATA_POSI      InData;
	PSC_CUMS_SNIP_BASE OutData;

	BOOL bCalcOK = TRUE;

	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))  return FALSE;
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

	BOOL bI=FALSE,bJ=FALSE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=0; i<2; ++i)
	{
		InData.Initialize();
		BOOL bPosiI = (i==0);

		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];

		double dShrThk[3]={0.0};
		double dbw = 0.0;
		double dFlgThk[2]={0.0}; 
		double dpMomPos[2][2]={0.0};
		Get_SectParam4CheckStrength(SectElem.SectPosi[i], dbw, dShrThk, dFlgThk, dpMomPos);

		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], &RshrElem.RshrPosi[i], &TndnElem.TndnPosi[i], NULL, NULL, InData);
		InData.iPosition = i; // I /J

		for(int j=0 ; j<2 ; ++j) //Max, Min
		{
			int kk = i*2+j;
			if(rData.CumsBase[kk].bCHK==FALSE) continue;

			int nType = j;
			m_pPscManager->ConvertCums_CivilToEngineSNiP(ElemK, rData.CumsBase[kk], OutData);
			if(!m_pPscManager->Calc_PscShearDesignSNiP(InData, OutData)) continue;      
			m_pPscManager->ConvertCums_EngineToCivilSNiP(OutData, rData.CumsBase[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_FatiDataSNiP(ElemPairK ElemK, T_FATI_SNIP &rData)
{
	PSC_DATA_POSI      InData;
	PSC_FATI_SNIP_BASE OutData;

	BOOL bCalcOK = TRUE;

	_PSC_MATL_ELEM MatlElem;
	if (!Get_PscMatlElem(ElemK, MatlElem))  return FALSE;
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

	BOOL bI=FALSE,bJ=FALSE;
	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=0; i<2; ++i)
	{
		InData.Initialize();
		BOOL bPosiI = (i==0);

		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];

		double dShrThk[3]={0.0};
		double dbw = 0.0;
		double dFlgThk[2]={0.0}; 
		double dpMomPos[2][2]={0.0};
		Get_SectParam4CheckStrength(SectElem.SectPosi[i], dbw, dShrThk, dFlgThk, dpMomPos);

		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], &RshrElem.RshrPosi[i], &TndnElem.TndnPosi[i], NULL, NULL, InData);

		for(int j=0 ; j<2 ; ++j) //Max, Min
		{
			int kk = i*2+j;
			if(rData.FatiBase[kk].bCHK==FALSE) continue;

			int nType = j;
			m_pPscManager->ConvertFati_CivilToEngineSNiP(ElemK, rData.FatiBase[kk], OutData);
			if(!m_pPscManager->Calc_PscFatigueDesignSNiP(InData, OutData)) continue;      
			m_pPscManager->ConvertFati_EngineToCivilSNiP(OutData, rData.FatiBase[kk]);      
		}
	}

	return bCalcOK;
}

void CDgnPscDataCtrl::ConvertCumsSNiP(const PSC_CUMS_SNIP_BASE &InData, T_CUMS_SNIP_BASE &rData)
{
	m_pPscManager->ConvertCums_EngineToCivilSNiP(InData, rData);
}
void CDgnPscDataCtrl::ConvertCrmtSNiP(const PSC_CUMS_SNIP_BASE &InData, T_CRMT_SNIP_BASE &rData)
{
	m_pPscManager->ConvertCrmt_EngineToCivilSNiP(InData, rData);
}
void CDgnPscDataCtrl::ConvertFatiSNiP(const PSC_FATI_SNIP_BASE &InData, T_FATI_SNIP_BASE &rData)
{
	m_pPscManager->ConvertFati_EngineToCivilSNiP(InData, rData);
}
void CDgnPscDataCtrl::ConvertBdcwSNiP(const PSC_BDCW_SNIP_BASE &InData, T_BDCW_SNIP_BASE &rData)
{
	m_pPscManager->ConvertBdcw_EngineToCivilSNiP(InData, rData);
}

BOOL CDgnPscDataCtrl::CalcfpsInfoLpPosition()
{
    ArrElemPairKey aElemKey;
    GetElementList4TendonLp(aElemKey);

    if ( aElemKey.GetSize()==0 ) return FALSE;

    for ( INT_PTR i=0; i<aElemKey.GetSize(); ++i )
    {
        ElemPairK ElemK = aElemKey[i];

        BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);

        PSC_DATA_MEMB MembInD;

        Get_PscDataPosi(ElemK, 0, MembInD.Pos[0], bComposite);
        Get_PscDataPosi(ElemK, 1, MembInD.Pos[1], bComposite);

        for ( int jj=0; jj<2; ++jj )
        {
            BOOL bPosiI = (jj==0 ? TRUE : FALSE);

            std::pair<ElemPairK, UINT> prElemPos(ElemK, jj);

            if ( m_amElemPosLp.find(prElemPos) == m_amElemPosLp.end() ) continue;


            PSC_DATA_POSI &InD = MembInD.Pos[jj];

			CalcReCalcDevelopmentLength(ElemK, jj, InD);

            for ( int kk=0; kk<2; ++kk ) //Max, Min
            {
                //if ( bChkFlex[kk]==FALSE ) continue;
                BOOL bPositive = (kk==0 ? TRUE : FALSE);

                _BDCY_PSC_LRFD_BASE OutData;
                                
                InD.CalcD.iBdcrCalcType = kk;
                OutData.bCHK = true;
                OutData.dMuy = bPositive ? 1.0 : -1.0;

				switch ( m_iDgnCode )
				{
				case AS_5100_5_17_PSC:
					{
						if ( !m_pPscManager->Calc_PscFlexureStrengthAS(InD, OutData) ) continue;
					}
					break;
				case AASHTO_LRFD08:
				case AASHTO_LRFD12:
				case AASHTO_LRFD14:
				case AASHTO_LRFD16:
				case AASHTO_LRFD17:
				case AASHTO_LRFD20:
				case AASHTO_LRFD24:
				case AREMA_23:
					{
						if ( !m_pPscManager->Calc_FlexureStrength(kk, InD, OutData) ) continue;
					}
					break;
				}                

                for ( INT_PTR nT=0; nT < OutData.arCalcTndnInfo.GetSize(); ++nT )
                {
                    const PSC_FRCR_TNCD& TncB = OutData.arCalcTndnInfo[nT];
                    _DGN_TNDN_TRAN TndnTranD;
                    if ( m_amTndnTran.Lookup(TncB.KeyTdna, TndnTranD) )
                    {
                        if ( TndnTranD.TranB[0].ElemLpK == ElemK )
                        {
                            if ( (jj==0 && TndnTranD.TranB[0].bLpI_End) || (jj==1 && !TndnTranD.TranB[0].bLpI_End) )
                            {
                                _DGN_TNDN_TRAN_PN& fpsD = kk==0 ? TndnTranD.TranB[0].Pos : TndnTranD.TranB[0].Neg;
                                fpsD.dfps_Lp = TncB.dfpsi_ca;
                            }                            
                        }
                        if ( TndnTranD.TranB[1].ElemLpK == ElemK )
                        {
                            if ( (jj==0 && TndnTranD.TranB[1].bLpI_End) || (jj==1 && !TndnTranD.TranB[1].bLpI_End) )
                            {
                                _DGN_TNDN_TRAN_PN& fpsD = kk==0 ? TndnTranD.TranB[1].Pos : TndnTranD.TranB[1].Neg;
                                fpsD.dfps_Lp = TncB.dfpsi_ca;
                            }
                        }                                               
                        m_amTndnTran.SetAt(TncB.KeyTdna, TndnTranD);
                    }
                }
            }
        }
    }

    Get_PscTdnafpsInfo(m_TdnaInfo);

    return TRUE;
}

BOOL CDgnPscDataCtrl::CalcReCalcDevelopmentLength(ElemPairK ElemK, int nPos, PSC_DATA_POSI& InD)
{
	double dH = InD.SectgD.dzDim;
	BOOL   bPosiI = (nPos == 0);

	for ( int kk = 0; kk < 2; ++kk ) //Max, Min
	{
		BOOL bPositive = (kk == 0 ? TRUE : FALSE);

		_BDCY_PSC_LRFD_BASE OutData;

		InD.CalcD.iBdcrCalcType = kk;
		OutData.bCHK = true;
		OutData.dMuy = bPositive ? 1.0 : -1.0;

		switch ( m_iDgnCode )
		{
		case AS_5100_5_17_PSC:
			{
				if ( !m_pPscManager->Calc_PscFlexureStrengthAS(InD, OutData) ) continue;
			}
			break;
		case AASHTO_LRFD08:
		case AASHTO_LRFD12:
		case AASHTO_LRFD14:
		case AASHTO_LRFD16:
		case AASHTO_LRFD17:
		case AASHTO_LRFD20:
		case AASHTO_LRFD24:
		case AREMA_23:
			{
				if ( !m_pPscManager->Calc_FlexureStrength(kk, InD, OutData) ) continue;
			}
			break;
		}

		for ( INT_PTR nT = 0; nT < OutData.arCalcTndnInfo.GetSize(); ++nT )
		{
			const PSC_FRCR_TNCD& TncB = OutData.arCalcTndnInfo[nT];
			_DGN_TNDN_TRAN TndnTranD;
			if ( m_amTndnTran.Lookup(TncB.KeyTdna, TndnTranD) )
			{
				if ( TndnTranD.TranB[0].ElemLpK == ElemK )
				{
					if ( (nPos == 0 && TndnTranD.TranB[0].bLpI_End) || (nPos == 1 && !TndnTranD.TranB[0].bLpI_End) )
					{
						_DGN_TNDN_TRAN_PN& fpsD = kk == 0 ? TndnTranD.TranB[0].Pos : TndnTranD.TranB[0].Neg;
						fpsD.dfps_Lp = TncB.dfpsi_ca;
						double dDevLen_Cal = 0.0;
						double dDevLen = m_pDataCtrl->Calc_DevelopeLength(TncB.KeyTdna, bPosiI, dH, TndnTranD.TranB[0].dfpe_Lpt, fpsD.dfps_Lp, dDevLen_Cal);
					}
				}
				if ( TndnTranD.TranB[1].ElemLpK == ElemK )
				{
					if ( (nPos == 0 && TndnTranD.TranB[1].bLpI_End) || (nPos == 1 && !TndnTranD.TranB[1].bLpI_End) )
					{
						_DGN_TNDN_TRAN_PN& fpsD = kk == 0 ? TndnTranD.TranB[1].Pos : TndnTranD.TranB[1].Neg;
						fpsD.dfps_Lp = TncB.dfpsi_ca;
					}
				}
				m_amTndnTran.SetAt(TncB.KeyTdna, TndnTranD);
			}
		}
	}

	return TRUE;
}

// AS 5100:5
BOOL CDgnPscDataCtrl::Get_StressCheckCS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_AS &FysgD, T_FPMT_JTG &FpsgD)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	PSC_FYSG_BASE OutData;	  

	BOOL bCalcOK=TRUE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
	int i=0, j=0;
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;    

		T_STAG_K StagK = FysgD.FysgGrB[0].StageK;
		T_STAG_D StagD; StagD.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD))
		{
			T_ELFM_D DayActive; DayActive.Initialize();
			pDoc->m_pPostCtrl->GetElfmDgn(StagK, ElemK, DayActive);  // Stage의 시작과 끝에서 재료 날짜.			
			InData.dDuration = StagD.dDuration + DayActive.dStartMatAge;
		}
		//
		StagK = FysgD.FysgSlB[0].StageK;
		StagD.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD))
		{
			T_ELFM_D DayActive; DayActive.Initialize();
			pDoc->m_pPostCtrl->GetElfmDgn(StagK, ElemK, DayActive);  // Stage의 시작과 끝에서 재료 날짜.			
			InData.dDurationSl = StagD.dDuration + DayActive.dStartMatAge;
		}

        if (m_iDgnCode==AS_5100_5_17_PSC)
        {
		    PSC_LRFD_STRS_CHK_POSI OutData;
		    OutData.nChkType = 0; // 0=시공단계 검토, 1=완성계 검토, 2=시공단계, 완성계 검토
		    for(int j=0 ; j<2 ; ++j)  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
		    {
			    int kk = i*2+j;
			    if(FysgD.FysgGrB[kk].bCHK==TRUE) 
			    {
				    m_pPscManager->ConvertFysg_CivilToEngineAS(ElemK, FysgD.FysgGrB[kk], OutData.FysgGr[j]);
				    m_pPscManager->ConvertFysg_CivilToEngineAS(ElemK, FysgD.FysgSlB[kk], OutData.FysgSl[j]);
			    }
			    if(FpsgD.FpmtBase[kk].bCHK==TRUE) m_pPscManager->ConvertFssg_CivilToEngineAASHTO(FpsgD.FpmtBase[kk], OutData.Fpsg[j]);
		    }

		    if(!m_pPscManager->Calc_PscStressCheckAS(InData, OutData)) continue;

		    for(j=0 ; j<2 ; ++j)  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
		    {
			    int kk = i*2+j;
			    if(FysgD.FysgGrB[kk].bCHK==TRUE) 
			    {
				    m_pPscManager->ConvertFysg_EngineToCivilAS(OutData.FysgGr[j], FysgD.FysgGrB[kk]);
				    m_pPscManager->ConvertFysg_EngineToCivilAS(OutData.FysgSl[j], FysgD.FysgSlB[kk]);
			    }
			    if(FpsgD.FpmtBase[kk].bCHK==TRUE) m_pPscManager->ConvertFssg_EngineToCVL(OutData.Fpsg[j], FpsgD.FpmtBase[kk]);
		    }
        }
        else ASSERT(0);

	}

	return bCalcOK;    
}

//BS 5400

BOOL CDgnPscDataCtrl::Get_StressCheckSLSBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_BS &FycmD, T_FPMS_PSC &FpmsD)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    PSC_FYSG_BASE OutData;

    BOOL bCalcOK=TRUE;

    DGN_JOINT_D JointD;
    m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
    int i=0, j=0;
    for ( i=0; i<2; i++ )  // 0=I, 1=J.
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        PSC_DATA_POSI &InData = MembInD.Pos[i];
        // Joint
        InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
        InData.iJointType = JointD.iJointType;
        if ( m_iDgnCode==BS5400_90_PSC||m_iDgnCode==TMH07_3_1989 || m_iDgnCode==BD_44_15_PSC || m_iDgnCode==CS_455_PSC || CDBLib::IsPscRatingCodeBS(m_iDgnCode) )
        {
            PSC_STRS_CHK_POSI OutData;
            OutData.nChkType = 1; // 0=시공단계 검토, 1=완성계 검토, 2=시공단계, 완성계 검토
            for ( int j=0; j<2; ++j )  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
            {
                int kk = i*2+j;
                if ( FycmD.FycmGrB[kk].bCHK==TRUE )
                {
                    m_pPscManager->ConvertFycm_CivilToEngineBS(ElemK, FycmD.FycmGrB[kk], OutData.Fycm[j]);
                    m_pPscManager->ConvertFycm_CivilToEngineBS(ElemK, FycmD.FycmSlB[kk], OutData.FycmSl[j]);
                }
                if ( FpmsD.FpmsBase[kk].bCHK==TRUE ) m_pPscManager->ConvertFpms_CivilToEngineBS(ElemK, FpmsD.FpmsBase[kk], OutData.Fpms[j]);
            }

            if ( !m_pPscManager->Calc_PscStressCheckBS(InData, OutData) ) continue;

            for ( j=0; j<2; ++j )  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
            {
                int kk = i*2+j;
                if ( FycmD.FycmGrB[kk].bCHK==TRUE )
                {
                    m_pPscManager->ConvertFycm_EngineToCivilBS(OutData.Fycm[j], FycmD.FycmGrB[kk]);
                    m_pPscManager->ConvertFycm_EngineToCivilBS(OutData.FycmSl[j], FycmD.FycmSlB[kk]);
                }
                if ( FpmsD.FpmsBase[kk].bCHK==TRUE ) m_pPscManager->ConvertFpms_EngineToCivilBS(OutData.Fpms[j], FpmsD.FpmsBase[kk]);
            }
        }
        else ASSERT(0);
    }

    return bCalcOK;
}


BOOL CDgnPscDataCtrl::Get_StressCheckCSBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_BS &FysgD, T_FPSG_PSC &FpsgD)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    PSC_FYSG_BASE OutData;

    BOOL bCalcOK=TRUE;

    DGN_JOINT_D JointD;
    m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
    int i=0, j=0;
    for ( i=0; i<2; i++ )  // 0=I, 1=J.
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        PSC_DATA_POSI &InData = MembInD.Pos[i];
        // Joint
        InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

        if ( m_iDgnCode==BS5400_90_PSC || m_iDgnCode == TMH07_3_1989 || m_iDgnCode==BD_44_15_PSC || m_iDgnCode==CS_455_PSC )
        {
            PSC_STRS_CHK_POSI OutData;
            OutData.nChkType = 0; // 0=시공단계 검토, 1=완성계 검토, 2=시공단계, 완성계 검토
            for ( int j=0; j<2; ++j )  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
            {
                int kk = i*2+j;
                if ( FysgD.FysgGrB[kk].bCHK )
                {
                    T_STAG_K StagK = FysgD.FysgGrB[kk].StageK;
                    T_STAG_D StagD; StagD.Initialize();
                    if ( m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD) )
                    {
                        T_ELFM_D DayActive; DayActive.Initialize();
                        pDoc->m_pPostCtrl->GetElfmDgn(StagK, ElemK, DayActive);  // Stage의 시작과 끝에서 재료 날짜.			
                        InData.dDuration = StagD.dDuration + DayActive.dStartMatAge;
                    }
                    //
                    if ( InData.bComposite )
                    {
                        StagK = FysgD.FysgSlB[kk].StageK;
                        StagD.Initialize();
                        if ( m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD) )
                        {
                            T_ELFM_D DayActive; DayActive.Initialize();
                            pDoc->m_pPostCtrl->GetElfmDgn(StagK, ElemK, DayActive);  // Stage의 시작과 끝에서 재료 날짜.			
                            InData.dDurationSl = StagD.dDuration + DayActive.dStartMatAge;
                        }
                    }
                    m_pPscManager->ConvertFysg_CivilToEngineBS(ElemK, FysgD.FysgGrB[kk], OutData.Fysg[j]);
                    m_pPscManager->ConvertFysg_CivilToEngineBS(ElemK, FysgD.FysgSlB[kk], OutData.FysgSl[j]);
                }
                if ( FpsgD.FpsgBase[kk].bCHK && !InData.bComposite)
                {
                    T_STAG_K StagK = FpsgD.FpsgBase[kk].StageK;
                    T_STAG_D StagD; StagD.Initialize();
                    if ( m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD) )
                    {
                        T_ELFM_D DayActive; DayActive.Initialize();
                        pDoc->m_pPostCtrl->GetElfmDgn(StagK, ElemK, DayActive);  // Stage의 시작과 끝에서 재료 날짜.			
                        InData.dDuration = StagD.dDuration + DayActive.dStartMatAge;
                    }
                    m_pPscManager->ConvertFpsg_CivilToEngineBS(ElemK, FpsgD.FpsgBase[kk], OutData.Fpsg[j]);
                }
            }

            if ( !m_pPscManager->Calc_PscStressCheckBS(InData, OutData) ) continue;

            for ( j=0; j<2; ++j )  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
            {
                int kk = i*2+j;
                if ( FysgD.FysgGrB[kk].bCHK )
                {
                    m_pPscManager->ConvertFysg_EngineToCivilBS(OutData.Fysg[j], FysgD.FysgGrB[kk]);
                    m_pPscManager->ConvertFysg_EngineToCivilBS(OutData.FysgSl[j], FysgD.FysgSlB[kk]);
                }
                if ( FpsgD.FpsgBase[kk].bCHK && !InData.bComposite)
                {
                    m_pPscManager->ConvertFpsg_EngineToCivilBS(OutData.Fpsg[j], FpsgD.FpsgBase[kk]);
                }
            }
        }
        else ASSERT(0);

    }

    return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_StressCheckSLS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_AS &FycmD, T_FPMS_PSC &FpmsD)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	PSC_FYSG_BASE OutData;	  

	BOOL bCalcOK=TRUE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
	int i=0, j=0;
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;    

        if (m_iDgnCode==AS_5100_5_17_PSC)
        {
		    PSC_LRFD_STRS_CHK_POSI OutData;
		    OutData.nChkType = 1; // 0=시공단계 검토, 1=완성계 검토, 2=시공단계, 완성계 검토
		    for(int j=0 ; j<2 ; ++j)  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
		    {
			    int kk = i*2+j;
			    if(FycmD.FycmGrB[kk].bCHK==TRUE)
			    {
				    m_pPscManager->ConvertFycm_CivilToEngineAS(ElemK, FycmD.FycmGrB[kk], OutData.FycmGr[j]);
				    m_pPscManager->ConvertFycm_CivilToEngineAS(ElemK, FycmD.FycmSlB[kk], OutData.FycmSl[j]);
			    }
			    if(FpmsD.FpmsBase[kk].bCHK==TRUE) m_pPscManager->ConvertFpms_CivilToEngineAS(ElemK, FpmsD.FpmsBase[kk], OutData.Fpms[j]);      
		    }

		    if(!m_pPscManager->Calc_PscStressCheckAS(InData, OutData)) continue;

		    for(j=0 ; j<2 ; ++j)  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
		    {
			    int kk = i*2+j;
			    if(FycmD.FycmGrB[kk].bCHK==TRUE)
			    {
				    m_pPscManager->ConvertFycm_EngineToCivilAS(OutData.FycmGr[j], FycmD.FycmGrB[kk]);
				    m_pPscManager->ConvertFycm_EngineToCivilAS(OutData.FycmSl[j], FycmD.FycmSlB[kk]);
			    }
			    if(FpmsD.FpmsBase[kk].bCHK==TRUE) m_pPscManager->ConvertFpms_EngineToCivilAS(OutData.Fpms[j], FpmsD.FpmsBase[kk]);      
		    }
        }
        else ASSERT(0);
	}

	return bCalcOK;    
}

BOOL CDgnPscDataCtrl::Get_TransferCheck(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_TRAN_PSC_AS& TranD)
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();

    BOOL bCalcOK=TRUE;

    for (int i=0; i<2; i++ )  // 0=I, 1=J.
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        PSC_DATA_POSI &InData = MembInD.Pos[i];

        if ( m_iDgnCode==AS_5100_5_17_PSC )
        {
            _TRAN_PSC_BASE TranB;

            _BDCW_PSC_LRFD_BASE CrackB;
            
            for ( int j=0; j<2; ++j )  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
            {
                int kk = i*2+j;
                if ( TranD.GrB[kk].bCHK==TRUE )
                {
                    m_pPscManager->ConvertTran_CivilToEngineAS(ElemK, TranD.GrB[kk], TranB);
                }
            }

            if ( !m_pPscManager->Calc_PscTransferStressAS(InData, TranB) ) continue;

            for (int j=0; j<2; ++j )  // 0=MAX, 1=MIN. or 0=Compression, 1=Tension
            {
                int kk = i*2+j;
                if ( TranD.GrB[kk].bCHK==TRUE )
                {
                    m_pPscManager->ConvertTran_EngineToCivilAS(TranB, TranD.GrB[kk]);
                }                
            }
        }
        else ASSERT(0);
    }

    return bCalcOK;
}

// BOOL CDgnPscDataCtrl::Get_FysgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_AS& FysgD)
// {
//   return TRUE;    
// }
// 
// BOOL CDgnPscDataCtrl::Get_FycmData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_AS& rData)
// BOOL CDgnPscDataCtrl::Get_FpsgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC& rData)
// BOOL CDgnPscDataCtrl::Get_FpmsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC& rData)
BOOL CDgnPscDataCtrl::Get_BdcwDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_AS& rData) 
{
	BOOL bCalcOK=TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<2 ; j++) //Top, Bottom
		{
			int kk = i*2+j;
			if(rData.BdcwB[kk].bCHK==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);

			_BDCW_PSC_LRFD_BASE OutData;

			int nType = j;
			InData.CalcD.iBdcwCalcType = nType;
			m_pPscManager->ConvertBdcw_CivilToEngineAS(ElemK, rData.BdcwB[kk], OutData);
			if(!m_pPscManager->Calc_PscCrackCheckAS(InData, OutData)) continue;  
			m_pPscManager->ConvertBdcw_EngineToCivilAS(OutData, rData.BdcwB[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcrDataAS(ElemPairK ElemK, BOOL bChkFlex[4], PSC_DATA_MEMB &MembInD, T_BDCR_PSC_AS& rData)
{
	BOOL bCalcOK=TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if (bChkFlex[kk]==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);

			_BDCY_PSC_LRFD_BASE OutData;

			int nType = j;
			InData.CalcD.iBdcrCalcType = nType;
			m_pPscManager->ConvertBdcr_CivilToEngineAS(ElemK, rData.BdcrB[kk], OutData);
			if(!m_pPscManager->Calc_PscFlexureStrengthAS(InData, OutData)) continue;  
			m_pPscManager->ConvertBdcr_EngineToCivilAS(OutData, rData.BdcrB[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_SHRR_PSC_AS& rData)
{
	BOOL bCalcOK=TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.ShrrB[kk].bCHK==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);

			_CUMS_AASHTO_BASE OutData;

			int nType = j;
			InData.CalcD.iBdcrCalcType = nType;
			m_pPscManager->ConvertCums_CivilToEngineAS(ElemK, rData.ShrrB[kk], OutData);
			if(!m_pPscManager->Calc_PscShearTorsionStrengthAS(InData, OutData)) continue;  
			m_pPscManager->ConvertCums_EngineToCivilAS(OutData, rData.ShrrB[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CrmtDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_TORR_PSC_AS& rData)
{
	for(int i=0 ; i<2; ++i)
	{

	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcwDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, _BDCW_PSC_LRFD_BASE *pData) 
{
	for(int i=0; i<2; ++i) // I,J
	{
		for(int j=0; j<2; ++j) // Top, Bottom
		{
			int kk = i*2 + j;
			if (pData[kk].LInf.bCHK==FALSE) continue;

			PSC_DATA_POSI &InData = MembInD.Pos[i];
			InData.CalcD.iBdcwCalcType = j;
			if(!m_pPscManager->Calc_PscCrackCheckAS(InData, pData[kk])) continue;  
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_CumsDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, _CUMS_AASHTO_BASE *pData)
{
	for(int i=0 ; i<2; ++i)
	{
		if (pData[i].bCHK==FALSE) continue;
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		if(!m_pPscManager->Calc_PscShearTorsionStrengthAS(InData, pData[i])) continue;  
	}
	return TRUE;

}
BOOL CDgnPscDataCtrl::Get_CrmtDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, _STCM_AASHTO_BASE *pData)
{
	for(int i=0 ; i<2; ++i)
	{
		if (pData[i].Tor.bCHK==FALSE) continue;
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		if(!m_pPscManager->Calc_PscShearTorsionStrengthAS(InData, pData[i])) continue;  
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcwDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_BS& rData)  
{
    BOOL bCalcOK=TRUE;

    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        PSC_DATA_POSI &InData = MembInD.Pos[i];
        if(i==1) MembInD.Pos[1].bClearMashMap = MembInD.Pos[0].bClearMashMap;

        for(int j=0 ; j<2 ; j++) //Top, Bottom
        {
            int kk = i*2+j;
            if(rData.BdcwB[kk].bCHK==FALSE) continue;
            BOOL bPositive = (j==0 ? TRUE : FALSE);

            PSC_BDCW_BASE OutData;

            int nType = j;
            InData.CalcD.iBdcwCalcType = nType;
            m_pPscManager->ConvertBdcw_CivilToEngineBS(ElemK, rData.BdcwB[kk], OutData);
            if(!m_pPscManager->Calc_PscCrackCheckBS(InData, OutData)) continue;  
            m_pPscManager->ConvertBdcw_EngineToCivilBS(OutData, rData.BdcwB[kk]);      
        }
    }

    return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcrDataBS(ElemPairK ElemK, BOOL bChkFlex[4], PSC_DATA_MEMB &MembInD, T_BDCR_PSC_BS& rData)
{
    BOOL bCalcOK=TRUE;

    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        PSC_DATA_POSI &InData = MembInD.Pos[i];

        for(int j=0 ; j<2 ; j++) //Max, Min
        {
            int kk = i*2+j;
            if (bChkFlex[kk]==FALSE) continue;
            BOOL bPositive = (j==0 ? TRUE : FALSE);

            PSC_FRCR_D OutData;

            int nType = j;
            InData.CalcD.iBdcrCalcType = nType;
            m_pPscManager->ConvertBdcr_CivilToEngineBS(ElemK, rData.BdcrB[kk], OutData);
            if(!m_pPscManager->Calc_PscFlexureStrengthBS(InData, OutData)) continue;  
            m_pPscManager->ConvertBdcr_EngineToCivilBS(OutData, rData.BdcrB[kk]);      
        }
    }

    return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_SHRR_PSC_BS& rData)
{
    BOOL bCalcOK=TRUE;

    DGN_JOINT_D JointD;
    m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        PSC_DATA_POSI &InData = MembInD.Pos[i];

        // Joint
        InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

        if(rData.ShrrB[i].bCHK==FALSE) continue;

        PSC_CUMS_BASE OutData;

        //InData.CalcD.iBdcrCalcType = nType;
        m_pPscManager->ConvertCums_CivilToEngineBS(ElemK, rData.ShrrB[i], OutData);
        if(!m_pPscManager->Calc_PscShearTorsionStrengthBS(InData, OutData)) continue;  
        m_pPscManager->ConvertCums_EngineToCivilBS(OutData, rData.ShrrB[i]);      
    }

    return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CrmtDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_TORR_PSC_BS& rData)
{
    BOOL bCalcOK=TRUE;

    for(int i=0; i<2; i++)
    {
        BOOL bPosiI = (i==0 ? TRUE : FALSE);
        PSC_DATA_POSI &InData = MembInD.Pos[i];

        if(rData.TorrB[i].bCHK==FALSE) continue;

        PSC_CRMT_BASE OutData;

        //InData.CalcD.iBdcrCalcType = nType;
        m_pPscManager->ConvertCrmt_CivilToEngineBS(ElemK, rData.TorrB[i], OutData);
        if(!m_pPscManager->Calc_PscShearTorsionStrengthBS(InData, OutData)) continue;  
        m_pPscManager->ConvertCrmt_EngineToCivilBS(OutData, rData.TorrB[i]);      
    }

    return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcwDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, PSC_BDCW_BASE *pData)
{
    for(int i=0; i<2; ++i) // I,J
    {
        for(int j=0; j<2; ++j) // Top, Bottom
        {
            int kk = i*2 + j;
            if (pData[kk].bCHK==false) continue;

            PSC_DATA_POSI &InData = MembInD.Pos[i];
            InData.CalcD.iBdcwCalcType = j;
            if(!m_pPscManager->Calc_PscCrackCheckBS(InData, pData[kk])) continue;  
        }
    }

    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcrDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, PSC_FRCR_D    *pData)
{
    for(int i=0 ; i<2; ++i)
    {
        if (pData[i].bChk==FALSE) continue;
        PSC_DATA_POSI &InData = MembInD.Pos[i];
        if(!m_pPscManager->Calc_PscFlexureStrengthBS(InData, pData[i])) continue;  
    }
    return TRUE;

}

BOOL CDgnPscDataCtrl::Get_CumsDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, PSC_CUMS_BASE *pData)
{
    for(int i=0 ; i<2; ++i)
    {
        if (pData[i].bCHK==FALSE) continue;
        PSC_DATA_POSI &InData = MembInD.Pos[i];
        if(!m_pPscManager->Calc_PscShearTorsionStrengthBS(InData, pData[i])) continue;  
    }
    return TRUE;

}

BOOL CDgnPscDataCtrl::Get_CrmtDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, PSC_CRMT_BASE *pData)
{
    for(int i=0 ; i<2; ++i)
    {
        if (pData[i].bCHK==FALSE) continue;
        PSC_DATA_POSI &InData = MembInD.Pos[i];
        if(!m_pPscManager->Calc_PscShearTorsionStrengthBS(InData, pData[i])) continue;  
    }
    return TRUE;
}



BOOL CDgnPscDataCtrl::Get_FysgReportData(ElemPairK ElemK, const T_FYSG_PSC& InData, PSC_FYSG_D& rData)
{
	for(int i=0; i<4; i++)
	{
		if(InData.FysgBase[i].bCHK) m_pPscManager->ConvertFysg_CivilToEngine(ElemK, InData.FysgBase[i], rData.FysgBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FysgReportData(ElemPairK ElemK, const T_FYSG_PSC_IRC& InData, PSC_FYSG_D& rData)//IRC112
{
	for(int i=0; i<4; i++)
	{
		if(InData.FysgBase[i].bCHK) m_pPscManager->ConvertFysg_CivilToEngine(ElemK, InData.FysgBase[i], rData.FysgBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FysgReportData(ElemPairK ElemK, const T_FYSG_PSC_IRS& InData, PSC_FYSG_D& rData)//IRS
{
	for(int i=0; i<4; i++)
	{
		if(InData.FysgBase[i].bCHK) m_pPscManager->ConvertFysg_CivilToEngine(ElemK, InData.FysgBase[i], rData.FysgBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FypcReportData(T_TDNA_K TdnaK, const T_FYPC_AASHTO& InData, PSC_FYPC_D& rData)
{
	if(InData.bCHK) m_pPscManager->ConvertFypc_CivilToEngine(TdnaK, InData, rData, TRUE);
	return TRUE;
}


BOOL CDgnPscDataCtrl::Get_FycmReportData(ElemPairK ElemK, const T_FYCM_PSC& InData, PSC_FYCM_D& rData)
{
	for(int i=0; i<4; i++)
	{
		if(InData.FycmBase[i].bCHK) m_pPscManager->ConvertFycm_CivilToEngine(ElemK, InData.FycmBase[i], rData.FycmBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FycmReportData(ElemPairK ElemK, const T_FYCM_PSC_IRC& InData, PSC_FYCM_D& rData)//IRC112
{
	for(int i=0; i<4; i++)
	{
		if(InData.FycmBase[i].bCHK) m_pPscManager->ConvertFycm_CivilToEngine(ElemK, InData.FycmBase[i], rData.FycmBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FycmReportData(ElemPairK ElemK, const T_FYCM_PSC_IRS& InData, PSC_FYCM_D& rData)//IRS
{
	for(int i=0; i<4; i++)
	{
		if(InData.FycmBase[i].bCHK) m_pPscManager->ConvertFycm_CivilToEngine(ElemK, InData.FycmBase[i], rData.FycmBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpsgReportData(ElemPairK ElemK, const T_FPSG_PSC& InData, PSC_FPSG_D& rData)
{
	for(int i=0; i<4; i++)
	{
		if(InData.FpsgBase[i].bCHK) m_pPscManager->ConvertFpsg_CivilToEngine(ElemK, InData.FpsgBase[i], rData.FpsgBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpsgReportData(ElemPairK ElemK, const T_FPSG_PSC_IRC& InData, PSC_FPSG_D& rData)//IRC112
{
	for(int i=0; i<4; i++)
	{
		if(InData.FpsgBase[i].bCHK) m_pPscManager->ConvertFpsg_CivilToEngine(ElemK, InData.FpsgBase[i], rData.FpsgBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpsgReportData(ElemPairK ElemK, const T_FPSG_PSC_IRS& InData, PSC_FPSG_D& rData)//IRS
{
	for(int i=0; i<4; i++)
	{
		if(InData.FpsgBase[i].bCHK) m_pPscManager->ConvertFpsg_CivilToEngine(ElemK, InData.FpsgBase[i], rData.FpsgBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsReportData(ElemPairK ElemK, const T_FPMS_PSC& InData, PSC_FPMS_D& rData)
{
	for(int i=0; i<4; i++)
	{
		if(InData.FpmsBase[i].bCHK) m_pPscManager->ConvertFpms_CivilToEngine(ElemK, InData.FpmsBase[i], rData.FpmsBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsReportData(ElemPairK ElemK, const T_FPMS_PSC_IRC& InData, PSC_FPMS_D& rData)//IRC112
{
	for(int i=0; i<4; i++)
	{
		if(InData.FpmsBase[i].bCHK) m_pPscManager->ConvertFpms_CivilToEngine(ElemK, InData.FpmsBase[i], rData.FpmsBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsReportData(ElemPairK ElemK, const T_FPMS_PSC_IRS& InData, PSC_FPMS_D& rData)//IRS
{
	for(int i=0; i<4; i++)
	{
		if(InData.FpmsBase[i].bCHK) m_pPscManager->ConvertFpms_CivilToEngine(ElemK, InData.FpmsBase[i], rData.FpmsBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcwReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	//  
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);
	//
	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);

	int nType = nMaxMin;
	InData.CalcD.iBdcwCalcType = nType;
	m_pPscManager->ConvertBdcw_CivilToEngine(ElemK, BaseD, rData, TRUE);      
	bCalcOK = m_pPscManager->Calc_PscCrackCheck(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcwReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_BASE& rData)//IRC112
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	//  
	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);
	//
	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);

	int nType = nMaxMin;
	InData.CalcD.iBdcwCalcType = nType;
	m_pPscManager->ConvertBdcw_CivilToEngine(ElemK, BaseD, rData, TRUE);      
	bCalcOK = m_pPscManager->Calc_PscCrackCheck(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcwReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_PSC_BASE_IRS& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_BASE& rData)//IRS
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	//  
	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}

    _PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);
	//
	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);

	int nType = nMaxMin;
	InData.CalcD.iBdcwCalcType = nType;
	m_pPscManager->ConvertBdcw_CivilToEngine(ElemK, BaseD, rData, TRUE);      
	bCalcOK = m_pPscManager->Calc_PscCrackCheck(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_DesignConditionData(BOOL bLoadRating, ElemPairK ElemK, int nIJ, int nMaxMin, PSC_DATA_POSI& InData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	//
	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
	// MQC-a/b. 5302-10, 균열 관련, 기준에 따라 _PSC_EXPC_ELEM 데이터가 없을 수 도 있기 때문에 return FALSE 하지 않는다.  
	_PSC_EXPC_ELEM ExpcElem;
	Get_PscExpcElem(ElemK, ExpcElem); 
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);
    
    // Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	// Section

	// bw
	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);
	ConvertPscCalcData(ElemK,nIJ, bCompositeGen, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], &SpbfElem.PosD[nIJ], InData);

	//khd SectPre 단면을 가져와서 저장
	if (bComposite==TRUE)
	{
		if(bCompositeGen) 
		{
			if(!ConvertPscCalcShearSectData(ElemK, nIJ, InData.SectD))
			{
				// Get Elem.
				T_ELEM_D ElemD; ElemD.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first,ElemD)) {return FALSE;}	

				T_SECT_K SectK = ElemD.elpro;

				CString strText; strText.Format(_LS(IDS_DGN_PSC_COMPOSITE_SECTION_CHECK), SectK);
				AfxMessageBox(strText); // _T("Please Check Composite Section for PSC Design!")
				return FALSE;
			}
		}

		InData.bComposite = bComposite;
		InData.bGenComposite = bCompositeGen;
		// 합성전 단면
		_PSC_SECT_ELEM SectElemPre;
		Get_PscSectElem(TRUE, ElemK, SectElemPre); // 합성전 환산단면
		_PSC_SECG_ELEM SecgElemPre;
		Get_PscSecgElem(ElemK, SecgElemPre);        // 합성전 순단면
		//_PSC_PSCN_ELEM PscnElem;
		//Get_PscPscnElem(ElemK, PscnElem);    // 전단연결재
		//_PSC_FACK_ELEM FackElem;
		//Get_PscFackElem(ElemK, FackElem);    // EC PSC 피로

		ConvertPscCalcSectData(ElemK, nIJ, bCompositeGen, &SectElemPre.SectPosi[nIJ], InData.SectPreD);
		ConvertPscCalcSectgData(&SecgElemPre.SectPosi[nIJ], InData.SectgPreD);
		//ConvertPscnData(nIJ, PscnElem, InData);
		//ConvertFackData(nIJ, FackElem, InData);

	}

    InData.pTdnaInfo = &m_TdnaInfo;

	// PSC일 경우 입력데이터는 kip/in 단위임
	// 출력 단위에 맞게 Indata 변환
	T_UNIT_INDEX PrtUnit;
	m_pDataCtrl->Get_PrintUnit(PrtUnit.nBase_Force, PrtUnit.nBase_Length); 
	m_pPscManager->ConvertPSCIndata_Unit(PrtUnit.nBase_Force, PrtUnit.nBase_Length, InData);

	return bCalcOK;
}


BOOL CDgnPscDataCtrl::Get_BdcyReportData(BOOL bLoadRating, ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCY_AASHTO_BASE& BaseD, PSC_DATA_POSI& InData, _BDCY_PSC_LRFD_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	//
	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);

	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	// Section

	// bw
	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);

	//khd SectPre 단면을 가져와서 저장
	if (bComposite==TRUE)
	{
		if(bCompositeGen) 
		{
			if(!ConvertPscCalcShearSectData(ElemK, nIJ, InData.SectD))
			{
				// Get Elem.
				T_ELEM_D ElemD; ElemD.Initialize();
				if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first,ElemD)) {return FALSE;}	

				T_SECT_K SectK = ElemD.elpro;

				CString strText; strText.Format(_LS(IDS_DGN_PSC_COMPOSITE_SECTION_CHECK), SectK);
				AfxMessageBox(strText); // _T("Please Check Composite Section for PSC Design!")
				return FALSE;
			}
		}

		InData.bComposite = bComposite;
		InData.bGenComposite = bCompositeGen;
		// 합성전 단면
		_PSC_SECT_ELEM SectElemPre;
		Get_PscSectElem(TRUE, ElemK, SectElemPre); // 합성전 환산단면
		_PSC_SECG_ELEM SecgElemPre;
		Get_PscSecgElem(ElemK, SecgElemPre);        // 합성전 순단면
		//_PSC_PSCN_ELEM PscnElem;
		//Get_PscPscnElem(ElemK, PscnElem);    // 전단연결재
		//_PSC_FACK_ELEM FackElem;
		//Get_PscFackElem(ElemK, FackElem);    // EC PSC 피로

		ConvertPscCalcSectData(ElemK, nIJ, bCompositeGen, &SectElemPre.SectPosi[nIJ], InData.SectPreD);
		ConvertPscCalcSectgData(&SecgElemPre.SectPosi[nIJ], InData.SectgPreD);
		//ConvertPscnData(nIJ, PscnElem, InData);
		//ConvertFackData(nIJ, FackElem, InData);

	}

	int nType = nMaxMin; // 0=Positive, 1=Negative
	InData.CalcD.iBdcrCalcType = nType;
	m_pPscManager->ConvertBdcy_CivilToEngineAASHTO(BaseD, rData);
	bCalcOK = m_pPscManager->Calc_FlexureStrength(nType, InData, rData);

	// PSC일 경우 출력 단위에 맞게 rData 변환
	if(!bLoadRating)
	{
		T_UNIT_INDEX PrtUnit;
		m_pDataCtrl->Get_PrintUnit(PrtUnit.nBase_Force, PrtUnit.nBase_Length);

		//CDgnPscCommon::Get_PrintUnit(PrtUnit.nBase_Force, PrtUnit.nBase_Length);
		m_pPscManager->ConvertBdcy_Unit(PrtUnit.nBase_Force, PrtUnit.nBase_Length, rData);
	}
	//

	return bCalcOK;
}


BOOL CDgnPscDataCtrl::Get_BdcrReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	//
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	//
	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	// Section

	// bw
	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK,nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);

	int nType = nMaxMin;
	InData.CalcD.iBdcrCalcType = nType;
	m_pPscManager->ConvertBdcr_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscFlexureStrength(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcrReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData)//IRC112
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	//
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	//
	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	// Section

	// bw
	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK,nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);

	int nType = nMaxMin;
	InData.CalcD.iBdcrCalcType = nType;
	m_pPscManager->ConvertBdcr_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscFlexureStrength(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcrReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_BASE_IRS& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData)//IRS
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	//
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	//
	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);


	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	// Section

	// bw
	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK,nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);

	int nType = nMaxMin;
	InData.CalcD.iBdcrCalcType = nType;
	m_pPscManager->ConvertBdcr_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscFlexureStrength(nType, InData, rData);

	return bCalcOK;
}


BOOL CDgnPscDataCtrl::Get_CumsReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CUMS_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;

	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);

	int nType = nMaxMin;
	m_pPscManager->ConvertCums_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscShearDesign(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CUMS_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE& rData)//IRC112
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;

	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);

	int nType = nMaxMin;
	m_pPscManager->ConvertCums_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscShearDesign(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CUMS_PSC_BASE_IRS& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE& rData)//IRS
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;

	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);

	int nType = nMaxMin;
	m_pPscManager->ConvertCums_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscShearDesign(nType, InData, rData);

	return bCalcOK;
}


BOOL CDgnPscDataCtrl::Get_CrmtReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CRMT_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>2) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK = TRUE;  
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect; SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);

	int nType = nMaxMin;
	m_pPscManager->ConvertCrmt_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscTorsionDesign(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CrmtReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CRMT_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE& rData)//IRC112
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>2) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK = TRUE;  
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect; SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);

	int nType = nMaxMin;
	m_pPscManager->ConvertCrmt_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscTorsionDesign(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_FysgCompReportData(ElemPairK ElemK, const T_FYSG_PSC_COMP& InData, PSC_FYSG_D& rData, BOOL bGirder)
{
	if(bGirder)
	{
		for(int i=0; i<4; i++)
		{
			if(InData.FysgGrBase[i].bCHK) m_pPscManager->ConvertFysgComp_CivilToEngine(ElemK, InData.FysgGrBase[i], rData.FysgBase[i], TRUE);
		}
	}
	else
	{
		for(int i=0; i<4; i++)
		{
			if(InData.FysgSlBase[i].bCHK) m_pPscManager->ConvertFysgComp_CivilToEngine(ElemK, InData.FysgSlBase[i], rData.FysgBase[i], FALSE);
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_CrmtReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CRMT_PSC_BASE_IRS& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE& rData)//IRS
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>2) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK = TRUE;  
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
	_PSC_PSCN_ELEM PscnElem;
	Get_PscPscnElem(ElemK, PscnElem);
	ConvertPscnData(nIJ, PscnElem, InData);
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);
    // Section g
	T_SPSC_SECT SpscgSect; SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);

	int nType = nMaxMin;
	m_pPscManager->ConvertCrmt_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscTorsionDesign(nType, InData, rData);

	return bCalcOK;
}



BOOL CDgnPscDataCtrl::Get_FycmCompReportData(ElemPairK ElemK, const T_FYCM_PSC_COMP& InData, PSC_FYCM_D& rData, BOOL bGirder)
{
	if(bGirder)
	{
		for(int i=0; i<4; i++)
		{
			if(InData.FycmGrBase[i].bCHK) m_pPscManager->ConvertFycmComp_CivilToEngine(ElemK, InData.FycmGrBase[i], rData.FycmBase[i], TRUE);
		}
	}
	else
	{
		for(int i=0; i<4; i++)
		{
			if(InData.FycmSlBase[i].bCHK) m_pPscManager->ConvertFycmComp_CivilToEngine(ElemK, InData.FycmSlBase[i], rData.FycmBase[i], FALSE);
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpsgCompReportData(ElemPairK ElemK, const T_FPSG_PSC_COMP& InData, PSC_FPSG_D& rData, BOOL bGirder)
{

	if(bGirder)
	{
		for(int i=0; i<4; i++)
		{
			if(InData.FpsgGrBase[i].bCHK) m_pPscManager->ConvertFpsgComp_CivilToEngine(ElemK, InData.FpsgGrBase[i], rData.FpsgBase[i], TRUE);
		}

	}
	else
	{
		for(int i=0; i<4; i++)
		{
			if(InData.FpsgSlBase[i].bCHK) m_pPscManager->ConvertFpsgComp_CivilToEngine(ElemK, InData.FpsgSlBase[i], rData.FpsgBase[i], FALSE);
		}

	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsCompReportData(ElemPairK ElemK, const T_FPMS_PSC_COMP& InData, PSC_FPMS_D& rData, BOOL bGirder)
{

	if(bGirder)
	{
		for(int i=0; i<4; i++)
		{
			if(InData.FpmsGrBase[i].bCHK) m_pPscManager->ConvertFpmsComp_CivilToEngine(ElemK, InData.FpmsGrBase[i], rData.FpmsBase[i], TRUE);
		}
	}
	else
	{
		for(int i=0; i<4; i++)
		{
			if(InData.FpmsSlBase[i].bCHK) m_pPscManager->ConvertFpmsComp_CivilToEngine(ElemK, InData.FpmsSlBase[i], rData.FpmsBase[i], FALSE);
		}
	}

	return TRUE;
}




BOOL CDgnPscDataCtrl::Get_BdcwCompReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	//  

	//   _PSC_SECT_ELEM SectElem;
	//   if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	//   
	//   _PSC_SECG_ELEM SecgElem;
	//   if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;

	// ************************************************************
	_PSC_SECT_ELEM SectElemPreD; // 합성전 환산단면
	if(!Get_PscSectElem(TRUE, ElemK, SectElemPreD)) return FALSE;  
	_PSC_SECG_ELEM SecgElemPreD; // 합성전 순단면
	if(!Get_PscSecgElem(ElemK, SecgElemPreD)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSectPreD = SecgElemPreD.SectPosi[nIJ];

	ConvertPscCalcDataPreD(ElemK,  nIJ, bCompositeGen, &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);
	// ************************************************************

	_PSC_SECT_ELEM SectElem; // 합성후 환산단면
	if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem )) return FALSE;  
	_PSC_SECG_ELEM SecgElem; // 합성후 순단면
	if(!Get_CompoPscSecgElem(ElemK, SecgElem )) bCalcOK=FALSE;
	// ************************************************************ 

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	T_SPSC_SECT SpscgSectPre = SecgElemPreD.SectPosi[nIJ]; 


	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);
	//
	ConvertPscCalcData(ElemK, nIJ, bCompositeGen, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);
	InData.bComposite = bComposite;
	InData.bGenComposite = bCompositeGen;

	// 
	ConvertPscCalcSectData(ElemK, nIJ, bCompositeGen, &SectElemPreD.SectPosi[nIJ], InData.SectPreD);
	ConvertPscCalcSectgData(&SpscgSectPre, InData.SectgPreD);
	//

	int nType = nMaxMin;
	InData.CalcD.iBdcwCalcType = nType;
	m_pPscManager->ConvertBdcwComp_CivilToEngine(ElemK, BaseD, rData, TRUE);        
	bCalcOK = m_pPscManager->Calc_PscCrackCheck(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcwAsminReportData(ElemPairK ElemK, int nIJ, const T_BDCW_PSC_BASE& BaseD, PSC_BDCW_BASE& rData)
{
	if(nIJ<0 || nIJ>3) return FALSE;
	double dStress = 0.0;
	if(nIJ%2==0)
	{
		dStress = fabs(BaseD.dft);
	}
	else
	{
		dStress = fabs(BaseD.dfb);
	}
	rData.bCHK_Asmin         = BaseD.bCHK;
	rData.bOK_Asmin          = BaseD.bOK;
	rData.dStress_Conc       = dStress; // 절대값
	rData.dRatio_Asmin       = BaseD.dRatio;//Ratio == Stress 같음!!
	rData.LcomK_Asmin        = BaseD.LcomK;
	rData.iKind_Asmin        = BaseD.iKind;
	rData.iMax_Asmin         = BaseD.iMax;
	rData.iLSTerm_Asmin      = BaseD.iLSTerm;
	rData.strLcomName_Asmin  = m_pPscManager->Get_LcomName4PSC(BaseD.LcomK);
	rData.dfct_eff           = BaseD.dAst;

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcwCompAsminReportData(ElemPairK ElemK, int nIJ, const T_BDCW_PSC_COMP_BASE& BaseD, PSC_BDCW_BASE& rData)
{
	if(nIJ<0 || nIJ>3) return FALSE;
	double dStress = 0.0;
	if(nIJ%2==0)
	{
		dStress = fabs(BaseD.dft);
	}
	else
	{
		dStress = fabs(BaseD.dfb);
	}
	rData.bCHK_Asmin         = BaseD.bCHK;
	rData.bOK_Asmin          = BaseD.bOK;
	rData.dStress_Conc       = dStress; // 절대값
	rData.dRatio_Asmin       = BaseD.dRatio;//Ratio == Stress 같음!!
	rData.LcomK_Asmin        = BaseD.LcomK;
	rData.iKind_Asmin        = BaseD.iKind;
	rData.iMax_Asmin         = BaseD.iMax;
	rData.iLSTerm_Asmin      = BaseD.iLSTerm;
	rData.strLcomName_Asmin  = m_pPscManager->Get_LcomName4PSC(BaseD.LcomK);
	rData.dfct_eff           = BaseD.dAst;

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FysgDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYSG_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYSG_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	BOOL bCalcOK=TRUE;

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	//
	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);

	int nType = nMaxMin;
	InData.CalcD.iFysgCalcType = nType;
	m_pPscManager->ConvertFysg_CivilToEngine(ElemK, BaseD, rData, TRUE);        

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FycmDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYCM_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYCM_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	BOOL bCalcOK=TRUE;

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	//
	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);

	int nType = nMaxMin;
	InData.CalcD.iFycmCalcType = nType;
	m_pPscManager->ConvertFycm_CivilToEngine(ElemK, BaseD, rData, TRUE);        

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FysgCompDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYSG_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYSG_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	// ************************************************************
	_PSC_SECT_ELEM SectElemPreD; // 합성전 환산단면
	if(!Get_PscSectElem(TRUE, ElemK, SectElemPreD)) return FALSE;  
	_PSC_SECG_ELEM SecgElemPreD; // 합성전 순단면
	if(!Get_PscSecgElem(ElemK, SecgElemPreD)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSectPreD = SecgElemPreD.SectPosi[nIJ];

	ConvertPscCalcDataPreD(ElemK, nIJ, bCompositeGen,  &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);
	// ************************************************************

	_PSC_SECT_ELEM SectElem; // 합성후 환산단면
	if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem )) return FALSE;  
	_PSC_SECG_ELEM SecgElem; // 합성후 순단면
	if(!Get_CompoPscSecgElem(ElemK, SecgElem )) bCalcOK=FALSE;
	// ************************************************************ 
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	T_SPSC_SECT SpscgSectPre = SecgElemPreD.SectPosi[nIJ]; 

	ConvertPscCalcData(ElemK, nIJ, bCompositeGen,  &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);

	ConvertPscCalcDataPreD(ElemK,  nIJ, bCompositeGen, &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);

	//

	int nType = nMaxMin;
	InData.CalcD.iFysgCalcType = nType;
	m_pPscManager->ConvertFysgComp_CivilToEngine(ElemK, BaseD, rData, TRUE);        

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FycmCompDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYCM_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYCM_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	// ************************************************************
	_PSC_SECT_ELEM SectElemPreD; // 합성전 환산단면
	if(!Get_PscSectElem(TRUE, ElemK, SectElemPreD)) return FALSE;  
	_PSC_SECG_ELEM SecgElemPreD; // 합성전 순단면
	if(!Get_PscSecgElem(ElemK, SecgElemPreD)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSectPreD = SecgElemPreD.SectPosi[nIJ];

	ConvertPscCalcDataPreD(ElemK, nIJ, bCompositeGen,  &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);
	// ************************************************************

	_PSC_SECT_ELEM SectElem; // 합성후 환산단면
	if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem )) return FALSE;  
	_PSC_SECG_ELEM SecgElem; // 합성후 순단면
	if(!Get_CompoPscSecgElem(ElemK, SecgElem )) bCalcOK=FALSE;
	// ************************************************************ 
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	T_SPSC_SECT SpscgSectPre = SecgElemPreD.SectPosi[nIJ]; 

	ConvertPscCalcData(ElemK, nIJ, bCompositeGen,  &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);

	ConvertPscCalcDataPreD(ElemK,  nIJ, bCompositeGen, &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);

	//

	int nType = nMaxMin;
	InData.CalcD.iFycmCalcType = nType;
	m_pPscManager->ConvertFycmComp_CivilToEngine(ElemK, BaseD, rData, TRUE);        

	return TRUE;
}


BOOL CDgnPscDataCtrl::Get_FatgReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FATG_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FATG_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;


	// ************************************************************
	_PSC_SECT_ELEM SectElemPreD; // 합성전 환산단면
	if(!Get_PscSectElem(TRUE, ElemK, SectElemPreD)) return FALSE;  
	_PSC_SECG_ELEM SecgElemPreD; // 합성전 순단면
	if(!Get_PscSecgElem(ElemK, SecgElemPreD)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSectPreD = SecgElemPreD.SectPosi[nIJ];

	ConvertPscCalcDataPreD(ElemK, nIJ, bCompositeGen,  &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);
	// ************************************************************

	_PSC_SECT_ELEM SectElem; // 합성후 환산단면
	if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem )) return FALSE;  
	_PSC_SECG_ELEM SecgElem; // 합성후 순단면
	if(!Get_CompoPscSecgElem(ElemK, SecgElem )) bCalcOK=FALSE;
	// ************************************************************ 

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	T_SPSC_SECT SpscgSectPre = SecgElemPreD.SectPosi[nIJ]; 


	//   double dShrThk[3]={0.0};
	//   double dbw = 0.0;
	//   double dFlgThk[2]={0.0}; 
	//   double dpMomPos[2][2]={0.0};
	//   Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);
	//
	ConvertPscCalcData(ElemK, nIJ, bCompositeGen,  &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);
	InData.bComposite = bComposite;
	InData.bGenComposite = bCompositeGen;

	// 
	ConvertPscCalcSectData(ElemK, nIJ, bCompositeGen, &SectElemPreD.SectPosi[nIJ], InData.SectPreD);
	ConvertPscCalcSectgData(&SpscgSectPre, InData.SectgPreD);
	//

	int nType = nMaxMin;
	InData.CalcD.iFatgCalcType = nType; // Top / Bottom
	m_pPscManager->ConvertFatg_CivilToEngine(ElemK, BaseD, rData, TRUE);        

	bCalcOK = m_pPscManager->Calc_PscFatigeCheck(nType, InData, rData);

	return bCalcOK;

}

BOOL CDgnPscDataCtrl::Get_FatgReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FATG_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_FATG_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;


	// ************************************************************
	_PSC_SECT_ELEM SectElemPreD; // 합성전 환산단면
	if(!Get_PscSectElem(TRUE, ElemK, SectElemPreD)) return FALSE;  
	_PSC_SECG_ELEM SecgElemPreD; // 합성전 순단면
	if(!Get_PscSecgElem(ElemK, SecgElemPreD)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSectPreD = SecgElemPreD.SectPosi[nIJ];

	ConvertPscCalcDataPreD(ElemK, nIJ, bCompositeGen,  &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);
	// ************************************************************

	_PSC_SECT_ELEM SectElem; // 합성후 환산단면
	if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem )) return FALSE;  
	_PSC_SECG_ELEM SecgElem; // 합성후 순단면
	if(!Get_CompoPscSecgElem(ElemK, SecgElem )) bCalcOK=FALSE;
	// ************************************************************ 

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	T_SPSC_SECT SpscgSectPre = SecgElemPreD.SectPosi[nIJ]; 


	//   double dShrThk[3]={0.0};
	//   double dbw = 0.0;
	//   double dFlgThk[2]={0.0}; 
	//   double dpMomPos[2][2]={0.0};
	//   Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);
	//
	ConvertPscCalcData(ElemK, nIJ, bCompositeGen,  &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);
	InData.bComposite = bComposite;
	InData.bGenComposite = bCompositeGen;

	// 
	ConvertPscCalcSectData(ElemK, nIJ, bCompositeGen, &SectElemPreD.SectPosi[nIJ], InData.SectPreD);
	ConvertPscCalcSectgData(&SpscgSectPre, InData.SectgPreD);
	//

	int nType = nMaxMin;
	InData.CalcD.iFatgCalcType = nType; // Top / Bottom
	m_pPscManager->ConvertFatg_CivilToEngine(ElemK, BaseD, rData, TRUE);        

	bCalcOK = m_pPscManager->Calc_PscFatigeCheck(nType, InData, rData);

	return bCalcOK;

}

BOOL CDgnPscDataCtrl::Get_FatgReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FATG_PSC_BASE_IRS& BaseD, PSC_DATA_POSI& InData, PSC_FATG_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);

	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;


	// ************************************************************
	_PSC_SECT_ELEM SectElemPreD; // 합성전 환산단면
	if(!Get_PscSectElem(TRUE, ElemK, SectElemPreD)) return FALSE;  
	_PSC_SECG_ELEM SecgElemPreD; // 합성전 순단면
	if(!Get_PscSecgElem(ElemK, SecgElemPreD)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSectPreD = SecgElemPreD.SectPosi[nIJ];

	ConvertPscCalcDataPreD(ElemK, nIJ, bCompositeGen,  &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);
	// ************************************************************

	_PSC_SECT_ELEM SectElem; // 합성후 환산단면
	if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem )) return FALSE;  
	_PSC_SECG_ELEM SecgElem; // 합성후 순단면
	if(!Get_CompoPscSecgElem(ElemK, SecgElem )) bCalcOK=FALSE;
	// ************************************************************ 

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	T_SPSC_SECT SpscgSectPre = SecgElemPreD.SectPosi[nIJ]; 


	//   double dShrThk[3]={0.0};
	//   double dbw = 0.0;
	//   double dFlgThk[2]={0.0}; 
	//   double dpMomPos[2][2]={0.0};
	//   Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);
	//
	ConvertPscCalcData(ElemK, nIJ, bCompositeGen,  &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);
	InData.bComposite = bComposite;
	InData.bGenComposite = bCompositeGen;

	// 
	ConvertPscCalcSectData(ElemK, nIJ, bCompositeGen, &SectElemPreD.SectPosi[nIJ], InData.SectPreD);
	ConvertPscCalcSectgData(&SpscgSectPre, InData.SectgPreD);
	//

	int nType = nMaxMin;
	InData.CalcD.iFatgCalcType = nType; // Top / Bottom
	m_pPscManager->ConvertFatg_CivilToEngine(ElemK, BaseD, rData, TRUE);        

	bCalcOK = m_pPscManager->Calc_PscFatigeCheck(nType, InData, rData);

	return bCalcOK;

}

BOOL CDgnPscDataCtrl::Get_BdcrCompReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bPscComposite = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	// ************************************************************
	_PSC_SECT_ELEM SectElemPreD; // 합성전 환산단면
	if(!Get_PscSectElem(TRUE, ElemK, SectElemPreD)) return FALSE;  
	_PSC_SECG_ELEM SecgElemPreD; // 합성전 순단면
	if(!Get_PscSecgElem(ElemK, SecgElemPreD)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSectPreD = SecgElemPreD.SectPosi[nIJ];

	ConvertPscCalcDataPreD(ElemK, nIJ, bPscComposite, &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);
	// ************************************************************

	_PSC_SECT_ELEM SectElem; // 합성후 환산단면
	if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem )) return FALSE;  
	_PSC_SECG_ELEM SecgElem; // 합성후 순단면
	if(!Get_CompoPscSecgElem(ElemK, SecgElem )) bCalcOK=FALSE;
	// ************************************************************ 


	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE; 
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	T_SPSC_SECT SpscgSectPre = SecgElemPreD.SectPosi[nIJ]; 

	// Section
	// bw
	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, bPscComposite, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);
	InData.bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	InData.bGenComposite  = bPscComposite;

	// 
	ConvertPscCalcSectData(ElemK, nIJ, bPscComposite, &SectElemPreD.SectPosi[nIJ], InData.SectPreD);
	ConvertPscCalcSectgData(&SpscgSectPre, InData.SectgPreD);
	//

	int nType = nMaxMin;
	InData.CalcD.iBdcrCalcType = nType;
	m_pPscManager->ConvertBdcrComp_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscFlexureStrength(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsCompReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CUMS_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;

	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	//   _PSC_SECT_ELEM SectElem;
	//   if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	//   
	//   _PSC_SECG_ELEM SecgElem;
	//   if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;

	// ************************************************************
	_PSC_SECT_ELEM SectElemPreD; // 합성전 환산단면
	if(!Get_PscSectElem(TRUE, ElemK, SectElemPreD)) return FALSE;  
	_PSC_SECG_ELEM SecgElemPreD; // 합성전 순단면
	if(!Get_PscSecgElem(ElemK, SecgElemPreD)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSectPreD = SecgElemPreD.SectPosi[nIJ];

	ConvertPscCalcDataPreD(ElemK, nIJ, FALSE, &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);
	// ************************************************************

	_PSC_SECT_ELEM SectElem; // 합성후 환산단면
	if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem )) return FALSE;  
	_PSC_SECG_ELEM SecgElem; // 합성후 순단면
	if(!Get_CompoPscSecgElem(ElemK, SecgElem )) bCalcOK=FALSE;
	// ************************************************************ 


	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);
	ConvertPscCalcData(ElemK,nIJ, bCompositeGen, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);
	InData.bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	InData.bGenComposite  = bCompositeGen;


	CDBDoc* pDoc = CDBDoc::GetDocPoint();			
	_PSC_PSCN_ELEM PscnElem;
	BOOL bPscComposite = pDoc->m_pPostCtrl->IsElem4CompositePSCSect(ElemK);		
	if(bPscComposite)
	{
		// 전단연결재 정보
		if(!Get_PscPscnElem(ElemK, PscnElem)) bCalcOK=FALSE;
		if(bPscComposite) ConvertPscnData(nIJ, PscnElem, InData);
	}

	int nType = nMaxMin;
	m_pPscManager->ConvertCumsComp_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscShearDesign(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CrmtCompReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CRMT_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>2) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK = TRUE;  
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	//   _PSC_SECT_ELEM SectElem;
	//   if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	//   
	//   _PSC_SECG_ELEM SecgElem;
	//   if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;

	// ************************************************************
	_PSC_SECT_ELEM SectElemPreD; // 합성전 환산단면
	if(!Get_PscSectElem(TRUE, ElemK, SectElemPreD)) return FALSE;  
	_PSC_SECG_ELEM SecgElemPreD; // 합성전 순단면
	if(!Get_PscSecgElem(ElemK, SecgElemPreD)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSectPreD = SecgElemPreD.SectPosi[nIJ];

	ConvertPscCalcDataPreD(ElemK, nIJ, FALSE, &SectElemPreD.SectPosi[nIJ], &SpscgSectPreD, InData);
	// ************************************************************

	_PSC_SECT_ELEM SectElem; // 합성후 환산단면
	if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem )) return FALSE;  
	_PSC_SECG_ELEM SecgElem; // 합성후 순단면
	if(!Get_CompoPscSecgElem(ElemK, SecgElem )) bCalcOK=FALSE;
	// ************************************************************ 


	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
    _PSC_SPBF_ELEM SpbfElem;
    Get_PscSpbfElem(ElemK, SpbfElem);

	// Section g
	T_SPSC_SECT SpscgSect; SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	BOOL bGenComposite = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);
	ConvertPscCalcData(ElemK, nIJ, bGenComposite, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, &SpbfElem.PosD[nIJ], InData);
	InData.bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	InData.bGenComposite  = bGenComposite;

	int nType = nMaxMin;
	m_pPscManager->ConvertCrmtComp_CivilToEngine(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscTorsionDesign(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_FysgReportData_SNiP(ElemPairK ElemK, const T_FYSG_SNIP& InData, PSC_FYSG_D& rData)
{
	for(int i=0; i<4; i++)
	{
		if(InData.FysgBase[i].bCHK) m_pPscManager->ConvertFysg_CivilToEngineSNiP(ElemK, InData.FysgBase[i], rData.FysgBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FypcReportData_SNiP(T_TDNA_K TdnaK, const T_FYPC_SNIP& InData, PSC_FYPC_D& rData)
{
	if(InData.bCHK) m_pPscManager->ConvertFypc_CivilToEngineSNiP(TdnaK, InData, rData, TRUE);
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FycmReportData_SNiP(ElemPairK ElemK, const T_FYCM_SNIP& InData, PSC_FYCM_D& rData)
{
	for(int i=0; i<4; i++)
	{
		if(InData.FycmBase[i].bCHK) m_pPscManager->ConvertFycm_CivilToEngineSNiP(ElemK, InData.FycmBase[i], rData.FycmBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpsgReportData_SNiP(ElemPairK ElemK, const T_FPSG_SNIP& InData, PSC_FPSG_D& rData)
{
	for(int i=0; i<4; i++)
	{
		if(InData.FpsgBase[i].bCHK) m_pPscManager->ConvertFpsg_CivilToEngineSNiP(ElemK, InData.FpsgBase[i], rData.FpsgBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsReportData_SNiP(ElemPairK ElemK, const T_FPMS_SNIP& InData, PSC_FPMS_D& rData)
{
	for(int i=0; i<4; i++)
	{
		if(InData.FpmsBase[i].bCHK) m_pPscManager->ConvertFpms_CivilToEngineSNiP(ElemK, InData.FpmsBase[i], rData.FpmsBase[i], TRUE);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcrReportData_SNiP(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;  

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
	//
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	//
	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;


	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
	// Section

	// bw
	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, NULL, InData);

	int nType = nMaxMin;
	InData.CalcD.iBdcrCalcType = nType;
	m_pPscManager->ConvertBdcr_CivilToEngineSNiP(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscFlexureStrength(nType, InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsReportData_SNiP(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CUMS_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_SNIP_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;

	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, NULL, InData);

	int nType = nMaxMin;
	m_pPscManager->ConvertCums_CivilToEngineSNiP(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscShearDesignSNiP(InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CrmtReportData_SNiP(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CRMT_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_SNIP_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;

	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, NULL, InData);

	int nType = nMaxMin;
	m_pPscManager->ConvertCrmt_CivilToEngineSNiP(ElemK, BaseD, rData, TRUE); // (kyung Sik)잠깐 막았어요..
	bCalcOK = m_pPscManager->Calc_PscShearDesignSNiP(InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_FatiReportData_SNiP(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FATI_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FATI_SNIP_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;

	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

	// Section g
	T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], NULL, NULL, InData);

	int nType = nMaxMin;
	m_pPscManager->ConvertFati_CivilToEngineSNiP(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscFatigueDesignSNiP(InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcwReportData_SNiP(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_SNIP_BASE& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	//PSC_DATA_POSI InData;

	BOOL bCalcOK = TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;

	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;

	_PSC_SECG_ELEM SecgElem;
	if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;
	_PSC_EXPC_ELEM ExpcElem;
	if(!Get_PscExpcElem(ElemK, ExpcElem)) return FALSE;

	// Section g
	T_SPSC_SECT &SpscgSect = SecgElem.SectPosi[nIJ];

	double dShrThk[3]={0.0};
	double dbw = 0.0;
	double dFlgThk[2]={0.0}; 
	double dpMomPos[2][2]={0.0};
	Get_SectParam4CheckStrength(SectElem.SectPosi[nIJ], dbw, dShrThk, dFlgThk, dpMomPos);

	ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], &RshrElem.RshrPosi[nIJ], &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);

	int nType = nMaxMin;
	m_pPscManager->ConvertBdcw_CivilToEngineSNiP(ElemK, BaseD, rData, TRUE);
	bCalcOK = m_pPscManager->Calc_PscCrackCheckSNiP(InData, rData);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_ReportBlock4SNiP(BOOL bRptPrint[6], PSC_DATA_POSI &InData, PSC_DRES_RPT_SNIP &rData)
{
	// BOOL bRptPrint[6] = {bCheckMomI_P, bCheckMomI_N, bCheckShrI, bCheckTorI, bCheckFatI, bCheckCraI};

	rData.aBlockName.RemoveAll();
	CStringW strBlock = _T("Block_DgnInfo"); rData.aBlockName.Add(strBlock);
	strBlock = _T("Block_Title_1st_LS"); rData.aBlockName.Add(strBlock);
	if (bRptPrint[0] && rData.FlexD[0].bChk) // Positive Moment
	{
		strBlock = _T("Block_Title_P_BendingMoment"); rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_PB_CompZone"); rData.aBlockName.Add(strBlock);
		if      (rData.FlexD[0].SNiP.nCneuType==1) strBlock = _T("Block_PB_X1");
		else if (rData.FlexD[0].SNiP.nCneuType==2) strBlock = _T("Block_PB_X2");
		else if (rData.FlexD[0].SNiP.nCneuType==3) strBlock = _T("Block_PB_X3");
		rData.aBlockName.Add(strBlock);
	}

	if (bRptPrint[1] && rData.FlexD[1].bChk) // Negaitive Moment
	{    
		strBlock = _T("Block_Title_N_BendingMoment"); rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_NB_CompZone"); rData.aBlockName.Add(strBlock);
		if      (rData.FlexD[1].SNiP.nCneuType==1) strBlock = _T("Block_NB_X1");
		else if (rData.FlexD[1].SNiP.nCneuType==2) strBlock = _T("Block_NB_X2");
		else if (rData.FlexD[1].SNiP.nCneuType==3) strBlock = _T("Block_NB_X3");
		rData.aBlockName.Add(strBlock);
	}

	if (bRptPrint[2] && rData.ShearD.bCHK) // Shear
	{
		strBlock = _T("Block_Title_Shear"); rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Sh_Con"); rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Sh_rein"); rData.aBlockName.Add(strBlock);
	}

	if (bRptPrint[3] && rData.TorsD.bCHK) // Torsion
	{
		strBlock = _T("Block_Title_Torsion");         rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Tor_Chk");               rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Tor_NSect_T");           rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Tor_SP_T");              rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Tor_NSect_L");           rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Tor_SP_L");              rData.aBlockName.Add(strBlock);

		strBlock = _T("Block_Title_Tor_Interaction"); rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Tor_Chk_Interaction1");  rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Tor_Chk_Interaction2");  rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Tor_Chk_Interaction3");  rData.aBlockName.Add(strBlock);
	}

	if (bRptPrint[4] && rData.FatiD.bCHK) // Fatigue
	{
		strBlock = _T("Block_Title_Fatigue"); rData.aBlockName.Add(strBlock);
		strBlock = _T("Block_Fatigue_Chk");   rData.aBlockName.Add(strBlock);
	}

	if (bRptPrint[5] && (rData.CrackD[0].bCHK || rData.CrackD[1].bCHK)) // Crack
	{
		strBlock = _T("Block_Title_2nd_LS");       rData.aBlockName.Add(strBlock);

		for (int i=0; i<2; ++i)
		{
			if (rData.CrackD[i].bCHK==FALSE) continue;

			if (i==0)
			{
				strBlock = _T("Block_Title_Crack_Resist_Top");    rData.aBlockName.Add(strBlock);
				strBlock = _T("Block_Crack_Long");                rData.aBlockName.Add(strBlock);        
			}
			else
			{
				strBlock = _T("Block_Title_Crack_Resist_Bottom"); rData.aBlockName.Add(strBlock);
				strBlock = _T("Block_Crack_Long_Bot");            rData.aBlockName.Add(strBlock);        
			}
			strBlock = _T("Block_Title_Crack_normal"); rData.aBlockName.Add(strBlock);

			if(rData.CrackD[i].bAllSectComp)
			{
				if (i==0) { strBlock = _T("Block_Crack_width_All_Compression");			rData.aBlockName.Add(strBlock); }
				else      { strBlock = _T("Block_Crack_width_All_Compression_Bottom");	rData.aBlockName.Add(strBlock); }
			}
			else
			{
				switch (InData.iExposureClass[i])
				{
				case 0: // 2a
					{
						if (i==0) { strBlock = _T("Block_Crack_normal_2a");     rData.aBlockName.Add(strBlock); }
						else      { strBlock = _T("Block_Crack_normal_2a_Bot"); rData.aBlockName.Add(strBlock); }
						break;
					}
				case 1: // 2b
					{
						if (m_iDgnCode==SNiP_20503_84_PSC ||m_iDgnCode==SNiP_20503_84_PSC_MKS)
						{
							if (i==0) { strBlock = _T("Block_Crack_normal_2b_TensStress");     rData.aBlockName.Add(strBlock); }
							else      { strBlock = _T("Block_Crack_normal_2b_TensStress_Bot"); rData.aBlockName.Add(strBlock); }
							if (InData.MatlD.dfck>35.0-cDGN_Zero)
							{
								if (i==0) { strBlock = _T("Block_Crack_normal_2b_B30_CompStress");     rData.aBlockName.Add(strBlock); }
								else      { strBlock = _T("Block_Crack_normal_2b_B30_CompStress_Bot"); rData.aBlockName.Add(strBlock); }
							}
							else
							{
								if (i==0) { strBlock = _T("Block_Crack_normal_2b_B35_CompStress"); rData.aBlockName.Add(strBlock); }
								else      { strBlock = _T("Block_Crack_normal_2b_B35_CompStress_Bot"); rData.aBlockName.Add(strBlock); }
							}
						}
						else if (m_iDgnCode==SP_35_13330_11_PSC || m_iDgnCode==SP_35_13330_11_PSC_MKS)
						{
							if (i==0) { strBlock = _T("Block_Crack_normal_2b");     rData.aBlockName.Add(strBlock); }
							else      { strBlock = _T("Block_Crack_normal_2b_Bot"); rData.aBlockName.Add(strBlock); }
						}

						strBlock = _T("Block_Title_Crack_width"); rData.aBlockName.Add(strBlock);
						if (i==0) 
						{
							if (rData.CrackD[i].bMixed==TRUE) strBlock = _T("Block_Crack_width");      
							else                              strBlock = _T("Block_Crack_width_TD_Top");
							rData.aBlockName.Add(strBlock); 
						}
						if (i==1) 
						{ 
							if (rData.CrackD[i].bMixed==TRUE) strBlock = _T("Block_Crack_width_Bottom");      
							else                              strBlock = _T("Block_Crack_width_TD_Bottom");
							rData.aBlockName.Add(strBlock);             
						}

						break;
					}
				case 2: // 3a
					{
						if (i==0) { strBlock = _T("Block_Crack_normal_3a");     rData.aBlockName.Add(strBlock); }
						else      { strBlock = _T("Block_Crack_normal_3a_Bot"); rData.aBlockName.Add(strBlock); }
						double dSigRbmc2Rat = rData.CrackD[i].dSigma_mc_Rb_rat;
						BOOL bIsInterpolation = (dSigRbmc2Rat>0.52 && dSigRbmc2Rat<0.8);
						if (InData.PscdD.iBrgType==0) // Highway and urban bridges
						{
							if (bIsInterpolation)
							{
								if (i==0) { strBlock = _T("Block_Crack_normal_3a_Highway3");     rData.aBlockName.Add(strBlock); }
								else      { strBlock = _T("Block_Crack_normal_3a_Highway3_Bot"); rData.aBlockName.Add(strBlock); }
							}
							else
							{
								if (dSigRbmc2Rat<0.52+cDgn_Zero)
								{
									if (i==0) { strBlock = _T("Block_Crack_normal_3a_Highway1");     rData.aBlockName.Add(strBlock); }
									else      { strBlock = _T("Block_Crack_normal_3a_Highway1_Bot"); rData.aBlockName.Add(strBlock); }
								}
								else
								{
									if (i==0) { strBlock = _T("Block_Crack_normal_3a_Highway2");     rData.aBlockName.Add(strBlock); }
									else      { strBlock = _T("Block_Crack_normal_3a_Highway2_Bot"); rData.aBlockName.Add(strBlock); }
								}
							}
						}
						else if (InData.PscdD.iBrgType==1) // Railroad bridge
						{
							if (bIsInterpolation)
							{
								if (i==0) { strBlock = _T("Block_Crack_normal_3a_Rail3");     rData.aBlockName.Add(strBlock); }
								else      { strBlock = _T("Block_Crack_normal_3a_Rail3_Bot"); rData.aBlockName.Add(strBlock); }
							}
							else
							{
								if (dSigRbmc2Rat<0.52+cDgn_Zero)
								{
									if (i==0) { strBlock = _T("Block_Crack_normal_3a_Rail1");     rData.aBlockName.Add(strBlock); }
									else      { strBlock = _T("Block_Crack_normal_3a_Rail1_Bot"); rData.aBlockName.Add(strBlock); }
								}
								else
								{
									if (i==0) { strBlock = _T("Block_Crack_normal_3a_Rail2");     rData.aBlockName.Add(strBlock); }
									else      { strBlock = _T("Block_Crack_normal_3a_Rail2_Bot"); rData.aBlockName.Add(strBlock); }
								}
							}
						}

						strBlock = _T("Block_Title_Crack_width"); rData.aBlockName.Add(strBlock);
						if (i==0) 
						{
							if (rData.CrackD[i].bMixed==TRUE) strBlock = _T("Block_Crack_width");      
							else                              strBlock = _T("Block_Crack_width_TD_Top");
							rData.aBlockName.Add(strBlock); 
						}
						if (i==1) 
						{ 
							if (rData.CrackD[i].bMixed==TRUE) strBlock = _T("Block_Crack_width_Bottom");      
							else                              strBlock = _T("Block_Crack_width_TD_Bottom");
							rData.aBlockName.Add(strBlock);             
						}

						break;
					}
				case 3: // 3b
				case 4: // 3c
					{
						strBlock = _T("Block_Title_Crack_width"); rData.aBlockName.Add(strBlock);
						if (i==0) 
						{
							if (rData.CrackD[i].bMixed==TRUE) strBlock = _T("Block_Crack_width");      
							else                              strBlock = _T("Block_Crack_width_TD_Top");
							rData.aBlockName.Add(strBlock); 
						}
						if (i==1) 
						{ 
							if (rData.CrackD[i].bMixed==TRUE) strBlock = _T("Block_Crack_width_Bottom");      
							else                              strBlock = _T("Block_Crack_width_TD_Bottom");
							rData.aBlockName.Add(strBlock);             
						}

						break;
					}
				default: ASSERT(0); break;
				}
			}
		}

	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FysgReportData_AS(ElemPairK ElemK, const T_FYSG_PSC_AS& InData, _FYSG_AASHTO& FysgGr, _FYSG_AASHTO& FysgSl)
{
	for (int i=0; i<4; ++i)
	{
		if (InData.FysgGrB[i].bCHK==TRUE) m_pPscManager->ConvertFysg_CivilToEngineAS(ElemK, InData.FysgGrB[i], FysgGr.FysgBase[i]);
		if (InData.bComposite)
		{
			if (InData.FysgSlB[i].bCHK==TRUE) m_pPscManager->ConvertFysg_CivilToEngineAS(ElemK, InData.FysgSlB[i], FysgSl.FysgBase[i]);
		}
	}
	return TRUE;  
}

BOOL CDgnPscDataCtrl::Get_TranReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_TRAN_PSC_AS_BASE& BaseD, PSC_DATA_POSI& InData, _TRAN_PSC_BASE& TranD)
{
    InData.CalcD.iBdcwCalcType = nMaxMin;
    m_pPscManager->ConvertTran_CivilToEngineAS(ElemK, BaseD, TranD);
    m_pPscManager->Calc_PscTransferStressAS(InData, TranD);

    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FycmReportData_AS(ElemPairK ElemK, const T_FYCM_PSC_AS& InData, _FYCM_AASHTO& FycmGr, _FYCM_AASHTO& FycmSl)
{
	for (int i=0; i<4; ++i)
	{
		if (InData.FycmGrB[i].bCHK==TRUE) m_pPscManager->ConvertFycm_CivilToEngineAS(ElemK, InData.FycmGrB[i], FycmGr.FycmBase[i]);
		if (InData.bComposite)
		{
			if (InData.FycmSlB[i].bCHK==TRUE) m_pPscManager->ConvertFycm_CivilToEngineAS(ElemK, InData.FycmSlB[i], FycmSl.FycmBase[i]);
		}
	}
	return TRUE;  
}

BOOL CDgnPscDataCtrl::Get_FpsgReportData_AS(ElemPairK ElemK, const T_FPMT_JTG& InData, _FPMT_AASHTO& rData)
{
	for (int i=0; i<4; ++i)
	{
		if(InData.FpmtBase[i].bCHK==TRUE) m_pPscManager->ConvertFpsg_CivilToEngineAS(ElemK, InData.FpmtBase[i], rData.FpmtBase[i]);
	}
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsReportData_AS(ElemPairK ElemK, const T_FPMS_PSC& InData, _FPMT_AASHTO& rData)
{
	for (int i=0; i<4; ++i)
	{
		if(InData.FpmsBase[i].bCHK==TRUE) m_pPscManager->ConvertFpms_CivilToEngineAS(ElemK, InData.FpmsBase[i], rData.FpmtBase[i]);
	}
	return TRUE;
}


BOOL CDgnPscDataCtrl::Get_BdcrReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_AS_BASE& BaseD, PSC_DATA_POSI& InData, _BDCY_PSC_LRFD_BASE& rData)
{
	InData.CalcD.iBdcrCalcType = nMaxMin;
	m_pPscManager->ConvertBdcr_CivilToEngineAS(ElemK, BaseD, rData);
	m_pPscManager->Calc_PscFlexureStrengthAS(InData, rData);
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_CumsReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_SHRR_PSC_AS_BASE& BaseD, PSC_DATA_POSI& InData, _CUMS_AASHTO_BASE& rData)
{
	m_pPscManager->ConvertCums_CivilToEngineAS(ElemK, BaseD, rData);
	m_pPscManager->Calc_PscShearTorsionStrengthAS(InData, rData);  
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_CrmtReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_TORR_PSC_AS_BASE& BaseD, PSC_DATA_POSI& InData, _STCM_AASHTO_BASE& rData)
{
	m_pPscManager->ConvertCrmt_CivilToEngineAS(ElemK, BaseD, rData);
	m_pPscManager->Calc_PscShearTorsionStrengthAS(InData, rData);  
	return TRUE;

}

//BOOL Get_FatiReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FATI_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FATI_SNIP_BASE& rData);
BOOL CDgnPscDataCtrl::Get_BdcwReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_PSC_AS_BASE& BaseD, PSC_DATA_POSI& InData, _BDCW_PSC_LRFD_BASE& rData)  
{
	InData.CalcD.iBdcwCalcType = nMaxMin;
	m_pPscManager->ConvertBdcw_CivilToEngineAS(ElemK, BaseD, rData);
	m_pPscManager->Calc_PscCrackCheckAS(InData, rData);  
	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcrReportData_BS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData)
{
    if ( nIJ<0 || nIJ>1 ) return FALSE;
    if ( nMaxMin<0 || nMaxMin>1 ) return FALSE;

    m_pPscManager->ConvertBdcr_CivilToEngineBS(ElemK, BaseD, rData);
    InData.CalcD.iBdcrCalcType = nMaxMin;
    m_pPscManager->Calc_PscFlexureStrengthBS(InData, rData);
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_CumsReportData_BS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_SHRR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE& rData)
{
    if ( nIJ<0 || nIJ>1 ) return FALSE;
    if ( nMaxMin<0 || nMaxMin>1 ) return FALSE;

    m_pPscManager->ConvertCums_CivilToEngineBS(ElemK, BaseD, rData);
    m_pPscManager->Calc_PscShearTorsionStrengthBS(InData, rData);  
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_CrmtReportData_BS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_TORR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE& rData)
{
    if ( nIJ<0 || nIJ>1 ) return FALSE;
    if ( nMaxMin<0 || nMaxMin>2 ) return FALSE;

    m_pPscManager->ConvertCrmt_CivilToEngineBS(ElemK, BaseD, rData);
    m_pPscManager->Calc_PscShearTorsionStrengthBS(InData, rData);  
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FysgDetailReportData_BS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYSG_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYSG_BASE &rData)
{
    PSC_STRS_CHK_POSI StrD;
    StrD.nChkType = 0; // 0=시공단계 검토, 1=완성계 검토, 2=시공단계, 완성계 검토
    if ( InData.nGirder == 0 )
    {
        m_pPscManager->ConvertFysg_CivilToEngineBS(ElemK, BaseD, StrD.Fysg[nMaxMin]);
    }
    else
    {
        m_pPscManager->ConvertFysg_CivilToEngineBS(ElemK, BaseD, StrD.FysgSl[nMaxMin]);
    }
    
    if ( InData.nGirder == 0 )
    {
        rData = StrD.Fysg[nMaxMin];
    }
    else
    {
        rData = StrD.FysgSl[nMaxMin];
    }

    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcrReportData_CS454(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData)
{
    m_pPscManager->ConvertBdcr_CivilToEngineBS(ElemK, BaseD, rData);
    InData.CalcD.iBdcrCalcType = nMaxMin;
    rData.strLcomName = GetRCaseName(BaseD.LcomK);
    rData.dGamma_f3 = GetGammaF(BaseD.LcomK);
    m_pPscManager->Calc_PscFlexureStrengthBS(InData, rData);
    rData.bChk = BaseD.bCHK;
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_CumsReportData_CS454(ElemPairK ElemK, int nIJ, int nMaxMin, const T_SHRR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE& rData)
{
    m_pPscManager->ConvertCums_CivilToEngineBS(ElemK, BaseD, rData);
    rData.strLcomName = GetRCaseName(BaseD.LcomK);
    rData.dGamma_f3 = GetGammaF(BaseD.LcomK);
    m_pPscManager->Calc_PscShearTorsionStrengthBS(InData, rData);
    rData.bCHK = BaseD.bCHK;
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_CrmtReportData_CS454(ElemPairK ElemK, int nIJ, int nMaxMin, const T_TORR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE& rData)
{
    m_pPscManager->ConvertCrmt_CivilToEngineBS(ElemK, BaseD, rData);
    rData.strLcomName = GetRCaseName(BaseD.LcomK);
    rData.dGamma_f3 = GetGammaF(BaseD.LcomK);
    m_pPscManager->Calc_PscShearTorsionStrengthBS(InData, rData);
    rData.bCHK = BaseD.bCHK;
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_ConcStressReportData_CS454(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYCM_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYCM_BASE &rData)
{
    PSC_STRS_CHK_POSI StrD;
    StrD.nChkType = 1; // 0=시공단계 검토, 1=완성계 검토, 2=시공단계, 완성계 검토

    m_pPscManager->ConvertFycm_CivilToEngineBS(ElemK, BaseD, StrD.Fycm[nMaxMin]);

    StrD.Fycm[nMaxMin].strLcomName = GetRCaseName(BaseD.LcomK);
    m_pPscManager->Calc_PscStressCheckBS(InData, StrD);

    rData = StrD.Fycm[nMaxMin];

    rData.bCHK = BaseD.bCHK;

    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FycmDetailReportData_BS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYCM_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYCM_BASE &rData)
{
    PSC_STRS_CHK_POSI StrD;
    StrD.nChkType = 1; // 0=시공단계 검토, 1=완성계 검토, 2=시공단계, 완성계 검토

    if ( InData.nGirder == 0)
    {
        m_pPscManager->ConvertFycm_CivilToEngineBS(ElemK, BaseD, StrD.Fycm[nMaxMin]);
    }
    else
    {
        m_pPscManager->ConvertFycm_CivilToEngineBS(ElemK, BaseD, StrD.FycmSl[nMaxMin]);
    }

    if (InData.nGirder == 0)
    {
        rData = StrD.Fycm[nMaxMin];
    }
    else
    {
        rData = StrD.FycmSl[nMaxMin];
    }

    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FysgReportData_BS(ElemPairK ElemK, const T_FYSG_PSC_BS& InData, PSC_FYSG_D& FysgGr, PSC_FYSG_D& FysgSl)
{
    for ( int i=0; i<4; ++i )
    {
        if ( InData.FysgGrB[i].bCHK==TRUE ) m_pPscManager->ConvertFysg_CivilToEngineBS(ElemK, InData.FysgGrB[i], FysgGr.FysgBase[i]);
        if ( InData.bComposite )
        {
            if ( InData.FysgSlB[i].bCHK==TRUE ) m_pPscManager->ConvertFysg_CivilToEngineBS(ElemK, InData.FysgSlB[i], FysgSl.FysgBase[i]);
        }
    }
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FycmReportData_BS(ElemPairK ElemK, const T_FYCM_PSC_BS& InData, PSC_FYCM_D& FycmGr, PSC_FYCM_D& FycmSl)
{
    for ( int i=0; i<4; ++i )
    {
        if ( InData.FycmGrB[i].bCHK==TRUE ) m_pPscManager->ConvertFycm_CivilToEngineBS(ElemK, InData.FycmGrB[i], FycmGr.FycmBase[i]);
        if ( InData.bComposite )
        {
            if ( InData.FycmSlB[i].bCHK==TRUE ) m_pPscManager->ConvertFycm_CivilToEngineBS(ElemK, InData.FycmSlB[i], FycmSl.FycmBase[i]);
        }
    }
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpsgReportData_BS(ElemPairK ElemK, const T_FPSG_PSC& InData, PSC_FPSG_D& rData)
{
    for ( int i=0; i<4; ++i )
    {
        if ( InData.FpsgBase[i].bCHK==TRUE ) m_pPscManager->ConvertFpsg_CivilToEngineBS(ElemK, InData.FpsgBase[i], rData.FpsgBase[i]);
    }
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsReportData_BS(ElemPairK ElemK, const T_FPMS_PSC& InData, PSC_FPMS_D& rData)
{
    for ( int i=0; i<4; ++i )
    {
        if ( InData.FpmsBase[i].bCHK==TRUE ) m_pPscManager->ConvertFpms_CivilToEngineBS(ElemK, InData.FpmsBase[i], rData.FpmsBase[i]);
    }
    return TRUE;
}


BOOL CDgnPscDataCtrl::Get_FypcReportData_BS(T_TDNA_K TdnaK, const T_FYPC_PSC_BS& BaseD, PSC_FYPC_D& rData)
{
    m_pPscManager->ConvertFypc_CivilToEngineBS(TdnaK, BaseD, rData);
    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcwReportData_BS(ElemPairK ElemK, const T_BDCW_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_BASE &rData)
{
    rData.strLcomName = GetRCaseName(BaseD.LcomK);
    m_pPscManager->ConvertBdcw_CivilToEngineBS(ElemK, BaseD, rData);
    m_pPscManager->Calc_PscCrackCheckBS(InData, rData);

    return TRUE;
}


BOOL CDgnPscDataCtrl::Check_Wmax_4Crack_Grade(int iExposureClass, int iLcomType, BOOL bPost,  BOOL bBonded, BOOL& bOK_Zero)
{
	// 도로교 한계상테 설계법 : 표 5.6.1, 5.6.2  

	int iCase =0;
	if      (bPost && bBonded)      iCase = 0;   // 포스트 텐션
	else if (!bPost)                iCase = 1;   // 프리 텐션
	else if (bPost && !bBonded)     iCase = 2;   // 비부착 프리스트레싱
	else                            iCase = 3;   // (예정) 철근 콘크리트

	// 최소설계등급 : 표 5.6.1
	int nGrade = 0;
	if(iCase==0)
	{
		switch(iExposureClass)
		{
		case Exposure_Class_EC1:
			nGrade = Exposure_Grade_D;
			break;
		case Exposure_Class_EC2:
		case Exposure_Class_EC3:
		case Exposure_Class_EC4:
			nGrade = Exposure_Grade_C;
			break;
		case Exposure_Class_ED1:
		case Exposure_Class_ED2:
		case Exposure_Class_ED3:
		case Exposure_Class_ES1:
		case Exposure_Class_ES2:
		case Exposure_Class_ES3:
			nGrade = Exposure_Grade_C;
			break;
		default:
			ASSERT(0);
			nGrade = Exposure_Grade_D;
		}  
	}
	else if(iCase==1)
	{
		switch(iExposureClass)
		{
		case Exposure_Class_EC1:
			nGrade = Exposure_Grade_D;
			break;
		case Exposure_Class_EC2:
		case Exposure_Class_EC3:
		case Exposure_Class_EC4:
			nGrade = Exposure_Grade_C;
			break;
		case Exposure_Class_ED1:
		case Exposure_Class_ED2:
		case Exposure_Class_ED3:
		case Exposure_Class_ES1:
		case Exposure_Class_ES2:
		case Exposure_Class_ES3:
			nGrade = Exposure_Grade_B;
			break;
		default:
			ASSERT(0);
			nGrade = Exposure_Grade_D;
		}  
	}
	else if(iCase == 2) 
	{
		nGrade = Exposure_Grade_E;
	}
	else 
	{
		ASSERT(0);
		nGrade = Exposure_Grade_E;
	}

	// 설계등급에 따른 하중조합 검토 : 표 5.6.2 
	//  (iLcomType== 1) (_T("사용하중I"));
	//  (iLcomType== 2) (_T("사용하중III/VI"));
	//  (iLcomType== 3) (_T("지속하중"));

	BOOL bOk = FALSE;

	switch(nGrade)
	{
	case Exposure_Grade_A:
		{
			bOk      = FALSE;
			bOK_Zero = (iLcomType== 1)? TRUE : FALSE;
			break;
		}
	case Exposure_Grade_B:
		{
			bOk      = (iLcomType== 1)? TRUE : FALSE;
			bOK_Zero = (iLcomType== 2)? TRUE : FALSE;
			break;
		}
	case Exposure_Grade_C:
		{
			bOk      = (iLcomType== 2)? TRUE : FALSE;
			bOK_Zero = (iLcomType== 3)? TRUE : FALSE;
			break;
		}
	case Exposure_Grade_D:
		{
			bOk      = (iLcomType== 2)? TRUE : FALSE;
			bOK_Zero = FALSE;
			break;
		}
	case Exposure_Grade_E:
		{
			bOk      = (iLcomType== 3)? TRUE : FALSE;
			bOK_Zero = FALSE;
			break;
		}
	default:
		{
			ASSERT(0);
			bOk = FALSE;
			bOK_Zero = FALSE;
		}
	}

	return bOk;
}

BOOL CDgnPscDataCtrl::Get_BdcrRatingData(ElemPairK ElemK, int nIJ, int nMaxMin, T_BDCR_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	int nType = nMaxMin;
	InData.CalcD.iBdcrCalcType = nType;
	m_pPscManager->ConvertBdcr_CivilToEngine(ElemK, BaseD, rData, FALSE);
	BOOL bCalcOK = m_pPscManager->Calc_PscFlexureStrength(nType, InData, rData);
	m_pPscManager->ConvertBdcr_EngineToCivil(rData, BaseD);

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_BdcrCompRatingData(ElemPairK ElemK, int nIJ, int nMaxMin, T_BDCR_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData)
{
	if(nIJ<0 || nIJ>1) return FALSE;
	if(nMaxMin<0 || nMaxMin>1) return FALSE;

	int nType = nMaxMin;
	InData.CalcD.iBdcrCalcType = nType;
	m_pPscManager->ConvertBdcrComp_CivilToEngine(ElemK, BaseD, rData, FALSE);
	BOOL bCalcOK = m_pPscManager->Calc_PscFlexureStrength(nType, InData, rData);
	m_pPscManager->ConvertBdcrComp_EngineToCivil(rData, BaseD);

	return bCalcOK;
}

//////////////////////////////////////////////////////////////////
//IRS START
/////////////////////////////////////////////////////////////////
BOOL CDgnPscDataCtrl::Get_FysgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_IRS& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	PSC_FYSG_BASE OutData;	

	BOOL bCalcOK=TRUE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);
	int i=0, j=0;
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		for(j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FysgBase[kk].bCHK==FALSE)  continue;

			T_STAG_K StagK = rData.FysgBase[kk].StageK;
			T_STAG_D StagD; StagD.Initialize();
			if(m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD))
			{
				T_ELFM_D DayActive; DayActive.Initialize();
				pDoc->m_pPostCtrl->GetElfmDgn(StagK, ElemK, DayActive);  // Stage의 시작과 끝에서 재료 날짜.			
				InData.dDuration = StagD.dDuration + DayActive.dStartMatAge;
			}

			InData.CalcD.iFysgCalcType = j; // 0=Compression, 1=Tension    
			m_pPscManager->ConvertFysg_CivilToEngine(ElemK, rData.FysgBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowbleStressBeforeLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFysg_EngineToCivil(OutData, rData.FysgBase[kk]);
		}
	}   
	return bCalcOK;    
}


BOOL CDgnPscDataCtrl::Get_FycmData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_IRS& rData)
{
	PSC_FYCM_BASE OutData;

	BOOL bCalcOK=TRUE;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	int i=0, j=0;
	BOOL bPreTndn[2]={TRUE,TRUE};
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{    
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;

		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FycmBase[i*2+j].bCHK==FALSE)  continue;
			//double dalComf, dalTenf;

			InData.CalcD.iFycmCalcType = j; // 0=Compression, 1=Tension
			m_pPscManager->ConvertFycm_CivilToEngine(ElemK, rData.FycmBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowbleStressAfterLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFycm_EngineToCivil(OutData, rData.FycmBase[kk]);      
		}
	}

	return bCalcOK;  
}

BOOL CDgnPscDataCtrl::Get_FpsgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC_IRS& rData)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	PSC_FPSG_BASE OutData;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpsgBase[kk].bCHK==FALSE)  continue;

			T_STAG_K StagK = rData.FpsgBase[kk].StageK;
			T_STAG_D StagD; StagD.Initialize();
			if(m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD))
			{
				T_ELFM_D DayActive; DayActive.Initialize();
				pDoc->m_pPostCtrl->GetElfmDgn(StagK, ElemK, DayActive);  // Stage의 시작과 끝에서 재료 날짜.			
				InData.dDuration = StagD.dDuration + DayActive.dStartMatAge;
			}

			InData.CalcD.iFpsgCalcType = j;
			m_pPscManager->ConvertFpsg_CivilToEngine(ElemK, rData.FpsgBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowblePrincipalStressBeforeLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFpsg_EngineToCivil(OutData, rData.FpsgBase[kk]);      
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FpmsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC_IRS& rData)
{
	PSC_FPMS_BASE OutData;

	DGN_JOINT_D JointD;
	m_pDataCtrl->Get_JointData4Element(ElemK, JointD);

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		PSC_DATA_POSI &InData = MembInD.Pos[i];
		// Joint
		InData.iJoint = i==0 ? JointD.iI_joint : JointD.iJ_joint;
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			int kk = i*2+j;
			if(rData.FpmsBase[kk].bCHK==FALSE)  continue;

			InData.CalcD.iFpmsCalcType = j;
			m_pPscManager->ConvertFpms_CivilToEngine(ElemK, rData.FpmsBase[kk], OutData);
			if(!m_pPscManager->Calc_PscAllowblePrincipalStressAfterLoss(j, InData, OutData)) continue;
			m_pPscManager->ConvertFpms_EngineToCivil(OutData, rData.FpmsBase[kk]);      
		}
	}

	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcwData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_IRS& rData, PSC_BDCW_D &rRes)
{
	PSC_BDCW_BASE OutData; 

	BOOL bCalcOK=TRUE;

	

	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);    
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		_PSC_PSCN_ELEM PscnElem;
		Get_PscPscnElem(ElemK, PscnElem);
		ConvertPscnData(i, PscnElem, InData);

		for(int j=0 ; j<2 ; j++) // 0=Top, 1=Bot
		{    
			BOOL bOK_Zero = TRUE;
			BOOL bOk_Cr   = TRUE;

			int kk = i*2+j;      
			int nType = j;
			InData.CalcD.iBdcwCalcType = nType;
			m_pPscManager->ConvertBdcw_CivilToEngine(ElemK, rData.BdcwBase[kk], OutData);

			if(m_iDgnCode == KSCE_LSD15_PSC || m_iDgnCode == KDS_24_14_21_2022_PSC )
			{
				int iExposureClass = (j==0)? InData.iExposureClass[0] : InData.iExposureClass[1];
				int iLcomType = OutData.iKind;
				BOOL bPost    = InData.TndnD.bPost;
				BOOL bBonded  = InData.TndnD.bBonded;

				bOk_Cr = Check_Wmax_4Crack_Grade(iExposureClass, iLcomType, bPost,  bBonded, bOK_Zero);
				//if(!bOk_Cr && !bOK_Zero) continue;
			}

			BOOL bCalcType = m_pPscManager->Calc_PscCrackCheck(nType, InData, OutData);

			m_pPscManager->ConvertBdcw_EngineToCivil(OutData, bOk_Cr, rData.BdcwBase[kk]);
			m_pPscManager->ConvertBdcw_EngineToCivil_zero(OutData, bOK_Zero, rData.BdcwBaseZero[kk]);

			rRes.BdcwBase[kk] = OutData;
		}
	}


	return TRUE;
}

BOOL CDgnPscDataCtrl::Get_BdcrData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_IRS& rData)
{
	PSC_FRCR_D OutData;

	BOOL bCalcOK=TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.BdcrBase[kk].bCHK==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);

			int nType = j;
			InData.CalcD.iBdcrCalcType = nType;
			m_pPscManager->ConvertBdcr_CivilToEngine(ElemK, rData.BdcrBase[kk], OutData);
			if(!m_pPscManager->Calc_PscFlexureStrength(nType, InData, OutData)) continue;  
			m_pPscManager->ConvertBdcr_EngineToCivil(OutData, rData.BdcrBase[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_CumsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, double adMr[4], T_CUMS_PSC_IRS& rData, PSC_CUMS_BASE& TempData)
{
	PSC_CUMS_BASE OutData;

	BOOL bCalcOK = TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];  

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.CumsBase[kk].bCHK==FALSE) continue;
            OutData = TempData;
			OutData.dMr = adMr[kk];
			int nType = j;
			m_pPscManager->ConvertCums_CivilToEngine(ElemK, rData.CumsBase[kk], OutData);
			if(!m_pPscManager->Calc_PscShearDesign(nType, InData, OutData)) continue;      
			m_pPscManager->ConvertCums_EngineToCivil(OutData, rData.CumsBase[kk]);      
		}
	}

	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_FatgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FATG_PSC_IRS& rData)
{
	PSC_FATG_BASE OutData;

	BOOL bCalcOK = TRUE;

	for(int i=0; i<2; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];  

		for(int j=0 ; j<2 ; j++) //Top, Bottom
		{
			int kk = i*2+j;
			if(rData.FatgBase[kk].bCHK==FALSE) continue;

			//OutData.dMr = adMr[kk];
			int nType = j;
			m_pPscManager->ConvertFatg_CivilToEngine(ElemK, rData.FatgBase[kk], OutData); // kk: 0=I_top 1=I_Bot 2=J_top 3=J_Bot
			if(!m_pPscManager->Calc_PscFatigeCheck(nType, InData, OutData)) continue;      
			m_pPscManager->ConvertFatg_EngineToCivil(OutData, rData.FatgBase[kk]);      
		}
	}

	return bCalcOK;
}



BOOL CDgnPscDataCtrl::Get_CrmtData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CRMT_PSC_IRS& rData)
{
	PSC_CRMT_BASE OutData;

	BOOL bCalcOK = TRUE;  

	for(int i=0; i<2; i++)//I End, J END
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		PSC_DATA_POSI &InData = MembInD.Pos[i];  

		for(int j=0 ; j<2 ; j++) //V Max,T Max
		{
			int kk = i*2+j;
			if(rData.CrmtBase[kk].bCHK==FALSE) continue;

			int nType = j;
			m_pPscManager->ConvertCrmt_CivilToEngine(ElemK, rData.CrmtBase[kk], OutData);
			if(!m_pPscManager->Calc_PscTorsionDesign(nType, InData, OutData)) continue;    
			m_pPscManager->ConvertCrmt_EngineToCivil(OutData, rData.CrmtBase[kk]);
		}
	}
	return bCalcOK;
}

BOOL CDgnPscDataCtrl::Get_FypcReportData(T_TDNA_K TdnaK, const T_FYPC_IRS& InData, PSC_FYPC_D& rData)
{
	if(InData.bCHK) m_pPscManager->ConvertFypc_CivilToEngine(TdnaK, InData, rData, TRUE);
	return TRUE;
}




///////////////////////////////////////////////////////////////
//IRS END
//////////////////////////////////////////////////////////////

CString CDgnPscDataCtrl::GetRCaseName(T_ALCS_K AlcsK)
{
    CString strName = _T("");
    T_ALCS_D AlcsD; AlcsD.Initialize();
    if (m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK,AlcsD)) // Rating case
    {
        strName = AlcsD.strCaseName;
    }
    return strName;
}

double CDgnPscDataCtrl::GetGammaF(T_ALCS_K AlcsK)
{
    double dGamma_f3 = 0.0;
    T_ALCS_D AlcsD; AlcsD.Initialize();
    if ( m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) // Rating case
    {
        dGamma_f3 = AlcsD.dGammaF3;
    }
    return dGamma_f3;
}

BOOL CDgnPscDataCtrl::Get_FycmDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYCM_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_FYCM_BASE& rData)
{
    if(nIJ<0 || nIJ>1) return FALSE;
    if(nMaxMin<0 || nMaxMin>1) return FALSE;

    BOOL bCalcOK=TRUE;

    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
    _PSC_SECG_ELEM SecgElem;
    if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
    _PSC_EXPC_ELEM ExpcElem;
    if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

    // Section g
    T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
    //
    ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);

    int nType = nMaxMin;
    InData.CalcD.iFycmCalcType = nType;
    m_pPscManager->ConvertFycm_CivilToEngine(ElemK, BaseD, rData, TRUE);        

    return TRUE;
}

BOOL CDgnPscDataCtrl::Get_FysgDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYSG_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_FYSG_BASE& rData)
{
    if(nIJ<0 || nIJ>1) return FALSE;
    if(nMaxMin<0 || nMaxMin>1) return FALSE;

    BOOL bCalcOK=TRUE;

    _PSC_MATL_ELEM MatlElem;
    if(!Get_PscMatlElem(ElemK, MatlElem)) return FALSE;
    _PSC_SECT_ELEM SectElem;
    if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
    _PSC_SECG_ELEM SecgElem;
    if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
    _PSC_RBAR_ELEM RbarElem;
    if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
    _PSC_TNDN_ELEM TndnElem;
    if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
    _PSC_EXPC_ELEM ExpcElem;
    if(!Get_PscExpcElem(ElemK, ExpcElem)) bCalcOK=FALSE;

    // Section g
    T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
    //
    ConvertPscCalcData(ElemK, nIJ, FALSE, &MatlElem, &SectElem.SectPosi[nIJ], &SpscgSect, &RbarElem.RbarPosi[nIJ], NULL, &TndnElem.TndnPosi[nIJ], &ExpcElem.ExpcPosi[nIJ], NULL, InData);

    int nType = nMaxMin;
    InData.CalcD.iFysgCalcType = nType;
    m_pPscManager->ConvertFysg_CivilToEngine(ElemK, BaseD, rData, TRUE);        

    return TRUE;
}