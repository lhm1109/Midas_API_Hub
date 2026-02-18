// DgnPscDataCtrl.cpp: implementation of the CDgnPscDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnSodDataCtrl.h"
#include "CRCForceCtrl.h"
#include "Dgn_OrthotropicDeckManager.h"
#include "DgnDataCtrl.h"
#include "DgnForceCtrl.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\PlateGirderDesign.h"
#include "..\wg_base\wg_base_I_PolyMaker.h"
#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_db\StlOrthDeckDesign.h"

#include "Dgn_DllManager_Base.h"
#include "..\dgnengine\idesign\DGN_link\ITmpDgnDrawLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnSodDataCtrl::CDgnSodDataCtrl() : CDgnPscCommon()
{ 
	//T_CPGD_D CpgdD; CpgdD.Initialize();  
	//m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);
	
	m_nDgnCode = m_iCivilCode;

	m_pSodManager = new CDgn_OrthotropicDeckManager;
	
//   m_mElemSpanK.RemoveAll();
// 	m_mElemSpanD.RemoveAll();
//   m_mCsgMatl.RemoveAll();
//   m_mCsgSect.RemoveAll();
//   m_mCsgTrst.RemoveAll();
}

CDgnSodDataCtrl::~CDgnSodDataCtrl()
{
	if(m_pSodManager!=NULL) 
	{
		delete m_pSodManager;
		m_pSodManager = NULL;
	}
}

void CDgnSodDataCtrl::SetDataCtrlPointer(CCRCDataCtrl *pDataCtrl)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);

//   T_CPGD_D CpgdD; CpgdD.Initialize();  
//   m_pDoc->m_pAttrCtrl2->GetDgnCpgd(CpgdD);

	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;
	switch (m_nDgnCode)
	{
		case SNiP_20503_84_SOD:
		case SP_35_13330_11_SOD:
			CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
			CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
			m_pDataCtrl->Set_UnitIndex(CodeUnit);
			break;
		default: ASSERT(0); break;
	}
	
	InitialData(m_nDgnCode);

}

BOOL CDgnSodDataCtrl::InitialData(int nDgnCode)
{
	if(!m_pSodManager->InitialData(DGNE_CODETYPE_SOD, nDgnCode)) return FALSE;

//   T_PSCD_D PscdD; PscdD.Initialize();
//   m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
//   
//   ConvertPscParamData(PscdD, m_SPscdD);
	
	return TRUE;
}

void CDgnSodDataCtrl::SetSodDgnCode(int nDgnCode)
{
	m_nDgnCode = nDgnCode;
}

void CDgnSodDataCtrl::SetChkSodDgnElem(CArray<T_ELEM_K,T_ELEM_K> &raElemK)
{
	if (raElemK.GetSize()>0) m_pDataCtrl->ConvertElemKeyToElemPairKey(raElemK, m_arElemK);
}

BOOL CDgnSodDataCtrl::Make_SodDgnData()
{
	int nReturn=0;	

	// Set arElemK	
	m_arElemK.RemoveAll();
	CArray<T_ELEM_K,T_ELEM_K> arElemK;  
	m_pDoc->m_pAttrCtrl2->GetSddpKeyList(arElemK);  
	CDBLib::GetElemforStlOrthDeck(arElemK);

	m_pDataCtrl->ConvertElemKeyToElemPairKey(arElemK, m_arElemK);
// 	if(m_pDataCtrl->Get_ElemListForPlateGirder(TRUE, arElemK))
// 	{
//     int nElemSize = arElemK.GetSize();
// 		for(int i=0; i<nElemSize; ++i)
// 		{
// 			T_ELEM_K ElemK = arElemK.GetAt(i);
//       //T_CGDP_D
// 			BOOL bSaveElemK = m_pDoc->m_pAttrCtrl->ExistCgdp(ElemK);			
// 			// Save ElemK.
// 			if(bSaveElemK)	m_arElemK.Add(ElemK);
// 		}
//     if(m_arElemK.GetSize()==0)  m_arElemK.Copy(arElemK);
// 	}
// 
//   Make_ElemSpanMapKey();
// 	Make_ElemSpanData();
// 
// 

	Make_ElemSpanData();
	GetDgnLengD();

	 if(nReturn==0)  nReturn = (Make_SodMatlDgnData()        ? 0 : 1);  
	 if(nReturn==0)  nReturn = (Make_SodSectDgnData()        ? 0 : 2);
	 if(nReturn==0)  nReturn = (Make_SodKappaFactorDgnData() ? 0 : 3);	
	 if(nReturn==0)  nReturn = (Make_SodFatiCheckPos()       ? 0 : 5);
	 if(nReturn==0)  nReturn = (Make_SodLocalForce4Deck()    ? 0 : 6);
	 if(nReturn==0)  nReturn = (Make_SodFloorBeamDgnData()   ? 0 : 7);
	 if(nReturn==0)  nReturn = (Make_SodWebPlateDgnData()    ? 0 : 8);
	 //if(nReturn==0)  nReturn = (Make_SodWebVerticalForce()   ? 0 : 9);
	 if(nReturn==0)  nReturn = (Make_SodSectCheckPart()      ? 0 :10);

	if(nReturn!=0)  return FALSE;

	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_SodMatlDgnData()
{
	CArray<T_SDMS_K, T_SDMS_K> aSdmsKey;
	m_pDoc->m_pAttrCtrl2->GetSdmsKeyList(aSdmsKey);
	int nSdmsSize = aSdmsKey.GetSize();
	for (int nSdms=0; nSdms<nSdmsSize; ++nSdms )
	{
		T_SDMS_K SdmsK = aSdmsKey[nSdms];
		T_SDMS_D SpmsD; SpmsD.Initialize();
		if (m_pDoc->m_pAttrCtrl2->GetSdms(SdmsK, SpmsD))
		{
			SLT_MATL_SNiP_POSD SectMatlD;
			for (int i=0; i<3; ++i)
			{
				T_MATD_D MatdD; MatdD.Initialize();
				if (m_pDoc->m_pAttrCtrl->GetMatlDesign(SpmsD.Part[i], MatdD))
				{          
					if (i==0) ConvertSodMatlProp(MatdD, SectMatlD.MatTopF);
					if (i==1) ConvertSodMatlProp(MatdD, SectMatlD.MatWeb);
					if (i==2) ConvertSodMatlProp(MatdD, SectMatlD.MatBotF);
				}
			}

			for (int i=0; i<3; ++i)
			{
				T_MATD_D MatdD; MatdD.Initialize();
				if (m_pDoc->m_pAttrCtrl->GetMatlDesign(SpmsD.Stiff[i], MatdD))
				{          
					if (i==0) ConvertSodMatlProp(MatdD, SectMatlD.MatTopF_Rib);
					if (i==1) ConvertSodMatlProp(MatdD, SectMatlD.MatWeb_Rib);
					if (i==2) ConvertSodMatlProp(MatdD, SectMatlD.MatBotF_Rib);
				}
			}
			m_mSodMatlSect.SetAt(SdmsK, SectMatlD);
		}
	}
	
	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_SodSectDgnData()
{
	m_mSodSect.RemoveAll();
	m_mSodSectElem.RemoveAll();

	//m_pMembCtrl->GetElemListByIncludeElem
	CArray<T_ELEM_K,T_ELEM_K> aCheckElemK;
	m_pDataCtrl->GetElemKeyList4ElemPair(EN_EL_BEAM, m_arElemK, aCheckElemK);
	int nChkElemSize = aCheckElemK.GetSize();

	T_SODSECT_STFN_INX     SectStfnId;
	STL_SECT_ELEMD_SNiP_OD SectElem;

	for(int i=0; i<nChkElemSize; ++i)
	{		
		T_ELEM_K ElemK = aCheckElemK[i];

		T_ELEM_D ElemD, ElemPreD;
	  ElemD.Initialize();
		ElemPreD.Initialize();
	  BOOL bGetElem    = m_pDoc->m_pPostCtrl->GetElemPost(ElemK,ElemD);	  
		BOOL bGetElemPre = m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemPreD);	  
	  if(!bGetElem || !bGetElemPre)	continue;
	  
	  T_SECT_K SectK = ElemD.elpro;
		T_SECT_K SectPreK = ElemPreD.elpro;

		if (m_mSodSect.Lookup(SectK, SectElem)) continue; // 있으면...
		SectElem.Initialize();
		SectStfnId.Initialize();

		T_TSGR_K TsgrK = m_pDoc->m_pAttrCtrl->GetElemTsgrKey(ElemK);

		T_SECT_D SectD;  
		
		if (TsgrK>0)
		{
			T_SECT_D TSectD;
			m_pDoc->m_pAttrCtrl->GetSect(SectPreK, TSectD);
			
			T_TSGR_D TsgrD;
			double dTotalLength = 0.0;
			double dPositionI   = 0.0;
			double dPositionJ   = 0.0;
			if (m_pDoc->m_pAttrCtrl->GetTsgr(TsgrK, TsgrD))
			{
				double dTsgrTotLength = 0.0;
				m_pDoc->m_pAttrCtrl->CalcTsgrTotLength(TsgrD, ElemK, dTsgrTotLength, dPositionI);
//         int nTsgrElemList = TsgrD.aElemList.GetSize();
//         for(int i=0; i<nTsgrElemList; ++i)
//         {
//           if(TsgrD.aElemList[i]==ElemK) dPositionI = dTsgrTotLength;
//           dTsgrTotLength += m_pDoc->calcLAVElem(TsgrD.aElemList[i]);
//         }
				dPositionJ = dPositionI + m_pDoc->calcLAVElem(ElemD);        

				m_pDoc->m_pSectDB->CalcTaperedSection(TSectD, TsgrD, dTsgrTotLength, dPositionI, dPositionJ, 0, SectD);
			}
		}
		else
		{
			m_pDoc->m_pPostCtrl->GetSectPost(SectK,SectD);
		}    

		T_STPO_D StpoD;
		CSectUtil::GetStressPointSOD_Stiffener(SectD, StpoD);

		int nSectType = 0;
//     if(SectD.nStype == D_SECT_TYPE_TAPERED && SectD.SectBefore.nStype == D_SECT_TYPE_COMPO_G) nSectType = D_SECT_TYPE_COMPO_G;
//     else if(SectD.nStype == D_SECT_TYPE_COMPO_G) nSectType = D_SECT_TYPE_COMPO_G;
//     else if (SectD.nStype == D_SECT_TYPE_TAPERED)
//     {
//       nSectType = SectD.SectBefore.nStype;      
//     }
//     else nSectType = SectD.nStype;

		BOOL bTapered = SectD.nStype == D_SECT_TYPE_TAPERED ? TRUE : FALSE;
		
		for(int j=0; j<2; ++j)
		{
			BOOL bPosiI = (j==0);
			//if (bTapered==FALSE && j==1) continue;
			T_SECT_SECTBASE_D &SectBaseD = (!bPosiI && bTapered && j==1) ? SectD.CmpTapJ : SectD.SectBefore.SectI;      

			//STL_SECT_INFO_OD &CsgSPos = SectElem.SectPos[j];
			//CsgSPos.SectInfo.bIsTapered = bTapered;

			// Composite General Section
			{

				BOOL bCalcOK = TRUE;//(bStiffn && bStiffCrack && bStiff3n && bStiffReba3n);

			  // Set Sect (0=I, 1=J).
			  if(bCalcOK)
			  {         
									 
					ConvertSodSectData(j, &SectD, StpoD, SectStfnId.Stiff[j], SectElem.SectD[j]);    

					T_SECT_STIFF_D SectStiff;
					SectStiff.aStiffShape.Copy(SectBaseD.SODStiffener.aStiffShape);
					CalcStiffnessOfStiffener(SectStiff, SectElem.SectD[j].SectOD.Stiffener);
				}        
			}
		}
		// Set Sect.
		m_mSodSect.SetAt(SectK, SectElem);
		m_SectStfnIdx.SetAt(SectK, SectStfnId);
	}


	if(m_mSodSect.GetCount()==0) return FALSE;  

	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_SodKappaFactorDgnData()
{
	CArray<T_SDPS_K, T_SDPS_K> aSdpsKey;
	m_pDoc->m_pAttrCtrl2->GetSdpsKeyList(aSdpsKey);
	int nSdpsSize = aSdpsKey.GetSize();
	if(nSdpsSize!=0)
	{
		for (int i=0; i<nSdpsSize; ++i)
		{
			T_SDPS_K SdpsK = aSdpsKey[i];
			T_SDPS_D SpdsD; SpdsD.Initialize();
			if (m_pDoc->m_pAttrCtrl2->GetSdps(SdpsK, SpdsD))
			{
				STL_KAPPA_INPUT_D KappaInD;      
				ConvertSodKappaFactor(SpdsD.SdpsSub[0], KappaInD);
				m_mSodKappaFactor.SetAt(SdpsK, KappaInD);
				
				STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD ChkKappaPoint;
				ConvertSodKappaFactorChkPos(SpdsD.SdpsSub[0], ChkKappaPoint);
				m_mSodKappaChkPos.SetAt(SdpsK, ChkKappaPoint);
				
				if (SpdsD.bSameIJ==FALSE) 
				{
					ConvertSodKappaFactor(SpdsD.SdpsSub[1], KappaInD);        
					ConvertSodKappaFactorChkPos(SpdsD.SdpsSub[1], ChkKappaPoint);
				}
				
				m_mSodKappaFactorJ.SetAt(SdpsK, KappaInD);
				m_mSodKappaChkPosJ.SetAt(SdpsK, ChkKappaPoint);
			}
		} 
	}
	else // Default 값 세팅
	{
		// Set arElemK	
		CArray<T_ELEM_K,T_ELEM_K> arElemK;  
		m_pDoc->m_pAttrCtrl2->GetSddpKeyList(arElemK);  
		CDBLib::GetElemforStlOrthDeck(arElemK);
		
		int nSize = arElemK.GetSize();

		for (int i=0; i<nSize; ++i)
		{
			T_ELEM_K ElemK = arElemK[i];

			T_ELEM_D ElemD;	ElemD.Initialize();
			if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK,ElemD)) ASSERT(0);		 
			
			T_SECT_K SectK = ElemD.elpro;

			T_SECT_D SectD;
			m_pDoc->m_pPostCtrl->GetSectPost(SectK,SectD);
			//m_pDoc->m_pAttrCtrl->GetSect(SectPreK, TSectD);

			CArray<T_POINT_SSM, T_POINT_SSM&> aStressPoint;

			STL_KAPPA_INPUT_D KappaInD;     
			KappaInD.bAutoCalc   = TRUE;
			KappaInD.dUDKappa_y  = 1.0;
			KappaInD.dUDKappa_z  = 1.0;
			m_mSodKappaFactor.SetAt(ElemK, KappaInD);
			m_mSodKappaFactorJ.SetAt(ElemK, KappaInD);

			STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD ChkKappaPoint;
			aStressPoint.RemoveAll();
			CSectUtil::GetStressPointSOD(SectD, 0, 0, aStressPoint, TRUE);  //  default point 가져오기		
			ChkKappaPoint.Initialize();
			ConvertSodKappaFactorChkPosDefault(aStressPoint, ChkKappaPoint);
			m_mSodKappaChkPos.SetAt(ElemK, ChkKappaPoint);

			aStressPoint.RemoveAll();
			CSectUtil::GetStressPointSOD(SectD, 0, 1, aStressPoint, TRUE);  //  default point 가져오기		
			ChkKappaPoint.Initialize();
			ConvertSodKappaFactorChkPosDefault(aStressPoint, ChkKappaPoint);
			m_mSodKappaChkPosJ.SetAt(ElemK, ChkKappaPoint);
		}

	}




	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_SodKappaFactorChkPos()
{
	//ConvertSodKappaFactorChkPosOne();
	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_SodFatiCheckPos()
{
	CArray<T_SDFC_K, T_SDFC_K> aSdfcKey;
	m_pDoc->m_pAttrCtrl2->GetSdfcKeyList(aSdfcKey);
	int nSdfcSize = aSdfcKey.GetSize();
	for (int i=0; i<nSdfcSize; ++i)
	{
		T_SDFC_K SdfcK = aSdfcKey[i];
		T_SDFC_D SdfcD; SdfcD.Initialize();
		if (m_pDoc->m_pAttrCtrl2->GetSdfc(SdfcK, SdfcD))
		{
			STL_POSD_DGND_SNiP_FAT_CHK_INFO_OD FatChkInfo;      
			ConvertSodFatiCheckPos(0, SdfcD, FatChkInfo);
			m_mSodFatiChkPos.SetAt(SdfcK, FatChkInfo);
			
			
			if (SdfcD.bSameIJ==FALSE) 
			{
				ConvertSodFatiCheckPos(1, SdfcD, FatChkInfo);
			}
			
			m_mSodFatiChkPosJ.SetAt(SdfcK, FatChkInfo);
		}
	}
	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_SodLocalForce4Deck()
{
	CArray<T_SDLF_K, T_SDLF_K> aSdlfKey;
	m_pDoc->m_pAttrCtrl2->GetSdlfKeyList(aSdlfKey);
	int nSdlfSize = aSdlfKey.GetSize();
	for (int i=0; i<nSdlfSize; ++i)
	{
		T_SDLF_K SdlfK = aSdlfKey[i];
		T_SDLF_D SdlfD; SdlfD.Initialize();

		T_ELEM_D ElemD; ElemD.Initialize();
		BOOL bGetElemPre = m_pDoc->m_pAttrCtrl->GetElem(SdlfK, ElemD);
		if (m_pDoc->m_pAttrCtrl2->GetSdlf(SdlfK, SdlfD))
		{
			STL_LFORCE_RIBDECK_CHKUNIT_LCOM LForRibDeck;
			ConvertSodLocalForce4Deck(ElemD.elpro, 0, SdlfD, LForRibDeck);
			m_mSodLForRibDeck.SetAt(SdlfK, LForRibDeck);
			//
			LForRibDeck.Initialize();
			ConvertSodLocalForce4Deck(ElemD.elpro, 1, SdlfD, LForRibDeck);
			m_mSodLForRibDeckJ.SetAt(SdlfK, LForRibDeck);
		}
	}

	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_SodFloorBeamDgnData()
{
	
	CArray<T_SDTS_K, T_SDTS_K> aSdtsKey;
	m_pDoc->m_pAttrCtrl2->GetSdtsKeyList(aSdtsKey);
	int nSdtsSize = aSdtsKey.GetSize();
	for (int i=0; i<nSdtsSize; ++i)
	{
		T_SDTS_K SdtsK = aSdtsKey[i];
		T_SDTS_D SdtsD; SdtsD.Initialize();
		if (m_pDoc->m_pAttrCtrl2->GetSdts(SdtsK, SdtsD))
		{      
			STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D StaFbeamD;      
			
	  bool bOK = ConvertSodFloorBeam(0, SdtsD, StaFbeamD);

	  if(bOK)
	  {
		m_mSodFloorBeam.SetAt(SdtsK, StaFbeamD);
	  }
			
			if (SdtsD.bBeamSameIJ && bOK) 
			{
		m_mSodFloorBeamJ.SetAt(SdtsK, StaFbeamD);
			}
	  else if(!SdtsD.bBeamSameIJ)
	  {
		  StaFbeamD.Initialize();
				if(ConvertSodFloorBeam(1, SdtsD, StaFbeamD))
		{
			m_mSodFloorBeamJ.SetAt(SdtsK, StaFbeamD);
		}
	  }
		}
	}

	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_SodWebPlateDgnData()
{  
	CArray<T_SDAP_K, T_SDAP_K> aSdapKey;
	m_pDoc->m_pAttrCtrl2->GetSdapKeyList(aSdapKey);
	int nSdapSize = aSdapKey.GetSize();
	for (int i=0; i<nSdapSize; ++i)
	{
		T_SDAP_K SdapK = aSdapKey[i];
		T_SDAP_D SdapD; SdapD.Initialize();
		if (m_pDoc->m_pAttrCtrl2->GetSdap(SdapK, SdapD))
		{    
			T_ELEM_D ElemD; ElemD.Initialize();
			if(!m_pDoc->m_pPostCtrl->GetElemPost(SdapK, ElemD)) continue;
			
			T_SECT_D SectD;
			m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro,SectD);
			int nStype = SectD.nStype;
			if (SectD.nStype==D_SECT_TYPE_TAPERED) nStype = SectD.SectBefore.nStype;

			STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM  LforWeb;
			STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM LforWebP;
			ConvertSodWebVerticalForce(nStype, 0, SdapD, LforWeb, LforWebP);
			m_mSodLForWeb.SetAt(SdapK, LforWeb);
			m_mSodLForWebPlate.SetAt(SdapK, LforWebP);
			
			//
			LforWeb.Initialize();
			LforWebP.Initialize();
			ConvertSodWebVerticalForce(nStype, 1, SdapD, LforWeb, LforWebP);      
			m_mSodLForWebJ.SetAt(SdapK, LforWeb);
			m_mSodLForWebPlateJ.SetAt(SdapK, LforWebP);
		}
	}

	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_SodWebVerticalForce()
{
	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_SodSectCheckPart()
{
	return TRUE;
}



BOOL CDgnSodDataCtrl::Check_StlOrthotropicDeck()
{
	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	if (pCheckSod==NULL) return FALSE;

	Make_SodDgnData();
	
	CArray<T_ELEM_K,T_ELEM_K> arCheckElemK; 
	//m_pDataCtrl->GetPgdElemKeyList(arCheckElemK);
	//int nChkElemSize = arCheckElemK.GetSize();
	int nChkElemSize = m_arElemK.GetSize();

	// Pre-Combined Composite Bridge
	BOOL bIsPLCB = m_pDoc->m_pAttrCtrl->ExistPlcb(); 

	T_SODC_ITEM ChkItem;
	int nLcomNum = m_pForcCtrl->Get_LcomDataCount();
	int nStrnLcomSize = m_pForcCtrl->Get_LcomNumForStrn();
	
	m_pForcCtrl->m_pDgnForceCtrl->m_iDgnMembAxis = 3;

	// Span 정보 계산 
	//m_pDataCtrl->Calc_LengthPosition4Span(arCheckElemK);

	CDgnDataCtrl DgnCtrl;
	STL_MEMB_POSD_SNiP_OD MembLcomD;  
	STL_MEMB_RES_SNiP_OD  ResD;

	m_pSodManager->GetTableValueSNiP(MembLcomD.TBval);
	ConvertSODGlobalData(MembLcomD.Memb.GDgn);

	for (int i=0; i<nChkElemSize; ++i)
	{        
	
		auto ElemK = m_arElemK[i];

		T_ELEM_D ElemD; ElemD.Initialize();
		if(!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD)) continue;

		T_SECT_D SectD;
		m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro,SectD);
		int nStype = SectD.nStype;
		if (SectD.nStype==D_SECT_TYPE_TAPERED) nStype = SectD.SectBefore.nStype;

		m_pForcCtrl->Set_ElemDataForCheck(ElemK);
	
		BOOL bChkIJ[2]={FALSE};
		T_SDDP_D SddpD; SddpD.Initialize();
		m_pDoc->m_pAttrCtrl2->GetSddp(ElemK.first, SddpD);  
		bChkIJ[0] = (SddpD.aSddpSub[0].GetSize()>0);
		bChkIJ[1] = (SddpD.aSddpSub[1].GetSize()>0);  
	
		STL_SECT_ELEMD_SNiP_OD SodSect;
		if (m_mSodSect.Lookup(ElemD.elpro, SodSect))
		{
			MembLcomD.Memb.PosD[0].SectInfo = SodSect.SectD[0];
			MembLcomD.Memb.PosD[1].SectInfo = SodSect.SectD[1];
			//MembLcomD.Memb.PosD[0].bSupprotNode = bSupprotNode[0];
			//MembLcomD.Memb.PosD[1].bSupprotNode = bSupprotNode[1];
		}
		else ASSERT(0);
		
//     T_KFAC_D KFacD; KFacD.Initialize();
//     T_KFAC_D KAutD; KAutD.Initialize();
//     BOOL bGetKfac = DgnCtrl.Get_DgnGenKfac(ElemK, KFacD, KAutD);    
//     Get_UnbracedLengthData(ElemK, bGetKfac, KFacD, MembLcomD.Memb.Ulen);

		T_SDLS_NUM_ELEM SdlsNumD;
		pCheckSod->GetLStfnAndPanelElem(ElemK.first, SdlsNumD);
		int nLSTopNum  = SdlsNumD.LsD[0].nTopNum + SdlsNumD.LsD[1].nTopNum;
		int nLSBotNum  = SdlsNumD.LsD[0].nBotNum + SdlsNumD.LsD[1].nBotNum;
		int nLSWebNum  = SdlsNumD.LsD[0].nWebNum + SdlsNumD.LsD[1].nWebNum;
		int nPanTopNum = SdlsNumD.LsD[0].nPanTNum + SdlsNumD.LsD[1].nPanTNum;
		int nPanBotNum = SdlsNumD.LsD[0].nPanBNum + SdlsNumD.LsD[1].nPanBNum;
		int nPanWebNum = SdlsNumD.LsD[0].nPanWNum + SdlsNumD.LsD[1].nPanWNum;    

		// 절점별 데이타
		Get_SodPosData(ElemK.first, ElemD.elpro, nStype, 0, MembLcomD.Memb);
		Get_SodPosData(ElemK.first, ElemD.elpro, nStype, 1, MembLcomD.Memb);

		int nChkLcomSize=1;
		MembLcomD.aUlmf.SetSize(nChkLcomSize);
		MembLcomD.aChkFor.SetSize(nChkLcomSize);
		
		STL_RES_BASE_SNiP_OD_SNiP MembResD[2]; // I,J

		// Get Design Length Info.
		T_EDLK_D DgnLengD;
		m_mDgnLengD.Lookup(ElemK.first, DgnLengD);
		for(int IJ=0; IJ<2; ++IJ) ConvertSodDgnLengthData(DgnLengD, MembResD[IJ]);

		// Make Elem Base Data
        MembLcomD.Memb.ChIf.nChkPos = 0;
		m_pSodManager->Check_MemberResSNiP(MembLcomD, MembResD[0]);
		MembLcomD.Memb.ChIf.nChkPos = 1;
		m_pSodManager->Check_MemberResSNiP(MembLcomD, MembResD[1]);

		ADGNFORCE aForceLcom;
		ADGNFORCE aForceTnpr;
		ADGNFORCE aForceTnsc;
		ADGNFORCE aForceLTerm;
		ADGNFORCE aForceSTerm;
		ADGNSHRSADD aStssLcom;
		if (!m_pForcCtrl->Get_LcomDataForDesign(ElemK, TRUE, TRUE, aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm, aForceSTerm, aStssLcom, nLcomNum)) return FALSE;

		int nForceLcomSize = aForceLcom.GetSize();  

	int nFlgStrn[2]={0};
	double dFlgSNRibMaxRat[4]={0.0};
	double dFlgSNDeckMaxRat[4]={0.0};
	double dFlgSNFBeamMaxRat[4]={0.0};

	double dWebSNRibMaxRatL[2]={0.0};
	double dWebSNDeckMaxRatL[2]={0.0};
	double dWebSNDeckNoMMaxRatL[2]={0.0};
	double dWebSNDeckVonMaxRatL[2]={0.0};

	double dWebSNRibMaxRatR[2]={0.0};
	double dWebSNDeckMaxRatR[2]={0.0};
	double dWebSNDeckNoMMaxRatR[2]={0.0};
	double dWebSNDeckVonMaxRatR[2]={0.0};

	//

	double dFlgStabFBMaxRat[2]={0.0};
	double dFlgStabFTBMaxRat[2]={0.0};
	double dFlgStabGDMaxRat[2]={0.0};
	double dFlgStabRibMaxRat[4]={0.0};
	double dFlgStabDeckGMaxRat[4]={0.0};
	double dFlgStabDeckLMaxRat[4]={0.0};
	double dFlgStabDeckMaxRat[4]={0.0};
	double dFlgStabFBeamGMaxRat[4]={0.0};
	double dFlgStabFBeamLMaxRat[4]={0.0};
	double dFlgStabFBeamMaxRat[4]={0.0};
	double dWebStabRibMaxRatL[2]={0.0};
	double dWebStabDeckMaxRatL[2]={0.0};
	double dWebStabRibMaxRatR[2]={0.0};
	double dWebStabDeckMaxRatR[2]={0.0};

	int nBFSNRib[4]={0};
	double dBFSNRibMaxRat[4]={0.0};


		T_SDSN_FLG_CR FlgStrnCrR;
		T_SDSN_WEB_CR WebStrnCrR;
		T_SDSB_FLG_CR FlgStabCrR;
		T_SDSB_WEB_CR WebStabCrR;

	T_SDSB_GD_CR  FBStabCrR;	// Flexural Buckling Cr
	T_SDSB_GD_CR  FTBStabCrR;	// Flexural Torsional Buckling Cr
	T_SDSB_GD_CR  GDStabCrR;

		int nDgnLcomNo = 0;
		int nOrgLcomNo = 0;
		int nSerLcomCount = 0;
		int nMaxMinType = 0;    
		CString strOrgLcomNa=_T("");

		// Kappa Check Start
		BOOL   bAllKappaF_I = TRUE, bAllKappaF_J = TRUE;
		double dCheckKappa_Rat_Max_I = 0.0,  dCheckKappa_Rat_Max_J = 0.0;
		aKAPAF_PonitInfo KAPAF_PonitInfo_I, KAPAF_PonitInfo_J; KAPAF_PonitInfo_I.RemoveAll(); KAPAF_PonitInfo_J.RemoveAll();
		
		STLCHKFOR_ARRAY aChkForceI;  aChkForceI.RemoveAll(); 
		STLCHKFOR_ARRAY aChkForceJ;  aChkForceJ.RemoveAll();
		STLCHKFOR_ARRAY aChkDLForceI;  aChkDLForceI.RemoveAll();
		STLCHKFOR_ARRAY aChkDLForceJ;  aChkDLForceJ.RemoveAll();

		CArray<int, int> aOrgLcomNo; aOrgLcomNo.RemoveAll();
		for(int n = 0; n < nForceLcomSize; ++n) // new make
		{
			int nDgnLcomNo = n + 1;
			if(!m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)){ASSERT(0);}
			
			int nOrgLcomNum = aOrgLcomNo.GetSize();
			BOOL bOverlap = FALSE;
			for(int a = 0; a < nOrgLcomNum; ++a)
			{
				if(aOrgLcomNo[a] == nOrgLcomNo)
				{
					bOverlap = TRUE;
					break;
				}
			}
			if(!bOverlap)
			{
				aOrgLcomNo.Add(nOrgLcomNo);    
			}
		}

		int nOrgLcomNum = aOrgLcomNo.GetSize();
		for(int b = 0;  b < nOrgLcomNum; ++b)
		{  
			STLCHKFOR_ARRAY aChkForceItemp;  aChkForceItemp.RemoveAll();
			STLCHKFOR_ARRAY aChkForceJtemp;  aChkForceJtemp.RemoveAll();
			STLCHKFOR_ARRAY aChkDLForceItemp;  aChkDLForceItemp.RemoveAll();
			STLCHKFOR_ARRAY aChkDLForceJtemp;  aChkDLForceJtemp.RemoveAll();

			int nLcomSize = 0, nDgnLcomKFirst = 0;
			int nOrgLcomCur = aOrgLcomNo[b];
			GetLcom4Kapa(nOrgLcomCur, aForceLcom, aForceLTerm, aChkForceItemp, aChkForceJtemp, aChkDLForceItemp, aChkDLForceJtemp, nLcomSize, nDgnLcomKFirst); 
			aChkForceI.Append(aChkForceItemp);
			aChkForceJ.Append(aChkForceJtemp);
			aChkDLForceI.Append(aChkDLForceItemp);
			aChkDLForceJ.Append(aChkDLForceJtemp);
		}
		
		STLCHKFOR_ARRAY aChkForce;   aChkForce.RemoveAll();
	STLCHKFOR_ARRAY aChkDLForce; aChkDLForce.RemoveAll();

		// I
	aChkForce.Copy(aChkForceI);
	aChkDLForce.Copy(aChkDLForceI);
	ConvertSodCalcChkForce4Kapa(aChkForce, aChkDLForce,  MembLcomD);
	BOOL bAllKappaF_I_tmp = TRUE; double dCheckKappa_Rat_Max_I_tmp = 0.0;
	m_pSodManager->Check_KappaF_StlOrthDeckSNiP(MembLcomD, MembResD[0], bAllKappaF_I_tmp, dCheckKappa_Rat_Max_I, KAPAF_PonitInfo_I);    

	// J
	aChkForce.RemoveAll();
	aChkDLForce.RemoveAll();
	aChkForce.Copy(aChkForceJ);
	aChkDLForce.Copy(aChkDLForceJ);
	ConvertSodCalcChkForce4Kapa(aChkForce, aChkDLForce,  MembLcomD); 
	BOOL bAllKappaF_J_tmp = TRUE; double dCheckKappa_Rat_Max_J_tmp = 0.0;
	m_pSodManager->Check_KappaF_StlOrthDeckSNiP(MembLcomD, MembResD[1], bAllKappaF_J_tmp, dCheckKappa_Rat_Max_J, KAPAF_PonitInfo_J);

		// Kappa Check End

		nDgnLcomNo    = 0;
		nOrgLcomNo    = 0;
		nSerLcomCount = 0;
		nMaxMinType   = 0;    
		strOrgLcomNa  = _T("");
		
		int nServLcomType=0;
		for(int j=0; j<nForceLcomSize; ++j)
		{
			int nDgnLcomNo = j+1;
			if(!m_pForcCtrl->Get_LcomType(nDgnLcomNo, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType))
			{
				ASSERT(0);
			}		    
 
			BOOL bULSLcom = m_pForcCtrl->Is_LcomForStrn(nDgnLcomNo);
			BOOL bSLSLcom = m_pForcCtrl->Is_LcomForStrs(nDgnLcomNo);   
			BOOL bFLSLcom = m_pForcCtrl->Is_LcomForFati(nDgnLcomNo);
			
			if (bFLSLcom) continue;

	  // kapa 계산을 위해 하중조합에 포함된 전체 케이스를 넘겨준다.
	  STLCHKFOR_ARRAY aChkForceI;
	  STLCHKFOR_ARRAY aChkForceJ;
	  STLCHKFOR_ARRAY aChkDLForceI;
	  STLCHKFOR_ARRAY aChkDLForceJ;
	  int nLcomSize =0;
	  int nDgnLcomKFirst=0;
	  GetLcom4Kapa(nOrgLcomNo, aForceLcom, aForceLTerm, aChkForceI, aChkForceJ, aChkDLForceI, aChkDLForceJ, nLcomSize, nDgnLcomKFirst);

			_DGN_FORC_MEMB ForMemb;
			m_pForcCtrl->GetDgnMemberForce(ElemK, nDgnLcomNo, ForMemb);
			ConvertSodMemberForce(ForMemb, MembLcomD.aUlmf[0]);
			MembLcomD.aChkFor[0].nLcomType = 0;
			
			_DGN_FORC_CRC &ForceLcom =  aForceLcom.GetAt(j);
			_DGN_FORC_CRC &ForceLT   = aForceLTerm[j];
			
			_DGN_FORC_CRC ForceST = ForceLcom;
			ForceST -= ForceLT;      
			
//       MembLcomD.aChkFor[0].Initialize();
//       CSG_FORCE_COMP &ComLcomF = MembLcomD.aChkFor[0];
//       if      (bULSLcom==TRUE) ComLcomF.nLcomType = 0;
//       else if (bSLSLcom==TRUE) 
//       {
//         ComLcomF.nLcomType = 1;
//         ComLcomF.nLcomSub  = nServLcomType;
//       }      
//       ComLcomF.nMax = nMaxMinType;
			
			
//       if (bIsSpanForI || bIsSpanForJ) 
//       {        
//         _DGN_FORC_CRC &SpanForI = aForceEndI[j];
//         _DGN_FORC_CRC &SpanForM = aForceMid [j];
//         _DGN_FORC_CRC &SpanForJ = aForceEndJ[j];
//         ConvertCsgCalcUlmForce(nPosI,nPosM,nPosJ, SpanForI, SpanForM, SpanForJ, MembLcomD.aUlmf[0]);
//         
//       }

	  T_SODD_DGN_FORCE SodForce;
	  T_SDSN_FLG_CASE StrnFlgT;
	  T_SDSN_FLG_CASE StrnFlgB;
	  T_SDSN_WEB_CASE StrnWebD;
	  T_SDSB_FLG_CASE StabFlgT;
	  T_SDSB_FLG_CASE StabFlgB;
	  T_SDSB_WEB_CASE StabWebD;
	  T_SDSB_GD_CASE  StabFB;	// Stability - Flexural Bucking
	  T_SDSB_GD_CASE  StabFTB;	// Stability - Flexural torsional Bucking
	  T_SDSB_GD_CASE  StabGD;	// Stability - Lateral torsional Bucking
	  T_SDCR_BASE LcomKeyD;
	  LcomKeyD.nLcomK      = nOrgLcomNo;
	  LcomKeyD.nDgnLcomK   = nDgnLcomNo;
	  LcomKeyD.nConcurrent = nMaxMinType;
	  LcomKeyD.nLcomSize   = nLcomSize;
	  LcomKeyD.nDgnLcomKFirst =nDgnLcomKFirst;
	  //LcomKeyD.nMaxMin = ???;

	  double dWBStrnDeckRatI[2]    = {0.0}; // L, R
	  double dWBStrnDeckNoMRatI[2] = {0.0}; // L, R
	  double dWBStrnDeckVonRatI[2] = {0.0}; // L, R
	  double dWBStrnRibRatI[2]     = {0.0}; // L, R

	  double dWBStrnDeckRatJ[2]    = {0.0}; // L, R
	  double dWBStrnDeckNoMRatJ[2] = {0.0}; // L, R
	  double dWBStrnDeckVonRatJ[2] = {0.0}; // L, R
	  double dWBStrnRibRatJ[2]     = {0.0}; // L, R

	  double dWBStabRibRatI[2]    = {0.0}; // L, R
	  double dWBStabDeckRatI[2]    = {0.0}; // L, R

	  double dWBStabRibRatJ[2]    = {0.0}; // L, R
	  double dWBStabDeckRatJ[2]    = {0.0}; // L, R	

			for (int k=0; k<2; ++k) // I,J
			{
				if(bChkIJ[k]==FALSE) continue;
				MembLcomD.Memb.ChIf.nChkPos = k;
				
				STL_FORCE_D ForOD;
				ConvertSodCheckForce(k, ForceLcom, ForOD);        // 설계용
				ConvertSODDFORCE(k, ForceLcom, SodForce.Elem[k]); // 저장용
		ConvertSODDFORCE(k, ForceLT, SodForce.ElemDL[k]); // 저장용

		STLCHKFOR_ARRAY aChkForce;   aChkForce.RemoveAll();
		STLCHKFOR_ARRAY aChkDLForce; aChkDLForce.RemoveAll();

		if(k==0) 
		{
			aChkForce.Copy(aChkForceI);
			aChkDLForce.Copy(aChkDLForceI);
		}
		else 
		{
			aChkForce.Copy(aChkForceJ);
			aChkDLForce.Copy(aChkDLForceJ);
		}
		ConvertSodCalcChkForce(ForOD, aChkDLForce, MembLcomD);  
		ConvertSodCalcChkForce4Kapa(aChkForce, aChkDLForce,  MembLcomD); // kapa 계산을 위한 하중조합 타입 추가.
		GetSodFlexuralBucklingData(ElemK.first, k, nDgnLcomNo, MembLcomD); // Flexural Buckling Check를 위한 데이터 컨버팅 

				// ULS
				if (bULSLcom==TRUE)
				{
					Get_LocalForceData(ElemK.first, nOrgLcomNo, k, MembLcomD);

					STL_MEMB_RES_STR_STA_SNiP_OD StrnStabD;
					
		  double dTFStrnRibRat  = 0.0;
		  double dBFStrnRibRat  = 0.0;
		  double dTFStrnDeckRat = 0.0;
		  double dBFStrnDeckRat = 0.0;

		  double dTFStrnFBRat   = 0.0;
		  double dBFStrnFBRat   = 0.0;

		  //
		  double dStbFBRat		= 0.0;
		  double dStbFTBRat		= 0.0;

		  double dStbGDRat		= 0.0;
		  double dTFStabRibRat  = 0.0;
		  double dBFStabRibRat  = 0.0;
		  double dTFStabDeckGRat = 0.0;
		  double dTFStabDeckLRat = 0.0;
		  double dTFStabDeckRat  = 0.0;
		  double dBFStabDeckGRat = 0.0;
		  double dBFStabDeckLRat = 0.0;
		  double dBFStabDeckRat  = 0.0;
		  double dTFStabFBGRat   = 0.0;
		  double dTFStabFBLRat   = 0.0;
		  double dTFStabFBRat    = 0.0;
		  double dBFStabFBGRat   = 0.0;
		  double dBFStabFBLRat   = 0.0;
		  double dBFStabFBRat    = 0.0;

					if (m_nDgnCode==SNiP_20503_84_SOD || m_nDgnCode==SP_35_13330_11_SOD)
					{
						// Kappa Factor setting for all load combination 
						BOOL bAllKappaF = TRUE; aKAPAF_PonitInfo KAPAF_PonitInfo; KAPAF_PonitInfo.RemoveAll();
						if     (k == 0){bAllKappaF = bAllKappaF_I; KAPAF_PonitInfo.Copy(KAPAF_PonitInfo_I);}
						else if(k == 1){bAllKappaF = bAllKappaF_J; KAPAF_PonitInfo.Copy(KAPAF_PonitInfo_J);}
						else{ASSERT(0);}

						StrnStabD.OD_Res_SNiP_StrengthD.KAPAF_PonitInfo.Copy(KAPAF_PonitInfo);
						StrnStabD.OD_Res_SNiP_StrengthD.KappaF.bAllChk = bAllKappaF;
						StrnStabD.OD_Res_SNiP_StabilityD.KAPAF_PonitInfo.Copy(KAPAF_PonitInfo);   
						StrnStabD.OD_Res_SNiP_StabilityD.KappaF.bAllChk = bAllKappaF;

						m_pSodManager->Check_StlOrthDeckSNiP(MembLcomD, MembResD[k], StrnStabD);
						//dRatio_F = StrnResD.FlexD.dRatio_F;
						//dRatio_S = StrnResD.ShearD.dRatio_S;
						ConvertSodSdsnFlg(nStype, TRUE,  MembResD[k], StrnStabD, StrnFlgT.StrnFlg[k], ChkItem.bSodCheck[0]); // strength - Top Flange  , ChkItem은 결과가 하나라도 있으면 TRUE
						ConvertSodSdsnFlg(nStype, FALSE, MembResD[k], StrnStabD, StrnFlgB.StrnFlg[k], ChkItem.bSodCheck[1]); // strength - Bot Flange
						ConvertSodSdsnWeb(nStype,        MembResD[k], StrnStabD, StrnWebD.StrnWeb[k], ChkItem.bSodCheck[2]); // strength - Web

			ConvertSodSdsbFB (nStype,        MembResD[k], StrnStabD, StabFB.StabGD[k],    ChkItem.bSodCheck[3]); // Stability - Flexural Bucking
			ConvertSodSdsbFTB(nStype,        MembResD[k], StrnStabD, StabFTB.StabGD[k],   ChkItem.bSodCheck[3]); // Stability - Flexural torsional Bucking
			ConvertSodSdsbGD (nStype,        MembResD[k], StrnStabD, StabGD.StabGD[k],    ChkItem.bSodCheck[3]); // Stability - Lateral torsional Bucking
						ConvertSodSdsbFlg(nStype, TRUE,  MembResD[k], StrnStabD, StabFlgT.StabFlg[k], ChkItem.bSodCheck[4]); // Stability - Top Flange - Global, Local, 
						ConvertSodSdsbFlg(nStype, FALSE, MembResD[k], StrnStabD, StabFlgB.StabFlg[k], ChkItem.bSodCheck[5]); // Stability - Bot Flange 
						ConvertSodSdsbWeb(nStype,        MembResD[k], StrnStabD, StabWebD.StabWeb[k], ChkItem.bSodCheck[6]); // Stability - Web
					}
					else ASSERT(0);

		  // Critical 값 찾기
		  // *********************
		  // Top
		  // *********************
		  T_SDSN_FLG  &StrnFlgTop = StrnFlgT.StrnFlg[k];  
		  T_SDSB_FLG  &StabFlgTop = StabFlgT.StabFlg[k];
		  for(int nPart=0; nPart<3; ++nPart) // L,M,R
		  {
			  // -Strength
			  T_SDPT_STRN_FLG &StrnChk = StrnFlgTop.StrnChk[nPart];
			  dTFStrnRibRat  =  max(dTFStrnRibRat,  StrnChk.dRat_Rib);
			  dTFStrnDeckRat =  max(dTFStrnDeckRat, StrnChk.dRat_Deck);
			  dTFStrnFBRat   =  max(dTFStrnFBRat,   StrnChk.dRat_FB);

			  // -Stability
			  T_SDPT_STAB_FLG &StabChk = StabFlgTop.StabChk[nPart];
			  dTFStabRibRat   = max(dTFStabRibRat,   StabChk.dRat_Rib);
			  dTFStabDeckGRat = max(dTFStabDeckGRat, StabChk.dRat_DeckG);
			  dTFStabDeckLRat = max(dTFStabDeckLRat, StabChk.dRat_DeckL);
			  dTFStabDeckRat  = max(dTFStabDeckRat,  StabChk.dRat_Deck);
			  dTFStabFBGRat   = max(dTFStabFBGRat,   StabChk.dRat_FBG);
			  dTFStabFBLRat   = max(dTFStabFBLRat,   StabChk.dRat_FBL);
			  dTFStabFBRat    = max(dTFStabFBRat,    StabChk.dRat_FB);

		  }

		  // *********************
		  // Bottom
		  // *********************
		  T_SDSN_FLG  &StrnFlgBot = StrnFlgB.StrnFlg[k];  
		  T_SDSB_FLG  &StabFlgBot = StabFlgB.StabFlg[k];
		  for(int nPart=0; nPart<3; ++nPart) // L,M,R
		  {
			  // -Strength
			  const T_SDPT_STRN_FLG &StrnChk = StrnFlgBot.StrnChk[nPart];
			  dBFStrnRibRat  =  max(dBFStrnRibRat,  StrnChk.dRat_Rib);
			  dBFStrnDeckRat =  max(dBFStrnDeckRat, StrnChk.dRat_Deck);
			  dBFStrnFBRat   =  max(dBFStrnFBRat,   StrnChk.dRat_FB);

			  // -Stability
			  const T_SDPT_STAB_FLG &StabChk = StabFlgBot.StabChk[nPart];
			  dBFStabRibRat   = max(dBFStabRibRat,   StabChk.dRat_Rib);
			  dBFStabDeckGRat = max(dBFStabDeckGRat, StabChk.dRat_DeckG);
			  dBFStabDeckLRat = max(dBFStabDeckLRat, StabChk.dRat_DeckL);
			  dBFStabDeckRat  = max(dBFStabDeckRat,  StabChk.dRat_Deck);
			  dBFStabFBGRat   = max(dBFStabFBGRat,   StabChk.dRat_FBG);
			  dBFStabFBLRat   = max(dBFStabFBLRat,   StabChk.dRat_FBL);
			  dBFStabFBRat    = max(dBFStabFBRat,    StabChk.dRat_FB);
		  }

		  // *********************
		  // Web
		  // *********************
		  T_SDSN_WEB  &StrnWeb  = StrnWebD.StrnWeb[k];  
		  T_SDSB_WEB  &StabWeb  = StabWebD.StabWeb[k];  

		  for(int nPart=0; nPart<2; ++nPart) // L,R
		  {
			  // -Strength
			  T_SDPT_STRN_WEB &WebStrnD = StrnWeb.WebD[nPart];

			  // -Stability
			  T_SDPT_STAB_WEB &WebStabD = StabWeb.WebD[nPart];

			  if(k==0)//I
			  {
				  dWBStrnRibRatI[nPart]     = max(dWBStrnRibRatI[nPart],     WebStrnD.dRat_Rib);
				  dWBStrnDeckNoMRatI[nPart] = max(dWBStrnDeckNoMRatI[nPart], WebStrnD.dRat_DeckNoM);
				  dWBStrnDeckVonRatI[nPart] = max(dWBStrnDeckVonRatI[nPart], WebStrnD.dRat_DeckVon);
				  dWBStrnDeckRatI[nPart]    = max(dWBStrnDeckRatI[nPart],    WebStrnD.dRat_Deck);

				  dWBStabRibRatI[nPart]  = max(dWBStabRibRatI[nPart],  WebStabD.dRat_Rib);
				  dWBStabDeckRatI[nPart] = max(dWBStabDeckRatI[nPart], WebStabD.dRat_Deck);
			  }
			  else  //J
			  {
				  dWBStrnRibRatJ[nPart]     = max(dWBStrnRibRatJ[nPart],     WebStrnD.dRat_Rib);
				  dWBStrnDeckNoMRatJ[nPart] = max(dWBStrnDeckNoMRatJ[nPart], WebStrnD.dRat_DeckNoM);
				  dWBStrnDeckVonRatJ[nPart] = max(dWBStrnDeckVonRatJ[nPart], WebStrnD.dRat_DeckVon);
				  dWBStrnDeckRatJ[nPart]    = max(dWBStrnDeckRatJ[nPart],    WebStrnD.dRat_Deck);

				  dWBStabRibRatJ[nPart]  = max(dWBStabRibRatJ[nPart],  WebStabD.dRat_Rib);
				  dWBStabDeckRatJ[nPart] = max(dWBStabDeckRatJ[nPart], WebStabD.dRat_Deck);
			  }
		  }

		  // *********************
		  // Global Deck_Stability
		  // *********************
		  T_SDSB_GD &FBStab		=  StabFB.StabGD[k];
		  T_SDSB_GD &FTBStab	=  StabFTB.StabGD[k];
		  T_SDSB_GD &GDStab		=  StabGD.StabGD[k];
		  dStbFBRat		= FBStab.dRat_GD;
		  dStbFTBRat	= FTBStab.dRat_GD;
		  dStbGDRat		= GDStab.dRat_GD;

		  int nTop  = 2*k;
		  int nBot  = 2*k+1;

		  // - Strength
		  GetCrFlgStrnRibRatio  (nFlgStrn[k], LcomKeyD, dTFStrnRibRat,  dBFStrnRibRat,       dFlgSNRibMaxRat[nTop],   dFlgSNRibMaxRat[nBot],   FlgStrnCrR.RibCr[nTop],   FlgStrnCrR.RibCr[nBot]);
		  GetCrFlgStrnDeckRatio (nFlgStrn[k], LcomKeyD, dTFStrnDeckRat, dBFStrnDeckRat,      dFlgSNDeckMaxRat[nTop],  dFlgSNDeckMaxRat[nBot],  FlgStrnCrR.DeckCr[nTop],  FlgStrnCrR.DeckCr[nBot]);
		  GetCrFlgStrnFBeamRatio(nFlgStrn[k], LcomKeyD, dTFStrnFBRat,   dBFStrnFBRat,        dFlgSNFBeamMaxRat[nTop], dFlgSNFBeamMaxRat[nBot], FlgStrnCrR.FBeamCr[nTop], FlgStrnCrR.FBeamCr[nBot]);
		  // - Strength : Web-Left - rib, Deck(max), Deck(Shear stress), Deck(Combined stress),
		  if(k==0)
		  {
			  GetCrWebStrnRibRatio  (nFlgStrn[k], LcomKeyD, dWBStrnRibRatI[0],     dWebSNRibMaxRatL[k],     WebStrnCrR.RibCrL[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckRatI[0],    dWebSNDeckMaxRatL[k],    WebStrnCrR.DeckCrL[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckNoMRatI[0], dWebSNDeckNoMMaxRatL[k], WebStrnCrR.DeckNoMCrL[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckVonRatI[0], dWebSNDeckVonMaxRatL[k], WebStrnCrR.DeckVonCrL[k]);
			  // - Strength : Web-Right
			  GetCrWebStrnRibRatio  (nFlgStrn[k], LcomKeyD, dWBStrnRibRatI[1],     dWebSNRibMaxRatR[k],     WebStrnCrR.RibCrR[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckRatI[1],    dWebSNDeckMaxRatR[k],    WebStrnCrR.DeckCrR[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckNoMRatI[1], dWebSNDeckNoMMaxRatR[k], WebStrnCrR.DeckNoMCrR[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckVonRatI[1], dWebSNDeckVonMaxRatR[k], WebStrnCrR.DeckVonCrR[k]);
		  }
		  else
		  {
			  GetCrWebStrnRibRatio  (nFlgStrn[k], LcomKeyD, dWBStrnRibRatJ[0],     dWebSNRibMaxRatL[k],     WebStrnCrR.RibCrL[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckRatJ[0],    dWebSNDeckMaxRatL[k],    WebStrnCrR.DeckCrL[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckNoMRatJ[0], dWebSNDeckNoMMaxRatL[k], WebStrnCrR.DeckNoMCrL[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckVonRatJ[0], dWebSNDeckVonMaxRatL[k], WebStrnCrR.DeckVonCrL[k]);
			  // - Strength : Web-Right
			  GetCrWebStrnRibRatio  (nFlgStrn[k], LcomKeyD, dWBStrnRibRatJ[1],     dWebSNRibMaxRatR[k],     WebStrnCrR.RibCrR[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckRatJ[1],    dWebSNDeckMaxRatR[k],    WebStrnCrR.DeckCrR[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckNoMRatJ[1], dWebSNDeckNoMMaxRatR[k], WebStrnCrR.DeckNoMCrR[k]);
			  GetCrWebStrnDeckRatio (nFlgStrn[k], LcomKeyD, dWBStrnDeckVonRatJ[1], dWebSNDeckVonMaxRatR[k], WebStrnCrR.DeckVonCrR[k]);
		  }

		  // Stability
		  // - FB, FTB, LTB
		  GetCrGDStabRatio      (nFlgStrn[k], LcomKeyD, dStbFBRat,      dFlgStabFBMaxRat[k],   FBStabCrR.GDCr[k]);  
		  GetCrGDStabRatio      (nFlgStrn[k], LcomKeyD, dStbFTBRat,     dFlgStabFTBMaxRat[k],  FTBStabCrR.GDCr[k]);  
		  GetCrGDStabRatio      (nFlgStrn[k], LcomKeyD, dStbGDRat,      dFlgStabGDMaxRat[k],   GDStabCrR.GDCr[k]);  
		  // - Rib
		  GetCrFlgStabRibRatio  (nFlgStrn[k], LcomKeyD, dTFStabRibRat,  dBFStabRibRat,         dFlgStabRibMaxRat[nTop],   dFlgStabRibMaxRat[nBot],   FlgStabCrR.RibCr[nTop],   FlgStabCrR.RibCr[nBot]);
		  // - Deck : Global, Local, max
		  GetCrFlgStabDeckRatio (nFlgStrn[k], LcomKeyD, dTFStabDeckGRat, dBFStabDeckGRat,      dFlgStabDeckGMaxRat[nTop],  dFlgStabDeckGMaxRat[nBot],  FlgStabCrR.DeckGCr[nTop],  FlgStabCrR.DeckGCr[nBot]);
		  GetCrFlgStabDeckRatio (nFlgStrn[k], LcomKeyD, dTFStabDeckLRat, dBFStabDeckLRat,      dFlgStabDeckLMaxRat[nTop],  dFlgStabDeckLMaxRat[nBot],  FlgStabCrR.DeckLCr[nTop],  FlgStabCrR.DeckLCr[nBot]);
		  GetCrFlgStabDeckRatio (nFlgStrn[k], LcomKeyD, dTFStabDeckRat,  dBFStabDeckRat,       dFlgStabDeckMaxRat[nTop],   dFlgStabDeckMaxRat[nBot],   FlgStabCrR.DeckCr[nTop],   FlgStabCrR.DeckCr[nBot]);
		  // - FB : Global, Local, max
		  GetCrFlgStabFBeamRatio(nFlgStrn[k], LcomKeyD, dTFStabFBGRat,   dBFStabFBGRat,        dFlgStabFBeamGMaxRat[nTop], dFlgStabFBeamGMaxRat[nBot], FlgStabCrR.FBeamGCr[nTop], FlgStabCrR.FBeamGCr[nBot]);
		  GetCrFlgStabFBeamRatio(nFlgStrn[k], LcomKeyD, dTFStabFBLRat,   dBFStabFBLRat,        dFlgStabFBeamLMaxRat[nTop], dFlgStabFBeamLMaxRat[nBot], FlgStabCrR.FBeamLCr[nTop], FlgStabCrR.FBeamLCr[nBot]);
		  GetCrFlgStabFBeamRatio(nFlgStrn[k], LcomKeyD, dTFStabFBRat,    dBFStabFBRat,         dFlgStabFBeamMaxRat[nTop],  dFlgStabFBeamMaxRat[nBot],  FlgStabCrR.FBeamCr[nTop],  FlgStabCrR.FBeamCr[nBot]);
		  if(k==0)
		  {
			  // - Web-left
			  GetCrWebStabRibRatio  (nFlgStrn[k], LcomKeyD, dWBStabRibRatI[0],  dWebStabRibMaxRatL[k],  WebStabCrR.RibCrL[k]);
			  GetCrWebStabDeckRatio (nFlgStrn[k], LcomKeyD, dWBStabDeckRatI[0], dWebStabDeckMaxRatL[k], WebStabCrR.DeckCrL[k]);
			  // - Web-right
			  GetCrWebStabRibRatio  (nFlgStrn[k], LcomKeyD, dWBStabRibRatI[1],  dWebStabRibMaxRatR[k],  WebStabCrR.RibCrR[k]);
			  GetCrWebStabDeckRatio (nFlgStrn[k], LcomKeyD, dWBStabDeckRatI[1], dWebStabDeckMaxRatR[k], WebStabCrR.DeckCrR[k]);
		  }
		  else
		  {
			  // - Web-left
			  GetCrWebStabRibRatio  (nFlgStrn[k], LcomKeyD, dWBStabRibRatJ[0],  dWebStabRibMaxRatL[k],  WebStabCrR.RibCrL[k]);
			  GetCrWebStabDeckRatio (nFlgStrn[k], LcomKeyD, dWBStabDeckRatJ[0], dWebStabDeckMaxRatL[k], WebStabCrR.DeckCrL[k]);
			  // - Web-right
			  GetCrWebStabRibRatio  (nFlgStrn[k], LcomKeyD, dWBStabRibRatJ[1],  dWebStabRibMaxRatR[k],  WebStabCrR.RibCrR[k]);
			  GetCrWebStabDeckRatio (nFlgStrn[k], LcomKeyD, dWBStabDeckRatJ[1], dWebStabDeckMaxRatR[k], WebStabCrR.DeckCrR[k]);
		  }


		  nFlgStrn[k]++;

		}
			}

	  // 각 하중조합에 대한 결과 저장.
	  pCheckSod->WriteSodLcomForce       (ElemK.first, nDgnLcomNo, SodForce);
	  pCheckSod->WriteSodTopFlgStrnResult(ElemK.first, nDgnLcomNo, nLSTopNum, StrnFlgT);
	  pCheckSod->WriteSodBotFlgStrnResult(ElemK.first, nDgnLcomNo, nLSBotNum, StrnFlgB);
	  pCheckSod->WriteSodWebStrnResult   (ElemK.first, nDgnLcomNo, nLSWebNum, StrnWebD);
	  pCheckSod->WriteSodTopFlgStabResult(ElemK.first, nDgnLcomNo, nLSTopNum, nPanTopNum, StabFlgT);
	  pCheckSod->WriteSodBotFlgStabResult(ElemK.first, nDgnLcomNo, nLSBotNum, nPanBotNum, StabFlgB);
	  pCheckSod->WriteSodWebStabResult   (ElemK.first, nDgnLcomNo, nLSWebNum, StabWebD);
	  pCheckSod->WriteSodFBStabResult    (ElemK.first, nDgnLcomNo, StabFB);
	  pCheckSod->WriteSodFTBStabResult   (ElemK.first, nDgnLcomNo, StabFTB);
	  pCheckSod->WriteSodGDStabResult    (ElemK.first, nDgnLcomNo, StabGD);      
		} 

	Check_SOD_Fatigue(ElemK.first, bChkIJ, MembLcomD, MembResD, aForceLcom, aForceLTerm, ChkItem.bSodCheck[7]);

	// 요소에서 가장 불리한 하중조합의 결과 저장.
	pCheckSod->WriteSodStrnFlgCrResult(ElemK.first, FlgStrnCrR);
	pCheckSod->WriteSodStrnWebCrResult(ElemK.first, WebStrnCrR);
	pCheckSod->WriteSodStabFlgCrResult(ElemK.first, FlgStabCrR);
	pCheckSod->WriteSodStabWebCrResult(ElemK.first, WebStabCrR);
	pCheckSod->WriteSodStabFBCrResult (ElemK.first, FBStabCrR);    
	pCheckSod->WriteSodStabFTBCrResult(ElemK.first, FTBStabCrR);    
	pCheckSod->WriteSodStabGDCrResult (ElemK.first, GDStabCrR);        
	}

	CArray<T_ELEM_K, T_ELEM_K> aElemKList4ForceEx; aElemKList4ForceEx.RemoveAll();
	pCheckSod->GetElem4ExForce(aElemKList4ForceEx);

	if(aElemKList4ForceEx.GetSize() > 0)
	{
	  for(int nElem = 0; nElem < aElemKList4ForceEx.GetSize(); ++nElem)
	  {
		  T_ELEM_K ElemK = aElemKList4ForceEx[nElem];
		  ElemPairK EPairK(ElemK, EN_EL_BEAM);

		  ADGNFORCE_EX aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm;
		  ADGNSHRSADD_EX aStssLcom;

		  if (!m_pForcCtrl->Get_LcomDataForDesign_EX(EPairK, TRUE, TRUE, aForceLcom, aForceTnpr, aForceTnsc, aForceLTerm, aStssLcom, nLcomNum))
		  {ASSERT(0);}

		  int nForceLcomSize4FB = aForceLcom.GetSize();
		  for(int j=0; j< nForceLcomSize4FB; ++j)
		  {
			  int nDgnLcomNo = j+1;
			  const _DGN_FORC_CRC_EX *pForceExD = &aForceLcom[j];
			  T_SODD_FORCE_EX SodForceEx;// For Expansion DgnForce for Flexural buckling
			  ConvertForceEx(*pForceExD, SodForceEx);
			  pCheckSod->WriteSodLcomForceEx       (ElemK, nDgnLcomNo, SodForceEx);
		  }			  
	  }
	}

	pCheckSod->WriteSodCheckItem(ChkItem);

	return TRUE;
}

BOOL CDgnSodDataCtrl::GetLcom4Kapa(int nOrgLcomNo, ADGNFORCE& aForceLcom, ADGNFORCE& aForceLTerm, STLCHKFOR_ARRAY& aChkForceI, 
								   STLCHKFOR_ARRAY& aChkForceJ, STLCHKFOR_ARRAY& aChkDLForceI, STLCHKFOR_ARRAY& aChkDLForceJ, int& nLcomSize, int& nDgnLcomKFirst)
{
	T_DGN_LCOM_ID StrnLcomId;
	T_DGN_LCOM_ID FatiLcomId;
	BOOL bStrn = TRUE,  bFati = TRUE;
	if (!m_pForcCtrl->m_aStrnLcomId.Lookup(nOrgLcomNo, StrnLcomId)) {bStrn = FALSE;} 
	if (!m_pForcCtrl->m_aFatiLcomId.Lookup(nOrgLcomNo, FatiLcomId)) {bFati = FALSE;}
	if (!bStrn && !bFati) return FALSE;

	int nStrnLcomSize2 = StrnLcomId.aDgnLcomId.GetSize();
	int nFatiLcomSize  = FatiLcomId.aDgnLcomId.GetSize();
	nLcomSize = nStrnLcomSize2 + nFatiLcomSize;
	
	int nDgnLcomNo = 0;
	//int nOrgLcomNo = 0;
	int nSerLcomCount = 0;
	int nMaxMinType = 0;    
	CString strOrgLcomNa=_T("");

	nDgnLcomKFirst =0;
	// strn lcom
	if(bStrn)
	{
		for (int j=0; j<nStrnLcomSize2; ++j)
		{
			int nDgnLcomId = StrnLcomId.aDgnLcomId[j];
			if(j==0) nDgnLcomKFirst = nDgnLcomId;
			
			if(!m_pForcCtrl->Get_LcomType(nDgnLcomId, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0);}
			
			int nLcomIndex = nDgnLcomId-1;
			_DGN_FORC_CRC &ForceLcom = aForceLcom[nLcomIndex];
			_DGN_FORC_CRC &ForceLT   = aForceLTerm[nLcomIndex];
			
			_DGN_FORC_CRC ForceST = ForceLcom;
			ForceST -= ForceLT;      
			
			STL_FORCE_D ForODI, ForODJ;
			ConvertSodCheckForce(0, ForceLcom, ForODI);
			ConvertSodCheckForce(1, ForceLcom, ForODJ);

			aChkForceI.Add(ForODI);
			aChkForceJ.Add(ForODJ);
			
			STL_FORCE_D ForDLI, ForDLJ;
			ConvertSodCheckForce(0, ForceLT, ForDLI);
			ConvertSodCheckForce(1, ForceLT, ForDLJ);  
			
			aChkDLForceI.Add(ForODI);
			aChkDLForceJ.Add(ForODJ);
	  }
	}

	if(bFati)
	{
		// fatigue lcom
		int nDgnLcomKey_First = 0;
		for (int k = nStrnLcomSize2; k < nLcomSize; ++k)
		{
			int nFatiLcomIdIndex = k - nStrnLcomSize2;
			int nDgnLcomId = FatiLcomId.aDgnLcomId[nFatiLcomIdIndex];
			if(k == nStrnLcomSize2){nDgnLcomKey_First = nDgnLcomId;}
			
			if(!m_pForcCtrl->Get_LcomType(nDgnLcomId, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0);}
			
			int nLcomIndex = nDgnLcomId-1;
			_DGN_FORC_CRC &ForceLcom = aForceLcom[nLcomIndex];
			_DGN_FORC_CRC &ForceLT   = aForceLTerm[nLcomIndex];
			
			_DGN_FORC_CRC ForceST = ForceLcom;
			ForceST -= ForceLT;      
			
			STL_FORCE_D ForODI, ForODJ;
			ConvertSodCheckForce(0, ForceLcom, ForODI);
			ConvertSodCheckForce(1, ForceLcom, ForODJ);

			aChkForceI.Add(ForODI);
			aChkForceJ.Add(ForODJ);
			
			STL_FORCE_D ForDLI, ForDLJ;
			ConvertSodCheckForce(0, ForceLT, ForDLI);
			ConvertSodCheckForce(1, ForceLT, ForDLJ);  
			
			aChkDLForceI.Add(ForODI);
			aChkDLForceJ.Add(ForODJ);   
		}
	}
	
	return TRUE;
}

BOOL CDgnSodDataCtrl::GetDgnLengD()
{
	m_mDgnLengD.RemoveAll();
	int nElemSize = m_arElemK.GetSize(); 
	CDgnDataCtrl DgnCtrl;

	for(int i = 0; i < nElemSize; ++i)
	{
		T_ELEM_K nElemK = 0;
		T_SPAN_K nSpanK = 0;
		T_MEMB_K nMembK = 0;
		BOOL bUnLeng = FALSE, bEffLengF = FALSE;

		nElemK = m_arElemK[i].first;

		T_EDLK_D DgnLengD;
		T_MEMB_D MembD; MembD.Initialize();
		T_SPAN_D SpanD; SpanD.Initialize();
		STL_MEMB_ULEN UlenD; UlenD.Initialize();
		double dDummy = 0.0;

		if(Get_UnbracedLengthData(nElemK, UlenD, bUnLeng, bEffLengF))   // First. Find Design length Data from defined Unbraced Length and Effective buckling Coeff Info.
		{         
			DgnLengD.bExistUbracedInfo = TRUE;

			if(bUnLeng)
			{
				DgnLengD.dDgnLengY = UlenD.dLy;
				DgnLengD.dDgnLengZ = UlenD.dLz;
			}
			else
			{
				double dElemLength = m_pDoc->calcLAVElem(nElemK);
				DgnLengD.dDgnLengY = dElemLength;
				DgnLengD.dDgnLengZ = dElemLength;
			}

			DgnLengD.dEffBuckCoeffY = UlenD.dKy;
			DgnLengD.dEffBuckCoeffZ = UlenD.dKz; 

			if(bEffLengF || bUnLeng)
			{
				DgnLengD.nSupTypeY[0] = -1; // Not apply support data, just using effective length factor.
				DgnLengD.nSupTypeY[1] = -1;

				DgnLengD.nSupTypeZ[0] = -1;
				DgnLengD.nSupTypeZ[1] = -1;
			}
			else
			{
				BOOL bSupt = FALSE;
				if(!m_pDoc->m_pAttrCtrl2->IsElemSup(nElemK, bSupt)){return FALSE;}
				if(bSupt)
				{
					BOOL bSupportNode[2] = {FALSE,}; 
					BOOL bFix[2][6]={FALSE,};      

					if(!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(nElemK, bSupportNode[0], bSupportNode[1], bFix[0], bFix[1])) {return FALSE;}

					for(int nEnd = 0; nEnd < 2; ++nEnd)
					{   
						if     ((bFix[nEnd][0] || bFix[nEnd][1]) && bFix[nEnd][4]) {DgnLengD.nSupTypeY[nEnd] = 0;}
						else if((bFix[nEnd][0] || bFix[nEnd][1]) && !bFix[nEnd][4]){DgnLengD.nSupTypeY[nEnd] = 1;}
						else                                                       {DgnLengD.nSupTypeY[nEnd] = 2;}

						if     ((bFix[nEnd][0] || bFix[nEnd][2]) && bFix[nEnd][5]) {DgnLengD.nSupTypeZ[nEnd] = 0;}
						else if((bFix[nEnd][0] || bFix[nEnd][2]) && !bFix[nEnd][5]){DgnLengD.nSupTypeZ[nEnd] = 1;}
						else                                                       {DgnLengD.nSupTypeZ[nEnd] = 2;}    
					}
				}
			}
		}
		else if(m_pDoc->m_pAttrCtrl->GetMembAssigned(nElemK, nMembK)) // Second. Find Design length Data from defined Memb Info.
		{
			if(m_pDoc->m_pAttrCtrl->GetMemb(nMembK, MembD))
			{
				DgnLengD.MembK = nMembK;
				DgnLengD.bExistMembInfo = TRUE;
				GetDgnLengDFromMemb(nElemK, MembD, DgnLengD);
			}      
		}
		else if(m_pDoc->m_pAttrCtrl2->GetSpanAssigned(nElemK, nSpanK)) // Third. Find Design length Data from defined Span Info.
		{
			if(m_pDoc->m_pAttrCtrl->GetSpan(nSpanK, SpanD))
			{
				DgnLengD.SpanK = nSpanK;
				DgnLengD.bExistSpanInfo = TRUE;    
				GetDgnLengDFromSpan(nElemK, SpanD, DgnLengD);
			}
		}
		else  // Fourth. No Input Data
		{
			double dElemLenght = m_pDoc->calcLAVElem(nElemK);
			DgnLengD.dDgnLengY = dElemLenght;
			DgnLengD.dDgnLengZ = dElemLenght;

			DgnLengD.dEffBuckCoeffY = 1.0;
			DgnLengD.dEffBuckCoeffZ = 1.0;       
			BOOL bSupt = FALSE;
			if(!m_pDoc->m_pAttrCtrl2->IsElemSup(nElemK, bSupt)){return FALSE;}
			if(bSupt)
			{
				BOOL bSupportNode[2] = {FALSE,}; 
				BOOL bFix[2][6]={FALSE,};      

				if(!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(nElemK, bSupportNode[0], bSupportNode[1], bFix[0], bFix[1])) {return FALSE;}

				for(int nEnd = 0; nEnd < 2; ++nEnd)
				{   
					if     ((bFix[nEnd][0] || bFix[nEnd][1]) && bFix[nEnd][4]) {DgnLengD.nSupTypeY[nEnd] = 0;}
					else if((bFix[nEnd][0] || bFix[nEnd][1]) && !bFix[nEnd][4]){DgnLengD.nSupTypeY[nEnd] = 1;}
					else                                                       {DgnLengD.nSupTypeY[nEnd] = 2;}

					if     ((bFix[nEnd][0] || bFix[nEnd][2]) && bFix[nEnd][5]) {DgnLengD.nSupTypeZ[nEnd] = 0;}
					else if((bFix[nEnd][0] || bFix[nEnd][2]) && !bFix[nEnd][5]){DgnLengD.nSupTypeZ[nEnd] = 1;}
					else                                                       {DgnLengD.nSupTypeZ[nEnd] = 2;}    
				}
			}
		}

		m_mDgnLengD.SetAt(nElemK, DgnLengD);
	}

	return TRUE;
}


BOOL CDgnSodDataCtrl::GetDgnLengDFromMemb(T_ELEM_K ElemK, const T_MEMB_D &MembD, T_EDLK_D &DgnLengD)
{
	// Get Memb Total Length
	double dLength = 0.0;
	if(m_pDoc->m_pAttrCtrl2->GetMembTotalLength(MembD, dLength))
	{
		DgnLengD.dDgnLengY = dLength;
		DgnLengD.dDgnLengZ = dLength;  
	}

	// Get Memb Support Type  
	int nMembElemSize = MembD.aElemList.GetSize();
	CArray<T_ELEM_K, T_ELEM_K> aMembElemK; aMembElemK.RemoveAll(); aMembElemK.Copy(MembD.aElemList);
	CArray<T_NODE_K, T_NODE_K> aEndMembNodeK;   aEndMembNodeK.RemoveAll(); aEndMembNodeK.SetSize(2);
	
	if(nMembElemSize <= 0) {return FALSE;}
	else if(nMembElemSize >= 1)
	{
		T_ELEM_K ElemIK = aMembElemK[0];
		T_ELEM_K ElemJk = aMembElemK[nMembElemSize-1];
		
		T_ELEM_D ElemID, ElemJD; ElemID.Initialize(); ElemJD.Initialize();
		m_pDoc->m_pAttrCtrl->GetElem(ElemIK, ElemID);
		m_pDoc->m_pAttrCtrl->GetElem(ElemJk, ElemJD);

		aEndMembNodeK[0] = ElemID.elnod[0];
		aEndMembNodeK[1] = ElemJD.elnod[1];
	}
	
	if(aEndMembNodeK.GetSize() != 2){return FALSE;}

	T_NODE_K nElemNodeK[2] = {aEndMembNodeK[0], aEndMembNodeK[1]}; 
	int nSupTypeY[2] = {0,};  // 0 : Fix, 1 : Hinge, 2 : Free
	int nSupTypeZ[2] = {0,};  // 0 : Fix, 1 : Hinge, 2 : Free

	for(int i = 0; i < 2; ++i)
	{
		BOOL bFix[6]={FALSE};
		T_NODE_K nNode = nElemNodeK[i];
		m_pDoc->m_pAttrCtrl->IsDofConstraintForDesign(nNode, bFix, FALSE);

		if     ((bFix[0] || bFix[1]) && bFix[4]) {nSupTypeY[i] = 0;} // ???
		else if((bFix[0] || bFix[1]) && !bFix[4]){nSupTypeY[i] = 1;} // ???
		else                                     {nSupTypeY[i] = 2;} // ???

		if     ((bFix[0] || bFix[2]) && bFix[5]) {nSupTypeZ[i] = 0;} // ???
		else if((bFix[0] || bFix[2]) && !bFix[5]){nSupTypeZ[i] = 1;} // ???
		else                                     {nSupTypeZ[i] = 2;} // ???

	}
	DgnLengD.nSupTypeY[0] = nSupTypeY[0];
	DgnLengD.nSupTypeZ[0] = nSupTypeZ[0];
	DgnLengD.nSupTypeY[1] = nSupTypeY[1];
	DgnLengD.nSupTypeZ[1] = nSupTypeZ[1];


	// Find Location in Memb.
	int nDevideNum = 4;
	double dTagetLength = dLength / double(nDevideNum);
	double dSumLengI = 0.0;
	for(int k =0; k < nMembElemSize; ++k)
	{
		double dLengthDel = m_pDoc->calcLAVElem(MembD.aElemList[k]);

		double dSumLengJ = dSumLengI + dLengthDel;

		if(MembD.aElemList[k] == ElemK)
		{
			if(dSumLengI < dTagetLength  || dSumLengI > (dLength - dTagetLength)){DgnLengD.nLoInfoInMemb[0] = 1;} // ???
			if(dSumLengI < dTagetLength  || dSumLengJ > (dLength - dTagetLength)){DgnLengD.nLoInfoInMemb[1] = 1;} // ???
			break;
		}

		dSumLengI += dLengthDel;
	}

	return TRUE;
}



BOOL CDgnSodDataCtrl::GetDgnLengDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, T_EDLK_D &DgnLengD)
{
	// Get Memb Total Length
	double dSpanLength = 0.0;
	dSpanLength += m_pDoc->calcLAVElem(ElemK);

	// Get Element Index in SpanD
	int nElemIndex = 0;
	int nElemNumInSpan = SPanD.aElemBase.GetSize();
	for(int i = 0; i < nElemNumInSpan; ++i)
	{
		T_ELEM_K TempElemK = SPanD.aElemBase[i].ElemK;
		if(ElemK == TempElemK)
		{
			nElemIndex = i;
			break;
		}
	}

	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
	T_NODE_K NodeK[2] = {(unsigned int)ElemD.elnod[0], (unsigned int)ElemD.elnod[1]}; // I & J

	BOOL bSupportNode[2] = {FALSE,}; // I & j
	BOOL bFix[2][6]={FALSE,};        // I & j
	BOOL bSpanEndFix[2][6]={FALSE,};        // In & De Direction

	if(!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(ElemK, bSupportNode[0], bSupportNode[1], bFix[0], bFix[1])) {return FALSE;}

	// i j 가 각각 인덱스가 커지는 방향인가? 아니면 작아지는 방향인가?
	BOOL bExpandDeIndex = TRUE, bExpandInIndex = TRUE;
	int nElemIndexMin = nElemIndex == 0 ? -1 : nElemIndex - 1;
	int nElemIndexMax = nElemIndex == nElemNumInSpan-1 ? -1 : nElemIndex + 1;

	// Check Decrease Index direction
	if(nElemIndexMin != -1)
	{
		T_ELEM_K PreElemK = SPanD.aElemBase[nElemIndexMin].ElemK;
		T_ELEM_D PreElemD;
		m_pDoc->m_pAttrCtrl->GetElem(PreElemK, PreElemD);
		T_NODE_K PreNodeK[2] = {(unsigned int)PreElemD.elnod[0], (unsigned int)PreElemD.elnod[1]}; // I & J    

		if     (PreNodeK[0] == NodeK[0] || PreNodeK[1] == NodeK[0]){bExpandDeIndex = !bSupportNode[0];} // Index decrease direction -> I direction
		else if(PreNodeK[0] == NodeK[1] || PreNodeK[1] == NodeK[1]){bExpandDeIndex = !bSupportNode[1];} // Index decrease direction -> J direction
		else{ASSERT(0);}
	}
	else
	{
		bExpandDeIndex = FALSE;
	}

	if(!bExpandDeIndex)
	{
		if(bSupportNode[0])
		{
			for(int a = 0; a < 6; ++a){bSpanEndFix[0][a] = bFix[0][a];}  
		}
		else if(bSupportNode[1])
		{
			for(int a = 0; a < 6; ++a){bSpanEndFix[0][a] = bFix[1][a];}  
		}
		else{ASSERT(0);}     
	}

	// Check Increase Index direction
	if(nElemIndexMax != -1)
	{
		T_ELEM_K NextElemK = SPanD.aElemBase[nElemIndexMax].ElemK;
		T_ELEM_D NextElemD;
		m_pDoc->m_pAttrCtrl->GetElem(NextElemK, NextElemD);
		T_NODE_K NextNodeK[2] = {(unsigned int)NextElemD.elnod[0], (unsigned int)NextElemD.elnod[1]}; // I & J    
		
		if     (NextNodeK[0] == NodeK[0] || NextNodeK[1] == NodeK[0]){bExpandInIndex = !bSupportNode[0];} // Index decrease direction -> I direction
		else if(NextNodeK[0] == NodeK[1] || NextNodeK[1] == NodeK[1]){bExpandInIndex = !bSupportNode[1];} // Index decrease direction -> J direction
		else{ASSERT(0);}

	}
	else
	{
		bExpandInIndex = FALSE;
	}

	if(!bExpandInIndex)
	{
		if(bSupportNode[0])
		{
			for(int a = 0; a < 6; ++a){bSpanEndFix[1][a] = bFix[0][a];}  
		}
		else if(bSupportNode[1])
		{
			for(int a = 0; a < 6; ++a){bSpanEndFix[1][a] = bFix[1][a];}  
		}
		else{ASSERT(0);}     
	}

	int nElemIndexDe = 0, nElemIndexIn = 0;

	// for Decrease Index Direction
	if(bExpandDeIndex)
	{
		int nDeRepeatNum = nElemIndexMin;
		BOOL bDeFix[2]={FALSE,}; 
		BOOL bSpanDeFix[2][6]={FALSE,};  

		for(int nDe = nElemIndexMin; nDe >= 0; --nDe)
		{
			T_ELEM_K Key = SPanD.aElemBase[nDe].ElemK;  
			double dL = m_pDoc->calcLAVElem(Key);
			dSpanLength += dL;

			if(!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(Key, bDeFix[0], bDeFix[1], bSpanDeFix[0], bSpanDeFix[1])){return FALSE;}
	
			if(bDeFix[0] || bDeFix[1])
			{
				if(bDeFix[0])
				{
					for(int a = 0; a < 6; ++a){bSpanEndFix[0][a] = bSpanDeFix[0][a];}
				}
				else if(bDeFix[1])
				{
					for(int a = 0; a < 6; ++a){bSpanEndFix[0][a] = bSpanDeFix[1][a];}  
				}
				else {ASSERT(0);}
				nElemIndexDe = nDe;
				break;
			}
		}
	}
	else
	{
		nElemIndexDe = nElemIndex;
	}

	// for Increase Index Direction
	if(bExpandInIndex)
	{
		int nDeRepeatNum = nElemIndexMax;
		BOOL bInFix[2]={FALSE,}; 
		BOOL bSpanInFix[2][6]={FALSE,};  

		for(int nIn = nElemIndexMax; nIn < nElemNumInSpan; ++nIn)
		{
			T_ELEM_K Key = SPanD.aElemBase[nIn].ElemK;  
			double dL = m_pDoc->calcLAVElem(Key);
			dSpanLength += dL;

			BOOL bElemSup = FALSE;
			if(!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(Key, bInFix[0], bInFix[1], bSpanInFix[0], bSpanInFix[1])){return FALSE;}

			if(bInFix[0] || bInFix[1])
			{
				if(bInFix[0])
				{
					for(int a = 0; a < 6; ++a){bSpanEndFix[1][a] = bSpanInFix[0][a];}     
				}
				else if(bInFix[1])
				{
					for(int a = 0; a < 6; ++a){bSpanEndFix[1][a] = bSpanInFix[1][a];}        
				}
				else {ASSERT(0);}
				nElemIndexIn = nIn;
				break;
			}
		}
	}
	else
	{
		nElemIndexIn = nElemIndex;
	}

	DgnLengD.dDgnLengY = dSpanLength;
	DgnLengD.dDgnLengZ = dSpanLength;

	// Get Memb Support Type  
	int nSupTypeY[2] = {0,};  // 0 : Fix, 1 : Hinge, 2 : Free
	int nSupTypeZ[2] = {0,};  // 0 : Fix, 1 : Hinge, 2 : Free

	for(int nEnd = 0; nEnd < 2; ++nEnd)
	{   
		if     ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][1]) && bSpanEndFix[nEnd][4]) {nSupTypeY[nEnd] = 0;} // ???
		else if((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][1]) && !bSpanEndFix[nEnd][4]){nSupTypeY[nEnd] = 1;} // ???
		else                                                                            {nSupTypeY[nEnd] = 2;} // ???
		
		if     ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][2]) && bSpanEndFix[nEnd][5]) {nSupTypeZ[nEnd] = 0;} // ???
		else if((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][2]) && !bSpanEndFix[nEnd][5]){nSupTypeZ[nEnd] = 1;} // ???
		else                                                                            {nSupTypeZ[nEnd] = 2;} // ???    
	}

	DgnLengD.nSupTypeY[0] = nSupTypeY[0];
	DgnLengD.nSupTypeZ[0] = nSupTypeZ[0];
	DgnLengD.nSupTypeY[1] = nSupTypeY[1];
	DgnLengD.nSupTypeZ[1] = nSupTypeZ[1];



	// Find Location in Memb.
	int nDevideNum = 4;
	double dChkLength = 0.0;
	double dTagetLength = dSpanLength / double(nDevideNum);


	double dSumLengI = 0.0; 
	for(int n = nElemIndexDe; n < nElemIndexIn + 1; ++n)
	{
		double dLengthDel = m_pDoc->calcLAVElem(SPanD.aElemBase[n].ElemK);
		double dSumLengJ = dSumLengI + dLengthDel;
		if(n == nElemIndex)
		{
			if(dSumLengI < dTagetLength  || dSumLengI > (dSpanLength - dTagetLength)){DgnLengD.nLoInfoInMemb[0] = 1;} // ???
			if(dSumLengI < dTagetLength  || dSumLengJ > (dSpanLength - dTagetLength)){DgnLengD.nLoInfoInMemb[1] = 1;} // ???
			break;
		}

		dSumLengI += dLengthDel;
	}

	return TRUE;
}

BOOL CDgnSodDataCtrl::GetDgnSuptLengDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, double &dSpanLength)
{
	// Get Memb Total Length
	dSpanLength = 0.0;

	// Get Element Index in SpanD
	int nElemIndex = 0;
	int nElemNumInSpan = SPanD.aElemBase.GetSize();
	for(int i = 0; i < nElemNumInSpan; ++i)
	{
		T_ELEM_K TempElemK = SPanD.aElemBase[i].ElemK;
		if(ElemK == TempElemK)
		{
			nElemIndex = i;
			break;
		}
	}

	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
	T_NODE_K NodeK[2] = {(unsigned int)ElemD.elnod[0], (unsigned int)ElemD.elnod[1]}; // I & J

	BOOL bSupportNode[2] = {FALSE,}; // I & j
	BOOL bFix[2][6]={FALSE,};        // I & j

	if(!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(ElemK, bSupportNode[0], bSupportNode[1], bFix[0], bFix[1])) {return FALSE;}

	// i j 가 각각 인덱스가 커지는 방향인가? 아니면 작아지는 방향인가?
	BOOL bExpandDeIndex = TRUE, bExpandInIndex = TRUE;
	int nElemIndexMin = nElemIndex == 0 ? -1 : nElemIndex - 1;
	int nElemIndexMax = nElemIndex == nElemNumInSpan-1 ? -1 : nElemIndex + 1;

	// Check Decrease Index direction
	if(nElemIndexMin != -1)
	{
		T_ELEM_K PreElemK = SPanD.aElemBase[nElemIndexMin].ElemK;
		T_ELEM_D PreElemD;
		m_pDoc->m_pAttrCtrl->GetElem(PreElemK, PreElemD);
		T_NODE_K PreNodeK[2] = {(unsigned int)PreElemD.elnod[0], (unsigned int)PreElemD.elnod[1]}; // I & J    

		if     (PreNodeK[0] == NodeK[0] || PreNodeK[1] == NodeK[0]){bExpandDeIndex = !bSupportNode[0];} // Index decrease direction -> I direction
		else if(PreNodeK[0] == NodeK[1] || PreNodeK[1] == NodeK[1]){bExpandDeIndex = !bSupportNode[1];} // Index decrease direction -> J direction
		else{ASSERT(0);}
	}
	else
	{
		bExpandDeIndex = FALSE;
	}

	// Check Increase Index direction
	if(nElemIndexMax != -1)
	{
		T_ELEM_K NextElemK = SPanD.aElemBase[nElemIndexMax].ElemK;
		T_ELEM_D NextElemD;
		m_pDoc->m_pAttrCtrl->GetElem(NextElemK, NextElemD);
		T_NODE_K NextNodeK[2] = {(unsigned int)NextElemD.elnod[0], (unsigned int)NextElemD.elnod[1]}; // I & J    

		if     (NextNodeK[0] == NodeK[0] || NextNodeK[1] == NodeK[0]){bExpandInIndex = !bSupportNode[0];} // Index decrease direction -> I direction
		else if(NextNodeK[0] == NodeK[1] || NextNodeK[1] == NodeK[1]){bExpandInIndex = !bSupportNode[1];} // Index decrease direction -> J direction
		else{ASSERT(0);}
	}
	else
	{
		bExpandInIndex = FALSE;
	}

	int nElemIndexDe = 0, nElemIndexIn = 0;

	// for Decrease Index Direction
	if(bExpandDeIndex)
	{
		int nDeRepeatNum = nElemIndexMin;
		BOOL bDeFix = FALSE; 

		for(int nDe = nElemIndexMin; nDe == 0; --nDe)
		{
			T_ELEM_K Key = SPanD.aElemBase[nDe].ElemK;  
			double dL = m_pDoc->calcLAVElem(Key);
			dSpanLength += dL;

			if(!m_pDoc->m_pAttrCtrl2->IsElemSup(Key, bDeFix)){return FALSE;}  
			if(bDeFix) break;
		}
	}

	// for Increase Index Direction
	if(bExpandInIndex)
	{
		int nDeRepeatNum = nElemIndexMax;
		BOOL bInFix = FALSE; 

		for(int nIn = nElemIndexMax; nIn < nElemNumInSpan; ++nIn)
		{
			T_ELEM_K Key = SPanD.aElemBase[nIn].ElemK;  
			double dL = m_pDoc->calcLAVElem(Key);
			dSpanLength += dL;

			BOOL bElemSup = FALSE;
			if(!m_pDoc->m_pAttrCtrl2->IsElemSup(Key, bInFix)){return FALSE;}  

			if(bInFix) break;
		}
	}

	return TRUE;
}

BOOL CDgnSodDataCtrl::GetDgnSuptTypeDFromSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, int nSpanEndSupType1YZ[2], int nSpanEndSupType2YZ[2])
{ 
	// Get Element Index in SpanD
	int nElemIndex = 0;
	int nElemNumInSpan = SPanD.aElemBase.GetSize();
	for(int i = 0; i < nElemNumInSpan; ++i)
	{
		T_ELEM_K TempElemK = SPanD.aElemBase[i].ElemK;
		if(ElemK == TempElemK)
		{
			nElemIndex = i;
			break;
		}
	}

	T_ELEM_D ElemD;
	m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
	T_NODE_K NodeK[2] = {(unsigned int)ElemD.elnod[0], (unsigned int)ElemD.elnod[1]}; // I & J

	BOOL bSupportNode[2] = {FALSE,}; // I & j
	BOOL bFix[2][6]={FALSE,};        // I & j
	BOOL bSpanEndFix[2][6]={FALSE,};        // In & De Direction

	if(!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(ElemK, bSupportNode[0], bSupportNode[1], bFix[0], bFix[1])) {return FALSE;}

	// i j 가 각각 인덱스가 커지는 방향인가? 아니면 작아지는 방향인가?
	BOOL bExpandDeIndex = TRUE, bExpandInIndex = TRUE;
	int nElemIndexMin = nElemIndex == 0 ? -1 : nElemIndex - 1;
	int nElemIndexMax = nElemIndex == nElemNumInSpan-1 ? -1 : nElemIndex + 1;

	// Check Decrease Index direction
	if(nElemIndexMin != -1)
	{
		T_ELEM_K PreElemK = SPanD.aElemBase[nElemIndexMin].ElemK;
		T_ELEM_D PreElemD;
		m_pDoc->m_pAttrCtrl->GetElem(PreElemK, PreElemD);
		T_NODE_K PreNodeK[2] = {(unsigned int)PreElemD.elnod[0], (unsigned int)PreElemD.elnod[1]}; // I & J    

		if     (PreNodeK[0] == NodeK[0] || PreNodeK[1] == NodeK[0]){bExpandDeIndex = !bSupportNode[0];} // Index decrease direction -> I direction
		else if(PreNodeK[0] == NodeK[1] || PreNodeK[1] == NodeK[1]){bExpandDeIndex = !bSupportNode[1];} // Index decrease direction -> J direction
		else{ASSERT(0);}
	}
	else
	{
		bExpandDeIndex = FALSE;
	}

	// Check Increase Index direction
	if(nElemIndexMax != -1)
	{
		T_ELEM_K NextElemK = SPanD.aElemBase[nElemIndexMax].ElemK;
		T_ELEM_D NextElemD;
		m_pDoc->m_pAttrCtrl->GetElem(NextElemK, NextElemD);
		T_NODE_K NextNodeK[2] = {(unsigned int)NextElemD.elnod[0], (unsigned int)NextElemD.elnod[1]}; // I & J    

		if     (NextNodeK[0] == NodeK[0] || NextNodeK[1] == NodeK[0]){bExpandInIndex = !bSupportNode[0];} // Index decrease direction -> I direction
		else if(NextNodeK[0] == NodeK[1] || NextNodeK[1] == NodeK[1]){bExpandInIndex = !bSupportNode[1];} // Index decrease direction -> J direction
		else{ASSERT(0);}
	}
	else
	{
		bExpandInIndex = FALSE;
	}

	int nElemIndexDe = 0, nElemIndexIn = 0;

	// for Decrease Index Direction
	if(bExpandDeIndex)
	{
		int nDeRepeatNum = nElemIndexMin;
		BOOL bDeFix[2]={FALSE,}; 
		BOOL bSpanDeFix[2][6]={FALSE,};  

		for(int nDe = nElemIndexMin; nDe == 0; --nDe)
		{
			T_ELEM_K Key = SPanD.aElemBase[nDe].ElemK;  

			if(!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(Key, bDeFix[0], bDeFix[1], bSpanDeFix[0], bSpanDeFix[1])){return FALSE;}

			if(bDeFix[0] || bDeFix[1])
			{
				if(bDeFix[0])
				{
					for(int a = 0; a < 6; ++a){bSpanEndFix[0][a] = bSpanDeFix[0][a];}
				}
				else if(bDeFix[1])
				{
					for(int a = 0; a < 6; ++a){bSpanEndFix[0][a] = bSpanDeFix[1][a];}  
				}
				else {ASSERT(0);}
				break;
			}
		}
	}

	// for Increase Index Direction
	if(bExpandInIndex)
	{
		int nDeRepeatNum = nElemIndexMax;
		BOOL bInFix[2]={FALSE,}; 
		BOOL bSpanInFix[2][6]={FALSE,};  

		for(int nIn = nElemIndexMax; nIn < nElemNumInSpan; ++nIn)
		{
			T_ELEM_K Key = SPanD.aElemBase[nIn].ElemK;  

			BOOL bElemSup = FALSE;
			if(!m_pDoc->m_pAttrCtrl2->GetElemSupTypeIJ(Key, bInFix[0], bInFix[1], bSpanInFix[0], bSpanInFix[1])){return FALSE;}

			if(bInFix[0] || bInFix[1])
			{
				if(bInFix[0])
				{
					for(int a = 0; a < 6; ++a){bSpanEndFix[1][a] = bSpanInFix[0][a];}     
				}
				else if(bInFix[1])
				{
					for(int a = 0; a < 6; ++a){bSpanEndFix[1][a] = bSpanInFix[1][a];}        
				}
				else {ASSERT(0);}
				break;
			}
		}
	}

	// Get Memb Support Type  
	int nSupTypeY[2] = {0,};  // 0 : Fix, 1 : Hinge, 2 : Free
	int nSupTypeZ[2] = {0,};  // 0 : Fix, 1 : Hinge, 2 : Free

	for(int nEnd = 0; nEnd < 2; ++nEnd)
	{   
		if     ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][1]) && bSpanEndFix[nEnd][4]) {nSupTypeY[nEnd] = 0;} 
		else if((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][1]) && !bSpanEndFix[nEnd][4]){nSupTypeY[nEnd] = 1;} 
		else                                                                            {nSupTypeY[nEnd] = 2;} 
		
		if     ((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][2]) && bSpanEndFix[nEnd][4]) {nSupTypeZ[nEnd] = 0;} 
		else if((bSpanEndFix[nEnd][0] || bSpanEndFix[nEnd][2]) && !bSpanEndFix[nEnd][4]){nSupTypeZ[nEnd] = 1;} 
		else                                                                            {nSupTypeZ[nEnd] = 2;}     
	}

	nSpanEndSupType1YZ[0] = nSupTypeY[0];
	nSpanEndSupType1YZ[0] = nSupTypeZ[0];
	nSpanEndSupType2YZ[1] = nSupTypeY[1];
	nSpanEndSupType2YZ[1] = nSupTypeZ[1];


	return TRUE;
}

BOOL CDgnSodDataCtrl::GetDgnElemLocDInSpan(T_ELEM_K ElemK, const T_SPAN_D &SPanD, const double dSpanLength, const int nStartElemIndex, const int nEndElemIndex, int nElemLocaD[2])
{
	// Find Location in Memb.

	int nDevideNum = 4;  
	double dTagetLength = dSpanLength / double(nDevideNum);

	int nElemIndex = 0;
	int nElemNumInSpan = SPanD.aElemBase.GetSize();
	for(int i = 0; i < nElemNumInSpan; ++i)
	{
		T_ELEM_K TempElemK = SPanD.aElemBase[i].ElemK;
		if(ElemK == TempElemK)
		{
			nElemIndex = i;
			break;
		}
	}

	double dSumLengI = 0.0; 
	for(int n = nStartElemIndex; n < nEndElemIndex + 1; ++n)
	{
		double dLengthDel = m_pDoc->calcLAVElem(SPanD.aElemBase[n].ElemK);
		double dSumLengJ = dSumLengI + dLengthDel;
		if(n == nElemIndex)
		{
			if(dSumLengI < dTagetLength  || dSumLengI > (dSpanLength - dTagetLength)){nElemLocaD[0] = 1;} // ???
			if(dSumLengI < dTagetLength  || dSumLengJ > (dSpanLength - dTagetLength)){nElemLocaD[1] = 1;} // ???
			break;
		}

		dSumLengI += dLengthDel;
	}


	return TRUE;
}


BOOL CDgnSodDataCtrl::GetSodFlexuralBucklingData(T_ELEM_K ElemK, int nIJ, int nDgnLcomK, STL_MEMB_POSD_SNiP_OD &MembLcomD)
{
	if(nIJ != 0 && nIJ != 1) return FALSE;

	STL_POSD_DGND_SNiP_FB_MEMB_TYPE& DgnFLBKD = MembLcomD.Memb.PosD[nIJ].PosDDgnInfo.FlexBuckMembType;
	DgnFLBKD.Initialize();
	T_FLBK_D FlbkD; FlbkD.Initialize();

	ElemPairK EPairK(ElemK, EN_EL_BEAM);

	if(m_pDoc->m_pAttrCtrl2->GetFlbk(ElemK, FlbkD)) // Flexural Buckling Data Exist Case
	{
		DgnFLBKD.bExistD	= true;
		DgnFLBKD.nMembType	= FlbkD.nMembType;

		T_MEMB_K MembK = 0;
		if(m_pDoc->m_pAttrCtrl->GetMembAssigned(ElemK, MembK)) // Member Assigned Case
		{
			T_MEMB_D MembD; MembD.Initialize();
			m_pDoc->m_pAttrCtrl->GetMemb(MembK, MembD);

			int nElemSize = MembD.aElemList.GetSize();
			DgnFLBKD.aMembCompD.SetSize(nElemSize);

			for(int nElem = 0; nElem < nElemSize; ++nElem)
			{
				// Get pure Length for each element
				T_ELEM_K ElemSubK = MembD.aElemList[nElem];
				DgnFLBKD.aMembCompD[nElem].dLength = m_pDoc->calcLAVElem(ElemSubK);
				ElemPairK ESubPairK(ElemSubK, EN_EL_BEAM);

				// Get Force
				_DGN_FORC_CRC_EX ForD;
				m_pForcCtrl->Get_LcomDataForDesign_LcomK_EX(ESubPairK, nDgnLcomK, TRUE, TRUE, ForD);
				ConvertSod5PosD(ForD, DgnFLBKD.aMembCompD[nElem]);	
			}
		}
		else // No Member Assigned Case, Only Element
		{
			DgnFLBKD.aMembCompD.SetSize(1);

			// Get pure Length for each element
			DgnFLBKD.aMembCompD[0].dLength = m_pDoc->calcLAVElem(ElemK);

			// Get Force
			_DGN_FORC_CRC_EX ForD;
			m_pForcCtrl->Get_LcomDataForDesign_LcomK_EX(EPairK, nDgnLcomK, TRUE, TRUE, ForD);
			ConvertSod5PosD(ForD, DgnFLBKD.aMembCompD[0]);	
		}
	}
	else  // No Flexural Buckling Data Exist Case
	{
		DgnFLBKD.bExistD = false;
	}

	return TRUE;
}

void CDgnSodDataCtrl::ConvertForceEx(const _DGN_FORC_CRC_EX& crInD, T_SODD_FORCE_EX& rData)
{
	for(int nPos = 0; nPos < 5; ++nPos)
	{
		rData.ForD[nPos].dForce[0] = crInD.dFxx[nPos];
		rData.ForD[nPos].dForce[1] = crInD.dFyy[nPos];
		rData.ForD[nPos].dForce[2] = crInD.dFzz[nPos];
		rData.ForD[nPos].dForce[3] = crInD.dMux[nPos];
		rData.ForD[nPos].dForce[4] = crInD.dMuy[nPos];
		rData.ForD[nPos].dForce[5] = crInD.dMuz[nPos];
	}
}


BOOL CDgnSodDataCtrl::ConvertSodDgnLengthData(const T_EDLK_D &DgnLengD, STL_RES_BASE_SNiP_OD_SNiP &rData)
{  
	rData.GDgnD.dDgnLengY         = DgnLengD.dDgnLengY;
	rData.GDgnD.dDgnLengZ         = DgnLengD.dDgnLengZ;
	rData.GDgnD.dEffLengF_Y       = DgnLengD.dEffBuckCoeffY;
	rData.GDgnD.dEffLengF_Z       = DgnLengD.dEffBuckCoeffZ;
	rData.GDgnD.dDgnLengZ         = DgnLengD.dDgnLengZ;
	rData.GDgnD.bExistSpanInfo    = DgnLengD.bExistSpanInfo;
	rData.GDgnD.bExistMembInfo    = DgnLengD.bExistMembInfo;
	rData.GDgnD.bExistUbracedInfo = DgnLengD.bExistUbracedInfo;
	rData.GDgnD.nLocationI        = DgnLengD.nLoInfoInMemb[0];
	rData.GDgnD.nLocationJ        = DgnLengD.nLoInfoInMemb[1];
	rData.GDgnD.nSuptTypeI_Y      = DgnLengD.nSupTypeY[0];
	rData.GDgnD.nSuptTypeI_Z      = DgnLengD.nSupTypeZ[0];
	rData.GDgnD.nSuptTypeJ_Y      = DgnLengD.nSupTypeY[1];
	rData.GDgnD.nSuptTypeJ_Z      = DgnLengD.nSupTypeZ[1];

	return TRUE;
}

BOOL CDgnSodDataCtrl::Make_ElemSpanData()
{
	m_mElemSpanD.RemoveAll();

	CArray<T_SPAN_K, T_SPAN_K> aSpanKeyList;
	m_pDoc->m_pAttrCtrl->GetSpanKeyList(aSpanKeyList);

	int nSpanKeySize = aSpanKeyList.GetSize();  
	T_SPAN_D SpanD; 
	for(int i=0; i<nSpanKeySize; ++i)
	{    
		SpanD.Initialize();
		T_SPAN_K SpanK = aSpanKeyList[i]; 
		m_pDoc->m_pAttrCtrl->GetSpan(SpanK, SpanD);
		int nSpanElemNum = SpanD.aElemBase.GetSize();
		UINT iL_count = 1;

		for(int j=0; j<nSpanElemNum; ++j)
		{    
			T_SPAN_BASE &SpanBase = SpanD.aElemBase[j];
			T_ELEM_K ElemK2 = SpanBase.ElemK;        
			m_mElemSpanD.SetAt(ElemK2, SpanBase);
		}
	}

	return TRUE;
}

BOOL CDgnSodDataCtrl::Check_SOD_Fatigue(T_ELEM_K ElemK,                                        
																				BOOL bChkIJ[],
																				STL_MEMB_POSD_SNiP_OD &MembLcomD,
																				STL_RES_BASE_SNiP_OD_SNiP *pMembResD,
																				ADGNFORCE &aForceLcom,
																				ADGNFORCE &aForceLTerm,
																				BOOL &bSodCheck)
{
	//if (CpgdD.bDesignParameter[4]==FALSE && CpgdD.bDesignParameter[5]==FALSE) return FALSE;

	CStlOrthDeckDesign *pCheckSod = m_pDoc->m_pPostCtrl->GetStlOrthDeckDesign();
	if (pCheckSod==NULL) return FALSE;

	CArray<T_LCOM_K,T_LCOM_K> aFatiLcomK;
	m_pForcCtrl->m_aFatiLcomId.GetSortedKeyListforUintK(aFatiLcomK);

	int nFatiCount[2]={0};
	double dFatiMaxRat[2]={0.0};

	T_SODD_DGN_FORCE SodForce;
	int nFatiLcomSize = aFatiLcomK.GetSize();

	T_SDFG_CR FatiCrR;

	for (int i=0; i<nFatiLcomSize; ++i)
	{
		T_DGN_LCOM_ID FatiLcomId;
		T_LCOM_K nLcomK = aFatiLcomK[i];
		if (!m_pForcCtrl->m_aFatiLcomId.Lookup(aFatiLcomK[i], FatiLcomId)) continue;  
		
		int nFatiConcSize = FatiLcomId.aDgnLcomId.GetSize();
		MembLcomD.aChkFor.RemoveAll();
		MembLcomD.aUlmf.RemoveAll();
		int nLcomSize=1;
		MembLcomD.aChkFor.SetSize(nLcomSize);
		MembLcomD.aUlmf.SetSize(nLcomSize);
		MembLcomD.aChkFor[0].nLcomType = 1;
		//MembLcomD.aChkFor[0].GlobalForceD.SetSize(nFatiConcSize);
		//MembLcomD.aChkFor[0].GlobalForceD_DL.SetSize(nFatiConcSize);
		
		int nOrgLcomNo = 0;
		int nSerLcomCount = 0;
		int nMaxMinType = 0;    
		CString strOrgLcomNa=_T("");
		int nFatiLcomType = 0;
		
		STLCHKFOR_ARRAY aChkForceI;
		aChkForceI.SetSize(nFatiConcSize);
		STLCHKFOR_ARRAY aChkForceJ;
		aChkForceJ.SetSize(nFatiConcSize);
		//
		STLCHKFOR_ARRAY aChkDLForceI;
		aChkDLForceI.SetSize(nFatiConcSize);
		STLCHKFOR_ARRAY aChkDLForceJ;
		aChkDLForceJ.SetSize(nFatiConcSize);
		int nDgnLcomKey_First = 0;
		for (int j=0; j<nFatiConcSize; ++j)
		{
			int nDgnLcomId = FatiLcomId.aDgnLcomId[j];
			if(j == 0){nDgnLcomKey_First = nDgnLcomId;}
			
			if(!m_pForcCtrl->Get_LcomType(nDgnLcomId, strOrgLcomNa, nOrgLcomNo, nSerLcomCount, nMaxMinType)) { ASSERT(0);}
			
			int nLcomIndex = nDgnLcomId-1;
			_DGN_FORC_CRC &ForceLcom = aForceLcom[nLcomIndex];
			_DGN_FORC_CRC &ForceLT   = aForceLTerm[nLcomIndex];
			
			_DGN_FORC_CRC ForceST = ForceLcom;
			ForceST -= ForceLT;      
			//ConvertCsgCalcChkForce(0, ForceNC, ForceLT, ForceST, ForceSum, aChkForceI[j]);
			//ConvertCsgCalcChkForce(1, ForceNC, ForceLT, ForceST, ForceSum, aChkForceJ[j]);

			STL_FORCE_D ForOD;
			ConvertSodCheckForce(0, ForceLcom, aChkForceI[j]);
			ConvertSodCheckForce(1, ForceLcom, aChkForceJ[j]);

			STL_FORCE_D ForDL;
			ConvertSodCheckForce(0, ForceLT, aChkDLForceI[j]);
			ConvertSodCheckForce(1, ForceLT, aChkDLForceJ[j]);      

			for (int k=0; k<2; ++k) 
			{
				ConvertSODDFORCE(k, ForceLcom, SodForce.Elem[k]);
		ConvertSODDFORCE(k, ForceLT,   SodForce.ElemDL[k]);
			}
			
			pCheckSod->WriteSodLcomForce(ElemK, nDgnLcomId, SodForce);
		}

		T_SDCR_BASE LcomKeyD;
		LcomKeyD.nLcomK         = nOrgLcomNo;
		LcomKeyD.nDgnLcomKFirst = nDgnLcomKey_First;
		LcomKeyD.nConcurrent    = nMaxMinType;
		LcomKeyD.nLcomSize      = nFatiConcSize;

		T_SDFG_CASE FatiC;

		for (int k=0; k<2; ++k) // I,J
		{
			if(bChkIJ[k]==FALSE) continue;
			BOOL bPosI = (k==0);
			MembLcomD.Memb.ChIf.nChkPos = k; 
			
			if (k==0)
			{
				MembLcomD.aChkFor[0].GlobalForceD.Copy(aChkForceI);
				MembLcomD.aChkFor[0].GlobalForceD_DL.Copy(aChkDLForceI);
			}
			else
			{
				MembLcomD.aChkFor[0].GlobalForceD.Copy(aChkForceJ);
				MembLcomD.aChkFor[0].GlobalForceD_DL.Copy(aChkDLForceJ);
			}

			STL_MEMB_RES_SNiP_FAT FatiResD;

			double dFatiPosRat  = 0.0;

			if (m_nDgnCode==SNiP_20503_84_SOD || m_nDgnCode==SP_35_13330_11_SOD)
			{
				m_pSodManager->Check_StlOrthDeckFatigueSNiP(MembLcomD, pMembResD[k], FatiResD);
				dFatiPosRat = FatiResD.ResCrtFatigueD.dRat_Ax;
				ConvertSodSdFati(pMembResD[k], FatiResD, FatiC.FatiB[k]);
				if(FatiC.FatiB[k].bChk) bSodCheck=TRUE;
			}

			GetCrFatigueRatio(nFatiCount[k], LcomKeyD, dFatiPosRat, dFatiMaxRat[k], FatiCrR.FatiCr[k]);

			nFatiCount[k]++;
		}
		pCheckSod->WriteSodChkFatiResult(ElemK, nOrgLcomNo, FatiC);
	}

	pCheckSod->WriteSodFatiPosCrResult(ElemK, FatiCrR);

	return TRUE;
}

BOOL CDgnSodDataCtrl::Get_SodPosData(T_ELEM_K ElemK, T_SECT_K SectK, int nStype, int nPos, STL_MEMB_POSD_OD &rData)
{
	if (nPos<0 || nPos>1) return FALSE;

	T_SDDP_D SddpD; SddpD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetSddp(ElemK, SddpD);  

	STL_SECT_POSD_SNiP_OD &PosChkD = rData.PosD[nPos];
	ConvertSodSectCheckPart(nStype, nPos, SddpD, PosChkD.ChkPosInfo);

	if (m_mSodMatlSect.Lookup(SectK, PosChkD.MatInfo)==FALSE)
	{
		T_ELEM_D ElemD; ElemD.Initialize();
		BOOL bGetElemPre = m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
		T_MATD_D MatdD; MatdD.Initialize();
		if (m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD))
		{          
			ConvertSodMatlProp(MatdD, PosChkD.MatInfo.MatTopF);
			PosChkD.MatInfo.MatWeb  = PosChkD.MatInfo.MatTopF;
			PosChkD.MatInfo.MatBotF = PosChkD.MatInfo.MatTopF;

			PosChkD.MatInfo.MatTopF_Rib = PosChkD.MatInfo.MatTopF;
			PosChkD.MatInfo.MatBotF_Rib = PosChkD.MatInfo.MatTopF;
			PosChkD.MatInfo.MatWeb_Rib  = PosChkD.MatInfo.MatTopF;      
		}
	}

	STL_SECT_ELEMD_SNiP_OD ElemSectInfo;
	if (m_mSodSect.Lookup(SectK, ElemSectInfo))
	{
		PosChkD.SectInfo = ElemSectInfo.SectD[nPos];
	}

	T_EWSF_D OD_EWSFD; OD_EWSFD.Initialize(); // effective scale factor를 고려한 단면 강성 변화
	if(m_pDoc->m_pAttrCtrl->GetEwsfElem(ElemK, OD_EWSFD))
	{
		if(nPos == 0)
		{
			PosChkD.SectInfo.SectOD.EffStiffness.dIyy =  PosChkD.SectInfo.SectOD.Stiffness.dIyy * OD_EWSFD.EwsfSubY.dScaleFactor;
			PosChkD.SectInfo.SectOD.EffStiffness.dCzp =  PosChkD.SectInfo.SectOD.Stiffness.dCzp * OD_EWSFD.EwsfSubY.dZtopFactor;
			PosChkD.SectInfo.SectOD.EffStiffness.dCzm =  PosChkD.SectInfo.SectOD.Stiffness.dCzm * OD_EWSFD.EwsfSubY.dZbotFactor;
			double dEfffDeltaZ = PosChkD.SectInfo.SectOD.Stiffness.dCzp - PosChkD.SectInfo.SectOD.EffStiffness.dCzp;
			PosChkD.SectInfo.SectOD.EffStiffness.dy1 = PosChkD.SectInfo.SectOD.EffStiffness.dCzp;
			PosChkD.SectInfo.SectOD.EffStiffness.dy2 = PosChkD.SectInfo.SectOD.EffStiffness.dCzp;    
			PosChkD.SectInfo.SectOD.EffStiffness.dy3 = -PosChkD.SectInfo.SectOD.EffStiffness.dCzm;
			PosChkD.SectInfo.SectOD.EffStiffness.dy4 = -PosChkD.SectInfo.SectOD.EffStiffness.dCzm; 

		}
		else if(nPos == 1)
		{
			PosChkD.SectInfo.SectOD.EffStiffness.dIyy =  PosChkD.SectInfo.SectOD.Stiffness.dIyy * OD_EWSFD.EwsfSubY.dScaleFactor_j;
			PosChkD.SectInfo.SectOD.EffStiffness.dCzp =  PosChkD.SectInfo.SectOD.Stiffness.dCzp * OD_EWSFD.EwsfSubY.dZtopFactor_j;
			PosChkD.SectInfo.SectOD.EffStiffness.dCzm =  PosChkD.SectInfo.SectOD.Stiffness.dCzm * OD_EWSFD.EwsfSubY.dZbotFactor_j;
			double dEfffDeltaZ = PosChkD.SectInfo.SectOD.Stiffness.dCzp - PosChkD.SectInfo.SectOD.EffStiffness.dCzp;
			PosChkD.SectInfo.SectOD.EffStiffness.dy1 = PosChkD.SectInfo.SectOD.EffStiffness.dCzp;
			PosChkD.SectInfo.SectOD.EffStiffness.dy2 = PosChkD.SectInfo.SectOD.EffStiffness.dCzp;  
			PosChkD.SectInfo.SectOD.EffStiffness.dy3 = -PosChkD.SectInfo.SectOD.EffStiffness.dCzm;
			PosChkD.SectInfo.SectOD.EffStiffness.dy4 = -PosChkD.SectInfo.SectOD.EffStiffness.dCzm;  
		}
		else{ASSERT(0);}    
	}

	double dArea = PosChkD.SectInfo.SectOD.Stiffness.dArea;
	double dRoy = dArea < cDGN_Zero ? 0.0 : sqrt(PosChkD.SectInfo.SectOD.Stiffness.dIyy/dArea);
	double dRoz = dArea < cDGN_Zero ? 0.0 : sqrt(PosChkD.SectInfo.SectOD.Stiffness.dIzz/dArea);
	
	PosChkD.SectInfo.SectOD.Stiffness.dRoy = dRoy;
	PosChkD.SectInfo.SectOD.Stiffness.dRoz = dRoz;

	dRoy = dArea < cDGN_Zero ? 0.0 : sqrt(PosChkD.SectInfo.SectOD.EffStiffness.dIyy/dArea);
	dRoz = dArea < cDGN_Zero ? 0.0 : sqrt(PosChkD.SectInfo.SectOD.EffStiffness.dIzz/dArea);

	PosChkD.SectInfo.SectOD.EffStiffness.dRoy = dRoy;
	PosChkD.SectInfo.SectOD.EffStiffness.dRoz = dRoz;


		
	if (nPos==0)
	{
		m_mSodKappaFactor.Lookup(ElemK, PosChkD.PosDDgnInfo.KappaInfo);
		m_mSodKappaChkPos.Lookup(ElemK, PosChkD.PosDDgnInfo.KappaChkPosInfo);
		m_mSodFatiChkPos.Lookup(ElemK, PosChkD.PosDDgnInfo.FatChkPosInfo);
		m_mSodFloorBeam.Lookup(ElemK, PosChkD.PosDDgnInfo.DgnStaD_FloorBDeckP);
	}
	else
	{
		m_mSodKappaFactorJ.Lookup(ElemK, PosChkD.PosDDgnInfo.KappaInfo);
		m_mSodKappaChkPosJ.Lookup(ElemK, PosChkD.PosDDgnInfo.KappaChkPosInfo);
		m_mSodFatiChkPosJ.Lookup(ElemK, PosChkD.PosDDgnInfo.FatChkPosInfo);
		m_mSodFloorBeamJ.Lookup(ElemK, PosChkD.PosDDgnInfo.DgnStaD_FloorBDeckP);
	}  

	return TRUE;
}

BOOL CDgnSodDataCtrl::Get_LocalForceData(T_ELEM_K ElemK, T_LCOM_K LcomK, int nPos, STL_MEMB_POSD_SNiP_OD &rData)
{
	if (nPos<0 || nPos>1) return FALSE;

	if (rData.aChkFor.GetSize()==0) return FALSE;

	STL_LCOM_FORCE_OD &StlLcomF = rData.aChkFor[0];

	StlLcomF.LocalForceD.Initialize();
	StlLcomF.LocalForceD.WebSigy_LForD.Initialize();
	StlLcomF.LocalForceD.WebPl_LForD.Initialize();

	if (nPos==0)
	{
		STL_LFORCE_RIBDECK_CHKUNIT_LCOM RibDeckLFor;
		if (m_mSodLForRibDeck.Lookup(ElemK, RibDeckLFor))
		{
			RibDeckLFor.mRibDeckFor.Lookup(LcomK, StlLcomF.LocalForceD.RibDeckUnit_LForD);      
		}
		//
		STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM WebSigyLFor;
		if (m_mSodLForWeb.Lookup(ElemK, WebSigyLFor))
		{
			WebSigyLFor.mWebSigYFor.Lookup(LcomK, StlLcomF.LocalForceD.WebSigy_LForD);
		}
		//
		STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM WebPlatLfor;
		if (m_mSodLForWebPlate.Lookup(ElemK, WebPlatLfor))
		{
			WebPlatLfor.mWebPlateFor.Lookup(LcomK, StlLcomF.LocalForceD.WebPl_LForD);
		}
	}
	else
	{
		STL_LFORCE_RIBDECK_CHKUNIT_LCOM RibDeckLFor;
		if (m_mSodLForRibDeckJ.Lookup(ElemK, RibDeckLFor))
		{
			RibDeckLFor.mRibDeckFor.Lookup(LcomK, StlLcomF.LocalForceD.RibDeckUnit_LForD);      
		}
		//
		STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM WebSigyLFor;
		if (m_mSodLForWebJ.Lookup(ElemK, WebSigyLFor))
		{
			WebSigyLFor.mWebSigYFor.Lookup(LcomK, StlLcomF.LocalForceD.WebSigy_LForD);
		}
		//
		STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM WebPlatLfor;
		if (m_mSodLForWebPlateJ.Lookup(ElemK, WebPlatLfor))
		{
			WebPlatLfor.mWebPlateFor.Lookup(LcomK, StlLcomF.LocalForceD.WebPl_LForD);
		}
	}

	return TRUE;
}

void CDgnSodDataCtrl::GetCrFlgStrnRibRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNRibRat, double dBFSNRibRat, double &dTFlgSNRibMaxR, double &dBFlgSNRibMaxR, T_SDCR_BASE &rTRib, T_SDCR_BASE &rBRib)
{
	if (nStrnCt==0)
	{
		dTFlgSNRibMaxR = dTFSNRibRat;
		dBFlgSNRibMaxR = dBFSNRibRat;    
		rTRib = LcomKeyD;
		rBRib = LcomKeyD;
	}
	else
	{            
		// Top
		if (dTFlgSNRibMaxR<dTFSNRibRat)
		{
			dTFlgSNRibMaxR = dTFSNRibRat;
			rTRib = LcomKeyD;
		}
		// Bottom
		if (dBFlgSNRibMaxR<dBFSNRibRat)
		{
			dBFlgSNRibMaxR = dBFSNRibRat;
			rBRib = LcomKeyD;
		}
	}
}

void CDgnSodDataCtrl::GetCrFlgStrnDeckRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNDeckRat, double dBFSNDeckRat, double &dTFlgSNDeckMaxR, double &dBFlgSNDeckMaxR, T_SDCR_BASE &rTDeck, T_SDCR_BASE &rBDeck)
{
	if (nStrnCt==0)
	{
		dTFlgSNDeckMaxR = dTFSNDeckRat;
		dBFlgSNDeckMaxR = dBFSNDeckRat;    
		rTDeck = LcomKeyD;
		rBDeck = LcomKeyD;
	}
	else
	{            
		// Top
		if (dTFlgSNDeckMaxR<dTFSNDeckRat)
		{
			dTFlgSNDeckMaxR = dTFSNDeckRat;
			rTDeck = LcomKeyD;
		}
		// Bottom
		if (dBFlgSNDeckMaxR<dBFSNDeckRat)
		{
			dBFlgSNDeckMaxR = dBFSNDeckRat;
			rBDeck = LcomKeyD;
		}
	}
}

void CDgnSodDataCtrl::GetCrFlgStrnFBeamRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSNFBeamRat, double dBFSNFBeamRat, double &dTFlgSNFBeamMaxR, double &dBFlgSNFBeamMaxR, T_SDCR_BASE &rTFbeam, T_SDCR_BASE &rBFbeam)
{
	if (nStrnCt==0)
	{
		dTFlgSNFBeamMaxR = dTFSNFBeamRat;
		dBFlgSNFBeamMaxR = dBFSNFBeamRat;    
		rTFbeam = LcomKeyD;
		rBFbeam = LcomKeyD;
	}
	else
	{            
		// Top
		if (dTFlgSNFBeamMaxR<dTFSNFBeamRat)
		{
			dTFlgSNFBeamMaxR = dTFSNFBeamRat;
			rTFbeam = LcomKeyD;
		}
		// Bottom
		if (dBFlgSNFBeamMaxR<dBFSNFBeamRat)
		{
			dBFlgSNFBeamMaxR = dBFSNFBeamRat;
			rBFbeam = LcomKeyD;
		}
	}
}

void CDgnSodDataCtrl::GetCrWebStrnRibRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSNRibRat, double &dWebSNRibMaxR, T_SDCR_BASE &rWRib)
{
	if (nStrnCt==0)
	{
		dWebSNRibMaxR = dWebSNRibRat;
		rWRib = LcomKeyD;
	}
	else
	{            
		if (dWebSNRibMaxR<dWebSNRibRat)
		{
			dWebSNRibMaxR = dWebSNRibRat;
			rWRib = LcomKeyD;
		}
	}
}

void CDgnSodDataCtrl::GetCrWebStrnDeckRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSNDeckRat, double &dWebSNDeckMaxR, T_SDCR_BASE &rWDeck)
{
	if (nStrnCt==0)
	{
		dWebSNDeckMaxR = dWebSNDeckRat;
		rWDeck = LcomKeyD;
	}
	else
	{            
		if (dWebSNDeckMaxR<dWebSNDeckRat)
		{
			dWebSNDeckMaxR = dWebSNDeckRat;
			rWDeck = LcomKeyD;
		}
	}
}


void CDgnSodDataCtrl::GetCrGDStabRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dStbGDRat,  double &dSBGDMaxR, T_SDCR_BASE &rGDCr)
{
	if (nStrnCt==0)
	{
		dSBGDMaxR = dStbGDRat;
		rGDCr = LcomKeyD;
	}
	else
	{            
		if (dSBGDMaxR<dStbGDRat)
		{
			dSBGDMaxR = dStbGDRat;
			rGDCr     = LcomKeyD;
		}
	}
}

void CDgnSodDataCtrl::GetCrFlgStabRibRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBRibRat, double dBFSBRibRat, double &dTFlgSBRibMaxR, double &dBFlgSBRibMaxR, T_SDCR_BASE &rTRib, T_SDCR_BASE &rBRib)
{
	if (nStrnCt==0)
	{
		dTFlgSBRibMaxR = dTFSBRibRat;
		dBFlgSBRibMaxR = dBFSBRibRat;    
		rTRib = LcomKeyD;
		rBRib = LcomKeyD;
	}
	else
	{            
		// Top
		if (dTFlgSBRibMaxR<dTFSBRibRat)
		{
			dTFlgSBRibMaxR = dTFSBRibRat;
			rTRib = LcomKeyD;
		}
		// Bottom
		if (dBFlgSBRibMaxR<dBFSBRibRat)
		{
			dBFlgSBRibMaxR = dBFSBRibRat;
			rBRib = LcomKeyD;
		}
	}
}

void CDgnSodDataCtrl::GetCrFlgStabDeckRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBDeckRat, double dBFSBDeckRat, double &dTFlgSBDeckMaxR, double &dBFlgSBDeckMaxR, T_SDCR_BASE &rTDeck, T_SDCR_BASE &rBDeck)
{
	if (nStrnCt==0)
	{
		dTFlgSBDeckMaxR = dTFSBDeckRat;
		dBFlgSBDeckMaxR = dBFSBDeckRat;    
		rTDeck = LcomKeyD;
		rBDeck = LcomKeyD;
	}
	else
	{            
		// Top
		if (dTFlgSBDeckMaxR<dTFSBDeckRat)
		{
			dTFlgSBDeckMaxR = dTFSBDeckRat;
			rTDeck = LcomKeyD;
		}
		// Bottom
		if (dBFlgSBDeckMaxR<dBFSBDeckRat)
		{
			dBFlgSBDeckMaxR = dBFSBDeckRat;
			rBDeck = LcomKeyD;
		}
	}
}

void CDgnSodDataCtrl::GetCrFlgStabFBeamRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dTFSBFBeamRat, double dBFSBFBeamRat, double &dTFlgSBFBeamMaxR, double &dBFlgSBFBeamMaxR, T_SDCR_BASE &rTFbeam, T_SDCR_BASE &rBFbeam)
{
	if (nStrnCt==0)
	{
		dTFlgSBFBeamMaxR = dTFSBFBeamRat;
		dBFlgSBFBeamMaxR = dBFSBFBeamRat;    
		rTFbeam = LcomKeyD;
		rBFbeam = LcomKeyD;
	}
	else
	{            
		// Top
		if (dTFlgSBFBeamMaxR<dTFSBFBeamRat)
		{
			dTFlgSBFBeamMaxR = dTFSBFBeamRat;
			rTFbeam = LcomKeyD;
		}
		// Bottom
		if (dBFlgSBFBeamMaxR<dBFSBFBeamRat)
		{
			dBFlgSBFBeamMaxR = dBFSBFBeamRat;
			rBFbeam = LcomKeyD;
		}
	}
}

void CDgnSodDataCtrl::GetCrWebStabRibRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSBRibRat, double &dWebSBRibMaxR, T_SDCR_BASE &rWRib)
{
	if (nStrnCt==0)
	{
		dWebSBRibMaxR = dWebSBRibRat;
		rWRib = LcomKeyD;
	}
	else
	{            
		if (dWebSBRibMaxR<dWebSBRibRat)
		{
			dWebSBRibMaxR = dWebSBRibRat;
			rWRib = LcomKeyD;
		}
	}
}

void CDgnSodDataCtrl::GetCrWebStabDeckRatio(int nStrnCt, const T_SDCR_BASE &LcomKeyD, double dWebSBDeckRat, double &dWebSBDeckMaxR, T_SDCR_BASE &rWDeck)
{
	if (nStrnCt==0)
	{
		dWebSBDeckMaxR = dWebSBDeckRat;
		rWDeck = LcomKeyD;
	}
	else
	{            
		if (dWebSBDeckMaxR<dWebSBDeckRat)
		{
			dWebSBDeckMaxR = dWebSBDeckRat;
			rWDeck = LcomKeyD;
		}
	}
}


void CDgnSodDataCtrl::GetCrFatigueRatio(int nFatiCt, const T_SDCR_BASE &LcomKeyD, double dFatiRat, double &dFatiMaxR, T_SDCR_BASE &rData)
{
	if (nFatiCt==0)
	{
		dFatiMaxR = dFatiRat;
		rData = LcomKeyD;
	}
	else
	{            
		if (dFatiMaxR<dFatiRat)
		{
			dFatiMaxR = dFatiRat;
			rData = LcomKeyD;
		}
	}
}

void CDgnSodDataCtrl::SetSodCrResultKey(int nLcomK, int nMaxMin, int nConcurrent, int nDgnLcomK, T_SDCR_BASE &rData)
{
	rData.nLcomK      = nLcomK;
	rData.nMaxMin     = nMaxMin;
	rData.nConcurrent = nConcurrent;
	rData.nDgnLcomK   = nDgnLcomK;
}

BOOL CDgnSodDataCtrl::Get_UnbracedLengthData(T_ELEM_K ElemK, BOOL bKfac, T_KFAC_D &KFacD, STL_MEMB_ULEN &rData)
{
	T_LENG_D LengD; LengD.Initialize();
	BOOL bAutoCalc=FALSE;
	double dLength=0.0;
	m_pForcCtrl->m_pDgnForceCtrl->m_pDgnDataCtrl->Get_DgnGenLeng(ElemK, 4, LengD, dLength);
	
	rData.dLy = LengD.BLy;
	rData.dLz = LengD.BLz;
	rData.dLb = LengD.BLu;
	
	if (bKfac)
	{
		rData.dKy = KFacD.BKy;
		rData.dKz = KFacD.BKz;
	}
	else
	{
		rData.dKy = rData.dKz = 1.0;
	}
	return TRUE;
}

BOOL CDgnSodDataCtrl::Get_UnbracedLengthData(T_ELEM_K ElemK, STL_MEMB_ULEN &rData, BOOL &bUnLeng, BOOL &bEffLengF)
{
	BOOL bLeng = FALSE;
	BOOL bKfac = FALSE;

	T_LENG_D LengD; LengD.Initialize();
	double dLength=0.0;
	if(m_pForcCtrl->m_pDgnForceCtrl->m_pDgnDataCtrl->Get_DgnGenLeng(ElemK, 4, LengD, dLength))
	{
		bLeng = TRUE;      
		rData.dLy = LengD.BLy;
		rData.dLz = LengD.BLz;
		rData.dLb = LengD.BLu;
	}

	CDgnDataCtrl DgnCtrl;
	T_KFAC_D KFacD; KFacD.Initialize();

	if(m_pDoc->m_pAttrCtrl->GetKfac(ElemK,KFacD))
	{
		bKfac = TRUE;
	}

	if (bKfac)
	{
		rData.dKy = KFacD.BKy;
		rData.dKz = KFacD.BKz;
	}
	else
	{
		rData.dKy = rData.dKz = 1.0;
	}

	bUnLeng   = bLeng;
	bEffLengF = bKfac;

	if(!bLeng && !bKfac) return FALSE;

	return TRUE;
}

void CDgnSodDataCtrl::ConvertSODGlobalData(STL_GDGND_OD_D &rData)
{
	T_SDPA_D SdpaD; SdpaD.Initialize();	
	m_pDoc->m_pAttrCtrl2->GetSdpa(SdpaD);
	
	rData.nBridType = SdpaD.nBridgeType;
	rData.dm        = SdpaD.dCondiFactor;
	rData.dGamma_n  = SdpaD.dSafeFactor;  
	rData.nChkForType = SdpaD.nStressComp;
	rData.nDgnCode  = GetConvertSodCodeCvlToEng(m_nDgnCode);
	for (int i=0; i<7; ++i) rData.bDgnParam[i] = SdpaD.bDgnParam[i];
}

void CDgnSodDataCtrl::ConvertSodMatlData()
{
	//rData.
}

void CDgnSodDataCtrl::ConvertSodMatl4SectPart()
{
}

void CDgnSodDataCtrl::ConvertSodMatlProp(T_MATD_D &MatD, STL_MATL_SNiP_PROP &rData)
{
	rData.strCodeName = CT2W(MatD.Data1.CodeName);
	rData.strMatlName = CT2W(MatD.Data1.CodeMatlName);
	rData.strSTLGrade = CT2W(MatD.Data1.CodeMatlName);
	rData.dRy         = MatD.Data1.Design.S_Fy1;
	rData.dRyn        = MatD.Data1.Design.S_Fy1;
	//rData.dRs         = MatD.Data1.Design.S_Fy;
	ConvertSodMatlStlPart(MatD.Data1.Analysis, MatD.Data1.Design, rData.Stl);
}
void CDgnSodDataCtrl::ConvertSodMatlStlPart(T_MATL_ANALYSIS &AnalMatl, T_MATL_DESIGN &DgnMatl, STL_MATL_SNiP &rData)
{
	rData.Elast       = AnalMatl.Elast;
	rData.Poisson     = AnalMatl.Poisson;
	rData.Thermal     = AnalMatl.Thermal;
	rData.Density     = AnalMatl.Density;
	rData.MassDensity = AnalMatl.MassDensity;
	rData.S_Fu        = DgnMatl.S_Fu;
	rData.S_Fy        = DgnMatl.S_Fy;
	rData.S_Fy1       = DgnMatl.S_Fy1;
	rData.S_Fy2       = DgnMatl.S_Fy2;
	rData.S_Fy3       = DgnMatl.S_Fy3;
	rData.S_Fy4       = DgnMatl.S_Fy4;
	rData.S_Fy5       = DgnMatl.S_Fy5;
	rData.S_Fy6       = DgnMatl.S_Fy6;
}

void CDgnSodDataCtrl::ConvertSodSectData(int nPos, T_SECT_D *pSectD, T_STPO_D &StpoD, T_SODSTFN_INX_D &rStfnId, STL_SECT_INFO_OD &rData)
{
	if (pSectD==NULL) return;
	if (nPos<0 || nPos>1) return;
	
	int nStype = pSectD->nStype;
	if (pSectD->nStype==D_SECT_TYPE_TAPERED) nStype = pSectD->SectBefore.nStype;
	rData.nStype = GetConvertSectTypeCvlToEng(nStype);
	rData.SName  = pSectD->SName;
	rData.Shape  =  GetConvertSectShapeCvlToEng(pSectD->SectBefore.Shape);
//   rData.SectInfo.nOffsetPoint = pSectD->SectBefore.nOffsetPoint;
//   rData.SectInfo.nLytype      = pSectD->SectBefore.nLytype;
//   rData.SectInfo.nLztype      = pSectD->SectBefore.nLztype;


	if (nPos==0)
	{
		ConvertSodSectBaseData(nStype, &pSectD->SectAfter.SectI, &pSectD->SectBefore.SectI, StpoD, rStfnId, rData.SectOD);
		if (pSectD->nStype==D_SECT_TYPE_TAPERED)
		{
			ConvertSectStiffnessData(pSectD->SectBefore.SectI.Stiffness, rData.SectOD.Stiffness);
		}
		else
		{
			ConvertSectStiffnessData(pSectD->SectBefore.SectI.Stiffness, rData.SectOD.Stiffness);
		}
		
		
		//ConvertCsgSectBaseData(&pSectD->SectAfter.SectJ, rData.SectInfo.Slab);
	}
	else if (nPos==1)
	{
		if (pSectD->nStype==D_SECT_TYPE_TAPERED) 
		{
			ConvertSodSectBaseData(nStype, &pSectD->SectAfter.SectJ, &pSectD->SectBefore.SectJ, StpoD, rStfnId, rData.SectOD);
			ConvertSectStiffnessData(pSectD->SectBefore.SectJ.Stiffness, rData.SectOD.Stiffness);
		}
		else 
		{
			ConvertSodSectBaseData(nStype, &pSectD->SectAfter.SectI, &pSectD->SectBefore.SectI, StpoD, rStfnId, rData.SectOD);
			ConvertSectStiffnessData(pSectD->SectBefore.SectI.Stiffness, rData.SectOD.Stiffness);
		}
		
		//ConvertCsgSectBaseData(&pSectD->SectAfter.SectJ, rData.SectInfo.Slab);
	}	
	rData.SectOD.EffStiffness = rData.SectOD.Stiffness;
	
	
	// **********************
	// 삽도 데이터
	// **********************
	//ConvertCsgSectPolygonData(nPos, pSectD, rData.SectInfo.After);
	ConvertSodSectPolygonData(nPos, pSectD, rData.SectOD);

}

BOOL CDgnSodDataCtrl::ConvertSodSectPolygonData(int nPos, T_SECT_D *pSectD, ST_SECT_SECTBASE_D &rData)
{

	CDb_DllManager_Base DllMgr;
	DGN_GSEC_SHAPE_D SectD; SectD.Initialize();
	int nPosConv = (nPos==0)? 0 : 4;
	if(!DllMgr.ConvertToDLL_SectData_Pos(*pSectD, nPosConv, SectD, TRUE, TRUE)) return FALSE;
	
	DGN_GSEC_POLYGON_LIST    arConcOutPoly;       arConcOutPoly.RemoveAll(); // 콘크리트 슬래브
	DGN_GSEC_POLYGON_LIST    arConcInPoly;        arConcInPoly.RemoveAll();
	DGN_GSEC_POLYGON_LIST    arSteelOutPoly;      arSteelOutPoly.RemoveAll(); // Steel 거더
	DGN_GSEC_POLYGON_LIST    arSteelInPoly;       arSteelInPoly.RemoveAll();
	DGN_GSEC_LINE_LIST       arSteelLine;         arSteelLine.RemoveAll();
	DGN_GSEC_VERTEXLINE_LIST arSteelVertexLine;   arSteelVertexLine.RemoveAll();
	DGN_GSEC_POLYGON_LIST    arSteelLinePoly;     arSteelLinePoly.RemoveAll();
	DGN_POLYGON_POS_LIST     arDeckPos;           arDeckPos.RemoveAll();
	double dOutPos[6];
	for(int i=0 ; i<6 ; i++) dOutPos[i] = 0.0;
	BOOL bLineDetail = TRUE;
	
    if ( !dgn::link::ITmpDgnDrawLib::GetInstance()->Get_SectionPolygonDataSOD(SectD, arDeckPos, arConcOutPoly, arConcInPoly, arSteelOutPoly, arSteelInPoly, arSteelLine, arSteelVertexLine, arSteelLinePoly, dOutPos, -SectD.dYBar, -SectD.dZBar, bLineDetail) ) return FALSE;
	if ( !dgn::link::ITmpDgnDrawLib::GetInstance()->Get_SectionPolygonDataIsCalcStiffSOD(SectD, rData)) return FALSE;

	int	nOutSize1 = arSteelOutPoly.GetSize();
	//int nOutSize2 = arConcOutPoly.GetSize();
	rData.aOuterPolygon.SetSize(nOutSize1);
	rData.arDeckPos.SetSize(nOutSize1);
	
	// 거더+보강재
	for(int i=0 ; i<nOutSize1 ; i++)
	{
		DGN_GSEC_POLYGON  POLYGON;
		int nSize = arSteelOutPoly[i].aVertex.GetSize();
		rData.aOuterPolygon[i].nMatkK = 2;
		rData.aOuterPolygon[i].aVertex.SetSize(nSize);
		rData.arDeckPos[i] = arDeckPos[i];
		
		for(int j=0 ; j<nSize ; j++)
		{
			rData.aOuterPolygon[i].aVertex[j].dX = arSteelOutPoly[i].aVertex[j].dX;
			rData.aOuterPolygon[i].aVertex[j].dY = arSteelOutPoly[i].aVertex[j].dY;
		}
	}
	

	return TRUE;

}

void CDgnSodDataCtrl::ConvertSodSectBaseData(int nStype, T_SECT_SECTBASE_D *pSectAfter, T_SECT_SECTBASE_D *pSectD, T_STPO_D &StpoD, T_SODSTFN_INX_D &rStfnId, ST_SECT_SECTBASE_D &rData)
{
	if (pSectD==NULL) return;
	
	rData.Shape  = pSectD->Shape;
	rData.DBName = pSectD->DBName;
	rData.SName  = pSectD->SName;
	rData.SName2 = pSectD->SName2;
	
	ConvertSodSectSize(nStype, pSectAfter, pSectD, rData);
	memcpy(rData.EffSize, rData.Size, sizeof(rData.EffSize));

	ConvertSodStiffenerData(nStype, pSectD, StpoD, &pSectD->SODStiffener, rData.Stiffener);
	//
	MakeSectStfnIndexInfo(nStype, pSectD, rStfnId);  
}

void CDgnSodDataCtrl::ConvertSodSectSize(int nStype, T_SECT_SECTBASE_D *pSectAfter, T_SECT_SECTBASE_D *pSectD, ST_SECT_SECTBASE_D &rData)
{
	double dTopL = pSectAfter->Size[0];
	double dBotL = pSectAfter->Size[1];  
	if (nStype==D_SECT_TYPE_STLG_I)
	{
		double dHw = pSectD->Size[4];
		double dtt = pSectD->Size[5];
		double dtb = pSectD->Size[6];
		double dHt = dHw+dtt+dtb;
		rData.Size[ 1] = dHw; // Hw
		rData.Size[ 2] = pSectD->Size[7];
		rData.Size[ 7] = dtt; // tt
		rData.Size[11] = dtb; // tb
		rData.Size[ 0] = dHt;
		
		double dBtl = pSectD->Size[0];
		double dBtr = pSectD->Size[1];
		double dBt  = dBtl + dBtr;
		rData.Size[ 4] = dBt;  
		rData.Size[ 5] = dBtl; 
		rData.Size[ 6] = dBtr;
		
		double dBbl = pSectD->Size[2];
		double dBbr = pSectD->Size[3];
		double dBb  = dBbl + dBbr;
		rData.Size[ 8] = dBb;  
		rData.Size[ 9] = dBbl; 
		rData.Size[10] = dBbr;

		double dSide = (dTopL + dBtl) - (dBotL + dBbl);
		double dTheta = 0.0;
		if (fabs(dSide)<m_dZero)
		{
			dTheta = 0.5*CMathFunc::m_pi;
		}
		else if (dSide>0.0)
		{
			double dTanV = (dHw==0.0) ? 0.0 : dSide/dHw;
			double dAngRad  = atan(dTanV);
			dTheta = 0.5*CMathFunc::m_pi - dAngRad;
		}
		else if (dSide<0.0)
		{
			double dTanV = (dHw==0.0) ? 0.0 : dSide/dHw;
			double dAngRad  = atan(dTanV);
			dTheta = 0.5*CMathFunc::m_pi + dAngRad;
		}
		rData.Size[3] = dTheta;
	}
	else if (nStype==D_SECT_TYPE_STLG_B)
	{
		double dHw = pSectD->Size[6];
		double dtt = pSectD->Size[7];
		double dtb = pSectD->Size[8];
		double dHt = dHw+dtt+dtb;
		rData.Size[ 1] = dHw; // Hw
		rData.Size[ 2] = pSectD->Size[9];  // twl
		rData.Size[ 3] = pSectD->Size[10]; // twr

		rData.Size[10] = dtt; // tt
		rData.Size[15] = dtb; // tb
		rData.Size[ 0] = dHt;
		
		double dBtl = pSectD->Size[0];
		double dBtm = pSectD->Size[1];
		double dBtr = pSectD->Size[2];
		double dBt  = dBtl + dBtm + dBtr;
		rData.Size[ 6] = dBt;  
		rData.Size[ 7] = dBtl; 
		rData.Size[ 8] = dBtm;
		rData.Size[ 9] = dBtr;
		
		double dBbl = pSectD->Size[3];
		double dBbm = pSectD->Size[4];
		double dBbr = pSectD->Size[5];
		double dBb  = dBbl + dBbm + dBbr;
		rData.Size[11] = dBb;  
		rData.Size[12] = dBbl; 
		rData.Size[13] = dBbm;
		rData.Size[14] = dBbr;

		double dSideL = (dTopL + dBtl) - (dBotL + dBbl);
		double dThetaL = 0.0;
		if (fabs(dSideL)<m_dZero)
		{
			dThetaL = 0.5*CMathFunc::m_pi;
		}
		else if (dSideL>0.0)
		{
			double dTanV = (dHw==0.0) ? 0.0 : dSideL/dHw;
			double dAngRad  = atan(dTanV);
			dThetaL = 0.5*CMathFunc::m_pi + dAngRad;
		}
		else if (dSideL<0.0)
		{
			double dTanV = (dHw==0.0) ? 0.0 : dSideL/dHw;
			double dAngRad  = atan(dTanV);
			dThetaL = 0.5*CMathFunc::m_pi - dAngRad;
		}
		rData.Size[4] = dThetaL;

		double dSideR = (dTopL + dBtl + dBtm) - (dBotL + dBbl + dBbm); 
		double dThetaR = 0.0;
		if (fabs(dSideR)<m_dZero)
		{
			dThetaR = 0.5*CMathFunc::m_pi;
		}
		else if (dSideR>0.0)
		{
			double dTanV = (dHw==0.0) ? 0.0 : dSideR/dHw;
			double dAngRad  = atan(dTanV);
			dThetaR = 0.5*CMathFunc::m_pi - dAngRad;
		}
		else if (dSideR<0.0)
		{
			double dTanV = (dHw==0.0) ? 0.0 : dSideR/dHw;
			double dAngRad  = atan(dTanV);
			dThetaR = 0.5*CMathFunc::m_pi + dAngRad;
		}
		rData.Size[5] = dThetaR;
	}
	else ASSERT(0);

}

void CDgnSodDataCtrl::MakeSectStfnIndexInfo(int nStype, T_SECT_SECTBASE_D *pSectD, T_SODSTFN_INX_D &rData)
{
	int nStiffSubSize = pSectD->SODStiffener.aSODStiffSub.GetSize();

	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aStiffData;
	for (int i=0; i<nStiffSubSize; ++i)
	{
		T_SECT_SOD_STIFF_SUB_D &PartStfnD = pSectD->SODStiffener.aSODStiffSub[i];
		
		MakeSectStfnPropileIndex(nStype, pSectD, PartStfnD, rData);    
	}
}

int CompareStiffSpace(const void* v1, const void* v2)
{
	T_SECT_SOD_STIFF_DATA_D* p1 = (T_SECT_SOD_STIFF_DATA_D*)v1;
	T_SECT_SOD_STIFF_DATA_D* p2 = (T_SECT_SOD_STIFF_DATA_D*)v2;
	if (p1->dSpacing > p2->dSpacing) return 1;
	else if (p1->dSpacing < p2->dSpacing) return -1;
	return 0;
}

void CDgnSodDataCtrl::MakeSectStfnPropileIndex(int nStype, T_SECT_SECTBASE_D *pSectD, T_SECT_SOD_STIFF_SUB_D &PartStfn, T_SODSTFN_INX_D &rData)
{
	if (PartStfn.nDeckPart<0 || PartStfn.nDeckPart>2) return;

	int nLStfnSize = PartStfn.aStiffData.GetSize();
	if (nLStfnSize<=0) return;
	
	T_SODSTFN_INX_MAP *pStfnInxMap = NULL;

	BOOL   bReCalSpace = IsReplaceStfnIndex(nStype, PartStfn.nRefPos, PartStfn.nDeckPos, PartStfn.nDeckPart);
	if (nStype==D_SECT_TYPE_STLG_B)
	{
		if (PartStfn.nDeckPos==0) // Top
		{
			if (PartStfn.nDeckPart==0) // Left Outstand
			{
				pStfnInxMap = &rData.StfnTL;
			}
			else if (PartStfn.nDeckPart==1) // Internal
			{        
				pStfnInxMap = &rData.StfnTM;        
			}
			else if (PartStfn.nDeckPart==2) // Right Outstand
			{
				pStfnInxMap = &rData.StfnTR;
			}
			else ASSERT(0);            
		}
		else if (PartStfn.nDeckPos==1) // web.L
		{
			pStfnInxMap = &rData.StfnWL;              
		}
		else if (PartStfn.nDeckPos==2) // web.R
		{
			pStfnInxMap = &rData.StfnWR;      
		}
		else if (PartStfn.nDeckPos==3) // Bottom
		{
			if (PartStfn.nDeckPart==0) // Left Outstand
			{
				pStfnInxMap = &rData.StfnBL;        
			}
			else if (PartStfn.nDeckPart==1) // Internal
			{        
				pStfnInxMap = &rData.StfnBM;                
			}
			else if (PartStfn.nDeckPart==2) // Right Outstand
			{
				pStfnInxMap = &rData.StfnBR;        
			}
			else ASSERT(0);                  
		}
		else ASSERT(0);
	}
	else if (nStype==D_SECT_TYPE_STLG_I)
	{
		if (PartStfn.nDeckPos==0) // Top
		{
			if (PartStfn.nDeckPart==0) // Left Outstand
			{
				pStfnInxMap = &rData.StfnTL;        
			}
			else if (PartStfn.nDeckPart==1) // Right Outstand
			{
				pStfnInxMap = &rData.StfnTR;                
			}
			else ASSERT(0);   
		}
		else if (PartStfn.nDeckPos==1) // web
		{
			pStfnInxMap = &rData.StfnWL;                    
		}
		else if (PartStfn.nDeckPos==2) // Bottom
		{
			if (PartStfn.nDeckPart==0) // Left Outstand
			{
				pStfnInxMap = &rData.StfnBL;                
			}
			else if (PartStfn.nDeckPart==1) // Right Outstand
			{
				pStfnInxMap = &rData.StfnBR;                
			}
			else ASSERT(0);                  
		}
		else ASSERT(0);
	}
	else ASSERT(0);

	CArray<T_SECT_SOD_STIFF_DATA_D, T_SECT_SOD_STIFF_DATA_D&> aLStiffD;
	aLStiffD.Copy(PartStfn.aStiffData);

//   for (int i=0; i<nLStfnSize; ++i)
//   {
//     T_SECT_SOD_STIFF_DATA_D &StiffData = aLStiffD[i];
//     StiffData.dSpacing = dWidth - StiffData.dSpacing;        
//   }

	//qsort(aLStiffD.GetData(), aLStiffD.GetSize(), sizeof(T_SECT_SOD_STIFF_DATA_D), CompareStiffSpace);

	for (int i=0; i<nLStfnSize; ++i)
	{
		int nId = (bReCalSpace==TRUE) ? nLStfnSize-1-i : i;
		pStfnInxMap->mStfnIdx.SetAt(aLStiffD[i].strStiffName, nId);
	}
}

CString CDgnSodDataCtrl::GetDeckPartName(int nId)
{
	CString strName;
	return strName;
}

void CDgnSodDataCtrl::ConvertSodKappaFactor(T_SDPS_SUB_D &InD, STL_KAPPA_INPUT_D &rData)
{
	rData.bAutoCalc   = (InD.nCalcType==0);
	rData.dUDKappa_y  = InD.dKappay;
	rData.dUDKappa_z  = InD.dKappaz;
}

void CDgnSodDataCtrl::ConvertSodKappaFactorChkPos(T_SDPS_SUB_D &InD, STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD &rData)
{
	CArray<T_POINT_SSM, T_POINT_SSM&> aKappaPt;
	aKappaPt.Copy(InD.aDefault);
	aKappaPt.Append(InD.aUserSel);
	int nKappaPtSize = aKappaPt.GetSize();
	rData.aKappaChkPosInfo.SetSize(nKappaPtSize);
	for (int i=0; i<nKappaPtSize; ++i)
	{
		ConvertSodKappaFactorChkPosOne(aKappaPt[i], rData.aKappaChkPosInfo[i]);
	}  
}

void CDgnSodDataCtrl::ConvertSodKappaFactorChkPosDefault(CArray<T_POINT_SSM, T_POINT_SSM&>& aStressPoint, STL_POSD_DGND_SNiP_CHK_KAPPA_POS_INFO_OD &rData)
{
	CArray<T_POINT_SSM, T_POINT_SSM&> aKappaPt; aKappaPt.RemoveAll();
	aKappaPt.Copy(aStressPoint);
	int nKappaPtSize = aKappaPt.GetSize();
	rData.aKappaChkPosInfo.SetSize(nKappaPtSize);
	for (int i=0; i<nKappaPtSize; ++i)
	{
		ConvertSodKappaFactorChkPosOne(aKappaPt[i], rData.aKappaChkPosInfo[i]);
	}  
}

void CDgnSodDataCtrl::ConvertSodKappaFactorChkPosOne(T_POINT_SSM &InD, STL_MEMB_POSD_SNiP_CHK_KAPPA &rData)
{
	rData.bChk    = TRUE;
	rData.nIndex  = 0;
	rData.strFatPosName = _T("");
	rData.Point.dX      = InD.dPointY;
	rData.Point.dY      = InD.dPointZ;
	rData.Eff_Point.dX  = InD.dPointY;
	rData.Eff_Point.dY  = InD.dPointZ;


// 	int nDeck=-1, int nDeckPart=-1, int nRibNum=0;
// 	CSectUtil::CheckPointInSODKappaPoint(T_SECT_D& SectD, double dPos, 0.0, 0.0, InD.dPointY, InD.dPointZ, nDeck, nDeckPart, nRibNum);


}

void CDgnSodDataCtrl::ConvertSodFatiCheckPos(int nPos, T_SDFC_D &InD, STL_POSD_DGND_SNiP_FAT_CHK_INFO_OD &rData)
{
	if (nPos<0 || nPos>1) return;
	//if (InD.bSameIJ==TRUE && nPos==1) InD.aSdfcSub[1].Copy(InD.aSdfcSub[0]); // 데이터 없을 경우 풀자. 
	double dLambda = InD.dLambda[nPos];
	int nFatiSize = InD.aSdfcSub[nPos].GetSize();
	rData.aFatChkPosInfo.SetSize(nFatiSize);
	for (int i=0; i<nFatiSize; ++i)
	{
		ConvertSodFatiCheckPosOne(InD.aSdfcSub[nPos].GetAt(i), rData.aFatChkPosInfo[i]);
		rData.aFatChkPosInfo[i].dLambda = dLambda;
	}
	//rData.dLambda = InD.dLambda[nPos];

}

void CDgnSodDataCtrl::ConvertSodFatiCheckPosOne(T_SDFC_SUB_D &InD, STL_MEMB_POSD_SNiP_FAT &rData)
{
	rData.nIndex  = 0;
	rData.strFatPosName = _T("");
	rData.Point.dX     = InD.Point.dPointY;
	rData.Point.dY     = InD.Point.dPointZ;
	rData.Eff_Point.dX = InD.Point.dPointY;
	rData.Eff_Point.dY = InD.Point.dPointZ;
	rData.dBeta   = InD.dBeta;
	rData.dLambda = 0.0;
}

void CDgnSodDataCtrl::ConvertSodLocalForce4Deck(T_SECT_K SectK, int nPos, T_SDLF_D &InD, STL_LFORCE_RIBDECK_CHKUNIT_LCOM &rData)
{
	int nChkPos = (InD.bSameIJ==TRUE) ? 0 : nPos;
	CArray<T_SDLF_SUB_D, T_SDLF_SUB_D&> &aLForce = (nChkPos==0) ? InD.aSdlfSub[0] : InD.aSdlfSub[1];
	int nLForSize = aLForce.GetSize();

	CMap<CString,LPCTSTR,int,int&> mMap;
	for (int i=0; i<nLForSize; ++i)
	{
		T_SDLF_SUB_D &StfnFor = aLForce[i];

		T_SODSECT_STFN_INX StfnIdxD;
		if (m_SectStfnIdx.Lookup(SectK, StfnIdxD))
		{
			int nIndex = 0;
			if (StfnIdxD.Stiff[nPos].StfnTL.mStfnIdx.Lookup(StfnFor.strStiffener, nIndex))
			{
				STL_LFORCE_RIBDECK_CHKUNIT_D LForRibDeckD;
				if (rData.mRibDeckFor.Lookup(StfnFor.LcomK, LForRibDeckD))
				{
					STL_LFORCE_RIBDECK_D &LForRD = LForRibDeckD.aRibDeckLForce_TopL[nIndex];
					ConvertSodLocalForce4DeckBase(StfnFor, LForRD);
					//LForRibDeckD.aRibDeckLForce_TopL.SetAt(nIndex, LForRD);
				}
				else
				{
					ConvertSodLocalForceArraySize(nPos, StfnIdxD, LForRibDeckD);
					STL_LFORCE_RIBDECK_D &LForRD = LForRibDeckD.aRibDeckLForce_TopL[nIndex];
					ConvertSodLocalForce4DeckBase(StfnFor, LForRD);
					//LForRibDeckD.aRibDeckLForce_TopL.SetAt(nIndex, LForRD);
				}
				rData.mRibDeckFor.SetAt(StfnFor.LcomK, LForRibDeckD);
			}
			else if (StfnIdxD.Stiff[nPos].StfnTM.mStfnIdx.Lookup(StfnFor.strStiffener, nIndex))
			{
				STL_LFORCE_RIBDECK_CHKUNIT_D LForRibDeckD;
				if (rData.mRibDeckFor.Lookup(StfnFor.LcomK, LForRibDeckD))
				{
					STL_LFORCE_RIBDECK_D &LForRD = LForRibDeckD.aRibDeckLForce_TopM[nIndex];
					ConvertSodLocalForce4DeckBase(StfnFor, LForRD);
					//LForRibDeckD.aRibDeckLForce_TopM.SetAt(nIndex, LForRD);
				}
				else
				{
					ConvertSodLocalForceArraySize(nPos, StfnIdxD, LForRibDeckD);
					STL_LFORCE_RIBDECK_D &LForRD = LForRibDeckD.aRibDeckLForce_TopM[nIndex];
					ConvertSodLocalForce4DeckBase(StfnFor, LForRD);
					//LForRibDeckD.aRibDeckLForce_TopM.SetAt(nIndex, LForRD);
				}
				rData.mRibDeckFor.SetAt(StfnFor.LcomK, LForRibDeckD);
			}
			else if (StfnIdxD.Stiff[nPos].StfnTR.mStfnIdx.Lookup(StfnFor.strStiffener, nIndex))
			{
				STL_LFORCE_RIBDECK_CHKUNIT_D LForRibDeckD;
				if (rData.mRibDeckFor.Lookup(StfnFor.LcomK, LForRibDeckD))
				{
					STL_LFORCE_RIBDECK_D &LForRD = LForRibDeckD.aRibDeckLForce_TopR[nIndex];
					ConvertSodLocalForce4DeckBase(StfnFor, LForRD);
					//LForRibDeckD.aRibDeckLForce_TopR.SetAt(nIndex, LForRD);
				}
				else
				{
					ConvertSodLocalForceArraySize(nPos, StfnIdxD, LForRibDeckD);
					STL_LFORCE_RIBDECK_D &LForRD = LForRibDeckD.aRibDeckLForce_TopR[nIndex];
					ConvertSodLocalForce4DeckBase(StfnFor, LForRD);
					//LForRibDeckD.aRibDeckLForce_TopR.SetAt(nIndex, LForRD);
				}
				rData.mRibDeckFor.SetAt(StfnFor.LcomK, LForRibDeckD);
			}
		}
	}

}

void CDgnSodDataCtrl::ConvertSodLocalForce4DeckBase(T_SDLF_SUB_D &InD, STL_LFORCE_RIBDECK_D &rData)
{
	rData.bInputType    = (InD.nRibType==0);
	rData.bIsExist_Rib  = TRUE;
	rData.bIsExist_Deck = TRUE;
	rData.dMy           = InD.dRibMy;
	rData.dFy           = InD.dRibFz;
	rData.dN            = InD.dRibN;
	rData.dSigx_Rib     = InD.dRibSigx;
	rData.dSigy_Rib     = InD.dRibSigy;
	rData.dTauxy_Rib    = InD.dRibSigxy;
	rData.dSigx_Deck    = InD.dDeckX;
	rData.dSigx         = InD.dDeckX;
	rData.dSigy         = InD.dDeckY;
	rData.dTauxy        = InD.dDeckXY;
}

void CDgnSodDataCtrl::ConvertSodLocalForceArraySize(int nPos, T_SODSECT_STFN_INX &InD, STL_LFORCE_RIBDECK_CHKUNIT_D &rData)
{
	rData.aRibDeckLForce_TopL.SetSize(InD.Stiff[nPos].StfnTL.mStfnIdx.GetCount());
	rData.aRibDeckLForce_TopM.SetSize(InD.Stiff[nPos].StfnTM.mStfnIdx.GetCount());
	rData.aRibDeckLForce_TopR.SetSize(InD.Stiff[nPos].StfnTR.mStfnIdx.GetCount());
}

bool CDgnSodDataCtrl::ConvertSodFloorBeam(int nPos, T_SDTS_D &InD, STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_CHKUNIT_D &rData)
{
	CString strPart = _LS(IDS_CMD_SECT_SOD_TOP_LEFT);
	if(!ConvertSodFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_TL)){return false;}
	strPart = _LS(IDS_CMD_SECT_SOD_TOP_CENTER);
	if(!ConvertSodFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_TM)){return false;}
	strPart = _LS(IDS_CMD_SECT_SOD_TOP_RIGHT);
	if(!ConvertSodFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_TR)){return false;}
	strPart = _LS(IDS_CMD_SECT_SOD_BOT_LEFT);
	if(!ConvertSodFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_BL)){return false;}
	strPart = _LS(IDS_CMD_SECT_SOD_BOT_CENTER);
	if(!ConvertSodFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_BM)){return false;}
	strPart = _LS(IDS_CMD_SECT_SOD_BOT_RIGHT);
	if(!ConvertSodFloorBeamPart(nPos, strPart, InD, rData.FloorBDeckP_Dgn_StaD_BR)){return false;}

	return true;
}

bool CDgnSodDataCtrl::ConvertSodFloorBeamPart(int nPos, CString &strFBPartName, T_SDTS_D &InD, STL_MEMB_POSD_SNiP_DGN_STA_FB_DP_D &rData)
{
	if (nPos<0 || nPos>1) return false;

	CArray<T_SDTS_BEAM_SUB_D, T_SDTS_BEAM_SUB_D&> *paBeam = (nPos==0) ? &InD.Beam[0].aBeam : &InD.Beam[1].aBeam;
	CArray<T_SDTS_DECK_SUB_D, T_SDTS_DECK_SUB_D&> *paDeck = (nPos==0) ? &InD.Deck[0].aDeck : &InD.Deck[1].aDeck;

	/// [CIVIL-10243] deck name이 영어로 저장되어 있는데, 다국어 문자열과 비교하면서 잘못 판단되는 문제 수정.
	const CString& strFBPartNameEng = CSectUtil::GetDeckNameMultiLang2Eng(strFBPartName);
	// Beam Part

	BOOL bFindPart=FALSE;
	int nPartSize = paBeam->GetSize();
	T_SDTS_BEAM_SUB_D FbeamPart; FbeamPart.Initialize();
	for (int i=0; i<nPartSize; ++i)
	{
		T_SDTS_BEAM_SUB_D &SdtsBS = paBeam->GetAt(i);
		if (SdtsBS.strDeck.CompareNoCase(strFBPartNameEng)==0)
		{
			bFindPart=TRUE;
			FbeamPart = SdtsBS;
			break;
		}
	}
	if (bFindPart==FALSE) {return true;}

	// Get Force
	for(int i=0; i <3 ; ++i) 
	{
		rData.aFB_Force[i].dN  = FbeamPart.Force[i].dN;
		rData.aFB_Force[i].dMy = FbeamPart.Force[i].dMy;
		rData.aFB_Force[i].dFy = FbeamPart.Force[i].dFz;
		rData.dhc[i] = FbeamPart.Force[i].dhc;
	}  

	CString strFBSectName=_T("");
	T_SDTS_SECT_D SdtsSect;
	for (int i=0; i<InD.aSect.GetSize(); ++i)
	{
		if (InD.aSect[i].strName.CompareNoCase(FbeamPart.strSect)==0) 
		{
			SdtsSect = InD.aSect[i];
			break;
		}
	}

	// Get Section of floor beam
	rData.SectFBInfo.SName = SdtsSect.strName;
	rData.SectFBInfo.nSectType = DGN_SECT_SHAPE_INDEX_REG_H;
	int nSize = 8;
	for (int i=0; i<nSize; ++i)
	{
		rData.SectFBInfo.Size[i] = SdtsSect.dSize[i];
	}
	
	T_SECT_D CalcS; CalcS.Initialize();
	T_SECT_STIFFNESS &StfnStiff = CalcS.SectBefore.SectI.Stiffness;
	CalcS.nStype = D_SECT_TYPE_REGULAR;
	CalcS.SectBefore.Shape = D_SECT_SHAPE_REG_H;
	for (int i=0; i<nSize; ++i) CalcS.SectBefore.SectI.Size[i] = SdtsSect.dSize[i];

	if(!CSectUtil::CalcSectData(CalcS)){return false;}
	
	ConvertSectStiffnessData(CalcS.SectBefore.SectI.Stiffness, rData.SectFBInfo.Stiffness);
	ConvertSectStiffnessData(CalcS.SectBefore.SectI.Stiffness, rData.SectFBInfo.EffStiffness);

	// Get Material of floor beam
	T_MATD_D MatdD; MatdD.Initialize();
	if (m_pDoc->m_pAttrCtrl->GetMatlDesign(FbeamPart.MatlK, MatdD))
	{          
		ConvertSodMatlProp(MatdD, rData.FB_MatD);   
	}


	// Get Deck Part Info.
	bFindPart=FALSE;
	nPartSize = paDeck->GetSize();
	T_SDTS_DECK_SUB_D FloorBeamD; FloorBeamD.Initialize();
	for (int i=0; i<nPartSize; ++i)
	{
		T_SDTS_DECK_SUB_D &SdtsBS = paDeck->GetAt(i);
		if (SdtsBS.strDeck.CompareNoCase(strFBPartNameEng)==0)
		{
			bFindPart=TRUE;
			FloorBeamD = SdtsBS;
			break;
		}
	}
	if (bFindPart==FALSE) {return true;}

	rData.bAutoCalck = (FloorBeamD.nKType==0);
	rData.bAutoCalca = (FloorBeamD.naType==0);
	rData.nk         = (int)FloorBeamD.dk;        
	rData.da         = FloorBeamD.da;        
	rData.dl         = FloorBeamD.dI;        
	rData.dL         = FloorBeamD.dL;        
	rData.df         = FloorBeamD.df;        

	 return true;
}

void CDgnSodDataCtrl::ConvertSodWebPlateInfo()
{
}

void CDgnSodDataCtrl::ConvertSodWebVerticalForce(int nStype, int nPos, T_SDAP_D &InD, STL_LFORCE_WEB_SIGY_CHKUNIT_LCOM &rForWeb, STL_LFORCE_WEB_PLATE_CHKUNIT_LCOM &rForWebP)
{
	if (nPos<0 || nPos>1) return;

	int nBetaType = InD.nBetaType[nPos];
	int nConnectType = InD.nConnectType[nPos] == 0 ? 1 : 0;

	double dBeta = 0.0;
	if     (nBetaType==0) dBeta = 0.3;
	else if(nBetaType==1) dBeta = 0.5;
	else if(nBetaType==2) dBeta = 0.8;
	else if(nBetaType==3) dBeta = 2.0;
	else if(nBetaType==4) dBeta = 1.5;
	else if(nBetaType==5) dBeta = 2.0;
	else ASSERT(0);
	

	int nDataSize = InD.aSdapSub[nPos].GetSize();

	for (int i=0; i<nDataSize; ++i)
	{
		T_SDAP_SUB_D &SdapSub = InD.aSdapSub[nPos].GetAt(i);

		STL_LFORCE_WEB_PLATE_CHKUNIT_D WebPlateD;
		ConvertSodWebPlateForce(nStype, dBeta, nConnectType, SdapSub, WebPlateD);
		rForWebP.mWebPlateFor.SetAt(SdapSub.LcomK, WebPlateD);

		//
		STL_LFORCE_WEB_SIGY_CHKUNIT_D WebSigyD;
		ConvertSodWebVerticalLcomForce(nStype, SdapSub, WebSigyD);
		rForWeb.mWebSigYFor.SetAt(SdapSub.LcomK, WebSigyD);
	}

}

void CDgnSodDataCtrl::ConvertSodWebPlateForce(int nStype, double dBeta, int nConnectType, T_SDAP_SUB_D &VerFor, STL_LFORCE_WEB_PLATE_CHKUNIT_D &rData)
{
	rData.bIsExistL = TRUE;
	rData.dBeta   = dBeta;
	rData.nFixType = nConnectType;

	int nPlateSizeL = VerFor.aLProp.GetSize();

	rData.aWebForceL_Pl.SetSize(nPlateSizeL);
	for (int i=0; i<nPlateSizeL; ++i)
	{
		CovertSodWebPlateForceBase(VerFor.aLProp[i], rData.aWebForceL_Pl[i]);
	}

	if (nStype==D_SECT_TYPE_STLG_B)
	{
		int nPlateSizeR = VerFor.aRProp.GetSize();
		rData.aWebForceR_Pl.SetSize(nPlateSizeR);
		rData.bIsExistR = TRUE;
		for (int i=0; i<nPlateSizeR; ++i)
		{
			CovertSodWebPlateForceBase(VerFor.aRProp[i], rData.aWebForceR_Pl[i]);
		}
	}
}

void CDgnSodDataCtrl::CovertSodWebPlateForceBase(T_SDAP_PROP_D &InD, STL_LFORCE_WEB_PLATE_D &rData)
{
	rData.da     = InD.dai;
	rData.dh     = InD.dhi;
	rData.dSigx  = InD.dpx;
	rData.dSigy  = InD.dpz;
	rData.dTauxy = InD.dpxz;
}

void CDgnSodDataCtrl::ConvertSodWebVerticalLcomForce(int nStype, T_SDAP_SUB_D &VerFor, STL_LFORCE_WEB_SIGY_CHKUNIT_D &rData)
{
	rData.bTopTypeL = VerFor.TopVert.bUse;
	rData.bBottomTypeL = VerFor.BotVert.bUse;
	rData.nAppForceTypeL = 3;
	rData.bTopTypeR = VerFor.TopVert.bUse;
	rData.bBottomTypeR = VerFor.BotVert.bUse;
	rData.nAppForceTypeR = 3;
	int nLcomSize = 1;
	rData.aWebForceL_Sigy.SetSize(nLcomSize);
	rData.aWebForceL_Sigy_B2T.SetSize(nLcomSize);
	rData.aWebForceR_Sigy.SetSize(nLcomSize);
	rData.aWebForceR_Sigy_B2T.SetSize(nLcomSize);
	
	//T_SDAP_VERT_D &Wfor = (VerFor.TopVert.bUse==TRUE) ? VerFor.TopVert : VerFor.BotVert;
		
	if (VerFor.TopVert.bUse)
	{
		ConvertSodWebVerticalForceBase(0, VerFor.TopVert, rData.aWebForceL_Sigy[0]);
	}
	if (VerFor.BotVert.bUse)
	{
		ConvertSodWebVerticalForceBase(0, VerFor.BotVert, rData.aWebForceL_Sigy_B2T[0]);
	}

	if (nStype==D_SECT_TYPE_STLG_B)
	{
		if (VerFor.TopVert.bUse)
		{
			ConvertSodWebVerticalForceBase(1, VerFor.TopVert, rData.aWebForceR_Sigy[0]);
		}
		if (VerFor.BotVert.bUse)
		{
			ConvertSodWebVerticalForceBase(1, VerFor.BotVert, rData.aWebForceR_Sigy_B2T[0]);
		}
	}

}

void CDgnSodDataCtrl::ConvertSodWebVerticalForceBase(int nWeb, T_SDAP_VERT_D &InD, STL_LFORCE_WEB_SIGY_D &rData)
{
	if (nWeb==0)
	{
	 rData.dF      = InD.dLqi;
	 rData.dP      = InD.dLpi;
	 rData.dLef    = InD.dLapi;
	}
	else
	{
		rData.dF      = InD.dRqi;
		rData.dP      = InD.dRpi;
		rData.dLef    = InD.dRapi;
	}
}

void CDgnSodDataCtrl::ConvertSodSectCheckPart(int nStype, int nPos, T_SDDP_D &InD, STL_CHK_POSD_SNiP_OD &rData)
{
	// BOOL CSectUtil::GetDeckNameList(T_SECT_D& SectD, CArray<CString, CString&>& aDeckNameList)
	int nSize = InD.aSddpSub[nPos].GetSize();
	for (int i=0; i<nSize; ++i)
	{
		T_SDDP_SUB_D &ChkPart = InD.aSddpSub[nPos].GetAt(i);

		/// [CIVIL-10243] deck name이 영어로 저장되어 있는데, 다국어 문자열과 비교하면서 잘못 판단되는 문제 수정.
		const CString& strDeckLang = CSectUtil::GetDeckNameEng2MultiLang(ChkPart.strDeck);
		if(nStype == D_SECT_TYPE_STLG_I)
		{
			if      (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_LEFT))   ==0) rData.bTopDeckPlateChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_RIGHT))  ==0) rData.bTopDeckPlateChk[2] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_DB_SECT_DECK_POS_WEB))    ==0) rData.bWebChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_LEFT))   ==0) rData.bBotDeckPlateChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_RIGHT))  ==0) rData.bBotDeckPlateChk[2] = TRUE;
			else ASSERT(0);
		}
		else if (nStype == D_SECT_TYPE_STLG_B)
		{
			if      (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_LEFT))     ==0) rData.bTopDeckPlateChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_CENTER))   ==0) rData.bTopDeckPlateChk[1] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_TOP_RIGHT))    ==0) rData.bTopDeckPlateChk[2] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_DB_SECT_DECK_POS_WEB_LEFT)) ==0) rData.bWebChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_DB_SECT_DECK_POS_WEB_RIGHT))==0) rData.bWebChk[1] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_LEFT))     ==0) rData.bBotDeckPlateChk[0] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_CENTER))   ==0) rData.bBotDeckPlateChk[1] = TRUE;
			else if (strDeckLang.CompareNoCase(_LS(IDS_CMD_SECT_SOD_BOT_RIGHT))    ==0) rData.bBotDeckPlateChk[2] = TRUE;
			else ASSERT(0);
		}

//     if(nStype == D_SECT_TYPE_STLG_I)
//     {
//       if      (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_TOP_LEFT))    ==0) rData.bTopDeckPlateChk[0] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_TOP_RIGHT))   ==0) rData.bTopDeckPlateChk[2] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_WEB))         ==0) rData.bWebChk[0] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_BOT_LEFT))    ==0) rData.bBotDeckPlateChk[0] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_BOT_RIGHT))   ==0) rData.bBotDeckPlateChk[2] = TRUE;
//       else ASSERT(0);
//     }
//     else if (nStype == D_SECT_TYPE_STLG_B)
//     {
//       if      (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_TOP_LEFT)   ) ==0) rData.bTopDeckPlateChk[0] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_TOP_CENTER) ) ==0) rData.bTopDeckPlateChk[1] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_TOP_RIGHT)  ) ==0) rData.bTopDeckPlateChk[2] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_WEB_LEFT)   ) ==0) rData.bWebChk[0] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_WEB_RIGHT)  ) ==0) rData.bWebChk[1] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_BOT_LEFT)   ) ==0) rData.bBotDeckPlateChk[0] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_BOT_CENTER) ) ==0) rData.bBotDeckPlateChk[1] = TRUE;
//       else if (ChkPart.strDeck.CompareNoCase(_LS(IDS_DB_SECTUTIL_DECK_BOT_RIGHT)  ) ==0) rData.bBotDeckPlateChk[2] = TRUE;
//       else ASSERT(0);
//     }
	}
}

void CDgnSodDataCtrl::ConvertSodCheckForce(int nPos, _DGN_FORC_CRC &For, STL_FORCE_D &rData)
{
	rData.dForce[0] = For.dFxx[nPos];
	rData.dForce[1] = For.dFyy[nPos];
	rData.dForce[2] = For.dFzz[nPos];
	rData.dForce[3] = For.dMux[nPos];
	rData.dForce[4] = For.dMuy[nPos];
	rData.dForce[5] = For.dMuz[nPos];

}

void CDgnSodDataCtrl::ConvertSodMemberForce(_DGN_FORC_MEMB &MFor,  STL_ULMF_D &rData)
{
	for (int i=0; i<cDGN_POSNUM; ++i)
	{
		rData.ForD[0].dForce[i] = MFor.ForMemb[0].dForce[i];
		rData.ForD[1].dForce[i] = MFor.ForMemb[2].dForce[i];
		rData.ForD[2].dForce[i] = MFor.ForMemb[4].dForce[i];
	}
}

void CDgnSodDataCtrl::ConvertSODDFORCE(int nPos, _DGN_FORC_CRC &For, T_SODD_FORCE &rData)
{
	rData.dForce[0] = For.dFxx[nPos];
	rData.dForce[1] = For.dFyy[nPos];
	rData.dForce[2] = For.dFzz[nPos];
	rData.dForce[3] = For.dMux[nPos];
	rData.dForce[4] = For.dMuy[nPos];
	rData.dForce[5] = For.dMuz[nPos];
}

void CDgnSodDataCtrl::ConvertSodCalcChkForce(STL_FORCE_D &ForOD, STLCHKFOR_ARRAY &aChkDLForce, STL_MEMB_POSD_SNiP_OD &rData)
{
	int nDataSize = rData.aChkFor.GetSize();
	if (nDataSize<=0) return;

	STL_LCOM_FORCE_OD &ChkForOD = rData.aChkFor[0];
	int nLcomS = 1;
	ChkForOD.GlobalForceD.SetSize(nLcomS);
	ChkForOD.GlobalForceD_DL.SetSize(nLcomS);

	STL_FORCE_D &ForceDL = aChkDLForce[0];  
	ChkForOD.GlobalForceD.SetAt(0, ForOD);
	ChkForOD.GlobalForceD_DL.SetAt(0, ForceDL);
}

void CDgnSodDataCtrl::ConvertSodCalcChkForce4Kapa(STLCHKFOR_ARRAY& aChkForce, STLCHKFOR_ARRAY& aChkDLForce, STL_MEMB_POSD_SNiP_OD &MembLcomD)
{
	for(int i=0; i<aChkForce.GetSize(); ++i)
	{
		MembLcomD.aChkFor[0].GlobalForceD.Add(aChkForce[i]); 
	}

	for(int i=0; i<aChkForce.GetSize(); ++i)  // GlobalForceD_DL은 GlobalForceD 개수에 맞춰서 저장한다.
	{
		MembLcomD.aChkFor[0].GlobalForceD_DL.Add(aChkDLForce[0]); 
	}


}

void CDgnSodDataCtrl::ConvertSod5PosD(const _DGN_FORC_CRC_EX& ForD, STL_POSD_DGND_SNiP_MEMB& rData)
{
	if(cPOSI_CRC_EX != cForcePosiNum){ASSERT(0);}
	for(int i = 0; i < cPOSI_CRC_EX; ++i)
	{
		rData.dForce[i][0] = ForD.dFxx[i];
		rData.dForce[i][1] = ForD.dFyy[i];
		rData.dForce[i][2] = ForD.dFzz[i];
		rData.dForce[i][3] = ForD.dMux[i];
		rData.dForce[i][4] = ForD.dMuy[i];
		rData.dForce[i][5] = ForD.dMuz[i];
	}
}

void CDgnSodDataCtrl::ConvertSodSdsnFlg(int nStype, BOOL bIsTop, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSN_FLG &rData,  BOOL &bSodCheck)
{
	int nPartSize = 0;
	if (nStype==D_SECT_TYPE_STLG_I)
	{
		nPartSize = 3;    
	}
	else if (nStype==D_SECT_TYPE_STLG_B)
	{
		nPartSize = 3;    
	}
	else ASSERT(0);

	for (int i=0; i<nPartSize; ++i) //L, M, R
	{
		STL_MEMB_RES_SNiP_STR_DP_CHK_UNIT &StrnPartR = (bIsTop==TRUE) ? LcomRes.OD_Res_SNiP_StrengthD.DeckPMembD_Top[i] : LcomRes.OD_Res_SNiP_StrengthD.DeckPMembD_Bot[i];
		ConvertStrnFlgPartEngToCVL(StrnPartR, rData.StrnChk[i]);

		if(rData.StrnChk[i].bChk) bSodCheck=TRUE;
	}

}

void CDgnSodDataCtrl::ConvertSodSdsnWeb(int nStype, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSN_WEB &rData, BOOL &bSodCheck)
{
	int nWebSize = 0;
	if (nStype==D_SECT_TYPE_STLG_I)
	{
		nWebSize = 1;    
	}
	else if (nStype==D_SECT_TYPE_STLG_B)
	{
		nWebSize = 2;    
	}
	else ASSERT(0);
	
	for (int i=0; i<nWebSize; ++i) //L, R
	{    
		ConvertStrnWebPartEngToCVL(LcomRes.OD_Res_SNiP_StrengthD.WebMembD[i], rData.WebD[i]);

		if(rData.WebD[i].bChk) bSodCheck=TRUE;
	}

}

void CDgnSodDataCtrl::ConvertSodSdsbFlg(int nStype, BOOL bIsTop, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSB_FLG &rData, BOOL &bSodCheck)
{
	int nPartSize = 3;
//   if (nStype==D_SECT_TYPE_STLG_I)
//   {
//     nPartSize = 3;    
//   }
//   else if (nStype==D_SECT_TYPE_STLG_B)
//   {
//     nPartSize = 3;    
//   }
//   else ASSERT(0);  

	for (int i=0; i<nPartSize; ++i) //L,M,R
	{
		STL_MEMB_RES_SNiP_STA_DP_CHK_UNIT &StabPartR = (bIsTop==TRUE) ? LcomRes.OD_Res_SNiP_StabilityD.DeckPMembD_Top[i] : LcomRes.OD_Res_SNiP_StabilityD.DeckPMembD_Bot[i];
		ConvertStabFlgPartEngToCVL(StabPartR, rData.StabChk[i]);

		if(rData.StabChk[i].bChk) bSodCheck=TRUE;
	}

}

void CDgnSodDataCtrl::ConvertSodSdsbWeb(int nStype, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSB_WEB &rData, BOOL &bSodCheck)
{
	int nWebSize = 0;
	if (nStype==D_SECT_TYPE_STLG_I)
	{
		nWebSize = 1;    
	}
	else if (nStype==D_SECT_TYPE_STLG_B)
	{
		nWebSize = 2;    
	}
	else ASSERT(0);
	
	for (int i=0; i<nWebSize; ++i) // L, R
	{    
		ConvertStabWebPartEngToCVL(LcomRes.OD_Res_SNiP_StabilityD.aResWebStaD[i], rData.WebD[i]);
		if(rData.WebD[i].bChk) bSodCheck=TRUE;
	}

}

void CDgnSodDataCtrl::ConvertSodSdsbFB(int nStype, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSB_GD &rData, BOOL &bSodCheck)
{
	const STL_MEMB_RES_SNiP_STA_FLEX_BUCK &StabFB = 	LcomRes.OD_Res_SNiP_StabilityD.GloblaSTA_FB;

	rData.bChk    = StabFB.bChk; 
	rData.dRat_GD = StabFB.dRatio;
	rData.bOK     = (StabFB.dRatio<=1.0)? TRUE : FALSE;  

	if(rData.bChk) bSodCheck = TRUE; 
}

void CDgnSodDataCtrl::ConvertSodSdsbFTB(int nStype, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSB_GD &rData, BOOL &bSodCheck)
{
	const STL_MEMB_RES_SNiP_STA_FLEX_TOR_BUCK &StabFTB = 	LcomRes.OD_Res_SNiP_StabilityD.GloblaSTA_FTB;

	rData.bChk    = StabFTB.bChk; 
	rData.dRat_GD = StabFTB.dRatio;
	rData.bOK     = (StabFTB.dRatio<=1.0)? TRUE : FALSE;  

	if(rData.bChk) bSodCheck = TRUE; 
}

void CDgnSodDataCtrl::ConvertSodSdsbGD(int nStype, STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_STR_STA_SNiP_OD &LcomRes, T_SDSB_GD &rData, BOOL &bSodCheck)
{
	STL_MEMB_RES_SNiP_STA_LT &StabGD = 	LcomRes.OD_Res_SNiP_StabilityD.GlobalSTA_LTB;

	rData.bChk    = StabGD.bChk; 
	rData.dRat_GD = StabGD.dRat;
	rData.bOK     = (StabGD.dRat<=1.0)? TRUE : FALSE;  

	if(rData.bChk) bSodCheck = TRUE; 
}

void CDgnSodDataCtrl::ConvertSodSdFati(STL_RES_BASE_SNiP_OD_SNiP &MembRes, STL_MEMB_RES_SNiP_FAT &LcomRes, T_SDFG_BASE &rData)
{
	STL_MEMB_RES_SNiP_FAT_POSD &FatD = LcomRes.ResCrtFatigueD;
	rData.bChk        = TRUE;
	rData.dSig_max_ef = FatD.dSig_max_ef;
	rData.dGamRym     = FatD.dCapa_Ax;
	rData.dSig_min    = FatD.dSigx_min;
	rData.dSig_max    = FatD.dSigx_max;
	rData.dRatio      = FatD.dRat_Ax;
	rData.bOK         = (rData.dRatio <=1.0)? TRUE : FALSE;

}

void CDgnSodDataCtrl::ConvertStrnFlgPartEngToCVL(STL_MEMB_RES_SNiP_STR_DP_CHK_UNIT &StrnPartR, T_SDPT_STRN_FLG &rData)
{
	int nRibSize  = StrnPartR.aResRibD.GetSize();
	int nDeckSize = StrnPartR.aDeckPlateD.GetSize();

	rData.aLSChk.SetSize(nRibSize);
	for (int i=0; i<nRibSize; ++i)
	{
		ConvertStrnLRibEngToCVL(StrnPartR.aResRibD[i], StrnPartR.aDeckPlateD[i], rData.aLSChk[i]);
	}
	//StrnPartR.nIndexCriticalRib

	// Deck
	double dRat_Deck = 0.0, dRat_Deck_Temp=0.0;
	BOOL bChk_Deck =FALSE;
	int nDeck_cr=0;
	for (int i=0; i<nDeckSize; ++i)
	{
		STL_MEMB_RES_SNiP_STR_DP &DeckPlateD =  StrnPartR.aDeckPlateD.GetAt(i);
		
		if(DeckPlateD.bChk) bChk_Deck= TRUE;
		dRat_Deck_Temp = DeckPlateD.dRat;
		//dRat_Deck      = max(dRat_Deck, dRat_Deck_Temp);
		if(i==0)
		{
			nDeck_cr = i;
			dRat_Deck  = dRat_Deck_Temp;    
		}
		else
		{
			if(dRat_Deck < dRat_Deck_Temp)
			{
				nDeck_cr = i;
				dRat_Deck  = dRat_Deck_Temp;    
			}
		}
	}
	rData.nDeck_cr  = nDeck_cr;
	rData.dRat_Deck = dRat_Deck;
	rData.bChk_Deck = bChk_Deck;

	// Rib
	double dRat_Rib = 0.0, dRat_Rib_Temp=0.0;
	BOOL bChk_Rib=FALSE;
	int nRib_cr=0;
	for (int i=0; i<nRibSize; ++i)
	{
		STL_MEMB_RES_SNiP_STR_RIB &RIB =  StrnPartR.aResRibD.GetAt(i);
		if(RIB.bChk) bChk_Rib= TRUE;
		dRat_Rib_Temp = (RIB.nGlobalLocal == 0) ? RIB.DeckPlate_Rib.dRat : RIB.dRat;   //max(RIB.dRat_Ry, RIB.dRat_Ryn);
		//dRat_Rib      = max(dRat_Rib, dRat_Rib_Temp);
		if(i==0)
		{
			nRib_cr = i;
			dRat_Rib  = dRat_Rib_Temp;    
		}
		else
		{
			if(dRat_Deck < dRat_Rib_Temp)
			{
				nRib_cr = i;
				dRat_Rib  = dRat_Rib_Temp;    
			}
		}
	}
	rData.nRib_cr  = nRib_cr;
	rData.dRat_Rib = dRat_Rib;
	rData.bChk_Rib = bChk_Rib;


	// FB
	double dRat_FB = 0.0, dRat_FB_Temp=0.0;
	BOOL bChk_FB = FALSE; 
	int nFB_cr=0;
	for (int i=0; i<3; ++i)
	{
		STL_MEMB_RES_SNiP_STR_FB &FloorBeamD =  StrnPartR.aFloorBeamD[i];
		if(FloorBeamD.bChk) bChk_FB= TRUE; 
		dRat_FB_Temp = FloorBeamD.dRat;
		//dRat_FB      = max(dRat_FB, dRat_FB_Temp);
		if(i==0)
		{
			nFB_cr = i;
			dRat_FB  = dRat_FB_Temp;    
		}
		else
		{
			if(dRat_FB < dRat_FB_Temp)
			{
				nFB_cr = i;
				dRat_FB  = dRat_FB_Temp;    
			}
		}
	}
	rData.nFB_cr    = nFB_cr;
	rData.dRat_FB   = dRat_FB;
	rData.bChk_FB   = bChk_FB;

	double dRat[3] = {dRat_Deck, dRat_Rib, dRat_FB};
	double dRat_Max=0.0;
	for(int i=0; i<3; ++i)
	{
		dRat_Max = max(dRat_Max, dRat[i]);
	}
	rData.dRat_Max  = dRat_Max;
	rData.bOK       = (dRat_Max <= 1.0)?  TRUE : FALSE;
	rData.bChk = (rData.bChk_Deck || rData.bChk_Rib || rData.bChk_FB)? TRUE : FALSE;

}

void CDgnSodDataCtrl::ConvertStrnLRibEngToCVL(STL_MEMB_RES_SNiP_STR_RIB &RibR, STL_MEMB_RES_SNiP_STR_DP &DeckR, T_SDLS_STRN_FLG &rData)
{
	rData.bChk       = TRUE;
	rData.nABPoint   = RibR.nABPoint;
	rData.dSigxc_r   = RibR.dSigxc;
	rData.dSigxp_r   = RibR.dSigxp;
	rData.dSigx_r    = RibR.dSigx;
	rData.dRyr       = RibR.dCapacity;
	rData.dRatioRib  = RibR.dRat_Ry;
	rData.bOKrib     = (rData.dRatioRib<=1.0)? TRUE : FALSE;      
	rData.nAB1point  = 0;  // ???
	rData.dSigx_d    = DeckR.dSigx;
	rData.dSigy_d    = DeckR.dSigy;
	rData.dTauxy_d   = DeckR.dTauxy;
	rData.dSigma_d   = DeckR.dSigx;
	rData.dRyd       = DeckR.dCapacity;
	rData.dRatioDeck = DeckR.dRat_Com;
	rData.bOKdeck    = (rData.bOKdeck<=1.0)? TRUE : FALSE; 
	rData.dRatioMax  = max(rData.dRatioRib, rData.dRatioDeck);
	rData.bOK        = (rData.dRatioMax<=1.0)? TRUE : FALSE;

}

void CDgnSodDataCtrl::ConvertStrnWebPartEngToCVL(STL_MEMB_RES_SNiP_STR_WEB_CHK_UNIT &StrnWebR, T_SDPT_STRN_WEB &rData)
{
	rData.bChk = StrnWebR.bChk;

	// 고려안됨.
//   rData.dTau_min[0] = StrnWebR.WebNoMD.KappaF.dtau_min_ef;      // 0=Top, 1=Bottom
//   rData.dTau_min[1] = StrnWebR.WebNoMD.KappaF.dtau_min_ef;         // Centroid
//   rData.dTau_max    = StrnWebR.WebNoMD.KappaF.dtau_max_ef;
//   rData.dTau_M0     = StrnWebR.WebNoMD.KappaF.dtaum;          // 4.30절 식(159)
//   rData.dRatioTau   = 0.0;  // ???
//   rData.bOK_tauM0   = 0.0;  // ???
//   rData.dRyr        = 0.0;  // Gamma*Ry*m, 4.31절 식(161)
//   rData.dRsr        = 0.0;  // Rs*m 

	// Rib
	int nRibSize = StrnWebR.aRibD.GetSize();
	rData.aWebChk.SetSize(nRibSize);
	double dRat[3]={0.0};
	double dRatTemp=0.0;
	BOOL bChk_Rib=FALSE;
	int nRib_cr =0;
	for (int i=0; i<nRibSize; ++i)
	{
		ConvertStrnWebPlateEngToCVL(StrnWebR.aRibD[i], rData.aWebChk[i]);
		dRatTemp = StrnWebR.aRibD[i].dRat;
		//dRat[0] = max(dRat[0], StrnWebR.aRibD[i].dRat);
		if(StrnWebR.aRibD[i].bChk) bChk_Rib = TRUE;

		if(i==0)
		{
			nRib_cr = i;
			dRat[0]  = dRatTemp;    
		}
		else
		{
			if(dRat[0]  < dRatTemp)
			{
				nRib_cr = i;
				dRat[0] = dRatTemp;    
			}
		}

	}
	rData.nRib_cr  = nRib_cr;
	rData.bChk_Rib = bChk_Rib;
	rData.dRat_Rib = dRat[0];
	

	//Deck - Shear/Von Mises
	dRat[1] = StrnWebR.WebNoMD.dRat;  // 4.30
	dRat[2] = StrnWebR.WebVonD.dRat;  // 4.31

	rData.bChk_DeckNoM = (dRat[1]>0.0)? TRUE : FALSE;
	rData.bChk_DeckVon = (dRat[2]>0.0)? TRUE : FALSE;
	rData.bChk_Deck = (dRat[1]>0.0 || dRat[2]>0.0)? TRUE : FALSE; 
	rData.dRat_DeckNoM = dRat[1];
	rData.dRat_DeckVon = dRat[2];
	rData.dRat_Deck    = max(dRat[1], dRat[2]);

	double dRat_max =0.0;
	for(int i=0; i<3; ++i)
	{
		dRat_max = max(dRat_max, dRat[i]);
	}
	rData.bChk = (rData.bChk_Deck || rData.bChk_Rib) ? TRUE : FALSE;
	rData.dRat_Max = dRat_max;
	rData.bOK = (rData.dRat_Max<=1.0)? TRUE : FALSE;
}

void CDgnSodDataCtrl::ConvertStrnWebPlateEngToCVL(STL_MEMB_RES_SNiP_STR_WEB_RIB &WebP, T_SDPS_STRN_WEB &rData)
{
	rData.bChk    = TRUE;
	rData.dSigx   = WebP.dSigx;
	rData.dSigy   = WebP.dSigy;
	rData.dTauxy  = WebP.dTauxy;
	rData.dSigsum = WebP.dDemand;
	
	rData.dRatioSig  = WebP.dRat_Com;
	rData.dRatioTau  = WebP.dRat_Sh;
	rData.dRatioMax  = WebP.dRat;
	rData.bOK_Sigsum = (rData.dRatioSig <=1.0)? TRUE : FALSE;
	rData.bOK_Tauxy  = (rData.dRatioTau <=1.0)? TRUE : FALSE;
	rData.bOK        = (rData.dRatioMax <=1.0)? TRUE : FALSE;

}

void CDgnSodDataCtrl::ConvertStabFlgPartEngToCVL(STL_MEMB_RES_SNiP_STA_DP_CHK_UNIT &StabPartR, T_SDPT_STAB_FLG &rData)
{
	// Rib
	int nRibSize  = StabPartR.aLRib_Sta.GetSize();
	rData.aLSChk.SetSize(nRibSize);
	double dRat_Rib=0.0;
	BOOL bChk_Rib =FALSE;
	int nRib_cr=0;
	int nRibPart_cr = 0;
	for (int i=0; i<nRibSize; ++i)
	{
		double dRat_Rib_Temp=0.0;
		int nRibPart_cr_temp = 0;
		ConvertStabLRibEngToCVL(StabPartR.aLRib_Sta[i], rData.aLSChk[i], nRibPart_cr_temp, dRat_Rib_Temp, bChk_Rib);
		if(i==0)
		{
			nRib_cr = i;
			dRat_Rib  = dRat_Rib_Temp;    
			nRibPart_cr = nRibPart_cr_temp;
		}
		else
		{
			if(dRat_Rib < dRat_Rib_Temp)
			{
				nRib_cr = i;
				dRat_Rib  = dRat_Rib_Temp;   
				nRibPart_cr = nRibPart_cr_temp;
			}
		}
	}
	rData.nRib_cr = nRib_cr;
	rData.nRibPan_cr = nRibPart_cr; //StabPartR.nIndexCriticalDP;

 	// Deck - Global stability
	STL_MEMB_RES_SNiP_STA_GDP &GDeckPlate_Sta = StabPartR.GDeckPlate_Sta;
 	BOOL bChk_DeckG = GDeckPlate_Sta.bChk;
	double dRat_DeckG = GDeckPlate_Sta.dRat;
	rData.nDeckG_cr = 0;	


	// Deck - Local stability(BTR)
	int nDeckSize = StabPartR.aLDeckPlate_Sta.GetSize();
	rData.aPanChk.SetSize(nDeckSize);

	BOOL bChk_DeckL = FALSE;
	double dRat_DeckL  = 0.0;
	int nDeckL_cr=0;
	for (int i=0; i<nDeckSize; ++i)
	{
		STL_MEMB_RES_SNiP_STA_LOCAL_BTR &LDeckPlate_Sta = StabPartR.aLDeckPlate_Sta[i];
		ConvertStabDeckEngToCVL(LDeckPlate_Sta, rData.aPanChk[i]);		
		if(LDeckPlate_Sta.bChk) bChk_DeckL= TRUE;

		//dRat_DeckL = max(dRat_DeckL, LDeckPlate_Sta.dRat);
		if(i==0)
		{
			nDeckL_cr  = i;
			dRat_DeckL = LDeckPlate_Sta.dRat;    
		}
		else
		{
			if(dRat_DeckL < LDeckPlate_Sta.dRat)
			{
				nDeckL_cr  = i;
				dRat_DeckL = LDeckPlate_Sta.dRat;    
			}
		}
	} 
	rData.nDeckL_cr = nDeckL_cr;	
	 
	// Floor Beam - Global stability
	BOOL bChk_FBG = FALSE;
	double dRat_FBG =0.0;
	int nFBG_cr=0;
	for(int i=0; i<3; ++i) // C1, C2, C3 -> nIndexCriticalFB
	{
		STL_MEMB_RES_SNiP_STA_FB &FloorBeam_Sta = StabPartR.aFloorBeam_Sta[i]; 

		if(FloorBeam_Sta.bChk) bChk_FBG= TRUE;  

		//dRat_FBG = max(dRat_FBG, FloorBeam_Sta.dRat);
		if(i==0)
		{
			nFBG_cr= i;
			dRat_FBG = FloorBeam_Sta.dRat;
		}
		else
		{ 
			if(dRat_FBG < FloorBeam_Sta.dRat)
			{
				nFBG_cr = i;
				dRat_FBG = FloorBeam_Sta.dRat;			 
			}
		}
	}
	rData.nFBG_cr = nFBG_cr; //StabPartR.nIndexCriticalFB;

	// Floor Beam - Local stability(BTR)
	BOOL bChk_FBL = FALSE;
	double dRat_FBL =0.0;
	double dRat_FBLAll =0.0;
	double dRat_FBLAll_temp =0.0;
	int nFBSize  = StabPartR.aLFB_Sta.GetSize();
	int nFBL_cr=0;
	int nFBLPan_cr=0;  // nFBL_cr 중 불리한 plate
	for (int i=0; i < nFBSize; ++i)
	{
		if(!StabPartR.aLFB_Sta[i].bChk){continue;}
		dRat_FBLAll_temp =0.0;

		int nFBLPan_cr_temp=0;
		int nFBPSize = StabPartR.aLFB_Sta[i].aRibPlateSTA.GetSize();    
		for(int j = 0; j < nFBPSize; ++j)
		{
			if(!StabPartR.aLFB_Sta[i].aRibPlateSTA[j].bChk){continue;}
			dRat_FBL = max(dRat_FBL, StabPartR.aLFB_Sta[i].aRibPlateSTA[j].dRat);
			bChk_FBL= TRUE;  

			//dRat_FBLAll_temp = max(dRat_FBLAll_temp, StabPartR.aLFB_Sta[i].aRibPlateSTA[j].dRat);
			if(j==0)
			{
				nFBLPan_cr_temp =j;
				dRat_FBLAll_temp = StabPartR.aLFB_Sta[i].aRibPlateSTA[j].dRat;
			}
			else
			{
				if(dRat_FBLAll_temp < StabPartR.aLFB_Sta[i].aRibPlateSTA[j].dRat)
				{
					nFBLPan_cr_temp =j;
					dRat_FBLAll_temp = StabPartR.aLFB_Sta[i].aRibPlateSTA[j].dRat;
				}
			}
		}

		if(i==0)
		{
			nFBL_cr=i;
			dRat_FBLAll = dRat_FBLAll_temp;
			nFBLPan_cr  = nFBLPan_cr_temp;
		}
		else
		{
			if(dRat_FBLAll < dRat_FBLAll_temp)
			{
				nFBL_cr=i;
				dRat_FBLAll = dRat_FBLAll_temp;
				nFBLPan_cr  = nFBLPan_cr_temp;
			}
		}
	}
	rData.nFBL_cr = nFBL_cr; //StabPartR.nIndexCriticalFB_LSta;
	rData.nFBLPan_cr =nFBLPan_cr;

	// *************************************
	// 개선
	// *************************************
	rData.dRat_Rib = dRat_Rib;
	rData.bChk_Rib = bChk_Rib;

	rData.dRat_DeckG = dRat_DeckG;
	rData.dRat_DeckL = dRat_DeckL;
	rData.dRat_Deck  = max(dRat_DeckG, dRat_DeckL);  
	rData.bChk_DeckG = bChk_DeckG;
	rData.bChk_DeckL = bChk_DeckL;
	rData.bChk_Deck  = (!bChk_DeckG && !bChk_DeckL)? FALSE : TRUE;

	rData.dRat_FBG = dRat_FBG; 
	rData.dRat_FBL = dRat_FBL;    
	rData.dRat_FB  = max(dRat_FBG, dRat_FBL);  
	rData.bChk_FBG = bChk_FBG;
	rData.bChk_FBL = bChk_FBL;
	rData.bChk_FB  = (!bChk_FBG && !bChk_FBL)? FALSE : TRUE;

	// max
	double dRat[3]={0.0};
	dRat[0] = rData.dRat_Rib;
	dRat[1] = rData.dRat_Deck;
	dRat[2] = rData.dRat_FB;

	double dRat_max =0.0;
	for(int i=0; i<3; ++i)
	{
		dRat_max = max(dRat_max, dRat[i]);
	}

	rData.bChk = (rData.bChk_Deck || rData.bChk_Rib || rData.bChk_FB) ? TRUE : FALSE;
	rData.dRat_Max = dRat_max;
	rData.bOK      = (dRat_max<=1.0) ? TRUE : FALSE;
}

void CDgnSodDataCtrl::ConvertStabLRibEngToCVL(STL_MEMB_RES_SNiP_STA_RIB_CHK_UNIT &RibR, T_SDSB_SECT_NOLS &rData, int &nRibPart_cr, double &dRat_Rib, BOOL &bChk_Rib)
{
	int nRibPartSize = RibR.aRibPlateSTA.GetSize();
	double dRat_Rib_max=0.0;
	nRibPart_cr = 0;
	for (int i=0; i<nRibPartSize; ++i)
	{
		if (i>7) continue;
		T_SDSB_PAN &SdsbPan = rData.PanChk[i];
		SdsbPan.dBTRa  = RibR.aRibPlateSTA[i].dBT_Rat_lim;
		SdsbPan.dRatio = RibR.aRibPlateSTA[i].dRat;
		SdsbPan.bChkBtr = (SdsbPan.dRatio <=1.0)? TRUE : FALSE;

		if(RibR.aRibPlateSTA[i].bChk) bChk_Rib = TRUE; 
		//dRat_Rib_max = max(dRat_Rib_max, SdsbPan.dRatio);
		if(i==0)
		{
			nRibPart_cr = i;
			dRat_Rib_max = SdsbPan.dRatio;
		}
		else
		{
			if(dRat_Rib_max < SdsbPan.dRatio)
			{
				nRibPart_cr = i;
				dRat_Rib_max = SdsbPan.dRatio;
			}
		}
		
	}  
	dRat_Rib = dRat_Rib_max;

}

void CDgnSodDataCtrl::ConvertStabDeckEngToCVL(STL_MEMB_RES_SNiP_STA_LOCAL_BTR &DeckR, T_SDSB_PAN &rData)
{
	rData.dBTRa   = DeckR.dBT_Rat_lim;
	rData.dRatio  = DeckR.dRat;
	rData.bChkBtr = (rData.dRatio  <=1.0)? TRUE : FALSE;
	
}

void CDgnSodDataCtrl::ConvertStabWebPartEngToCVL(STL_MEMB_RES_SNiP_STA_WEB &StabWebR, T_SDPT_STAB_WEB &rData)
{
	// Web_plate
	int nPanSize = StabWebR.aWebPlateD.GetSize();
	rData.aWebChk.SetSize(nPanSize);
	double dRat_Deck = 0.0;
	BOOL bChk_Deck = FALSE;
	for (int i=0; i<nPanSize; ++i)
	{
		STL_MEMB_RES_SNiP_STA_WEB_PLATE &WebPlateD = StabWebR.aWebPlateD[i];
		ConvertStabWebPanEngToCVL(WebPlateD, rData.aWebChk[i]);

		dRat_Deck = max(dRat_Deck, WebPlateD.dRat);
		if(WebPlateD.bChk) bChk_Deck = TRUE;
	}
	rData.nPan_cr = 0; // ???


	// Web_Rib
	double dRat_Rib = 0.0; 
	double dRat_RibAll = 0.0; 
	BOOL bChk_Rib = FALSE;
	int nRib_cr = 0;
	int nRibSize = StabWebR.aLRib_Sta.GetSize();
	int nRibPan_cr= 0;
	for(int i=0; i<nRibSize; ++i) // 리브 개수
	{
		STL_MEMB_RES_SNiP_STA_RIB_CHK_UNIT &LRib_Sta =  StabWebR.aLRib_Sta.GetAt(i);

		int nSize = LRib_Sta.aRibPlateSTA.GetSize();
		int nRibPan_cr_temp=0;
		for(int j=0; j<nSize; ++j) // 리브에 포함된 plate 수
		{
			STL_MEMB_RES_SNiP_STA_LOCAL_BTR &RibPlateSTA =  LRib_Sta.aRibPlateSTA.GetAt(j);

			//dRat_Rib = max(dRat_Rib, RibPlateSTA.dRat); 
			if(RibPlateSTA.bChk) bChk_Rib=TRUE;
			if(j==0)
			{
				nRibPan_cr_temp = j;
				dRat_Rib = RibPlateSTA.dRat;
			}
			else
			{
				if(dRat_Rib < RibPlateSTA.dRat)
				{
					nRibPan_cr_temp = j;
					dRat_Rib = RibPlateSTA.dRat;
				}
			}
		}

		if(i==0)
		{
			nRib_cr = i;
			dRat_RibAll  = dRat_Rib;    
			nRibPan_cr   = nRibPan_cr_temp;
		}
		else
		{
			if(dRat_RibAll < dRat_Rib)
			{
				nRib_cr = i;
				dRat_RibAll  = dRat_Rib;    
				nRibPan_cr   = nRibPan_cr_temp;
			}
		}
	}

	// *************************************
	// 개선
	// *************************************
	rData.nRib_cr  = nRib_cr;
	rData.nRibPan_cr = nRibPan_cr;
	rData.dRat_Deck = dRat_Deck;
	rData.bChk_Deck = bChk_Deck; 

	rData.dRat_Rib = dRat_RibAll;
	rData.bChk_Rib = bChk_Rib;
	
	rData.bChk = (rData.bChk_Deck || rData.bChk_Rib) ? TRUE : FALSE;
	
	rData.dRat_Max = max(dRat_Deck, dRat_Rib);
	rData.bOK = (rData.dRat_Max <=1.0)? TRUE : FALSE;
}

void CDgnSodDataCtrl::ConvertStabWebPanEngToCVL(STL_MEMB_RES_SNiP_STA_WEB_PLATE &WebP, T_SDPS_STAB_WEB_PAN &rData)
{
	rData.bChk          = WebP.bChk;
	rData.nAppItemType  = WebP.nIndex;
	rData.nChkPlateType = WebP.nChkPlateType;
	
	rData.dSigx     = WebP.dSigx;
	rData.dSigy     = WebP.dSigy;
	rData.dTauxy    = WebP.dTauxy;
	rData.dSigx_cr  = WebP.dSigx_cr;
	rData.dSigy_cr  = WebP.dSigy_cr;
	rData.dTauxy_cr = WebP.dTauxy_cr;
	rData.dComRat   = WebP.dRat;
	rData.bOK       = (rData.dComRat <=1.0)? TRUE : FALSE;    
}