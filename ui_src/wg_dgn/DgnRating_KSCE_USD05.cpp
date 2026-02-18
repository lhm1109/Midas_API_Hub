// DgnBridgeLoadRating.cpp: implementation of the CDgnBridgeLoadRating class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnRating_KSCE_USD05.h"

#include "..\wg_db\wg_db_MathFunc.h"
#include "..\wg_base\wg_base_MsgDll.h"


CDgnRating_KSCE_USD05::CDgnRating_KSCE_USD05()
{
	//m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	m_dZero=1.0E-07;
	m_PSC_iITER = 100;  
	
	CString csDesignOutput = CProduct::GetTestEnvValue(_ULS(Design Output));
	m_bTestMode = csDesignOutput==_ULS(yes) ? TRUE : FALSE;
}

CDgnRating_KSCE_USD05::~CDgnRating_KSCE_USD05()
{
}

///////////////////////////////////////////
// For Connection to CRCDataCtrl.

void CDgnRating_KSCE_USD05::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
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

	Make_BridgeLoadRatingDgnData();
}

void CDgnRating_KSCE_USD05::SetDataCtrlPointer4Rating(int nCodePSC, CCRCDataCtrl* pDataCtrl)
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
	
	Make_BridgeLoadRatingDgnData();
	
	m_iCivilCode = nCodePSC;
	m_Print.SetDataCtrlPointer(m_pDataCtrl,m_pForcCtrl);
	m_Print.Set_PrintFlag(TRUE);
	
}

BOOL CDgnRating_KSCE_USD05::Get_RatingAlwStress(ElemPairK ElemK, BOOL bCompoSect, T_RKST_CASE& RkstCaseD)
{  
	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	if (bCompoSect==TRUE)
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem))
		{
			return FALSE;
		}
	}
	else
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem))
		{
			return FALSE;
		}
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

		T_RKST_BASE RkstBaseD = RkstCaseD.RkstBase[i];
		
		if(RkstBaseD.bCheck==FALSE)  continue;
		
		if(!Calc_AllowbleStressMethod(bPreTndn[i], MatlElem.MatdD, SpscSect, arRbarPosi, arTndnPosi, RkstBaseD)) continue;      

		RkstCaseD.RkstBase[i] = RkstBaseD; // 여기서는 I,J만 사용한다.
		
	}

	return bCalcOK;
}

BOOL CDgnRating_KSCE_USD05::Get_RatingFlexuralStrength(ElemPairK ElemK, BOOL bCompoSect, T_RAKR_D& RakrD, T_RKMC_CASE& RkmcCaseD)
{  
	BOOL bIter=FALSE; 
	if(RakrD.ifpsMethod==1) bIter = TRUE; //0=설계기준, 1=Iteration

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	if (bCompoSect==TRUE)
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem))
		{
			return FALSE;
		}
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem))
		{
			return FALSE;
		}
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}	
	
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

	// rp
	// PS강재의 종류(0=저릴랙세이션(0.28), 1=응력제거강재(0.4), 2=강봉(0.55))
	int iTndnKind = RakrD.iTndnKind;
	double drp = 0.0;    
	Get_TndnKindrp(iTndnKind, drp);
	
	for(int i=0; i<2; i++)
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
		if(!RakrD.bRefRebar) arRbarPosi.RemoveAll();
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[i].arTndnPosi);
		BOOL bRebarExist=TRUE;
		BOOL bTndnExist=TRUE;
		if(arRbarPosi.GetSize()==0 && arTndnPosi.GetSize()==0)
		{
			bRebarExist=FALSE;
			bTndnExist=FALSE;
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

		if(!bRebarExist && !bTndnExist) RkmcCaseD.RkmcBase[i].bCheck=FALSE;
		// if(RkmcCaseD.RkmcBase[i].bCheck==FALSE) continue;  // 체크여부와 관계없이 RF계산 위해서.
		T_BDCY_KSCE_BASE BdcyBase;
		BdcyBase.Initialize();
	
		// 강도계산.
		BdcyBase.dMuy  =  RkmcCaseD.RkmcBase[i].dM_L; //RkmcCaseD.RkmcBase[i].dMdl;  
		BOOL bPositive = (BdcyBase.dMuy>=0 ? TRUE : FALSE);    
		
		if (SectElem.SectPosi[i].bCompoSect==TRUE) bIter = TRUE; // Composite이면 Iter로만. 
		Get_FlexureStrength(TRUE, bPosiI, bIter, bPositive, dPhib, MatlElem, SectElem.SectPosi[i], SpscSect, arOuter, arpInner, SpscgSect, 
														drp, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcyBase/* BdcyBaseDetail*/);

		// 내하력 평가
		const double dElast = MatlElem.MatdD.Data1.Analysis.Elast;
		m_pDataCtrl->Calc_FlexuralStrengthMethod(RakrD.nSurveyMeth, BdcyBase.dpMny, dElast, RkmcCaseD.RkmcBase[i]);

		// 등급평가용 강도 계산.
		BdcyBase.dMuy  =  RkmcCaseD.RkmcBase[i].dMd_str + RkmcCaseD.RkmcBase[i].dM_L_str; 
		bPositive = (BdcyBase.dMuy>=0 ? TRUE : FALSE);    
		Get_FlexureStrength(TRUE, bPosiI, bIter, bPositive, dPhib, MatlElem, SectElem.SectPosi[i], SpscSect, arOuter, arpInner, SpscgSect, 
		                       	drp, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcyBase/* BdcyBaseDetail*/);
		RkmcCaseD.RkmcBase[i].dPhiMn_Rank = BdcyBase.dpMny;


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

BOOL CDgnRating_KSCE_USD05::Get_RatingFlexuralStrength_Print(CMSExcel* pXL, ElemPairK ElemK, int nIJ,  BOOL bCompoSect, CString strTilteName, T_RAKR_D& RakrD, T_RKMC_CASE& RkmcCaseD)
{  
	BOOL bIter=FALSE; 
	if(RakrD.ifpsMethod==1) bIter = TRUE; //0=설계기준, 1=Iteration

	BOOL bCalcOK=TRUE;
	_PSC_MATL_ELEM MatlElem;
	if(!Get_PscMatlElem(ElemK, MatlElem))
	{
		return FALSE;
	}
	_PSC_SECT_ELEM SectElem;
	_PSC_SECG_ELEM SecgElem;
	if (bCompoSect==TRUE)
	{
		if(!Get_CompoPscSectElem(TRUE, ElemK, SectElem))
		{
			return FALSE;
		}
		if(!Get_CompoPscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}
	else
	{
		if(!Get_PscSectElem(TRUE, ElemK, SectElem))
		{
			return FALSE;
		}
		if(!Get_PscSecgElem(ElemK, SecgElem)) bCalcOK=FALSE;
	}	
	
	_PSC_RBAR_ELEM RbarElem;
	if(!Get_PscRbarElem(TRUE, ElemK, RbarElem)) bCalcOK=FALSE;
	_PSC_TNDN_ELEM TndnElem;
	if(!Get_PscTndnElem(TRUE, ElemK, TndnElem)) bCalcOK=FALSE;

	// rp
	// PS강재의 종류(0=저릴랙세이션(0.28), 1=응력제거강재(0.4), 2=강봉(0.55))
	int iTndnKind = RakrD.iTndnKind;
	double drp = 0.0;    
	Get_TndnKindrp(iTndnKind, drp);
	
	//for(int i=0; i<2; i++)
	{
		//int i = nIJ;
		BOOL bPosiI = (nIJ==0 ? TRUE : FALSE);    
			
		// Section g
		T_SPSC_SECT SpscgSect = SecgElem.SectPosi[nIJ];
		// Section
		T_SPSC_SECT SpscSect = SectElem.SectPosi[nIJ].SpscSect;
		CArray<T_SPSC_COOR,T_SPSC_COOR> arOuter;
		arOuter.Copy(SectElem.SectPosi[nIJ].arOuter);
		CArray<T_SPSC_COOR_AR*,T_SPSC_COOR_AR*> arpInner;  
		int iInnerCellNum = SectElem.SectPosi[nIJ].iInnerCellNum;
		for(int m=0; m<iInnerCellNum; m++)
		{
			CArray<T_SPSC_COOR,T_SPSC_COOR> arInner;
			arInner.Copy(SectElem.SectPosi[nIJ].arInner[m]);
			arpInner.Add(&arInner);
		}
			
		// RBar
		CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&> arRbarPosi;
		arRbarPosi.Copy(RbarElem.RbarPosi[nIJ].arRbarPosi);
		if(!RakrD.bRefRebar) arRbarPosi.RemoveAll();
		// Tendon
		CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&> arTndnPosi;
		arTndnPosi.Copy(TndnElem.TndnPosi[nIJ].arTndnPosi);
		BOOL bRebarExist=TRUE;
		BOOL bTndnExist=TRUE;
		if(arRbarPosi.GetSize()==0 && arTndnPosi.GetSize()==0)
		{
			bRebarExist=FALSE;
			bTndnExist=FALSE;
		}

		// bw
		double dShrThk[3];
		dShrThk[0] = SectElem.SectPosi[nIJ].dpShrThk[0];
		dShrThk[1] = SectElem.SectPosi[nIJ].dpShrThk[1];
		dShrThk[2] = SectElem.SectPosi[nIJ].dpShrThk[2];
		double dbw = min(min(dShrThk[0],dShrThk[1]),dShrThk[2]);
		double dFlgThk[2]={SectElem.SectPosi[nIJ].dpFlgThk[0], SectElem.SectPosi[nIJ].dpFlgThk[1]}; 
		double dpMomPos[2][2];
		dpMomPos[0][0] = SectElem.SectPosi[nIJ].dpMomPos[2][0];
		dpMomPos[0][1] = SectElem.SectPosi[nIJ].dpMomPos[2][1];
		dpMomPos[1][0] = SectElem.SectPosi[nIJ].dpMomPos[3][0];
		dpMomPos[1][1] = SectElem.SectPosi[nIJ].dpMomPos[3][1];    
		
		//Phi
		double dPhib, dPhiv;
		BOOL bPhi = m_pDataCtrl->Get_KscePhi(dPhib, dPhiv);

		T_BDCY_KSCE_BASE BdcyBase;

		for(int nPosNeg=0; nPosNeg<2; ++nPosNeg) // 정/부
		{
			if(!bRebarExist && !bTndnExist) RkmcCaseD.RkmcBase[nPosNeg].bCheck=FALSE;
			if(RkmcCaseD.RkmcBase[nPosNeg].bCheck==FALSE) return FALSE;

			BdcyBase.Initialize();
			BdcyBase.dMuy =   RkmcCaseD.RkmcBase[nPosNeg].dMdl;                        // RkmcCaseD.RkmcBase[nPosNeg].dM_L;
			BOOL bPositive =  (RkmcCaseD.RkmcBase[nPosNeg].dMdl>=0 ? TRUE : FALSE);    //(RkmcCaseD.RkmcBase[nPosNeg].dM_L>=0 ? TRUE : FALSE);      
			
			// 검토 위치 출력
			if(nPosNeg==0)
			{
				 m_Print_ksce10.Set_CMSExcel_KSCE10(m_pXL, m_strBaseBook, m_iCivilCode);
				 m_Print_ksce10.Print_Rating_Title(strTilteName, m_pXL);  // Rating 출력일 때
			}
			m_Print_ksce10.Print_ElementIJ_KSCE10(ElemK.first, bPosiI, bPositive, pXL);      

			// 강도계산.
			if (SectElem.SectPosi[nIJ].bCompoSect==TRUE) bIter = TRUE; // Composite이면 Iter로만. 
			Get_FlexureStrength(TRUE, bPosiI, bIter, bPositive, dPhib, MatlElem, SectElem.SectPosi[nIJ], SpscSect, arOuter, arpInner, SpscgSect, 
				drp, dbw, dFlgThk, dpMomPos, arRbarPosi, arTndnPosi, BdcyBase, pXL, FALSE, TRUE);
			//
			const double dElast = MatlElem.MatdD.Data1.Analysis.Elast;
			m_pDataCtrl->Calc_FlexuralStrengthMethod(RakrD.nSurveyMeth, BdcyBase.dpMny, dElast, RkmcCaseD.RkmcBase[nPosNeg]);

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

BOOL CDgnRating_KSCE_USD05::Calc_AllowbleStressMethod(BOOL bPre, T_MATD_D& MatdD, T_SPSC_SECT& SpscSect, CArray<_DGN_RBAR_CRC,_DGN_RBAR_CRC&>& arRbar, CArray<_DGN_TNDN_CRC,_DGN_TNDN_CRC&>& arTndn, 
																											T_RKST_BASE& RkstBaseD)
{
	const int POSI=4;

	// 시공후 검토 이므로 dfe만 사용한다.
	double dfi[3] = {0., 0., 0.,}; 
	double dfe[3] = {0., 0., 0.,}; // 0,1=인장(Top,Bottom), 2=압축
	double dfci = 0.0; // 사용안함. 
	double dAlwCompStr=0.0, dAlwTensStr=0.0;
	if(!Get_AllowbleStressOfConc(bPre, dfci, MatdD, SpscSect, arRbar, arTndn, dfi, dfe)) return FALSE;

	//double dExtraRatio  = m_pForcCtrl->Get_ExtraRatio(RkstBaseD.nRCaseK); //!/ nRCaseK -> Dgn LcomK로 수정해야함. 

	double dfa_p[POSI]; // 인장 또는 압축 
	double dfd_p[POSI]; // 1,2,3,4 고정하중에 대한 응력 
	double dfdm_p[POSI];
	double dfl_p[POSI];
	double dRF_p[POSI]; // 내하율
	double dP0_p[POSI]; // 기본내하력
	double dP0a_p[POSI]; // 공용내하력
	//
	double dfd_top=0.0;
	double dfd_bot=0.0;
	double dfl_top=0.0;
	double dfl_bot=0.0;
	double di_calc=RkstBaseD.dImpFactor;
	double di_real=RkstBaseD.dRealImpFactor;
	double dAddResponce = RkstBaseD.dAddResponce;
	double dKs = 0.0;
	double dKr = RkstBaseD.dKr;
	double dKt = RkstBaseD.dKt;  
	double dP_L = RkstBaseD.dDgnLoad; // 설계활하중(예,DB-18)
	double d1_plus_i_calc = 1.0+di_calc;
	double di_rat = 0.0;
	double dDisp_rat = 0.0;
	double dDispZ_real = RkstBaseD.dRealDispZ;
	double dDispZ_calc = RkstBaseD.dCalc_defl;

	//(2008.04.03) Add by Unsang :: Add Variable for ExcelReport // ModifyFct = Ks x Kt x Kr
	double dKsKrKt = 0.0;

	int i;

	for(i=0; i<POSI; i++)
	{
		dfa_p[i] = 0.0;
		dfd_p[i] = dfdm_p[i] = RkstBaseD.dfd_p[i];
		dfl_p[i] = RkstBaseD.dfl_p[i];
		dRF_p[i] = 0.0;
		dP0_p[i] = 0.0;
	}

	di_rat = d1_plus_i_calc/(1.0+di_real);
	dDisp_rat = dDispZ_real==0.0 ? 0.0 : fabs(dDispZ_calc/dDispZ_real);
	
	// 실측 보정계수
	dKs = dDisp_rat*di_rat* dAddResponce;

	//(2008.04.03) Add by Unsang :: Add Variable
	dKsKrKt = dKs * dKr * dKt;

	for(i=0; i<POSI; i++)
	{
		// Live Load를 기준으로 허용응력 결정.
		double dfta = i==0 || i==1 ? dfe[0] : dfe[1]; // dfe[0]=Top Tension, dfe[1] = Bottom Tension
		dfa_p[i] = dfl_p[i]<0.0 ? dfe[2] : dfta;
		//
		if(dfl_p[i]>=0.0)
		{
			if(dfd_p[i]<0.0) dfdm_p[i] = 0.0;
		}
		else
		{
			if(dfd_p[i]>0.0) dfdm_p[i] = 0.0;
		}    
		// 내하율(R.F.)의 계산.
		dRF_p[i] = dfl_p[i]*d1_plus_i_calc==0.0 ? 0.0 : (dfa_p[i]-dfdm_p[i])/(dfl_p[i]*d1_plus_i_calc);
		// 기본내하력 
		dP0_p[i] = dRF_p[i]*dP_L;
		// 공용내하력
		//dP0a_p[i] = dP0_p[i]*dKs*dKr*dKt;
		dP0a_p[i] = dP0_p[i]*dKsKrKt;
	}

	//
	for(i=0; i<3; i++)
		RkstBaseD.dfa_s[i] = dfe[i]; // 0,1=T,B인장 2=압축

	RkstBaseD.d1_plus_i_calc = d1_plus_i_calc;
	RkstBaseD.dDisp_rat = dDisp_rat;
	RkstBaseD.di_rat = di_rat;
	RkstBaseD.dKs = dKs;
	RkstBaseD.dKsKrKt = dKsKrKt;

	for(i=0; i<4; i++)
	{    
		RkstBaseD.dfa_p[i]  = dfa_p[i];
		RkstBaseD.dfd_p[i]  = dfd_p[i];
		RkstBaseD.dfdm_p[i]  = dfdm_p[i];
		RkstBaseD.dRF_p[i]  = dRF_p[i];
		RkstBaseD.dP0_p[i]  = dP0_p[i];
		RkstBaseD.dP0a_p[i] = dP0a_p[i];
		RkstBaseD.bOK_p[i] = dP0a_p[i]>dP_L ? TRUE : FALSE;
	} 

	/*
	// 상연(1,2), 하연(3,4) 각각 불리한 고용내하력 결정.
	UINT iPartT=0, iPartB=0;
	if(dP0a_p[0]>dP0a_p[1])
	{
		iPartT = 1
	}
	else
	{
		iPartT = 0;
	}
	//
	if(dP0a_p[2]>dP0a_p[3])
	{
		iPartB = 3
	}
	else
	{
		iPartB = 2;
	}
	
	for(i=0; i<2; i++)
	{
		int iPart  = i==0 ? iPartT : iPartB;
		RkstBaseD.dfd_TB[i]  = dfd_p[iPart];
		RkstBaseD.dfl_TB[i]  = dfl_p[iPart];
		RkstBaseD.dfa_TB[i]  = dfa_p[iPart];
		RkstBaseD.dRF_TB[i]  = dRF_p[iPart];
		RkstBaseD.dP0_TB[i]  = dP0_p[iPart];
		RkstBaseD.dP0a_TB[i] = dP0a_p[iPart];
	} 
	*/
			 
	return TRUE;
}

//BOOL CDgnRating_KSCE_USD05::Calc_FlexuralStrengthMethod(int nSurveyMeth, double dpMn, _PSC_MATL_ELEM& MatlD, T_RKMC_BASE& RkmcBaseD)
//{
//
//  double dRF = 0.0; // 내하율
//  double dP0 = 0.0; // 기본내하력
//  double dP0a = 0.0; // 공용내하력
//  //  
//  double dM_L= RkmcBaseD.dM_L;
//  double dMd = RkmcBaseD.dMd;
//  double dMd_m = dMd;
//  double di_calc=RkmcBaseD.dImpFactor;
//  double di_real=RkmcBaseD.dRealImpFactor;
//  double dAddResponce = RkmcBaseD.dAddResponce;
//  double dKs = 0.0;
//  double dKm = RkmcBaseD.dKm;  
//  double dP_L = RkmcBaseD.dDgnLoad; // 설계활하중(예,DB-18)
//  double d1_plus_i_calc = 1.0+di_calc;
//  double di_rat = 0.0;
//  double dDisp_rat = 0.0;
//  double dDispZ_real = RkmcBaseD.dRealDispZ;
//  double dDispZ_calc = RkmcBaseD.dCalc_defl;  
//  RkmcBaseD.dPhiMn = dpMn;
//
//	double dElast = MatlD.MatdD.Data1.Analysis.Elast;
//	double dCalcEwip = RkmcBaseD.dFl / dElast;
//	double dEstiEwip = RkmcBaseD.dEstiEwip;
//	double dEwip_rat = dEstiEwip==0.0 ? 0.0 : fabs(dCalcEwip/dEstiEwip);
//  RkmcBaseD.dCalcEwip = dCalcEwip;
//	RkmcBaseD.dEwip_rat = dEwip_rat;
//
//  //(2008.04.03) Add by Unsang :: Add Variable
//  double dKsPerKm = 0.0;
//
//
//  if(dM_L>=0.0) // Positive moment
//  {
//    // M_L이 Positive 일 때 Md가 Negative면 0
//    if(dMd<0.0) dMd_m = 0.0; 
//  }
//  else
//  {
//    if(dMd>0.0) dMd_m = 0.0;
//    dpMn = -1.0*dpMn;
//  }
//
//  di_rat = d1_plus_i_calc/(1.0+di_real);
//
//  dDisp_rat = dDispZ_real==0.0 ? 0.0 : fabs(dDispZ_calc/dDispZ_real);
//
// 
//  // 실측 보정계수 nSurveyMeth = 0:처짐 1:변형률
//	if(nSurveyMeth == 0) dKs = dDisp_rat*di_rat *dAddResponce;
//	else                 dKs = dEwip_rat*di_rat *dAddResponce;
//  //(2008.04.03) Add by Unsang :: Add Variable
//  dKsPerKm = dKm < cDGN_Zero ? 0.0 : dKs/dKm;
//
//  // 내하율(R.F.)의 계산.
//  dRF = dM_L*d1_plus_i_calc==0.0 ? 0.0 : (fabs(dpMn) - fabs(dMd_m))/(fabs(dM_L)*d1_plus_i_calc);
//  // 기본내하력 
//  dP0 = dRF*dP_L;
//  // 공용내하력
//  dP0a = dP0*dKsPerKm;  // dP0*dKs/dKm;
//
//  RkmcBaseD.dMd_m = dMd_m;
//  RkmcBaseD.d1_plus_i_calc = d1_plus_i_calc;
//  RkmcBaseD.dDisp_rat = dDisp_rat;
//  RkmcBaseD.di_rat = di_rat;
//  RkmcBaseD.dKs = dKs;
//  RkmcBaseD.dRF = dRF;
//  RkmcBaseD.dP0 = dP0;
//  RkmcBaseD.dP0a = dP0a;
//  RkmcBaseD.bOK = dP0a<dP_L ? FALSE : TRUE;
//
//  //(2008.04.03) Add by Unsang ::
//  RkmcBaseD.dKsPerKm = dKsPerKm;
//
//  return TRUE;
//}
