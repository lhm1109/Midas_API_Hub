#include "stdafx.h"
#include "BrdgSeisEvalKISTEC2015.h"

#include "..\wg_db\BrdgSeisEvalResult.h"
#include "..\wg_dgn\DgnProgressDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_MathFunc.h"

CBrdgSeisEvalKISTEC2015::CBrdgSeisEvalKISTEC2015(void)
{
    m_CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    m_CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
}

CBrdgSeisEvalKISTEC2015::~CBrdgSeisEvalKISTEC2015(void)
{
}

void CBrdgSeisEvalKISTEC2015::CalcIeff(const T_MPCC_D& MpccD, double dIeffRat[2], T_BRDGEVAL_RESULT Result[2], BOOL bRecal)
{
    // current unit.
    dIeffRat[0] = dIeffRat[1] = 1.0;

    Result[0].initialize();  // LONGI, TRANS
    Result[1].initialize();

    // always calculate in bottom position.
    UINT iCol = MpccD.nEvgpColm;
    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if(!pResult) { ASSERT(0); }
    SEIS_RES_KEY MphiLK = pResult->GetKey(MpccD.EvgpK, iCol, LONGI, static_cast<EN_POSITION>(MpccD.nEvgpPos));
    SEIS_RES_KEY MphiTK = pResult->GetKey(MpccD.EvgpK, iCol, TRANS, static_cast<EN_POSITION>(MpccD.nEvgpPos));

    // Get M-Phi status result; m-phi 항상 재계산
    if(bRecal)
    {
        T_MPCC_D Code_MpccD = const_cast<T_MPCC_D&>(MpccD);
        CalcMomentCurvature(Code_MpccD); 
    }
    T_BRDGEVAL_MPHI_STATE mphiL, mphiT;
    if(!pResult->GetMPhiState(MphiLK, mphiL)) mphiL.initialize();
    if(!pResult->GetMPhiState(MphiTK, mphiT)) mphiT.initialize();

    double dEc1 = Get_Ec(MpccD.MatlConcK1);
    double dEc2 = Get_Ec(MpccD.MatlConcK1Trans);

    T_SECT_D SectD;
    if(!m_pDoc->m_pAttrCtrl->GetSectDesign(MpccD.SectK, SectD)) { ASSERT(0); return; }

    BOOL bOldCurveType = FALSE;
    int nCalcOption = 1;

    if(SectD.nStype == D_SECT_TYPE_TAPERED)
    {
        if(MpccD.nPosition == 1)
        {
            // SectSrc를 dPositionJ / dElemLen 위치에서 보간한 정보를 SectOut의 I단에 넣음 
            // (주의!!) SectOut의 I단 정보만 의미 있음!! J단정보는 아무 의미 없음!!
            // (주의!!) Size만 유효 하고 그외 값들은 유효 하지 않음
            T_TSGR_D DataTsgr;
            DataTsgr.Initialize();
            DataTsgr.nVariation_y = 0; // linear
            DataTsgr.nVariation_z = 0; // linear

            T_SECT_D SectTmpD;
            SectTmpD = SectD;
            double dTmpElemLen = 10.0;

            if(!m_pDoc->m_pSectDB->CalcTaperedSection(SectTmpD, DataTsgr, dTmpElemLen, dTmpElemLen/2.0, dTmpElemLen, nCalcOption, SectD, bOldCurveType, 0))
            {
                ASSERT(0);
            }
            m_pDoc->m_pSectDB->CalcSectData(SectD);    
        }
        else if(MpccD.nPosition == 2) SectD.SectBefore.SectI = SectD.SectBefore.SectJ;
    }


    double dI     = 0.0;
    double dEIeff = 0.0;
    double dIeff  = 0.0;
    // Logitudinal
    dI = SectD.SectBefore.SectI.Stiffness.Ryy;
    dEIeff = mphiL.dCurvature[4]>0.0 ? mphiL.dMoment[4] / mphiL.dCurvature[4] : 0.0;
    dIeff  = dEc1>0.0 ? dEIeff / dEc1 : 0.0;
    dIeffRat[0] = dI>0.0 && dIeff>0.0 ? dIeff / dI : 1.0;

    // Transverse
    dI = SectD.SectBefore.SectI.Stiffness.Rzz;
    dEIeff = mphiT.dCurvature[4]>0.0 ? mphiT.dMoment[4] / mphiT.dCurvature[4] : 0.0;
    dIeff  = dEc2>0.0 ? dEIeff / dEc2 : 0.0;
    dIeffRat[1] = dI>0.0 && dIeff>0.0 ? dIeff / dI : 1.0;


	// 개선
	T_BSED_D BsedD; BsedD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD);
	//if (BsedD.nDesignCode == KISTEC_2015 || BsedD.nDesignCode == KISTEC_2019)

	BOOL bBrdgSeisEvalResult = pResult->ExistResult();
    if (bBrdgSeisEvalResult)
    {
		//ResultD[0].EvalD.EvalD.dMe_pd
		//ResultD[0].MPhiD.StateD.dMoment[4]
        EN_POSITION enPos = MpccD.nEvgpPos == 0 ? TOP : BOTTOM;

		if (!pResult->ReadSeisEvalResultMPhi(MpccD.EvgpK, iCol, LONGI, enPos, Result[0].MPhiD)) Result[0].MPhiD.initialize();
		if (!pResult->ReadSeisEvalResultMPhi(MpccD.EvgpK, iCol, TRANS, enPos, Result[1].MPhiD)) Result[1].MPhiD.initialize();

		if (!pResult->ReadSeisEvalResultEval(MpccD.EvgpK, iCol, LONGI, enPos, 0, TRUE, Result[0].EvalD)) Result[0].EvalD.initialize();
		if (!pResult->ReadSeisEvalResultEval(MpccD.EvgpK, iCol, TRANS, enPos, 0, TRUE, Result[1].EvalD)) Result[1].EvalD.initialize();


        //////////////////////////////////////////////////////////////////////////
        T_BRDGEVAL_RESULT rData[2][2];
		int nCheck = 0;
		for (int i = 0; i < 2; ++i)
		{
			EN_POSITION enPos = i == 0 ? BOTTOM : TOP;
			for (int j = 0; j < 2; ++j)
			{
				EN_LOCAL_DIR enDir = j == 0 ? LONGI : TRANS;
				if (!pResult->ReadSeisEvalResultMPhi(MpccD.EvgpK, iCol, enDir, enPos,          rData[i][j].MPhiD)) rData[i][j].MPhiD.initialize();
				if (!pResult->ReadSeisEvalResultEval(MpccD.EvgpK, iCol, enDir, enPos, 0, TRUE, rData[i][j].EvalD)) rData[i][j].EvalD.initialize();

				if (!rData[i][j].EvalD.bCheck) nCheck++;
			}
		}

    }
}

void CBrdgSeisEvalKISTEC2015::CalcHe(const T_MPCC_K& MpccTopK, const T_MPCC_K& MpccBotK, const EN_LOCAL_DIR& enDir, const EN_BEHAVIOR_TYPE& enBehavior, const int& nCalcType, const double& dHc, double dHe[2])
{
    // current unit.

    dHe[0] = dHe[1] = dHc;
    if(enBehavior == RAHMEN)
    {
        if(nCalcType == 0) // Hc/2
        {
            dHe[0] /= 2.0;
            dHe[1] /= 2.0;
        }
        else // by curvature
        {
            T_MPCC_D MpccTopD, MpccBotD;
            if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccTopK, MpccTopD)) { ASSERT(0); return; }
            if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccBotK, MpccBotD)) { ASSERT(0); return; }

            CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if(!pResult) { ASSERT(0); return; }
            SEIS_RES_KEY BotK = pResult->GetKey(MpccBotD.EvgpK, MpccBotD.nEvgpColm, enDir, BOTTOM);
            SEIS_RES_KEY TopK = pResult->GetKey(MpccTopD.EvgpK, MpccTopD.nEvgpColm, enDir, TOP);

            T_BRDGEVAL_MPHI_STATE mphi_bot, mphi_top;
            // Get M-Phi status result; m-phi 항상 재계산
            CalcMomentCurvature(const_cast<T_MPCC_D&>(MpccBotD)); 
            if(!pResult->GetMPhiState(BotK, mphi_bot)) { ASSERT(0); return; }
            CalcMomentCurvature(const_cast<T_MPCC_D&>(MpccTopD)); 
            if(!pResult->GetMPhiState(TopK, mphi_top)) { ASSERT(0); return; }

            dHe[0] = dHc * mphi_bot.dCurvature[4] / (mphi_bot.dCurvature[4] + mphi_top.dCurvature[4]);
            dHe[1] = dHc * mphi_top.dCurvature[4] / (mphi_bot.dCurvature[4] + mphi_top.dCurvature[4]);
        }
    }
}

void CBrdgSeisEvalKISTEC2015::Calc_BendingCapacity(const SEIS_RES_KEY& SeisResK, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_ULT_DISP_DUCT& UltDsipD, T_BRDGEVAL_BEND_CAPA& BendCapaD)
{
    // Unit; kN, m
    // 4.4.1.2 교각의 휨성능 곡선
    double dfy  = ParamD.MatlD.dfy;
    double ddbl = ParamD.BarD.ddbl;
    double dHe  = ParamD.OptD.dHe;

    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if(!pResult) { ASSERT(0); return; }
    T_BRDGEVAL_MPHI_STATE MPhiStateD;
    if(!pResult->GetMPhiState(SeisResK, MPhiStateD)) return;

    double dMy = MPhiStateD.dMoment[4];
    double dMu = MPhiStateD.dMoment[5];
    double dphiy = MPhiStateD.dCurvature[4];
    double dphiu = MPhiStateD.dCurvature[5];

    double dFy = dHe>0.0 ? dMy / dHe : 0.0; // 식(4.4.1a)
    double dFu = dHe>0.0 ? dMu / dHe : 0.0; // 식(4.4.1b)

    double dLp_cal = 0.08*dHe + 0.022*dfy*ddbl;   // 식(4.4.2d)
    double dLp_lim = 0.044*dfy*ddbl;              // 식(4.4.2d)
    double dLp     = max(dLp_cal, dLp_lim);       // 식(4.4.2d)
    double dThetap = (dphiu - dphiy) * dLp;       // 식(4.4.2c)
    double dDeltay = dphiy * pow(dHe, 2.0) / 3.0; // 식(4.4.2a)
    double dDeltap = ((dMu / dMy) - 1.0)*dDeltay + dThetap*(dHe - dLp/2.0); // 식(4.4.2b)
    double dDeltau = dDeltay + dDeltap;           // 식(4.4.2e)

    SEIS_RES_K Key;
    Key.KeyMap = SeisResK;
    double dmuDeltac = dDeltau / dDeltay;
    double dmuDeltamax = Get_MaxSupplyDispDuct(ParamD.OptD.nSplice, ParamD.SetcD.strShape, Key.Key.enDir, ParamD.OptD.nBHmethod, ParamD.SetcD.dArea, ParamD.SetcD.dB, ParamD.SetcD.dH, ParamD.OptD.dlsp);
	if (dmuDeltamax == DBL_MAX)
	{
		BendCapaD.dDeltac = 0.0;
	}
    else if(dmuDeltac > dmuDeltamax)
    {
        BendCapaD.dDeltac = dmuDeltamax * dDeltay;
        BendCapaD.dFn     = dFy + (dFu-dFy)/(dDeltau-dDeltay)*(BendCapaD.dDeltac-dDeltay);
    }
	else if(dmuDeltac <= dmuDeltamax)
	{
		BendCapaD.dDeltac = dmuDeltamax * dDeltay;
		BendCapaD.dFn     = dFu;
	}

    BendCapaD.dFy = dFy;
    BendCapaD.dFu = dFu;
    BendCapaD.dDeltay = dDeltay;
    BendCapaD.dDeltau = dDeltau;

    UltDsipD.dDeltay = dDeltay;
    UltDsipD.dDeltap = dDeltap;
    UltDsipD.dDeltau = dDeltau;
    UltDsipD.dDeltac = BendCapaD.dDeltac;
    UltDsipD.dLp     = dLp;
    UltDsipD.dLp_cal = dLp_cal;
    UltDsipD.dLp_lim = dLp_lim;
    UltDsipD.dThetap = dThetap;
    UltDsipD.dmu_Deltac   = dmuDeltac;
    UltDsipD.dmu_DeltaMax = dmuDeltamax;
    UltDsipD.dFn     = BendCapaD.dFn;
}

void CBrdgSeisEvalKISTEC2015::Calc_ShearCapacity(const EN_LOCAL_DIR& enDir, T_BRDGEVAL_CALC_PARAM& ParamD, const T_BRDGEVAL_ULT_DISP_DUCT& UltDsipD, T_BRDGEVAL_SHR_CAPA& ShearCapaD)
{
    auto lambda_CalcConcSRF = [this] (const double& dmu_delta)
    {
        double dk = 0.3 - 0.1*(dmu_delta - 2.0);
        dk = min(dk, 0.3);
        dk = max(dk, 0.0);
        return dk;
    };

    CString strShape = ParamD.SetcD.strShape;
    EN_PIER_TYPE enPierType = Get_PIER_TYPE(enDir, ParamD.OptD.nSplice, strShape, ParamD.OptD.nBHmethod, ParamD.SetcD.dArea, ParamD.SetcD.dB, ParamD.SetcD.dH);
    double dfck  = ParamD.MatlD.dfck;
    double dfyh  = ParamD.MatlD.dfyh;
    double db    = enDir==LONGI ? ParamD.SetcD.dB : ParamD.SetcD.dH;
    double dh    = enDir==LONGI ? ParamD.SetcD.dH : ParamD.SetcD.dB;
    double dArea = ParamD.SetcD.dArea;
	double ddbl  = ParamD.BarD.ddbl;
    double ddbs  = ParamD.BarD.ddbs;
    double ddc   = ParamD.BarD.ddc;
    double dAsp  = ParamD.BarD.dAsp;
    double dAv   = ParamD.BarD.dAv;
    double ds    = ParamD.BarD.ds;
	int nlegs    = ParamD.BarD.nlegs;
    double drho_sh = ParamD.BarD.drhosh;
    double dts   = dh;
    double dLs   = ParamD.OptD.dHe;

    double dVn1=0.0, dVn2=0.0;
    double dVc=0.0, dVc2=0.0, dVc5=0.0, dVs=0.0, dVp=0.0;
    double dD1 = 0.0, dD2 = 0.0;

    double dPtop     = ParamD.ForceD.dPtop; // (+)압축, (-)인장; 교각상부 축력
    dPtop     = max(dPtop, 0.0);
    
	double dAvCal = 0.0;
    if( enPierType == WALL )
    {
        Get_WallVn(ParamD, ShearCapaD);
    }
    else
    {
        double dAe = ParamD.OptD.dAeRat*dArea;
        double dk  = lambda_CalcConcSRF(UltDsipD.dmu_Deltac);
        dVc   = dk * sqrt(dfck) * dAe;
        dk    = lambda_CalcConcSRF(2.0);
        dVc2  = dk * sqrt(dfck) * dAe;
        dk    = lambda_CalcConcSRF(5.0);
        dVc5  = dk * sqrt(dfck) * dAe; // 0.0

        if(ds > 0.0) // 전단배근이 있는 경우
        {
            double dpi = 4.0*atan(1.0); // 3.141592.....
            if(strShape == D_SECT_SHAPE_REG_P || strShape == D_SECT_SHAPE_REG_SR) // section type
            {
                //dD1 = db - ddc - ddbs;
				dD1 = db - 2.0*ddc + ddbl + ddbs;
                dVs = dpi/2.0 * dAsp * dfyh * dD1 / ds;
            }
            else if(strShape == D_SECT_SHAPE_REG_TRK || strShape == D_SECT_SHAPE_REG_STRK)
            {
                //dD1 = ParamD.SetcD.dH - ddc - ddbs; // DL
				dD1 = ParamD.SetcD.dH - 2.0*ddc + ddbl + ddbs;
                dD2 = ParamD.SetcD.dB - ParamD.SetcD.dH; // DT

                dVs = dpi/2.0*dAsp*dfyh*dD1/ds;
				if (enDir == LONGI) 
				{
					int nLeg = nlegs - 2;
					if (nLeg < 0) nLeg = 0;

					dAvCal = dAsp * nLeg;
					dVs += dAvCal * dfyh*dD2 / ds;
				}
                else if(enDir == TRANS)
                { 
                    dVs += dAv*dfyh*dD2/ds;
					dAvCal = dAv;
                }
            }
            else if(strShape == D_SECT_SHAPE_REG_HTRK)
            {
                //dD1 = ParamD.SetcD.dH - ddc - ddbs;
				dD1 = ParamD.SetcD.dH - 2.0*ddc + ddbl + ddbs;
                dD2 = ParamD.SetcD.dB - ParamD.SetcD.dH/2.0;

                dVs = (dpi/2.0*dAsp*dfyh*dD1/ds) / 2.0;
                if(enDir == LONGI)
                { 
					int nLeg = nlegs - 1;
					if (nLeg < 0) nLeg = 0;

					dAvCal = dAsp * nLeg;
                    dVs += dAvCal*dfyh*dD2/ds;
                }
                else
                { 
                    dVs += dAv*dfyh*dD2/ds;
					dAvCal = dAv;
                }
            }
            else
            {
                //dD1 = dh - ddc - ddbs;
				dD1 = dh - 2.0*ddc + ddbl + ddbs;
                dVs = dAv * dfyh * dD1 / ds;
            }
        }

        dVp = 0.15 * dPtop* dts / dLs;

        // Set Result
        ShearCapaD.enPierType = enPierType;
        ShearCapaD.dVn1 = dVn1;
        ShearCapaD.dVn2 = dVn2;
        ShearCapaD.dVc  = dVc;
        ShearCapaD.dVc2 = dVc2;
        ShearCapaD.dVc5 = dVc5;
        ShearCapaD.dVc8 = dVc5;
        ShearCapaD.dVs  = dVs;
		ShearCapaD.dAvCal = dAvCal;
        ShearCapaD.dVp  = dVp;
        ShearCapaD.dD1  = dD1;
        ShearCapaD.dD2  = dD2;
    }
}

void CBrdgSeisEvalKISTEC2015::Get_WallVn(T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_SHR_CAPA& ShearCapaD)
{
    double dfck  = ParamD.MatlD.dfck;
    double dfyh  = ParamD.MatlD.dfyh;
    double dh    = ParamD.SetcD.dH;
    double db    = ParamD.SetcD.dB;
    double ddc   = ParamD.BarD.ddc;
    double drho_sh = ParamD.BarD.drhosh;

    double dD1  = dh - ddc;
    double dVn1 = 0.66 * sqrt(dfck) * db * dD1;
    double dVn2 = (0.165*sqrt(dfck) + drho_sh*dfyh) * db * dD1;

    // Set Result
    ShearCapaD.enPierType = WALL;
    ShearCapaD.dVn1 = dVn1;
    ShearCapaD.dVn2 = dVn2;
    ShearCapaD.dD1  = dD1;
	ShearCapaD.nWallShearType = 1; // 콘크리트
}

void CBrdgSeisEvalKISTEC2015::CalcCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_CAPACITY& CapaD)
{
    CapaD.initialize();
    CapaD.bCheck = TRUE;
    CapaD.iColm  = iColm;

    SEIS_RES_K Key;
    Key.Key.GrupK = EvgpK;
    Key.Key.iColm = iColm;
    Key.Key.enPos = enPos;
    Key.Key.enDir = enDir;

    // 교각 휨강도 계산 / 교각의 극한변위 및 공급변위연성도 산정
    Calc_BendingCapacity(Key.KeyMap, ParamD, CapaD.DispDuctD, CapaD.BendCapaD); 
    // 전단강도 계산
    Calc_ShearCapacity(enDir, ParamD, CapaD.DispDuctD, CapaD.ShearCapaD);
}
#include <tuple>
BOOL CBrdgSeisEvalKISTEC2015::DoSeisEval()
{
    CCurUnitSaver UnitSaver(TRUE);
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);

    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if(!pResult) { ASSERT(0); }
    pResult->DeleteAllResult();

    clock_t TimeStart = clock();

    T_BSED_D BsedD;
    if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);
    CString strCode = CDBLib::GetBrdgSeisEvalCodeName(BsedD.nDesignCode);

    CString strMsg;
    strMsg.Format(_LS(IDS_DGN_COM_START_BRDG_SEIS_EVAL), strCode);
    Write_GDisplayAnalString(strMsg, FALSE, FALSE);
        
    CArray<T_EVGP_K, T_EVGP_K> arEvgpK; // 검토 대상 Evaluation Group List
    m_pDoc->m_pAttrCtrl2->GetEvgpKeyList(arEvgpK);
    int nEvgp = arEvgpK.GetCount();
    if(nEvgp==0)
    {
        AfxMessageBox(_LS(IDS_DGN_BRDG_SEIS_NOEVGP));
        return FALSE;
    }
    
    // 하중조합별로 - seismic type
    BOOL bLcom = FALSE; // 나중을 위해 옵션으로 남겨둠.
    CArray<T_LCOM_K, T_LCOM_K> arLcomK;
    m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, arLcomK);
    int nLcom = arLcomK.GetSize();

    if(nLcom==0)
    {
        AfxMessageBox(_LS(IDS_DGN_BRDG_SEIS_NOLCOM));
        return FALSE;
    }

	//////////////////////////////////////////////////////////////////////////
	int nRs = 0;
	int nTh = 0;
	for (int i = 0; i < nLcom; i++)
	{

		T_LCOM_D LcomD;
		if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[i], LcomD)) continue;

		if (LcomD.nSeisLcomType > 0 || LcomD.nSeisLcomType < 4)
		{
			m_pDoc->m_pAttrCtrl->GetSeizEvalLcomType(LcomD, nRs, nTh);
		}
	}
	if (nRs > 0 && nTh > 0)
	{
		m_pDoc->DisplayHistoryMessage(_T("[Error] Response Spectrum Load Case and Time History Load Case cannot be considered together."));
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////

    if(!bLcom)
    {
        arLcomK.RemoveAll();
        arLcomK.Add(0); // Envelope
        nLcom = 1;
    }

    BOOL bStop = FALSE;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_BRDG_SEIS_EVAL);
    
    // 검토 리스트
    arPairKey arEvgpColm;
    arEvgpColm.RemoveAll();
	arPairKey arEvgpPier;
	arEvgpPier.RemoveAll();
    for(int i=0; i<nEvgp; ++i)
    {   
        T_EVGP_K EvgpK = arEvgpK[i];
        T_EVGP_D EvgpD;
        if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD))
        {
            strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOEVGP), i+1, nEvgp);
            GDisplayAnalString(strMsg);
            continue;
        }
        if(EvgpD.nType!=0) // only pier.
		{
			PairKey pEvgpCol(EvgpK, i);
			arEvgpPier.Add(pEvgpCol);
			continue; 
		}
        int nCol = EvgpD.aEvgpColm.GetSize();
        for(int j=0; j<nCol; ++j)
        {
            T_EVPM_K EvmpK = m_pDoc->m_pAttrCtrl2->GetEvpmKey(EvgpK, j);
            if(!m_pDoc->m_pAttrCtrl2->ExistEvpm(EvmpK))
            {
                strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOEVALPARAM), EvgpD.strName);
                GDisplayAnalString(strMsg);
                continue; 
            }

            PairKey pEvgpCol(EvgpK, j);
            arEvgpColm.Add(pEvgpCol);
        }
    }
    INT_PTR nEvgpCol = arEvgpColm.GetSize();
	INT_PTR nEvgpPier = arEvgpPier.GetSize();
    if(nEvgpCol==0 && nEvgpPier==0)
    {
        //GDisplayAnalString(_LS(IDS_DGN_COM_END_BRDG_SEIS_EVAL));
        //AfxMessageBox(_LS(IDS_DGN_COM_END_BRDG_SEIS_EVAL));
        return FALSE;
    }
    
    BOOL bSamePos = FALSE;
    auto lambda_PrintMsg = [this] (const CString& strFormat, const CString& strEvgp, const CString& strColm, const int& nNum, const int& nTotal, BOOL& bSamePos)
    {
        CString strName;
        strName.Format(_T("%s(%s)"), strEvgp, strColm);

        CString strMsg;
        strMsg.Format(strFormat, strName);
        GDisplayAnalStringSamePos(strMsg);
        strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_CHK), nNum, nTotal, strName);
        GDisplayAnalString(strMsg);
        bSamePos = FALSE;
    };

    const bool bConsiderPlainConc = CDBLib::IsSeisEvalCodeAllowPlainConc(BsedD.nDesignCode); // KALIS2023

    int nCalcCount=0;
    for(int i=0; i<nEvgpCol; ++i)
    {
        PairKey pEvgpCol = arEvgpColm[i];

        T_EVGP_K EvgpK = pEvgpCol.first;
        UINT     iCol  = pEvgpCol.second;

        // Evaluation Group 검토 조건
        T_EVGP_D EvgpD;
        if(!m_pDoc->m_pAttrCtrl2->GetEvgp(EvgpK, EvgpD))
        { 
            strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOEVGP), i+1, nEvgp);
            GDisplayAnalStringSamePos(strMsg);
            bSamePos = FALSE;
            continue; 
        }
        T_EVPM_K EvmpK = m_pDoc->m_pAttrCtrl2->GetEvpmKey(EvgpK, iCol);
        if(!m_pDoc->m_pAttrCtrl2->ExistEvpm(EvmpK))
        {
            strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOEVALPARAM), EvgpD.strName);
            GDisplayAnalStringSamePos(strMsg);
            bSamePos = FALSE;
            continue; 
        }
            
        CString strCol = EvgpD.aEvgpColm[iCol].strName;
        CString strPierColm;
        strPierColm.Format(_T("%s(%s)"), EvgpD.strName, strCol);

        BOOL bRAHMEN = IsRAHMEN(EvgpK, iCol);
        T_MPCC_K MpccK_Bot = GetMpccKey(EvgpK, iCol, BOTTOM);
        T_MPCC_K MpccK_Top = GetMpccKey(EvgpK, iCol, TOP);
        bool bPlainConc = false;
        // m-phi가 설정안된경우
        if(MpccK_Bot==0 && MpccK_Top==0)
        {
            if(bConsiderPlainConc)
            {
                // KALIS2023 - Plain Concrete
                bPlainConc = !ExistRebar(EvgpK, iCol, TOP) && !ExistRebar(EvgpK, iCol, BOTTOM);
            }
            
            if(!bPlainConc)
            {
                lambda_PrintMsg(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOMPHI), EvgpD.strName, strCol, i + 1, nEvgpCol, bSamePos);
                continue;
            }
        }
        else if(bRAHMEN)
        {
            if(MpccK_Bot==0)
            {
                lambda_PrintMsg(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOMPHI_BOT), EvgpD.strName, strCol, i+1, nEvgpCol, bSamePos);
                continue;
            }
            else if(MpccK_Top==0)
            {
                lambda_PrintMsg(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOMPHI_TOP), EvgpD.strName, strCol, i+1, nEvgpCol, bSamePos);
                continue;
            }
        }
        else
        {
            if(MpccK_Bot==0)
            {
                lambda_PrintMsg(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOMPHI_BOT), EvgpD.strName, strCol, i+1, nEvgpCol, bSamePos);
                continue;
            }
        }

        // Step #1. M-Phi 계산.
        BOOL bExistMPhi[2] = { FALSE, FALSE };
        if(!bPlainConc)
        {
            if(MpccK_Bot != 0)
            {
                T_MPCC_D MpccD;
                if(m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK_Bot, MpccD) && MpccD.bIdealize)
                {
                    int nChk = CalcMomentCurvature(MpccK_Bot, TRUE);
                    if(nChk == 0) bExistMPhi[0] = TRUE;
                }
            }
            if(MpccK_Top != 0)
            {
                T_MPCC_D MpccD;
                if(m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK_Top, MpccD) && MpccD.bIdealize)
                {
                    int nChk = CalcMomentCurvature(MpccK_Top, TRUE);
                    if(nChk == 0) bExistMPhi[1] = TRUE;
                }
            }

            // m-phi가 계산안된경우
            if(!bExistMPhi[0] && !bExistMPhi[1])
            {
                lambda_PrintMsg(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOTMPHI), EvgpD.strName, strCol, i + 1, nEvgpCol, bSamePos);
                continue;
            }
            else if(bRAHMEN)
            {
                if(!bExistMPhi[0])
                {
                    lambda_PrintMsg(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOTMPHI_BOT), EvgpD.strName, strCol, i + 1, nEvgpCol, bSamePos);
                    continue;
                }
                else if(!bExistMPhi[1])
                {
                    lambda_PrintMsg(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOTMPHI_TOP), EvgpD.strName, strCol, i + 1, nEvgpCol, bSamePos);
                    continue;
                }
            }
            else
            {
                if(!bExistMPhi[0])
                {
                    lambda_PrintMsg(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_NOTMPHI_BOT), EvgpD.strName, strCol, i + 1, nEvgpCol, bSamePos);
                    continue;
                }
            }
        }

        double dPcolbot = Calc_PierColmBotEnvAxialForce(EvgpK, iCol);
        double dPcoltop = Calc_PierColmTopEnvAxialForce(EvgpK, iCol);
        CMapEx<T_ELNK_K, T_ELNK_K, double, double&> mapERect;
        CMapEx<T_NLNK_K, T_NLNK_K, double, double&> mapNRect;
        double dPtop    = Calc_PierCapSuppEnvAxialForce(EvgpK, iCol, mapERect, mapNRect);

        // Local Direction; Longitudinal, Transverse
        for(int l=0; l<2; ++l) 
        {
            EN_LOCAL_DIR enDir = l==0 ? LONGI : TRANS;

            // Check position
            for(int k=0; k<2; ++k) // bottom / top
            { 
                EN_POSITION enPos = k==0 ? BOTTOM : TOP;
                T_MPCC_K    MpccK = k==0 ? MpccK_Bot : MpccK_Top;

                // Check M-Phi result
                if(MpccK > 0)
                {
                    if(!bExistMPhi[k] || !Check_MPhiRes(EvgpK, iCol, enDir, enPos))
                    {
                        if(enDir == LONGI && enPos == BOTTOM)
                            strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_IDEAL_BL), strPierColm);
                        else if(enDir == LONGI && enPos == TOP)
                            strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_IDEAL_TL), strPierColm);
                        else if(enDir == TRANS && enPos == BOTTOM)
                            strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_IDEAL_BT), strPierColm);
                        else if(enDir == TRANS && enPos == TOP)
                            strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_SKIP_IDEAL_TT), strPierColm);

                        if(bSamePos)
                            GDisplayAnalStringSamePos(strMsg);
                        else
                            GDisplayAnalString(strMsg);

                        bSamePos = FALSE;
                        continue;
                    }
                }
                else if(!bPlainConc) continue;

                T_BRDGEVAL_CALC_PARAM OrgParamD;
                GetCalcParam(EvgpK, iCol, enDir, enPos, bPlainConc, OrgParamD);

                if(OrgParamD.OptD.dHe < cDgn_Zero) continue; // 캔틸레버 상부여도 0.0이 들어옴. 그래서 skip됨.

                // Load-combination
                for(int j=0; j<nLcom; ++j)
                {
                    BOOL bMinMax = FALSE;
                    if(bLcom)
                    {
                        T_LCOM_D LcomD;
                        if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[j], LcomD)) continue;
                        if(LcomD.nActive != 1) continue;
                        bMinMax = m_pDoc->m_pAttrCtrl->IsMaxMinComb(LcomD);
                    }
                    int nMinMax = bMinMax ? 2 : 1;
                    for(int mm=0; mm<nMinMax; ++mm) // Min/Max type
                    {
                        T_BRDGEVAL_CALC_PARAM ParamD = OrgParamD;
                        if(bPlainConc)
                        {
                            GetCalcParam_Force(EvgpK, iCol, enDir, enPos, arLcomK[j], mm, ParamD);
                        }
                        else
                        {
                            GetCalcParam_Force(MpccK, iCol, enDir, arLcomK[j], mm, ParamD);
                        }
                        SetCalcParam_AxialForce(dPtop, dPcoltop, dPcolbot, ParamD.ForceD);
                        ParamD.OptD.nMaxLcom = bMinMax ? (mm==0 ? 1 : -1) : 0;
                        ParamD.OptD.iColm    = iCol;

                        // Step #2. 휨강도 계산.
                        // Step #3. 극한변위 및 공급변위연성도 계산.
                        // Step #4. 전단강도 계산.
                        T_BRDGEVAL_CAPACITY CapaD;
                        CapaD.initialize();
                        if(!bPlainConc) CalcCapacity(EvgpK, iCol, enDir, enPos, ParamD, CapaD);

                        // Step #5. 파괴모드 / 소요역량 산정
                        T_BRDGEVAL_EVALUATION EvalD;
                        EvalD.initialize();
                        if(!bPlainConc)
                        {
                            CheckFailureType(enDir, enPos, CapaD, ParamD.SetcD.strShape, EvalD.ModeD);
                            CalcReqCapacity(EvgpK, iCol, enDir, enPos, EvalD.ModeD.enFalureMode, ParamD, EvalD.EvalD);
                        }
                        else
                        {
                            // Plain Concrete
                            CalcPlainConcCapacity(EvgpK, iCol, enDir, enPos, EvalD.ModeD.enFalureMode, ParamD, EvalD);
                        }
                        EvalD.bCheck = TRUE;
                        EvalD.dRat = SafeDiv(EvalD.ModeD.dFpc, EvalD.EvalD.dFpd, _INFI4);
                        EvalD.iColm = iCol;

                        // save result
                        if(bLcom)
                        {
                            pResult->WriteSeisEvalResultParam(EvgpK, iCol, enDir, enPos, arLcomK[j], (mm==0), ParamD);
                            pResult->WriteSeisEvalResultCapa(EvgpK, iCol, enDir, enPos, arLcomK[j], (mm==0), CapaD);
                            pResult->WriteSeisEvalResultEval(EvgpK, iCol, enDir, enPos, arLcomK[j], (mm==0), EvalD);
                        }

                        // Get max data
                        T_BRDGEVAL_EVALUATION MaxEvalD;
                        if(!pResult->GetEvalRes(EvgpK, iCol, enDir, enPos, MaxEvalD)) MaxEvalD.initialize();
                        if(!MaxEvalD.bCheck || MaxEvalD.dRat < EvalD.dRat)
                        {
                            // save result
                            pResult->SetCalcParam(EvgpK, iCol, enDir, enPos, ParamD);
                            pResult->SetCapaRes  (EvgpK, iCol, enDir, enPos, CapaD);
                            pResult->SetEvalRes  (EvgpK, iCol, enDir, enPos, EvalD);
                        }

                        nCalcCount++;
                    } // mm
                } // j
            } // k
        } // l    
        
        strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_CHK), i+1, nEvgpCol, strPierColm);
        if(!bSamePos) { GDisplayAnalString(strMsg); bSamePos = TRUE; }
        else          { GDisplayAnalStringSamePos(strMsg);           }

        bStop = ProDlg.Update_ProgressDlg(i+1, nEvgpCol);
        if(bStop) break;
    }  // i
    ProDlg.Delete_ProgressDlg();

    if(!bStop)
    {
        // End message  
        clock_t TimeEnd = clock();
        double dTimeSpan = (double)(TimeEnd-TimeStart) / CLOCKS_PER_SEC;
        strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_TIME), dTimeSpan);
        GDisplayAnalString(strMsg);
        strMsg.Format(_LS(IDS_DGN_COM_END_BRDG_SEIS_EVAL), strCode);
        GDisplayAnalString(strMsg);

        // Write file
        pResult->WriteSeisEvalMaxResult();

		if(nEvgpCol == 0)
		{
			AfxMessageBox(_LS(IDS_DGN_BRDG_SEIS_EVGPCOL_NOTHING));
		}
        else if(nCalcCount==0)
        {
            AfxMessageBox(_LS(IDS_DGN_BRDG_SEIS_EVAL_NOTHING));
        }
    }
    else
    {
        pResult->DeleteAllResult();
        GDisplayAnalString(_LS(IDS_DGN_BRDG_STOP));
        return FALSE;
    }

    return TRUE;
}

// Step #5. 파괴모드 산정
void CBrdgSeisEvalKISTEC2015::CheckFailureType(const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const T_BRDGEVAL_CAPACITY& CapaD, const CString strShape, T_BRDGEVAL_FAILURE_MODE& ModeD)
{
    // 공급역량; Capacity
    double dDeltac=0.0, dVn=0.0;
    ModeD.enFalureMode = GetFailureMode(CapaD.BendCapaD, CapaD.ShearCapaD, dDeltac, dVn);
    ModeD.bRoundSect = (strShape == D_SECT_SHAPE_REG_P || strShape == D_SECT_SHAPE_REG_SR) ? TRUE : FALSE;

    if(ModeD.enFalureMode == TYPE1_BEND) // Type I [휨파괴]
    {
        ModeD.dDeltac      = dDeltac;
        ModeD.dFn          = dVn;
        ModeD.dVn          = dVn;
        ModeD.dFpc         = min(CapaD.DispDuctD.dmu_Deltac, CapaD.DispDuctD.dmu_DeltaMax);
    }
    else if(ModeD.enFalureMode == TYPE2_BEND_SHR) // Type II [휨-전단파괴]
    {
        ModeD.dDeltac      = dDeltac;
        ModeD.dVn          = dVn;
        ModeD.dFn          = ModeD.dVn;
        ModeD.dFpc         = dDeltac / CapaD.BendCapaD.dDeltay;
    }
    else if(ModeD.enFalureMode == TYPE3_SHR) // Type III [전단파괴]
    {
        ModeD.dDeltac      = dDeltac;
        ModeD.dVn          = dVn;
        ModeD.dFn          = ModeD.dVn;
        ModeD.dFpc         = ModeD.dVn;
    }
}

// Step #6. 공급역량 산정
void CBrdgSeisEvalKISTEC2015::CalcReqCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const EN_FAILURE_MODE& enFalureMode, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_EVAL_CAPA& EvalD)
{
    EvalD.initialize();

    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if(!pResult) { ASSERT(0); }
    T_BRDGEVAL_MPHI_STATE StateD;
    if(!pResult->GetMPhiState(EvgpK, iColm, enDir, enPos, StateD)) { ASSERT(0); return; }

    double dMy       = StateD.dMoment[4];
    double dMe       = (enDir == LONGI) ? fabs(ParamD.ForceD.dForce[4]) : fabs(ParamD.ForceD.dForce[5]); // 조합탄성지진력
    double dVe       = (enDir == LONGI) ? fabs(ParamD.ForceD.dForce[2]) : fabs(ParamD.ForceD.dForce[1]); // 조합탄성지진력
    double dPtop     = ParamD.ForceD.dPtop; // (+)압축, (-)인장; 교각상부 축력
    dPtop     = max(dPtop, 0.0);
    double dDisp     = ParamD.ForceD.dDisp; // 받침상단 변위(pier cap 최대변위)
    // 도로교설계기준(2015) 8.10.2.3 교각의 P-delta 효과 적용
    if(ParamD.OptD.enBehavior == RAHMEN) dDisp *= (1.5/2.0); 
    else                                 dDisp *= 1.5;
    double dMe_pd    = dMe + dPtop*dDisp;     // p-delta 고려

    double dRs  = 0.0;
    double dLambda_DR = 0.0;
    double dFpd = 0.0;
    if(enFalureMode == TYPE1_BEND || enFalureMode == TYPE2_BEND_SHR)
    {
        dRs = (dMy < dMe_pd) ? dMe_pd/dMy : 1.0;
        double dT  = ParamD.OptD.dT;
        double dTs = ParamD.OptD.dTs;

        if (dT == 0.0) dLambda_DR = 1.0;
        else           dLambda_DR = dT < 1.25*dTs ? ((1.0-1.0/dRs)*(1.25*dTs/dT)+1.0/dRs) : 1.0;

        dFpd = dLambda_DR * dRs;
    }
    else if(enFalureMode == TYPE3_SHR)
    {
        dFpd = dVe;
    }

    // Set result
    EvalD.dMe    = dMe;
    EvalD.dP     = dPtop;
    EvalD.dDelta = ParamD.ForceD.dDisp;
    EvalD.dMe_pd = dMe_pd;
    EvalD.dRs    = dRs;
    EvalD.dT     = ParamD.OptD.dT;
    EvalD.dTs    = ParamD.OptD.dTs;
    EvalD.dLadbdaDR = dLambda_DR;
    EvalD.dVe    = dVe;
    EvalD.dFpd   = dFpd;
}

////////////////////////////
// 최대공급변위연성도 μΔ,max
double CBrdgSeisEvalKISTEC2015::Get_MaxSupplyDispDuct(const int& nSplice, const CString& strShape, const EN_LOCAL_DIR& enDir, const int& nBHmethod, const double& dArea, const double& dB, const double& dH, const double& dlsp)
{
    const double dZero = 1.0E-7;
    double dmuDeltaMax = DBL_MAX;
    if(nSplice == 0 || nSplice == 1)  return dmuDeltaMax; // 단일철근 or 50% 이음
    else if(nSplice == 2) // 100% 이음
    {
        if(strShape == D_SECT_SHAPE_REG_P || strShape == D_SECT_SHAPE_REG_SR)
        {
            if(dlsp < dZero || (dlsp/dH) < 0.5+dZero) dmuDeltaMax = 1.80;
        }
        else
        {
            if(enDir==LONGI)
            {
                double dBHrat = Get_BHrat(nBHmethod, strShape, dArea, dB, dH);
                if(dBHrat > 4.0-dZero) dmuDeltaMax = 1.00;
                else if(dBHrat < 1.5+dZero) dmuDeltaMax = 1.50;
                else                        dmuDeltaMax = 1.80-0.2*dBHrat;
            }
            else
            {
                dmuDeltaMax = 1.50;
            }
        }
    }
    else ASSERT(0);

    return dmuDeltaMax;
}

EN_PIER_TYPE CBrdgSeisEvalKISTEC2015::Get_PIER_TYPE(const EN_LOCAL_DIR& enDir, const int& nSplice, const CString& strShape, const int& nBHmethod, const double& dArea, const double& dB, const double& dH)
{
    const double dZero = 1.0E-7;
    EN_PIER_TYPE enPierType = EN_PIER_TYPE::NONE;
	if (enDir == TRANS)
	{
		//if (nSplice == 0 || nSplice == 1)  enPierType = GENERAL; // 단일철근 or 50% 이음
		//else if (nSplice == 2) // 100% 이음
		{
			if (strShape == D_SECT_SHAPE_REG_P || strShape == D_SECT_SHAPE_REG_SR)
			{
				enPierType = GENERAL;
			}
			else
			{
				//if (enDir == TRANS/*LONGI*/)
				{
					double dBHrat = Get_BHrat(nBHmethod, strShape, dArea, dB, dH);
					if (dBHrat > 4.0 - dZero)      enPierType = WALL;
					else if (dBHrat < 1.5 + dZero) enPierType = GENERAL;
					else                        enPierType = TRANSIENT;
				}
				//else
				//{
				//	enPierType = GENERAL;
				//}
			}
		}
	}
	else
	{
		enPierType = GENERAL;
	}

    return enPierType;
}

// 종횡비 계산
double CBrdgSeisEvalKISTEC2015::Get_BHrat(const int& nMethod, const CString& strShape, const double& dArea, const double& dB, const double& dH)
{
    double dHs = dH;
    double dBs = dB;

    if(nMethod==1) // 제안된 방법
    {
        double dpi = 4.0*atan(1.0); // 3.141592.....
        if(strShape == D_SECT_SHAPE_REG_STRK)
            dBs += (dpi/4.0-1.0)*dHs;
        else if(strShape == D_SECT_SHAPE_REG_HTRK)
            dBs += (dpi/4.0-1.0)*dHs/2.0;
        else
            dBs = dArea/dHs;
    }

    return (dBs/dHs);
}

EN_FAILURE_MODE CBrdgSeisEvalKISTEC2015::GetFailureMode(const T_BRDGEVAL_BEND_CAPA& BendCapaD, const T_BRDGEVAL_SHR_CAPA& ShearCapaD, double& dDeltac, double& dVn)
{
    struct _Point
    {
        double dx, dy;
        _Point() 
        {
            dx = 0.0;
            dy = 0.0;
        }
        _Point(double xval, double yval) 
        {
            dx = xval;
            dy = yval;
        }
        _Point(_Point& pt)
        {
            dx = pt.dx;
            dy = pt.dy;
        }
        void init()
        {
            dx = 0.0;
            dy = 0.0;
        }
        void Set(_Point& pt)
        {
            dx = pt.dx;
            dy = pt.dy;
        }
        void Set(double xval, double yval)
        {
            dx = xval;
            dy = yval;
        }
    };
    struct _LINE
    {
        _Point Pnt[2];
        _LINE() 
        {
            Pnt[0].init();
            Pnt[1].init();
        }
        _LINE(_Point& pt1, _Point& pt2) 
        {
            Pnt[0].Set(pt1);
            Pnt[1].Set(pt2);
        }
        _LINE(_LINE& line)
        {
            Pnt[0].Set(line.Pnt[0]);
            Pnt[1].Set(line.Pnt[1]);
        }
        void Set(_Point& pt1, _Point& pt2)
        {
            Pnt[0].Set(pt1);
            Pnt[1].Set(pt2);
        }
        void Set(double dx1, double dy1, double dx2, double dy2)
        {
            Pnt[0].Set(dx1, dy1);
            Pnt[1].Set(dx2, dy2);
        }
    };

    CArray<_LINE, _LINE&> arMoment; arMoment.RemoveAll();
    CArray<_LINE, _LINE&> arShear;  arShear.RemoveAll();

    // Moment Line
    // 원점-항복점
    _LINE LineD;
    LineD.Pnt[0].init();
    LineD.Pnt[1].Set(BendCapaD.dDeltay, BendCapaD.dFy);
    arMoment.Add(LineD);
    // 항복점-극한점
    LineD.Pnt[0].Set(BendCapaD.dDeltay, BendCapaD.dFy);
    if(BendCapaD.dDeltac >= BendCapaD.dDeltay)
        LineD.Pnt[1].Set(BendCapaD.dDeltac, BendCapaD.dFn);
    else
        LineD.Pnt[1].Set(BendCapaD.dDeltau, BendCapaD.dFu);
    arMoment.Add(LineD);

    // Shear Line
    if(ShearCapaD.dVn1 > 0.0 || ShearCapaD.dVn2 > 0.0)
    {
        double dVn = Get_WallVn(ShearCapaD);
        LineD.Pnt[0].Set(0.0, dVn);
        LineD.Pnt[1].Set(8.0*BendCapaD.dDeltay, dVn);
        arShear.Add(LineD);
    }
    else
    {
        double dVn = ShearCapaD.dVc2 + ShearCapaD.dVs + ShearCapaD.dVp;
        LineD.Pnt[0].Set(0.0, dVn);
        LineD.Pnt[1].Set(2.0*BendCapaD.dDeltay, dVn);
        arShear.Add(LineD);

        LineD.Pnt[0].Set(2.0*BendCapaD.dDeltay, dVn);
        dVn = ShearCapaD.dVc5 + ShearCapaD.dVs + ShearCapaD.dVp;
        LineD.Pnt[1].Set(5.0*BendCapaD.dDeltay, dVn);
        arShear.Add(LineD);

        LineD.Pnt[0].Set(5.0*BendCapaD.dDeltay, dVn);
        LineD.Pnt[1].Set(8.0*BendCapaD.dDeltay, dVn);
        arShear.Add(LineD);
    }

    // Get cross point.
    int nMoment = arMoment.GetSize();
    int nShear  = arShear.GetSize();
    _Point CrossPnt;
    BOOL bIntersect=FALSE;
    for(int i=0; i<nMoment; ++i)
    {
        double SCoor_i[3] = {arMoment[i].Pnt[0].dx, arMoment[i].Pnt[0].dy, 0.0};
        double SCoor_j[3] = {arMoment[i].Pnt[1].dx, arMoment[i].Pnt[1].dy, 0.0};
        for(int j=0; j<nShear; ++j)
        {
            double Coor_i[3] = {arShear[j].Pnt[0].dx, arShear[j].Pnt[0].dy, 0.0};
            double Coor_j[3] = {arShear[j].Pnt[1].dx, arShear[j].Pnt[1].dy, 0.0};
            double dDist = 0.0;
            double dCross[3] = {0.,0.,0.};
            if(CMathFunc::mathIntersectLine2(SCoor_i, SCoor_j, Coor_i, Coor_j, cDGN_Zero, dDist, dCross))
            {
                CrossPnt.dx = dCross[0];
                CrossPnt.dy = dCross[1];
                bIntersect=TRUE; break;
            }
            CrossPnt = arMoment[i].Pnt[1];
        }
        if(bIntersect) break;
    }

    dDeltac = CrossPnt.dx;
    dVn     = CrossPnt.dy;

    if(bIntersect) 
    { 
        if(dVn < BendCapaD.dFy-cDGN_Zero) // Type III [전단파괴]
            return TYPE3_SHR;
        else // Type II [휨-전단파괴]
            return TYPE2_BEND_SHR;
    }

    return TYPE1_BEND;  // Type I [휨파괴]
}

double CBrdgSeisEvalKISTEC2015::Get_WallVn(const T_BRDGEVAL_SHR_CAPA& ShearCapaD)
{
    double dVn = min(ShearCapaD.dVn1, ShearCapaD.dVn2);
    return dVn;
}

// sa; longi - sax, trans - say
BOOL CBrdgSeisEvalKISTEC2015::Check_Inter3Faces(double& dca1, const double& dca2, const double& dca2p, const double& dhcop, const double& dsa, const double& dLp)
{
    double d15ca1 = 1.5*dca1;
    if(dLp < 2.0*d15ca1)
    {
        // 콘크리트파괴 저항면적 상정시 3면 이상 가장자리의 영향 검토
        BOOL bChk1s = dca2  < d15ca1;
        BOOL bChk2s = dca2p < d15ca1;
        BOOL bChk3s = dhcop < d15ca1;
        if(bChk1s && bChk2s && bChk3s)
        {
            double dha = min(dhcop, d15ca1);

            // 3면이 가장자리의 영향을 받으므로 하중작용방향으로 연단거리 재산정 필요
            dca1 = max(dca2/1.5, dha/1.5);
            dca1 = max(dca1, dsa/3.0);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CBrdgSeisEvalKISTEC2015::Check_Inter4Faces(double& dhef, const int& iDir, const double& dca1, const double& dca1p, const double& dca2, const double& dca2p, const double& dsax, const double& dsay, const int& nnB, const double& ds1, const double& ds2)
{
    double d15hef = 1.5*dhef;
    double dnB    = static_cast<double>(nnB);
    BOOL bChk1s = dca1  < d15hef;
    BOOL bChk2s = FALSE;
    if(iDir==0) bChk2s = dca1p < d15hef;
    else        bChk2s = dca2  < d15hef;
    BOOL bChk3s = FALSE;
    if(iDir==0) bChk3s = dca2  < d15hef;
    else        bChk3s = dca2p < d15hef;
    BOOL bChk4s = FALSE;
    double dmaxs = 0.0;
    if(iDir==0)
    {
        double ds1sax = max(ds1, dsax);
        double ds2say = ds2*(dnB-1.0) + dsay;
        dmaxs = max(ds1sax, ds2say); // 앵커간 최대 길이
        bChk4s = dmaxs < 2.0*d15hef;
    }
    else
    {
        double ds1say = max(ds1, dsay);
        double ds2sax = ds2*(dnB-1.0) + dsax;
        dmaxs = max(ds1say, ds2sax); // 앵커간 최대 길이
        bChk4s = dmaxs < 2.0*d15hef;
    }

    double dcamax=0.0;
    if(bChk1s && bChk2s && bChk3s && bChk4s)
    {
        if(iDir==0)
        {
            dcamax = max(max(dca1, dca1p), dca2);
        }
        else
        {
            dcamax = max(dca1, max(dca2, dca2p));
        }
        dhef = max(dcamax/1.5, dmaxs/3.0);

        return TRUE;
    }

    return FALSE;
}

void CBrdgSeisEvalKISTEC2015::CalcPlainConcCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const EN_FAILURE_MODE& enFalureMode, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_EVALUATION& EvalD)
{
	EvalD.initialize();
	ASSERT(0);
}

