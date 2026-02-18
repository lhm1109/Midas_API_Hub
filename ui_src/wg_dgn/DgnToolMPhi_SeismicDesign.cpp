// DgnToolMPhi_SeismicDesign.cpp: implementation of the CDgnToolMPhi_SeismicDesign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnToolMPhi_SeismicDesign.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "..\wg_db\SectDB.h"
#include "..\wg_db\MatlDB.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\InitCtrl.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\MembCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\TendonInfo.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_db\SectUtil.h"

#include "..\wg_base\wg_base_I_PolyMaker.h"


#include "DgnProgressDlg.h"

CDgnToolMPhi_SeismicDesign::CDgnToolMPhi_SeismicDesign()
{

}

CDgnToolMPhi_SeismicDesign::~CDgnToolMPhi_SeismicDesign()
{

}

//////////////////////////////////////////////////////////////////////////

BOOL CDgnToolMPhi_SeismicDesign::Calc_RLCC(CArray<T_RLCC_K, T_RLCC_K>& arRlccK, _DGN_RESERVE_LATERAL_CAPA_RES& ResData)
{
	T_UNIT_INDEX CurrUnit;
	
	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(PROGRESS_TYPE_RLC_ANALYSIS_CHK);

	//++++++++++++++++++++++++++++++++++++++++++++
	// Set Code Unit, Get Current Unit.
	// See ('04.12.13)사카이,마부치 회의자료. p.14.
	m_CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;	// mm.
	m_CodeUnit.nBase_Force	= D_UNITSYS_FORCE_INDEX_N;		// N.	
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	m_CurrUnit = CurrUnit;
	//++++++++++++++++++++++++++++++++++++++++++++
		
	if(m_CodeUnit.nBase_Force < 0 || m_CodeUnit.nBase_Length < 0)	return PMCV_ERR_RBAR_KIND;
	if(CurrUnit.nBase_Force < 0 || CurrUnit.nBase_Length < 0)	return PMCV_ERR_RBAR_KIND;

	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Get Data by Code Unit.
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);
	//++++++++++++++++++++++++++++++++++++++++++++

	
	ResData.Initialize();
	
	int nRlccSize = arRlccK.GetSize();
	T_RLCO_D RlcoD;
	if(!m_pDoc->m_pAttrCtrl->GetRlco(RlcoD)) m_pDoc->m_pAttrCtrl->GetDefaultRlco(RlcoD);

	// Global Data.
	T_MPGB_D MpgbD;
	MpgbD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMpgb(MpgbD))	return FALSE;
	int nTypeStressStrain = MpgbD.nCode + 1;          // 0:_T("콘크리트표준시방서"), 1:_T("도시V (평성14년)"), 2:_T("도시III (평성14년, σc,σck)"), 3:_T("도시III (평성14년, σc,0.85σck)"), 4:_T("도시V (평성24년)")

	// 계산서 전체정보 기록 //////////////////////////////////////////////////
	ResData.DataUnit = m_CodeUnit;
	ResData.bEarthquakeTypeI = RlcoD.bEarthquakeTypeI;
	ResData.bEarthquakeTypeII = RlcoD.bEarthquakeTypeII;
	ResData.List.SetSize(nRlccSize);
	ResData.strAnalysis = _LS(IDS_DGN_RLC_ANALYSIS);

	if (nTypeStressStrain==PMCV_CODE_MPHI_JP5_24) //신규준(평성 24년) 0:A1 1:A2 2:B1 3:B2 4:C
	{
		if     (RlcoD.nRegionalKind == 0) ResData.strRegionalKind = _T("A1");
		else if(RlcoD.nRegionalKind == 1) ResData.strRegionalKind = _T("A2");
		else if(RlcoD.nRegionalKind == 2) ResData.strRegionalKind = _T("B1");
		else if(RlcoD.nRegionalKind == 3) ResData.strRegionalKind = _T("B2");
		else if(RlcoD.nRegionalKind == 4) ResData.strRegionalKind = _T("C");
		else {m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit); ASSERT(0); return FALSE; };
	}
	else // 이전 기준 0:A 1:B 2:C 
	{
		if     (RlcoD.nRegionalKind == 0) ResData.strRegionalKind = _T("A");
		else if(RlcoD.nRegionalKind == 1) ResData.strRegionalKind = _T("B");
		else if(RlcoD.nRegionalKind == 2) ResData.strRegionalKind = _T("C");
		else {m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit); ASSERT(0); return FALSE; };
	}
	if     (RlcoD.nGroundKind == 0) ResData.strGroundKind = _T("I");
	else if(RlcoD.nGroundKind == 1) ResData.strGroundKind = _T("II");
	else if(RlcoD.nGroundKind == 2) ResData.strGroundKind = _T("III");
	else {m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit); ASSERT(0); return FALSE; };
	ResData.dczKhc0_LimType1 = 0.3;
	ResData.dczKhc0_LimType1 = 0.6;	
	//////////////////////////////////////////////////////////////////////////

	T_RLCC_D RlccD;
	int i;
	for(i=0 ; i<nRlccSize ; i++) ResData.List[i].Initialize();

	MPHI_DATA_ELEM InData;

	BOOL bChk = TRUE;
	for(i=0 ; i<nRlccSize ; i++)
	{		
		if(!m_pDoc->m_pAttrCtrl->GetRlcc(arRlccK[i], RlccD)) continue;				
		
		ProDlg.m_strName.Format(_LS(IDS_DGN_RLC_CHECK_PRODUCING), RlccD.strName);
		if(ProDlg.Update_ProgressDlg(2*i+1,nRlccSize*2))	{ bChk = FALSE;  break;}

		if(RlccD.nType == 0)
		{// RC단주교각
			if(!Calc_RCSinglePier(RlcoD, RlccD, ResData.List[i], InData)) ResData.List[i].bIsRes = FALSE;
		}
		else 
		{
			ASSERT(0);
			ResData.List[i].bIsRes = FALSE;
		}
		if(ProDlg.Update_ProgressDlg(2*i+2,nRlccSize*2))	{ bChk = FALSE;  break;}
		if(!ResData.List[i].bIsRes)
		{
			CString strMsg;  strMsg.Format(_LS(IDS_DGN_RLC_CALC_MEMB_FAIL_Err), ResData.List[i].strName);
			GSaveHistoryFormat(strMsg);  
		}
	}
	ResData.nDgnLevel = InData.UserD.nEst23Type; /*m_nEst23Type*/
	if     (ResData.nDgnLevel == 0) { ResData.strBridgeType = _T("A");  ResData.strDgnLevel.Format(_T("%s3"), _LS(IDS_DGN_RLC_DGNLEVEL)); }
	else if(ResData.nDgnLevel == 1) { ResData.strBridgeType = _T("B");  ResData.strDgnLevel.Format(_T("%s2"), _LS(IDS_DGN_RLC_DGNLEVEL)); }
	else {m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit); ASSERT(0); return FALSE; };	
	
	//++++++++++++++++++++++++++++++++++++++++++++
	// <Remember> Set Data by User Unit.
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);
	//++++++++++++++++++++++++++++++++++++++++++++

	ProDlg.Delete_ProgressDlg();
	return bChk;
}
BOOL CDgnToolMPhi_SeismicDesign::Calc_RCSinglePier(T_RLCO_D& RlcoD, T_RLCC_D& RlccD, _DGN_RESERVE_LATERAL_CAPA_MEMB_RES& MembResD, MPHI_DATA_ELEM& InData)
{
	// 계산서 부재정보 초기화 ////////////////////////////////////////////////
	MembResD.Initialize();
	MembResD.strName = RlccD.strName;
	//////////////////////////////////////////////////////////////////////////

	CArray<_DGN_MPHI_STRN_D, _DGN_MPHI_STRN_D&> arMPhiStrn;
	CArray<_DGN_MPHI_MOMENT_D, _DGN_MPHI_MOMENT_D> arM0y, arM0z;
	CArray<_DGN_MPHI_MOMENT_D, _DGN_MPHI_MOMENT_D> arM_curr;
	CArray<T_ELEM_K, T_ELEM_K> arElemKey;
	_DGN_MPHI_STRN_D MPhiStrn;

	//Get Input Data
	InData.Initialize();

	double dpMom[4][4], dpPhi[4][4];
	double dM0y_I, dM0y_J, dM0z_I, dM0z_J;
	double dM0y, dP0y, dPhi0y, dDelta0y;
	double dM0z, dP0z, dPhi0z, dDelta0z;
	double dActH, dMembHeight, dWidth, dHeight;
	double dDeltaRa;
	double dLpy, dLpz;
	int    nLpTypey = 0;
	int    nLpTypez = 0;
	double dRhos[2];   //획구속근의 체적비 (0:교축방향, 1:교축직각방향) 
	double dEdes[2];   //하강구배 (0:교축방향, 1:교축직각방향) 
	double decc[2];    //최대압축응력시의 변형률 (0:교축방향, 1:교축직각방향) 
	double decu[2];    //최종 변형률 (0:교축방향, 1:교축직각방향) 
	double dSigcc[2];  //최대압축강도 (0:교축방향, 1:교축직각방향) 	
	// 0:Major+, 1:Major-, 2:Minor+, 3:Minor-	
	int nCriticalSign_y = 0; 	
	int nCriticalSign_z = 2; 
	double dMc[4],  dPc[4], dPhic[4], dDeltac[4];
	double dMy0[4], dPy0[4], dPhiy0[4], dDeltay0[4];
	double dMy[4],  dPy[4], dPhiy[4], dDeltay[4];
	double dMu[4],  dPu[4], dPhiu[4], dDeltau[4];
	double dPs[4], dPs0[4], dSc[4], dSs[4], dcc[4], dce[4], dcpt[4], dtc[4], dbw[4], dDeff[4];
	int    nFailureType[4];
	double dKhc[4], dKhc0[4], dmua[4], dPa[4], dcs[4], dcz[4], dcp[4], dW[4], dalpha[4];
	double ddeltaR[4], dmur[4], dcR[4], dr[4];
	BOOL   bCapaChk[4], bDisChk[4];

	double dSf[4]; //***// PMS.5568 Civil 일본 내진설계 대응; 섬유시트에 의한 전단보강

	double dEc = 0.0;
	double dFc = 0.0;
	double dEsr = 0.0;
	double dFyr = 0.0;
	double dFys = 0.0;
	double dArea = 0.0;
	double da_tie = 0.0;
	double dDensity = 0.0;
	double dAngle_tie[2] = {0.0, };
	double dAw_tie[2] = {0.0, };
	
	_DGN_RESERVE_LATERAL_CAPA_MEMB_UNIT_RES MembUnitResD;
	double dtrrad = CMathFunc::m_trrad;
	int i=0, j=0, k=0, nTypeEq=0, nElemSize=0, iTopPosi=0, iBottomPosi=0, nErrID=0;
	T_ELEM_K ElemK;
	BOOL bChk;
	BOOL bSignChk = FALSE; // +,- 모멘트 부호에 따라 Check 여부

	int nSectCon = 0;

	// 초기화
	for(i=0 ; i<4 ; i++)
	{
		dMc[i] = dPc[i] = dPhic[i] = dDeltac[i] = 0.0;		
		dMy0[i] = dPy0[i] = dPhiy0[i] = dDeltay0[i] = 0.0;
		dMy[i] =  dPy[i] = dPhiy[i] = dDeltay[i] = 0.0;
		dMu[i] =  dPu[i] = dPhiu[i] = dDeltau[i] = 0.0;
		dPs[i] = dPs0[i] = dSc[i] = dSs[i] = dcc[i] = dce[i] = dcpt[i] = dtc[i] = dbw[i] = dDeff[i] = 0.0;
		nFailureType[i] = 0;
		dKhc[i] = dKhc0[i] = dmua[i] = dPa[i] = dcs[i] = dcz[i] = dcp[i] = dW[i] = dalpha[i] = 0.0;
		ddeltaR[i] = dmur[i] = dcR[i] = dr[i] = 0.0;
		bCapaChk[i] = bDisChk[i] = FALSE;

		dSf[i] = 0.0; //***// PMS.5568 Civil 일본 내진설계 대응
	}
	
	dActH = RlccD.dActHeight;
	if(dActH <= 0.0) return FALSE;
	dDeltaRa = dActH/100.0;
	
	arElemKey.Copy(RlccD.aElemList);
	nElemSize = arElemKey.GetSize();
	SortElemList(arElemKey, iTopPosi, iBottomPosi, dMembHeight);

	if(nElemSize==0) return FALSE;

	int nEQType = 0;
	for(nTypeEq=0 ; nTypeEq<2 ; nTypeEq++)
	{
		if(nTypeEq==0)
		{
			if(!RlcoD.bEarthquakeTypeI) continue;
			InData.UserD.nEQType = PMCV_EQ_TYPE::TYPE1;
			nEQType = PMCV_EQ_TYPE::TYPE1;
		}
		else 
		{
			if(!RlcoD.bEarthquakeTypeII) continue;
			InData.UserD.nEQType = PMCV_EQ_TYPE::TYPE2;
			nEQType = PMCV_EQ_TYPE::TYPE2;
		}

		arMPhiStrn.SetSize(nElemSize);
		arM0y.SetSize(nElemSize);			
		arM0z.SetSize(nElemSize);
		arM_curr.SetSize(nElemSize);
		bChk = TRUE;
		for(i=0 ; i<nElemSize ; i++)
		{
			ElemK = arElemKey.GetAt(i);

			MPhiStrn.Initialize();				
			MPhiStrn.nElem = ElemK;
			
			//m_iPosi = 0;	// I.
			nErrID = Cal_MphiStrn(ElemK, POS_I, TRUE, TRUE, InData, MPhiStrn.dpMom_I, MPhiStrn.dpPhi_I); 
			if(nErrID != 0) { PrintErrorMessage(nErrID, ElemK);  bChk = FALSE; break; }
			dM0y_I = InData.ForcD.dMuy;
			dM0z_I = InData.ForcD.dMuz;
			//dM0y_I = m_dMuy;  dM0z_I = m_dMuz;
			
			//m_iPosi = 2;	// J.
			nErrID = Cal_MphiStrn(ElemK, POS_J, TRUE, TRUE, InData, MPhiStrn.dpMom_J, MPhiStrn.dpPhi_J);
			if(nErrID != 0) { PrintErrorMessage(nErrID, ElemK);  bChk = FALSE; break; }
			dM0y_J = InData.ForcD.dMuy;
			dM0z_J = InData.ForcD.dMuz;
			//dM0y_J = m_dMuy;  dM0z_J = m_dMuz;

			nSectCon = InData.SectD.nSectCon;
			if(nSectCon == PMCV_SECT_HTRK || nSectCon == PMCV_SECT_GEN) bSignChk = TRUE;

			for(j=0 ; j<4 ; j++)
			{
				for(k=0 ; k<4 ; k++)
				{ 
					MPhiStrn.dpMom_M[j][k] = (MPhiStrn.dpMom_I[j][k] + MPhiStrn.dpMom_J[j][k])/2.0;
					MPhiStrn.dpPhi_M[j][k] = (MPhiStrn.dpPhi_I[j][k] + MPhiStrn.dpPhi_J[j][k])/2.0;
				}
			}
			arMPhiStrn.SetAt(i, MPhiStrn);
			arM0y[i].dM[0] = dM0y_I;
			arM0y[i].dM[1] = (dM0y_I+dM0y_J)/2.0;
			arM0y[i].dM[2] = dM0y_J;
			arM0z[i].dM[0] = dM0z_I;
			arM0z[i].dM[1] = (dM0z_I+dM0z_J)/2.0;
			arM0z[i].dM[2] = dM0z_J;
			if(i==0)
			{
				dM0y = iBottomPosi==0 ? dM0y_I : dM0y_J;
				dM0z = iBottomPosi==0 ? dM0z_I : dM0z_J;
			}
		}			
		if(!bChk) return FALSE;
		double dArtU[4] = {0.0, };
		double dDeffU[4] = {0.0, };
		// Bottom 부분의 M-Phi 계산
		ElemK = arElemKey.GetAt(0);
		// Set Position
		InData.nPosition = iBottomPosi;
		//m_iPosi = iBottomPosi;	
		nErrID = Cal_MphiStrn(ElemK, iBottomPosi, TRUE, TRUE, InData, dpMom, dpPhi, dArtU, dDeffU, TRUE);
		if(nErrID != 0) { PrintErrorMessage(nErrID, ElemK);  return FALSE; }
		// input data
		const int nStressStrainType = InData.UserD.nStressStrainType;
		const auto& SectBaseD = InData.SectD.SectBaseD;
		const int nEst23Type = InData.UserD.nEst23Type;
		nSectCon = InData.SectD.nSectCon;
		dEc = InData.MatlD.dEc;
		dFc = InData.MatlD.dFc;
		dEsr = InData.MatlD.dEsr;
		dFyr = InData.MatlD.dFyr;
		dFys = InData.MatlD.dFys;
		dDensity = InData.MatlD.dDensity;
		dArea  = InData.SectD.dArea;
		da_tie = InData.RbarD.da_tie;
		dAngle_tie[0] = InData.UserD.dAngle_tie[0];
		dAngle_tie[1] = InData.UserD.dAngle_tie[1];
		dAw_tie[0] = InData.RbarD.dAw_tie[0];
		dAw_tie[1] = InData.RbarD.dAw_tie[1];
		dWidth  = SectBaseD[0].Stiffness.Cym + SectBaseD[0].Stiffness.Cyp;
		dHeight = SectBaseD[0].Stiffness.Czm + SectBaseD[0].Stiffness.Czp;
		// 전단철근이 없는 경우
		if((RlccD.bTraverse && dAw_tie[0] <= 0.0) || (RlccD.bLongitude && dAw_tie[1] <= 0.0))	
		{ PrintErrorMessage(RLCC_ERR_SHEAR_RBAR, ElemK);  return FALSE; }	//전단철근이 없는 경우

		if (nStressStrainType==PMCV_CODE_MPHI_JP5_24)
		{
			dLpy = Calc_Lp_st(1, InData); //major
			dLpz = Calc_Lp_st(3, InData); //minor
		}
		else
		{
			dLpy = 0.2*dActH - 0.1*dHeight;
			if     (dLpy < 0.1*dHeight) { dLpy = 0.1*dHeight;  nLpTypey = 1; }
			else if(dLpy > 0.5*dHeight) { dLpy = 0.5*dHeight;  nLpTypey = 2; }
			else                        { nLpTypey = 0; }
			dLpz = 0.2*dActH - 0.1*dWidth;
			if     (dLpz < 0.1*dWidth) { dLpz = 0.1*dWidth;  nLpTypez = 1; }
			else if(dLpz > 0.5*dWidth) { dLpz = 0.5*dWidth;  nLpTypez = 2; }
			else                        { nLpTypez = 0; }
		}
		
		int nAxisDirMajor = 1; // 1or2 Major
		int nAxisDirMinor = 3; // 3or4 Minor

		dRhos[0] = Calc_Rhos(nAxisDirMajor, nStressStrainType, nSectCon, InData.RbarD, TRUE);
		dRhos[1] = Calc_Rhos(nAxisDirMinor, nStressStrainType, nSectCon, InData.RbarD, TRUE);
		BOOL bExistSect = TRUE; // only consider existing section
		dEdes[0] = Calc_Edes(nAxisDirMajor, InData, bExistSect);
		dEdes[1] = Calc_Edes(nAxisDirMinor, InData, bExistSect);
		decc[0]  = Calc_ecc(nAxisDirMajor, InData, bExistSect);
		decc[1]  = Calc_ecc(nAxisDirMinor, InData, bExistSect);
		decu[0]  = Calc_ecu(nAxisDirMajor, InData, bExistSect);
		decu[1]  = Calc_ecu(nAxisDirMinor, InData, bExistSect);
		dSigcc[0]= Calc_Sigcc(nAxisDirMajor, InData, bExistSect);
		dSigcc[1]= Calc_Sigcc(nAxisDirMinor, InData, bExistSect);

		// 사하중시의 수평내력 및 수평변위 계산
		dP0y = dM0y/dActH;
		dPhi0y = Calc_Phi(TRUE, iBottomPosi, dM0y, arMPhiStrn[0]);		
		dP0z = dM0z/dActH;		
		dPhi0z = Calc_Phi(FALSE, iBottomPosi, dM0z, arMPhiStrn[0]);

		if(RlcoD.nInitialForce == 2) //Check!!! 사용자 정의시 전체 동일 모멘트 적용시에는 해당 항목 제거 할것
		{
			if(!CalcPierMoment(dP0y, dActH, arElemKey, arM0y)) return FALSE;
			if(!CalcPierMoment(dP0z, dActH, arElemKey, arM0z)) return FALSE;
		}		
		
		if(!CalcPhiDis(TRUE, arElemKey, arM0y, arMPhiStrn, dDelta0y)) return FALSE;		
		if(!CalcPhiDis(FALSE, arElemKey, arM0z, arMPhiStrn, dDelta0z)) return FALSE;
		if(!bSignChk)
		{
			dM0y = fabs(dM0y);
			dP0y = fabs(dP0y);
			dPhi0y = fabs(dPhi0y);
			dDelta0y = fabs(dDelta0y);
			dM0z = fabs(dM0z);
			dP0z = fabs(dP0z);
			dPhi0z = fabs(dPhi0z);
			dDelta0z = fabs(dDelta0z);
		}

		BOOL bMajorAxis;
		bChk = TRUE;
		for(i=0; i<AXIS_DIR_NUM ; i++) // j = 0:Major+, 1:Major-, 2:Minor+, 3:Minor-
		{
			bMajorAxis = (i==0 || i==1);
			if(!bSignChk && (i==1 || i==3)) continue;
			if(!RlccD.bTraverse  && bMajorAxis) continue;
			if(!RlccD.bLongitude && !bMajorAxis) continue;
			//균열시의 수평내력과 수평변위
			dMc[i] = dpMom[0][i];
			dPc[i] = dMc[i]/dActH;
			dPhic[i] = dpPhi[0][i];
			if(!CalcPierMoment(dPc[i], dActH, arElemKey, arM_curr)) { bChk=FALSE;  break; }
			if(!CalcPhiDis(bMajorAxis, arElemKey, arM_curr, arMPhiStrn,dDeltac[i])) { bChk=FALSE;  break; }
			//초기항복시의 수평내력과 수평변위
			dMy0[i] = dpMom[1][i];
			dPy0[i] = dMy0[i]/dActH;
			dPhiy0[i] = dpPhi[1][i];
			if(!CalcPierMoment(dPy0[i], dActH, arElemKey, arM_curr)) { bChk=FALSE;  break; }
			if(!CalcPhiDis(bMajorAxis, arElemKey, arM_curr, arMPhiStrn, dDeltay0[i])) { bChk=FALSE;  break; }
			//항복시의 수평내력과 수평변위 (내진성능2)
			dMy[i] = dpMom[2][i];
			dPy[i] = dMy[i]/dActH;
			dPhiy[i] = dpPhi[2][i];
			if(!CalcPierMoment(dPy[i], dActH, arElemKey, arM_curr)) { bChk=FALSE;  break; }
			if(!CalcPhiDis(bMajorAxis, arElemKey, arM_curr, arMPhiStrn, dDeltay[i])) { bChk=FALSE;  break; }
			//종국시의 수평내력과 수평변위 (내진성능3)
			dMu[i] = dpMom[3][i];
			dPu[i] = dMu[i]/dActH;
			dPhiu[i] = dpPhi[3][i];
			//if(!CalcPierMoment(dPu[i], dActH, arElemKey, arM_curr)) { bChk=FALSE;  break; }
			//if(!CalcPhiDis(bMajorAxis, arElemKey, arM_curr, arMPhiStrn, dDeltau[i])) { bChk=FALSE;  break; }
			//5299 한계상태 계산 방법 변경(내진성능2,3)
			if (nStressStrainType==PMCV_CODE_MPHI_JP5_24)
			{
				if (nEst23Type==1)//내진성능2
				{
					//5299 내진성능2 Delta y 값 계산
					dDeltay[i] = (fabs(dpMom[1][i]) > PMCV_dZERO) ? dDeltay0[i] * (dpMom[2][i]/dpMom[1][i]) : dDeltay0[i];
					if(bMajorAxis) dDeltau[i] = dDeltay[i] + (dPhiy[i]-dPhiy0[i])*dLpy*(dActH-dLpy/2.0);
					else           dDeltau[i] = dDeltay[i] + (dPhiy[i]-dPhiy0[i])*dLpz*(dActH-dLpz/2.0);
				}
				else //내진성능3
				{
					//5299 내진성능3 Delta y 값 계산
					dDeltay[i] = (fabs(dpMom[1][i]) > PMCV_dZERO) ? dDeltay0[i] * (dpMom[3][i]/dpMom[1][i]) : dDeltay0[i];
					if(bMajorAxis) dDeltau[i] = dDeltay[i] + (dPhiu[i]-dPhiy0[i])*dLpy*(dActH-dLpy/2.0);
					else           dDeltau[i] = dDeltay[i] + (dPhiu[i]-dPhiy0[i])*dLpz*(dActH-dLpz/2.0);
				}
			}
			else
			{
				if(bMajorAxis) dDeltau[i] = dDeltay[i] + (dPhiu[i]-dPhiy[i])*dLpy*(dActH-dLpy/2.0);
				else           dDeltau[i] = dDeltay[i] + (dPhiu[i]-dPhiy[i])*dLpz*(dActH-dLpz/2.0);

			}
			
			
			// 전단내력산출
			dbw[i]   = Get_dBv(bMajorAxis, InData.SectD);
			double dD_Height = (bMajorAxis ? dHeight : dWidth);
			dDeff[i] = Get_dDeff(bMajorAxis, dD_Height, dbw[i], InData);
			
			double dPt = ((dbw[i]*dDeff[i])<PMCV_dZERO) ? 100.*dArtU[i]/dD_Height :100.* dArtU[i]/(dbw[i]*dDeff[i]);
			double dfc = dFc;
			//***// PMS.5568 Civil 일본 내진설계 대응
			// 기설 부와 보강부 콘크리트의 강도가 다른 경우, 면적비에 따라 전단응력을 보정
			if(Reinforced_RCCover())
			{
				double dfcrcc = 0.0;
				double dArcc  = 0.0;
				dfc = (dFc*dArea + dfcrcc*dArcc) / (dArea+dArcc);
			}
			dtc[i]   = Calc_ta(nStressStrainType, dfc);
			dcpt[i]  = Calc_cpt(dPt);
			dce[i]   = Calc_ce(dDeff[i]);
			dcc[i]   = Calc_cc(nEQType);			
			dSc[i]   = dcc[i]*dce[i]*dcpt[i]*dtc[i]*dbw[i]*dDeff[i];
			double dAw_tie_temp = (bMajorAxis) ? dAw_tie[0] : dAw_tie[1];
			double dAngle_tie_temp = (bMajorAxis) ? dAngle_tie[0] : dAngle_tie[1];
			//***// PMS.5568 Civil 일본 내진설계 대응
			// Aw×σsy=(기설 전단철근 단면적Ah1×기설 띠철근 항복강도σsy1)+{보강 전단철근 단면적 Ah2×(기설 띠철근 간격/보강 띠철근 간격)}×보강 띠철근 항복강도 σsy2
			double dAw2 = 0.0, dSigsy2 = 0.0;
			if(Reinforced_RCCover())
			{
				dAw2 = 0.0; //보강 전단철근 단면적
				dSigsy2 = 0.0;
			}
			//5299 전단내력 계산 식 변경 
			if ((dDeff[i]/1.15) > dMembHeight) //d/1.15>h(교각 높이) 인 경우, d/1.15 대신교각 높이 h를 사용
			{
				dSs[i] = (da_tie==0.0) ? 0.0 : (dAw_tie_temp*dFys + dAw2*dSigsy2)*dMembHeight*(sin(dAngle_tie_temp*dtrrad)+cos(dAngle_tie_temp*dtrrad))/da_tie;
			}
			else
			{
				dSs[i] = (da_tie==0.0) ? 0.0 : (dAw_tie_temp*dFys + dAw2*dSigsy2)*dDeff[i]*(sin(dAngle_tie_temp*dtrrad)+cos(dAngle_tie_temp*dtrrad))/1.15/da_tie;
			}

			//***// PMS.5568 Civil 일본 내진설계 대응
			if(Reinforced_FRP())
			{ 
				double dsigf = Cal_Sigf(); // σF : 섬유 시트의 설계용 인장강도(N/mm2)
				double dtf   = 0.0; // 섬유 시트 1장당 두께(mm)
				int    nnf   = 0;   // 섬유 시트 매수
				double daf   = 0.0; // 섬유 시트의 폭(mm)

				double dtheta= dPI / 2.0; // Θ：전단철근 및 섬유 시트가 부재 연직 축과 이루는 각도(°), 90도로 고정합니다.
				double da    = da_tie; // 띠철근의 간격
				
				double dTf   = dtf * static_cast<double>(nnf); // 섬유시트 두께

				dSf[i] = dTf * dsigf * dDeff[i] * (sin(dtheta) + cos(dtheta)) / 1.15;
			}
			
			//***// PMS.5568 Civil 일본 내진설계 대응
			double dCdc = Calc_Cdc(bMajorAxis, dMembHeight, dDeff[i]);// 전단 스팬에 의해 콘크리트가 부담하는 전단내력의 할증계수
			double dCds = Calc_Cds(bMajorAxis, dMembHeight, dDeff[i]);// 전단 스팬에 의해 띠철근이 부담하는 전단내력의 저감계수

			dPs[i]   = dCdc*dSc[i] + dCds*(dSs[i] + dSf[i]);
			dPs0[i]  = dcc[i]==0.0 ?  dSs[i] : dSc[i]/dcc[i] + dSs[i];
			
			// 파괴형태의 판정
			if(dPu[i] <= dPs[i])
			{// 휨파괴형
				nFailureType[i] = 0;				
			}
			else 
			{
				if(dPu[i] <= dPs0[i])
				{//휨파괴형에서 전단파괴 이행형					
					nFailureType[i] = 1;
				}
				else 
				{//전단파괴형				
					nFailureType[i] = 2;
				}
			}

			// 지진시보유수평내력 
			if(nEQType == 1)
			{//TypeI
				dalpha[i] = nEst23Type == 1 ? 2.4 : 3.0; // DgnLevel == 0 내진성능3  DgnLevel == 1 내진성능2
			}
			else if(nEQType == 2)
			{//TypeI
				dalpha[i] = nEst23Type == 1 ? 1.2 : 1.5; // DgnLevel == 0 내진성능3  DgnLevel == 1 내진성능2
			}
			else ASSERT(0);
			//5299 허용 소성율 계산방법 변경!!
			if (nStressStrainType==PMCV_CODE_MPHI_JP5_24)
			{
				dalpha[i] = 1.2; // 1.2 고정
				if(nFailureType[i] == 0) dmua[i] = (dalpha[i]*dDeltay[i] == 0.0) ? 0.0 : (dDeltau[i])/dalpha[i]/dDeltay[i];
				else                     dmua[i] = 1.0;
			}
			else
			{
				if(nFailureType[i] == 0) dmua[i] = (dalpha[i]*dDeltay[i] == 0.0) ? 1.0 : 1.0 + (dDeltau[i]-dDeltay[i])/dalpha[i]/dDeltay[i];
				else                     dmua[i] = 1.0;
			}
			dcs[i]  = (2.0*dmua[i]-1.0 < PMCV_dZERO) ? 1.0 : 1.0 / (sqrt(2.0*dmua[i]-1.0));
			dcz[i]  = Calc_cz(nStressStrainType, RlcoD.nRegionalKind, nEQType);
			dcp[i]  = (nFailureType[i] == 2) ? 1.0 : 0.5; // 표6.4.3
			if(bMajorAxis) dKhc0[i] = Calc_Khc0(nStressStrainType, nEQType, RlcoD.nGroundKind, RlccD.dNaturalFrequency[0]);
			else           dKhc0[i] = Calc_Khc0(nStressStrainType, nEQType, RlcoD.nGroundKind, RlccD.dNaturalFrequency[1]);
			
			if     (nEQType == 1) dKhc[i] = dcs[i] * max(0.3, dcz[i] * dKhc0[i]); // 도로교 시방서 p89-93
			else if(nEQType == 2) dKhc[i] = dcs[i] * max(0.6, dcz[i] * dKhc0[i]); 
			else ASSERT(0);
			dKhc[i] = max(0.4*dcz[i], dKhc[i]);
			dW[i] = RlccD.dWu + dcp[i]*RlccD.dWp;

			if     (nFailureType[i] == 0) dPa[i] = dPu[i];
			else if(nFailureType[i] == 1) dPa[i] = dPu[i];
			else if(nFailureType[i] == 2) dPa[i] = dPs0[i];

			bCapaChk[i] = dPa[i] >= dKhc[i]*dW[i];


			if(nEst23Type == 1)
			{// 내진성능2 일때에만 검토
				dmur[i] = 1.0/2.0*(pow(dcz[i]*dKhc0[i]*dW[i]/dPa[i], 2)+1.0);
				dcR[i] = 0.6; // 철근콘크리트교량에서는 0.6으로 한다
				dr[i]  = 0.0; // 철근콘크리트교량에서는 0.0으로 한다
				if(bMajorAxis)
				{				
					if(i==0) ddeltaR[i] = dcR[i] * (dmur[i]-1.0) * (1.0-dr[i]) * (dDeltay[i] - max(0.0, dDelta0y)); 
					else     ddeltaR[i] = dcR[i] * (dmur[i]-1.0) * (1.0-dr[i]) * (dDeltay[i] - max(0.0, -dDelta0y)); 
				}
				else 
				{				
					if(i==2) ddeltaR[i] = dcR[i] * (dmur[i]-1.0) * (1.0-dr[i]) * (dDeltay[i] - max(0.0, dDelta0z)); 
					else     ddeltaR[i] = dcR[i] * (dmur[i]-1.0) * (1.0-dr[i]) * (dDeltay[i] - max(0.0, -dDelta0z)); 
				}
				
				ddeltaR[i] = fabs(ddeltaR[i]);

				bDisChk[i] = (ddeltaR[i] <= dDeltaRa);
			}		
		}	
		if(!bChk) return FALSE;		

		if(bSignChk)
		{
			// Major
			if(bCapaChk[0] == bCapaChk[1] && bDisChk[0] == bDisChk[1])
			{
				if(dKhc[0]*dW[0] == 0.0 || dKhc[1]*dW[1] == 0.0) 
				{
					if(dKhc[0]*dW[0] == 0.0 && dKhc[1]*dW[1] == 0.0) nCriticalSign_y = (dPa[0] <= dPa[1]) ? 0 : 1;
					else if(dKhc[0]*dW[0] == 0.0) nCriticalSign_y = 1;
					else                          nCriticalSign_y = 0;
				}
				else 
					nCriticalSign_y = (dPa[0]/(dKhc[0]*dW[0]) <= dPa[1]/(dKhc[1]*dW[1])) ? 0 : 1;
			}
			else 
			{
				if(bCapaChk[0] != bCapaChk[1])
				{ nCriticalSign_y = (!bCapaChk[0]) ? 0 : 1; }
				else 
				{ nCriticalSign_y = (!bDisChk[0]) ? 0 : 1; }
			}

			// Minor
			if(bCapaChk[2] == bCapaChk[3] && bDisChk[2] == bDisChk[3])
			{
				
				if(dKhc[2]*dW[2] == 0.0 || dKhc[3]*dW[3] == 0.0) 
				{
					if(dKhc[2]*dW[2] == 0.0 && dKhc[3]*dW[3] == 0.0) nCriticalSign_z = (dPa[2] <= dPa[3]) ? 2 : 3;
					else if(dKhc[2]*dW[2] == 0.0) nCriticalSign_z = 3;
					else                          nCriticalSign_z = 2;
				}
				else 
					nCriticalSign_z = (dPa[2]/(dKhc[2]*dW[2]) <= dPa[3]/(dKhc[3]*dW[3])) ? 2 : 3;
			}
			else 
			{
				if(bCapaChk[2] != bCapaChk[3])
				{ nCriticalSign_z = (!bCapaChk[2]) ? 2 : 3; }
				else 
				{ nCriticalSign_z = (!bDisChk[2]) ? 2 : 3; }
			}
		}

		
		// 계산서 부재의 개별계산정보 기록 //////////////////////////////////////////////////
		int nCriticalSign;
		for(i=0 ; i<2 ;i++)
		{
			nCriticalSign = (i==0) ? nCriticalSign_y : nCriticalSign_z;
			MembUnitResD.Initialize();

			MembUnitResD.nFailureType = nFailureType[nCriticalSign];
			MembUnitResD.strFailureType = Get_FailureName(MembUnitResD.nFailureType);
			MembUnitResD.dKhc = dKhc[nCriticalSign];
			MembUnitResD.dmua = dmua[nCriticalSign];
			MembUnitResD.dPa  = dPa[nCriticalSign];
			MembUnitResD.bCapaChk = bCapaChk[nCriticalSign];

			MembUnitResD.ddeltaR = ddeltaR[nCriticalSign];
			MembUnitResD.dmur    = dmur[nCriticalSign];
			MembUnitResD.bDisChk = bDisChk[nCriticalSign];

			MembUnitResD.dMc      = dMc[nCriticalSign];
			MembUnitResD.dPhic    = dPhic[nCriticalSign];
			MembUnitResD.dPc      = dPc[nCriticalSign];
			MembUnitResD.ddeltac  = dDeltac[nCriticalSign];
			MembUnitResD.dMy0     = dMy0[nCriticalSign];
			MembUnitResD.dPhiy0   = dPhiy0[nCriticalSign];
			MembUnitResD.dPy0     = dPy0[nCriticalSign];
			MembUnitResD.ddeltay0 = dDeltay0[nCriticalSign];
			MembUnitResD.dMy      = dMy[nCriticalSign];
			MembUnitResD.dPhiy    = dPhiy[nCriticalSign];
			MembUnitResD.dPy      = dPy[nCriticalSign];
			MembUnitResD.ddeltay  = dDeltay[nCriticalSign];
			MembUnitResD.dMu      = dMu[nCriticalSign];
			MembUnitResD.dPhiu    = dPhiu[nCriticalSign];
			MembUnitResD.dPu      = dPu[nCriticalSign];
			MembUnitResD.ddeltau  = dDeltau[nCriticalSign];
			
			MembUnitResD.dPs      = dPs[nCriticalSign];
			MembUnitResD.dPs0     = dPs0[nCriticalSign];
			MembUnitResD.dSc      = dSc[nCriticalSign];
			MembUnitResD.dSs      = dSs[nCriticalSign];
			MembUnitResD.dcc      = dcc[nCriticalSign];
			MembUnitResD.dce      = dce[nCriticalSign];
			MembUnitResD.dcpt     = dcpt[nCriticalSign];
			MembUnitResD.dtc      = dtc[nCriticalSign];
			MembUnitResD.dbw      = dbw[nCriticalSign];
			MembUnitResD.dDeff    = dDeff[nCriticalSign];
			MembUnitResD.dKhc0    = dKhc0[nCriticalSign];
			MembUnitResD.dcs      = dcs[nCriticalSign];
			MembUnitResD.dcz      = dcz[nCriticalSign];
			MembUnitResD.dW       = dW[nCriticalSign];
			MembUnitResD.dcp      = dcp[nCriticalSign];
			MembUnitResD.dalpha   = dalpha[nCriticalSign];
			MembUnitResD.dcR      = dcR[nCriticalSign];
			MembUnitResD.dr       = dr[nCriticalSign];
			//MembUnitResD.strFilePath_LoadCuvaFig = ???; Check!!! 작업요망 : 삽도
			if(nEQType == 1)
			{
				if(i==0) MembResD.ResType1Trav = MembUnitResD;
				else     MembResD.ResType1Long = MembUnitResD;
			}
			else 
			{
				if(i==0) MembResD.ResType2Trav = MembUnitResD;
				else     MembResD.ResType2Long = MembUnitResD;
			}
		}	
		//////////////////////////////////////////////////////////////////////////		
	}

	// 계산서 부재정보 기록 //////////////////////////////////////////////////
	
	MembResD.bIsRes = TRUE;
	MembResD.strName = RlccD.strName;
	MembResD.bTraverse = RlccD.bTraverse;
	MembResD.bLongitude = RlccD.bLongitude;
	MembResD.dLength = dMembHeight;
	MembResD.nDivSize = nElemSize;
	MembResD.strShape = Get_SectName(nSectCon);
	MembResD.dWidth = dWidth;
	MembResD.dHeight = dHeight;

	MembResD.strType = Get_MembTypeName(RlccD.nType);
	MembResD.dWu = RlccD.dWu;
	MembResD.dWp = RlccD.dWp;
	MembResD.dWp_Beam = max(0, RlccD.dWp - Calc_SelfWeight(arElemKey));	
	MembResD.dM0[0] = RlccD.dInitMoment[0];
	MembResD.dM0[1] = RlccD.dInitMoment[1];
	MembResD.dActHeight = dActH;
	MembResD.dRD = RlccD.dInitAxisForce;
	MembResD.ddeltaRa = dDeltaRa;
	MembResD.dT[0] = RlccD.dNaturalFrequency[0];
	MembResD.dT[1] = RlccD.dNaturalFrequency[1];

	MembResD.dWeightDensity = dDensity;
	MembResD.dEc_conc   = dEc;
	MembResD.dfck_conc  = dFc;
	MembResD.dEs_rebar  = dEsr;
	MembResD.dfsy_rebar = dFyr;
	MembResD.da_tie     = da_tie;
	MembResD.dAngle_tie = dAngle_tie[0];
	MembResD.dAw_tie[0] = dAw_tie[0];
	MembResD.dAw_tie[1] = dAw_tie[1];
	MembResD.dfsy_tie   = dFys;

	for(i=0 ; i<2 ; i++)
	{
		MembResD.dRhos[i]   = dRhos[i];
		MembResD.dEdes[i]   = dEdes[i];
		MembResD.decc[i]    = decc[i];
		MembResD.decu[i]    = decu[i];
		MembResD.dSigcc[i]  = dSigcc[i];
		MembResD.dPhi0[i]   = (i==0) ? dPhi0y   : dPhi0z  ;
		MembResD.dP0[i]     = (i==0) ? dP0y     : dP0z    ;
		MembResD.ddelta0[i] = (i==0) ? dDelta0y : dDelta0z;
		MembResD.dLp[i]     = (i==0) ? dLpy     : dLpz    ;
		MembResD.nLpType[i] = (i==0) ? nLpTypey : nLpTypez;
	}

	MembResD.ElemK_Bottom = arElemKey[0];
	MembResD.iPosi_Bottom = iBottomPosi;

	MembResD.KeyList.Copy(RlccD.aElemList);

	//MembResD.strFilePath_ShapeFig = ???; Check!!! 작업요망 : 삽도
	

	//////////////////////////////////////////////////////////////////////////

	return TRUE;	
}

double CDgnToolMPhi_SeismicDesign::Calc_SelfWeight(CArray<T_ELEM_K, T_ELEM_K>& arElemK)
{
	double dWeight = 0.0;
	T_ELEM_D ElemD;
	T_SECT_D SectD;
	T_MATD_D MatdD;
	T_NODE_D NodeD1, NodeD2;
	int i;
	int nSize = arElemK.GetSize();
	if(nSize == 0) return 0.0;

	double dLength;
	for(i=0 ; i<nSize ; i++)
	{	
		ElemD.Initialize();
		SectD.Initialize();

		if(m_pDoc->m_pAttrCtrl->GetElemTsgrKey(arElemK[i]) != 0)
		{			
			if(!m_pDoc->m_pPostCtrl->GetElemPost(arElemK[i],  ElemD)) return 0.0;
			if(!m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro, SectD)) return 0.0;
		}
		else 
		{	
			if(!m_pDoc->m_pAttrCtrl->GetElem(arElemK[i], ElemD))	return 0.0;
			if(!GetSectData(arElemK[i], ElemD.elpro, SectD))	return 0.0;
		}
		
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) return 0.0;
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) return 0.0;

		if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) return 0.0;
		
		dLength = CMathFunc::mathLength(NodeD1.x, NodeD1.y, NodeD1.z, NodeD2.x, NodeD2.y, NodeD2.z);
		dWeight += MatdD.Data1.Analysis.Density * dLength * (SectD.SectBefore.SectI.Stiffness.Area + SectD.SectBefore.SectJ.Stiffness.Area);
	}

	return dWeight;
}

BOOL CDgnToolMPhi_SeismicDesign::CalcPhiDis(BOOL bMajorAxis, CArray<T_ELEM_K, T_ELEM_K>& arElemK, CArray<_DGN_MPHI_MOMENT_D, _DGN_MPHI_MOMENT_D>& arMoment, CArray<_DGN_MPHI_STRN_D, _DGN_MPHI_STRN_D&>& arMPhiStrn, double& dDis, int nBasicPos, double dBasicRx, double dBasicRy, double dBasicRz, BOOL bAbsDis)
{
	dDis = 0.0;
	double dBasicTheta=0.0;

	if(!CDBLib::GetSortElem_Position(arElemK)) return FALSE;
	int nSize = arElemK.GetSize();
	if(nSize != arMoment.GetSize()) return FALSE;
	if(nSize != arMPhiStrn.GetSize()) return FALSE;
	
	T_ELEM_D ElemD;
	T_NODE_D NodeD1, NodeD2;
	if(!m_pDoc->m_pAttrCtrl->GetElem(arElemK[0],ElemD))	return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetElem(arElemK[nSize-1],ElemD))	return FALSE;
	if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) return FALSE;
	BOOL bTopI = (NodeD1.z > NodeD2.z);
	//기준점의 위치조정 (1:I, 2:J, 3:Bottom, 4:Top)
	if(nBasicPos == 3)  nBasicPos = bTopI ? 2 : 1;
	if(nBasicPos == 4)  nBasicPos = bTopI ? 1 : 2;

	//회전각을 휨방향에 대한 회전각으로 변환
	double dLocalVector[3][3];
	m_pDoc->calcElemLocalVector(arElemK[0], dLocalVector);
	CMathFunc::mathGCS2UCS(dBasicRx, dBasicRy, dBasicRz, dLocalVector);
	if(bMajorAxis) { dBasicTheta = dBasicRy * (nBasicPos == 1 ? -1.0 :  1.0); }
	else           { dBasicTheta = dBasicRz * (nBasicPos == 1 ?  1.0 : -1.0); }

	CArray<double, double> arLength;
	CArray<double, double> arPhi_I, arPhi_J;// Elem의 I단 곡률 및 J단 곡률
	CArray<double, double> arTheta, arDis;  // Elem의 I단(nBasicPos==2) 또는 J단(nBasicPos==1) 회전각, Elem구간의 변위
	arLength.SetSize(nSize);
	arPhi_I.SetSize(nSize);
	arPhi_J.SetSize(nSize);
	arTheta.SetSize(nSize);
	arDis.SetSize(nSize);

	int i;
	for(i=0 ; i<nSize ; i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetElem(arElemK[i],ElemD))	return FALSE;
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1)) return FALSE;
		if(!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2)) return FALSE;
		arLength[i] = CMathFunc::mathLength(NodeD1.x, NodeD1.y, NodeD1.z, NodeD2.x, NodeD2.y, NodeD2.z);
		arPhi_I[i]  = Calc_Phi(bMajorAxis, 0, arMoment[i].dM[0], arMPhiStrn[i]);
		arPhi_J[i]  = Calc_Phi(bMajorAxis, 2, arMoment[i].dM[2], arMPhiStrn[i]);
	}
	
	if(nBasicPos == 1)
	{	
		for(i=0 ; i<nSize ; i++)
		{
			if(i==0) 
			{
				arTheta[i] = (arPhi_I[i]+arPhi_J[i])/2.0 * arLength[i] + dBasicTheta;
				arDis[i]   = (arTheta[i]+dBasicTheta)/2.0 * arLength[i];
			}
			else
			{
				arTheta[i] = (arPhi_I[i]+arPhi_J[i])/2.0 * arLength[i] + arTheta[i-1];
				arDis[i]   = (arTheta[i]+arTheta[i-1])/2.0 * arLength[i];
			}
		}
	}
	else
	{
		for(i=nSize-1 ; i>=0 ; i--)
		{
			if(i==nSize-1) 
			{
				arTheta[i] = (arPhi_I[i]+arPhi_J[i])/2.0 * arLength[i] + dBasicTheta;
				arDis[i]   = (arTheta[i]+dBasicTheta)/2.0 * arLength[i];
			}
			else
			{
				arTheta[i] = (arPhi_I[i]+arPhi_J[i])/2.0 * arLength[i] + arTheta[i+1];
				arDis[i]   = (arTheta[i]+arTheta[i+1])/2.0 * arLength[i];
			}
		}
	}
	
	for(i=0 ; i<nSize ; i++)
		dDis += arDis[i];

	//수평에 대한 변위로 변환시켜줌
	double dHorizonFactor = 1.0;
	if(bMajorAxis)
	{ dHorizonFactor = sqrt(dLocalVector[2][0]*dLocalVector[2][0] + dLocalVector[2][1]*dLocalVector[2][1]); }
	else 
	{ dHorizonFactor = sqrt(dLocalVector[1][0]*dLocalVector[1][0] + dLocalVector[1][1]*dLocalVector[1][1]); }

	dDis *= dHorizonFactor; 

	if(bAbsDis) dDis = fabs(dDis);

	return TRUE;
}

double CDgnToolMPhi_SeismicDesign::Calc_Phi(BOOL bMajorAxis, int iPosi, double dMoment, _DGN_MPHI_STRN_D& MPhiStrnD)
{
	if(!MPhiStrnD.bUserCurve)
	{	
		int iAxisDir;
		if(bMajorAxis) iAxisDir = dMoment>=0.0 ? 0 : 1;
		else           iAxisDir = dMoment>=0.0 ? 2 : 3;

		double dpMom[4];
		double dpPhi[4];

		int i;
		for(i=0 ; i<4 ; i++)
		{
			if(iPosi == 0)
			{
				dpMom[i] = MPhiStrnD.dpMom_I[i][iAxisDir];
				dpPhi[i] = MPhiStrnD.dpPhi_I[i][iAxisDir];
			}
			else if(iPosi == 1)
			{
				dpMom[i] = MPhiStrnD.dpMom_M[i][iAxisDir];
				dpPhi[i] = MPhiStrnD.dpPhi_M[i][iAxisDir];
			}
			else
			{
				dpMom[i] = MPhiStrnD.dpMom_J[i][iAxisDir];
				dpPhi[i] = MPhiStrnD.dpPhi_J[i][iAxisDir];
			}
		}
		

		for(i=0 ; i<4 ; i++)
		{
			if(fabs(dMoment) < dpMom[i])
			{
				if(i==0) return dpPhi[i]/dpMom[i]*fabs(dMoment) * (dMoment<0 ? -1.0 : 1.0);
				else     return (dpPhi[i-1] + (dpPhi[i]-dpPhi[i-1])/(dpMom[i]-dpMom[i-1])*(fabs(dMoment)-dpMom[i-1])) * (dMoment<0 ? -1.0 : 1.0);
			}
		}
		// 종국시의 강도를 초과하는 경우 종국시의 값을 넘겨줌
		return dpPhi[3];
	}

	CArray<T_RSDL_MPHI_D,T_RSDL_MPHI_D> arUserMPhi;
	if(bMajorAxis) arUserMPhi.Copy(MPhiStrnD.arUserMPhi[0]);
	else           arUserMPhi.Copy(MPhiStrnD.arUserMPhi[1]);

	double dM_min, dM_max;
	double dPhi_min, dPhi_max;
	int nSize = arUserMPhi.GetSize();
	int i;
	for(i=0 ; i<nSize ; i++)
	{
		if(i==0)
		{
			dM_min   = dM_max   = arUserMPhi[i].dMoment;
			dPhi_min = dPhi_max = arUserMPhi[i].dCurvature;
		}
		else 
		{
			dM_min   = min(dM_min,   arUserMPhi[i].dMoment);
			dM_max   = max(dM_max,   arUserMPhi[i].dMoment);
			dPhi_min = min(dPhi_min, arUserMPhi[i].dCurvature);
			dPhi_max = max(dPhi_max, arUserMPhi[i].dCurvature);
		}
	}

	BOOL bSig = TRUE;
	double dMaxM = (iPosi==0) ? MPhiStrnD.dpMom_I[3][dMoment > 0.0 ? 0 : 1] : MPhiStrnD.dpMom_J[3][dMoment > 0.0 ? 0 : 1];
	double dMaxPHI = (iPosi==0) ? MPhiStrnD.dpPhi_I[3][dMoment > 0.0 ? 0 : 1] : MPhiStrnD.dpPhi_J[3][dMoment > 0.0 ? 0 : 1];

	if(dMoment < 0.0)
	{ 
		if(dM_min >= 0.0 && dPhi_min >= 0.0) 
		{ bSig = FALSE;  dMoment = fabs(dMoment); }
	}
	if(dMoment > dMaxM)
	{
		return (bSig ? 1.0 : -1.0) * dMaxPHI;
	}

	for(i=0 ; i<nSize ; i++)
	{
		if(dMoment < arUserMPhi[i].dMoment)
		{
			if(i==0) 
			{
				if(dM_min >= 0.0 && dPhi_min >= 0.0) return (bSig ? 1.0 : -1.0) * arUserMPhi[i].dCurvature/arUserMPhi[i].dMoment*dMoment;
				else                                 return dPhi_min;
			}
			else   
			{
				double dx1 = arUserMPhi[i-1].dCurvature;
				double dx2 = arUserMPhi[i].dCurvature;
				double dy1 = arUserMPhi[i-1].dMoment;
				double dy2 = arUserMPhi[i].dMoment;
				double dy = dMoment;
				double dx= dx1 + (dy - dy1) * (dx2 - dx1) / (dy2 - dy1);

				return (bSig ? 1.0 : -1.0) * dx;
		
			}
		}
	}

	return (bSig ? dPhi_max : dPhi_min);
}
double CDgnToolMPhi_SeismicDesign::Get_dBv(BOOL bAxisY, MPHI_SECT_DATA& InData)
{  
	double dBv = 0.0;
	const int nSectCon = InData.nSectCon;
	const auto& SectBaseD = InData.SectBaseD;

	switch (nSectCon)
	{
	case  PMCV_SECT_B:
		dBv = (bAxisY ? 2*SectBaseD[0].Size[2] : 2*SectBaseD[0].Size[3]);
		break;
	case PMCV_SECT_P:
		{
			double dB_Out = SectBaseD[0].Size[0];                 //Pipe 바깥 직경
			double dB_In  = dB_Out - (2.*SectBaseD[0].Size[1]);   //Pipe 안쪽 직경
			dBv = sqrt(dPI*pow(dB_Out,2)/4.) - sqrt(dPI*pow(dB_In,2)/4.);
		}
		break;
	case  PMCV_SECT_SR:
		dBv = sqrt(SectBaseD[0].Stiffness.Area);
		break;
	case  PMCV_SECT_SB:
		dBv = (bAxisY ? SectBaseD[0].Size[1] : SectBaseD[0].Size[0]);
		break;
	case  PMCV_SECT_OCT:
		dBv = 2*SectBaseD[0].Size[4]; 
		break;
	case  PMCV_SECT_SOCT:
		dBv = (bAxisY ? SectBaseD[0].Stiffness.Area/SectBaseD[0].Size[0] : SectBaseD[0].Stiffness.Area/SectBaseD[0].Size[1]);
		break;
	case  PMCV_SECT_TRK:
		dBv = 2*SectBaseD[0].Size[2];
		break;
	case  PMCV_SECT_STRK:
	case  PMCV_SECT_HTRK:
		dBv = (bAxisY ? SectBaseD[0].Stiffness.Area/SectBaseD[0].Size[0] : SectBaseD[0].Stiffness.Area/SectBaseD[0].Size[1]);
		break;
	case  PMCV_SECT_GEN:
		dBv = Get_dBv_Gen(bAxisY, InData);
		break;
	default:
		ASSERT(0);
		break;
	}
	return dBv;
}

double CDgnToolMPhi_SeismicDesign::Get_dBv_Gen(BOOL bAxisY, MPHI_SECT_DATA& InData)
{	
	double dBv_Cent = 0.0, dBv_Area = 0.0;
	const auto& SectBaseD = InData.SectBaseD;
	const int nOutSize = SectBaseD[0].aOuterPolygon.GetSize();
	const int nInSize  = SectBaseD[0].aInnerPolygon.GetSize();

	if(bAxisY && 0.0 != (SectBaseD[0].Stiffness.Czm + SectBaseD[0].Stiffness.Czp))
		dBv_Area =  SectBaseD[0].Stiffness.Area/(SectBaseD[0].Stiffness.Czm + SectBaseD[0].Stiffness.Czp);
	else if(!bAxisY && 0.0 != (SectBaseD[0].Stiffness.Cym + SectBaseD[0].Stiffness.Cyp))
		dBv_Area =  SectBaseD[0].Stiffness.Area/(SectBaseD[0].Stiffness.Cym + SectBaseD[0].Stiffness.Cyp);

	
	double dLine[2][2];
	if(bAxisY) 
	{
		dLine[0][0] = -2.0 * SectBaseD[0].Stiffness.Cym;
		dLine[1][0] =  2.0 * SectBaseD[0].Stiffness.Cyp;
		dLine[0][1] = dLine[1][1] = 0.0;
	}
	else 
	{
		dLine[0][0] = dLine[1][0] = 0.0;
		dLine[0][1] = -2.0 * SectBaseD[0].Stiffness.Czm;
		dLine[1][1] =  2.0 * SectBaseD[0].Stiffness.Czp;			
	}

	int nPointSize = 0;
	double (*coor2D)[2];
	for(int i = 0 ; i < nOutSize ; ++i)
	{
		nPointSize = SectBaseD[0].aOuterPolygon[i].aVertex.GetSize();
		coor2D = new double[nPointSize][2];  	
		for(int j = 0 ; j < nPointSize ; ++j)
		{
			coor2D[j][0] = SectBaseD[0].aOuterPolygon[i].aVertex[j].dX;
			coor2D[j][1] = SectBaseD[0].aOuterPolygon[i].aVertex[j].dY;
		}

		dBv_Cent += CMathFunc::mathsInsideLength(dLine, nPointSize, coor2D);
		delete [] coor2D;			
	}

	for(int i = 0 ; i < nInSize ; ++i)
	{
		nPointSize = SectBaseD[0].aInnerPolygon[i].aVertex.GetSize();
		coor2D = new double[nPointSize][2];  	
		for(int j = 0 ; j < nPointSize ; ++j)
		{
			coor2D[j][0] = SectBaseD[0].aInnerPolygon[i].aVertex[j].dX;
			coor2D[j][1] = SectBaseD[0].aInnerPolygon[i].aVertex[j].dY;
		}

		dBv_Cent -= CMathFunc::mathsInsideLength(dLine, nPointSize, coor2D);
		delete [] coor2D;			
	}
	return min(dBv_Area, dBv_Cent);
}

double CDgnToolMPhi_SeismicDesign::Calc_cc(int nTypeEq)
{
	if(nTypeEq == 1) return 0.6;
	if(nTypeEq == 2) return 0.8;
	ASSERT(0);
	return 0.0;
}
double CDgnToolMPhi_SeismicDesign::Calc_ce(double dDeff)
{		
	double arDeff[4] = {1000.0, 3000.0, 5000.0, 10000.0};
	double arCe[4]   = {1.0,    0.7,    0.6,    0.5    };
	if(dDeff < arDeff[0]) return arCe[0];
	
	for(int i=1 ; i<4 ; i++)
	{
		if(dDeff < arDeff[i])
		{
			return arCe[i-1] + (arCe[i]-arCe[i-1])/(arDeff[i]-arDeff[i-1])*(dDeff-arDeff[i-1]);
		}
	}
	return arCe[3];
}
double CDgnToolMPhi_SeismicDesign::Calc_cpt(double dPt)
{
	double arPt[4]  = {0.2, 0.3, 0.5, 1.0};
	double arCpt[4] = {0.9, 1.0, 1.2, 1.5};
	for(int i=1 ; i<4 ; i++)
	{
		if(dPt < arPt[i])
		{
			return arCpt[i-1] + (arCpt[i]-arCpt[i-1])/(arPt[i]-arPt[i-1])*(dPt-arPt[i-1]);
		}
	}
	return arCpt[3];
}
double CDgnToolMPhi_SeismicDesign::Calc_ta(int nStressStrainType, double dfck)
{
	double arFck[5] = {21.0, 24.0, 27.0, 30.0, 40.0};
	double arTc[5]  = {0.33, 0.35, 0.36, 0.37, 0.41};
	double dTc = 0.0;
	if (nStressStrainType==PMCV_CODE_MPHI_JP5_24)
	{
		//5299 콘크리트의 설계강도 상한치  30(N/mm2)으로 제한되었음
		for(int i=1 ; i<3 ; i++)
		{
			if(dfck < arFck[i])
			{
				return arTc[i-1] + (arTc[i]-arTc[i-1])/(arFck[i]-arFck[i-1])*(dfck-arFck[i-1]);
			}
		}
		dTc = arTc[2] + (arTc[3]-arTc[2])/(arFck[3]-arFck[2])*(dfck-arFck[2]);
	}
	else
	{
		for(int i=1 ; i<4 ; i++)
		{
			if(dfck < arFck[i])
			{
				return arTc[i-1] + (arTc[i]-arTc[i-1])/(arFck[i]-arFck[i-1])*(dfck-arFck[i-1]);
			}
		}
		dTc = arTc[3] + (arTc[4]-arTc[3])/(arFck[4]-arFck[3])*(dfck-arFck[3]);
	}
	
	return dTc;
}

double CDgnToolMPhi_SeismicDesign::Calc_cz(int nStressStrainType, int nRegionalKind, int nTypeEq)
{
	//신규준(평성 24년) 0:A1 1:A2 2:B1 3:B2 4:C
	if (nStressStrainType==PMCV_CODE_MPHI_JP5_24)
	{
		if(nRegionalKind == 0) return (nTypeEq==1) ? 1.2 : 1.0;  //A1
		if(nRegionalKind == 1) return (nTypeEq==1) ? 1.0 : 1.0;  //A2
		if(nRegionalKind == 2) return (nTypeEq==1) ? 1.2 : 0.85; //B1
		if(nRegionalKind == 3) return (nTypeEq==1) ? 1.0 : 0.85; //B2
		if(nRegionalKind == 4) return (nTypeEq==1) ? 0.8 : 0.7;  //C
	}
	else
	{
		if(nRegionalKind == 0) return 1.0; // A
		if(nRegionalKind == 1) return 0.85;// B
		if(nRegionalKind == 2) return 0.7; // C
	}

	ASSERT(0);
	return 0.0;
}
double CDgnToolMPhi_SeismicDesign::Calc_Khc0(int nStressStrainType, int nTypeEq, int nGroundKind, double dT)
{
	if(nTypeEq==1)
	{// TypeI
		if (nStressStrainType==PMCV_CODE_MPHI_JP5_24)
		{
			if(nGroundKind == 0)
			{// I
				if(dT < 0.16)      return 2.58*pow(dT,(1.0/3.0));
				else if(dT <= 0.6) return 1.4;
				else               return 0.996*pow(dT,(-2.0/3.0));
			}
			else if(nGroundKind == 1)
			{// II
				if(dT < 0.22)      return 2.15*pow(dT,(1.0/3.0));
				else if(dT <= 0.9) return 1.3;
				else               return 1.21*pow(dT,(-2.0/3.0));
			}
			else if(nGroundKind == 2)
			{// III
				if(dT < 0.34)      return 1.72*pow(dT,(1.0/3.0));
				else if(dT <= 1.4) return 1.2;
				else               return 1.5*pow(dT,(-2.0/3.0));
			}
		}
		else
		{
			if(nGroundKind == 0)
			{// I
				if(dT <= 1.4) return 0.7;
				else          return 0.876*pow(dT,(-2.0/3.0));
			}
			else if(nGroundKind == 1)
			{// II
				if(dT < 0.18)      return max(0.7, 1.51*pow(dT,(1.0/3.0)));
				else if(dT <= 1.6) return 0.85;
				else               return 1.16*pow(dT,(-2.0/3.0));
			}
			else if(nGroundKind == 2)
			{// III
				if(dT < 0.29)      return max(0.7, 1.51*pow(dT,(1.0/3.0)));
				else if(dT <= 2.0) return 1.0;
				else               return 1.59*pow(dT,(-2.0/3.0));
			}
		}
	}
	else if(nTypeEq==2)
	{// TypeII
		if(nGroundKind == 0)
		{// I
			if(dT < 0.3)       return 4.46*pow(dT,(2.0/3.0));
			else if(dT <= 0.7) return 2.0;
			else               return 1.24*pow(dT,(-4.0/3.0));
		}
		else if(nGroundKind == 1)
		{// II
			if(dT < 0.4)       return 3.22*pow(dT,(2.0/3.0));
			else if(dT <= 1.2) return 1.75;
			else               return 2.23*pow(dT,(-4.0/3.0));
		}
		else if(nGroundKind == 2)
		{// III
			if(dT < 0.5)       return 2.38*pow(dT,(2.0/3.0));
			else if(dT <= 1.5) return 1.50;
			else               return 2.57*pow(dT,(-4.0/3.0));
		}
	}
	ASSERT(0);
	return 0.0;
}
CString CDgnToolMPhi_SeismicDesign::Get_SectName(int nShape)
{	
	if(nShape == PMCV_SECT_B) // Box Shape
		return _LS(IDS_DGN_COLM_TEXT_SECT_BOX);
	else if(nShape == PMCV_SECT_P) // Pipe Shape
		return _LS(IDS_DGN_COLM_TEXT_SECT_PIPE);
	else if(nShape == PMCV_SECT_SR) // Solid Round Shape
		return _LS(IDS_DGN_COLM_TEXT_SECT_SRND);
	else if(nShape == PMCV_SECT_SB) // Solid Box Shape
		return _LS(IDS_DGN_COLM_TEXT_SECT_SREC);
	else if(nShape == PMCV_SECT_OCT) // Octagon Shape
		return _LS(IDS_DGN_COLM_TEXT_SECT_OCTA);
	else if(nShape == PMCV_SECT_SOCT) // Solid Octagon Shape
		return _LS(IDS_DGN_COLM_TEXT_SECT_SOCT);
	else if(nShape == PMCV_SECT_TRK) // Track Shape
		return _LS(IDS_DGN_COLM_TEXT_SECT_TRCK);
	else if(nShape == PMCV_SECT_STRK) // Solid Track Shape
		return _LS(IDS_DGN_COLM_TEXT_SECT_STRK);
	else if(nShape == PMCV_SECT_HTRK) // Half Track Shape
		return _LS(IDS_DGN_COLM_TEXT_SECT_HTRK);
	else if(nShape == PMCV_SECT_GEN)  // General Shape
		return _LS(IDS_DGN_COLM_TEXT_SECT_GEN);
	else ASSERT(0);

	return _T("");
}

CString CDgnToolMPhi_SeismicDesign::Get_MembTypeName(int nType)
{
	if(nType == 0) return _LS(IDS_DGN_RLC_RCSINGLEPIER);
	else ASSERT(0);

	return _T("");
}

CString CDgnToolMPhi_SeismicDesign::Get_FailureName(int nType)
{
	if(nType == 0) return _LS(IDS_DGN_RLC_MOMENTFAILURE);
	if(nType == 1) return _LS(IDS_DGN_RLC_MOMSHEARFAILURE);
	if(nType == 2) return _LS(IDS_DGN_RLC_SHEARFAILURE);
	ASSERT(0);

	return _T("");
}

void CDgnToolMPhi_SeismicDesign::PrintErrorMessage(int nErrID, UINT ElemK)
{
	if(nErrID > 0)
	{
		// Show Message.
		CString strMsg=_T("");
				 if(nErrID==PMCV_ERR_ELEM_DATA)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_ELEM_DATA), ElemK);
		else if(nErrID==PMCV_ERR_MATL_DATA)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_MATL_DATA), ElemK);
		else if(nErrID==PMCV_ERR_SECT_DATA)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_SECT_DATA), ElemK);
		else if(nErrID==PMCV_ERR_RBAR_DATA)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_RBAR_DATA), ElemK);
		else if(nErrID==PMCV_ERR_FRAM_DATA)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_FRAM_DATA), ElemK);
		else if(nErrID==PMCV_ERR_MEMB_TYPE)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_MEMB_TYPE), ElemK);
		else if(nErrID==PMCV_ERR_AREA_MAKE)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_AREA_MAKE), ElemK);
		else if(nErrID==PMCV_ERR_RBAR_ARRY)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_RBAR_ARRY), ElemK);
		else if(nErrID==PMCV_ERR_TNDN_ARRY)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_TNDN_ARRY), ElemK);
		else if(nErrID==PMCV_ERR_RBAR_KIND)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_RBAR_KIND), ElemK);
		else if(nErrID==PMCV_ERR_MPHI_DATA)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_MPHI_DATA), ElemK);
		else if(nErrID==PMCV_ERR_INTL_DATA)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_INTL_DATA), ElemK);
		else if(nErrID==PMCV_ERR_POSI_DATA)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_POSI_DATA), ElemK);
		else if(nErrID==PMCV_ERR_PMCV_MYLD)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_PMCV_MYLD), ElemK);
		else if(nErrID==PMCV_ERR_PMCV_MULT)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_PMCV_MULT), ElemK);
		else if(nErrID==PMCV_ERR_PMCV_MCRK)   strMsg.Format(_LS(IDS_DB_PSCD_SKIP_PMCV_MCRK), ElemK);
		else if(nErrID==RLCC_ERR_TSG_SHAPE_SCALE) strMsg = _LS(IDS_DGN_RLCC_SKIP_TSG_SHAPE_SCALE);
		else if(nErrID==RLCC_ERR_TSG_RBAR_EQ) strMsg = _LS(IDS_DGN_RLCC_SKIP_TSG_RBAR_EQ);
		else if(nErrID==RLCC_ERR_SHEAR_RBAR)  strMsg = _LS(IDS_DGN_RLCC_SKIP_SHEAR_RBAR);
		else	ASSERT(0);
	  GSaveHistoryFormatNF(strMsg);
	}
}

BOOL CDgnToolMPhi_SeismicDesign::GetSectData(int iElem, int iSect, T_SECT_D& SectD)
{
	SectD.Initialize();
	int iMatlType = 0;
	int iMembType = 0;

	T_ELEM_D ElemD;
	ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(iElem, ElemD))	return FALSE;

	T_MATD_D MatdD;
	MatdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD);
	if(MatdD.Type==_T("S"))					iMatlType = PMCV_MATL_STL;
	else if(MatdD.Type==_T("C"))		iMatlType = PMCV_MATL_CON;
	else if(MatdD.Type==_T("SRC"))	iMatlType = PMCV_MATL_SRC;
	else	return FALSE;

	if(m_pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
	{
		if(m_pDoc->m_pAttrCtrl->ExistMbtp(iElem))	// by User.
		{
			T_MBTP_D MbtpD;
			MbtpD.Initialize();
			m_pDoc->m_pAttrCtrl->GetMbtp(iElem,MbtpD);
			iMembType = MbtpD.nMbType;
		}
		else	// by Program.
		{
			iMembType = m_pDoc->m_pAttrCtrl->GetMemberType(iElem);
		}
	}
	
	
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(iSect, SectD))	return FALSE;
	int iStype1 = SectD.nStype;
	int iStype2 = SectD.SectBefore.nStype;
	CString strSectShape = SectD.SectBefore.Shape;
	
	// Change by ZINU.('05.03.07). If CMPWEB(파형강판), Src -> Con.
	if(strSectShape==D_SECT_SHAPE_PSC_CMPWEB)	iMatlType = PMCV_MATL_CON;

	BOOL bConc = (iMatlType==PMCV_MATL_CON ? TRUE : FALSE);
	BOOL bBeam = (iMembType==D_MBTP_BEAM ? TRUE : FALSE);
	BOOL bPscb = (iStype1==D_SECT_TYPE_PSC || (iStype1==D_SECT_TYPE_TAPERED && iStype2==D_SECT_TYPE_PSC) ? TRUE : FALSE);
	if(bConc && bBeam && bPscb)
	{
		// Remember NOT Secd But Sect (Only PSC).
		if(!m_pDoc->m_pAttrCtrl->GetSect(iSect, SectD))	return FALSE;
	}
	return TRUE;
}

// 가상함수 재정의 ///////////////////////////////////////////////////////

void CDgnToolMPhi_SeismicDesign::Init_InputData()
{
	//CDbToolMphi::Init_InputData();
	//추가된 변수 초기화
	//m_iRchkPosiCvl_Gen_Main = m_iRchkPosiCvl_Gen_Sub = 0;
	//m_tRchkColmCvl_Gen.Initialize();
	
	//m_dWeightDensity = 0.0;
	//m_da_tie = 0.0;    
	//m_dAngle_tie[0] = m_dAngle_tie[1] = 0.0;
	//m_dAw_tie[0] = m_dAw_tie[1] = 0.0;

}

BOOL CDgnToolMPhi_SeismicDesign::Get_MatlData(int iMatl, int iSect, MPHI_MATL_DATA& InData)
{
	T_MATD_D MatdD;
	MatdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(iMatl, MatdD)) return FALSE;
	InData.dDensity = MatdD.Data1.Analysis.Density;
	return CDbToolMphi::Get_MatlData(iMatl, iSect, InData);
}

BOOL CDgnToolMPhi_SeismicDesign::Get_SectData(int iElem, int iSect, int iPosi, int nMembType, int nMatlType,
                                              const MPHI_MATL_DATA& MatlD, const MPHI_USER_DATA& UserD, MPHI_SECT_DATA& InData)
{
	if(nMatlType != PMCV_MATL_CON) return CDbToolMphi::Get_SectData(iElem, iSect, iPosi, nMembType, nMatlType, MatlD, UserD, InData);
	/////////////////////////
	// Get Section.
	T_SECT_D SectD;
	SectD.Initialize();
	if(m_pDoc->m_pAttrCtrl->GetElemTsgrKey(iElem) != 0)
	{
		T_ELEM_D ElemD;  ElemD.Initialize();
		if(!m_pDoc->m_pPostCtrl->GetElemPost(iElem,       ElemD)) return FALSE;
		if(!m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro, SectD)) return FALSE;
	}
	else 
	{	
		if(!GetSectData(iElem, iSect, SectD))	return FALSE;
	}
	int iStype1 = SectD.nStype;
	int iStype2 = SectD.SectBefore.nStype;
	CString strSectShape = SectD.SectBefore.Shape;
	
	BOOL bConc = (nMatlType==PMCV_MATL_CON ? TRUE : FALSE);
	BOOL bBeam = (nMembType==D_MBTP_BEAM ? TRUE : FALSE);
	BOOL bPscb = (iStype1==D_SECT_TYPE_PSC || (iStype1==D_SECT_TYPE_TAPERED && iStype2==D_SECT_TYPE_PSC) ? TRUE : FALSE);

	if(bPscb)                    return CDbToolMphi::Get_SectData(iElem, iSect, iPosi, nMembType, nMatlType, MatlD, UserD, InData);
	if(iStype1==D_SECT_TYPE_SRC) return CDbToolMphi::Get_SectData(iElem, iSect, iPosi, nMembType, nMatlType, MatlD, UserD, InData);

	InData.nSectK = iSect;
	T_SECT_SECTBASE_D SectBaseD[2];
	SectBaseD[0].Initialize();
	SectBaseD[1].Initialize();
	// Con/Stl.
	SectBaseD[0] = (iPosi>0 && iStype1==D_SECT_TYPE_TAPERED ? SectD.SectBefore.SectJ : SectD.SectBefore.SectI);

	if(nMatlType==PMCV_MATL_CON)
	{
		if(nMembType==D_MBTP_BEAM)	// Beam.
		{
			// Check Sect Type.
			if(iStype1==D_SECT_TYPE_REGULAR)	// RC-Beam.
			{
				// Check Sect Shape.
				if(strSectShape==_T("T"))				InData.nSectCon = PMCV_SECT_T;
				else if(strSectShape==_T("SB"))	InData.nSectCon = PMCV_SECT_SB;
				else												return FALSE;
			}
			else	return FALSE;
		}
		else if(nMembType==D_MBTP_COLUMN)	// Column.
		{
			// Check Sect Type.
			if(iStype1==D_SECT_TYPE_REGULAR || iStype1==D_SECT_TYPE_TAPERED)	
			{			
				// Check Sect Shape.
				if(strSectShape==D_SECT_SHAPE_REG_B)					InData.nSectCon = PMCV_SECT_B;
				else if(strSectShape==D_SECT_SHAPE_REG_P)			InData.nSectCon = PMCV_SECT_P;
				else if(strSectShape==D_SECT_SHAPE_REG_SR)		InData.nSectCon = PMCV_SECT_SR;
				else if(strSectShape==D_SECT_SHAPE_REG_SB)		InData.nSectCon = PMCV_SECT_SB;
				else if(strSectShape==D_SECT_SHAPE_REG_OCT)		InData.nSectCon = PMCV_SECT_OCT;
				else if(strSectShape==D_SECT_SHAPE_REG_SOCT)	InData.nSectCon = PMCV_SECT_SOCT;
				else if(strSectShape==D_SECT_SHAPE_REG_TRK)		InData.nSectCon = PMCV_SECT_TRK;
				else if(strSectShape==D_SECT_SHAPE_REG_STRK)	InData.nSectCon = PMCV_SECT_STRK;
				else if(strSectShape==D_SECT_SHAPE_REG_HTRK)	InData.nSectCon = PMCV_SECT_HTRK;
				else if(strSectShape==D_SECT_SHAPE_REG_GEN) 	InData.nSectCon = PMCV_SECT_GEN;
				else return FALSE;
			}
			else if(iStype1==D_SECT_TYPE_USER)
			{
				if(strSectShape==D_SECT_SHAPE_REG_GEN) 	  InData.nSectCon = PMCV_SECT_GEN;
				else	return FALSE;
			}
			else return FALSE;
		}
		else	return FALSE;
		// Set Sect Data.
		InData.dAreaCon = SectBaseD[0].Stiffness.Area;
		InData.dIyyCon  = SectBaseD[0].Stiffness.Ryy;
		InData.dIzzCon  = SectBaseD[0].Stiffness.Rzz;
		InData.dCymCon	= SectBaseD[0].Stiffness.Cym;
		InData.dCypCon	= SectBaseD[0].Stiffness.Cyp;
		InData.dCzmCon	= SectBaseD[0].Stiffness.Czm;
		InData.dCzpCon	= SectBaseD[0].Stiffness.Czp;
	}
	else 
	{ ASSERT(0); return FALSE; }
	// Only SRC is NOT 1.O.
	// Save  
  const double dRatE = MatlD.Get_RatioE();
	InData.dArea= InData.dAreaCon + dRatE*InData.dAreaStl;
	InData.dIyy	= InData.dIyyCon + dRatE*InData.dIyyStl;
	InData.dIzz	= InData.dIzzCon + dRatE*InData.dIzzStl;
	InData.dCym	= max(InData.dCymCon, InData.dCymStl);
	InData.dCyp	= max(InData.dCypCon, InData.dCypStl);
	InData.dCzm	= max(InData.dCzmCon, InData.dCzmStl);
	InData.dCzp	= max(InData.dCzpCon, InData.dCzpStl);
	InData.SectBaseD[0] = SectBaseD[0];
	InData.SectBaseD[1] = SectBaseD[1];

	return TRUE;
}


BOOL CDgnToolMPhi_SeismicDesign::Get_RbarData(int iElem, int iSect, int iPosi, int nMembType, const MPHI_SECT_DATA&SectD, MPHI_RBAR_DATA& InData)
{		
	if(iPosi > 2)	ASSERT(0);
	//Input
	const int nSectCon = SectD.nSectCon;
	const auto& SectBaseD = SectD.SectBaseD;
	// RC Beam, Column.
	T_RCHK_D RchkD;
	RchkD.Initialize();
	BOOL bGetRchk = m_pDoc->m_pAttrCtrl->GetRchk(iSect, RchkD);
	if(bGetRchk)
	{
	  if(nMembType==D_MBTP_BEAM)
		{
			InData.nRchkPosiCvl = iPosi;	// 0=I,1=M,2=J.
			InData.tRchkBeamCvl = RchkD.BEAM;
		}
		else if(nMembType==D_MBTP_COLUMN)
		{
			InData.nRchkPosiCvl = 0;	// Always 0=I.
			InData.tRchkColmCvl = RchkD.COLM;
		}
		else	return FALSE;
		double dSbarAs	= m_pDoc->m_pMatlDB->Get_RebarArea(InData.tRchkColmCvl.strSubBarNa[0]);
		InData.da_tie = InData.tRchkColmCvl.dSubBarDist[0];    
		//***// PMS.5568 Civil 일본 내진설계 대응 - [전단내력] 전단내력 산출 시의 전단철근 단면적 Aw
		InData.dAw_tie[0] = dSbarAs * InData.tRchkColmCvl.dSubBarNum_y[0];
		InData.dAw_tie[1] = dSbarAs * InData.tRchkColmCvl.dSubBarNum_z[0];
	}
	// PSC-Beam.
	T_RPSC_D RpscD;
	RpscD.Initialize();
	BOOL bGetRpsc = m_pDoc->m_pAttrCtrl->GetRpsc(iSect, RpscD);
	if(bGetRpsc)
	{
		if(nMembType==D_MBTP_BEAM)
		{
			InData.nRpscPosiCvl = (RpscD.bMbarJ && iPosi>0 ? 1 : 0);	// 0=I,1=J.
			InData.tRpscBeamCvl = RpscD;
		}
		else	return FALSE;
	}
	// RC Beam General Section	
	T_REBT_D RebtD;
	BOOL bGetRebt = m_pDoc->m_pAttrCtrl->GetRebt(iSect, RebtD);//현재(09.06.26) 일본 설계시 T_RBTE_D는사용하지 않음
	if(bGetRebt)
	{
		if(nSectCon != PMCV_SECT_GEN) return FALSE;
		if(RebtD.bIJBothMain)  InData.nRchkPosiCvl_Gen_Main = 0;
		else                   InData.nRchkPosiCvl_Gen_Main = (iPosi==0 ? 0 : 2);
		if(RebtD.bIJBothShear) InData.nRchkPosiCvl_Gen_Sub  = 0;
		else                   InData.nRchkPosiCvl_Gen_Sub  = (iPosi==0 ? 0 : 2);

		if(m_pDoc->m_pAttrCtrl->GetElemTsgrKey(iElem) == 0)
		{
			InData.tRchkColmCvl_Gen = RebtD;			
		}
		else 
		{ 
			if(CSectUtil::IsScaleTaperedShape_Gen(iSect,TRUE)) { PrintErrorMessage(RLCC_ERR_TSG_SHAPE_SCALE, iElem);  return FALSE; }// _T("* Skip : 임의형상으로 이루어진 Tapered Section Group의 I,J 형상은 크기외에는 동일하여야 합니다.")
			if(!RebtD.bIJBothMain || !RebtD.bIJBothShear)      { PrintErrorMessage(RLCC_ERR_TSG_RBAR_EQ, iElem);  return FALSE; }// _T("* Skip : 임의형상으로 이루어진 Tapered Section Group의 철근 배근은 I,J공통적용이어야 합니다.")
			
			T_SECT_D SectD_Org;  SectD_Org.Initialize();
			GetSectData(iElem, iSect, SectD_Org);
			double dHalfY_Org  = (SectBaseD[0].Stiffness.Cym+SectBaseD[0].Stiffness.Cyp)/2.0;
			double dHalfZ_Org  = (SectBaseD[0].Stiffness.Czm+SectBaseD[0].Stiffness.Czp)/2.0;
			double dHalfY_Conv = (SectD_Org.SectBefore.SectI.Stiffness.Cym+SectD_Org.SectBefore.SectI.Stiffness.Cyp)/2.0;
			double dHalfZ_Conv = (SectD_Org.SectBefore.SectI.Stiffness.Czm+SectD_Org.SectBefore.SectI.Stiffness.Czp)/2.0;
			double dScaleY     = (SectBaseD[0].Stiffness.Cym+SectBaseD[0].Stiffness.Cyp)/(SectD_Org.SectBefore.SectI.Stiffness.Cym+SectD_Org.SectBefore.SectI.Stiffness.Cyp);
			double dScaleZ     = (SectBaseD[0].Stiffness.Czm+SectBaseD[0].Stiffness.Czp)/(SectD_Org.SectBefore.SectI.Stiffness.Czm+SectD_Org.SectBefore.SectI.Stiffness.Czp);

			InData.tRchkColmCvl_Gen = RebtD;
			int nSize = RebtD.arMainRebar[0].GetSize();
			InData.tRchkColmCvl_Gen.arMainRebar[InData.nRchkPosiCvl_Gen_Main].SetSize(nSize);
			T_REBT_MABR MainBarD;
			for(int i = 0; i < nSize; i++)
			{
				MainBarD = RebtD.arMainRebar[0].GetAt(i);
				MainBarD.dCenterY = dHalfY_Conv + (MainBarD.dCenterY-dHalfY_Org)*dScaleY;
				MainBarD.dCenterZ = dHalfZ_Conv + (MainBarD.dCenterZ-dHalfZ_Org)*dScaleZ;
				InData.tRchkColmCvl_Gen.arMainRebar[InData.nRchkPosiCvl_Gen_Main].SetAt(i, MainBarD);
			}
			InData.tRchkColmCvl_Gen.ShearRebar[1] = InData.tRchkColmCvl_Gen.ShearRebar[0];
			InData.tRchkColmCvl_Gen.ShearRebar[2] = InData.tRchkColmCvl_Gen.ShearRebar[0];
		}

		double dSbarAs= m_pDoc->m_pMatlDB->Get_RebarArea(InData.tRchkColmCvl_Gen.ShearRebar[InData.nRchkPosiCvl_Gen_Sub].strSubRebarName);
		InData.da_tie = InData.tRchkColmCvl_Gen.ShearRebar[InData.nRchkPosiCvl_Gen_Sub].dSubRebarSpace;    

		//***// PMS.5568 Civil 일본 내진설계 대응 - [전단내력] 전단내력 산출 시의 전단철근 단면적 Aw
		InData.dAw_tie[0] = dSbarAs * InData.tRchkColmCvl_Gen.ShearRebar[InData.nRchkPosiCvl_Gen_Sub].dSubRebarNum[0];
		InData.dAw_tie[1] = dSbarAs * InData.tRchkColmCvl_Gen.ShearRebar[InData.nRchkPosiCvl_Gen_Sub].dSubRebarNum[1];
	}
	if(bGetRchk || bGetRebt || bGetRpsc)	return TRUE;
	else											            return FALSE;
}


BOOL CDgnToolMPhi_SeismicDesign::Get_AreaUnit(int iElem, int iPosi, int nMembType, int nMatlType, int nSliceNum, 
																							BOOL bSlicing, const MPHI_RBAR_DATA& RbarD, MPHI_SECT_DATA& InData)
{
	if(nMatlType!= PMCV_MATL_CON)         return CDbToolMphi::Get_AreaUnit(iElem, iPosi, nMembType, nMatlType, nSliceNum, bSlicing, RbarD, InData);
	if(nMembType!= D_MBTP_COLUMN)	        return CDbToolMphi::Get_AreaUnit(iElem, iPosi, nMembType, nMatlType, nSliceNum, bSlicing, RbarD, InData);
	if(InData.nSectCon != PMCV_SECT_GEN)  return CDbToolMphi::Get_AreaUnit(iElem, iPosi, nMembType, nMatlType, nSliceNum, bSlicing, RbarD, InData);
	BOOL bCon=TRUE;
	_DB_POLY_DATA PolyData;
	_DB_POLY_LIST* pPolyInnList;
	_DB_POLY_LIST PolyOutList;
	_DB_PONT_UNIT PontUnit;

	int i=0, j=0, k=0, nSize=0, nPolySize=0;
	double dYc	= InData.dCym;
	double dZc	= InData.dCzm;


	// Outer(CCW).
	nPolySize = InData.SectBaseD[0].aOuterPolygon.GetSize();
	if(nPolySize != 1) return FALSE;
	PolyOutList.RemoveAll();
	
	nSize = InData.SectBaseD[0].aOuterPolygon[0].aVertex.GetSize();
	PolyOutList.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		PontUnit.Initialize();
		PontUnit.dy = InData.SectBaseD[0].aOuterPolygon[0].aVertex[i].dX + dYc; 
		PontUnit.dz = InData.SectBaseD[0].aOuterPolygon[0].aVertex[i].dY + dZc; 
		PolyOutList.SetAt(i,PontUnit);
	}
	PolyData.arOutPoly.Copy(PolyOutList);
	// Inner(CW).
	nPolySize = InData.SectBaseD[0].aInnerPolygon.GetSize();
	PolyData.arInnPoly.SetSize(nPolySize);
	for(i=0 ; i<nPolySize ; i++)
	{	
		pPolyInnList = new _DB_POLY_LIST;
		nSize = InData.SectBaseD[0].aInnerPolygon[i].aVertex.GetSize();
		pPolyInnList->SetSize(nSize);
		for(j=0 ; j<nSize ; j++)
		{			
			PontUnit.Initialize();
			PontUnit.dy = InData.SectBaseD[0].aInnerPolygon[i].aVertex[j].dX + dYc; 
			PontUnit.dz = InData.SectBaseD[0].aInnerPolygon[i].aVertex[j].dY + dZc; 
			pPolyInnList->SetAt(j,PontUnit);
		}
		PolyData.arInnPoly.SetAt(i,pPolyInnList);
	}
	// Get Sliced Parts.
	if(bSlicing)
	{
		double dyDim = InData.dCym + InData.dCyp;
		double dzDim = InData.dCzm + InData.dCzp;
		//*************************************
		// Get Polygon by Cutting Line.
		I_PolyMaker*  pPolyMaker;
		pPolyMaker = I_PolyMakerFactory::GetInstance()->New_PMaker();

		// For Debugging.
		BOOL bPrint=TRUE;
		//++++++++++++++++++++++++++
		if(m_bTestMphi)
		{
			Prt_Debug(bPrint,_T("++ LINE ++++++++++++++++++++++++++++++++++++++++++++++++"));
			Prt_Debug(bPrint,_T("++ Type, Number, Cent_y, Cent_z, Area."));
		}
		//++++++++++++++++++++++++++
		
		// Outer.
		CArray<XGL_3dp,XGL_3dp&> arOutCut;
		for(i=0; i<PolyData.arOutPoly.GetSize(); i++)
		{
			_DB_PONT_UNIT PontUnit = PolyData.arOutPoly.GetAt(i);
			double dx = PontUnit.dy;
			double dy = PontUnit.dz;
			XGL_3dp OutXGL;
			OutXGL.Set(dx,dy,0.0);
			arOutCut.Add(OutXGL);
			// For Debugging.
			//++++++++++++++++++++++++++
			if(m_bTestMphi)	Prt_Debug(bPrint,_T("OUT"),i+1,dx,dy);
			//++++++++++++++++++++++++++
		}
		pPolyMaker->SetOutterPoly(arOutCut);
		
		// Inner.
		for(i=0; i<PolyData.arInnPoly.GetSize(); i++)
		{
			CArray<XGL_3dp,XGL_3dp&> arInnCut;
			_DB_POLY_LIST* pPolyList = PolyData.arInnPoly[i];
			for(j=0; j<pPolyList->GetSize(); j++)
			{
				_DB_PONT_UNIT PontUnit = pPolyList->GetAt(j);
				double dx = PontUnit.dy;
				double dy = PontUnit.dz;
				XGL_3dp InnXGL;
				InnXGL.Set(dx,dy,0.0);
				arInnCut.Add(InnXGL);
				// For Debugging.
				//++++++++++++++++++++++++++
				if(m_bTestMphi)
				{
					CString strKind=_T(""); strKind.Format(_T("IN%d"),i+1);
					Prt_Debug(bPrint,strKind,j+1,dx,dy);
				}
				//++++++++++++++++++++++++++
			}
			pPolyMaker->AddInnerPoly(arInnCut);
		}

		// For Debugging.
		//++++++++++++++++++++++++++
		if(m_bTestMphi)
		{
			Prt_Debug(bPrint,_T("// PART ++++++++++++++++++++++++++++++++++++++++++++++++"));
			Prt_Debug(bPrint,_T("// Type, Number, Cent_y, Cent_z, Area."));
		}
		//++++++++++++++++++++++++++

		// Make Polygon by Cutting Line.
		MPHI_PART_LIST arPartUnit[2];
		_DB_PART_UNIT PartUnit;
		for(i=0; i<2; i++)	// 0=Major, 1=Minor.
		{
			double dPartH = (i==0 ? dzDim : dyDim)/(double)nSliceNum;
			CArray<XGL_Line3d,XGL_Line3d&> arSlicingLines;
			for(j=0; j<=nSliceNum; j++)
			{
				// For Tolerance.
				double dRat	 = (j==0 ? PMCV_dZERO : j/(double)nSliceNum);
				double dxSta = (i==0 ? 0.0-PMCV_dZERO   : dRat*dyDim);
				double dxEnd = (i==0 ? dyDim+PMCV_dZERO : dRat*dyDim);
				double dySta = (i==0 ? dRat*dzDim : 0.0-PMCV_dZERO  );
				double dyEnd = (i==0 ? dRat*dzDim : dzDim+PMCV_dZERO);
				XGL_Line3d LineGL;
				LineGL.m_P1.Set(dxSta,dySta,0.0);	// Sta.
				LineGL.m_P2.Set(dxEnd,dyEnd,0.0);	// End.
				arSlicingLines.Add(LineGL);
			}
			pPolyMaker->SetSlicingLines(arSlicingLines);

			if(!pPolyMaker->MakeSlicedPolygons())	ASSERT(0);

			// Set Size.
			int iResPolySize = pPolyMaker->GetResultPolyCount();
			arPartUnit[i].SetSize(iResPolySize);
			int iResPolyCount=0;

			// For Debugging.
			int iDebugCount=0;
			// Result Polygon.
			for(j=0; j<pPolyMaker->GetResultPolyCount(); j++)
			{
				CArray<XGL_3dp,XGL_3dp&> arResPoly;
				pPolyMaker->GetResultPoly(j,arResPoly);
				for(k=0; k<arResPoly.GetSize(); k++)
				{
					CArray<XGL_3dp,XGL_3dp&> arResPoly;
					pPolyMaker->GetResultPoly(j,arResPoly);
					int iResNum = arResPoly.GetSize();
					// Calculate Center(x,y), Area.
					//---------------------------------
					double* dpx = new double[iResNum];
					double* dpy = new double[iResNum];
					//---------------------------------
					for(k=0; k<iResNum; k++)
					{
						dpx[k] = arResPoly[k].x();
						dpy[k] = arResPoly[k].y();
					}
					double dxCen=0.0, dyCen=0.0, dArea=0.0;
					if(!CMathFunc::mathPolyCentroid(iResNum,dpx,dpy,dxCen,dyCen,dArea))	ASSERT(0);
					//---------------------------------
					delete []dpx;
					delete []dpy;
					//---------------------------------
					PartUnit.Initialize();
					PartUnit.dyz[0] = dxCen;
					PartUnit.dyz[1] = dyCen;
					PartUnit.dArea	= dArea;
					arPartUnit[i].SetAt(iResPolyCount, PartUnit);
					iResPolyCount++;
					// For Debugging.
					//++++++++++++++++++++++++++
					if(m_bTestMphi)
					{
						iDebugCount++;
						Prt_Debug(bPrint,(i==0?_T("HOR"):_T("VER")),iDebugCount,dxCen,dyCen,dArea);
					}
					//++++++++++++++++++++++++++
				}
			}
		}
		for(i=0; i<2; i++)	// 0=Major, 1=Minor.
		{
			PartUnit.Initialize();
			int iPartSize = arPartUnit[i].GetSize();
			if(bCon)	InData.arConUnit[i].SetSize(iPartSize);
			else			InData.arStlUnit[i].SetSize(iPartSize);
			for(j=0; j<iPartSize; j++)
			{
				PartUnit = arPartUnit[i].GetAt(j);
				if(bCon)	InData.arConUnit[i].SetAt(j,PartUnit);
				else			InData.arStlUnit[i].SetAt(j,PartUnit);
			}
		}
	}

	// Delete because of New Operator.
	for(i=0; i<PolyData.arInnPoly.GetSize(); i++)
	{
		if(PolyData.arInnPoly[i])
		{
			PolyData.arInnPoly[i]->RemoveAll();
			delete PolyData.arInnPoly[i];
			PolyData.arInnPoly[i] = NULL;
		}
	}
	PolyData.arInnPoly.RemoveAll();
	
	return TRUE;
}
BOOL CDgnToolMPhi_SeismicDesign::Get_RbarUnit(int nMembType, int nMatlType, double* dd, const MPHI_SECT_DATA&SectD, MPHI_MATL_DATA& MatlD, MPHI_RBAR_DATA& InData)
{
	const double dFys = MatlD.dFys;
	const int nSectCon = SectD.nSectCon;
	if(nMatlType!= PMCV_MATL_CON) return CDbToolMphi::Get_RbarUnit(nMembType, nMatlType, dd, SectD, MatlD, InData);
	if(nMembType!= D_MBTP_COLUMN)	return CDbToolMphi::Get_RbarUnit(nMembType, nMatlType, dd, SectD, MatlD, InData);
	if(nSectCon != PMCV_SECT_GEN) return CDbToolMphi::Get_RbarUnit(nMembType, nMatlType, dd, SectD, MatlD, InData);

	InData.arRbarUnit.RemoveAll();
	const double dYc	= SectD.dCym;
	const double dZc	= SectD.dCzm;
	const int nSize = InData.tRchkColmCvl_Gen.arMainRebar[InData.nRchkPosiCvl_Gen_Main].GetSize();

	double dDia = 0.0;
	double dAs = 0.0;
	T_REBT_MABR MainBarD;
	_DB_MPHI_RBAR_UNIT RbarUnit;
	InData.arRbarUnit.SetSize(nSize);
	for(int i=0; i<nSize ; i++)
	{
		MainBarD = InData.tRchkColmCvl_Gen.arMainRebar[InData.nRchkPosiCvl_Gen_Main].GetAt(i);
		Get_RbarParam(MainBarD.strRebarName,dDia,dAs);
		if(dDia == 0.0 || dAs == 0.0) return FALSE;
		RbarUnit.Initialize();
		RbarUnit.dDia		= dDia;
		RbarUnit.dAs		= dAs;
		RbarUnit.dyz[0]	= MainBarD.dCenterY + dYc;
		RbarUnit.dyz[1]	= MainBarD.dCenterZ + dZc;
		InData.arRbarUnit.SetAt(i, RbarUnit);
	}

	// Get Rhos.
	// See ('04.12.13)사카이,마부치 회의자료. p.17.
	CString strSbarNa	= InData.tRchkColmCvl_Gen.ShearRebar[InData.nRchkPosiCvl_Gen_Sub].strSubRebarName;
	double dSbarNum	= InData.tRchkColmCvl_Gen.ShearRebar[InData.nRchkPosiCvl_Gen_Sub].dSubRebarNum[0];
	double dSbarDist= InData.tRchkColmCvl_Gen.ShearRebar[InData.nRchkPosiCvl_Gen_Sub].dSubRebarSpace;
	double dSbarDia	= m_pDoc->m_pMatlDB->Get_RebarDia(strSbarNa);
	double dSbarAs1	= m_pDoc->m_pMatlDB->Get_RebarArea(strSbarNa);

	//***// PMS.5568 Civil 일본 내진설계 대응 - 횡구속 철근 단면적 Ah 및 단면 2차 모멘트 Ih 고려 필요
	InData.dRhos[0] = (dSbarDist*dd[0]==0.0 ? 0.0 : 4*(dSbarNum/2.)*dSbarAs1/(dSbarDist*dd[0]));
	dSbarNum	= InData.tRchkColmCvl_Gen.ShearRebar[InData.nRchkPosiCvl_Gen_Sub].dSubRebarNum[1];	
	InData.dRhos[1] = (dSbarDist*dd[1]==0.0 ? 0.0 : 4*(dSbarNum/2.)*dSbarAs1/(dSbarDist*dd[1]));
	//***// PMS.5568 Civil 일본 내진설계 대응
	if(Reinforced_StlCover()) // 강판피복 보강시
	{
		double dt_plate[2] = {0.0, 0.0};
		double dsigsy2[2]  = {0.0, 0.0};
		InData.dRhos[0] += (dSbarDist*dd[0]==0.0 ? 0.0 : 4*(dt_plate[0]*dSbarDist*(dsigsy2[0]/dFys))/(dSbarDist*dd[0]));
		InData.dRhos[1] += (dSbarDist*dd[1]==0.0 ? 0.0 : 4*(dt_plate[1]*dSbarDist*(dsigsy2[1]/dFys))/(dSbarDist*dd[1]));
	}
	InData.dRhos[0] = min(InData.dRhos[0], 0.018);	
	InData.dRhos[1] = min(InData.dRhos[1], 0.018);

	// For Debugging.
	//++++++++++++++++++++++++++
	BOOL bPrint=TRUE;
	if(m_bTestMphi)
	{
		Prt_Debug(bPrint,_T("// RBAR ++++++++++++++++++++++++++++++++++++++++++++++++"));
		Prt_Debug(bPrint,_T("// Type, Number, Cent_y, Cent_z, Area."));
		for(int i=0; i<InData.arRbarUnit.GetSize(); i++)
		{
			_DB_MPHI_RBAR_UNIT RbarUnit = InData.arRbarUnit.GetAt(i);
			double dx	 = RbarUnit.dyz[0];
			double dy	 = RbarUnit.dyz[1];
			double dAs = RbarUnit.dAs;
			// For Debugging.
			Prt_Debug(bPrint,_T("R"),i+1,dx,dy,dAs);
		}
	}
	//++++++++++++++++++++++++++

	return TRUE;
}

BOOL CDgnToolMPhi_SeismicDesign::Get_UserData(const int iElem, const int iPosi, MPHI_USER_DATA& UserD)
{
	T_ELEM_D ElemD;
	ElemD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetElem(iElem, ElemD))	return FALSE;
	const int iMatl = ElemD.elmat;
	const int iSect = ElemD.elpro;
	/////////////////////////////////////////
	// Global Data.
	T_MPGB_D MpgbD;
	MpgbD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetMpgb(MpgbD))	return FALSE;
	T_RLCO_D RlcoD;
	if (!m_pDoc->m_pAttrCtrl->GetRlco(RlcoD)) m_pDoc->m_pAttrCtrl->GetDefaultRlco(RlcoD);

	// MPHI_USER_DATA
	//UserD.nEQType     = RlcoD.bEarthquakeTypeI;	       	// 1=Type1, 2=Type2
	UserD.nInitForce = RlcoD.nInitialForce;						// 0=Equilibrium Element Force Table, 1=Load Case.
	UserD.nLcaseType = MpgbD.nLoadCaseType;						// 0=Static Load Case, 1=Construction Stage Load Case.
	UserD.nYieldPoint = MpgbD.nYieldPoint + 1;					// 0=초항복시, 1=항복시
	UserD.nIterMphi = max(MpgbD.nMaxIteration, 1);			// 1 이상
	UserD.nSliceNum = max(MpgbD.nSectionDivision, 2);	// 2 이상
	UserD.dTolM0 = min(MpgbD.dTolerance, 0.2);			// 0.2 이하
	UserD.bIncludeTS = MpgbD.bIncludeTendon;           //Check if Tendon Secondary is included or not.
	UserD.nStressStrainType = MpgbD.nCode + 1;          // 0=Concrete Standard Spec. 1=Municipal V(14), 2=Municipal III(Sigma_ck), 3=Municipal III(0.85Sigma_ck) 4=Municipal V(24)

	/////////////////////////////////////////
	// Matl Data.
	T_MPHG_D MphgD;
	MphgD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetMphg(iMatl, MphgD))	return FALSE;
	UserD.nTypeFbt = MphgD.nConsiderSigbt;		// 0=Consider, 1=Do not Consider
	UserD.nEst23Type = MphgD.nLimitState;       // 0:A종(내진성능3), 1:B종(내진성능2)
	/////////////////////////////////////////
	// Sect Data.
	T_MPST_D MpstD;
	MpstD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetMpst(iSect, MpstD))	return FALSE;
	UserD.dAlpha[0] = MpstD.dAlpha;
	UserD.dAlpha[1] = MpstD.dAlphaZ;
	UserD.dBeta[0] = MpstD.dBeta;
	UserD.dBeta[1] = MpstD.dBetaZ;
	UserD.dd[0] = MpstD.dDim;
	UserD.dd[1] = MpstD.dDimz;
	UserD.da[0] = MpstD.da;
	UserD.da[1] = MpstD.daZ;
	UserD.nns[0] = MpstD.nnsY;
	UserD.nns[1] = MpstD.nnsZ;
	UserD.dd2[0] = MpstD.dd2Y;
	UserD.dd2[1] = MpstD.dd2Z;
	UserD.dl[0] = MpstD.dlY;  //유효좌굴길이 I
	UserD.dl[1] = MpstD.dlZ;  //유효좌굴길이 I
	// Seismic Design
	UserD.dAngle_tie[0] = MpstD.dAngY; // Ang. Y (전단철근과 연직축의 각도)
	UserD.dAngle_tie[1] = MpstD.dAngZ;

	T_SECT_D SectD;
	if (!m_pDoc->m_pAttrCtrl->GetSect(iSect, SectD)) return FALSE;

	if (IsSameAlphaZDir(SectD.SectBefore.Shape, MpgbD.nCode))
	{
		UserD.dAlpha[1] = MpstD.dAlpha;
	}
	if (IsSameBetaZDir(SectD.SectBefore.Shape, MpgbD.nCode))
	{
		UserD.dBeta[1] = MpstD.dBeta;
	}
	if (IsSameDZDir(SectD.SectBefore.Shape))
	{
		UserD.dd[1] = MpstD.dDim;
	}
	if (IsSameNsZDir(SectD.SectBefore.Shape))
	{
		UserD.nns[1] = MpstD.nnsY;
	}
	if (IsSameD2ZDir(SectD.SectBefore.Shape))
	{
		UserD.dd2[1] = MpstD.dd2Y;
	}

	return TRUE;
}

double CDgnToolMPhi_SeismicDesign::Get_dDeff(BOOL bAxisY, double dHeight, double dBw, MPHI_DATA_ELEM& InData)
{  
	double dDv = 0.0;
	double dDRebar_Tens = 0.0;
	const int nSectCon = InData.SectD.nSectCon;
	const auto&  SectBaseD = InData.SectD.SectBaseD;
	if(!Calc_dDeff_Rebar(bAxisY, InData, dDRebar_Tens)) ASSERT(0);
 
	switch (nSectCon)
	{
	case PMCV_SECT_P: // Pipe Shape
		{
			double dBv_Out = sqrt(dPI*pow(SectBaseD[0].Size[0],2)/4.);
			dDv = dHeight - (dHeight - dBv_Out)/2. -dDRebar_Tens;
		}
		break;
	case PMCV_SECT_SR: // Solid Round Shape
		{
			double dBv = Get_dBv(bAxisY, InData.SectD);
			dDv = dHeight - (dHeight - dBv)/2. - dDRebar_Tens; //원형 윗부분 공제한 뒤, 원형 인장측 철근 중심 길이를 공제!
		}
		break;
	default:
		{
			dDv = dHeight - dDRebar_Tens;
		}
		break;
	}
	
	if (dDv < dHeight/2.) // 예외처리
	{
		dDv = dHeight/2.;
		ASSERT(0);
	}
	return dDv;
}

BOOL CDgnToolMPhi_SeismicDesign::Calc_dDeff_Rebar(IN const BOOL bAxisY, MPHI_DATA_ELEM& InData, OUT double& dDeff)
{
	const int nMembType = InData.MembD.nMembType;
	const auto& tRchkColmCvl = InData.RbarD.tRchkColmCvl;
	const int nPosition = InData.nPosition;
	if (!nMembType==D_MBTP_COLUMN) return FALSE;
	

	int nPosi = (bAxisY) ? 0 : 1;
	double dTotal_Rebar_Area = 0.0;
	double dTatal_Rebar_Area_Dc = 0.0;
	for (int i=0; i<T_RCHK_COLM_LAYER; i++)
	{
		double dDc  = tRchkColmCvl.dDc[nPosition][i]; // 3:I,M,J, 5:Layer
		int iBarNum = tRchkColmCvl.iBarNum[nPosition][nPosi][i]; // 3:I,M,J 2: Pos 5:Layer
		if (iBarNum < PMCV_dZERO) break;
		CString strBarNa_Size1 = tRchkColmCvl.strBarNa1[nPosition][nPosi][i];
		CString strBarNa_Size2 = tRchkColmCvl.strBarNa2[nPosition][nPosi][i];
		double dAs_Rebar_Size1 = m_pDoc->m_pMatlDB->Get_RebarArea(strBarNa_Size1);
		double dAs_Rebar_Size2 = m_pDoc->m_pMatlDB->Get_RebarArea(strBarNa_Size2);
		if (dAs_Rebar_Size2<PMCV_dZERO) dAs_Rebar_Size2 = dAs_Rebar_Size1;
		double dAs_Rebar_Dia = (dAs_Rebar_Size1 + dAs_Rebar_Size2) / 2.;
		double dAs_Rebar_Area = dPI * dAs_Rebar_Dia / 4.;
		
		dTotal_Rebar_Area += (dAs_Rebar_Area*iBarNum);
		dTatal_Rebar_Area_Dc += (dAs_Rebar_Area*dDc*iBarNum); 
		
		dDeff = dTatal_Rebar_Area_Dc / dTotal_Rebar_Area;
	}

	//***// PMS.5568 Civil 일본 내진설계 대응
	if(Reinforced_RCCover())
	{
		// RC 피복 보강의 철근 고려 - 정착한 것만



	}

	return TRUE;
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//***// PMS.5568 Civil 일본 내진설계 대응
// 전단내력 산출 시 전단 스팬에 의한 전단내력에의 영향을 고려합니다.
// 전단 스팬에 의해 콘크리트가 부담하는 전단내력의 할증계수
// a＝교각 높이, 교각 높이는 교각 그룹별로 Cooping부의 강역부분을 제외한 순수 보요소의 높이
double CDgnToolMPhi_SeismicDesign::Calc_Cdc(const double& bMajor, const double& da, const double& dDeff)
{		
	//**// 조사조건-공통조건-기타설정-전단스팬비 고려 여부
	return 1.0; 

	const double arad[5]  = {0.5, 1.0, 1.5, 2.0, 2.5};
	const double arCdc[5] = {6.4, 4.0, 2.5, 1.6, 1.0};
	double dad = da / dDeff;
	if(dad < arad[0]) return arCdc[0];

	for(int i=1 ; i<5 ; i++)
	{
		if(dad < arad[i])
		{
			return arCdc[i-1] + (arCdc[i]-arCdc[i-1])/(arad[i]-arad[i-1])*(dad-arad[i-1]);
		}
	}
	return arCdc[4];
}

// 전단 스팬에 의해 띠철근이 부담하는 전단내력의 저감계수
double CDgnToolMPhi_SeismicDesign::Calc_Cds(const double& bMajor, const double& da, const double& dDeff)
{		
	//**// 조사조건-공통조건-기타설정-전단스팬비 고려 여부
	return 1.0; 

	return (da/dDeff) / 2.5;
}

