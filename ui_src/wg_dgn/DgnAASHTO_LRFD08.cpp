// DgnAASHTO_LRFD08.cpp: implementation of the CDgnAASHTO_LRFD08 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "DgnAASHTO_LRFD08.h"
#include "DgnBridgeLoadRating.h"

#include "CRCForceCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "Dgn_PSCManager.h"
#include "CRCDataCtrl.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnAASHTO_LRFD08::CDgnAASHTO_LRFD08()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_pPscManager = new CDgn_PSCManager;

}

CDgnAASHTO_LRFD08::~CDgnAASHTO_LRFD08()
{
	if(!m_pPscManager) 
	{
		delete m_pPscManager;
		m_pPscManager = NULL;
	}
}

void CDgnAASHTO_LRFD08::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);	

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
	CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_KIP;  
	if(CDBLib::IsPscCodeCSA(pDataCtrl->Get_CivilCode()))
	{
		CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
		CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
	}

	m_pDataCtrl->Set_UnitIndex(CodeUnit);
	//m_pDataCtrl->Get_CurrUnitIndex(CodeUnit);

    switch ( pDataCtrl->Get_CivilCode() )
    {
    case AASHTO_LRFD12_PSC_RATING:
    case AASHTO_LRFD19_PSC_RATING:
        Make_BridgeLoadRatingDgnData();
        break;
    default:
        Make_PscDgnData();
        break;
    }

	InitialData();
}

BOOL CDgnAASHTO_LRFD08::InitialData()
{
	/*
	HINSTANCE hDllCalc = GetOrLoad_DllHandle4PSC();
	if(hDllCalc == NULL) return FALSE;

	m_pBeforeLoss          = (PDGNCALC*)GetProcAddress(hDllCalc, "Get_AllowbleStress_Conc_BeforeLosses")); 
	if(!m_pBeforeLoss) return FALSE;

	m_pAfterLoss           = (PDGNCALC*)GetProcAddress(hDllCalc, "Get_AllowableStress_Conc_AfterLosses"));
	if(!m_pAfterLoss) return FALSE;

	m_pPrincipalBeforeLoss = (PDGNCALC*)GetProcAddress(hDllCalc, "Get_AllowablePrincipalStress_Conc_BeforeLosses"));
	if(!m_pPrincipalBeforeLoss) return FALSE;

	m_pPrincipalAfterLoss  = (PDGNCALC*)GetProcAddress(hDllCalc, "Get_AllowablePrincipalStress_Conc_AfterLosses"));
	if(!m_pPrincipalAfterLoss) return FALSE;

	m_pTendonStress        = (PDGNCALC*)GetProcAddress(hDllCalc, "Get_AllowableStress_Tendon"));
	if(!m_pTendonStress) return FALSE;
	
	m_pFlexureStrength     = (PDGNCALC*)GetProcAddress(hDllCalc, "Get_FlexureStrength"));
	if(!m_pFlexureStrength) return FALSE;

	m_pTensionRebarReq     = (PDGNCALC*)GetProcAddress(hDllCalc, "Get_TensionRebarReq"));
	if(!m_pTensionRebarReq) return FALSE;

	m_CrackCheck           = (PDGNCALC*)GetProcAddress(hDllCalc, "Get_CrackCheck"));
	if(!m_CrackCheck) return FALSE;

	m_ShearDesign          = (PDGNCALC*)GetProcAddress(hDllCalc, "Get_ShearDesign"));
	if(!m_ShearDesign) return FALSE;

	m_TorsionDesign        = (PDGNCALC*)GetProcAddress(hDllCalc, "Get_TorsionDesign"));
	if(!m_TorsionDesign) return FALSE;
	*/
	T_PSCD_D PscdD; PscdD.Initialize();
    T_BLRS_D BlrsD; BlrsD.Initialize();

    switch ( m_pDataCtrl->Get_CivilCode() )
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
    default:
        m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
        if ( !m_pPscManager->InitialData(DGNE_CODETYPE_PSC, PscdD.iDgnCode) ) return FALSE;
        ConvertPscParamData(PscdD, m_SPscdD);
        break;
    }

	return TRUE;
}


BOOL CDgnAASHTO_LRFD08::Get_FysgData(ElemPairK ElemK, T_FYSG_AASHTO& rData)
{
	return TRUE;
}

BOOL CDgnAASHTO_LRFD08::Get_FysgData(ElemPairK ElemK, T_FYSG_D& rData)
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

BOOL CDgnAASHTO_LRFD08::Get_FysgData(int nGirder, ElemPairK ElemK, T_FYSG_D& rData)
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


BOOL CDgnAASHTO_LRFD08::Get_FypcData(T_TDNA_K TdnaK, T_FYPC_AASHTO& rData)
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

BOOL CDgnAASHTO_LRFD08::Get_FycmData(ElemPairK ElemK, T_FYCM_AASHTO& rData)
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

BOOL CDgnAASHTO_LRFD08::Get_FycmData(int nGirder, ElemPairK ElemK, T_FYCM_AASHTO& rData)
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

BOOL CDgnAASHTO_LRFD08::Get_FssgData(ElemPairK ElemK, T_FPMT_JTG& rData)
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

BOOL CDgnAASHTO_LRFD08::Get_FpmsData(ElemPairK ElemK, T_FPMS_JTG& rData)
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

BOOL CDgnAASHTO_LRFD08::Get_BdcwData(ElemPairK ElemK, T_BDCW_AASHTO& rData, _BDCW_AASHTO* pRes, int nPrtUnit)
{
	PSC_DATA_POSI InData;
	_BDCW_PSC_LRFD_BASE OutData; OutData.Initialize();

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
	else if(bComposite) //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else ASSERT(0);

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

BOOL CDgnAASHTO_LRFD08::Get_TbarData(ElemPairK ElemK, T_TBAR_AASHTO& rData)
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
	else if(bComposite) //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
	}
	else ASSERT(0);

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

BOOL CDgnAASHTO_LRFD08::Get_BdcyData(ElemPairK ElemK, T_BDCY_AASHTO& rData, _BDCY_AASHTO* pRes, int nPrtUnit)
{
	PSC_DATA_POSI InData; InData.Initialize();
	_BDCY_PSC_LRFD_BASE OutData; OutData.Initialize();

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}

    T_STCT_D StctD; StctD.Initialize();
    m_pDoc->m_pAttrCtrl->GetStct(StctD);  

    T_STAG_K nGdLastStagK=0;
    int nLastStepNo=0;
    m_pForcCtrl->GetGirderLastStageKey(ElemK, nGdLastStagK, nLastStepNo);

	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);
	BOOL bCompositeGen = m_pDataCtrl->IsCompositeGenPSCSect(ElemK);
	if(!bComposite)
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else if(bComposite) //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else ASSERT(0);

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

	// rp
	// PS강재의 종류(0=저릴랙세이션(0.28), 1=응력제거강재(0.4), 2=강봉(0.55))
	//int iTndnKind = PscdD.iTndnKind;
	//double drp = 0.0;    
	//Get_TndnKindrp(iTndnKind, drp);

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

		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], NULL, &TndnElem.TndnPosi[i], NULL, NULL, InData);
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

            // 시공중 거더 Force
            _DGN_FORC_CRC FcsDL;
            int nForceBeforeType=1;
            int nDgnLcomNo = rData.BdcyBase[kk].LcomK;
            if (!m_pForcCtrl->Get_GirderForce4CSLoadOfLCOM(FALSE, StctD, ElemK, nGdLastStagK, nLastStepNo,  nDgnLcomNo, nForceBeforeType, FcsDL, 1.0, TRUE))
            {
                CString strSgldName = _LS(IDS_DB_SGLD_DL);	// DL.
                T_SGLD_K SgldK = m_pDoc->m_pAttrCtrl->GetSgldKey(strSgldName);

                double dFactor = 1.0;
                _DGN_FORC_CRC ForcMinTemp;
                m_pForcCtrl->Get_CompositeForceBySgld(ElemK, SgldK, nGdLastStagK, nLastStepNo, dFactor, FcsDL, ForcMinTemp);
            }
            rData.BdcyBase->dMdnc = FcsDL.dMuy[i];
			
			int nType = j;
			m_pPscManager->ConvertBdcy_CivilToEngineAASHTO(rData.BdcyBase[kk], OutData);
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
			if(pRes) pRes->BdcyBase[kk] = OutData;
			
			//if(!Get_FlexureStrength(TRUE, bPosiI, bIter, bPositive, dPhib, MatlElem, SpscSect, arOuter, arpInner, SpscgSect, 
			//                        drp, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcrKsce.BdcyBase[k])) continue;
		}
	}
	
	return bCalcOK;
}

BOOL CDgnAASHTO_LRFD08::Get_CumsData(ElemPairK ElemK, T_CUMS_AASHTO& rData, _CUMS_AASHTO* pRes, int nPrtUnit)
{
	PSC_DATA_POSI InData;
	_CUMS_AASHTO_BASE OutData; OutData.Initialize();
	
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
	else if(bComposite) //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else ASSERT(0);
	
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

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
		
		ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], &RshrElem.RshrPosi[i], &TndnElem.TndnPosi[i], NULL, NULL, InData);

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int kk = i*2+j;
			if(rData.CumsBase[kk].bCHK==FALSE) continue;
			
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

BOOL CDgnAASHTO_LRFD08::Get_CrmtData(ElemPairK ElemK, T_CRMT_AASHTO& rData, _CRMS_AASHTO* pRes, int nPrtUnit)
{
	PSC_DATA_POSI InData;
	_STCM_AASHTO_BASE OutData; OutData.Initialize();

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
	else if(bComposite) //khd AASHTO_LRFD12 Composite 단면 추가
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem)) return FALSE;
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else ASSERT(0);

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

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

	  ConvertPscCalcData(ElemK, i, FALSE, &MatlElem, &SectElem.SectPosi[i], &SpscgSect, &RbarElem.RbarPosi[i], &RshrElem.RshrPosi[i], &TndnElem.TndnPosi[i], NULL, NULL, InData);

		for(int j=0 ; j<3 ; j++) //Vmax, Vmin, Tmax
		{
			int kk = i*3+j;
			if(rData.CrmtBase[kk].bCHK==FALSE) continue;

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

