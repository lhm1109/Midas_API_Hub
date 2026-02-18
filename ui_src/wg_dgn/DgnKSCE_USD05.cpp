// DgnKSCE_USD05.cpp: implementation of the CDgnKSCE_USD05 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnKSCE_USD05.h"

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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define DEGTORAD (3.14159265358979323846/180.0)

CDgnKSCE_USD05::CDgnKSCE_USD05()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	m_dZero=1.0E-07;
	m_PSC_iITER = 100;//30;
}

CDgnKSCE_USD05::~CDgnKSCE_USD05()
{
}

///////////////////////////////////////////
// For Connection to CRCDataCtrl.

void CDgnKSCE_USD05::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
	CDgnPscCommon::SetDataCtrlPointer(pDataCtrl);
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	// Set Code Unit.
	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
	m_pDataCtrl->Set_UnitIndex(CodeUnit);     
	//m_pDataCtrl->Get_CurrUnitIndex(CodeUnit);	
	//m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

	if(m_iCivilCode==KSCE_USD05 || m_iCivilCode==KSCE_RAIL_USD04)
	{
		m_Print.SetDataCtrlPointer(m_pDataCtrl,m_pForcCtrl);
	}
	else if(m_iCivilCode==KSCE_USD10 || m_iCivilCode==KSCE_RAIL_USD11)
	{
		m_Print.SetDataCtrlPointer(m_pDataCtrl,m_pForcCtrl);
		
	}
	else ASSERT(0);
				
	Make_PscDgnData();
}

CPrtKSCE_USD05* CDgnKSCE_USD05::Get_PrtKscePointer()	{return &m_Print;}
CPrtKSCE_USD10* CDgnKSCE_USD05::Get_PrtKscePointer10()	
{/*return &m_Print_ksce10;*/
//  m_Print_ksce10 = (CPrtKSCE_USD10 *)m_pDataCtrl->Get_PrtKsce10Pointer();

	return &m_Print_ksce10; 
}


BOOL CDgnKSCE_USD05::Get_KsceFysg(ElemPairK ElemK, T_FYSG_D& FysgD)
{
	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(FALSE, ElemK, SectElem))
	{
		return FALSE;
	}
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(FALSE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK=FALSE;

	int i=0, j=0;
	BOOL bPreTndn[2]={TRUE,TRUE};
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);

		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;      
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
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

		for(j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			if(FysgD.FysgBase[i*2+j].bCHK==FALSE)  continue;
			double dalComf, dalTenf;
			if(!Get_BeforeAllowbleStressofConc(bPreTndn[i], MatlElem.MatdD, SpscSect, arRbarPosi, arTndnPosi, FysgD.FysgBase[i*2+j] , dalComf, dalTenf)) continue;

			double dExtraRatio  = 1.0;
/*
			double dAllowStress = 0.0;     
			if(FysgD.FysgBase[i*2+j].dFMAX>0.0) dAllowStress = dExtraRatio*dalComf;
			else                                dAllowStress = dExtraRatio*dalTenf;      
			// Set Data. CodeUnit -> CurrUnit.
			FysgD.FysgBase[i*2+j].dALW = -1.0*dAllowStress;
*/
			// Set Data. CodeUnit -> CurrUnit.
			double dAlwStr = (-1.0)*dExtraRatio*(j==0 ? dalComf : dalTenf);
			FysgD.FysgBase[i*2+j].dALW = dAlwStr;
			FysgD.FysgBase[i*2+j].dRatio = (dAlwStr==0.0)? 1.0/m_dZero: FysgD.FysgBase[i*2+j].dFMAX/dAlwStr;
		}
	}   
	return bCalcOK;
}

// 2. 사용하중시 PC강재 최대응력도 (텐던별)
BOOL CDgnKSCE_USD05::Get_KsceFypc(T_TDNA_K TdnaK, T_FYPC_KSCE& FypcKsce)
{
	Get_AllowbleStressofPC(TdnaK, FypcKsce);
	return TRUE;
}	

//  3. 합성응력(FY_COM)
BOOL CDgnKSCE_USD05::Get_KsceFycm(ElemPairK ElemK, T_FYCM_D& FycmD)
{
	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(FALSE, ElemK, SectElem))
	{
		return FALSE;
	}
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(FALSE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK=FALSE;
	
	int i=0, j=0;
	BOOL bPreTndn[2]={TRUE,TRUE};
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;      
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
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

		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			if(FycmD.FycmBase[i*2+j].bCHK==FALSE)  continue;
			double dalComf, dalTenf;
			if(!Get_AfterAllowbleStressofConc(bPreTndn[i], MatlElem.MatdD, SpscSect, arRbarPosi, arTndnPosi, FycmD.FycmBase[i*2+j], dalComf, dalTenf)) continue;
			double dExtraRatio  = m_pForcCtrl->Get_ExtraRatio(FycmD.FycmBase[i*2+j].LcomK);
/*
			double dAllowStress = 0.0;     
			if(FycmD.FycmBase[i*2+j].dFMAX>0.0) dAllowStress = dExtraRatio*dalComf;
			else                                dAllowStress = dExtraRatio*dalTenf;            
			// Set Data. CodeUnit -> CurrUnit.
			FycmD.FycmBase[i*2+j].dALW =(-1.0*dAllowStress);
*/
			// Set Data. CodeUnit -> CurrUnit.
			FycmD.FycmBase[i*2+j].dALW = (-1)*dExtraRatio*(j==0 ? dalComf : dalTenf);
			//FycmD.FycmBase[i*2+j].dALWLC = (-1)*(j==0 ? dalComf : dalTenf); // 계산서에서 사용
		}
	}
	return bCalcOK;
}

//  4. 휨 균열폭 (BND_CW)
BOOL CDgnKSCE_USD05::Get_KsceBdcw(ElemPairK ElemK, T_BDCW_D& BdcwD)
{
	BOOL bCalcOK=TRUE;
	
	// Get Crack Width.
	T_PSCD_D PscdD; PscdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetPscd(PscdD);

	int i,j,k;
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
	
	T_MATD_D MatdD = MatlElem.MatdD;
	
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);    
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
		arOuter.Copy(SectElem.SectPosi[i].arOuter);
		CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
		int iInnerCellNum = SectElem.SectPosi[i].iInnerCellNum;
		for(int m=0; m<iInnerCellNum; m++)
		{
			CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
			arInner.Copy(SectElem.SectPosi[i].arInner[m]);
			arpInner.Add(&arInner);
		}
			
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);

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

		for(j=0 ; j<2 ; j++) // 0=Top, 1=Bot
		{
			_DGN_FORC_CRC ForcLcomNTP;
			// FALSE : Except Tendon Primary
			if(!m_pDataCtrl->Get_ConForc(ElemK, FALSE, BdcwD.BdcwBase[i*2+j].LcomK, ForcLcomNTP)) continue;
			double dMu_tp = ForcLcomNTP.dMuy[i];
			_DGN_FORC_CRC ForcLcom;
			if(!m_pDataCtrl->Get_ConForc(ElemK, TRUE, BdcwD.BdcwBase[i*2+j].LcomK, ForcLcom)) continue;
			BOOL   bTopCalc = j==0 ? TRUE : FALSE;
			double dMu = ForcLcom.dMuy[i];
			double dftop = -1.0*(BdcwD.BdcwBase[i*2+j].dFT);
	    double dfbot = -1.0*(BdcwD.BdcwBase[i*2+j].dFB);
			double dfs=0.0, dw=0.0, dwa=0.0;
			if(!Get_CrackCheck(bTopCalc, PscdD.iCrackWidth[j], MatdD, dbw, dShrThk, dpMomPos, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dMu, dftop, dfbot, dMu_tp, dfs, dw, dwa)) continue;
			BdcwD.BdcwBase[i*2+j].dFRT = (dfs);
			BdcwD.BdcwBase[i*2+j].dWC  = dw; 
			BdcwD.BdcwBase[i*2+j].dAWC = dwa; 
		}
		// Delete because of New Operator.
		for(k=0; k<arpInner.GetSize(); k++)
		{
			if(arpInner[k])
			{
				arpInner[k]->RemoveAll();
				delete arpInner[k];
				arpInner[k] = NULL; 
			}
		}
		arpInner.RemoveAll();    
	}
	
	return bCalcOK;
}

//  5. 인장철근 (RBAR_T)
BOOL CDgnKSCE_USD05::Get_KsceTbar(ElemPairK ElemK, T_TBAR_D& TbarD, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/, int iUseKind/*0*/)
{
	// iUseKind (0=Not Use Result, 1=Top, 2=Bot).
	BOOL bCalcOK=TRUE;	

	int iLcomNum = m_pDataCtrl->Get_DgnLcomNum();
	ADGNFORCE aForceLcom;
	ADGNSTRSADD aStressAdd;
	//!/ Tendon 포함이 맞는지 확인
	int nForType=0;
	if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, TRUE, FALSE, aForceLcom, aStressAdd, iLcomNum))	return FALSE;
	ASSERT(aForceLcom.GetSize()==aStressAdd.GetSize());
	if(m_pForcCtrl->Get_LcomNumForStrs()==0)	return FALSE;

	ADGNSTRESS aStressLcom;
	ADGNSTRESS aStressTnpr;
	if(!m_pForcCtrl->Get_LcomDataForStress(ElemK, nForType, TRUE, FALSE, aStressLcom, aStressTnpr, iLcomNum)) return FALSE;

	// Get Tbar.
	if(iUseKind > 0)
	{
		TbarD.Initialize();
		if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetTbarData(ElemK,TbarD))	return FALSE;
	}

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
	
	T_MATD_D MatdD = MatlElem.MatdD;

	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=iSta; i<iEnd; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		BOOL bPrint = m_Print.Get_PrintFlag();

		// Section
		T_SPSC_SECT SpscSect; SpscSect = SectElem.SectPosi[i].SpscSect;
		CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
		arOuter.Copy(SectElem.SectPosi[i].arOuter);
		CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
		int iInnerCellNum = SectElem.SectPosi[i].iInnerCellNum;
		for(int m=0; m<iInnerCellNum; m++)
		{
			CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
			arInner.Copy(SectElem.SectPosi[i].arInner[m]);
			arpInner.Add(&arInner);
		}
		// Section Gross
		//T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];

		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		
		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[i].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[i].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[i].dpShrThk[2];
		double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);

		if(iUseKind > 0)	// Use Result.
		{
			// Forc.
			for(int j=0; j<2; j++) // Top, Bot.
			{
				BOOL bTopCalc = (j==0 ? TRUE : FALSE);
				int k = i*2+j;
				if(bPrint)  m_Print.Print_TbarElementIJ(ElemK.first, bPosiI, bTopCalc);      
				if(TbarD.TbarBase[k].bCHK==FALSE) continue;
				int iDgnLcomNo = TbarD.TbarBase[k].LcomK;

				if(iDgnLcomNo==0) continue;
			  int m = iDgnLcomNo-1;
			  _DGN_STRS_CRC StressCrc = aStressLcom.GetAt(m);
			  //_DGN_STRS_ADD StrsAdd = aStressAdd.GetAt(j);
			  _DGN_STRS_CRC StrsMax, StrsMin;
			  StrsMax.Initialize();
			  StrsMin.Initialize();
			  if(!m_pForcCtrl->Get_StrsByStrs(ElemK, iDgnLcomNo, StressCrc, StrsMax, StrsMin))	continue;
			  double dftop = -StrsMin.dStop[i];
			  double dfbot = -StrsMin.dSbot[i];
			  double dAreq, dAuse, dAmin;
			  if(!Get_TensionRebar(bTopCalc, bPrint, MatdD, dbw, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dftop, dfbot, dAreq, dAuse, dAmin)) continue;
			}
		}
		else	// Not Use Result.
		{
			UINT TopMaxLcomK=0, BotMaxLcomK=0;
			double dAreqTopMax=0.0, dAminTopMax=0.0, dAuseTopMax=0.0;
			double dAreqBotMax=0.0, dAminBotMax=0.0, dAuseBotMax=0.0;

			// Get Tbar.
			_DGN_STRS_CRC StrsMax, StrsMin;
			for(int j=0; j<2; j++) // Top, Bot.
			{
				BOOL bTopCalc = (j==0 ? TRUE : FALSE);
				int k = i*2+j;
				if(TbarD.TbarBase[k].bCHK==FALSE) continue;
				if(bPrint)  m_Print.Print_TbarElementIJ(ElemK.first, bPosiI, bTopCalc);      
				int iDgnLcomNo = TbarD.TbarBase[k].LcomK;
				int m = iDgnLcomNo-1;

				_DGN_STRS_CRC StressCrc = aStressLcom.GetAt(m);				
				StrsMax.Initialize();
				StrsMin.Initialize();
				if(!m_pForcCtrl->Get_StrsByStrs(ElemK, iDgnLcomNo, StressCrc, StrsMax, StrsMin))	continue;
				double dftop = -StrsMin.dStop[i];
				double dfbot = -StrsMin.dSbot[i];
				double dAreq=0.0, dAuse=0.0, dAmin=0.0;
				if(j==0)
				{ 
					if(dftop>0.0) 
					{
						if(!Get_TensionRebar(bTopCalc, bPrint, MatdD, dbw, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dftop, dfbot, dAreq, dAuse, dAmin)) continue;
						else {TopMaxLcomK=iDgnLcomNo; dAreqTopMax=dAreq; dAminTopMax=dAmin; dAuseTopMax=dAuse;}
					}
					else TbarD.TbarBase[k].LcomK = 0;
				}
				else if(j==1)
				{
					if(dfbot>0.0)
					{
						if(!Get_TensionRebar(bTopCalc, bPrint, MatdD, dbw, SpscSect, arOuter, arpInner, arRbarPosi, arTndnPosi, dftop, dfbot, dAreq, dAuse, dAmin)) continue;
						else {BotMaxLcomK=iDgnLcomNo; dAreqBotMax=dAreq; dAminBotMax=dAmin; dAuseBotMax=dAuse;}
					}
					else TbarD.TbarBase[k].LcomK = 0;
				}        
			}
			// 0=I-TOP-MAX, 1=I-BOT-MAX, 2=J-TOP-MAX, 3=J-BOT-MAX.
			if(TopMaxLcomK > 0)
			{
				CString strOrgLcomNaTopMax=_T("");
				int iMaxMinTypeTopMax=0;
				if(!m_pForcCtrl->Get_LcomType(TopMaxLcomK, strOrgLcomNaTopMax, iMaxMinTypeTopMax))	ASSERT(0);
				TbarD.TbarBase[i*2+0].LcomK	= TopMaxLcomK;
				TbarD.TbarBase[i*2+0].iKind	= m_pForcCtrl->Get_LcomKind(TopMaxLcomK);
				TbarD.TbarBase[i*2+0].iMax	= iMaxMinTypeTopMax;
				TbarD.TbarBase[i*2+0].dREQ	= dAreqTopMax;
				TbarD.TbarBase[i*2+0].dMIN	= dAminTopMax;
				TbarD.TbarBase[i*2+0].dUSE	= dAuseTopMax;
			}
			if(BotMaxLcomK > 0)
			{
				CString strOrgLcomNaBotMax=_T("");
				int iMaxMinTypeBotMax=0;
				if(!m_pForcCtrl->Get_LcomType(BotMaxLcomK, strOrgLcomNaBotMax, iMaxMinTypeBotMax))	ASSERT(0);
				TbarD.TbarBase[i*2+1].LcomK	= BotMaxLcomK;
				TbarD.TbarBase[i*2+1].iKind	= m_pForcCtrl->Get_LcomKind(BotMaxLcomK);
				TbarD.TbarBase[i*2+1].iMax	= iMaxMinTypeBotMax;
				TbarD.TbarBase[i*2+1].dREQ	= dAreqBotMax;
				TbarD.TbarBase[i*2+1].dMIN	= dAminBotMax;
				TbarD.TbarBase[i*2+1].dUSE	= dAuseBotMax;
			}
		}

		// Delete because of New Operator.
		for(int k=0; k<arpInner.GetSize(); k++)
		{
			if(arpInner[k])
			{
				arpInner[k]->RemoveAll();
				delete arpInner[k];
				arpInner[k] = NULL; 
			}
		}
		arpInner.RemoveAll();  

	}
	return bCalcOK;
}

// 가설중 경사 인장 응력도 (FST_SG)
BOOL CDgnKSCE_USD05::Get_KsceFssg(ElemPairK ElemK, T_FPMT_JTG& FpmtKsce)
{	
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	T_MATD_D MatdD = MatlElem.MatdD;
	double dAFS=0., dAFST=0.;      
	Get_AllowblePrincipalStressOfConc(MatdD, dAFS, dAFST);
	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			if(FpmtKsce.FpmtBase[i*2+j].bCHK==FALSE)  continue;
			
			FpmtKsce.FpmtBase[i*2+j].dAFP = -dAFST;      
		}
	}
	
	return TRUE;
}

// 설계하중시 경사 인장 응력도 (전단력 최대시) (FST_MS)
BOOL CDgnKSCE_USD05::Get_KsceFsms(ElemPairK ElemK, T_FPMS_JTG& FpmsKsce)
{
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	T_MATD_D MatdD = MatlElem.MatdD;
	double dAFS=0., dAFST=0.;
	Get_AllowblePrincipalStressOfConc(MatdD, dAFS, dAFST);
	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			if(FpmsKsce.FpmsBase[i*2+j].bCHK==FALSE)  continue;
						
			FpmsKsce.FpmsBase[i*2+j].dAFP = -dAFST;
		}
	}

	return TRUE;
}

// 설계하중시 경사 인장 응력도 (비틀림 최대시) (FST_MT)
BOOL CDgnKSCE_USD05::Get_KsceFsmt(ElemPairK ElemK, T_FSMT_D& FsmtD)
{
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	T_MATD_D MatdD = MatlElem.MatdD;
	double dAFS=0., dAFST=0.;
	Get_AllowblePrincipalStressOfConc(MatdD, dAFS, dAFST);
	for(int i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		for(int j=0 ; j<2 ; j++)  // 0=MAX, 1=MIN.
		{
			if(FsmtD.FsmtBase[i*2+j].bCHK==FALSE)  continue;
			
			FsmtD.FsmtBase[i*2+j].dAFS  = -dAFS;
			FsmtD.FsmtBase[i*2+j].dAFST = -dAFST;
		}
	}

	return TRUE;
}
//6-1. 계수하중시 휨강도 (강축)
BOOL CDgnKSCE_USD05::Get_KsceBdcy(ElemPairK ElemK, T_BDCY_KSCE& BdcrKsce, CMSExcel* pXL/*NULL*/, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/)
{
 	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_PSCD_D PscdD; PscdD.Initialize();
	pDoc->m_pAttrCtrl->GetPscd(PscdD);

	BOOL bIter=FALSE; 
	if(PscdD.ifpsMethod==1) bIter = TRUE; //0=설계기준, 1=Iteration

	BOOL bCalcOK=TRUE;
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

	// rp
	// PS강재의 종류(0=저릴랙세이션(0.28), 1=응력제거강재(0.4), 2=강봉(0.55))
	int iTndnKind = PscdD.iTndnKind;
	double drp = 0.0;    
	Get_TndnKindrp(iTndnKind, drp);

	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=iSta; i<iEnd; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		BOOL bPrint = m_Print.Get_PrintFlag();
		//if(bPrint)  m_Print.Print_ElementIJ(ElemK, bPosiI, bPositive);      
			
		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
		arOuter.Copy(SectElem.SectPosi[i].arOuter);
		CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
		int iInnerCellNum = SectElem.SectPosi[i].iInnerCellNum;
		for(int m=0; m<iInnerCellNum; m++)
		{
			CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
			arInner.Copy(SectElem.SectPosi[i].arInner[m]);
			arpInner.Add(&arInner);
		}
			
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		if(!PscdD.bRefRebar) arRbarPosi.RemoveAll();
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		BOOL bRebarExist=TRUE;
		BOOL bTndnExist=TRUE;
		if(arRbarPosi.GetSize()==0 && arTndnPosi.GetSize()==0)
		{
			bRebarExist=FALSE;
			bTndnExist=FALSE;
			CString sEndIJ = (bPosiI ? _T("I") : _T("J"));
			CString sErrMsg = _T("");
			sErrMsg.Format(_LS(IDS_DGN_PSC_NO_REBAR_TENDON_Err), ElemK, sEndIJ);
			//GSaveHistoryNF(sErrMsg);    
		}

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
		double dPhib, dPhiv;
		BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);

		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int k = i*2+j;
			if(!bRebarExist && !bTndnExist) BdcrKsce.BdcyBase[k].bCHK=FALSE;
			if(BdcrKsce.BdcyBase[k].bCHK==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);

//      if(bPrint)  m_Print.Print_ElementIJ(ElemK, bPosiI, bPositive);      

			if(m_iCivilCode==KSCE_USD05 || m_iCivilCode==KSCE_RAIL_USD04)
			{
				if(bPrint)  m_Print.Print_ElementIJ(ElemK.first, bPosiI, bPositive);      
			}
			else if(m_iCivilCode==KSCE_USD10 || m_iCivilCode==KSCE_RAIL_USD11)
			{
				if(bPrint)  m_Print_ksce10.Print_ElementIJ_KSCE10(ElemK.first, bPosiI, bPositive, pXL);      
			}
			else ASSERT(0);

			if(!Get_FlexureStrength(TRUE, bPosiI, bIter, bPositive, dPhib, MatlElem, SectElem.SectPosi[i], SpscSect, arOuter, arpInner, SpscgSect, 
															drp, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcrKsce.BdcyBase[k], pXL)) continue;
			
		}
		// Delete because of New Operator.
		for(int k=0; k<arpInner.GetSize(); k++)
		{
			if(arpInner[k])
			{
				arpInner[k]->RemoveAll();
				delete arpInner[k];
				arpInner[k] = NULL; 
			}
		}
		arpInner.RemoveAll();  

	}
	return bCalcOK;
}

// 6-2. 계수하중시 휨강도 (약축)
BOOL CDgnKSCE_USD05::Get_KsceBdcz(ElemPairK ElemK, T_BDCZ_KSCE& BdcrKsce)
{
 	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_PSCD_D PscdD; PscdD.Initialize();
	pDoc->m_pAttrCtrl->GetPscd(PscdD);

	BOOL bCalcOK = TRUE;
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

	for(int i=0 ; i<2 ; i++)  //I,J
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
		arOuter.Copy(SectElem.SectPosi[i].arOuter);
		CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
		int iInnerCellNum = SectElem.SectPosi[i].iInnerCellNum;
		for(int m=0; m<iInnerCellNum; m++)
		{
			CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
			arInner.Copy(SectElem.SectPosi[i].arInner[m]);
			arpInner.Add(&arInner);
		}
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		if(!PscdD.bRefRebar) arRbarPosi.RemoveAll();
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		BOOL bRebarExist=TRUE;
		BOOL bTndnExist=TRUE;
		if(arRbarPosi.GetSize()==0 && arTndnPosi.GetSize()==0)
		{
			bRebarExist=FALSE;
			bTndnExist=FALSE;
			CString sEndIJ = (bPosiI ? _T("I") : _T("J"));
			CString sErrMsg = _T("");
			sErrMsg.Format(_LS(IDS_DGN_PSC_NO_REBAR_TENDON_Err), ElemK, sEndIJ);
			//GSaveHistoryNF(sErrMsg);    
		}

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
		double dPhib, dPhiv;
		BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);
		
		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int k = i*2+j;
			if(!bRebarExist && !bTndnExist) BdcrKsce.BdczBase[k].bCHK=FALSE;
			if(BdcrKsce.BdczBase[k].bCHK==FALSE) continue;
			BOOL bPositive = (j==0 ? TRUE : FALSE);
			if(!Get_FlexureStrength(TRUE, bPosiI, TRUE, bPositive, dPhib, MatlElem, SpscSect, arOuter, arpInner, SpscgSect, 
															dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcrKsce.BdczBase[k])) continue;
		}
		// Delete because of New Operator.
		for(int k=0; k<arpInner.GetSize(); k++)
		{
			if(arpInner[k])
			{
				arpInner[k]->RemoveAll();
				delete arpInner[k];
				arpInner[k] = NULL; 
			}
		}
		arpInner.RemoveAll();  
	}
	return bCalcOK;
}

//  7. 가설중 경사 인장 응력도 (FST_SG) - JSCE02와 동일
//  8. 설계하중시 경사 인장 응력도 (전단력 최대시) (FST_MS) - JSCE02와 동일
//  9. 설계하중시 전단균열 (전단력 최대시) (CW_MS) - 제외

// 10,11. 계수하중시 전단설계(전단력 최대시)
BOOL CDgnKSCE_USD05::Get_KsceCums(ElemPairK ElemK, T_CUMS_KSCE& CumsKsce, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/)
{
	BOOL bCalcOK = TRUE;

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
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

	T_MATD_D MatdD = MatlElem.MatdD;

	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=iSta; i<iEnd; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		BOOL bPrint = m_Print.Get_PrintFlag();
	
		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);    
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
		T_RPSC_SBAR RpscSbar = RshrElem.RshrPosi[i].RpscSbar;
		double dfdt = RshrElem.RshrPosi[i].dfdt;
		double dfdb = RshrElem.RshrPosi[i].dfdb;
		double dVd  = RshrElem.RshrPosi[i].dVd;
		double dMd  = RshrElem.RshrPosi[i].dMd;    
		
		//Phi
		double dPhib, dPhiv;
		BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);
		for(int j=0 ; j<2 ; j++) //Max, Min
		{
			int k = i*2+j;
			if(CumsKsce.CumsBase[k].bCHK==FALSE) continue;
			if(m_iCivilCode==KSCE_USD05 || m_iCivilCode==KSCE_RAIL_USD04)
			{
				if(bPrint) m_Print.Print_ElementIJ(ElemK.first, bPosiI);
			}
			else if(m_iCivilCode==KSCE_USD10 || m_iCivilCode==KSCE_RAIL_USD11)
			{
				if(bPrint)  m_Print_ksce10.Print_ElementIJ_Shear_KSCE10(ElemK.first, bPosiI);      
			}
			else ASSERT(0);


			if(!Get_ShearDesign(bPosiI, dPhiv, MatdD, SpscSect, SpscgSect, dbw, dfdt, dfdb, dVd, dMd, arRbarPosi, arTndnPosi, RpscSbar, CumsKsce.CumsBase[k])) continue;      
		}
	}
	return bCalcOK;
}

// 12. 설계하중시 경사 인장 응력도 (비틀림 최대시) (FST_MT) - JSCE02와 동일
// 13. 설계하중시 전단균열 (비틀림 최대시) (CW_MT) - 제외

// 14,15. 계수하중시 비틀림설계(비틀림 최대시)
BOOL CDgnKSCE_USD05::Get_KsceCrmt(ElemPairK ElemK, T_CRMT_KSCE& CrmtKsce, BOOL bI/*TRUE*/, BOOL bJ/*TRUE*/)
{
	BOOL bCalcOK = TRUE;
	
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
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

	T_MATD_D MatdD = MatlElem.MatdD;

	int iSta = (bI ? 0 : 1);	// I.
	int iEnd = (bJ ? 2 : 1);	// J.
	for(int i=iSta; i<iEnd; i++)
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		BOOL bPrint = m_Print.Get_PrintFlag();
	
		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);    
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
		T_RPSC_SBAR RpscSbar = RshrElem.RshrPosi[i].RpscSbar;
		double dfdt = RshrElem.RshrPosi[i].dfdt;
		double dfdb = RshrElem.RshrPosi[i].dfdb;
		double dVd  = RshrElem.RshrPosi[i].dVd;
		double dMd  = RshrElem.RshrPosi[i].dMd;    
		//Phi
		double dPhib, dPhiv;
		BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);

		BOOL bSolidSect;
		if(!SectElem.SectPosi[i].bClosedSect) bSolidSect=TRUE;    // bClosedSect -> Box형
		else                                  bSolidSect=FALSE;
		double dAoh = SectElem.SectPosi[i].dAoh;
		double dPh  = SectElem.SectPosi[i].dPh;
		double dKt  = SectElem.SectPosi[i].dKt;    
		double dTorThk = SectElem.SectPosi[i].dTorThk;
		double dPcp = SectElem.SectPosi[i].dPcp;
		double dAcp = SectElem.SectPosi[i].dAcp;

		for(int j=0 ; j<3 ; j++) //Max, Min
		{
			int k = i*3+j;
			if(CrmtKsce.CrmtBase[k].bCHK==FALSE) continue;

			if(m_iCivilCode==KSCE_USD05 || m_iCivilCode==KSCE_RAIL_USD04)
			{
				if(bPrint) m_Print.Print_ElementIJ(ElemK.first, bPosiI);
			}
			else if(m_iCivilCode==KSCE_USD10 || m_iCivilCode==KSCE_RAIL_USD11)
			{
				if(bPrint)  m_Print_ksce10.Print_ElementIJ_Tortion_KSCE10(ElemK.first, bPosiI);  
			}
			else ASSERT(0);

			if(!Get_TorsionDesign(TRUE, bPosiI, dPhiv, MatdD, SpscSect, SpscgSect, bSolidSect, dAoh, dPh, dbw, dTorThk, dPcp, dAcp, dfdt, dfdb, dVd, dMd, arTndnPosi, arRbarPosi, RpscSbar, CrmtKsce.CrmtBase[k])) continue;
		}
	}
	return bCalcOK;
}

BOOL CDgnKSCE_USD05::Get_KsceTndn(ElemPairK ElemK, T_TNDN_KSCE& TndnKsce)
{
	BOOL bCalcOK = TRUE;
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
	if(!Get_PscSecgElem(ElemK, SecgElem)) 
	{
		return FALSE;
	}

	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;
	_PSC_RSHR_ELEM RshrElem;
	if(!Get_PscRshrElem(TRUE, ElemK, RshrElem)) bCalcOK=FALSE;

	for(int i=0; i<2; i++)
	{
		BOOL bPre=TRUE;  // TRUE=Pre-tension, FALSE=Post-tension  
		// Section
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[i];
		T_SPSC_SECT SpscSect  = SectElem.SectPosi[i].SpscSect;
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		int iPreCount=0;
		for(int k=0; k<arTndnPosi.GetSize(); k++)
		{
			 _DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(k);
			 int iLoadType = rTndn.iLoadType; // 0:Pre-tension, 1:Post-tension
			 if(iLoadType==0) iPreCount++;
		}
		if(iPreCount>=arTndnPosi.GetSize()/2) bPre = TRUE;
		else                                  bPre = FALSE;

		double dFlgThk[2]={SectElem.SectPosi[i].dpFlgThk[0], SectElem.SectPosi[i].dpFlgThk[1]}; 

		_PSC_ALWS_POSI AlwsPosi; 
		AlwsPosi.Initialize();
		if(!Get_AllowbleStressOfConc(bPre, MatlElem.MatdD, SpscSect, arRbarPosi, AlwsPosi)) continue;

		for(int j=0 ; j<2 ; j++) // Top, Bot.
		{
			int k = i*2+j;
			if(TndnKsce.TndnBase[k].bCHK==FALSE) continue;
			if(!Get_ReqTendon(ElemK, i, j, AlwsPosi, dFlgThk, SpscgSect, SpscSect, 
												arRbarPosi, arTndnPosi, TndnKsce.TndnBase[k])) continue;
		}
	}
	return bCalcOK;

}
///////////////////////////////////////////
// For Calculation.

BOOL CDgnKSCE_USD05::Get_BeforeAllowbleStressofConc(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn, 
																							T_FYSG_BASE &FysgBase , double& dalComf, double& dalTenf)
{
/*
	BOOL bTop;
	double dStress[6];
	double dMin = FysgBase.dFT;
	dStress[0] = FysgBase.dFT;
	dStress[1] = FysgBase.dFB;
	dStress[2] = FysgBase.dFTL;
	dStress[3] = FysgBase.dFBL;
	dStress[4] = FysgBase.dFTR;
	dStress[5] = FysgBase.dFBR;
	//dStress[6] = FysgD.FysgBase.dFMAX;
	for(int i=0 ; i<6 ; i++)
	{
		if(dMin>dStress[i]) bTop = (i%2==0 ? TRUE : FALSE);
		dMin = min(dMin,dStress[i]);    
	}
*/
	// Tens=(-), Comp=(+).
	BOOL bTop = (FysgBase.dFT < FysgBase.dFB ? TRUE : FALSE);
	
	double dfi[3] = {0., 0., 0.,};
	double dfe[3] = {0., 0., 0.,};
	if(!Get_AllowbleStressOfConc(bPre, FysgBase.dfci, MatdD, SpscSect, arRbar, arTndn, dfi, dfe)) return FALSE;

	// Compression Stress.
	dalComf = dfi[2];

	// Tension Stress
	if(bTop) dalTenf = dfi[0];
	else     dalTenf = dfi[1];

	return  TRUE;
}

BOOL CDgnKSCE_USD05::Get_AfterAllowbleStressofConc(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn, 
																									 T_FYCM_BASE &FycmBase, double& dalComf, double& dalTenf)
{
/*  
	BOOL bTop;
	double dStress[6];
	double dMin=FycmBase.dFT;
	dStress[0] = FycmBase.dFT;
	dStress[1] = FycmBase.dFB;
	dStress[2] = FycmBase.dFTL;
	dStress[3] = FycmBase.dFBL;
	dStress[4] = FycmBase.dFTR;
	dStress[5] = FycmBase.dFBR;
	//dStress[6] = FycmD.FycmBase.dFMAX;
	for(int i=0 ; i<6 ; i++)
	{
		if(dMin>dStress[i]) bTop = (i%2==0 ? TRUE : FALSE);
		dMin = min(dMin,dStress[i]);    
	}
*/
	// Tens=(-), Comp=(+).
	BOOL bTop = (FycmBase.dFT < FycmBase.dFB ? TRUE : FALSE);

	double dfi[3] = {0., 0., 0.,};
	double dfe[3] = {0., 0., 0.,};
	double dfci = 0.0; // 이 함수에서는 필요없음. 
	if(!Get_AllowbleStressOfConc(bPre, dfci, MatdD, SpscSect, arRbar, arTndn, dfi, dfe)) return FALSE;

	// Compression Stress.
	dalComf = dfe[2];

	// Tension Stress
	if(bTop) dalTenf = dfe[0];
	else     dalTenf = dfe[1];
		 
	return TRUE;
}

BOOL CDgnKSCE_USD05::Get_AllowableStressofLcom(BOOL bStage, ElemPairK ElemK, double dfci, double* dAlw)
{
	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))        return FALSE;
	_PSC_SECT_ELEM SectElem;
	if(!Get_PscSectElem(FALSE, ElemK, SectElem)) return FALSE;
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(FALSE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(FALSE, ElemK, TndnElem)) bCalcOK=FALSE;
	
	int i=0, j=0;
	BOOL bPreTndn[2]={TRUE,TRUE};
	for(i=0 ; i<2 ; i++)  // 0=I, 1=J.
	{
		BOOL bPosiI = (i==0 ? TRUE : FALSE);
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[i].SpscSect;      
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[i].arRbarPosi);
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
		double dfi[3] = {0., 0., 0.};
		double dfe[3] = {0., 0., 0.};
		if(!Get_AllowbleStressOfConc(bPreTndn[i], dfci, MatlElem.MatdD, SpscSect, arRbarPosi, arTndnPosi, dfi, dfe)) continue;
		dAlw[i*3+0] = bStage ? dfi[0] : dfe[0];
		dAlw[i*3+1] = bStage ? dfi[1] : dfe[1];
		dAlw[i*3+2] = bStage ? dfi[2] : dfe[2];
	}

	return TRUE;
}

BOOL CDgnKSCE_USD05::Get_AllowbleStressOfConc(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, _PSC_ALWS_POSI& AlwsPosi)
{
	//Material
	double dfck  = MatdD.Data1.Design.C_fc;
	double df_ci = MatdD.Data1.Design.C_fci;
	
	//Compression Stress
	if(bPre) AlwsPosi.dfci = -0.6*df_ci;
	else     AlwsPosi.dfci = -0.55*df_ci;

	AlwsPosi.dfce = -0.4*dfck;
	
	double dAst, dds, dAsc, ddc;
	if(!Get_ReBarInfor(TRUE, 0.0, SpscSect, arRbar, dAst, dds, dAsc, ddc)) return FALSE;
	AlwsPosi.dfti[0] = ( dAsc>0.0 ? 0.5*sqrt(df_ci) : min(1.4, 0.25*sqrt(df_ci)));
	AlwsPosi.dfti[1] = ( dAst>0.0 ? 0.5*sqrt(df_ci) : min(1.4, 0.25*sqrt(df_ci)));
	
	AlwsPosi.dfte[0] = ( dAsc>0.0 ? 0.5*sqrt(dfck) : 0.0);
	AlwsPosi.dfte[1] = ( dAst>0.0 ? 0.5*sqrt(dfck) : 0.0);

	return TRUE;
}

BOOL CDgnKSCE_USD05::Get_AllowbleStressOfConc(BOOL bPre, double df_ci, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn, double* dfi, double* dfe)
{
	// dfi : 0=fti(Top), 1=fti(Bottom), 2=fci
	// dfe : 0=fte(Top), 1=fte(Bottom), 2=fce
	//Material
	double dfck  = MatdD.Data1.Design.C_fc;
	//double df_ci = MatdD.Data1.Design.C_fci;
	

	if(m_iCode==KSCE_USD10)   
	{
		dfi[2] = -0.6*df_ci;    //Compression Stress - 손실전
		 
		dfe[2] = -0.45*dfck;     //Compression Stress - 손실 후
	}
	else
	{
		if(bPre) dfi[2] = -0.6*df_ci;
		else     dfi[2] = -0.55*df_ci;

		dfe[2] = -0.4*dfck;
	}


	// 미리 압축력을 가한 인장구역
	BOOL bExistTndn;  
	BOOL bBond;
	if(arTndn.GetSize()==0) bExistTndn = FALSE;
	else                    bExistTndn = TRUE;

	int iBondTypeCount=0;
	for(int i=0; i<arTndn.GetSize(); i++)
	{
		_DGN_TNDN_CRC TndnInf = arTndn.GetAt(i);
		if(TndnInf.iBondType == 0) iBondTypeCount++;
	}

	if(iBondTypeCount>arTndn.GetSize()/2.) bBond = TRUE;
	else                                   bBond = FALSE;

	double dAst, dds, dAsc, ddc;
	if(!Get_ReBarInfor(TRUE, 0.0, SpscSect, arRbar, dAst, dds, dAsc, ddc)) return FALSE;

	if(m_iCode==KSCE_USD10)  
	{
		// 손실전
		dfi[0] =  ( dAsc>0.0 || bBond ? 0.5*sqrt(df_ci) : 0.0);
		dfi[1] =  ( dAst>0.0 || bBond ? 0.5*sqrt(df_ci) : 0.0);
		
		// 손실 후 - 2005기준을 따름.(2010기준에는 없다..)
		if(bExistTndn)
		{
			dfe[0] = ( dAsc>0.0 || bBond ? 0.5*sqrt(dfck) : 0.0);
			dfe[1] = ( dAst>0.0 || bBond ? 0.5*sqrt(dfck) : 0.0);
		}
		else
		{
			dfe[0] = ( dAsc>0.0 || bBond ? 0.5*sqrt(df_ci) : min(1.4, 0.25*sqrt(df_ci)));
			dfe[1] = ( dAst>0.0 || bBond ? 0.5*sqrt(df_ci) : min(1.4, 0.25*sqrt(df_ci)));  
		}
			

	}
	else if(m_iCode==KSCE_RAIL_USD04)
	{
		if(bExistTndn)
		{
			dfi[0] = 0.0, dfi[1] = 0.0;
			dfe[0] = 0.0, dfe[1] = 0.0;
		}
		else
		{
			dfi[0] = ( dAsc>0.0 || bBond ? 0.5*sqrt(df_ci) : min(1.4, 0.25*sqrt(df_ci)));
			dfi[1] = ( dAst>0.0 || bBond ? 0.5*sqrt(df_ci) : min(1.4, 0.25*sqrt(df_ci)));
	
			dfe[0] = ( dAsc>0.0 || bBond ? 0.5*sqrt(dfck) : 0.0);
			dfe[1] = ( dAst>0.0 || bBond ? 0.5*sqrt(dfck) : 0.0);
		}
	}
	else 
	{
		if(bExistTndn)
		{
			dfi[0] = dfe[0] = ( dAsc>0.0 || bBond ? 0.5*sqrt(dfck) : 0.0);
			dfi[1] = dfe[1] = ( dAst>0.0 || bBond ? 0.5*sqrt(dfck) : 0.0);
		}
		else
		{
			dfi[0] = dfe[0] = ( dAsc>0.0 || bBond ? 0.5*sqrt(df_ci) : min(1.4, 0.25*sqrt(df_ci)));
			dfi[1] = dfe[1] = ( dAst>0.0 || bBond ? 0.5*sqrt(df_ci) : min(1.4, 0.25*sqrt(df_ci)));  
		}
	}

	return TRUE;
}

BOOL CDgnKSCE_USD05::Get_AllowbleStressofPC(T_TDNA_K TdnaK, T_FYPC_KSCE& FypcKsce)
{ 
	int iLoadType=0.0, iRelxType=0.0;
	double dfpu=0.0, dfpy=0.0;
	double dTotalTendonLength=0.0;
	int iBoundaryNum=0;
	if(!m_pDataCtrl->Get_ConTndn(TdnaK,iLoadType,iRelxType,dfpu,dfpy, dTotalTendonLength, iBoundaryNum))  return FALSE;

	double dAFDL1=0.0, dAFDL2=0.0, dAFLL=0.0;
	// KSCE-USD03 
	// Transfer Stage.
	if(m_iCode==KSCE_USD10)
	{
		dAFDL1 = min(0.74*dfpu, 0.82*dfpy);
		dAFDL2 = min(0.74*dfpu, 0.82*dfpy);
	}
	else
	{
		if(iLoadType==0)  // 프리텐션
		{
			if(iRelxType==0) dAFDL1 = 0.75*dfpu;
			else             dAFDL1 = 0.70*dfpu;
		}
		else if(iLoadType==1) // 포스트텐션
		{
			dAFDL1 = 0.70*dfpu;
			dAFDL2 = 0.83*dfpy;
		}
		else  ASSERT(0);

	}
	// Service Load Stage.
	dAFLL = 0.8*dfpy;  //KSCE_USD10에서 명시되지 않아 KSCE_USD05기준을 따른다.

	FypcKsce.dAFDL1 = dAFDL1;
	FypcKsce.dAFDL2 = dAFDL2;
	FypcKsce.dAFLL  = dAFLL;
	return (iLoadType==1 ? dAFDL1*dAFDL2*dAFLL > 0.0 : dAFDL1*dAFLL > 0.0);
}

BOOL CDgnKSCE_USD05::Get_AllowblePrincipalStressOfConc(T_MATD_D& MatdD, double& dAFS, double& dAFST)
{  
	double dfck  = MatdD.Data1.Design.C_fc;
	dAFS = 0.35*sqrt(dfck);
	dAFST= 0.35*sqrt(dfck);

	return TRUE;
}

BOOL CDgnKSCE_USD05::Get_CrackCheck(BOOL bTopCalc, int iCrackType, T_MATD_D& MatdD, double dbw, double dFlgThk[2], double dpMomPos[2][2], T_SPSC_SECT& SpscSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar,
																	 CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double dMu, double dftop, double dfbot, double dMu_tp, double& dfs, double& dw, double& dwa)
{
	if(arRbar.GetSize()==0) return FALSE;
	//Material
	//double dEc =  MatdD.Data1.Analysis.Elast;
	double dfck= MatdD.Data1.Design.C_fc;
	double dEc = MatdD.Data1.Analysis.Elast;
	double dfca = 0.4*dfck;  //Concrete 허용응력 (4.5.2.1 콘크리트 허용응력. 도.설 2003, 587p)
	double dfy =  MatdD.MainRebarData.B_fy;
	double dEs =  MatdD.MainRebarData.B_Elast;
	double dfsa= min(dfy/2.0,180.0);     //Rebar 허용응력, Ref. 도.교(2003) 74page
	 
	//Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy  = dyDim - dyCen;
	double dCpz  = dzDim - dzCen;
	double dArea = SpscSect.dArea;
	double dIyy  = SpscSect.dIyy;
	double dIzz  = SpscSect.dIzz;
 
	//Required Data
	double dh = SpscSect.dzDim;
	double dyst = 0.0, dyten = 0.0;
	double dR = 0.0;
	double dAsY = 0.0, dAstol= 0.0;
	double dd = 0.0, dmax=0.0, dcmax=0.0;
	
	double dpos = 0.0;
	double dLen = 0.0;
	double dEffA=0.0 , dQ=0.0;
	int    iNumberofRbar=0;
	double dEffAofRbar=0.0;
			
	int i=0, j=0;
	BOOL bCalculation = TRUE;
	if(bTopCalc) { if(dftop<=0.0) bCalculation = FALSE; }
	else         { if(dfbot<=0.0) bCalculation = FALSE; }

	if(bTopCalc)
	{
		dyten = dh - dzCen;
		for(i=0 ; i<arRbar.GetSize() ; i++)
		{
			_DGN_RBAR_CRC rBar = arRbar.GetAt(i);
			if(rBar.dz>0.0)
			{
				dAstol += rBar.dArea;
				dAsY += rBar.dArea*fabs(rBar.dz);
				iNumberofRbar++;
				if(dmax<fabs(rBar.dz)+0.5*rBar.dDia) 
				{
					dmax = fabs(rBar.dz)+0.5*rBar.dDia;  //dC를 위한 변수(도심 기준)
					dcmax = fabs(rBar.dz);                     //ddc를 위한 변수(도심 기준)  
				}
			}
		}
		if(dAstol<=0.0 || iNumberofRbar==0) return FALSE;
		dyst = (dAstol==0.0 ? 0.0 : dAsY/dAstol);
		dd = dzCen + dyst;                       
		//Calculation A.
		dpos = dh-2.0*(dh-dd);
		dLen = SpscSect.dyDim;
		if(!CalcSectPSC_QA(arOuter, arInner, TRUE, TRUE, dpos, dLen, dEffA, dQ)) return FALSE;  //BOOL 방향, 축
		dEffAofRbar = dEffA/iNumberofRbar;
	}
	else
	{
		dyten = dzCen;
		for(i=0 ; i<arRbar.GetSize() ; i++)
		{
			_DGN_RBAR_CRC rBar = arRbar.GetAt(i);
			if(rBar.dz<0.0)
			{
				dAstol += rBar.dArea;
				dAsY += rBar.dArea*fabs(rBar.dz);
				iNumberofRbar++;
				if(dmax<fabs(rBar.dz)+0.5*rBar.dDia) 
				{
					dmax = fabs(rBar.dz)+0.5*rBar.dDia;
					dcmax = fabs(rBar.dz);
				}
			}
		}
		if(dAstol<=0.0 || iNumberofRbar==0.0) return FALSE;
		dyst = (dAstol==0.0 ? 0.0 : dAsY/dAstol);
		dd = (dh-dzCen) + dyst;
		dpos = 2.0*(dh-dd);
		dLen = dyDim;
		if(!CalcSectPSC_QA(arOuter, arInner, FALSE, TRUE, dpos, dLen, dEffA, dQ)) return FALSE;
		dEffAofRbar = dEffA/iNumberofRbar;
	}
	dR = ( dyst==0.0 ? 0.0 : dyten/dyst );
	
	//dc
	double ddc = dyten - dcmax;
	
	//Calculation fs.
	//Calculation Neutral
	
	double dEpsi_cp=0.0, dEpsi_st=0.0, dEpsi_sc=0.0;
	double dEpsi_ce=0.0, dEpsi_pe=0.0, dEpsi_ps=0.0;

	BOOL bYaxis = TRUE;
	double dns = (dEc==0.0 ? 0.0 : dEs/dEc);      
	double dnp = 0.;
		
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	double dgap=0.0;
	double dAptol=0.0, dNtodp=0.0, ddp=0.0;
	double dAsttol=0.0, dNtodst=0.0, ddst=0.0;
	double dAsctol=0.0, dNtodsc=0.0, ddsc=0.0;
	double dApt=0.0, dApc=0.0, dAst=0.0, dAsc=0.0;
		
	double dNeuC=0.0;
	double dfst=0.0, dfsc=0.0;
	
	double daArea=0.0, daQ=0.0, daCent=0.0;
	double da=0.0, dCc=0.0, dTp=0.0, dTst=0.0, dCsc=0.0, dCcDel=0.0;
	double dfps=0.0, dMps=0.0;
	double dMn=0.0, dphiMn=0.0, dMn_Cal=0.0, dphiMn_Cal=0.0;
	double dTpTimesep=0.0, dPeTimesep=0.0, dfpsiApsi=0.0;
	double dTOL = 10; //1kgf
	double dTOL2 = 0.001;
	double dToler=0.0;
	
	//계산시 사용되는 변수..Iteration & 도로교설계기준 공통
	double dApw=0.0, dApf=0.0;
	double dqp=0.0;
	double dfpetotal=0.0, dPetotal=0.0;  
	double dfpu=0.0;
	double dfpsave=0.0;
	double dRhop_tot=0.0, dRhop = 0.0, dRhos=0.0, dRhoc=0.0;
	double dbdp=0.0, dbds=0.0, dQa=0.0;  

	// Code의 공식을 사용할 때.
	BOOL bOneType=TRUE;
	BOOL bBondType=TRUE;
	UINT iTenTndnNum=0;
	UINT iBondNum   =0;
	UINT iUnbondNum =0;
	double dhf=0.0;
	double dfpe_unb_cal = 0.0;
	double dfpe_unb = 0.0;
	double d05fpu = 0.0;
	double dMn_ps=0.0;
	double dMn_beta1 = 0.0;
	double dphiMn_beta1=0.0;  
	 // 철도기준에 사용되는 변수.
	 double dfpy_bond=0.0;
	 double dfpy_unbond=0.0; 
	 double dfps_cal=0.0;
	 double dfps_un_max=0.0;
	 double dfps_un_com=0.0;

	double dIner=(bYaxis==TRUE ? dIyy : dIzz);
	BOOL bPositive = (bTopCalc ? FALSE : TRUE);
	BOOL bDirec = (bPositive ? TRUE : FALSE);
	double dLength = (bYaxis==TRUE ? dyDim : dzDim);
	double dbf_top = dyDim;
	double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
	double db  = (bPositive ? dbf_top : dbf_bot);    
	
	//0:Pei 1:Apsi 2:dpi 3:epi 4:Epsi_ps 5:fpsi 6:Apsi·fpsi 7:Tpi·epi 8:Pei·epi
	CArray<CString,CString> strTndn;
	CArray<double,double> arfps[9]; 
	BOOL bPrint = m_Print.Get_PrintFlag();

	double dfpsA=0.0;

	//Assume C. C = kd
	dNeuC = (bYaxis==TRUE ? dzDim : dyDim) * 0.5;
	double dCp = 0.0;
	if(bPositive) dCp = (bYaxis==TRUE ? dCpz : dCpy);
	else          dCp = (bYaxis==TRUE ? dzCen : dyCen);
	double dCtemp=0.0;
	int NumberofTendon = arTndnPosi.GetSize();
	UINT iCount = 1;  
	// Flange Height.
	dhf = (bPositive ? dFlgThk[0] : dFlgThk[1]);

	// 중립축 결정을 위해서. 
	BOOL bPosiForce=TRUE;
	double dNeuC1 = 0.0;
	double dNeuC2 = 0.0;
	BOOL bStop = TRUE;
			
	CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> rarRbarPosi;
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> rarTndnPosi;
	
	if(bCalculation)
	{
		do
		{
			bPosiForce = TRUE;
			iTenTndnNum=0;
			iBondNum   =0;
			iUnbondNum =0;
			dfpsiApsi  = 0.0;
			if(bPrint)
			{
				strTndn.RemoveAll();
				for( i=0; i<9; i++) arfps[i].RemoveAll();        
			
				dTpTimesep = 0.0;
			}

			dPetotal=0.0;
			//beta1에 대한 면적은 Iteration으로 계산하지 않음. ->나중에 Iteration 적용.
			da = dNeuC;
			double daTemp = (bPositive ? dh-da : da);
			if(CalcSectPSC_QA(arOuter, arInner, bDirec, bYaxis, daTemp, dLength, daArea, daQ))
			{
				daCent = (daArea==0.0 ? 0.0 : daQ/daArea);
				dCc    = dNeuC==0.0 ? 0.0 : daQ/dNeuC; // dalpha1*dfck*daArea
			}
	
			//dNeuC을 기준으로 Rebar(Ac,d) 계산 
			dCtemp = (bPositive ? dNeuC : dh-dNeuC);
			_DGN_TNDN_RBAR rTdRb;
			rTdRb.Initialize();      
			BOOL bGetOk = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbar, arTndnPosi, rTdRb);
			dAptol =(bPositive ? rTdRb.dApttol : rTdRb.dApctol); 
			ddp    =(bPositive ? rTdRb.ddpt    : dh-rTdRb.ddpc);
			if(dAptol==0.0) ddp = 0.0;
			dAsttol=(bPositive ? rTdRb.dAsttol : rTdRb.dAsctol); 
			ddst   =(bPositive ? rTdRb.ddst    : dh-rTdRb.ddsc);
			if(dAsttol==0.0) ddst = 0.0;
			dAsctol=(bPositive ? rTdRb.dAsctol : rTdRb.dAsttol); 
			ddsc   =(bPositive ? rTdRb.ddsc    : dh-rTdRb.ddst);
			if(dAsctol==0.0) ddsc = 0.0;
			
			double dbdp_It = 0.0;
			double dbd_It = 0.0;      
			if(dhf>=da)
			{
				dbd_It = db*ddst;
				dRhos  = (dbd_It==0.0 ? 0.0 : dAsttol/dbd_It);
				dRhoc  = (dbd_It==0.0 ? 0.0 : dAsctol/dbd_It);

				dbdp_It = db*ddp;
				dRhop = (dbdp_It==0.0 ? 0.0 : dAptol/dbdp_It);
			}
			else if(dhf<da)
			{
				dbd_It = dbw*ddst;
				dRhos  = (dbd_It==0.0 ? 0.0 : dAsttol/dbd_It);
				dRhoc  = (dbd_It==0.0 ? 0.0 : dAsctol/dbd_It);

				dbdp_It = dbw*ddp;
				dRhop = (dbdp_It==0.0 ? 0.0 : dAptol/dbdp_It);
			}

			Get_TndnReBarTransform(bYaxis, bPositive, dNeuC, dCp, SpscSect, arRbar, arTndnPosi, rarRbarPosi, rarTndnPosi);

			dfps=0.0; dMps=0.0; dTp=0.0;
			double dfpu_min = 1.7E308; 
			for(i=0 ; i<NumberofTendon ; i++)
			{
				_DGN_TNDN_CRC rTnInf = rarTndnPosi.GetAt(i);
				double dposi = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
				double dEpsi  = rTnInf.dEps;
			
				if(bPositive && dposi<0.0) 
				{
					double ddpi = dNeuC + fabs(dposi);
					double dPe  = rTnInf.dArea * rTnInf.dStress[0];
					dEpsi_cp = (dNeuC==0.0 ? 0.0 : (ddpi-dNeuC)/dNeuC);
					dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
					dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe/(dArea*dEc)*(1.0+(dposi*dposi)/(dIner/dArea));  /**/
					dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
					dnp = dEc==0.0 ? 0.0 : dEpsi/dEc;
					if(rTnInf.iBondType==0)
					{
						double dfps_cp = dnp * dEpsi_cp;
						double dfps_pe_ce = (dfca==0.0 ? 0.0 : dEpsi*(dEpsi_pe+dEpsi_ce)/dfca);
						dfpsA = dfps_cp + dfps_pe_ce;
					}
					else if(rTnInf.iBondType==1) 
					{
						dfpy_unbond = rTnInf.dfpy;
						double dfpe_unbi = rTnInf.dStress[0];
						if(m_iCode==KSCE_USD05)
						{
							if(dfpe_unbi>=0.5*rTnInf.dfpu)     dfps = dfpe_unbi + 105.0; 
							else if(dfpe_unbi<0.5*rTnInf.dfpu) dfps = 0.5*rTnInf.dfpu   + 105.0;          
						}
						else if( m_iCode==KSCE_USD10)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 420.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
						else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 400.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
						else ASSERT(0);
						dfpsA = dfca==0.0 ? 0.0 :dfps/dfca;
					}
					dPetotal += rTnInf.dArea*rTnInf.dStress[0];
					dTp += dfpsA*rTnInf.dArea;
					dMps += dfps*rTnInf.dArea*fabs(dposi); 
					dfpu_min = min(rTnInf.dfpu,dfpu_min);          
					dfpsiApsi  += dfps*rTnInf.dArea;
				}
				else if(!bPositive && dposi>0.0)
				{
					double ddpi = dNeuC + fabs(dposi);
					double dPe  = rTnInf.dArea * rTnInf.dStress[0];
					dEpsi_cp = (dNeuC==0.0 ? 0.0 : (ddpi-dNeuC)/dNeuC);
					dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
					dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe/(dArea*dEc)*(1.0+(dposi*dposi)/(dIner/dArea));
					dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
					dnp = dEc==0.0 ? 0.0 : dEpsi/dEc;
					if(rTnInf.iBondType==0)
					{
						double dfps_cp = dnp * dEpsi_cp;
						double dfps_pe_ce = (dfca==0.0 ? 0.0 : dEpsi*(dEpsi_pe+dEpsi_ce)/dfca);
						dfpsA = dfps_cp + dfps_pe_ce;
					}
					else if(rTnInf.iBondType==1) 
					{
						dfpy_unbond = rTnInf.dfpy;
						double dfpe_unbi = rTnInf.dStress[0];
						if(m_iCode==KSCE_USD05)
						{
							if(dfpe_unbi>=0.5*rTnInf.dfpu)     dfps = dfpe_unbi + 105.0; 
							else if(dfpe_unbi<0.5*rTnInf.dfpu) dfps = 0.5*rTnInf.dfpu   + 105.0;          
						}
						else if(m_iCode==KSCE_USD10)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 420.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
						else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 400.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
						else ASSERT(0);
						dfpsA = dfca==0.0 ? 0.0 :dfps/dfca;
					}
					dPetotal += rTnInf.dArea*rTnInf.dStress[0];
					dTp += dfpsA*rTnInf.dArea;
					dMps += dfps*rTnInf.dArea*fabs(dposi);
					dfpu_min = min(rTnInf.dfpu,dfpu_min);
					dfpsiApsi  += dfps*rTnInf.dArea;

				}               
			}
			dfpu = dfpu_min;
				
			dTst = dns*dAsttol*(ddst-dNeuC); 
			dCsc = (dns-1.)*dAsctol*(dNeuC-ddsc);
		  //dTp = dAptol*dfps;
		
			double dComp = dCc + dCsc;
		  dCcDel = dTp + dTst - dComp;
			dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);

			if(dCcDel<0.0) bPosiForce = FALSE;

			///
			if(iCount==1) dNeuC2 = dNeuC;
			dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
			if(fabs(dToler)<dTOL2) break;
			if(iCount>m_PSC_iITER) break;
			
			if(dCcDel<=0.0) dNeuC2 = dNeuC;
			else            dNeuC1 = dNeuC;
			dNeuC = 0.5*(dNeuC1 + dNeuC2);
			///
			
			if(iCount>50 && fabs(dNeuC1-dNeuC2)<0.00001) bStop= TRUE;
			if(fabs(dToler)<dTOL2)                       bStop= TRUE;

//       if(bPosiForce)
//       {
//         dNeuC += 0.3*dh;
//         dNeuC2 = dNeuC;
//         dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
//         if(fabs(dToler)<dTOL2) break;
//         if(iCount>m_PSC_iITER) break;
//       }
//       else
//       {
//         if(iCount==1) dNeuC2 = dNeuC;
//         dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
//         if(fabs(dToler)<dTOL2) break;
//         if(iCount>m_PSC_iITER) break;
//       
//         if(dCcDel<=0.0) dNeuC2 = dNeuC;
//         else            dNeuC1 = dNeuC;
//         dNeuC = 0.5*(dNeuC1 + dNeuC2);
//       }

			++iCount;

		}while(!bStop); //fabs(dToler)>dTOL2    
	}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	double dMomLength = dAsttol*(ddst-(dNeuC-daCent));
	dfs = dMomLength==0.0 ? 0.0 : fabs(dMu_tp)/dMomLength;
	if(dfs<0.0) dfs = 0.0;
	if(dfs>dfsa) dfs = dfsa;  
		
	//휨 균열폭 
	dw = 1.08*dR*dfs*pow((ddc*dEffAofRbar),1./3.)*(1.0E-05);

	double dC = (dyten - dmax);  //mm
	switch(iCrackType)
	{
	case 0: dwa = 0.006*dC; break;
	case 1: dwa = 0.005*dC; break;
	case 2: dwa = 0.004*dC; break;
	case 3: dwa = 0.0035*dC; break;
	}
	if(dw<0.0) return FALSE;

	if(!bCalculation) dwa = -9999.0;

	return TRUE;
}

BOOL CDgnKSCE_USD05::CalctensLength(double dftop, double dfbot, double dh, double& dx)
{
	//(+) : 인장
	double dStrHratio=0.0;
	if(dftop<0.0 && dfbot<0.0) dx=0.0; 
	else if((dftop>=0.0 && dfbot<=0.0) || (dftop<=0.0 && dfbot>=0.0))
	{
		if(dftop>0.0)
		{
			dStrHratio = (dh==0.0 ? 0.0 : (fabs(dfbot)+dftop)/dh);
			dx = (dStrHratio==0.0 ? 0.0 : dftop/dStrHratio);
		}
		else
		{
			dStrHratio = (dh==0.0 ? 0.0 : (fabs(dftop)+dfbot)/dh);
			dx = (dStrHratio==0.0 ? 0.0 : dfbot/dStrHratio);
		}
	}
	else if(dftop>0.0 && dfbot>0.0) dx = dh;

	return TRUE;
}

BOOL CDgnKSCE_USD05::CalcSectPSC_QA(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, BOOL bDir, BOOL baxis, double dpos, double dlen, double& dCutArea, double& dQ)
{
	// Outer=CCW, Inn=CW.
	dQ = 0.0;
	dCutArea = 0.0;
	//*************************************
	// Get Polygon by Cutting Line.
	I_PolyMaker* pPolyMaker;
	pPolyMaker = I_PolyMakerFactory::GetInstance()->New_PMaker();
	int i=0, j=0, k=0;    
	// Outer.
	CArray<XGL_3dp,XGL_3dp&> arOutCut;
	for(i=0; i<arOuter.GetSize(); i++)
	{
		T_SPSC_COOR SpscCoor = arOuter[i];
		for(j=0; j<SpscCoor.arSpscPont[0].GetSize(); j++)
		{
			// x,y Coordinate (from Left, Bottom).
			double dx = SpscCoor.arSpscPont[0].GetAt(j);
			double dy = SpscCoor.arSpscPont[1].GetAt(j);
			XGL_3dp OutXGL;
			OutXGL.Set(dx,dy,0.0);
			arOutCut.Add(OutXGL);
		}
	}
	pPolyMaker->SetOutterPoly(arOutCut);
		
	// Inner.
	for(i=0; i<arInner.GetSize(); i++)
	{
		CArray<XGL_3dp,XGL_3dp&> arInnCut;
		T_SPSC_COOR_AR* apSpscCoor = arInner[i];
		for(j=0; j<apSpscCoor->GetSize(); j++)
		{
			arInnCut.RemoveAll();
			T_SPSC_COOR SpscCoor = apSpscCoor->GetAt(j);
			for(k=0; k<SpscCoor.arSpscPont[0].GetSize(); k++)
			{
				// x,y Coordinate (from Left, Bottom).
				double dx = SpscCoor.arSpscPont[0].GetAt(k);
				double dy = SpscCoor.arSpscPont[1].GetAt(k);
				XGL_3dp InnXGL;
				InnXGL.Set(dx,dy,0.0);
				arInnCut.Add(InnXGL);
			}
			pPolyMaker->AddInnerPoly(arInnCut);
		}
	}

	double dZero=1.0E-07;
	// Make Polygon by Cutting Line.
	// Get Left Part of Cutting Line (Sta->End).
	XGL_3dp StaXGL, EndXGL;
	double dxSta=0.0, dxEnd=0.0, dySta=0.0, dyEnd=0.0;
	if(bDir==1)  // 1: Left->Right , Top->Bot  0: Right->Left, Bot->Top
	{
		 dxSta = (baxis==1 ? (-1)*dZero : dpos);  //1: y-axis 0: z-axis
	   dxEnd = (baxis==1 ? dlen+dZero : dpos);
	   dySta = (baxis==1 ? dpos : dlen+dZero);
	   dyEnd = (baxis==1 ? dpos : (-1)*dZero);
	}
	else
	{
		 dxSta = (baxis==1 ? dlen+dZero : dpos);  //1: y-axis 0: z-axis
	   dxEnd = (baxis==1 ? (-1)*dZero : dpos);
	   dySta = (baxis==1 ? dpos : (-1)*dZero);
	   dyEnd = (baxis==1 ? dpos : dlen+dZero);
	}

	StaXGL.Set(dxSta,dySta,0.0);
	EndXGL.Set(dxEnd,dyEnd,0.0);
	pPolyMaker->SetCuttingLine(StaXGL,EndXGL);
	if(!pPolyMaker->MakeResult())	ASSERT(0);
	// Result Polygon.
	for(j=0; j<pPolyMaker->GetResultPolyCount(); j++)
	{
		CArray<XGL_3dp,XGL_3dp&> arResPoly;
		pPolyMaker->GetResultPoly(j,arResPoly);
		int iResNum = arResPoly.GetSize();
		//---------------------------------
		// Calculate Center(x,y), Area.
		double* dpx = new double[iResNum];
		double* dpy = new double[iResNum];
		for(k=0; k<iResNum; k++)
		{
			dpx[k] = arResPoly[k].x();
			dpy[k] = arResPoly[k].y();
		}
		double dxCen=0.0, dyCen=0.0, dArea=0.0;
		if(!CMathFunc::mathPolyCentroid(iResNum,dpx,dpy,dxCen,dyCen,dArea))	ASSERT(0);
		delete []dpx;
		delete []dpy;
		//---------------------------------
		dCutArea += dArea;
		if(baxis==1)			dQ += dArea*fabs(dyCen-dpos);
		else if(baxis==0)	dQ += dArea*fabs(dxCen-dpos);
		else	ASSERT(0);
	}
	//------------------------------------------------------------------------------
	// Hole 을 얻어 오는것이 별도 함수로 분리 되면서 추가된 부분 
	for(j=0; j<pPolyMaker->GetResultHolePolyCount(); j++)
	{
		CArray<XGL_3dp,XGL_3dp&> arResPoly;
		pPolyMaker->GetResultHolePoly(j,arResPoly);
		int iResNum = arResPoly.GetSize();
		//---------------------------------
		// Calculate Center(x,y), Area.
		double* dpx = new double[iResNum];
		double* dpy = new double[iResNum];
		for(k=0; k<iResNum; k++)
		{
			dpx[k] = arResPoly[k].x();
			dpy[k] = arResPoly[k].y();
		}
		double dxCen=0.0, dyCen=0.0, dArea=0.0;
		if(!CMathFunc::mathPolyCentroid(iResNum,dpx,dpy,dxCen,dyCen,dArea))	ASSERT(0);
		delete []dpx;
		delete []dpy;
		//---------------------------------
		dCutArea -= dArea;
		if(baxis==1)			dQ -= dArea*fabs(dyCen-dpos);
		else if(baxis==0)	dQ -= dArea*fabs(dxCen-dpos);
		else	ASSERT(0);
	}

	I_PolyMakerFactory::GetInstance()->Del_PMaker(pPolyMaker);
	
	
	return TRUE;
}

BOOL CDgnKSCE_USD05::Get_TensionRebar(BOOL bTopCalc,  BOOL bPrint, T_MATD_D MatdD, double dbw, T_SPSC_SECT& SpscgSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi,
																			double dftop, double dfbot, double& dAreq, double& dAuse, double& dAmin)
{
	//Material
	double dfck= MatdD.Data1.Design.C_fc;
	double dfy = MatdD.MainRebarData.B_fy;
	double dfsa= min(dfy/2.0,180.0);     //Rebar 허용응력, Ref. 도.교(2003) 74page
	double dEs = MatdD.MainRebarData.B_Elast;
 
	//Section
	double dyDim = SpscgSect.dyDim;
	double dzDim = SpscgSect.dzDim;
	double dyCen = SpscgSect.dyCen;
	double dzCen = SpscgSect.dzCen;
	double dArea = SpscgSect.dArea;
			
	//Required Data
	double dWidth = 0.0;
	double dh = SpscgSect.dzDim;
	double dyten=0.0, dAstol=0.0, dAsY=0.0, dyst=0.0;
	double dLen=0.0, dd=0.0, dbd=0.0, dx=0.0, ddistb=0.0;
	double dTfor=0.0, dfts=0.0;
	double dAten=0.0, dYcent=0.0, dfcent=0.0;
	double dQ=0.0;
	double dAsover=0.0;
	
	double dApt=0.0, dApc=0.0, dAst=0.0, dAsc=0.0;
	double ddp=0.0, ddst=0.0, ddsc=0.0;
	double dFS = 1.15;
	dAuse = 0.0;

	BOOL bYaxis = TRUE;
	
	int i=0, j=0;
	double dCtemp = (!bTopCalc ? dh-dzCen : dzCen);
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();      
	BOOL bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscgSect, arRbar, arTndnPosi, rTdRb);
	if(!bTopCalc)  // 정모멘트 
	{
		dApt=rTdRb.dApttol; ddp  = (dApt==0.0 ? 0.0 : rTdRb.ddpt);
		dApc=rTdRb.dApctol;
		dAst=rTdRb.dAsttol; ddst = (dAst==0.0 ? 0.0 : rTdRb.ddst);
		dAsc=rTdRb.dAsctol; ddsc = (dAsc==0.0 ? 0.0 : rTdRb.ddsc);
	}
	else
	{
		dApt=rTdRb.dApctol; ddp  = (dApt==0.0 ? 0.0 : dh - rTdRb.ddpc);
		dApc=rTdRb.dApttol; 
		dAst=rTdRb.dAsctol; ddst = (dAst==0.0 ? 0.0 : dh - rTdRb.ddsc);
		dAsc=rTdRb.dAsttol; ddsc = (dAsc==0.0 ? 0.0 : dh - rTdRb.ddst);
	}

	dAuse = dAst; // Top.Bottom모두 dAst를 사용함.
	if(ddst<=0.0) ddst = 0.8*dh;
	dbd = dbw * ddst;  

	//Print..  
	if(bPrint) m_Print.Print_TbarDesign1();
	//Tension(+) Compression
	bCalcOK = TRUE;
	
	if(bTopCalc) // Top Calculation.
	{
		if(dftop>0.0) 
		{
			dyten = dh-dzCen;
			dLen = dyDim;
			ddistb = dh - dx;
			if(dfbot<0.0)
			{
				if(!CalctensLength(dftop, dfbot, dh, dx)) return FALSE;  //tension length

				//Calcultion Tension force
				if(!CalcSectPSC_QA(arOuter, arInner, TRUE, TRUE, ddistb, dLen, dAten, dQ)) bCalcOK = FALSE;
				dYcent = (dAten==0.0 ? 0.0 : dQ/dAten);
				dfcent = (dx==0.0 ? 0.0 : dYcent*dftop/dx);
				dTfor = dfcent*dAten;

				dAreq = (dfsa==0.0 ? 0.0: dTfor/dfsa);
				dAsover = 4.0/3.0*dAreq;
				//Print.
				if(bPrint) m_Print.Print_TbarDesign2(dftop, dfbot, dh, dx, dYcent, dfcent, dAten, dTfor, dfsa, dFS, dAreq);
			}
			else if(dfbot>0.0)
			{
				double dft=0.0, dfm=0.0;
				dyten = dh;
				if(dftop>dfbot)
				{       
					//Calcultion Tension force
					dft    = dftop - dfbot;
					dfm    = dzCen*dft/dh;
					dfcent = dfbot+dfm;
					dTfor  = dfcent*dArea;  //Ag
				}
				else if(dfbot>dftop)
				{
					//Calcultion Tension force
					dYcent = dzCen;
					dft    = dfbot - dftop;
					dfm    = (dh-dzCen)*dft/dh;
					dfcent = dftop+dfm;
					dTfor  = dfcent*dArea;  //Ag
				}
				else if(dfbot==dftop)
				{
					dfcent = dftop;
					dTfor  = dfcent*dArea;  //Ag
				}

				dAreq = (dfsa==0.0 ? 0.0: dTfor/dfsa);
				dAsover = 4.0/3.0*dAreq;
				//Print.
				if(bPrint) m_Print.Print_TbarDesign3(dft, dfm, dftop, dfbot, dh, dx, dYcent, dfcent, dTfor, dfsa, dFS, dAreq, dyten, dArea);
			}    
		}
		else ASSERT(0);
	}
	else
	{
		if(dfbot>0.0)
		{
			dyten = dzCen;
			dLen = dyDim;      
			if(dftop<0.0)
			{
				if(!CalctensLength(dftop, dfbot, dh, dx)) return FALSE;  //tension length

				//Calcultion Tension force
				if(!CalcSectPSC_QA(arOuter, arInner, FALSE, TRUE, dx, dLen, dAten, dQ)) bCalcOK = FALSE;
				dYcent = (dAten==0.0 ? 0.0 : dQ/dAten);
				dfcent = (dx==0.0 ? 0.0 : dYcent*dfbot/dx);
				dTfor = dfcent*dAten;

				dAreq = (dfsa==0.0 ? 0.0: dTfor/dfsa);
				dAsover = 4.0/3.0*dAreq;
				//Print.
				if(bPrint) m_Print.Print_TbarDesign2(dftop, dfbot, dh, dx, dYcent, dfcent, dAten, dTfor, dfsa, dFS, dAreq);
			}
			else if(dftop>0.0)
			{
				double dft=0.0, dfm=0.0;
				dyten = dh;
				if(dftop>dfbot)
				{       
					//Calcultion Tension force
					dft    = dftop - dfbot;
					dfm    = dzCen*dft/dh;
					dfcent = dfbot+dfm;
					dTfor  = dfcent*dArea;  //Ag
				}
				else if(dfbot>dftop)
				{
					//Calcultion Tension force
					dYcent = dzCen;
					dft    = dfbot - dftop;
					dfm    = (dh-dzCen)*dft/dh;
					dfcent = dftop+dfm;
					dTfor  = dfcent*dArea;  //Ag
				}
				else if(dfbot==dftop)
				{
					dfcent = dftop;
					dTfor  = dfcent*dArea;  //Ag
				}

				dAreq = (dfsa==0.0 ? 0.0: dTfor/dfsa);
				dAsover = 4.0/3.0*dAreq;
				//Print.
				if(bPrint) m_Print.Print_TbarDesign3(dft, dfm, dftop, dfbot, dh, dx, dYcent, dfcent, dTfor, dfsa, dFS, dAreq, dyten, dArea);
			}    
		}
		else ASSERT(0);
	}

	//Calculation As,min.
	// 4.3.6.1 (1) 항목만 적용. ( (2). 항목의 정정구조물이란것을 판단 할 수 없다. )
	double dAmin1 = (dfy==0.0 ? 0.0 : 0.25*sqrt(dfck)/dfy*dbd); // 도로교 2005, 248p
	double dAmin2 = (dfy==0.0 ? 0.0 : 1.4/dfy*dbd); 
	dAmin = max(dAmin1, dAmin2);
	
	return bCalcOK;
}

//휨강도(Y-axis)
BOOL CDgnKSCE_USD05::Get_FlexureStrength(BOOL bYaxis, BOOL bPosiI, BOOL bIter, BOOL bPositive, double dPhib, _PSC_MATL_ELEM& MatlElem, _PSC_SECT_POSI &SectPosD, T_SPSC_SECT& SpscSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, T_SPSC_SECT& SpscgSect,
																				 double drp, double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_BDCY_KSCE_BASE& BdcrKsce, CMSExcel* pXL, BOOL bApwM, BOOL bRatingPrt)

{
	T_MATD_D &MatD = MatlElem.MatdD;
	//Material
	double dfck= MatD.Data1.Design.C_fc;
	double dEc = MatD.Data1.Analysis.Elast;
	double dfy = MatD.MainRebarData.B_fy;
	double dEs = MatD.MainRebarData.B_Elast;  
	 // Slab
	double dfck_s = MatD.Data2.Design.C_fc;
	double dEc_s  = MatD.Data2.Analysis.Elast;
	double dfy_s  = MatD.MainRebarData2.B_fy;
	double dEs_s  = MatD.MainRebarData2.B_Elast;  
	double dm2    = (dfck==0.0) ? 1.0 : dfck_s/dfck;
	double dnEc   = (dEc==0.0)? 0.0 : dEc_s/dEc;
	
	//Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy  = dyDim - dyCen;
	double dCpz  = dzDim - dzCen;
	double dArea = SpscSect.dArea;
	double dIyy  = SpscSect.dIyy;
	double dIzz  = SpscSect.dIzz;
	
	//Section_g
	double dyDim_g = SpscgSect.dyDim;
	double dzDim_g = SpscgSect.dzDim;
	double dyCen_g = SpscgSect.dyCen;
	double dzCen_g = SpscgSect.dzCen;
	double dCpy_g  = dyDim_g - SpscgSect.dyCen;
	double dCpz_g  = dzDim_g - SpscgSect.dzCen;
	double dAg     = SpscgSect.dArea;
	double dIyy_g  = SpscgSect.dIyy;
	double dIzz_g  = SpscgSect.dIzz;
	double dIner_g=(bYaxis==TRUE ? dIyy_g : dIzz_g);

 // Composite Data,
	double dBc = SectPosD.dBc;// * dnEc; // 슬래브 폭
	double dtc = SectPosD.dtc;           // 슬래브 두께
	double dHh = SectPosD.dHh;

	double dH_gr = dzDim-dtc-dHh; // 거더 높이
	

	//Force
	double dMu = BdcrKsce.dMuy;
		
	double dApcmax=0.0; //최대강재량
	double dphi = dPhib;
	double dalpha1     = 0.85; // phi와는 다른 값.
	double dBeta1      = Get_Beta1(dfck);
	double dBeta1_s    = Get_Beta1(dfck_s);
	double d036beta1   = 0.36*dBeta1;
	double d036beta1_s = 0.36*dBeta1_s;
	 
	double dEpsi_cu = 0.003;
	double dEpsi_cp=0.0, dEpsi_st=0.0, dEpsi_sc=0.0;
	double dEpsi_ce=0.0, dEpsi_pe=0.0, dEpsi_ps=0.0;
		
	int i=0,j=0;
	double dgap=0.0;
	double dAstol=0.0, dAsY=0.0;
	double dAptol=0.0, dNtodp=0.0, ddp=0.0;
	double dAsttol=0.0, dNtodst=0.0, ddst=0.0;
	double dAsctol=0.0, dNtodsc=0.0, ddsc=0.0;
	double dApt=0.0, dApc=0.0, dAst=0.0, dAsc=0.0;
		
	double dNeuC=0.0;
	double ddist=0.0;
	double dfst=0.0, dfsc=0.0;
	double dfst_cal=0.0, dfsc_cal=0.0;
	
	double daArea=0.0, daQ=0.0, daCent=0.0;
	double da=0.0, dCc=0.0, dTp=0.0, dTst=0.0, dCsc=0.0, dCcDel=0.0;
	double da_g=0.0;
	double da_s=0.0, dCc_s=0.0;
	double dfps=0.0, dMps=0.0;
	double dfps_temp_cal =0.0, dfps_temp = 0.0;
	double dMn=0.0, dphiMn=0.0, dMn_Cal=0.0, dphiMn_Cal=0.0;
	double dTpTimesep=0.0, dPeTimesep=0.0, dfpsiApsi=0.0;
	double dTOL = 10; //1kgf
	double dTOL2 = 0.001;
	double dToler=0.0;
	
	//계산시 사용되는 변수..Iteration & 도로교설계기준 공통
	double dApw=0.0, dApf=0.0;
	double dqp=0.0;
	double dfpetotal=0.0, dPetotal=0.0;  
	double dfpu=0.0;
	double dfpsave=0.0;
	double dRhop_tot=0.0, dRhop = 0.0, dRhos=0.0, dRhoc=0.0;
	double dbdp=0.0, dbds=0.0, dQa=0.0;  

	// Code의 공식을 사용할 때.
	BOOL bOneType=TRUE;
	BOOL bBondType=TRUE;
	UINT iTenTndnNum=0;
	UINT iBondNum   =0;
	UINT iUnbondNum =0;
	double dhf=0.0;
	double dfpe_unb_cal = 0.0;
	double dfpe_unb = 0.0;
	double d05fpu = 0.0;
	double dMn_ps=0.0;
	double dMn_beta1 = 0.0;
	double dphiMn_beta1=0.0;  
	 // 철도기준에 사용되는 변수.
	 double dfpy_bond=0.0;
	 double dfpy_unbond=0.0; 
	 double dfps_cal=0.0;
	 double dfps_un_max=0.0;
	 double dfps_un_com=0.0;

	double dIner=(bYaxis==TRUE ? dIyy : dIzz);
	BOOL bDirec = (bPositive ? TRUE : FALSE);
	double dh = (bYaxis==TRUE ? dzDim : dyDim);
	double dHt  = dzDim + dtc + dHh;
	double dHsb = dtc + dHh;
//   if (SectPosD.bCompoSect==TRUE)
//   {
//     dh = dHt;
//   }

	double dLength = (bYaxis==TRUE ? dyDim : dzDim);
	double dbf_top = dyDim;
	double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
	double db   = (bPositive ? dbf_top : dbf_bot);   
	double db_g = db;
	if (SectPosD.bCompoSect==TRUE)
	{
		if (bPositive) db = dBc;
	}

	//0:Pei 1:Apsi 2:dpi 3:epi 4:Epsi_ps 5:fpsi 6:Apsi·fpsi 7:Tpi·epi 8:Pei·epi
	CArray<CString,CString> strTndn; strTndn.RemoveAll();
	CArray<double,double> arfps[9];  
	for( i=0; i<9; i++) arfps[i].RemoveAll();        

	BOOL bPrint = (bRatingPrt)? TRUE :  m_Print.Get_PrintFlag();

	//Assume C.
	dNeuC = (bYaxis==TRUE ? dzDim : dyDim) * 0.5;
	double dCp = 0.0;
	if(bPositive) dCp = (bYaxis==TRUE ? dCpz : dCpy);
	else          dCp = (bYaxis==TRUE ? dzCen : dyCen);
	double dCtemp=0.0;
	int NumberofTendon = arTndnPosi.GetSize();
	UINT iCount = 1;  
	// Flange Height.
	dhf = (bPositive ? dFlgThk[0] : dFlgThk[1]);
	double dhf_c = dhf;
	if (SectPosD.bCompoSect==TRUE)
	{
		if (bPositive) dhf_c = dHsb;
	}


	// Composite 설계시
	CArray<T_SPSC_COOR,T_SPSC_COOR> arOuterTemp;  arOuterTemp.SetSize(1);
	T_SPSC_COOR OuterTemp;
	double daArea_gr=0.0,   daQ_gr=0.0; 
	double da_gr=0.0,    daCent_gr=0.0; 
	double dCc_gr=0.0;  
	
	double daArea_sl=0.0,   daQ_sl=0.0; 
	double da_sl=0.0,    daCent_sl=0.0; 
	double dCc_sl=0.0;  

	double dBeta1_Temp = dBeta1; // 거더기준. 
	//

	// 중립축 결정을 위해서. 
	BOOL bPosiForce=TRUE;
	double dNeuC1 = 0.0;
	double dNeuC2 = 0.0;
	BOOL bStop = FALSE;
			
	//double dCsb  = 0.0;
	//double dC_sb = (dBeta1_s==0.0) ? 0.0 : dtc/*dtc/dBeta1_s*/;
	double dposi_max =0.0;
	if(bIter)
	{    
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> rarRbarPosi;
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> rarTndnPosi;
		do
		{
			bPosiForce = TRUE;
			iTenTndnNum=0;
			iBondNum   =0;
			iUnbondNum =0;
			dfpsiApsi  = 0.0;
			if(bPrint)
			{
				strTndn.RemoveAll();
				for( i=0; i<9; i++) arfps[i].RemoveAll();        
				
				dTpTimesep = 0.0;
			}

			dPetotal=0.0;
			BOOL bCalcCompBlock=TRUE;


			if(SectPosD.bCompoSect==TRUE)
			{
				if(bPositive)
				{
					if(dNeuC < dtc+dHh) 
					{
						dBeta1_Temp = dBeta1_s;
						dBeta1      = dBeta1_s;
					}
				}
				da = dBeta1_Temp *dNeuC;     // 거더기준.
				// **************************************************
				// 거더
				OuterTemp.Initialize();  OuterTemp = arOuter[0];
				arOuterTemp.RemoveAll(); arOuterTemp.Add(OuterTemp);
				double daTemp = (bPositive ? dh-da : da);

				if(CalcSectPSC_QA(arOuterTemp, arInner, bDirec, bYaxis, daTemp, dLength, daArea_gr, daQ_gr))
				{
					daCent_gr = (daArea_gr==0.0 ? 0.0 : daQ_gr/daArea_gr);  // 응력블럭 하부에서 응력블럭 도심까지 거리
					dCc_gr    = 0.85*dfck*daArea_gr;  //dalpha1*dfck*daArea; 
				}

				// slab
				OuterTemp.Initialize();  OuterTemp = arOuter[1];
				arOuterTemp.RemoveAll(); arOuterTemp.Add(OuterTemp);
				if(CalcSectPSC_QA(arOuterTemp, arInner, bDirec, bYaxis, daTemp, dLength, daArea_sl, daQ_sl))
				{
					daCent_sl = (daArea_sl==0.0 ? 0.0 : daQ_sl/daArea_sl);  // 응력블럭 하부에서 응력블럭 도심까지 거리
					daArea_sl *= dnEc;               // by 고희중 GJ 제안.
					dCc_sl    = 0.85*dfck*daArea_sl; //dalpha1*dfck*daArea; 
					//dCc_sl *= dnEc;   
				}
				// **************************************************

				daArea = daArea_gr + daArea_sl;
				dCc = dCc_gr + dCc_sl;
			}
			else
			{
				da = dBeta1*dNeuC;
				double daTemp = (bPositive ? dh-da : da);

				dCc = 0.0;
				if (bCalcCompBlock==TRUE)
				{
					if(CalcSectPSC_QA(arOuter, arInner, bDirec, bYaxis, daTemp, dLength, daArea, daQ))
					{
						daCent = (daArea==0.0 ? 0.0 : daQ/daArea);
						dCc    = dalpha1*dfck*daArea; //
					}
				}
			}
		
			//dNeuC을 기준으로 Rebar(Ac,d) 계산 
			dCtemp = (bPositive ? dNeuC : dh-dNeuC);
			_DGN_TNDN_RBAR rTdRb;
			rTdRb.Initialize();      
			BOOL bGetOk = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb);
			dAptol =(bPositive ? rTdRb.dApttol : rTdRb.dApctol); 
			ddp    =(bPositive ? rTdRb.ddpt    : dh-rTdRb.ddpc);
			if(dAptol==0.0) ddp = 0.0;
			dAsttol=(bPositive ? rTdRb.dAsttol : rTdRb.dAsctol); 
			ddst   =(bPositive ? rTdRb.ddst    : dh-rTdRb.ddsc);
			if(dAsttol==0.0) ddst = 0.0;
			dAsctol=(bPositive ? rTdRb.dAsctol : rTdRb.dAsttol); 
			ddsc   =(bPositive ? rTdRb.ddsc    : dh-rTdRb.ddst);
			if(dAsctol==0.0) ddsc = 0.0;
				
			double dbdp_It = 0.0;
			double dbd_It = 0.0;      
			if(dhf_c>=da)
			{
				dbd_It = db*ddst;
				dRhos  = (dbd_It==0.0 ? 0.0 : dAsttol/dbd_It);
				dRhoc  = (dbd_It==0.0 ? 0.0 : dAsctol/dbd_It);

				dbdp_It = db*ddp;
				dRhop = (dbdp_It==0.0 ? 0.0 : dAptol/dbdp_It);
			}
			else if(dhf_c<da)
			{
				dbd_It = dbw*ddst;
				dRhos  = (dbd_It==0.0 ? 0.0 : dAsttol/dbd_It);
				dRhoc  = (dbd_It==0.0 ? 0.0 : dAsctol/dbd_It);

				dbdp_It = dbw*ddp;
				dRhop = (dbdp_It==0.0 ? 0.0 : dAptol/dbdp_It);
			}

			Get_TndnReBarTransform(bYaxis, bPositive, dNeuC, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);

			dfps=0.0; dMps=0.0; dTp=0.0;
			double dfpu_min = 1.7E308; 
			// Add by Jaeoh ('06.06.15).
			double dPe_Sum=0.0, dMe_Sum=0.0, dep_Sum=0.0;
			for(i=0 ; i<NumberofTendon ; i++)
			{
				_DGN_TNDN_CRC &rTnInf = rarTndnPosi.GetAt(i);
				double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
				if(bPositive && dposi<0.0) 
				{
					dPe_Sum += rTnInf.dArea*rTnInf.dStress[0];
					dMe_Sum += rTnInf.dArea*rTnInf.dStress[0]*dposi;
				}
				else if(!bPositive && dposi>0.0)
				{
					dPe_Sum += rTnInf.dArea*rTnInf.dStress[0];
					dMe_Sum += rTnInf.dArea*rTnInf.dStress[0]*dposi;
				}
				dep_Sum = dPe_Sum==0.0 ? 0.0 : dMe_Sum/dPe_Sum;
			}

			for(i=0 ; i<NumberofTendon ; i++)
			{
				_DGN_TNDN_CRC &rTnInf = rarTndnPosi.GetAt(i);
				double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
				
				if(bPositive && dposi<0.0) 
				{
					double ddpi = dNeuC + fabs(dposi);
					dposi_max = max(dposi_max, ddpi);
					double dPe  = rTnInf.dArea * rTnInf.dStress[0];
					dEpsi_cp = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddpi-dNeuC)/dNeuC);
					dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
					dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe_Sum/(dArea*dEc)*(1.0+(dep_Sum*dposi)/(dIner/dArea));  /**/
					dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
					if(rTnInf.iBondType==0)   dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);
					else if(rTnInf.iBondType==1) 
					{
						dfpy_unbond = rTnInf.dfpy;
						double dfpe_unbi = rTnInf.dStress[0];
						if(m_iCode==KSCE_USD05)
						{
							if(dfpe_unbi>=0.5*rTnInf.dfpu)     dfps = dfpe_unbi + 105.0; 
							else if(dfpe_unbi<0.5*rTnInf.dfpu) dfps = 0.5*rTnInf.dfpu   + 105.0;          
						}
						else if( m_iCode==KSCE_USD10)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 420.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
						else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 400.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
						else ASSERT(0);
					}
					dfps = min(dfps,rTnInf.dfpu);
					dPetotal += rTnInf.dArea*rTnInf.dStress[0];
					dTp += dfps*rTnInf.dArea;
					dMps += dfps*rTnInf.dArea*fabs(dposi); 
					dfpu_min = min(rTnInf.dfpu,dfpu_min);          
					dfpsiApsi  += dfps*rTnInf.dArea;

					if(bPrint)
					{            
						strTndn.Add(rTnInf.strTndnGrup);
						arfps[0].Add(dPe);                // 0:Pei
						arfps[1].Add(rTnInf.dArea);       // 1:Apsi
						arfps[2].Add(ddpi);               // 2:dpi
						arfps[3].Add(fabs(dposi));        // 3:epi
						arfps[4].Add(dEpsi_ps);             // 4:Epsi_ps
										
						double dApsi_fpsi = dfps*rTnInf.dArea;
						double dTpi_epi = dApsi_fpsi*fabs(dposi);
						double dPei_epi = (rTnInf.dArea*rTnInf.dStress[0])*fabs(dposi);
						arfps[5].Add(dfps);       //5:fpsi
						arfps[6].Add(dApsi_fpsi); //6:Apsi·fpsi
						arfps[7].Add(dTpi_epi);   //7:Tpi·epi
						arfps[8].Add(dPei_epi);   //8:Pei·epi
						dTpTimesep += dTpi_epi;
					}

				}
				else if(!bPositive && dposi>0.0)
				{
					double ddpi = dNeuC + fabs(dposi);
					dposi_max = max(dposi_max, ddpi);
					double dPe  = rTnInf.dArea * rTnInf.dStress[0];
					dEpsi_cp = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddpi-dNeuC)/dNeuC);
					dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
					dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe_Sum/(dArea*dEc)*(1.0+(dep_Sum*dposi)/(dIner/dArea));
					dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
					if(rTnInf.iBondType==0)   dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);
					else if(rTnInf.iBondType==1) 
					{
						dfpy_unbond = rTnInf.dfpy;
						double dfpe_unbi = rTnInf.dStress[0];
						if(m_iCode==KSCE_USD05 )
						{
							if(dfpe_unbi>=0.5*rTnInf.dfpu)     dfps = dfpe_unbi + 105.0; 
							else if(dfpe_unbi<0.5*rTnInf.dfpu) dfps = 0.5*rTnInf.dfpu   + 105.0;          
						}
						else if(m_iCode==KSCE_USD10)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 420.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
						else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 400.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
						else ASSERT(0);
					}
					dfps = min(dfps,rTnInf.dfpu);
					dPetotal += rTnInf.dArea*rTnInf.dStress[0];
					dTp += dfps*rTnInf.dArea;
					dMps += dfps*rTnInf.dArea*fabs(dposi);
					dfpu_min = min(rTnInf.dfpu,dfpu_min);
					dfpsiApsi  += dfps*rTnInf.dArea;

					if(bPrint)
					{            
						strTndn.Add(rTnInf.strTndnGrup);
						arfps[0].Add(dPe);                // 0:Pei
						arfps[1].Add(rTnInf.dArea);       // 1:Apsi
						arfps[2].Add(ddpi);               // 2:dpi
						arfps[3].Add(fabs(dposi));        // 3:epi
						arfps[4].Add(dEpsi_ps);           // 4:Epsi_ps            
				
						double dApsi_fpsi = dfps*rTnInf.dArea;
						double dTpi_epi = dApsi_fpsi*fabs(dposi);
						double dPei_epi = (rTnInf.dArea*rTnInf.dStress[0])*fabs(dposi);
						arfps[5].Add(dfps);       //5:fpsi
						arfps[6].Add(dApsi_fpsi); //6:Apsi·fpsi
						arfps[7].Add(dTpi_epi);   //7:Tpi·epi
						arfps[8].Add(dPei_epi);   //8:Pei·epi
						dTpTimesep += dTpi_epi;
					}

				}               
			}
			dfpu = dfpu_min;
		 
			if(dAsttol>0.0) dEpsi_st = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddst-dNeuC)/dNeuC);
			else dEpsi_st=0.0;
			if(dAsctol>0.0) dEpsi_sc = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(dNeuC-ddsc)/dNeuC);
			else dEpsi_sc=0.0;

			dfst_cal = dEpsi_st*dEs;
			dfsc_cal = dEpsi_sc*dEs;        
			dfst = min(dfst_cal, dfy);
			dfsc = min(dfsc_cal, dfy);   
					
			dTst = dAsttol*dfst;
			dCsc = dAsctol*dfsc;
		  //dTp = dAptol*dfps;
			
			if (SectPosD.bCompoSect==TRUE)
			{
				double dComp = dCc;
				double dTens = dTp + dTst;
				double dForce_ratio = dTens==0.0 ? fabs(dComp/cDGN_Zero) : fabs(dComp/dTens);

				dCcDel = dTens - dComp;
				dToler = dForce_ratio-1.0;  //(dCompForce==0.0 ? 0.0 : dCcDel/dCompForce);

				if(fabs(dToler)<dTOL2) break;
				if(iCount>cPSC_ITERLIM-1) break;

				if(dCcDel>=0.0) dNeuC2 = dNeuC; // 인장력이 클경우
				else            dNeuC1 = dNeuC; // 압축력이 클경우
				
				dNeuC = 0.5*(dNeuC1 + dNeuC2);  //Get neutral axis position for next iteration    
			}
			else
			{
				//************************************************
				double dComp = dCc;
				dCcDel = dTp + dTst - dComp;
				dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);

				if(dCcDel<0.0) bPosiForce = FALSE;

				///
				if(iCount==1) dNeuC2 = dNeuC;
				dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
				if(fabs(dToler)<dTOL2) break;
				if(iCount>m_PSC_iITER) break;
			
				if(dCcDel<=0.0) dNeuC2 = dNeuC;
				else            dNeuC1 = dNeuC;
				dNeuC = 0.5*(dNeuC1 + dNeuC2);
				///
				//************************************************
			}

		 
			if(iCount>50 && fabs(dNeuC1-dNeuC2)<0.00001) bStop= TRUE;
			if(fabs(dToler)<dTOL2)                       bStop= TRUE;

//       if(bPosiForce)
//       {
//         dNeuC += 0.3*dh;
//         dNeuC2 = dNeuC;
//         dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
//         if(fabs(dToler)<dTOL2) break;
//         if(iCount>m_PSC_iITER) break;
//       }
//       else
//       {
//         if(iCount==1) dNeuC2 = dNeuC;
//         dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
//         if(fabs(dToler)<dTOL2) break;
//         if(iCount>m_PSC_iITER) break;
//         
//         if(dCcDel<=0.0) dNeuC2 = dNeuC;
//         else            dNeuC1 = dNeuC;
//         dNeuC = 0.5*(dNeuC1 + dNeuC2);
//       }

			++iCount;

		}while(!bStop); // fabs(dToler)>dTOL2
		
		dfpsave = (dAptol==0.0 ? 0.0 : dfpsiApsi/dAptol);
		
		//phi 재계산
		if(m_iCode==KSCE_USD10 || m_iCode==KSCE_RAIL_USD11)
		{
			double ddp = dposi_max; //(dTp==0.0 ? 0.0 : dNeuC + dMps/dTp);
			Get_Phi_Flexure(dNeuC, ddp, dphi);
		}
		//dNeuC에서 dMn..
		if (SectPosD.bCompoSect==TRUE)
		{
			if (bPositive)
			{
				//dMn = dCc_s*(dNeuC-0.5*dtc) + dCc*(dNeuC-(da_g-daCent)) + dMps + dTst*(ddst-dNeuC); 
				dMn = dCc_sl*(dNeuC-da+daCent_sl) + dCc_gr*(dNeuC-da+daCent_gr)  + dMps + dTst*(ddst-dNeuC); 			 
				// test
				double dMn01 = dCc_sl*(dNeuC-da+daCent_sl) + dCc_gr*(dNeuC-da+daCent_gr) ;
				daCent =  (dCc_sl*daCent_sl + dCc_gr*daCent_gr)/(dCc_sl + dCc_gr);
				double dMn02 = (dCc_sl + dCc_gr)*(dNeuC-da+daCent);
			}
			else
			{
				dMn = dCc*(dNeuC-(da-daCent_gr)) + dMps + dTst*(ddst-dNeuC); 
				daCent  = daCent_gr;
			}
			
		}
		else
		{
			dMn = dCc*(dNeuC-(da-daCent)) + dMps + dTst*(ddst-dNeuC); 
		}
		
		dMn_Cal = dMn;   
		dphiMn_Cal = dphi*dMn;    

		double dAp = dAptol; // 인장측 긴장재만. 
		dApw = 0.0;    

		if(m_iCode==KSCE_USD05 || m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
		{ // KSCE_USD10은 제외

			if(dhf_c>=da) // Rectangle.
			{
				dqp = (dfck==0.0 ? 0.0: dRhop*dfpsave/dfck);  //dfpsave
			}
			else if(dhf_c<da)
			{
				dApf = (dfpsave==0.0 ? 0.0 : dalpha1*dfck*(db-dbw)*dhf_c/dfpsave);
				if(dAsttol>0.0)
				{
					dApw = (dfpsave==0.0 ? 0.0 : dAp + (dAsttol*dfy/dfpsave) - dApf);
				}
				else
				{
					dApw = dAp - dApf;
				}
				
				dqp = (dbw*ddp ==0.0 || dfck==0.0 ? 0.0 : dApw/(dbw*ddp)*dfpsave/dfck);
			}
			else ASSERT(0);
			
			if(da<=dhf_c)  // 사각형 단면.
			{
				dqp = dRhop*dfpsave/dfck;

				if (SectPosD.bCompoSect==TRUE)
				{
					if (da<dHsb && bPositive)
					{
						dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1_s,2))*dfck_s*dBc*pow(ddp,2);
						
						if(dqp>d036beta1_s)
						{        
							dMn = min(dMn, dMn_beta1);
						}
					}
					else
					{
						dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*db*pow(ddp,2);
						
						if(dqp>d036beta1)
						{        
							dMn = min(dMn, dMn_beta1);
						}
					}

				}
				else
				{
					dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*db*pow(ddp,2);
					
					if(dqp>d036beta1)
					{        
						dMn = min(dMn, dMn_beta1);
					}
				}
			}
			else if(da>dhf_c)  // 플랜지를 갖는 단면.
			{
				if (SectPosD.bCompoSect==TRUE && bPositive)
				{
					dqp = (dbw*ddp*dfck==0.0 ? 0.0 : dApw*dfpsave/(dbw*ddp*dfck));
					dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*dbw*pow(ddp,2)
						+dalpha1*dfck_s*(dBc-dbw)*dhf*(ddp-0.5*dtc)
						+dalpha1*dfck*(db_g-dbw)*dhf*(ddp-0.5*dhf);
					if(dqp>d036beta1)
					{        
						dMn = min(dMn, dMn_beta1);
					}
				}
				else
				{
					dqp = (dbw*ddp*dfck==0.0 ? 0.0 : dApw*dfpsave/(dbw*ddp*dfck));
					dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*dbw*pow(ddp,2)
						+dalpha1*dfck*(db-dbw)*dhf*(ddp-0.5*dhf);
					if(dqp>d036beta1)
					{        
						dMn = min(dMn, dMn_beta1);
					}

				}
			}
		}

		dphiMn = dphi*dMn;
		dphiMn_beta1 = dphi * dMn_beta1;    

	}
	else
	{
		//도로교설계기준 KSCE-USD05

		// 등가 응력 사각형 깊이 결정
			 
		dNeuC = 0.5*dh;  // 초기가정 
		double dCp = 0.0;
		if(bPositive) dCp = (bYaxis==TRUE ? dCpz : dCpy);
		else          dCp = (bYaxis==TRUE ? dzCen : dyCen);
		double dCtemp=0.0;
						
		BOOL bCalcOK;    
		double dTps=0.0, dCps=0.0;   
		
		double dfpu_bond=0.0; 
		double dfpu_unbond=0.0;    
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> rarRbarPosi;
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> rarTndnPosi;

		double dMn_co=0.0;    
		double dMn_st=0.0;
		double dMn_cf=0.0;
		
		double ddp_max=0.0; 
		do
		{
			bPosiForce = TRUE; 
			dTst=0.0, dTps=0.0, dCsc=0.0, dCps=0.0;
			da = dBeta1*dNeuC;
			//중립축을 기준으로 Tendon dy,dz를 수정.
			dCtemp = (bPositive ? dNeuC : dh-dNeuC);
			_DGN_TNDN_RBAR rTdRb;
			rTdRb.Initialize();      
			bCalcOK = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb);
			if(bPositive)  // 정모멘트 
			{
				dApt=rTdRb.dApttol; ddp  = (dApt==0.0 ? 0.0 : rTdRb.ddpt);
				dApc=rTdRb.dApctol;
				dAst=rTdRb.dAsttol; ddst = (dAst==0.0 ? 0.0 : rTdRb.ddst);
				dAsc=rTdRb.dAsctol; ddsc = (dAsc==0.0 ? 0.0 : rTdRb.ddsc);

				ddp_max = rTdRb.ddpt_max;
			}
			else
			{
				dApt=rTdRb.dApctol; ddp  = (dApt==0.0 ? 0.0 : dh - rTdRb.ddpc);
				dApc=rTdRb.dApttol; 
				dAst=rTdRb.dAsctol; ddst = (dAst==0.0 ? 0.0 : dh - rTdRb.ddsc);
				dAsc=rTdRb.dAsttol; ddsc = (dAsc==0.0 ? 0.0 : dh - rTdRb.ddst);

				ddp_max = rTdRb.ddpc_max;
			}

			double dbdp_It = 0.0;
			double dbd_It = 0.0;
			
			if(dhf>=da)
			{
				dbd_It = db*ddst;
				dRhos  = (dbd_It==0.0 ? 0.0 : dAst/dbd_It);
				dRhoc  = (dbd_It==0.0 ? 0.0 : dAsc/dbd_It);

				dbdp_It = db*ddp;
				dRhop = (dbdp_It==0.0 ? 0.0 : dApt/dbdp_It);
			}
			else if(dhf<da)
			{
				dbd_It = dbw*ddst;
				dRhos  = (dbd_It==0.0 ? 0.0 : dAst/dbd_It);
				dRhoc  = (dbd_It==0.0 ? 0.0 : dAsc/dbd_It);

				dbdp_It = dbw*ddp;
				dRhop = (dbdp_It==0.0 ? 0.0 : dApt/dbdp_It);
			}

			bCalcOK = Get_TndnReBarTransform(bYaxis, bPositive, dNeuC, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);

			// Tendon.
			dPetotal    = 0.0;
			dfpu_bond   = 1.0/m_dZero;  dfpu_unbond = 1.0/m_dZero;
			dfpy_bond   = 1.0/m_dZero;  dfpy_unbond = 1.0/m_dZero;
			double dfpu_bond_sum   = 0.0;
			double dfpu_unbond_sum = 0.0;
			double dfpu_sum = 0.0;
			double dfpe_sum_un = 0.0; 
			
			iTenTndnNum=0;
			iBondNum   =0;
			iUnbondNum =0;

			for(i=0; i<rarTndnPosi.GetSize(); i++)
			{
				_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
				double dfpui = Tndn.dfpu;
				if(bPositive)
				{
					// Ap
					if(Tndn.dz<0.0)
					{
						iTenTndnNum++;
						double dfpui = Tndn.dfpu;
						double dfpyi = Tndn.dfpy;
						if(Tndn.iBondType==0) 
						{
							iBondNum++;
							dfpu_bond = min(dfpu_bond, dfpui);  // 한요소에 Tendon Material 여러가지 이면 최소.
							dfpy_bond = min(dfpy_bond, dfpyi);
						}
						else if(Tndn.iBondType==1) 
						{
							iUnbondNum++;
							dfpu_unbond = min(dfpu_unbond, dfpui);
							dfpy_unbond = min(dfpy_unbond, dfpyi);
							double dfpei = Tndn.dStress[0];
							dfpe_sum_un += dfpei; //!/ x방향이 아니라 합력으로?.
						}
						dPetotal += Tndn.dArea*Tndn.dStress[0];
					}
				}
				else if(!bPositive)
				{
					// Ap
					if(Tndn.dz>0.0)
					{
						iTenTndnNum++;
						double dfpui = Tndn.dfpu;
						double dfpyi = Tndn.dfpy;
						if(Tndn.iBondType==0) 
						{
							iBondNum++;
							dfpu_bond = min(dfpu_bond, dfpui);
							dfpy_bond = min(dfpy_bond, dfpyi);
						}
						else if(Tndn.iBondType==1) 
						{
							iUnbondNum++;
							dfpu_unbond = min(dfpu_unbond, dfpui);
							dfpy_unbond = min(dfpy_unbond, dfpyi);
							double dfpei = Tndn.dStress[0];
							//dfpei = max(dfpei, 0.5*dfpui);
							dfpe_sum_un += dfpei;     //!/ 
						}
						
						dPetotal += Tndn.dArea*Tndn.dStress[0];
					}
				}
			}

			if(iBondNum>0 && iUnbondNum==0)
			{
				bBondType = TRUE;
				bOneType  = TRUE;
			}
			else if(iBondNum==0 && iUnbondNum>0)
			{
				bBondType = FALSE;
				bOneType  = TRUE;
			}
			else if(iBondNum>0 && iUnbondNum>0)
			{
				bBondType = FALSE;
				bOneType  = FALSE;
			}      

			dTps = 0.0;
			if(bOneType)
			{        
				if(bBondType)
				{
					dfpu = dfpu_bond; 
					if(m_iCode==KSCE_USD05)
					{
						dfps = (ddp==0.0 ? 0.0 : dfpu*(1.0-drp/dBeta1*(dRhop*dfpu/dfck+ddst/ddp*(dRhos*dfy/dfck))));
					}
					else if(m_iCode==KSCE_USD10 || m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
					{
						dfps_temp_cal = ddp==0.0 ? 0.0 : dRhop*dfpu/dfck+ddst/ddp*(dRhos*dfy/dfck-dRhoc*dfy/dfck);
						dfps_temp     = ddp==0.0 ? 0.0 : max(dfps_temp_cal, 0.17);
						dfps = ddp==0.0 ? 0.0 : dfpu*(1.0-drp/dBeta1*dfps_temp);
					} 
					else ASSERT(0);
				}
				else
				{
					dfpu = dfpu_unbond; 
					dfpe_unb_cal = (iUnbondNum==0 ? 0.0 : dfpe_sum_un/iUnbondNum);          
					dfpe_unb = max(dfpe_unb_cal, 0.5*dfpu);
					if(m_iCode==KSCE_USD05 )
					{
						dfps = dfpe_unb + 105.0;
					}
					else if(m_iCode==KSCE_USD10 )
					{
						// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
						dfps_cal = (dRhop==0.0 ? dfpe_unb_cal + 70.0 : dfpe_unb_cal + 70.0 + dfck/(100.0*dRhop));  
						dfps_un_com = dfpe_unb_cal + 420.0;
						dfps_un_max = min(dfpy_unbond, dfps_un_com);
						dfps = min(dfps_cal, dfps_un_max);
					}
					else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
					{
						// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
						dfps_cal = (dRhop==0.0 ? dfpe_unb_cal + 70.0 : dfpe_unb_cal + 70.0 + dfck/(100.0*dRhop));  
						dfps_un_com = dfpe_unb_cal + 400.0;
						dfps_un_max = min(dfpy_unbond, dfps_un_com);
						dfps = min(dfps_cal, dfps_un_max);
					}
				}
				dfps = min(dfps,dfpu);

				d05fpu = 0.5*dfpu;
				if(dhf>=da)
				{
					dTps = dApt*dfps;
				}
				else if(dhf<da)
				{
					dApf = (dfps==0.0 ? 0.0 : dalpha1*dfck*(db-dbw)*dhf/dfps);
					if(m_iCode==KSCE_USD05 ||m_iCode==KSCE_USD10)
					{
						if(dAst>0.0) { dApw = (dfps==0.0 ? 0.0 : dApt + (dAst*dfy/dfps) - dApf); }
						else         { dApw = dApt - dApf; }
					}
					else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
						dApw = dApt - dApf;
				
					dTps = dApt*dfps;
					//dTps = dApw*dfps;
				}
			}
			else if(!bOneType)
			{
				strTndn.RemoveAll();
				for( i=0; i<9; i++)
					arfps[i].RemoveAll();

				double dfps_bond = 0.0;
				double dfps_unbond = 0.0;
				double dfps_unbond_sum=0.0;
				dfpu = dfpu_bond;
				if(m_iCode==KSCE_USD05)
				{
					dfps_bond = (ddp==0.0 ? 0.0 : dfpu*(1.0-drp/dBeta1*(dRhop*dfpu/dfck+ddst/ddp*(dRhos*dfy/dfck))));
				}          
				else if(m_iCode==KSCE_USD10 || m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
				{
					double dfps_bond_temp_cal = (ddp==0.0) ? 0.0 : dRhop*dfpu/dfck+ddst/ddp*(dRhos*dfy/dfck-dRhoc*dfy/dfck);
					double dfps_bond_temp     = (ddp==0.0) ? 0.0 : max(dfps_bond_temp_cal, 0.17);
					dfps_bond = ddp==0.0 ? 0.0 : dfpu*(1.0-drp/dBeta1*dfps_bond_temp);
				}
				else ASSERT(0);
					
				for(i=0; i<rarTndnPosi.GetSize(); i++)
				{
					_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
					double dfpui = Tndn.dfpu;
					double dApsi = Tndn.dArea;
					double ddpi = 0.0;
					if(bPositive)
					{            
						if(Tndn.dz<0.0)
						{
							ddpi = dNeuC + fabs(Tndn.dz);
							strTndn.Add(Tndn.strTndnGrup);
							arfps[0].Add(dApsi);          //0:Apsi
							arfps[1].Add(dfpui);          //1:fpui              
							if(Tndn.iBondType==0) 
							{
								dTps += dApsi*dfps_bond;                
								arfps[2].Add(dfps_bond);    //2:fps_bond
								arfps[3].Add(ddpi);         //3:dpi
								arfps[4].Add(0.0);          //4:Type 0.0=bond
							}
							else if(Tndn.iBondType==1) 
							{
								double dfpe_unbond = Tndn.dStress[0]; //!/                
								double dfpsi=0.0;
								if(m_iCode==KSCE_USD05)
								{
									dfpe_unbond = max(dfpe_unbond, 0.5*dfpui);
									dfpsi = dfpe_unbond + 105.0;
								}
								else if(m_iCode==KSCE_USD10)
								{
									dfpsi = (dRhop==0.0 ? dfpe_unbond + 70.0 : dfpe_unbond + 70.0 + dfck/(100.0*dRhop));
									dfps_un_com = dfpe_unbond + 420.0;
									dfps_un_max = min(dfpy_unbond, dfps_un_com);
									dfpsi = min(dfpsi, dfps_un_max);
								}
								else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
								{
									dfpsi = (dRhop==0.0 ? dfpe_unbond + 70.0 : dfpe_unbond + 70.0 + dfck/(100.0*dRhop));
									dfps_un_com = dfpe_unbond + 400.0;
									dfps_un_max = min(dfpy_unbond, dfps_un_com);
									dfpsi = min(dfpsi, dfps_un_max);
								}
								dfpsi = min(dfpsi,dfpui);
								dTps += dApsi*dfpsi;
								arfps[2].Add(dfpsi);      //2:fps_bond
								arfps[3].Add(ddpi);    //3:dpi
								arfps[4].Add(1.0);        //4:Type 0 보다 크면 unbond
							}
						}
					}
					else if(!bPositive)
					{
						// Ap
						if(Tndn.dz>0.0)
						{
							ddpi = dNeuC + fabs(Tndn.dz);
							strTndn.Add(Tndn.strTndnGrup);
							arfps[0].Add(dApsi);          //0:Apsi
							arfps[1].Add(dfpui);          //1:fpui
							if(Tndn.iBondType==0) 
							{
								dTps += dApsi*dfps_bond;
								arfps[2].Add(dfps_bond);    //2:fps_bond
								arfps[3].Add(ddpi);         //3:dpi
								arfps[4].Add(0.0);          //4:Type 0.0=bond
							}
							else if(Tndn.iBondType==1) 
							{
								double dfpe_unbond = Tndn.dStress[0]; //!/                
								double dfpsi=0.0;
								if(m_iCode==KSCE_USD05)
								{
									dfpe_unbond = max(dfpe_unbond, 0.5*dfpui);
									dfpsi = dfpe_unbond + 105.0;
								}
								else if(m_iCode==KSCE_USD10)
								{
									dfpsi = (dRhop==0.0 ? dfpe_unbond + 70.0 : dfpe_unbond + 70.0 + dfck/(100.0*dRhop));
									dfps_un_com = dfpe_unbond + 420.0;
									dfps_un_max = min(dfpy_unbond, dfps_un_com);
									dfpsi = min(dfpsi, dfps_un_max);
								}
								else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
								{
									dfpsi = (dRhop==0.0 ? dfpe_unbond + 70.0 : dfpe_unbond + 70.0 + dfck/(100.0*dRhop));
									dfps_un_com = dfpe_unbond + 400.0;
									dfps_un_max = min(dfpy_unbond, dfps_un_com);
									dfpsi = min(dfpsi, dfps_un_max);
								}
								dfpsi = min(dfpsi,dfpui);
								dfps_unbond_sum += dfpsi;
								dTps += dApsi*dfpsi;
								arfps[2].Add(dfpsi);      //2:fps_bond
								arfps[3].Add(ddpi);       //3:dpi
								arfps[4].Add(1.0);        //4:Type 0 보다 크면 unbond
							}
						}
					}
				} // for
				dfps_unbond = (iUnbondNum==0.0 ? 0.0 : dfps_unbond_sum/iUnbondNum);
				dfps = (dfps_bond + dfps_unbond) / 2.0; // if(!OneType) fps는 평균으로 한다. 
				dTps = dfps*dApt;
			} // !bOneType

			double dCc=0.0;
			if(!bOneType)
			{
				if(da<=dhf)
					dCc = dalpha1*dfck*db*da;
				else if(da>dhf)
				{     
					if(da<=dh)     dCc = dalpha1*dfck*(dbw*da + (db-dbw)*dhf);
					else if(da>dh) dCc = dalpha1*dfck*(dbw*dh + (db-dbw)*dhf);
				}
			}
			else if(bOneType)
			{
				if(da<=dhf)
					dCc = dalpha1*dfck*db*da;
				else if(da>dhf)
				{     
					if(da<=dh)     dCc = dalpha1*dfck*(dbw*da + (db-dbw)*dhf);
					else if(da>dh) dCc = dalpha1*dfck*(dbw*dh + (db-dbw)*dhf);

					//if(da<=dh)     dCc = dalpha1*dfck*(dbw*da); // + (db-dbw)*dhf);
					//else if(da>dh) dCc = dalpha1*dfck*(dbw*dh); // + (db-dbw)*dhf);
				}
			}
	
			dTst = (m_iCode==KSCE_RAIL_USD04 ? 0.0 : dAst*dfy);
			double dTens = dTst + dTps; //(da<=dhf ? dTst + dTps : dTps);

			double dComp = dCc;
			double dCcDel = dTens - dComp;

			if(dCcDel<0.0) bPosiForce = FALSE;

			///
			if(iCount==1) dNeuC2 = dNeuC;
			dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
			if(fabs(dToler)<dTOL2) break;
			if(iCount>m_PSC_iITER) break;

			if(dCcDel<=0.0) dNeuC2 = dNeuC;
			else            dNeuC1 = dNeuC;
			dNeuC = 0.5*(dNeuC1 + dNeuC2);
			///

			if(iCount>50 && fabs(dNeuC1-dNeuC2)<0.00001) bStop= TRUE;
			if(fabs(dToler)<dTOL2)                       bStop= TRUE;

//       if(bPosiForce) // Tens > Comp
//       {
//         //dNeuC += 0.3*dh;
//         dNeuC += 0.1*dNeuC;
//         dNeuC2 = dNeuC;
//         dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
//         if(fabs(dToler)<dTOL2) break;
//         if(iCount>m_PSC_iITER) break;
//       }
//       else  // Tens < Comp
//       {
//         
//         if(iCount==1) dNeuC2 = dNeuC;
//         dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
//         if(fabs(dToler)<dTOL2) break;
//         if(iCount>m_PSC_iITER) break;
//         
//         if(dCcDel<=0.0) dNeuC2 = dNeuC;
//         else            dNeuC1 = dNeuC;
//         dNeuC = 0.5*(dNeuC1 + dNeuC2);
//         
//         /*
//         dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
//         if(fabs(dToler)<dTOL2) break;
//         //if(iCount>m_PSC_iITER) break;
//         dNeuC -= 1.0;
//         if(iCount>5000) break;
//         */
//       }

			++iCount;
	
		}while(!bStop); // fabs(dToler)>dTOL2 || 

		// 휨강도 계산.
		dApf = (dfps==0.0 ? 0.0 : dalpha1*dfck*(db-dbw)*dhf/dfps);
		if(m_iCode==KSCE_USD05 || m_iCode==KSCE_USD10 )
		{
			if(dAst>0.0) { dApw = (dfps==0.0 ? 0.0 : dApt + (dAst*dfy/dfps) - dApf); }
			else         { dApw = dApt - dApf; }
		}
		else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
			dApw = dApt - dApf;
 
		// Code로 계산할 경우 Apw<0 이면 상세계산으로 한 번 더 계산한다. 
		// 상세로 계산할 경우 Apw<0 이면 다시 계산하지 않음. 
		if(!bIter && iCount>m_PSC_iITER)
		{
			if(!Get_FlexureStrength(bYaxis, bPosiI, TRUE, bPositive, dPhib, MatlElem, SectPosD, SpscSect, arOuter, arInner, SpscgSect, drp, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcrKsce, /*BdcrDetail,*/ pXL, TRUE, bRatingPrt)) return FALSE;

			return TRUE;
		}

		if(!bOneType) 
		{    
			for(i=0; i<rarTndnPosi.GetSize(); i++)
			{          
				_DGN_TNDN_CRC Tndn = rarTndnPosi.GetAt(i);
				double dfpui = Tndn.dfpu;
				double dApsi = Tndn.dArea;
				double dpi   = 0.0;
				if(bPositive)
				{
					// Ap
					if(Tndn.dz<0.0)
					{
						iTenTndnNum++;            
						if(Tndn.iBondType==0) 
						{
							double dfp = (i+1 > arfps[2].GetSize())? dfps : arfps[2].GetAt(i); // dfps
							dMn_ps += dApsi*dfp*fabs(Tndn.dz);
						}
						else if(Tndn.iBondType==1) 
						{
//              double dfpe_unbond = Tndn.dStress[0]; //!/
//               dfpe_unbond = max(dfpe_unbond, 0.5*dfpui);
//               double dfpsi=0.0;
//               if(m_iCode==KSCE_USD05)
//                 dfpsi = dfpe_unbond + 105.0;
//               else if(m_iCode==KSCE_RAIL_USD04 ||m_iCode==KSCE_RAIL_USD11 || m_iCode==KSCE_USD10)
//                 dfpsi = (dRhop==0.0 ? dfpe_unbond + 70.0 : dfpe_unbond + 70.0 + dfck/(100.0*dRhop));
// 
//               dfpsi = min(dfpsi,dfpui);

							double dfpe_unbond = Tndn.dStress[0]; //!/                
							double dfpsi=0.0;
							if(m_iCode==KSCE_USD05)
							{
								dfpe_unbond = max(dfpe_unbond, 0.5*dfpui);
								dfpsi = dfpe_unbond + 105.0;
							}
							else if(m_iCode==KSCE_USD10)
							{
								dfpsi = (dRhop==0.0 ? dfpe_unbond + 70.0 : dfpe_unbond + 70.0 + dfck/(100.0*dRhop));
								dfps_un_com = dfpe_unbond + 420.0;
								dfps_un_max = min(dfpy_unbond, dfps_un_com);
								dfpsi = min(dfpsi, dfps_un_max);
							}
							else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
							{
								dfpsi = (dRhop==0.0 ? dfpe_unbond + 70.0 : dfpe_unbond + 70.0 + dfck/(100.0*dRhop));
								dfps_un_com = dfpe_unbond + 400.0;
								dfps_un_max = min(dfpy_unbond, dfps_un_com);
								dfpsi = min(dfpsi, dfps_un_max);
							}
							dfpsi = min(dfpsi,dfpui);

							dMn_ps += dApsi*dfpsi*fabs(Tndn.dz);
						}

					}
				}
				else if(!bPositive)
				{
					// Ap
					if(Tndn.dz>0.0)
					{
						iTenTndnNum++;
						if(Tndn.iBondType==0) 
						{
							double dfp =  (i+1 > arfps[2].GetSize())? dfps : arfps[2].GetAt(i); // dfps
							dMn_ps += dApsi*dfp*fabs(Tndn.dz); 
						}
						else if(Tndn.iBondType==1) 
						{
							//
//               double dfpe_unbond = Tndn.dStress[0]; //!/
//               dfpe_unbond = max(dfpe_unbond, 0.5*dfpui);
//               double dfpsi=0.0;
//               if(m_iCode==KSCE_USD05)
//                 dfpsi = dfpe_unbond + 105.0;
//               else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11  ||m_iCode==KSCE_USD10)
//                 dfpsi = (dRhop==0.0 ? dfpe_unbond + 70.0 : dfpe_unbond + 70.0 + dfck/(100.0*dRhop));
// 
//               dfpsi = min(dfpsi,dfpui);
							//

							double dfpe_unbond = Tndn.dStress[0]; //!/                
							double dfpsi=0.0;
							if(m_iCode==KSCE_USD05)
							{
								dfpe_unbond = max(dfpe_unbond, 0.5*dfpui);
								dfpsi = dfpe_unbond + 105.0;
							}
							else if(m_iCode==KSCE_USD10)
							{
								dfpsi = (dRhop==0.0 ? dfpe_unbond + 70.0 : dfpe_unbond + 70.0 + dfck/(100.0*dRhop));
								dfps_un_com = dfpe_unbond + 420.0;
								dfps_un_max = min(dfpy_unbond, dfps_un_com);
								dfpsi = min(dfpsi, dfps_un_max);
							}
							else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
							{
								dfpsi = (dRhop==0.0 ? dfpe_unbond + 70.0 : dfpe_unbond + 70.0 + dfck/(100.0*dRhop));
								dfps_un_com = dfpe_unbond + 400.0;
								dfps_un_max = min(dfpy_unbond, dfps_un_com);
								dfpsi = min(dfpsi, dfps_un_max);
							}
							dfpsi = min(dfpsi,dfpui);


							dMn_ps += dApsi*dfpsi*fabs(Tndn.dz);
						}
				
					}
				}
			}
		}
		
		if(da<=dhf)  // 사각형 단면.
		{
			dqp = dRhop*dfps/dfck;
			dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*db*pow(ddp,2);
			
			if(bOneType)
			{
				if(m_iCode==KSCE_USD05 || m_iCode==KSCE_USD10)
				{
					dMn_ps = (ddp==0.0 ? 0.0 : dApt*dfps*ddp*(1.0-0.59*(dRhop*dfps/dfck + ddst/ddp*dRhos*dfy/dfck)));
					dMn_st = (ddst==0.0 ? 0.0 : dAst*dfy*ddst*(1.0-0.59*(ddp/ddst*dRhop*dfps/dfck + dRhos*dfy/dfck)));      
				}
				else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
				{
					dMn_ps = (ddp==0.0 ? 0.0 : dApt*dfps*ddp*(1.0-0.59*(dRhop*dfps/dfck)));
					dMn_st = 0.0;
				}

				dMn = dMn_ps + dMn_st;
			}
			else if(!bOneType)
			{
				// 중립축을 기준으로 
				if(m_iCode==KSCE_USD05 || m_iCode==KSCE_USD10)
				{
					dMn_co = dalpha1*dfck*da*db*(dNeuC-da/2.);
					dMn_st = dAst*dfy*(ddst-dNeuC);
				}
				else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
				{
					dMn_co = dalpha1*dfck*da*db*(dNeuC-da/2.);
					dMn_st = 0.0;
				}
				dMn = dMn_co + dMn_ps + dMn_st;  
			}

			// 철근만 있을 경우.
			if(dApt==0 && dAst>0.0)
			{
				dMn = dalpha1*dfck*da*db*(ddst-da/2.);
			}

		}
		else if(da>dhf)  // 플랜지를 갖는 단면.
		{
			dqp = (dbw*ddp*dfck==0.0 ? 0.0 : dApw*dfps/(dbw*ddp*dfck));
			dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*dbw*pow(ddp,2)
													+dalpha1*dfck*(db-dbw)*dhf*(ddp-0.5*dhf);
			if(bOneType)
			{
				if(m_iCode==KSCE_USD05 || m_iCode==KSCE_USD10)
				{
					dMn_ps = (ddp==0.0 ? 0.0 : dApw*dfps*ddp*(1.0-0.59*(dApw/(dbw*ddp)*dfps/dfck)));
					dMn_st = dAst*dfy*(ddst-ddp);
					dMn_cf = dalpha1*dfck*(db-dbw)*dhf*(ddp-0.5*dhf);
					dMn = dMn_ps + dMn_st + dMn_cf;
				}
				else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
				{
					dMn = dApw*dfps*(ddp-da/2.)+dalpha1*dfck*(db-dbw)*dhf*(ddp-0.5*dhf);
				}
			}
			else if(!bOneType)
			{
				if(m_iCode==KSCE_USD05 || m_iCode==KSCE_USD10 )
				{
					dMn_co = dalpha1*dfck*da*dbw*(dNeuC-da/2.) + dalpha1*dfck*(db-dbw)*dhf*(dNeuC-dhf/2.);
					dMn_st = dAst*dfy*(ddst-dNeuC);
					dMn = dMn_co + dMn_ps + dMn_st;
				}
				else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
				{
					dMn_co = dalpha1*dfck*da*dbw*(dNeuC-da/2.) + dalpha1*dfck*(db-dbw)*dhf*(dNeuC-dhf/2.);
					dMn_st = 0.0;
					dMn = dMn_co + dMn_ps + dMn_st;
				}
			}      

			// 철근만 있을 경우.
			if(dApt==0 && dAst>0.0)
			{
				dMn = dalpha1*dfck*da*dbw*(ddst-da/2.) + dalpha1*dfck*(db-dbw)*dhf*(ddst-dhf/2.);        
			}
		}

		//double ddp = (dTp==0.0 ? 0.0 : dNeuC + dMps/dTp);
		//phi 재계산
		if(m_iCode==KSCE_USD10 || m_iCode==KSCE_RAIL_USD11)
		{
			Get_Phi_Flexure(dNeuC, ddp_max, dphi);
		}

		dMn_Cal = dMn;
		dphiMn_Cal = dphi * dMn;

		if(m_iCode==KSCE_USD05 || m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
		{ // KSCE_USD10은 제외
			if(dqp>d036beta1)
			{
				dMn = min(dMn, dMn_beta1);
			}
		}

		dphiMn_beta1 = dphi * dMn_beta1;
		dphiMn  = dphi * dMn;
		dAptol  = dApt;
		dAsttol = dAst;
		dAsctol = dAsc;
		dfpsave = dfps;

	}  //도로교설계기준 KSCE-USD05
	
	double dSc=0.0;
	if(bYaxis) 
	{
		if(bPositive) dSc = (dzCen_g==0.0 ? 0.0 : dIyy_g/dzCen_g);
		else          dSc = (dzDim_g-dzCen_g==0.0 ? 0.0 : dIyy_g/(dzDim_g-dzCen_g));    
	}
	else
	{
		if(bPositive) dSc = (dyCen_g==0.0 ? 0.0 : dIzz_g/dyCen_g);
		else          dSc = (dyDim_g-dyCen_g==0.0 ? 0.0 : dIzz_g/(dyDim_g-dyCen_g));    
	}
	double dCp_g = 0.0;
	if(bPositive) dCp_g = (bYaxis==TRUE ? dCpz_g : dCpy_g);
	else          dCp_g = (bYaxis==TRUE ? dzCen_g : dyCen_g);
	double dyt_g = dh-dCp_g;
	
	double dyt = dh-dCp;
	
	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnConCent;
	arTndnConCent.RemoveAll();
	for(i=0 ; i<NumberofTendon ; i++)
	{
		_DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
		_DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
		rTnfrC = rTndn;
		double dyposi = (bYaxis==TRUE ? rTndn.dz : rTndn.dy);
		dgap =  (bPositive ? dCp_g-dCp : dCp-dCp_g); 
		if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
		else       rTnfrC.dy = rTndn.dy+dgap;
		arTndnConCent.Add(rTnfrC);
	}

	// fpcc Calculation.
	double dAps=0.0, dPeep=0.0, dPe_t=0.0, dVp=0.0, dfpe_ave=0.0;
	Get_fpeData(bYaxis, dMu, dAg, dIner_g, dyt_g, arTndnConCent, dAps, dPeep, dPe_t, dVp, dfpe_ave);
	
	double dRatM = (dphiMn==0.0 ? 0.0 : fabs(dMu)/dphiMn);
	double dfru=0.63*sqrt(dfck);
	double dMcr=(dfru+dfpe_ave)*dSc;
	double d12Mcr=1.2*dMcr;
	double d43Muy = 4./3.*dMu;

	if(m_iCode==KSCE_USD05 || m_iCode==KSCE_USD10)
	{
		if(ddp==0.0 && ddst==0.0)
		{
			bPrint = FALSE;
			BdcrKsce.bCHK = FALSE; // 텐던 & 철근이 없으면 FALSE
		}
	}
	else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
	{
		if(ddp==0.0)
		{
			bPrint = FALSE;
			BdcrKsce.bCHK = FALSE; // 텐던 & 철근이 없으면 FALSE
		}
	}
	else ASSERT(0);

	BdcrKsce.dMuy   = dMu;	          //면내 계수모멘트		
	BdcrKsce.dfpe   = dfpe_ave;	    //손실이 일어난 후 강재의 유효프리스트레스	
	BdcrKsce.dAps   = dAptol;	        //인장측 PS강재의 단면적	
	BdcrKsce.dAst   = dAsttol;	      //인장측 철근의 단면적		
	BdcrKsce.dpst   = dRhos;	        //인장측 철근비			
	BdcrKsce.dIy    = dIner_g;	      //강축 단면 2차모멘트 		
	BdcrKsce.db	    = db;		          //단면폭
	BdcrKsce.dh	    = dzDim;		      //단면춤
	BdcrKsce.ddst   = ddst;	          //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		
	BdcrKsce.ddps   = ddp;	          //인장측 PS강재의 유효춤	
	BdcrKsce.dfps   = (bIter ? dfpsave : dfps); //계수하중 하에서 PS강재 평균응력	
	BdcrKsce.da     = da;		          //등가 직사각형 응력블럭의 깊이	
	BdcrKsce.dpMny  = dphiMn;	        //면내 설계휨모멘트	
	BdcrKsce.dRatM  = dRatM;	        //면내 휨강도 안전율	
	BdcrKsce.dqp    = dqp;		        //강재지수			
	BdcrKsce.d036Beta1 = d036beta1;   //최대 PS강재량 제한값	
	BdcrKsce.d12Mcr = d12Mcr;	        //최소 강재 균열모멘트	
	BdcrKsce.d43Muy = d43Muy;	        //
	//계산시 사용되는 중간값
	BdcrKsce.dPe    = dPetotal;		    //유효긴장력			
	BdcrKsce.dAsc   = dAsctol;	      //압축측 철근의 단면적	
	BdcrKsce.ddsc   = ddsc;	          //콘크리트압축 연단에서 압축측 철근 중심까지의 거리		
	BdcrKsce.dpsc   = dRhoc;	        //압축측 철근비		
	BdcrKsce.dyt    = dyt_g;		      //총단면적의 중립축에서 인장 연단까지의 거리	
	BdcrKsce.deps   = ddp-dCp_g;	    //PS강재의 도심의 편심량 
	BdcrKsce.dAc    = dAg; 		      //콘크리트의 전면적     
	BdcrKsce.dfy    = dfy;            //인장철근의 설계 항복강도 	
	BdcrKsce.dfck   = dfck;	          //콘크리트 설계기준강도 		
	BdcrKsce.dbeta1 = dBeta1;         //0.85fck의 압축응력이 균등하게 분포하는 직사각형 블록의 깊이 a의 비                
	BdcrKsce.drp    = drp; 	          //PS강재의 종류에 따른 계수	
	BdcrKsce.dphi   = dphi; 	        //강도감도계수		


	if(bPrint)
	{
		m_Print.m_BdcyParam.Initialize();    

		m_Print.m_BdcyParam.iCount = iCount;
		m_Print.m_BdcyParam.dfck   = dfck;
		m_Print.m_BdcyParam.dbw    = dbw;
		m_Print.m_BdcyParam.db     = db;

		m_Print.m_BdcyParam.dBc     = dBc   ;
		m_Print.m_BdcyParam.dtc     = dtc   ;
		m_Print.m_BdcyParam.dHh     = dHh;
		m_Print.m_BdcyParam.dH_gr   = dH_gr ;

		m_Print.m_BdcyParam.dAps   = dAptol;
		m_Print.m_BdcyParam.dfy    = dfy;
		m_Print.m_BdcyParam.ddp    = ddp;
		m_Print.m_BdcyParam.dAs    = dAsttol;
		m_Print.m_BdcyParam.dfpu   = dfpu;
		m_Print.m_BdcyParam.dd     = ddst;
		m_Print.m_BdcyParam.dAc    = dAg;
		m_Print.m_BdcyParam.dIy    = dIner_g;
		m_Print.m_BdcyParam.dyb    = dyt_g;
		m_Print.m_BdcyParam.dPe    = dPetotal; // 결정된 중립축 기준.
		m_Print.m_BdcyParam.dbeta1 = dBeta1;
		m_Print.m_BdcyParam.dep    = ddp-dCp_g;
		m_Print.m_BdcyParam.dMu    = dMu;
		m_Print.m_BdcyParam.drp    = drp;
		m_Print.m_BdcyParam.dhf    = dhf;

		// test 도로교10, 철도교11
//     BdcrDetail.bIter        = bIter;
//     BdcrDetail.bOneType     = bOneType;
//     BdcrDetail.bBondType    = bBondType;
//     BdcrDetail.bApwM        = bApwM;
//     BdcrDetail.dNeuC        = dNeuC;
//     BdcrDetail.daArea       = daArea;
//     BdcrDetail.dCc          = dCc;
//     BdcrDetail.dRhop        = dRhop;
//     BdcrDetail.dfpy_unbond  =dfpy_unbond;
//     BdcrDetail.dfpe_unb_cal = dfpe_unb_cal;
//     BdcrDetail.dfpe_unb     = dfpe_unb;
//     BdcrDetail.d05fpu       = d05fpu;
//     BdcrDetail.dfps_cal     = dfps_cal;
//     BdcrDetail.dfps_un_com  = dfps_un_com;
//     BdcrDetail.dfps_un_max  = dfps_un_max;
//     BdcrDetail.dMn_ps       = dMn_ps;
//     BdcrDetail.dApw         = dApw;
//     BdcrDetail.dApf         = dApf;
//     BdcrDetail.dMn_Cal      = dMn_Cal;
//     BdcrDetail.dphiMn_Cal   = dphiMn_Cal;
//     BdcrDetail.dPe_t        = dPe_t;
//     BdcrDetail.dPeep        = dPeep;
//     BdcrDetail.dMcr         = dMcr;
//     BdcrDetail.dTp          = dTp       ;
//     BdcrDetail.dTpTimesep   = dTpTimesep;
//     BdcrDetail.dPeTimesep   = dPeTimesep;
//     BdcrDetail.dEpsi_cu     = dEpsi_cu;
//     BdcrDetail.dEpsi_st     = dEpsi_st;
//     BdcrDetail.dfst         = dfst;
//     BdcrDetail.dfst_cal     = dfst_cal;
//     BdcrDetail.dEs          = dEs;
//     BdcrDetail.dTst         = dTst;
//     BdcrDetail.daCent       = daCent;
//     BdcrDetail.dMps         = dMps;
// 
//     BdcrDetail.strTndn.Copy(strTndn);
//     for(int i=0; i<9; i++) BdcrDetail.arfps[i].Copy(arfps[i]);
		
		m_Print.m_BdcyParam.bIter        = bIter;
		m_Print.m_BdcyParam.bOneType     = bOneType;
		m_Print.m_BdcyParam.bBondType    = bBondType;
		m_Print.m_BdcyParam.bApwM        = bApwM;
		m_Print.m_BdcyParam.dNeuC        = dNeuC;
		m_Print.m_BdcyParam.daArea       = daArea;
		m_Print.m_BdcyParam.dCc          = dCc;
		m_Print.m_BdcyParam.dRhop        = dRhop;
		m_Print.m_BdcyParam.dfpy_unbond  =dfpy_unbond;
		m_Print.m_BdcyParam.dfpe_unb_cal = dfpe_unb_cal;
		m_Print.m_BdcyParam.dfpe_unb     = dfpe_unb;
		m_Print.m_BdcyParam.d05fpu       = d05fpu;
		m_Print.m_BdcyParam.dfps_cal     = dfps_cal;
		m_Print.m_BdcyParam.dfps_un_com  = dfps_un_com;
		m_Print.m_BdcyParam.dfps_un_max  = dfps_un_max;
		m_Print.m_BdcyParam.dMn_ps       = dMn_ps;
		m_Print.m_BdcyParam.dApw         = dApw;
		m_Print.m_BdcyParam.dApf         = dApf;
		m_Print.m_BdcyParam.dMn_Cal      = dMn_Cal;
		m_Print.m_BdcyParam.dphiMn_Cal   = dphiMn_Cal;
		m_Print.m_BdcyParam.dPe_t        = dPe_t;
		m_Print.m_BdcyParam.dPeep        = dPeep;
		m_Print.m_BdcyParam.dMcr         = dMcr;
		m_Print.m_BdcyParam.dTp          = dTp       ;
		m_Print.m_BdcyParam.dTpTimesep   = dTpTimesep;
		m_Print.m_BdcyParam.dPeTimesep   = dPeTimesep;
		m_Print.m_BdcyParam.dEpsi_cu     = dEpsi_cu;
		m_Print.m_BdcyParam.dEpsi_st     = dEpsi_st;
		m_Print.m_BdcyParam.dfst         = dfst;
		m_Print.m_BdcyParam.dfst_cal     = dfst_cal;
		m_Print.m_BdcyParam.dEs          = dEs;
		m_Print.m_BdcyParam.dTst         = dTst;
		m_Print.m_BdcyParam.daCent       = daCent;
		m_Print.m_BdcyParam.dMps         = dMps;
		m_Print.m_BdcyParam.dphiMn_beta1 = dphiMn_beta1;
		m_Print.m_BdcyParam.dfps_temp_cal = dfps_temp_cal;
		m_Print.m_BdcyParam.dfps_temp     = dfps_temp;   
		
		m_Print.m_BdcyParam.strTndn.Copy(strTndn);
		m_Print.m_BdcyParam.arfps0.Copy(arfps[0]);
		m_Print.m_BdcyParam.arfps1.Copy(arfps[1]);
		m_Print.m_BdcyParam.arfps2.Copy(arfps[2]);
		m_Print.m_BdcyParam.arfps3.Copy(arfps[3]);
		m_Print.m_BdcyParam.arfps4.Copy(arfps[4]);
		m_Print.m_BdcyParam.arfps5.Copy(arfps[5]);
		m_Print.m_BdcyParam.arfps6.Copy(arfps[6]);
		m_Print.m_BdcyParam.arfps7.Copy(arfps[7]);
		m_Print.m_BdcyParam.arfps8.Copy(arfps[8]);

		// 도로교05, 철도교04 일 때만. 
		if(m_iCivilCode==KSCE_USD05 || m_iCivilCode==KSCE_RAIL_USD04)
		{
			if(bIter)
			{
				m_Print.Print_BdcyDesign0(bApwM);
				m_Print.Print_BdcyDesign4(dNeuC,  dBeta1,  da , dfck,  daArea,  dCc);
				m_Print.Print_BdcyDesign5(strTndn, arfps, dTp, dTpTimesep, dPeTimesep);  //9 = Column
				m_Print.Print_BdcyDesign6(dEpsi_cu, ddst, dNeuC, dEpsi_st, dfst, dfst_cal, dEs, dfy, dAsttol,  dTst);
				//m_Print.Print_BdcyDesign7(dTp, dTst, dCc, dCcDel, dTOL);
				m_Print.Print_BdcyDesign8(dCc, dNeuC, da, daCent, dMps, dTst, ddst, dMn_Cal, dphiMn_Cal, dMu);
				m_Print.Print_BdcyDesign_Code5(bIter, dphi, dfpu, drp, dBeta1, dRhop, dfck, da, dhf, db, dbw, ddst, ddp, dqp, dRhos, 
					dApw, dAptol, dApf, dAsttol, dfy, dfpsave, dphiMn, dphiMn_beta1, d036beta1, dphiMn_Cal);
				//if(dhf>=da) { m_Print.Print_BdcyDesign9(da, dhf, dRhop, dfpsave, dfck, dqp, d036beta1); }
				//else        { m_Print.Print_BdcyDesign91(da, dhf, dApw, dAptol, dApf, dAsttol, db, dbw, ddp, dfck, dfpsave, dfy, dqp, d036beta1); } 
				m_Print.Print_BdcyDesign10(bIter, dPe_t, dAg, dPeep, dyt_g, dIner_g, dfpe_ave, dfck, dphiMn, d12Mcr, dMu);
			}
			else
			{        
				m_Print.Print_BdcyDesign0(bApwM);
				m_Print.Print_BdcyDesign_Code1(dNeuC, dBeta1, da, dfck);
				if(bOneType)
				{
					if(bBondType) m_Print.Print_BdcyDesign_Code2(dfpu, drp, dBeta1, dRhop, dfck, ddst, ddp, dRhos, dRhoc, dfy, dfps);
					else          m_Print.Print_BdcyDesign_Code3(dfck, dfpu, dfpy_unbond, dfpe_unb_cal, dfpe_unb, d05fpu, dfps, dfps_cal, dRhop, dfps_un_com, dfps_un_max);
					//m_Print.Print_BdcyDesign_Code3(dfpu, dfpe_unb_cal, dfpe_unb, d05fpu, dfps);
					
					m_Print.Print_BdcyDesign_Code4(da, db, dbw, dApw, dApf, dAptol, dAst, dhf, dfpu, drp, dBeta1, dRhop, 
						dfck, ddst, ddp, dRhos, dfy, dfps, dphi, dMn_Cal, dphiMn_Cal, dMu);
				}
				else
				{
					m_Print.Print_BdcyDesign_Code6(strTndn, arfps);
					m_Print.Print_BdcyDesign_Code7(dNeuC, da, db, dbw, dAst, dhf, dfck, ddst, ddp, dfy, dphi, dMn_ps, dphiMn, dMu);
				}
				
				m_Print.Print_BdcyDesign_Code5(bIter, dphi, dfpu, drp, dBeta1, dRhop, dfck, da, dhf, db, dbw, ddst, ddp, dqp, dRhos, 
					dApw, dAptol, dApf, dAsttol, dfy, dfps, dphiMn, dphiMn_beta1, d036beta1, dphiMn_Cal);
				m_Print.Print_BdcyDesign10(bIter, dPe_t, dAg, dPeep, dyt_g, dIner_g, dfpe_ave, dfck, dphiMn, d12Mcr, dMu);
				
			}
		}
		else if(m_iCivilCode==KSCE_USD10 || m_iCivilCode==KSCE_RAIL_USD11)
		{
			m_Print_ksce10.SetCompositeSect(SectPosD.bCompoSect);
			m_Print_ksce10.Print_BdcyDesign(BdcrKsce, m_Print.m_BdcyParam, pXL);     
			double test = 0;
		}
	}

	/*
	////////////////////////////
	TRACE(_T("------------------\n"));
	for(i=0 ; i<arTndnPosi.GetSize() ; i++)
	{
			_DGN_TNDN_CRC rTnInf = arTndnPosi.GetAt(i);
			TRACE(_T("A = %lf, dz = %lf\n"), rTnInf.dArea, rTnInf.dz);
	}
	*/
 
	
	return TRUE;  
}

//휨강도(Z-axis)
BOOL CDgnKSCE_USD05::Get_FlexureStrength(BOOL bYaxis, BOOL bPosiI, BOOL bIter, BOOL bPositive, double dPhib, _PSC_MATL_ELEM& MatlElem, T_SPSC_SECT& SpscSect, CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*>& arInner, T_SPSC_SECT& SpscgSect,
																				 double dbw, double dFlgThk[2], double dpMomPos[2][2], CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_BDCZ_KSCE_BASE& BdcrKsce, CMSExcel* pXL)

{
	T_MATD_D MatD = MatlElem.MatdD;
	//Material
	double dfck= MatD.Data1.Design.C_fc;
	double dEc = MatD.Data1.Analysis.Elast;
	double dfy = MatD.MainRebarData.B_fy;
	double dEs = MatD.MainRebarData.B_Elast;  
	//Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy  = dyDim - dyCen;
	double dCpz  = dzDim - dzCen;
	double dArea = SpscSect.dArea;
	double dIyy  = SpscSect.dIyy;
	double dIzz  = SpscSect.dIzz;
	
	//Section_g
	double dyDim_g = SpscgSect.dyDim;
	double dzDim_g = SpscgSect.dzDim;
	double dyCen_g = SpscgSect.dyCen;
	double dzCen_g = SpscgSect.dzCen;
	double dCpy_g  = dyDim_g - SpscgSect.dyCen;
	double dCpz_g  = dzDim_g - SpscgSect.dzCen;
	double dAg     = SpscgSect.dArea;
	double dIyy_g  = SpscgSect.dIyy;
	double dIzz_g  = SpscgSect.dIzz;
	double dIner_g=(bYaxis==TRUE ? dIyy_g : dIzz_g);

	//Force
	double dMu = BdcrKsce.dMuy;
		
	double dApcmax=0.0; //최대강재량
	double dphi = dPhib;
	double dalpha1 = 0.85; // phi와는 다른 값.
	double dBeta1 = Get_Beta1(dfck);
	double d036beta1 = 0.36*dBeta1;
	 
	double dEpsi_cu = 0.003;
	double dEpsi_cp=0.0, dEpsi_st=0.0, dEpsi_sc=0.0;
	double dEpsi_ce=0.0, dEpsi_pe=0.0, dEpsi_ps=0.0;
		
	int i=0,j=0;
	double dgap=0.0;
	double dAstol=0.0, dAsY=0.0;
	double dAptol=0.0, dNtodp=0.0, ddp=0.0;
	double dAsttol=0.0, dNtodst=0.0, ddst=0.0;
	double dAsctol=0.0, dNtodsc=0.0, ddsc=0.0;
	double dApt=0.0, dApc=0.0, dAst=0.0, dAsc=0.0;
		
	double dNeuC=0.0;
	double ddist=0.0;
	double dfst=0.0, dfsc=0.0;
	
	double daArea=0.0, daQ=0.0, daCent=0.0;
	double da=0.0, dCc=0.0, dTp=0.0, dTst=0.0, dCsc=0.0, dCcDel=0.0;
	double dfps=0.0, dMps=0.0;
	double dMn=0.0, dphiMn=0.0, dMn_Cal=0.0, dphiMn_Cal=0.0;
	double dTpTimesep=0.0, dPeTimesep=0.0, dfpsiApsi=0.0;
	double dTOL = 10; //1kgf
	double dTOL2 = 0.001;
	double dToler=0.0;
	
	//계산시 사용되는 변수..Iteration & 도로교설계기준 공통
	double dApw=0.0, dApf=0.0;
	double dqp=0.0;
	double dfpetotal=0.0, dPetotal=0.0;  
	double dfpu=0.0;
	double dfpsave=0.0;
	double dRhop_tot=0.0, dRhop = 0.0, dRhos=0.0, dRhoc=0.0;
	double dbdp=0.0, dbds=0.0, dQa=0.0;  

	// Code의 공식을 사용할 때.
	BOOL bOneType=TRUE;
	BOOL bBondType=TRUE;
	UINT iTenTndnNum=0;
	UINT iBondNum   =0;
	UINT iUnbondNum =0;
	double dhf=0.0;
	double dfpe_unb_cal = 0.0;
	double dfpe_unb = 0.0;
	double d05fpu = 0.0;
	double dMn_ps=0.0;
	double dMn_beta1 = 0.0;
	double dphiMn_beta1=0.0;  
	 // 철도기준에 사용되는 변수.
	 double dfpy_bond=0.0;
	 double dfpy_unbond=0.0; 
	 double dfps_cal=0.0;
	 double dfps_un_max=0.0;
	 double dfps_un_com=0.0;

	double dIner=(bYaxis==TRUE ? dIyy : dIzz);
	BOOL bDirec = (bPositive ? TRUE : FALSE);
	double dh = (bYaxis==TRUE ? dzDim : dyDim);
	//double db = (bYaxis==TRUE ? dyDim : dzDim);
	double dLength = (bYaxis==TRUE ? dyDim : dzDim);
	double dbf_top = dyDim;
	double dbf_bot = fabs(dpMomPos[0][0] - dpMomPos[1][0]); // 두점의 길이가 아님 x좌표 의 차이로함.
	double db  = (bPositive ? dbf_top : dbf_bot);    

	//0:Pei 1:Apsi 2:dpi 3:epi 4:Epsi_ps 5:fpsi 6:Apsi·fpsi 7:Tpi·epi 8:Pei·epi
	CArray<CString,CString> strTndn;
	CArray<double,double> arfps[9]; 
	// Z-axis 은 Print 하지 않는다. 
	BOOL bPrint = FALSE; //m_Print.Get_PrintFlag();

	//Assume C.
	dNeuC = (bYaxis==TRUE ? dzDim : dyDim) * 0.5;
	double dCp = 0.0;
	if(bPositive) dCp = (bYaxis==TRUE ? dCpz : dCpy);
	else          dCp = (bYaxis==TRUE ? dzCen : dyCen);
	double dCtemp=0.0;
	int NumberofTendon = arTndnPosi.GetSize();
	UINT iCount = 1;  
	// Flange Height.
	dhf = (bPositive ? dFlgThk[0] : dFlgThk[1]);

	// 중립축 결정을 위해서. 
	BOOL bPosiForce=TRUE;
	double dNeuC1 = 0.0;
	double dNeuC2 = 0.0;
	BOOL bStop = FALSE;

			
	if(bIter)
	{     
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> rarRbarPosi;
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> rarTndnPosi;
		do
		{
			bPosiForce = TRUE;
			iTenTndnNum=0;
			iBondNum   =0;
			iUnbondNum =0;
			dfpsiApsi  = 0.0;
			if(bPrint)
			{
				strTndn.RemoveAll();
				for( i=0; i<9; i++) arfps[i].RemoveAll();        
				
				dTpTimesep = 0.0;
			}

			dPetotal=0.0;
			//beta1에 대한 면적은 Iteration으로 계산하지 않음. ->나중에 Iteration 적용.
			da = dBeta1*dNeuC;
			double daTemp = (bPositive ? dh-da : da);
			if(CalcSectPSC_QA(arOuter, arInner, bDirec, bYaxis, daTemp, dLength, daArea, daQ))
			{
				daCent = (daArea==0.0 ? 0.0 : daQ/daArea);
				dCc    = dalpha1*dfck*daArea; //
			}
		
			//dNeuC을 기준으로 Rebar(Ac,d) 계산 
			dCtemp = (bPositive ? dNeuC : dh-dNeuC);
			_DGN_TNDN_RBAR rTdRb;
			rTdRb.Initialize();      
			BOOL bGetOk = Get_TndnReBarInfor(bYaxis, dCtemp, SpscSect, arRbarPosi, arTndnPosi, rTdRb);
			dAptol =(bPositive ? rTdRb.dApttol : rTdRb.dApctol); 
			ddp    =(bPositive ? rTdRb.ddpt    : dh-rTdRb.ddpc);
			if(dAptol==0.0) ddp = 0.0;
			dAsttol=(bPositive ? rTdRb.dAsttol : rTdRb.dAsctol); 
			ddst   =(bPositive ? rTdRb.ddst    : dh-rTdRb.ddsc);
			if(dAsttol==0.0) ddst = 0.0;
			dAsctol=(bPositive ? rTdRb.dAsctol : rTdRb.dAsttol); 
			ddsc   =(bPositive ? rTdRb.ddsc    : dh-rTdRb.ddst);
			if(dAsctol==0.0) ddsc = 0.0;
				
			double dbdp_It = 0.0;
			double dbd_It = 0.0;      
			if(dhf>=da)
			{
				dbd_It = db*ddst;
				dRhos  = (dbd_It==0.0 ? 0.0 : dAsttol/dbd_It);
				dRhoc  = (dbd_It==0.0 ? 0.0 : dAsctol/dbd_It);

				dbdp_It = db*ddp;
				dRhop = (dbdp_It==0.0 ? 0.0 : dAptol/dbdp_It);
			}
			else if(dhf<da)
			{
				dbd_It = dbw*ddst;
				dRhos  = (dbd_It==0.0 ? 0.0 : dAsttol/dbd_It);
				dRhoc  = (dbd_It==0.0 ? 0.0 : dAsctol/dbd_It);

				dbdp_It = dbw*ddp;
				dRhop = (dbdp_It==0.0 ? 0.0 : dAptol/dbdp_It);
			}

			Get_TndnReBarTransform(bYaxis, bPositive, dNeuC, dCp, SpscSect, arRbarPosi, arTndnPosi, rarRbarPosi, rarTndnPosi);

			dfps=0.0; dMps=0.0; dTp=0.0;
			double dfpu_min = 1.7E308; 
			double dPe_Sum=0.0, dMe_Sum=0.0, dep_Sum=0.0;
			for(i=0 ; i<NumberofTendon ; i++)
			{
				_DGN_TNDN_CRC rTnInf = rarTndnPosi.GetAt(i);
				double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
				if(bPositive && dposi<0.0) 
				{
					dPe_Sum += rTnInf.dArea*rTnInf.dStress[0];
					dMe_Sum += rTnInf.dArea*rTnInf.dStress[0]*dposi;
				}
				else if(!bPositive && dposi>0.0)
				{
					dPe_Sum += rTnInf.dArea*rTnInf.dStress[0];
					dMe_Sum += rTnInf.dArea*rTnInf.dStress[0]*dposi;
				}
				dep_Sum = dPe_Sum==0.0 ? 0.0 : dMe_Sum/dPe_Sum;
			}

			for(i=0 ; i<NumberofTendon ; i++)
			{
				_DGN_TNDN_CRC rTnInf = rarTndnPosi.GetAt(i);
				double dposi   = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
				
				if(bPositive && dposi<0.0) 
				{
					double ddpi = dNeuC + fabs(dposi);
					double dPe  = rTnInf.dArea * rTnInf.dStress[0];
					dEpsi_cp = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddpi-dNeuC)/dNeuC);
					dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
					dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe_Sum/(dArea*dEc)*(1.0+(dep_Sum*dposi)/(dIner/dArea));  /**/
					dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
					if(rTnInf.iBondType==0)   dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);
					else if(rTnInf.iBondType==1) 
					{
						dfpy_unbond = rTnInf.dfpy;
						double dfpe_unbi = rTnInf.dStress[0];
						if(m_iCode==KSCE_USD05 )
						{
							if(dfpe_unbi>=0.5*rTnInf.dfpu)     dfps = dfpe_unbi + 105.0; 
							else if(dfpe_unbi<0.5*rTnInf.dfpu) dfps = 0.5*rTnInf.dfpu   + 105.0;          
						}
						else if(m_iCode==KSCE_USD10)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 420.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
						else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 400.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
					}
					dfps = min(dfps,rTnInf.dfpu);
					dPetotal += rTnInf.dArea*rTnInf.dStress[0];
					dTp += dfps*rTnInf.dArea;
					dMps += dfps*rTnInf.dArea*fabs(dposi); 
					dfpu_min = min(rTnInf.dfpu,dfpu_min);          
					dfpsiApsi  += dfps*rTnInf.dArea;

					if(bPrint)
					{            
						strTndn.Add(rTnInf.strTndnGrup);
						arfps[0].Add(dPe);                // 0:Pei
						arfps[1].Add(rTnInf.dArea);       // 1:Apsi
						arfps[2].Add(ddpi);               // 2:dpi
						arfps[3].Add(fabs(dposi));        // 3:epi
						arfps[4].Add(dEpsi_ps);           // 4:Epsi_ps
										
						double dApsi_fpsi = dfps*rTnInf.dArea;
						double dTpi_epi = dApsi_fpsi*fabs(dposi);
						double dPei_epi = (rTnInf.dArea*rTnInf.dStress[0])*fabs(dposi);
						arfps[5].Add(dfps);       //5:fpsi
						arfps[6].Add(dApsi_fpsi); //6:Apsi·fpsi
						arfps[7].Add(dTpi_epi);   //7:Tpi·epi
						arfps[8].Add(dPei_epi);   //8:Pei·epi
						dTpTimesep += dTpi_epi;
					}

				}
				else if(!bPositive && dposi>0.0)
				{
					double ddpi = dNeuC + fabs(dposi);
					double dPe  = rTnInf.dArea * rTnInf.dStress[0];
					dEpsi_cp = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddpi-dNeuC)/dNeuC);
					dEpsi_pe = (rTnInf.dEps==0.0 ? 0.0 : rTnInf.dStress[0]/rTnInf.dEps);
					dEpsi_ce = dArea==0.0 || dEc==0.0 || dIner==0.0 ? 0.0 : dPe_Sum/(dArea*dEc)*(1.0+(dep_Sum*dposi)/(dIner/dArea));
					dEpsi_ps = dEpsi_pe+dEpsi_ce+dEpsi_cp;
					if(rTnInf.iBondType==0)   dfps = Get_StressOfTendon(rTnInf.dfpy, rTnInf.dfpu, rTnInf.dEps, dEpsi_ps);
					else if(rTnInf.iBondType==1) 
					{
						dfpy_unbond = rTnInf.dfpy;
						double dfpe_unbi = rTnInf.dStress[0];
						if(m_iCode==KSCE_USD05 )
						{
							if(dfpe_unbi>=0.5*rTnInf.dfpu)     dfps = dfpe_unbi + 105.0; 
							else if(dfpe_unbi<0.5*rTnInf.dfpu) dfps = 0.5*rTnInf.dfpu   + 105.0;          
						}
						else if(m_iCode==KSCE_USD10)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 420.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
						else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
						{
							// (가) 지간과 높이의 비가 35이하인 경우. //!/ 우선 고정.
							dfps_cal = (dRhop==0.0 ? dfpe_unbi + 70.0 : dfpe_unbi + 70.0 + dfck/(100.0*dRhop));  
							dfps_un_com = dfpe_unbi + 400.0;
							dfps_un_max = min(dfpy_unbond, dfps_un_com);
							dfps = min(dfps_cal, dfps_un_max);
						}
					}
					dfps = min(dfps,rTnInf.dfpu);
					dPetotal += rTnInf.dArea*rTnInf.dStress[0];
					dTp += dfps*rTnInf.dArea;
					dMps += dfps*rTnInf.dArea*fabs(dposi);
					dfpu_min = min(rTnInf.dfpu,dfpu_min);
					dfpsiApsi  += dfps*rTnInf.dArea;

					if(bPrint)
					{            
						strTndn.Add(rTnInf.strTndnGrup);
						arfps[0].Add(dPe);                // 0:Pei
						arfps[1].Add(rTnInf.dArea);       // 1:Apsi
						arfps[2].Add(ddpi);               // 2:dpi
						arfps[3].Add(fabs(dposi));        // 3:epi
						arfps[4].Add(dEpsi_ps);           // 4:Epsi_ps            
				
						double dApsi_fpsi = dfps*rTnInf.dArea;
						double dTpi_epi = dApsi_fpsi*fabs(dposi);
						double dPei_epi = (rTnInf.dArea*rTnInf.dStress[0])*fabs(dposi);
						arfps[5].Add(dfps);       //5:fpsi
						arfps[6].Add(dApsi_fpsi); //6:Apsi·fpsi
						arfps[7].Add(dTpi_epi);   //7:Tpi·epi
						arfps[8].Add(dPei_epi);   //8:Pei·epi
						dTpTimesep += dTpi_epi;
					}

				}               
			}
			dfpu = dfpu_min;
		 
			if(dAsttol>0.0) dEpsi_st = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(ddst-dNeuC)/dNeuC);
			else dEpsi_st=0.0;
			if(dAsctol>0.0) dEpsi_sc = (dNeuC==0.0 ? 0.0 : dEpsi_cu*(dNeuC-ddsc)/dNeuC);
			else dEpsi_sc=0.0;

			dfst = min(dEpsi_st*dEs, dfy);
			dfsc = min(dEpsi_sc*dEs, dfy);   
					
			dTst = dAsttol*dfst;
			dCsc = dAsctol*dfsc;
		  //dTp = dAptol*dfps;
			
			double dComp = dCc;
		  dCcDel = dTp + dTst - dCc;
			dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);

			if(dCcDel<0.0) bPosiForce = FALSE;

			///
			if(iCount==1) dNeuC2 = dNeuC;
			dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
			if(fabs(dToler)<dTOL2) break;
			if(iCount>m_PSC_iITER) break;
			
			if(dCcDel<=0.0) dNeuC2 = dNeuC;
			else            dNeuC1 = dNeuC;
			dNeuC = 0.5*(dNeuC1 + dNeuC2);
			///
			
			if(iCount>50 && fabs(dNeuC1-dNeuC2)<0.00001) bStop= TRUE;
			if(fabs(dToler)<dTOL2)                       bStop= TRUE;

//       if(bPosiForce)
//       {
//         dNeuC += 0.3*dh;
//         dNeuC2 = dNeuC;
//         dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
//         if(fabs(dToler)<dTOL2) break;
//         if(iCount>m_PSC_iITER) break;
//       }
//       else
//       {
//         if(iCount==1) dNeuC2 = dNeuC;
//         dToler = (dComp==0.0 ? 0.0 : dCcDel/dComp);
//         if(fabs(dToler)<dTOL2) break;
//         if(iCount>m_PSC_iITER) break;
//         
//         if(dCcDel<=0.0) dNeuC2 = dNeuC;
//         else            dNeuC1 = dNeuC;
//         dNeuC = 0.5*(dNeuC1 + dNeuC2);
//       }

			++iCount;

		}while(!bStop); //  fabs(dToler)>dTOL2
		
		dfpsave = (dAptol==0.0 ? 0.0 : dfpsiApsi/dAptol);

		//dNeuC에서 dMn..
		dMn = dCc*(dNeuC-(da-daCent)) + dMps + dTst*(ddst-dNeuC); 
		dMn_Cal = dMn;   
		dphiMn_Cal = dphi*dMn;    

		double dAp = dAptol; // 인장측 긴장재만. 
		dApw = 0.0;    

		if(dhf>=da) // Rectangle.
		{
			dqp = (dfck==0.0 ? 0.0: dRhop*dfpsave/dfck);  //dfpsave
		}
		else if(dhf<da)
		{
			dApf = (dfpsave==0.0 ? 0.0 : dalpha1*dfck*(db-dbw)*dhf/dfpsave);
			if(dAsttol>0.0)
			{
				dApw = (dfpsave==0.0 ? 0.0 : dAp + (dAsttol*dfy/dfpsave) - dApf);
			}
			else
			{
				dApw = dAp - dApf;
			}
			
			dqp = (dbw*ddp ==0.0 || dfck==0.0 ? 0.0 : dApw/(dbw*ddp)*dfpsave/dfck);
		}
		else ASSERT(0);
		
		if(da<=dhf)  // 사각형 단면.
		{
			dqp = dRhop*dfps/dfck;
			dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*db*pow(ddp,2);
		 
			if(dqp>d036beta1)
			{        
				dMn = min(dMn, dMn_beta1);
			}
		}
		else if(da>dhf)  // 플랜지를 갖는 단면.
		{
			dqp = (dbw*ddp*dfck==0.0 ? 0.0 : dApw*dfps/(dbw*ddp*dfck));
			dMn_beta1 = (0.36*dBeta1-0.08*pow(dBeta1,2))*dfck*dbw*pow(ddp,2)
													+dalpha1*dfck*(db-dbw)*dhf*(ddp-0.5*dhf);
			if(dqp>d036beta1)
			{        
				dMn = min(dMn, dMn_beta1);
			}

		}

		dphiMn = dphi*dMn;
		dphiMn_beta1 = dphi * dMn_beta1;    

	}
	else ASSERT(0);
	
	double dCp_g = 0.0;
	if(bPositive) dCp_g = (bYaxis==TRUE ? dCpz_g : dCpy_g);
	else          dCp_g = (bYaxis==TRUE ? dzCen_g : dyCen_g);
	double dyt_g = dh-dCp_g;
	
	double dyt = dh-dCp;

	// fpcc Calculation.
	double dAps=0.0, dPeep=0.0, dPe_t=0.0, dVp=0.0, dfpe_ave=0.0;
	double dRatM = (dphiMn==0.0 ? 0.0 : fabs(dMu)/dphiMn);
	double dfru=0.63*sqrt(dfck);
	double dMcr=0.0;
	double d12Mcr=0.0;

	if(m_iCode==KSCE_USD05 || m_iCode==KSCE_USD10)
	{
		if(ddp==0.0 && ddst==0.0)
		{
			bPrint = FALSE;
			BdcrKsce.bCHK = FALSE; // 텐던 & 철근이 없으면 FALSE
		}
	}
	else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
	{
		if(ddp==0.0)
		{
			bPrint = FALSE;
			BdcrKsce.bCHK = FALSE; // 텐던 & 철근이 없으면 FALSE
		}
	}

	BdcrKsce.dMuy   = dMu;	          //면내 계수모멘트		
	BdcrKsce.dpMny  = dphiMn;	        //면내 설계휨모멘트	
	BdcrKsce.dRatM  = dRatM;	        //면내 휨강도 안전율	

	BdcrKsce.dfpe   = 0.0;	    //손실이 일어난 후 강재의 유효프리스트레스	
	BdcrKsce.dAps   = 0.0;	        //인장측 PS강재의 단면적	
	BdcrKsce.dAst   = 0.0;	      //인장측 철근의 단면적		
	BdcrKsce.dpst   = 0.0;	        //인장측 철근비			
	BdcrKsce.dIy    = 0.0;	      //강축 단면 2차모멘트 		
//  BdcrKsce.db	    = 0.0;		          //단면폭
//  BdcrKsce.dh	    = 0.0;		      //단면춤
	BdcrKsce.ddst   = 0.0;	          //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		
	BdcrKsce.ddps   = 0.0;	          //인장측 PS강재의 유효춤	
	BdcrKsce.dfps   = 0.0; //계수하중 하에서 PS강재 평균응력	
	BdcrKsce.da     = 0.0;		          //등가 직사각형 응력블럭의 깊이	
	BdcrKsce.dqp    = 0.0;		        //강재지수			
	BdcrKsce.d036Beta1 = 0.0;   //최대 PS강재량 제한값	
	BdcrKsce.d12Mcr = 0.0;	        //최소 강재 균열모멘트	  
	BdcrKsce.dPe    = 0.0;		    //유효긴장력			
	BdcrKsce.dAsc   = 0.0;	      //압축측 철근의 단면적	
	BdcrKsce.ddsc   = 0.0;	          //콘크리트압축 연단에서 압축측 철근 중심까지의 거리		
	BdcrKsce.dpsc   = 0.0;	        //압축측 철근비		
	BdcrKsce.dyt    = 0.0;		      //총단면적의 중립축에서 인장 연단까지의 거리	
	BdcrKsce.deps   = 0.0;	    //PS강재의 도심의 편심량 
	BdcrKsce.dAc    = 0.0; 		      //콘크리트의 전면적     
	BdcrKsce.dfy    = 0.0;            //인장철근의 설계 항복강도 	
	BdcrKsce.dfck   = 0.0;	          //콘크리트 설계기준강도 		
	BdcrKsce.dbeta1 = 0.0;         //0.85fck의 압축응력이 균등하게 분포하는 직사각형 블록의 깊이 a의 비                
	BdcrKsce.drp    = 0.0; 	          //PS강재의 종류에 따른 계수	
	BdcrKsce.dphi   = 0.0; 	        //강도감도계수		
	
	return TRUE;  
}

double CDgnKSCE_USD05::Get_Beta1(double dfc)
{
	// Factor for stress block width.
	double dBeta=0.0;

	if(dfc <= 28.0)	dBeta = 0.85;
	else						dBeta = max(0.85-0.007*(dfc-28.0), 0.65);
	
	return dBeta;
}

double CDgnKSCE_USD05::Get_StressOfTendon(double df_py, double df_pu, double dEp, double dEpsi_ps)
{
	double dfp=0.0;
	if(df_pu<=(1860.0-m_dZero)) // SWPC 7A
	{
		if(dEpsi_ps<=(0.0076+m_dZero)) dfp = dEpsi_ps*dEp;
		else                           dfp = (1720.0-0.276/(dEpsi_ps-0.0064));       
	}
	else // SWPC 7B
	{
		if(dEpsi_ps<=(0.0086+m_dZero)) dfp = dEpsi_ps*dEp;
		else                           dfp = (1860.0-0.276/(dEpsi_ps-0.007));
	}

	if(dfp>df_pu) dfp = df_pu;
	return dfp;  
}


BOOL CDgnKSCE_USD05::Get_ReBarInfor(BOOL bYaxis, double dGap, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
	  								 double& dAs, double& dds, double& dAsc, double& ddc)
{
	int i=0;
	double ddst=0.0, ddsc=0.0;
	double dAttol=0.0, dActol=0.0;
	double dAtY=0.0, dAcY=0.0;
	for(i=0 ; i<arRbarPosi.GetSize(); i++)
	{
		_DGN_RBAR_CRC rRebar = arRbarPosi.GetAt(i);
		if(bYaxis)
		{
			rRebar.dz = rRebar.dz + dGap;
			if(rRebar.dz<0.0)
			{
				dAttol += rRebar.dArea;
				dAtY += rRebar.dArea*rRebar.dz;
			}
			else
			{
				dActol += rRebar.dArea;
				dAcY += rRebar.dArea*rRebar.dz;
			}
		}
		else
		{
			rRebar.dy = rRebar.dy + dGap;
			if(rRebar.dy<0.0)
			{
				dAttol += rRebar.dArea;
				dAtY += rRebar.dArea*rRebar.dz;
			}
			else
			{
				dActol += rRebar.dArea;
				dAcY += rRebar.dArea*rRebar.dz;
			}
		}
	}
	dAs = dAttol;  dds = (dAttol==0.0 ? 0.0 : dAtY/dAttol);
	dAsc = dActol; ddc = (dActol==0.0 ? 0.0 : dAcY/dActol);

	return TRUE;
}									

BOOL CDgnKSCE_USD05::Get_fpeData(BOOL bYaxis, double dMu, double dAg, double dIy, double dyt, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, double& dAps, double& dPeep, double& dPe, double& dVp, double& dfpe)
{
	dAps=dPe=dPeep=dVp=0.0;
	for(int i=0 ; i<arTndnPosi.GetSize() ; i++) // Gross 기준으로.
	{
		_DGN_TNDN_CRC rTnInf = arTndnPosi.GetAt(i);
		double dposi = (bYaxis==TRUE ? rTnInf.dz : rTnInf.dy);
		double dPei = rTnInf.dArea*rTnInf.dStress[0];      
		double dPei_epi = (rTnInf.dArea*rTnInf.dStress[0])*fabs(dposi);
		dAps += rTnInf.dArea;
		dPe  += rTnInf.dStress[0]*rTnInf.dArea; 
		dVp  += rTnInf.dStress[2]*rTnInf.dArea;  //Total Tendon.
		if(dMu>=0.0)  
		{      
			if(dposi<0.0)       
			{        
				dPeep += dPei_epi;        
			}      
			else if(dposi>=0.0) 
			{        
				dPeep -= dPei_epi;
			}      
		}      
		else if(dMu<0.0)
		{      
			if(dposi<0.0) 
			{        
				dPeep -= dPei_epi;
			}       
			else if(dposi>=0.0)
			{        
				dPeep += dPei_epi;        
			}
			
		}    
	}  

	double dfpe1 = (dAg==0.0 ? 0.0 : dPe / dAg);
	double dfpe2 = (dIy==0.0 ? 0.0 : dPeep / dIy * dyt);
	dfpe = dfpe1 + dfpe2; 

	return TRUE;
}

BOOL CDgnKSCE_USD05::Get_ShearDesign(BOOL bPosiI, double dPhiv, T_MATD_D& MatD, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, double dShrThk, double dfdt, double dfdb, double dVd, double dMd,
																		 CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_RPSC_SBAR& RpscSbar, T_CUMS_KSCE_BASE& CumsKsce)
{
	int iVciType = 2; // 1=자중고려함, 2=자중고려안함.
	
	//Material
	double dfck= MatD.Data1.Design.C_fc;
	double dEc = MatD.Data1.Analysis.Elast;
	double dfy = MatD.MainRebarData.B_fy;
	double dEs = MatD.MainRebarData.B_Elast;
	double dfys = MatD.SubRebarData.B_fy;
	double dEsv = MatD.SubRebarData.B_Elast;
	double dfyp = dfys; // Assumed by ZINU.('04.08.17).
	//Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy  = dyDim - dyCen;
	double dCpz  = dzDim - dzCen;
	double dArea = SpscSect.dArea;
	double dIyy  = SpscSect.dIyy;
	double dIzz  = SpscSect.dIzz;
	//Force
	double dMu = CumsKsce.dMu;
	double dVu = CumsKsce.dVu;
	 
	double dphiv = dPhiv;
	double dfpu=0.0, dfpy=0.0;
	double dAps=0.0;
	double dVn=0.0, dVc=0.0, dVs_req=0.0, dVs=0.0, dVs_min=0.0, dVpc=0.0, dVp=0.0;
	double dVci=0.0, dVci_cal=0.0, dVci_min=0.0, dVcw=0.0;
	double dVi=0.0;  //Vd : 부재의 자중만에 의해 생기는 하중계수를 고려하지 않은 전단력
	double dMmax=0.0;
	double dd=0.0, dbwd=0.0, dbwdp=0.0;
	double dbw=dShrThk;
	double dH = dzDim;
	double dyt = (dMu>=0.0 ? dzCen : dCpz); 
	double dI = dIyy;
	double dd_org=0.0;
	double ddp_org=0.0;
		
	//Gross Section
	BOOL bYaxis = TRUE;
	double dyDim_g = SpscgSect.dyDim;
	double dzDim_g = SpscgSect.dzDim;
	double dyCen_g = SpscgSect.dyCen;
	double dzCen_g = SpscgSect.dzCen;
	double dCpy_g  = dyDim_g - SpscgSect.dyCen;
	double dCpz_g  = dzDim_g - SpscgSect.dzCen;
	double dAg     = SpscgSect.dArea;
	double dIyy_g  = SpscgSect.dIyy;
	double dIzz_g  = SpscgSect.dIzz;
	double dIner_g=(bYaxis==TRUE ? dIyy_g : dIzz_g);
		
	double dCp=0.0, dCp_g=0.0;
	if(dMu>=0.0) dCp = (bYaxis==TRUE ? dCpz : dCpy);
	else         dCp = (bYaxis==TRUE ? dzCen : dyCen);
	if(dMu>=0.0) dCp_g = (bYaxis==TRUE ? dCpz_g : dCpy_g);
	else         dCp_g = (bYaxis==TRUE ? dzCen_g : dyCen_g);

	double dSc=0.0;
	if(bYaxis) 
	{
		if(dMu>=0.0) dSc = (dzCen_g==0.0 ? 0.0 : dIyy_g/dzCen_g);
		else         dSc = (dzDim_g-dzCen_g==0.0 ? 0.0 : dIyy_g/(dzDim_g-dzCen_g));    
	}
	else
	{
		if(dMu>=0.0) dSc = (dyCen_g==0.0 ? 0.0 : dIzz_g/dyCen_g);
		else         dSc = (dyDim_g-dyCen_g==0.0 ? 0.0 : dIzz_g/(dyDim_g-dyCen_g));    
	}
	
	double dyt_g = dH-dCp_g;

	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnConCent;
	arTndnConCent.RemoveAll();
	double dfpu_min = 1.7E308; 
	double dfpy_min = 1.7E308; 
	for(int i=0 ; i<arTndnPosi.GetSize() ; i++)
	{
		_DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
		_DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
		rTnfrC = rTndn;
		double dyposi = (bYaxis==TRUE ? rTndn.dz : rTndn.dy);
		double dgap =  (dMu>=0.0 ? dCp_g-dCp : dCp-dCp_g); 
		if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
		else       rTnfrC.dy = rTndn.dy+dgap;
		arTndnConCent.Add(rTnfrC);
		dfpu_min = min(rTnfrC.dfpu,dfpu_min);   dfpu = dfpu_min;
		dfpy_min = min(rTnfrC.dfpy,dfpy_min);   dfpy = dfpy_min;
	}

	// fpcc Calculation.
	double dfpe_ave=0.0, dPe=0.0, dPeep=0.0;  // dPe는 단면의 전체 텐던.
	Get_fpeData(bYaxis, dMu, dAg, dIner_g, dyt_g, arTndnConCent, dAps, dPeep, dPe, dVp, dfpe_ave);
	dVp = fabs(dVp);

	//Get ds  
	double dAptol=0.0,  ddp=0.0;
	double dAsttol=0.0, ddst=0.0;
	double dAsctol=0.0, ddsc=0.0;
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();      
	BOOL bGetOk = Get_TndnReBarInfor(TRUE, dCpz, SpscSect, arRbarPosi, arTndnPosi, rTdRb);  
	dAptol =(dMu>=0.0 ? rTdRb.dApttol : rTdRb.dApctol); 
	ddp    =(dMu>=0.0 ? rTdRb.ddpt    : dH-rTdRb.ddpc); ddp = (dAptol==0.0 ? 0.0 : ddp);    
	dAsttol=(dMu>=0.0 ? rTdRb.dAsttol : rTdRb.dAsctol); 
	ddst   =(dMu>=0.0 ? rTdRb.ddst    : dH-rTdRb.ddsc); ddst = (dAsttol==0.0 ? 0.0 : ddst);
	dAsctol=(dMu>=0.0 ? rTdRb.dAsctol : rTdRb.dAsttol); 
	ddsc   =(dMu>=0.0 ? rTdRb.ddsc    : dH-rTdRb.ddst); ddsc = (dAsctol==0.0 ? 0.0 : ddsc);
	
	dd = dd_org = ddst;  
	ddp_org = ddp;
	if( dd==0.0 || dd<0.8*dH) dd = 0.8*dH;
	dbwd = dbw*dd;
	if( ddp==0.0 || ddp<0.8*dH) ddp = 0.8*dH;
	dbwdp = dbw*ddp;  
		
	dVi  = dVu - dVd; 
	dMmax= dMu - dMd; 
	double dMcr=0.0, dfpc=0.0;
	double dfd = (dIyy_g==0.0 ? 0.0 : dMd * dyt_g / dIyy_g);  
	double dVci_incl=0.0;
	//+++++++++++++++++++++++++++++++++++++++++
	if(iVciType==1)      // 자중 고려안함
	{
		dMcr = dSc*(0.5*sqrt(dfck)+dfpe_ave-dfd);   //dfd: 외부 작용하중에 의해 인장응력이 발생하는 단면의 최외연에서의 사용 고정하중에 의한 응력  
		dVci_incl = (dMmax==0.0 ? dVd : dVd + dVi*dMcr/dMmax);
	}
	else if(iVciType==2)  // 자중 고려하는 식
	{ 
		dMcr = dSc*(0.5*sqrt(dfck)+dfpe_ave);
		dVci_incl = (dMu==0.0 ? 0.0 : dVu*dMcr/dMu);
	}
	//+++++++++++++++++++++++++++++++++++++++++  
	
	dfpc = (dAg==0.0 ? 0.0 : dPe/dAg);  //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력..
	if(m_iCode==KSCE_USD05 || m_iCode==KSCE_USD10)
	{
	  if(iVciType==1)       dVci_cal = (dMmax==0.0 ? 0.05*sqrt(dfck)*dbwd + fabs(dVd) : 0.05*sqrt(dfck)*dbwd + fabs(dVd + dVi*fabs(dMcr/dMmax)) );
		else if(iVciType==2)  dVci_cal = (dMu==0.0 ? 0.05*sqrt(dfck)*dbwd : 0.05*sqrt(dfck)*dbwd + fabs(dVu*dMcr/dMu));
		dVci_min = 0.14*sqrt(dfck)*dbwd;
		dVci = max(dVci_cal, dVci_min);  
		dVcw = (0.29*sqrt(dfck)+0.3*dfpc)*dbwd + dVp;
	}
	else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
	{
	  if(iVciType==1)       dVci_cal = (dMmax==0.0 ? 0.05*sqrt(dfck)*dbwdp + fabs(dVd) : 0.05*sqrt(dfck)*dbwdp + fabs(dVd + dVi*fabs(dMcr/dMmax)) );
		else if(iVciType==2)  dVci_cal = (dMu==0.0 ? 0.05*sqrt(dfck)*dbwdp : 0.05*sqrt(dfck)*dbwdp + fabs(dVu*dMcr/dMu));
		dVci_min = 0.14*sqrt(dfck)*dbwdp;
		dVci = max(dVci_cal, dVci_min);  
		dVcw = (0.29*sqrt(dfck)+0.3*dfpc)*dbwdp + dVp;
	}
	dVc = min(dVci, dVcw);
	double d05pVc = 0.5*dphiv*dVc;      //계수전단력 Vu가 phiVc의 1/2을 초과하는 경우
	double dpVc = dphiv*dVc;
	//Calculation dVs 
	double dAv=0.0, dsv=0.0, dThetaV=0.0;
	double dVsmax=0.0;
	dsv = RpscSbar.dSbarPitch;  
	dAv = (dsv==0.0 ? 0.0 : RpscSbar.dSbarAw);
	if(bPosiI)  dThetaV = RpscSbar.dSbarTheta;  /**/
	else        dThetaV = RpscSbar.dSbarTheta;
	dVsmax=(2./3.)*sqrt(dfck)*dbwd;
	dVs =  (dsv==0.0 ? 0.0 : dAv*dfys*(sin(dThetaV)+cos(dThetaV))*dd/dsv);
	dVs = fabs(dVs);
	dVs_req = (dphiv==0.0 ? 0.0 : (fabs(dVu)-dphiv*dVc)/dphiv);
	dVs_min = min(dVs, dVsmax);
	//Calculation dVpc
	double dAp=0.0, dsp=0.0, dThetaP=0.0;
	dsp = RpscSbar.dTndnDist;
	dAp = (dsp==0.0 ? 0.0 : RpscSbar.dTndnAp);
	if(bPosiI) dThetaP = RpscSbar.dTndnTheta;
	else       dThetaP = RpscSbar.dTndnTheta; 
	dVpc = (dsp==0.0 ? 0.0 : dAp*dfyp*(sin(dThetaP)+cos(dThetaP))*dd/dsp);
	// Change by Jaeoh. ('06.06.15). dVpc(전단강봉)는 기준에 없는 값이므로 고려하지 않는 것으로 수정
	dVn = dVc + dVs_min; // dVc + dVs_min + dVpc -> dVc + dVs_min
	double dpVn = dphiv*dVn;
	double dRatV= (dpVn==0.0 ? 0.0 : dVu/dpVn);
	//Avreq,Avmin
	double dVu_pVc = fabs(dVu)-dphiv*dVc; //-dphiv*dVpc;
	double d08rfckbwd = dphiv*0.8*sqrt(dfck)*dbwd;
	double dAv_req=0.0;
	if(dVu_pVc>0.0) dAv_req = (dphiv*dfys*(sin(dThetaV)+cos(dThetaV))*dd==0.0 ? 0.0 : dVu_pVc*dsv/(dphiv*dfys*(sin(dThetaV)+cos(dThetaV))*dd));
	else                dAv_req = 0.0;
	
	double dAvmin = (dfys==0.0 ? 0.0 : 0.35*dbw*dsv/dfys);

	double dAvmin01 =0.0;
	double dAvmin02 =0.0;
	if(m_iCode==KSCE_USD05 || m_iCode==KSCE_RAIL_USD04)
	{

	}
	else if(m_iCode==KSCE_USD10 || m_iCode==KSCE_RAIL_USD11)
	{
		dAvmin01 = (dfys==0.0 ? 0.0 : 0.0625*sqrt(dfck)*dbw*dsv/dfys);
		dAvmin02 = (dfys==0.0 ? 0.0 : 0.35*dbw*dsv/dfys);
		dAvmin = max(dAvmin01, dAvmin02);
	}
	else ASSERT(0);

	CumsKsce.dVu     =dVu;	    //면내 계수전단력		[Unit=]
	CumsKsce.dVci    =dVci;     //전단과 모멘트의 조합에 의해 사인장균열이 발생할 때 콘크리트가 부담하는 공칭 전단강도 [Unit=Force]
	CumsKsce.dVcw    =dVcw;     //복부의 과도한 주인장응력으로 인해 사인장균열이 발생할 때 콘크리트가 부담하는 공칭 전단강도			[Unit=Force]
	CumsKsce.dVc     =dVc;	 	  //콘크리트가 부담하는 공칭 전단강도			[Unit= Force]
	CumsKsce.dVs     =dVs_min;	 	  //전단철근이 부담하는 공칭 전단강도 			[Unit= Force]
	CumsKsce.dpVn    =dpVn;	    //단면의 공칭전단력		[Unit= Force]
	CumsKsce.dRatV   =dRatV;    //공칭전단력비
	CumsKsce.dAv     =dAv;		  //간격 s 이내에 있는 전단철근의 단면적	[Unit=Area]
	CumsKsce.dAvmin  =dAvmin;	  //최소전단철근	[Unit= Area]
	CumsKsce.dAvreq	 =dAv_req;	//필요전단철근	[Unit= Area]
	//설계시 사용되는 중간값
	CumsKsce.dbw     =dbw;		  //복부의 폭	[Unit=Length]
	CumsKsce.dd      =dd;		    //콘크리트압축 연단에서 인장측 철근 중심까지의 거리		[Unit=Length]
	CumsKsce.dVi     =dVi; 	    //dVu-dVi				[Unit=Force]
	CumsKsce.dVd     =dVd;		  //사용 고정하중에 의해 발생하는 단면의 전단력			[Unit=Force]
	CumsKsce.dMmax   =dMmax;	  //dMu-dMd				[Unit= Moment]
	CumsKsce.dMd     =dMd;		  //하중계수를 고려하지 않은 고정하중 모멘트	   	     [Unit=Moment]
	CumsKsce.dMcr    =dMcr;	    //외부 작용하중에 의한 단면의 휨 균열모멘트			[Unit=Moment]
	CumsKsce.dIy     =dIner_g; 	//강축 단면 2차모멘트 		[Unit=Length^4]
	CumsKsce.dh			 =dzDim;    //단면춤 		[Unit=Length]
	CumsKsce.dyt     =dyt_g;	  //총단면적의 중립축에서 인장 연단까지의 거리			[Unit=Length]
	CumsKsce.dfpe    =dfpe_ave; //손실이 일어난 후 강재의 유효프리스트레스			[Unit=Stress]
	CumsKsce.dfd     =dfd;      //외부 작용하중에 의해 인장응력이 발생하는 단면의 최외연에서의 사용 고정하중에의한 응력		 [Unit=Stress]
	CumsKsce.dfpc    =dfpc;	    //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력	 [Unit=Stress]
	CumsKsce.dVp     =dVp;      //단면의 유효프리스트레스 힘의 수직 분력				[Unit=Stress]
	CumsKsce.dfck    =dfck;	    //콘크리트 설계기준강도 			[Unit=Stress]
	CumsKsce.dsv     =dsv; 	    //경사인장철근의 간격			[Unit=Length]
	CumsKsce.dThetaV =dThetaV;  //경사인장철근의 각도			[Unit=Rad]
	CumsKsce.dsp     =dsp; 	    //전단강봉의 간격				[Unit=Length]
	CumsKsce.dThetap =dThetaP;  //전단강봉의 각도			[Unit=Rad]
	CumsKsce.dAp     =dAp; 	    //전단강봉의 배치량   [Unit= Area]


	BOOL bPrint = m_Print.Get_PrintFlag();

	if(bPrint)
	{
		m_Print.m_CumsParam.dfck = dfck;
		m_Print.m_CumsParam.dbw  = dbw;
		m_Print.m_CumsParam.ddst = dd_org;
		m_Print.m_CumsParam.dfy  = dfy;
		m_Print.m_CumsParam.dh   = dH;
		m_Print.m_CumsParam.dIy  = dIner_g;
		m_Print.m_CumsParam.dfpu = dfpu;
		m_Print.m_CumsParam.ddp  = ddp;
		m_Print.m_CumsParam.ddp_org = ddp_org;

		m_Print.m_CumsParam.dAc  = dAg;
		m_Print.m_CumsParam.dfpy = dfpy;
		m_Print.m_CumsParam.dyt  = dyt_g;
		m_Print.m_CumsParam.dAps = dAps;

		// 도로교10
		m_Print.m_CumsParam.dMu = dMu;
		m_Print.m_CumsParam.dPe = dPe;
		m_Print.m_CumsParam.dVci_cal = dVci_cal;
		m_Print.m_CumsParam.dVci_incl = dVci_incl;
		m_Print.m_CumsParam.dVci_min  = dVci_min;
		m_Print.m_CumsParam.dphiv   = dphiv;
		m_Print.m_CumsParam.dpVc    = dpVc;
		m_Print.m_CumsParam.d05pVc  = d05pVc; 
		m_Print.m_CumsParam.dVs_req = dVs_req; 
		m_Print.m_CumsParam.dVsmax  = dVsmax; 
		m_Print.m_CumsParam.dVs_min = dVs_min; 
		m_Print.m_CumsParam.dVs_cal = dVs;
		m_Print.m_CumsParam.dAvmin01 = dAvmin01; 
		m_Print.m_CumsParam.dAvmin02 = dAvmin02; 
		
		
		if(m_iCivilCode==KSCE_USD05 || m_iCivilCode==KSCE_RAIL_USD04)
		{
			m_Print.Print_CumsDesign0();
			//m_Print.Print_CumsDesign1(dfck, dbw, ddst , dfy, dH, dIner_g);
			//m_Print.Print_CumsDesign2(dfpu, ddp, dAg, dfpy, dyt_g, dAps);
			m_Print.Print_CumsDesign3(dMu, dVu, dMd, dVd, dfd, dPe, dfpe_ave);
			m_Print.Print_CumsDesign4(dH, dd, dd_org, dphiv*dH, ddp, ddp_org, dIner_g, dyt_g, dfck, dfpe_ave, dfd, dMcr);
			m_Print.Print_CumsDesign5(dMu, dMd, dMmax, dVu, dVd, dVi, dVci_cal, dVci, dVci_incl, dfck, dbw, dd, ddp, dMcr, dVci_min);
			m_Print.Print_CumsDesign6(dPe, dAg, dfpc, dfck, dbw, dd, ddp, dVp, dVcw, dVci, dVc);
			//m_Print.Print_CumsDesign7(dVc, dphiv, d05pVc, dpVc, dVu, dbw, dd, dVp, dVcw, dVci);
			m_Print.Print_CumsDesign8(dVu, dVc, dphiv, dpVc, d05pVc, dpVn, dfck, dbw, dd, dVs_req, dVsmax,
															 dVs, dVs_min, dsv, dfy, dThetaV, dAv, dAv_req, dAvmin);     
		}
		else if(m_iCivilCode==KSCE_USD10 || m_iCivilCode==KSCE_RAIL_USD11)
		{
			m_Print_ksce10.Print_CumsDesign(CumsKsce, m_Print.m_CumsParam);              
		}
		else ASSERT(0);
	}

	return TRUE;

}

BOOL CDgnKSCE_USD05::Get_TorsionDesign(BOOL bYaxis, BOOL bPosiI, double dPhiv, T_MATD_D MatD, T_SPSC_SECT& SpscSect, T_SPSC_SECT& SpscgSect, BOOL bClosedSect, double dAoh, double dPh, double dShrThk, double dTorThk, double dPcp, double dAcp, double dfdt, double dfdb, double dVd, double dMd, 
																			 CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi, T_RPSC_SBAR& RpscSbar, T_CRMT_KSCE_BASE& CrmtKsce)
{
	int iVciType = 2; // 1=자중고려함, 2=자중고려안함.

	//if(!RpscSbar.bLbarExist) return FALSE;
	//Material
	double dfck= MatD.Data1.Design.C_fc;
	double dEc = MatD.Data1.Analysis.Elast;
	double dfy = MatD.MainRebarData.B_fy;
	double dEs = MatD.MainRebarData.B_Elast;
	double dfys = MatD.SubRebarData.B_fy;
	double dEsv = MatD.SubRebarData.B_Elast;
	double dfyv = MatD.SubRebarData.B_fy; //폐쇄된 횡비틀림 철근의 항복강도.
	double dfyl = MatD.MainRebarData.B_fy; //종방향 비틀림 철근의 항복강도.
	//Section
	double dyDim = SpscSect.dyDim;
	double dzDim = SpscSect.dzDim;
	double dyCen = SpscSect.dyCen;
	double dzCen = SpscSect.dzCen;
	double dCpy  = dyDim - dyCen;
	double dCpz  = dzDim - dzCen;
	double dArea = SpscSect.dArea;
	double dIyy  = SpscSect.dIyy;
	double dH = dzDim;
	double dyt = dzCen; 
	double dI = dIyy;
	double dd_org=0.0;
	double ddp_org=0.0;
	
	//Force
	double dMu = CrmtKsce.dMu;
	double dTu = fabs(CrmtKsce.dTu);
	double drTu = CrmtKsce.dTu;
	double dVu = CrmtKsce.dVu;

	//Gross Section
	double dyDim_g = SpscgSect.dyDim;
	double dzDim_g = SpscgSect.dzDim;
	double dyCen_g = SpscgSect.dyCen;
	double dzCen_g = SpscgSect.dzCen;
	double dCpy_g  = dyDim_g - SpscgSect.dyCen;
	double dCpz_g  = dzDim_g - SpscgSect.dzCen;
	double dAg     = SpscgSect.dArea;
	double dIyy_g  = SpscgSect.dIyy;
	double dIzz_g  = SpscgSect.dIzz;
	double dIner_g=(bYaxis==TRUE ? dIyy_g : dIzz_g);

	double dfpu=0.0, dfpy=0.0;
	double dAps=0.0;
	double dd=0.0, dbwd=0.0, dbwdp=0.0;

	BOOL bTorsionChk = FALSE;
	double dCp=0.0, dCp_g=0.0;
	if(dMu>=0.0) dCp = (bYaxis==TRUE ? dCpz : dCpy);
	else         dCp = (bYaxis==TRUE ? dzCen : dyCen);
	if(dMu>=0.0) dCp_g = (bYaxis==TRUE ? dCpz_g : dCpy_g);
	else         dCp_g = (bYaxis==TRUE ? dzCen_g : dyCen_g);

	double dSc=0.0;
	if(bYaxis) 
	{
		if(dMu>=0.0) dSc = (dzCen_g==0.0 ? 0.0 : dIyy_g/dzCen_g);
		else         dSc = (dzDim_g-dzCen_g==0.0 ? 0.0 : dIyy_g/(dzDim_g-dzCen_g));    
	}
	else
	{
		if(dMu>=0.0) dSc = (dyCen_g==0.0 ? 0.0 : dIzz_g/dyCen_g);
		else         dSc = (dyDim_g-dyCen_g==0.0 ? 0.0 : dIzz_g/(dyDim_g-dyCen_g));    
	}
	
	double dyt_g = (dMu>=0.0 ? dH-dCp_g : dCp_g);

	CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnConCent;
	arTndnConCent.RemoveAll();
	double dfpu_min = 1.7E308; 
	double dfpy_min = 1.7E308; 
	for(int i=0 ; i<arTndnPosi.GetSize() ; i++)
	{
		_DGN_TNDN_CRC rTndn = arTndnPosi.GetAt(i);
		_DGN_TNDN_CRC rTnfrC; rTnfrC.Initialize();
		rTnfrC = rTndn;
		double dyposi = (bYaxis==TRUE ? rTndn.dz : rTndn.dy);
		double dgap =  (dMu>=0.0 ? dCp_g-dCp : dCp-dCp_g); 
		if(bYaxis) rTnfrC.dz = rTndn.dz+dgap;
		else       rTnfrC.dy = rTndn.dy+dgap;
		arTndnConCent.Add(rTnfrC);
		dfpu_min = min(rTnfrC.dfpu,dfpu_min);   dfpu = dfpu_min;
		dfpy_min = min(rTnfrC.dfpy,dfpy_min);   dfpy = dfpy_min;

	}

	// fpcc Calculation.
	double dfpe_ave=0.0, dPe=0.0, dPeep=0.0, dVp=0.0;;  // dPe는 단면의 전체 텐던.
	Get_fpeData(bYaxis, dMu, dAg, dIner_g, dyt_g, arTndnConCent, dAps, dPeep, dPe, dVp, dfpe_ave);
	dVp = fabs(dVp);

	//Get ds  
	double dAptol=0.0,  ddp=0.0;
	double dAsttol=0.0, ddst=0.0;
	double dAsctol=0.0, ddsc=0.0;
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();      
	BOOL bGetOk = Get_TndnReBarInfor(TRUE, dCpz, SpscSect, arRbarPosi, arTndnPosi, rTdRb);  
	dAptol =(dMu>=0.0 ? rTdRb.dApttol : rTdRb.dApctol); 
	ddp    =(dMu>=0.0 ? rTdRb.ddpt    : dH-rTdRb.ddpc); ddp = (dAptol==0.0 ? 0.0 : ddp);    
	dAsttol=(dMu>=0.0 ? rTdRb.dAsttol : rTdRb.dAsctol); 
	ddst   =(dMu>=0.0 ? rTdRb.ddst    : dH-rTdRb.ddsc); ddst = (dAsttol==0.0 ? 0.0 : ddst);
	dAsctol=(dMu>=0.0 ? rTdRb.dAsctol : rTdRb.dAsttol); 
	ddsc   =(dMu>=0.0 ? rTdRb.ddsc    : dH-rTdRb.ddst); ddsc = (dAsctol==0.0 ? 0.0 : ddsc);
	
	double dbw=dShrThk;
	double dAst=dAsttol;

	dd = dd_org = ddst;  
	ddp_org = ddp;
	if( dd==0.0 || dd<0.8*dH) dd = 0.8*dH;
	dbwd = dbw*dd;
	if( ddp==0.0 || ddp<0.8*dH) ddp = 0.8*dH;
	dbwdp = dbw*ddp;    
		
	double dVi   = dVu - dVd;
	double dMmax = dMu-dMd;
	double dfd = (dIyy_g==0.0 ? 0.0 : fabs(dMd) * dyt_g / dIyy_g);
	double dMcr=0., dVci_incl=0.;
	if(iVciType==1)      
	{
		dMcr = dSc*(0.5*sqrt(dfck)+dfpe_ave-dfd);   //dfd: 외부 작용하중에 의해 인장응력이 발생하는 단면의 최외연에서의 사용 고정하중에 의한 응력  
		dVci_incl = (dMmax==0.0 ? dVd : dVd + dVi*dMcr/dMmax);
	}
	else if(iVciType==2) 
	{
		dMcr = dSc*(0.5*sqrt(dfck)+dfpe_ave);
		dVci_incl = (dMu==0.0 ? 0.0 : dVu*dMcr/dMu);
	}
	
	double dVci_cal=0.0, dVci_min=0.0, dVci=0.0, dfpc=0.0, dVcw=0.0, dVc=0.0;
	if(m_iCode==KSCE_USD05 || m_iCode==KSCE_USD10)
	{
	  if(iVciType==1)       dVci_cal = (dMmax==0.0 ? 0.05*sqrt(dfck)*dbwd + fabs(dVd) : 0.05*sqrt(dfck)*dbwd + fabs(dVd + dVi*fabs(dMcr/dMmax)) );
		else if(iVciType==2)  dVci_cal = (dMu==0.0   ? 0.05*sqrt(dfck)*dbwd : 0.05*sqrt(dfck)*dbwd + fabs(dVu*dMcr/dMu));
		dVci_min = 0.14*sqrt(dfck)*dbwd;
		dVci = max(dVci_cal, dVci_min);
		dfpc = (dAg==0.0 ? 0.0 : dPe/dAg);  //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력..
		dVcw = (0.29*sqrt(dfck)+0.3*dfpc)*dbwd + dVp;
		dVc = min(dVci, dVcw);
	}
	else if(m_iCode==KSCE_RAIL_USD04 || m_iCode==KSCE_RAIL_USD11)
	{
	  if(iVciType==1)       dVci_cal = (dMmax==0.0 ? 0.05*sqrt(dfck)*dbwdp + fabs(dVd) : 0.05*sqrt(dfck)*dbwdp + fabs(dVd + dVi*fabs(dMcr/dMmax)) );
		else if(iVciType==2)  dVci_cal = (dMu==0.0   ? 0.05*sqrt(dfck)*dbwdp : 0.05*sqrt(dfck)*dbwdp + fabs(dVu*dMcr/dMu));
		dVci_min = 0.14*sqrt(dfck)*dbwdp;
		dVci = max(dVci_cal, dVci_min);
		dfpc = (dAg==0.0 ? 0.0 : dPe/dAg);  //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력..
		dVcw = (0.29*sqrt(dfck)+0.3*dfpc)*dbwdp + dVp;
		dVc = min(dVci, dVcw);
	}

	double dTcr=0.0;  
	double dTorSkipValue=0.0;
	double dphit = dPhiv;
	//Pcp,Acp
	//Acp : 콘크리트 단면의 바깥 둘레로 둘러싸인 단면적으로, 속빈 단면에서는 속빈 면적을 포함.  

//   dTcr         =(dPcp==0.0 ? 0.0 : 1.06*sqrt(dfck)*pow(dAcp,2.0)/dPcp*sqrt(1.0+dfpc/(1.06*sqrt(dfck))));
//   dTorSkipValue=(dPcp==0.0 ? 0.0 : dphit*0.26*sqrt(dfck)*pow(dAcp,2.0)/dPcp*sqrt(1.0+dfpc/(1.06*sqrt(dfck))));
	dTcr         =(dPcp==0.0 ? 0.0 :        1.0/3.0*sqrt(dfck)*pow(dAcp,2.0)/dPcp*sqrt(1.0+dfpc/(1.0/3.0*sqrt(dfck))));    
	dTorSkipValue=(dPcp==0.0 ? 0.0 : dphit*1.0/12.0*sqrt(dfck)*pow(dAcp,2.0)/dPcp*sqrt(1.0+dfpc/(1.0/3.0*sqrt(dfck))));
	
	// dAoh : 비틀림 저항철근의 중심선으로 폐쇄된 면적
	// dPh  : 외곽부 폐쇄 횡방향 비틀림 철근의 중심선의 둘레길이.
	//전단력과 비틀림에 의한 전단응력
	double dStrST=0.0, dStrST_lim=0.0;
	double dtw=dTorThk; 
	double dA0=0.0;
	double dthetaT=0.0;
	double dAohOverPh=0.0;
	if(bClosedSect) 
	{
		if(dbwd==0.0 && dAoh!=0.0) dStrST = dTu*dPh/(1.7*pow(dAoh,2.0));
		else if(dbwd!=0.0 && dAoh==0.0) dStrST = fabs(dVu)/dbwd;
		else if(dbwd==0.0 && dAoh==0.0) dStrST = 0.0;
		else dStrST = sqrt(pow(dVu/dbwd,2.0)+pow(dTu*dPh/(1.7*pow(dAoh,2.0)),2.0));
	}
	else
	{
		dAohOverPh = (dPh==0.0 ? 0.0 : dAoh/dPh);
		if(dtw < dAohOverPh) 
		{
			if(dbwd==0.0 && dAoh*dtw!=0.0) dStrST = dTu/(1.7*dAoh*dtw);
			else if(dbwd!=0.0 && dAoh*dtw==0.0) dStrST = fabs(dVu)/dbwd;
			else if(dbwd==0.0 && dAoh*dtw==0.0) dStrST = 0.0;
			else dStrST = fabs(dVu)/dbwd + dTu/(1.7*dAoh*dtw);
		}    
		else
		{
			if(dbwd==0.0 && dAoh!=0.0) dStrST = dTu*dPh/(1.7*pow(dAoh,2.0));
			else if(dbwd!=0.0 && dAoh==0.0) dStrST = fabs(dVu)/dbwd;
			else if(dbwd==0.0 && dAoh==0.0) dStrST = 0.0;
			else dStrST = fabs(dVu)/dbwd + dTu*dPh/(1.7*pow(dAoh,2.0));
		}    
	}  
	//dStrST_lim = (dbwd==0.0 ? 0.0 : dphit*(dVc/dbwd+2.12*sqrt(dfck))); 
	dStrST_lim = (dbwd==0.0 ? 0.0 : dphit*(dVc/dbwd+2.0/3.0*sqrt(dfck))); 
			
	double dst = RpscSbar.dLbarPitch;    // 횡방향철근의 간격
	double dAt = RpscSbar.dLbarAwt;      // 간격 s내의 비틀림에 저항하는 폐쇄스터럽의 다리 1개의 면적, 혹은 비틀림모멘트에 대한 복부 개당의 횡방향 필요 철근량.
	//압축경사각으로 30~60도 
	double dThetaC=0.0; 
	if(dAst*dfy*0.4>=dPe) dThetaC = 45.0*DEGTORAD;  //fs값 대신fy를 
	else if(dAst*dfy*0.4<dPe) dThetaC = 37.5*DEGTORAD;
	else if(dAst==0.0) dThetaC = 45.0*DEGTORAD;  
	dA0 = 0.85*dAoh;  
	double dTn = (dst==0.0 || sin(dThetaC)==0.0 ? 0.0 : 2.0*dA0*dAt*dfyv*cos(dThetaC)/sin(dThetaC)/dst);
	double dpTn= dphit*dTn;
	double dRatT= (dpTn==0.0 ? 0.0 : dTu/dpTn);
	double dAluse = RpscSbar.dLbarAlt;
	double dAlreq = (dst==0.0 || dfyl==0.0 || sin(dThetaC)==0.0 ? 0.0 : dAt/dst*dPh*(dfyv/dfyl)*pow(cos(dThetaC)/sin(dThetaC),2));
	double dAv = RpscSbar.dSbarAw;
	double dAtmin =0.0;
	double dAtmin01 =0.0;
	double dAtmin02 =0.0;
	double dAtmin_cal =0.0;
	if(m_iCode==KSCE_USD05 || m_iCode==KSCE_RAIL_USD04)
	{
		if(0.35*dbw*dst/dfyv-dAv>0.0) dAtmin = (0.35*dbw*dst/dfyv-dAv)/2.0;
		else                          dAtmin = 0.0;   
	}
	else if(m_iCode==KSCE_USD10 || m_iCode==KSCE_RAIL_USD11)
	{
		dAtmin01 = (dfys==0.0 ? 0.0 : 0.0625*sqrt(dfck)*dbw*dst/dfys);
		dAtmin02 = (dfys==0.0 ? 0.0 : 0.35*dbw*dst/dfys);
		dAtmin_cal = max(dAtmin01, dAtmin02);

		if(dAtmin_cal-dAv>0.0) dAtmin = (dAtmin_cal-dAv)/2.0;
		else                   dAtmin = 0.0;
	}
	else ASSERT(0);

	double dAtst = (dst==0.0 ? 0.0 : dAt/dst);
	double dbwfyv =  (dfyv==0.0 ? 0.0 : 0.175*dbw/dfyv);
	double AtOverS = max(dAtst, dbwfyv);
	double dAlmin = (dfyl==0.0 ? 0.0 : 0.42*sqrt(dfck)*dAcp/dfyl-AtOverS*dPh*dfyv/dfyl);
	double dAlmin_cal = dAlmin;
	
	if(fabs(dTorSkipValue) <= fabs(dTu)) bTorsionChk = TRUE;

	// Test +++++++++++++++++++++++
	//dTorSkipValue = 1.0E+20; //!/
	//bTorsionChk = TRUE;
	// ++++++++++++++++++++++++++++
		
	CrmtKsce.dTu		 =drTu;		          //계수 비틀림모멘트					[Unit=Moment]
	//CrmtKsce.dTcr    =dTcr;           //비틀림 균열 모멘트 					[Unit=Moment]		
	CrmtKsce.dTcr    =dTorSkipValue;    // 1/4Tcr
	CrmtKsce.dAoh    =dAoh;             //비틀림 저항철근의 중심선으로 폐쇄된 면적	[Unit=Area]
	CrmtKsce.dVc	 	 =dVc;	 	          //콘크리트가 부담하는 공칭 전단강도			[Unit= Force]
	CrmtKsce.dvVT	   =dStrST;	          //전단력과 비틀림에 의한 전단응력 			[Unit= Stress]
	CrmtKsce.dvVTmax =dStrST_lim;	      //전단균열을 유발하는 응력과 제한응력의 합	[Unit= Stress]
	CrmtKsce.dpTn	   =dpTn;	            //공칭비틀림 모멘트 					[Unit=Torsion]
	CrmtKsce.dRatT   =dRatT;            //비틀림강도 안전률					[Unit=None]
	CrmtKsce.dAt		 =dAt;		          //간격 s 내의 비틀림에 저항하는 폐쇄스터럽의 다리 1개의 면적	[Unit=Area]
	CrmtKsce.dAtmin	 =dAtmin;	          //횡방향 폐쇄스터럽의 최소 면적			[Unit= Area]
	CrmtKsce.dAluse	 =dAluse;	          //종방향 비틀림 철근의 총면적				[Unit=Area]
	CrmtKsce.dAlreq  =dAlreq;           //종방향 비틀림 철근의 총면적				[Unit=Area]
	CrmtKsce.dAlmin	 =dAlmin;	          //최소 종방향 비틀림 철근량				[Unit= Area]
	CrmtKsce.dAcp	   =dAcp;	            //콘크리트 단면의 바깥 둘레로 둘러싸인 단면적		[Unit= Area]
	CrmtKsce.dPcp	   =dPcp;	            //전단면의 둘레의 길이						[Unit=Length]
	CrmtKsce.dfpc 	 =dfpc; 	          //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력	 [Unit=Stress]
	CrmtKsce.dfck	   =dfck;	            //콘크리트 설계기준강도 					[Unit=Stress]
	CrmtKsce.dVu		 =dVu;		          //단면의 계수 전단력					[Unit= Force]
	CrmtKsce.dPh		 =dPh;		          //외곽부 폐쇄 횡방향 비틀림 철근의 중심선의 둘레길이     [Unit= Length]
	CrmtKsce.dbw		 =dbw;		          //복부의 폭							[Unit=Length]
	CrmtKsce.dt 		 =dtw;		          //벽의 두께 				 			[Unit=Length]
	CrmtKsce.dThetaC =dThetaC;          //압축경사각 			 			[Unit=Rad]
	CrmtKsce.dfyv	   =dfyv;	            //폐쇄된 횡비틀림 철근의 항복강도	  		[Unit=Stress]
	CrmtKsce.dfyl    =dfyl;             //종방향 비틀림 철근의 항복강도			[Unit=Stress]

	BOOL bPrint = m_Print.Get_PrintFlag();
	if(bPrint)
	{
		m_Print.m_CrmtParam.dfck = dfck;
		m_Print.m_CrmtParam.dbw  = dbw;
		m_Print.m_CrmtParam.dt   = dtw;
		m_Print.m_CrmtParam.dfy  = dfy;
		m_Print.m_CrmtParam.dh   = dH;
		m_Print.m_CrmtParam.dIy  = dIner_g;

		m_Print.m_CrmtParam.dfpu = dfpu;
		m_Print.m_CrmtParam.dd   = dd;
		m_Print.m_CrmtParam.ddp  = ddp;
		m_Print.m_CrmtParam.dd_org   = dd_org;
		m_Print.m_CrmtParam.ddp_org  = ddp_org;

		m_Print.m_CrmtParam.dAc  = dAg;
		m_Print.m_CrmtParam.dfpy = dfpy;
		m_Print.m_CrmtParam.dyt  = dyt_g;
		m_Print.m_CrmtParam.dAps = dAps;
		m_Print.m_CrmtParam.dPcp = dPcp;
		m_Print.m_CrmtParam.dAcp = dAcp;

		// 도로교10
		m_Print.m_CrmtParam.dMu = dMu;
		m_Print.m_CrmtParam.dMd = dMd;
		m_Print.m_CrmtParam.dVd = dVd;
		m_Print.m_CrmtParam.dfd = dfd;
		m_Print.m_CrmtParam.dPe = dPe;
		m_Print.m_CrmtParam.dfpe = dfpe_ave;

		m_Print.m_CrmtParam.dTcr = dTcr;  // 실제Tcr,  dTorSkipValue 아님;    
		m_Print.m_CrmtParam.dphit = dphit;
		m_Print.m_CrmtParam.bTorsionChk = bTorsionChk;
		m_Print.m_CrmtParam.dMcr = dMcr;
		m_Print.m_CrmtParam.dMmax     = dMmax;
		m_Print.m_CrmtParam.dVi       = dVi;
		m_Print.m_CrmtParam.dVci_cal  = dVci_cal;
		m_Print.m_CrmtParam.dVci      = dVci;
		m_Print.m_CrmtParam.dVci_incl = dVci_incl;
		m_Print.m_CrmtParam.dVci_min  = dVci_min;
		m_Print.m_CrmtParam.dVp  = dVp;
		m_Print.m_CrmtParam.dVcw = dVcw;    
		m_Print.m_CrmtParam.bClosedSect = bClosedSect;
		m_Print.m_CrmtParam.dAohOverPh  = dAohOverPh;

		m_Print.m_CrmtParam.dA0 = dA0;
		m_Print.m_CrmtParam.dst = dst;
		m_Print.m_CrmtParam.dTn = dTn;
		m_Print.m_CrmtParam.dAlmin_cal = dAlmin_cal;
		m_Print.m_CrmtParam.dAv = dAv;
		m_Print.m_CrmtParam.dAtmin01   =dAtmin01  ;
		m_Print.m_CrmtParam.dAtmin02   =dAtmin02  ;
		m_Print.m_CrmtParam.dAtmin_cal =dAtmin_cal;
		m_Print.m_CrmtParam.dAtst   = dAtst;
		m_Print.m_CrmtParam.dbwfyv  = dbwfyv;
		m_Print.m_CrmtParam.AtOverS = AtOverS;
			
		//
			 
		if(m_iCivilCode==KSCE_USD05 || m_iCivilCode==KSCE_RAIL_USD04)
		{     
			m_Print.Print_CrmtDesign0();
			//m_Print.Print_CrmtDesign1(dfck, dbw, dtw , dfy, dH, dIner_g);
			//m_Print.Print_CrmtDesign2(dfpu, dd, ddp, dAg, dfpy, dyt, dAps, dPcp, dAcp);
			m_Print.Print_CrmtDesign3(dMu, drTu, dVu, dMd, dVd, dfd, dPe, dfpe_ave);
			m_Print.Print_CrmtDesign4(dfck, dAcp, dPcp, dfpc, dTorSkipValue, drTu, dfpe_ave);
			
			if(bTorsionChk)
			{
				m_Print.Print_CrmtDesign5();
				//콘크리트가 부담하는 전단강도의 산정은 전단 검토와 같음.
				m_Print.Print_CumsDesign4(dH, dd, dd_org, dPhiv*dH, ddp, ddp_org, dIner_g, dyt_g, dfck, dfpe_ave, dfd, dMcr);
				m_Print.Print_CumsDesign5(dMu, dMd, dMmax, dVu, dVd, dVi, dVci_cal, dVci, dVci_incl, dfck, dbw, dd, ddp, dMcr, dVci_min);
				m_Print.Print_CumsDesign6(dPe, dAg, dfpc, dfck, dbw, dd, ddp, dVp, dVcw, dVci, dVc);
				
				m_Print.Print_CrmtDesign6(bClosedSect, dAohOverPh, dVu, dVc, dbw, dd, ddp, drTu, 
					dfck, dTorSkipValue, dPh, dAoh, dtw, dStrST, dStrST_lim);
				m_Print.Print_CrmtDesign7(dAoh, dA0, dAt, dfyv, dst, dThetaC, dpTn, dfyl, dPh, dAlmin);
				m_Print.Print_CrmtDesign8(dbw, dst, dfyv, dAv, dAtmin, dfck, dAcp, dAt, dPh, dfyl, dAlmin, dAtst, dbwfyv, AtOverS);    
			}
		}
		else if(m_iCivilCode==KSCE_USD10 || m_iCivilCode==KSCE_RAIL_USD11)
		{
			m_Print_ksce10.Print_CrmtDesign(CrmtKsce, m_Print.m_CrmtParam);              

		}
		else ASSERT(0);


	}
	

	return TRUE;
			
}

BOOL CDgnKSCE_USD05::CalcTorsion_Pcp_Acp(CArray<T_SPSC_COOR,T_SPSC_COOR>& arOuter, double& dPcp, double& dAcp)
{
	T_SPSC_COOR aCoor = arOuter[0];
	UINT iNumPoint = arOuter[0].arSpscPont[0].GetSize();
	double dxC=0.0, dyC=0.0;
	double* dpx = new double[iNumPoint];
	double* dpy = new double[iNumPoint];
	//Pcp
	for(int i=0; i<iNumPoint; i++)
	{
		int iPrev = i;
		int iNext = (i<iNumPoint-1 ? i+1 : 0);
		double dy1 = aCoor.arSpscPont[0].GetAt(iPrev);
		double dz1 = aCoor.arSpscPont[1].GetAt(iPrev);
		double dy2 = aCoor.arSpscPont[0].GetAt(iNext);
		double dz2 = aCoor.arSpscPont[1].GetAt(iNext);			
		dPcp += sqrt(pow((dy1-dy2),2)+pow((dz1-dz2),2));

		dpx[i] = aCoor.arSpscPont[0].GetAt(i);
	  dpy[i] = aCoor.arSpscPont[1].GetAt(i);
	}
	//Acp  
	if(!CMathFunc::mathPolyCentroid(iNumPoint,dpx,dpy,dxC,dyC,dAcp))	ASSERT(0);
	delete [] dpx; delete [] dpy;
	
	return TRUE;
	
}

double CDgnKSCE_USD05::Get_Reduc()
{
	double dR=0.8;
	return dR; 
}

BOOL CDgnKSCE_USD05::Get_ReqTendon(ElemPairK ElemK, UINT iIJ, UINT iTopBot, _PSC_ALWS_POSI& AlwsPosi, double dFlgThk[2], 
																	 T_SPSC_SECT& SpscgSect, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbarPosi,
																	 CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndnPosi, T_TNDN_KSCE_BASE& TndnKsce)
{
	// Material.

	double dfti = (iTopBot==0 ? AlwsPosi.dfti[0] : AlwsPosi.dfti[1]);
	double dfci = AlwsPosi.dfci;
	double dfte = (iTopBot==0 ? AlwsPosi.dfte[0] : AlwsPosi.dfte[1]);
	double dfce = AlwsPosi.dfce;

	// Section.
	double dh    = SpscgSect.dzDim;   // 높이
	double dyb   = SpscgSect.dzCen;   // 중립축에서 아래연단 까지 거리.
	double dyt   = dh - dyb;          // 중립축에서 위연단 까지 거리.
	double dAg   = SpscgSect.dArea;   // 전단면적
	double dIyy  = SpscgSect.dIyy;    // Iyy
	// T_SPSC_SECT, Slt, Srt, Slb, Srb는 약축에 대한 단면상수..
	double dSlt  = SpscgSect.dSlt;
	double dSrt  = SpscgSect.dSrt;
	double dSlb  = SpscgSect.dSlb;
	double dSrb  = SpscgSect.dSrb;
	double dSt   = max(dSlt, dSrt);
	double dSb   = max(dSlb, dSrb);

	if(dIyy==0.0) return FALSE;
	/////////////////////////
	dSt = dIyy/dyt;
	dSb = dIyy/dyb;
	/////////////////////////

	double dFlgThk_Top = dFlgThk[0];
	double dFlgThk_Bot = dFlgThk[1];
	// 직사각형일 경우 flange 두께가 전체높이임. 
	if(fabs(dFlgThk_Top-dh)<m_dZero) 
	{    
		dFlgThk_Top = dyt/2.0;
		dFlgThk_Bot = dyb/2.0;
	}

	// Moment. 
	double dM_DL = TndnKsce.dMd;
	double dMa   = TndnKsce.dMa;

	double dR = Get_Reduc();   
	double dr2 = sqrt(dIyy/dAg);
	double dkb = dr2/dyt; // 하핵점 : 위연단에 인장응력이 발생하지 않는 압축단면력의 한계위치.
	double dkt = dr2/dyb; // 상핵점 : 아래연단에 인장응력이 발생하지 않는 압축단면력의 한계위치.  
	
	// e : 중립축 아래(+), 중립축 위(-) 
	BOOL bPi_Magnel = TRUE;
	BOOL bPi_Check = TRUE;
	double dPi_use = 0.0;
	double dPi_mid = 0.0;
	
	// 중립축을 기준으로 위 아래 텐던의 도심.
	double de = 0.0; 

	//중립축을 기준으로 Tendon dy,dz를 수정.
	double dApt=0.0, dNtodpt=0.0, dapt=0.0;
	double dApc=0.0, dNtodpc=0.0, dapc=0.0;
	double dAst=0.0, dNtodst=0.0, dast=0.0;
	double dAsc=0.0, dNtodsc=0.0, dasc=0.0;  

	double dCtemp = dyt;
	_DGN_TNDN_RBAR rTdRb;
	rTdRb.Initialize();      
	BOOL bCalcOK = Get_TndnReBarInfor(TRUE, dCtemp, SpscgSect, arRbarPosi, arTndnPosi, rTdRb);
	if(iTopBot==0) // 0=Top, 1=Bot
	{
		double dAtndn_Top = rTdRb.dApctol;
		if( dAtndn_Top>0.0 )
		{
			de = -rTdRb.dNtodpc;
		}
		else if( dAtndn_Top<=0.0 )
		{
			de = -(dyt - dFlgThk_Top/2.0); 
		}
	}
	else if(iTopBot==1)
	{
		double dAtndn_Bot = rTdRb.dApttol;    
		if( dAtndn_Bot>0.0 )
		{
			de = rTdRb.dNtodpt;
		}
		else if( dAtndn_Bot<=0.0 )
		{
			de = dyb - dFlgThk_Bot/2.0;      
		}    
	}

	// 재하능력극한상태로 계산할 때 필요.
	if(dMa>=0.0)  
	{
		dApt=rTdRb.dApttol, dNtodpt=(dApt==0.0 ? 0.0 : rTdRb.dNtodpt), dapt=(dApt==0.0 ? 0.0 : dh - rTdRb.ddpt);
		dApc=rTdRb.dApctol, dNtodpc=(dApc==0.0 ? 0.0 : rTdRb.dNtodpc), dapc=rTdRb.ddpc;
		dAst=rTdRb.dAsttol, dast=(dAst==0.0 ? 0.0 : dh - rTdRb.ddst);
		dAsc=rTdRb.dAsctol, dasc=rTdRb.ddsc;
	}
	else if(dMa<0.0)
	{
		dApt=rTdRb.dApctol, dNtodpt=(dApt==0.0 ? 0.0 : rTdRb.dNtodpc), dapt=rTdRb.ddpc;
		dApc=rTdRb.dApttol, dNtodpc=(dApc==0.0 ? 0.0 : rTdRb.dNtodpt), dapc=(dApc==0.0 ? 0.0 : dh - rTdRb.ddpt);
		dAst=rTdRb.dAsctol, dast=rTdRb.ddsc;
		dAsc=rTdRb.dAsttol, dasc=(dAsc==0.0 ? 0.0 : dh - rTdRb.ddst);
	}

	// Magnel.    
	double dPti=0.0;
	double dPci=0.0;
	double dPte=0.0;
	double dPce=0.0;
	double dPi_min=0.0;
	double dPi_max=0.0;
	double dPi_req=0.0;

	double dParam_te=0.0;
	double dParam_ce=0.0;
	double dParam_ci=0.0;
	double dParam_ti=0.0;
	double dPti_Top=0.0;
	double dPti_Bot=0.0;
	double dPci_Top=0.0;
	double dPci_Bot=0.0;
	double dParam_ci_Top=0.0;
	double dParam_ti_Bot=0.0;
	double dParam_ti_Top=0.0;
	double dParam_ci_Bot=0.0;

	if(dM_DL>=0.0 && dMa>=0.0)
	{
		// 정모멘트.    
		// 식(A) : 위연단 시공단계 허용인장응력을 만족할 때.
		double dPti=0.0;
		double dParam_ti = 1.0/dAg-de/dSt;
		if( dParam_ti != 0.0 )
		{
			dPti = (-dfti-dM_DL/dSt) / dParam_ti;
			// if(dParam_ti>0.0) {}   // dPi_scope > dPti
			// if(dParam_ti<0.0) {}   // dPi_scope < dPti
		}
		
		// 식(B) : 아래연단 시공단계 허용압축응력을 만족할 때. 
		double dParam_ci = 1.0/dAg+de/dSb;
		double dPci = (dfci+dM_DL/dSb)/dParam_ci;
		// dPi_scope < dPci

		// 식(C) : 위연단 사용단계 허용압축응력을 만족할 때.
		double dPce=0.0;
		double dParam_ce = dR*(1.0/dAg-de/dSt);
		if( dParam_ce != 0.0 )
		{
			dPce = (dfce-dMa/dSt) / dParam_ce;
			// if(dParam_ce>0.0) {}   // dPi_scope < dPce
			// if(dParam_ce<0.0) {}   // dPi_scope > dPce
		}

		// 식(D) : 아래연단 사용단계 허용인장응력을 만족할 때.
		double dParam_te = dR*(1.0/dAg+de/dSb);
		double dPte = (-dfte+dMa/dSb)/dParam_te;
		// dPi_scope > dPte

		if( dParam_ti>0.0 )
		{
			// dParam_ce>0.0 이다.
			
			// dPi_scope > dPti
			// dPi_scope < dPci
			// dPi_scope < dPce
			// dPi_scope > dPte
			dPi_min = max(dPti, dPte);
			dPi_max = min(dPci, dPce);
			if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
			else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
			else                       { dPi_req = dPi_min; }                           
		}
		else if( dParam_ti<0.0 )
		{
			// dParam_ce<0.0 이다.
			
			// dPi_scope < dPti
			// dPi_scope < dPci
			// dPi_scope > dPce
			// dPi_scope > dPte
			dPi_min = max(dPce,dPte);
			dPi_max = min(dPti,dPci);
			if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
			else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
			else                       { dPi_req = dPi_min; }
		}
		else //dParam_ti==0.0
		{
			// dPi_scope < dPci
			// dPi_scope > dPte
			dPi_min = dPte;
			dPi_max = dPci;
			if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
			else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
			else                       { dPi_req = dPi_min; }
		}
	}

	else if(dM_DL<=0.0 && dMa<=0.0)
	{
		if(dM_DL!=0.0 && dMa!=0.0)
		{
			// 부모멘트.    
			// 식(A) : 위연단 사용단계 허용인장응력을 만족할 때.
			dPte=0.0;
			dParam_te = dR*(1.0/dAg-de/dSt);
			if( dParam_te != 0.0 )
			{
				dPte = (-dfte-dMa/dSt) / dParam_te;
				// if(dParam_te>0.0) {}   // dPi_scope > dPte
				// if(dParam_te<0.0) {}   // dPi_scope < dPte
			}
		
			// 식(B) : 아래연단 사용단계 허용압축응력을 만족할 때. 
			dParam_ce = dR*(1.0/dAg+de/dSb);
			dPce = (dfce+dMa/dSb)/dParam_ce;
			// dPi_scope < dPce

			// 식(C) : 위연단 시공단계 허용압축응력을 만족할 때.
			dPce=0.0;
			dParam_ci = (1.0/dAg-de/dSt);
			if( dParam_ci != 0.0 )
			{
				dPci = (dfci-dM_DL/dSt) / dParam_ci;
				// if(dParam_ci>0.0) {}   // dPi_scope < dPci
				// if(dParam_ci<0.0) {}   // dPi_scope > dPci
			}

			// 식(D) : 아래연단 시공단계 허용인장응력을 만족할 때.
			dParam_ti = (1.0/dAg+de/dSb);
			dPti = (-dfti+dM_DL/dSb)/dParam_ti;
			// dPi_scope > dPti

			if( dParam_te>0.0 )
			{
				// dParam_ce>0.0 이다.
			
				// dPi_scope > dPte
				// dPi_scope < dPce
				// dPi_scope < dPci
				// dPi_scope > dPti
				dPi_min = max(dPte, dPti);
				dPi_max = min(dPce, dPci);
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }                           
			}
			else if( dParam_te<0.0 )
			{
				// dParam_ce<0.0 이다.
			
				// dPi_scope < dPte
				// dPi_scope < dPce
				// dPi_scope > dPci
				// dPi_scope > dPti
				dPi_min = max(dPci,dPti);
				dPi_max = min(dPte,dPce);
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }
			}
			else //dParam_te==0.0
			{
				// dPi_scope < dPce
				// dPi_scope > dPti
				dPi_min = dPti;
				dPi_max = dPce;
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }
			}
		}

	}

	// 시공단계 정모멘트 , 사용단계 부모멘트.
	else if(dM_DL>=0.0 && dMa<=0.0)
	{
		if(dM_DL!=0.0 && dMa!=0.0)
		{
			// 정모멘트.    
			// 식(A) : 위연단 시공단계 허용인장응력을 만족할 때.
			double dParam_ti_Top = 1.0/dAg-de/dSt;
			if( dParam_ti_Top != 0.0 )
			{
				double dPti_Top = (-dfti-dM_DL/dSt) / dParam_ti_Top;
				// if(dParam_ti_Top>0.0)  // dPi_scope > dPti_Top
				// if(dParam_ti_Top<0.0)  // dPi_scope < dPti_Top
			}
		
			// 식(B) : 아래연단 시공단계 허용압축응력을 만족할 때. 
			double dParam_ci_Bot = 1.0/dAg+de/dSb;
			if( dParam_ci_Bot !=0.0 )
			{
				double dPci_Bot = (dfci+dM_DL/dSb)/dParam_ci_Bot;
				// if(dParam_ci_Bot>0.0)  // dPi_scope < dPci_Bot
				// if(dParam_ci_Bot<0.0)  // dPi_scope > dPci_Bot
			}
			
			// 식(C) : 위연단 시공단계 허용압축응력을 만족할 때.
			double dParam_ci_Top = 1.0/dAg-de/dSt;
			if( dParam_ci_Top != 0.0 )
			{
				double dPci_Top = (dfci-dM_DL/dSt) / dParam_ci_Top;
				// if(dParam_ci_Top>0.0)  // dPi_scope < dPci_Bot
				// if(dParam_ci_Top<0.0)  // dPi_scope > dPci_Bot
			}
			
			// 식(D) : 아래연단 시공단계 허용인장응력을 만족할 때.
			double dParam_ti_Bot = 1.0/dAg+de/dSb;
			if( dParam_ti_Bot != 0.0 )
			{
				double dPti_Bot = (-dfti+dM_DL/dSb)/dParam_ti_Bot;
				// if(dParam_ti_Bot>0.0)  // dPi_scope > dPti_Bot
				// if(dParam_ti_Bot<0.0)  // dPi_scope < dPti_Bot
			}
			

			// 식(E) : 위연단 사용단계 허용인장응력을 만족할 때.
			double dParam_te = dR*(1.0/dAg-de/dSt);
			if( dParam_te != 0.0 )
			{
				double dPte = (-dfte-dMa/dSt) / dParam_te;
				// if(dParam_te>0.0) {}   // dPi_scope > dPte
				// if(dParam_te<0.0) {}   // dPi_scope < dPte
			}

			// 식(F) : 아래연단 사용단계 허용인장응력을 만족할 때.
			double dParam_ce = dR*(1.0/dAg+de/dSb);
			if( dParam_ce != 0.0 )
			{
				double dPce = (dfce+dMa/dSb)/dParam_ce;
				// if(dParam_ce>0.0) {}   // dPi_scope < dPce
				// if(dParam_ce<0.0) {}   // dPi_scope > dPce
			}
			

			if( dParam_ti_Top>0.0 && dParam_ci_Top>0.0 )
			{
				// if dParam_ti_Top>0, ( dParam_ci_Bot>0, dParam_te>0 )
				// if dParam_ci_Top>0, ( dParam_ti_Bot>0, dParam_ce>0 ) 

				// (A)
				// if(dParam_ti_Top>0) {}   // dPi_scope > dPti_Top
				// if(dParam_ti_Top<0) {}   // dPi_scope < dPti_Top
				// (B)
				// if(dParam_ci_Top>0) {}   // dPi_scope < dPci_Top
				// if(dParam_ci_Top<0) {}   // dPi_scope > dPci_Top
				// (C)
				// if(dParam_ci_Bot>0) {}   // dPi_scope < dPci_Bot
				// if(dParam_ci_Bot<0) {}   // dPi_scope > dPci_Bot
				// (D)
				// if(dParam_ti_Bot>0) {}   // dPi_scope > dPti_Bot
				// if(dParam_ti_Bot<0) {}   // dPi_scope < dPti_Bot
				// (E)
				// if(dParam_te>0.0) {}   // dPi_scope > dPte
				// if(dParam_te<0.0) {}   // dPi_scope < dPte
				// (F)
				// if(dParam_ce>0.0) {}   // dPi_scope < dPce
				// if(dParam_ce<0.0) {}   // dPi_scope > dPce
				
				// if(dParam_ti_Top>0) {}   // dPi_scope > dPti_Top
				// if(dParam_ci_Top>0) {}   // dPi_scope < dPci_Top
				// if(dParam_ci_Bot>0) {}   // dPi_scope < dPci_Bot
				// if(dParam_ti_Bot>0) {}   // dPi_scope > dPti_Bot
				// if(dParam_te>0.0) {}     // dPi_scope > dPte
				// if(dParam_ce>0.0) {}     // dPi_scope < dPce


				dPi_min = max( max(dPti_Top, dPti_Bot), dPte );
				dPi_max = min( min(dPci_Top, dPci_Bot), dPce );
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }                           
			}
			else if( dParam_ti_Top<0.0 && dParam_ci_Top<0.0 )
			{
				// if(dParam_ti_Top<0) {}   // dPi_scope < dPti_Top
				// if(dParam_ci_Top<0) {}   // dPi_scope > dPci_Top
				// if(dParam_ci_Bot<0) {}   // dPi_scope > dPci_Bot
				// if(dParam_ti_Bot<0) {}   // dPi_scope < dPti_Bot
				// if(dParam_te<0.0) {}     // dPi_scope < dPte
				// if(dParam_ce<0.0) {}     // dPi_scope > dPce
			
				dPi_min = max( max(dPci_Top, dPci_Bot), dPce );
				dPi_max = min( min(dPti_Top, dPti_Bot), dPte );
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }
			}
			else if( dParam_ti_Top>0.0 && dParam_ci_Top<0.0 )
			{
				// if dParam_ti_Top>0, ( dParam_ci_Bot>0, dParam_te>0 )
				// if dParam_ci_Top>0, ( dParam_ti_Bot>0, dParam_ce>0 ) 

				// if(dParam_ti_Top>0) {}   // dPi_scope > dPti_Top
				// if(dParam_ci_Top<0) {}   // dPi_scope > dPci_Top
				// if(dParam_ci_Bot>0) {}   // dPi_scope < dPci_Bot
				// if(dParam_ti_Bot<0) {}   // dPi_scope < dPti_Bot
				// if(dParam_te>0.0) {}     // dPi_scope > dPte
				// if(dParam_ce<0.0) {}     // dPi_scope > dPce

				dPi_min = max( max(dPti_Top, dPci_Top), max(dPte, dPce));
				dPi_max = min(dPte, dPce);
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }
			}
			else if( dParam_ti_Top<0.0 && dParam_ci_Top>0.0 )
			{ 
				// if(dParam_ti_Top<0) {}   // dPi_scope < dPti_Top
				// if(dParam_ci_Top>0) {}   // dPi_scope < dPci_Top
				// if(dParam_ci_Bot<0) {}   // dPi_scope > dPci_Bot
				// if(dParam_ti_Bot>0) {}   // dPi_scope > dPti_Bot
				// if(dParam_te<0.0) {}     // dPi_scope < dPte
				// if(dParam_ce>0.0) {}     // dPi_scope < dPce

				dPi_min = max(dPci_Bot, dPti_Bot);
				dPi_max = min( (dPti_Top, dPci_Top), min(dPte, dPce) );
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }        
			}
		}
	}
	
	// 시공단계 부모멘트 , 사용단계 정모멘트.
	else if(dM_DL<=0.0 && dMa>=0.0)
	{
		if(dM_DL!=0.0 && dMa!=0.0)
		{
			// 부모멘트.    
			// 식(A) : 위연단 시공단계 허용압축응력을 만족할 때.
			dParam_ci_Top = 1.0/dAg-de/dSt;
			if( dParam_ci_Top != 0.0 )
			{
				dPci_Top = (dfci-dM_DL/dSt) / dParam_ci_Top;
				// if(dParam_ci_Top>0.0) {}   // dPi_scope < dPci_Top
				// if(dParam_ci_Top<0.0) {}   // dPi_scope > dPci_Top
			}
		
			// 식(B) : 아래연단 시공단계 허용인장응력을 만족할 때. 
			dParam_ti_Bot = 1.0/dAg+de/dSb;
			if( dParam_ti_Bot !=0.0 )
			{
				dPti_Bot = (-dfti+dM_DL/dSb)/dParam_ti_Top;
				// if(dParam_ti_Bot>0.0) {}   // dPi_scope > dPti_Bot
				// if(dParam_ti_Bot<0.0) {}   // dPi_scope < dPti_Bot
			}
			
			// 식(C) : 위연단 시공단계 허용인장응력을 만족할 때.
			dParam_ti_Top = 1.0/dAg-de/dSt;
			if( dParam_ti_Top != 0.0 )
			{
				dPti_Top = (-dfti-dM_DL/dSt) / dParam_ti_Top;
				// if(dParam_ti_Top>0.0) {}   // dPi_scope > dPti_Top
				// if(dParam_ti_Top<0.0) {}   // dPi_scope < dPti_Top
			}
			
			// 식(D) : 아래연단 시공단계 허용압축응력을 만족할 때.
			dParam_ci_Bot = 1.0/dAg+de/dSb;
			if( dParam_ci_Bot != 0.0 )
			{
				dPci_Bot = (dfci+dM_DL/dSb)/dParam_ci_Bot;
				// if(dParam_ci_Bot>0.0) {}   // dPi_scope < dPci_Bot
				// if(dParam_ci_Bot<0.0) {}   // dPi_scope > dPci_Bot
			}

			// 식(E) : 위연단 사용단계 허용압축응력을 만족할 때.
			dParam_ce = dR*(1.0/dAg-de/dSt);
			if( dParam_ce != 0.0 )
			{
				dPce = (dfce-dMa/dSt) / dParam_ce;
				// if(dParam_ce>0.0) {}   // dPi_scope < dPce
				// if(dParam_ce<0.0) {}   // dPi_scope > dPce
			}

			// 식(F) : 아래연단 사용단계 허용인장응력을 만족할 때.
			dParam_te = dR*(1.0/dAg+de/dSb);
			if( dParam_te != 0.0 )
			{
				dPte = (-dfte+dMa/dSb)/dParam_te;
				// if(dParam_te>0.0) {}   // dPi_scope > dPte
				// if(dParam_te<0.0) {}   // dPi_scope < dPte
			}      

			if( dParam_ci_Top>0.0 && dParam_ti_Bot>0.0 )
			{
				// if dParam_ci_Top>0, ( dParam_ti_Top>0, dParam_ce>0 )
				// if dParam_ti_Bot>0, ( dParam_ci_Bot>0, dParam_te>0 ) 

				// (A)
				// if(dParam_ci_Top>0.0)  // dPi_scope < dPci_Top
				// if(dParam_ci_Top<0.0)  // dPi_scope > dPci_Top
				// (B)
				// if(dParam_ti_Bot>0.0)  // dPi_scope > dPti_Bot
				// if(dParam_ti_Bot<0.0)  // dPi_scope < dPti_Bot
				// (C)
				// if(dParam_ti_Top>0.0)  // dPi_scope > dPti_Top
				// if(dParam_ti_Top<0.0)  // dPi_scope < dPti_Top
				// (D)
				// if(dParam_ci_Bot>0.0)  // dPi_scope < dPci_Bot
				// if(dParam_ci_Bot<0.0)  // dPi_scope > dPci_Bot
				// (E)
				// if(dParam_ce>0.0)      // dPi_scope < dPce
				// if(dParam_ce<0.0)      // dPi_scope > dPce
				// (F)
				// if(dParam_te>0.0)      // dPi_scope > dPte
				// if(dParam_te<0.0)      // dPi_scope < dPte

				// if(dParam_ci_Top>0.0)  // dPi_scope < dPci_Top
				// if(dParam_ti_Bot>0.0)  // dPi_scope > dPti_Bot
				// if(dParam_ti_Top>0.0)  // dPi_scope > dPti_Top
				// if(dParam_ci_Bot>0.0)  // dPi_scope < dPci_Bot
				// if(dParam_ce>0.0)      // dPi_scope < dPce
				// if(dParam_te>0.0)      // dPi_scope > dPte

				dPi_min = max( max(dPti_Bot, dPti_Top), dPte );
				dPi_max = min( min(dPci_Top, dPci_Bot), dPce );
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }                           
			}
			else if( dParam_ci_Top<0.0 && dParam_ti_Bot<0.0 )
			{
				// if(dParam_ci_Top<0.0)  // dPi_scope > dPci_Top
				// if(dParam_ti_Bot<0.0)  // dPi_scope < dPti_Bot
				// if(dParam_ti_Top<0.0)  // dPi_scope < dPti_Top
				// if(dParam_ci_Bot<0.0)  // dPi_scope > dPci_Bot
				// if(dParam_ce<0.0)      // dPi_scope > dPce
				// if(dParam_te<0.0)      // dPi_scope < dPte
			
				dPi_min = max( max(dPci_Top, dPci_Bot), dPce );
				dPi_max = min( min(dPti_Top, dPti_Bot), dPte );
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }
			}
			else if( dParam_ci_Top>0.0 && dParam_ti_Bot<0.0 )
			{
				// if dParam_ci_Top>0, ( dParam_ti_Top>0, dParam_ce>0 )
				// if dParam_ti_Bot>0, ( dParam_ci_Bot>0, dParam_te>0 ) 

				// if(dParam_ci_Top>0.0)  // dPi_scope < dPci_Top
				// if(dParam_ti_Bot<0.0)  // dPi_scope < dPti_Bot
				// if(dParam_ti_Top>0.0)  // dPi_scope > dPti_Top
				// if(dParam_ci_Bot<0.0)  // dPi_scope > dPci_Bot
				// if(dParam_ce>0.0)      // dPi_scope < dPce
				// if(dParam_te<0.0)      // dPi_scope < dPte

				dPi_min = max( dPti_Top, dPci_Bot );
				dPi_max = min( min(dPci_Top, dPti_Bot), min(dPce, dPte) );
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }
			}
			else if( dParam_ci_Top<0.0 && dParam_ti_Bot>0.0 )
			{ 
				// if(dParam_ci_Top<0.0)  // dPi_scope > dPci_Top
				// if(dParam_ti_Bot>0.0)  // dPi_scope > dPti_Bot
				// if(dParam_ti_Top<0.0)  // dPi_scope < dPti_Top
				// if(dParam_ci_Bot>0.0)  // dPi_scope < dPci_Bot
				// if(dParam_ce<0.0)      // dPi_scope > dPce
				// if(dParam_te>0.0)      // dPi_scope > dPte
				
				dPi_min = max( max(dPci_Top, dPti_Bot), max(dPce, dPte) );
				dPi_max = min(dPti_Top, dPci_Bot);
				if( dPi_min<dPi_max )      { dPi_req = dPi_min; }
				else if( dPi_min>dPi_max ) { bPi_Check = FALSE; }
				else                       { dPi_req = dPi_min; }        
			}
			else ASSERT(0);
		}
	}

	// 검토
	//double dftopi = dPi/dAg - (dPi*de)*dyt/dIyy + dM_DL*dyt/dIyy;
	//double dfboti = dPi/dAg + (dPi*de)*dyb/dIyy - dM_DL*dyb/dIyy;
	//double dftope = dPe/dAg - (dPe*de)*dyt/dIyy + dMa  *dyt/dIyy;
	//double dfbote = dPe/dAg + (dPe*de)*dyb/dIyy - dMa  *dyb/dIyy;

	double dfpy = 0.0;
	double dfpyt=1.0E+7, dfpyti=0.0;
	double dfpyb=1.0E+7, dfpybi=0.0;

	BOOL bTopExist=FALSE, bBotExist=FALSE;
	for(int i=0; i<arTndnPosi.GetSize(); i++)
	{
		_DGN_TNDN_CRC Tndn =arTndnPosi.GetAt(i);
		
		if(Tndn.dz<0.0)
		{      
			bBotExist=TRUE;
			dfpybi = Tndn.dfpy;
			dfpyb  = min(dfpyb, dfpybi);
		}
		else if(Tndn.dz>0.0)
		{
			bTopExist=TRUE;
			dfpyti = Tndn.dfpy;
			dfpyt  = min(dfpyt, dfpyti);        
		}
	}
		
	// Chagne by Jaeoh ('05.04.23)
	if(iTopBot==0) 
	{ 
		//!/ Top,Bot에 텐던이 다 없다면?.
		if(bTopExist) dfpy = dfpyt;
		else          dfpy = dfpyb; // 압축텐던이 없으면 인장텐던의 값으로. 
	}
	else if(iTopBot==1)
	{
		if(bBotExist) dfpy = dfpyb;
		else          dfpy = dfpyt;
	}
		
	if(!bTopExist && !bBotExist) { dfpy = 0.0; }


	// dAtnStrn : 재하능력극한상태로 예측계산.
	// dAymin   : Magnel공식을 이용한 계산. 
	double dApmin=0.0;
	dApmin = (dfpy==0.0 ? 0.0 : dPi_req/dfpy);

	TndnKsce.de    = -de; // Table에 표현할 때는 Top(+), Bottom(-)
	TndnKsce.dPimin = dPi_req;
	TndnKsce.dApmin = dApmin; // 두 조건단 만족하기 위해. 
	TndnKsce.dApuse = 0.0; //dAyuse;
	
	return bCalcOK;
}

void CDgnKSCE_USD05::Get_Phi_Flexure(double dc, double ddt, double& dPhi)
{
	double depsilon_t = (dc==0.0)? 0.0 : 0.003*(ddt/dc-1.0); 
	if     (depsilon_t<=0.002)   dPhi = 0.70;
	else if(depsilon_t>=0.005)   dPhi = 0.85;
	else                         dPhi = 0.70 + (0.85-0.70)/(0.005-0.002)*(depsilon_t - 0.002);

}

BOOL CDgnKSCE_USD05::Print_BdcyDesign_Table(CArray<T_ELEM_K,T_ELEM_K>& arElemK, CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE>& arBdcrKsce, int i, CMSExcel* pXL)
{
	m_Print_ksce10.Print_BdcyDesign_Table(arElemK, arBdcrKsce, i, pXL); 

	return TRUE;
}


BOOL CDgnKSCE_USD05::Print_CumsDesign_Table(CArray<T_ELEM_K,T_ELEM_K>& arElemK, CArray<PRT_CUMS_KSCE,PRT_CUMS_KSCE>& arCumsKsce, CMSExcel* pXL)
{
	m_Print_ksce10.Print_CumsDesign_Table(arElemK, arCumsKsce, pXL); 
	
	return TRUE;
}


	
void CDgnKSCE_USD05::Set_CMSExcel_KSCE10(CMSExcel* pXL, CString strBaseBook, int nCodePSC)
{
	m_pXL = pXL;
	m_strBaseBook = strBaseBook;
	m_iCivilCode = nCodePSC;
}
