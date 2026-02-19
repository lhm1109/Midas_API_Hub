#include "stdafx.h"
#include "BrdgSeisEvalKISTEC2015.h"

#include "..\wg_db\BrdgSeisEvalResult.h"
#include "DgnProgressDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_MathFunc.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bearing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CBrdgSeisEvalKISTEC2015::DoSeisEvalBearing()
{
    CCurUnitSaver UnitSaver(TRUE);
    m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(m_CodeUnit);

    clock_t TimeStart = clock();

    T_BSED_D BsedD;
    if ( !m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD) ) ASSERT(0);
    CString strCode = CDBLib::GetBrdgSeisEvalCodeName(BsedD.nDesignCode);

    CString strMsg;
    strMsg.Format(_LS(IDS_DGN_COM_START_BRDG_SEIS_EVAL_BEARING), strCode);
    Write_GDisplayAnalString(strMsg, FALSE, FALSE);

    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if ( !pResult ) { ASSERT(0); }
    pResult->DeleteBearingResult();

    // Get Bearing Evaluation.
    CArray<T_BREV_K, T_BREV_K> arAllBrevK; arAllBrevK.RemoveAll();
    CArray<T_BREV_K, T_BREV_K> arBrevK; arBrevK.RemoveAll();
    CArray<CString, CString&> arBrevName; arBrevName.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetBrevKeyList(arAllBrevK);
    INT_PTR nBrev = arAllBrevK.GetSize();

    // Check Evaluation is possible
    for ( INT_PTR i=0; i<nBrev; ++i )
    {
        T_BREV_K BrevK = arAllBrevK[i];

        int nNGType = 0;
        T_BREV_D BrevD;
        BrevD.Initialize();
        if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) ASSERT(0);

        T_ANEV_D AnevD;
        T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);
        if ( AnevK == 0 ) nNGType = 1;
        else
        {
			T_BRPR_D BrprD;
			for (int j = 0; j < AnevD.BearingProp.aBrprKs.GetSize(); j++)
			{
				for (int k = 0; k < AnevD.BearingProp.aBrprKs[j].aBrprK.GetSize(); k++)
				{
					T_BRPR_K BrprK = AnevD.BearingProp.aBrprKs[j].aBrprK[k];

					if (!m_pDoc->m_pAttrCtrl2->ExistBrpr(BrprK)) 
					{
						int nBrevType = AnevD.SuppType.aLayer[j].aBrevType[k];
						if(nBrevType == -1) continue;

						nNGType = 2;
					}
				}
			}
        }

        if ( nNGType > 0 )
        {
            if ( nNGType == 1 )
                strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_BEARING_EVAL_SKIP_ANEV), BrevD.strName);
            else if ( nNGType == 2 )
                strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_BEARING_EVAL_SKIP_BRBR), BrevD.strName);

            Write_GDisplayAnalString(strMsg, FALSE, FALSE);
        }
        arBrevK.Add(BrevK);
        arBrevName.Add(BrevD.strName);
    }
    nBrev = arBrevK.GetSize();

    if ( nBrev==0 )
    {
        AfxMessageBox(_LS(IDS_DGN_BRDG_SEIS_NOBREV));
        return;
    }

    BOOL bLcom = FALSE; // 나중을 위해 옵션으로 남겨둠.
    CArray<T_LCOM_K, T_LCOM_K> arLcomK; arLcomK.RemoveAll();
    arLcomK.Add(0);// Envelope
    INT_PTR nLcom = 1;
    if ( !bLcom )
    {
        arLcomK.RemoveAll();
        m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, arLcomK);
        INT_PTR nLcom = arLcomK.GetSize();
        if ( nLcom==0 )
        {
            AfxMessageBox(_LS(IDS_DGN_BRDG_SEIS_NOLCOM));
            return;
        }
    }

    BOOL bStop = FALSE;
    CDgnProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_BRDG_SEIS_EVAL_BEARING);

	int nTotalNum = 0;
    BOOL bSamePos = FALSE;
    int nCalcCount=0;
    for ( INT_PTR i=0; i<nBrev; ++i )
    {
        T_BREV_K BrevK = arBrevK[i];

        for ( INT_PTR j=0; j<nLcom; ++j )
        {
            BOOL bMinMax = FALSE;
            if ( bLcom )
            {
                T_LCOM_D LcomD;
                if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[j], LcomD) ) continue;
                if ( LcomD.nActive != 1 ) continue;
                bMinMax = m_pDoc->m_pAttrCtrl->IsMaxMinComb(LcomD);
            }
            int nMinMax = bMinMax ? 2 : 1;
            for ( int mm=0; mm<nMinMax; ++mm ) // Min/Max type
            {
                T_BRDGEVAL_BEARING ResD;
                ResD.initialize();
                ResD.ParamD.initialize();
                if ( !Get_BearingParam(BrevK, ResD, TRUE) ) continue;

                Calc_BearingBody(BrevK, ResD);
                Calc_BearingAnchor(BrevK, ResD);
                Calc_BearingSupport_Length(BrevK, ResD);

                ResD.bCheck = TRUE;

                // save result
                if ( bLcom )
                {
                    pResult->WriteSeisEvalResultBearing(BrevK, arLcomK[j], (mm==0), nTotalNum, ResD);
                }

                // Get max data
                T_BRDGEVAL_BEARING MaxEvalD;
                if ( !pResult->GetEvalBearingRes(BrevK, MaxEvalD) ) MaxEvalD.initialize();
                if ( !MaxEvalD.bCheck || MaxEvalD.dRat < ResD.dRat )
                {
                    // save result
                    pResult->SetEvalBearingRes(BrevK, ResD);
                }
                nCalcCount++;
            }
        }

        strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_CHK_BEARING), i+1, nBrev, arBrevName[i]);
        if ( !bSamePos ) { GDisplayAnalString(strMsg); bSamePos = TRUE; }
        else { GDisplayAnalStringSamePos(strMsg); }

        bStop = ProDlg.Update_ProgressDlg(i+1, nBrev);
        if ( bStop ) break;
    }
    ProDlg.Delete_ProgressDlg();

    if ( !bStop )
    {
        // End message  
        clock_t TimeEnd = clock();
        double dTimeSpan = (double)(TimeEnd-TimeStart) / CLOCKS_PER_SEC;
        strMsg.Format(_LS(IDS_DGN_BRDG_SEIS_EVAL_TIME_BEARING), dTimeSpan);
        GDisplayAnalString(strMsg);
        strMsg.Format(_LS(IDS_DGN_COM_END_BRDG_SEIS_EVAL_BEARING), strCode);
        GDisplayAnalString(strMsg);

        // Write file
        pResult->WriteSeisEvalBearingMaxResult();

        if ( nCalcCount==0 )
        {
            AfxMessageBox(_LS(IDS_DGN_BRDG_SEIS_EVAL_BEARING_NOTHING));
        }
    }
    else
    {
        pResult->DeleteBearingResult();
        GDisplayAnalString(_LS(IDS_DGN_BRDG_STOP_BEARING));
    }
}

BOOL CBrdgSeisEvalKISTEC2015::Get_BearingParam(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD, BOOL bDesign, BOOL bAnchorDirPosi/*=TRUE*/)
{
    T_BRDGEVAL_BEARING_PARAM& rParmD = rResD.ParamD;

    T_BSED_D BsedD;
    if ( !m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD) ) return FALSE;

    T_BREV_D BrevD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) return FALSE;

    T_ANEV_D AnevD;
	T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD, bAnchorDirPosi);
	if (AnevK == 0) return FALSE;

    rParmD.iEvalCode = BsedD.nDesignCode;
    rParmD.iLcomK    = 0;
    rParmD.nMaxLcom  = 0; // -1:Min, 0:None, +1:Max

    rParmD.dRelAngle = AnevD.dRelAngle;

    int nBearingPropNumb = 0;
    T_ELEM_K btpkTemp;
    CMap<UINT, UINT, UINT, UINT> mapExistBearingPara; mapExistBearingPara.InitHashTable(HASHSIZEELEM);
    T_BRPR_D BrprD;
    for ( int j = 0; j<AnevD.BearingProp.aBrprKs.GetSize(); j++ )
    {
        for ( int k = 0; k<AnevD.BearingProp.aBrprKs[j].aBrprK.GetSize(); k++ )
        {
            T_BRPR_K BrprK = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, j, k);
            nBearingPropNumb += 1;
            if ( !mapExistBearingPara.Lookup(BrprK, btpkTemp) )
            {
                mapExistBearingPara.SetAt(BrprK, btpkTemp);
            }
        }
    }

    rResD.AnchorParamAll.SetSize(nBearingPropNumb);

    nBearingPropNumb = 0;

    for ( int j = 0; j<AnevD.BearingProp.aBrprKs.GetSize(); j++ )
    {
        for ( int k = 0; k<AnevD.BearingProp.aBrprKs[j].aBrprK.GetSize(); k++ )
        {
            T_BRPR_K BrprK = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, j, k);
            if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(BrprK, BrprD) ) BrprD.Initialize();

            ConvertAnchorParam(j, k, BrevD, AnevD, BrprD, rResD.ParamD, rResD.AnchorParamAll[nBearingPropNumb]);
            rResD.AnchorParamAll[nBearingPropNumb].dBPNumb = mapExistBearingPara.GetSize(); //계산서 출력 부분, 불필요한거 없애는 용도
            nBearingPropNumb += 1;
        }
    }

    rParmD.bUserConc   = AnevD.Fail.bUser[0];
    rParmD.bUserPryOut = AnevD.Fail.bUser[1];

    if ( rParmD.bUserConc )
    {
        rParmD.dhefConc[0] = AnevD.Fail.ConcFailL.dHa;
        rParmD.dhefConc[1] = AnevD.Fail.ConcFailT.dHa;
    }
    if ( rParmD.bUserPryOut )
    {
        rParmD.dhefPryo[0] = AnevD.Fail.PryoutL.dHef;
        rParmD.dhefPryo[1] = AnevD.Fail.PryoutL.dHef;
    }

    int nSax = AnevD.aSax.GetSize();
    double dSax = nSax>0 ? AnevD.aSax[0] : 0.0;
    for ( int i=1; i<nSax; ++i )   dSax = min(dSax, AnevD.aSax[i]); // 최소값
    int nSay = AnevD.aSay.GetSize();
    double dSay = nSay>0 ? AnevD.aSay[0] : 0.0;
    for ( int i=1; i<nSay; ++i )   dSay = min(dSay, AnevD.aSay[i]); // 최소값
    rParmD.dSax = max(dSax - (rResD.AnchorParamAll[0].nnL-1.0)*rResD.AnchorParamAll[0].dS1, 0.0); // AnevD.dSax가 받침 중심간 거리임
    rParmD.dSay = max(dSay - (rResD.AnchorParamAll[0].nnT-1.0)*rResD.AnchorParamAll[0].dS2, 0.0); // AnevD.dSay가 받침 중심간 거리임
    rParmD.nnBCL = nSax + 1; // 교축저항 앵커열 배치수
    rParmD.nnBCT = nSay + 1; // 교축직각 저항 앵커열 배치수

    rParmD.dfck  = AnevD.dfck;
    rParmD.dhcop = AnevD.dHcop;

    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    std::vector<std::vector<T_BRPR_D>> aArBrprD;
    for ( int i = 0; i < AnevD.BearingProp.aBrprKs.GetSize(); i++ )
    {
        aArBrprD.push_back(std::vector<T_BRPR_D>());
        for ( int j = 0; j < AnevD.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++ )
        {
			if(CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, i, j) > 0)
			{
				if(!pDoc->m_pAttrCtrl2->GetBrpr(CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, i, j), BrprD)) ASSERT(0);
				aArBrprD[i].push_back(BrprD);
			}
			else
			{
				BrprD.Initialize();
				aArBrprD[i].push_back(BrprD);
			}
        }
    }

	int nDIR_POS = (bAnchorDirPosi) ? 0 : 1;
    CDgnSeisAnchorShapeMgr AnchorShapeMgr;
    AnchorShapeMgr.SetData(aArBrprD, AnevD, bAnchorDirPosi);
	AnchorShapeMgr.SetDULR(nDIR_POS, nDIR_POS);
	AnchorShapeMgr.SetPryoutDULR(nDIR_POS, nDIR_POS);

	CDgnSeisAnchorShapeMgr AnchorShapeMgr2;
	T_ANEV_D AnevD2;
	if (!AnevD.bTensChk)
	{
		AnevD2 = AnevD;
		AnevD2.bTensChk = TRUE;
		T_ANEV_TENS_CHK_D TensChk; TensChk.Initialize();
		m_pDoc->m_pAttrCtrl2->SetAnevTensionNew2(AnevD2.aLayer, AnevD2.BrevK, AnevD2.TensChk, TensChk);
		//m_pDoc->m_pAttrCtrl2->SetAnevTension(AnevD2.aSax, AnevD2.aSay, AnevD2.BrevK, AnevD2.TensChk, TensChk);
		AnevD2.TensChk.Initialize();
		AnevD2.TensChk = TensChk;
		AnchorShapeMgr2.SetData(aArBrprD, AnevD2, bAnchorDirPosi);
		AnchorShapeMgr2.SetDULR(nDIR_POS, nDIR_POS);
		AnchorShapeMgr2.SetPryoutDULR(nDIR_POS, nDIR_POS);

	}


    if ( bDesign == TRUE )
    {
        Get_Rs_LabdaDR(BrevK, rParmD.dRs[0], rParmD.dLadbdaDR[0], rParmD.dRs[1], rParmD.dLadbdaDR[1]);

        // Set forces.
        rParmD.ForceD.dPtop = Get_Ptop(BrevK); // 기둥 1개의 상부 축력
        rParmD.ForceD.dPtopBearing = Get_Ptop(BrevK, true); // 받침 전체 축력
        double dTheta = AnevD.dTheta_q2; // 받침 배치각 사용자 정의
        Get_BeringForce(BrevK, dTheta, rParmD.ForceD.dForce[2], rParmD.ForceD.dForce[4], rParmD.ForceD.dForce[1], rParmD.ForceD.dForce[5]);
        Get_Disp(BrevK, dTheta, rParmD.ForceD.dDisp, rParmD.ForceD.dDispT, rParmD.ForceD.LinkL, rParmD.ForceD.LinkT);
	}
	else
	{
		rParmD.dRs[0] = 1.0;
		rParmD.dRs[1] = 1.0;
		rParmD.dLadbdaDR[0] = 1.0;
		rParmD.dLadbdaDR[1] = 1.0;

		rParmD.ForceD.dPtop = 1.0;
		rParmD.ForceD.dForce[1] = 1.0;
		rParmD.ForceD.dForce[2] = 1.0; 
		rParmD.ForceD.dForce[4] = 1.0; 
		rParmD.ForceD.dForce[5] = 1.0;

		Calc_BearingBody(BrevK, rResD);

		rResD.AnchorD[0].FASD.dFASD = rResD.BodyD[0].HorD.dFBD;
		rResD.AnchorD[1].FASD.dFASD = rResD.BodyD[1].HorD.dFBD;
	}
    CArray<T_ELNK_K, T_ELNK_K> aElnkK, aElnkK2;
    CArray<T_NLNK_K, T_NLNK_K> aNlnkK, aNlnkK2;

    double dMaxP = -DBL_MAX;
    T_NODE_K NodeK1; 
    T_NODE_K NodeK2;
	int ndx = 0;
    for ( int j = 0; j<AnevD.BearingProp.aBrprKs.GetSize(); j++ )
    {
        for ( int k = 0; k<AnevD.BearingProp.aBrprKs[j].aBrprK.GetSize(); k++ )
        {
            AnchorShapeMgr.GetBearingLink(j, k, aElnkK, aNlnkK, FALSE);
            double dMaxPtmp = Get_MaxTensionMulti(BrevK, j, k, aElnkK, aNlnkK, 0);
            AnchorShapeMgr.GetBearingLinkNode(j, k, NodeK1, NodeK2, FALSE);

			if(AnevD.bTensChk)
			{
				if(aElnkK.GetSize()>0)
					rResD.AnchorParamAll[ndx].nLinKOrg = aElnkK[0];
				else if (aNlnkK.GetSize() > 0)
					rResD.AnchorParamAll[ndx].nLinKOrg = aNlnkK[0];
			}
			else
			{
				AnchorShapeMgr2.GetBearingLink(j, k, aElnkK2, aNlnkK2, FALSE);
				if(aElnkK2.GetSize()>0)
					rResD.AnchorParamAll[ndx].nLinKOrg = aElnkK2[0];
				else if(aNlnkK2.GetSize()>0)
					rResD.AnchorParamAll[ndx].nLinKOrg = aNlnkK2[0];					
			}
			ndx++;

            if (dMaxPtmp > dMaxP )
            {
				dMaxP = dMaxPtmp;
				rParmD.ForceD.dMaxP = dMaxP;
                ConvertAnchorParam(j, k, BrevD, AnevD, BrprD, rResD.ParamD, rResD.AnchorTens);

				if(AnevD.bTensChk)
				{
					if(aElnkK.GetSize()>0)
						rResD.AnchorTens.nLinK = aElnkK[0];
					else if(aNlnkK.GetSize() > 0)
						rResD.AnchorTens.nLinK = aNlnkK[0];

				}
				rResD.AnchorTens.nNodeI = NodeK1;
                rResD.AnchorTens.nNodeJ = NodeK2;
				rResD.AnchorTens.nBrI = j;
				rResD.AnchorTens.nBrJ = k;
            }
        }
    }

    // Rebar
    if ( AnevD.bRebar )
    {
        if ( AnevD.Rebar.bTension )
        {
            rParmD.bTensRebar = TRUE;
            rParmD.dFyat = AnevD.Rebar.dFyat;
            rParmD.ddsat = AnevD.Rebar.ddsat;
        }
        if ( AnevD.Rebar.bShear )
        {
            rParmD.bShearRebar = TRUE;
            rParmD.dfyas[0] = AnevD.Rebar.dFyasL;
            rParmD.ddsas[0] = AnevD.Rebar.ddsasL;
            rParmD.nNas[0]  = static_cast<double>(AnevD.Rebar.nNasL);

            rParmD.dfyas[1] = AnevD.Rebar.dFyasT;
            rParmD.ddsas[1] = AnevD.Rebar.ddsasT;
            rParmD.nNas[1]  = static_cast<double>(AnevD.Rebar.nNasT);
        }
    }

    return TRUE;
}

double CBrdgSeisEvalKISTEC2015::Get_BearingNumb(const EN_LOCAL_DIR& enDir, const int nBI, const int nBJ, const int enBearing, const T_ANEV_D& AnevD, const T_BREV_D& BrevD, const CString strBearProp)
{
    T_BRPR_D BrprD;

    UINT nBearing = 0;
//     if ( enBearing == BEARING_ELAST )
//     {
//         // consider all if Elastic bearing
//         nBearing = BrevD.n2wFixed + BrevD.n2wMovable + BrevD.n1wLMovable + BrevD.n1wTMovable;
//     }
//     else
    {
        CMap<UINT, UINT, UINT, UINT> mapExistBearingParaAll; mapExistBearingParaAll.InitHashTable(HASHSIZEELEM);
        //T_ELEM_K btpkTempAll;

     //   if ( AnevD.bSuppType )
     //   {


     ////       INT_PTR nLayer = AnevD.SuppType.aLayer.GetSize();
     ////       for ( INT_PTR j=0; j<nLayer; ++j )
     ////       {
     ////           INT_PTR nSupp = AnevD.SuppType.aLayer[j].aBrevType.GetSize();
     ////           for ( INT_PTR k=0; k<nSupp; ++k )
     ////           {
					////T_BRPR_K BrprK = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, j, k);

     ////               if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(BrprK, BrprD) ) BrprD.Initialize();
					////
     ////               if ( !mapExistBearingParaAll.Lookup(BrprK, btpkTempAll) )
     ////               {
     ////                   mapExistBearingParaAll.SetAt(BrprK, btpkTempAll);
     ////               }
     ////               else
     ////               {

     ////               }
     ////           }
     ////       }
     //   }
     //   else
     //   {
     //       switch ( enDir )
     //       {
     //       case LONGI:
     //           nBearing = BrevD.n2wFixed + BrevD.n1wTMovable;
     //           break;
     //       case TRANS:
     //           nBearing = BrevD.n2wFixed + BrevD.n1wLMovable;
     //           break;
     //       default:
     //           break;
     //       }
     //   }

		
        switch(AnevD.SuppType.aLayer[AnevD.SuppType.aLayer.GetSize() - nBI - 1].aBrevType[nBJ])    //( AnevD.SuppType.aLayer[nBI].aBrevType[nBJ] )
        {
        case BEARING_SUPPORT_FX: // 고정단
            nBearing++;
            break;
        case BEARING_SUPPORT_MS: // 양방향 가동단
			if (enBearing == BEARING_ELAST) nBearing++;
            break;
        case BEARING_SUPPORT_LS: // 교축 가동단
			if (enBearing == BEARING_ELAST) nBearing++;
			else
			{
				if (enDir == TRANS) nBearing++;
			}
            break;
        case BEARING_SUPPORT_TS: // 교직 가동단
			if (enBearing == BEARING_ELAST) nBearing++;
			else
			{
				if (enDir == LONGI) nBearing++;
			}
            break;
		case -1:
			break;
        default:
            ASSERT(0);
            break;
        }


    }

    return static_cast<double>(nBearing);
}


void CBrdgSeisEvalKISTEC2015::Calc_BearingBody(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
    T_ANEV_D AnevD;
    T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);
    if ( AnevK==0 ) ASSERT(0);

    double dTheta=AnevD.dTheta_q2;
    BOOL bRoundSect = FALSE;
    Get_Fn(BrevK, dTheta, rResD.BodyD[0].HorD.dFn, rResD.BodyD[1].HorD.dFn, bRoundSect); // 다주식은 평균값
    rResD.BodyD[0].HorD.bRoundSect = bRoundSect;
    rResD.BodyD[1].HorD.bRoundSect = bRoundSect;

    const double dRad = rResD.ParamD.dRelAngle * CMathFunc::m_trrad;
    const double dCos = fabs(cos(dRad));
    const double dSin = fabs(sin(dRad));

    const double dFnLCos = rResD.BodyD[0].HorD.dFn * dCos;
    const double dFnTSin = rResD.BodyD[1].HorD.dFn * dSin;
    const double dFnTCos = rResD.BodyD[1].HorD.dFn * dCos;
    const double dFnLSin = rResD.BodyD[0].HorD.dFn * dSin;

    if (bRoundSect)
    {
        rResD.BodyD[0].HorD.dFnAng = rResD.BodyD[0].HorD.dFn;
        rResD.BodyD[1].HorD.dFnAng = rResD.BodyD[1].HorD.dFn;
    }
    else
    {
		rResD.BodyD[0].HorD.dFnAng = dFnLCos + dFnTSin;
		rResD.BodyD[1].HorD.dFnAng = dFnTCos + dFnLSin;
    }

    const int iAnchorParaSize = rResD.AnchorParamAll.GetSize();

    T_BREV_D BrevD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) BrevD.Initialize();

	T_EVGP_D EvgpD;
	if (!m_pDoc->m_pAttrCtrl2->GetEvgp(BrevD.EvgpK, EvgpD)) return;

    for ( int i=0; i<2; ++i ) // Longitudinal, Transverse
    {
        T_BRDGEVAL_BEARING_BODY &BodyD = rResD.BodyD[i];

        BodyD.bCheck = TRUE;
        if ( i==0 )
        {
            BodyD.enDir    = LONGI;
            BodyD.HorD.dVe = rResD.ParamD.ForceD.dForce[2];
        }
        else
        {
            BodyD.enDir    = TRANS;
            BodyD.HorD.dVe = rResD.ParamD.ForceD.dForce[1];
        }
    }

    const double dVELCos = rResD.BodyD[0].HorD.dVe * dCos;
    const double dVETSin = rResD.BodyD[1].HorD.dVe * dSin;
    const double dVETCos = rResD.BodyD[1].HorD.dVe * dCos;
    const double dVELSin = rResD.BodyD[0].HorD.dVe * dSin;

    rResD.BodyD[0].HorD.dVeAng = dVELCos + dVETSin;
    rResD.BodyD[1].HorD.dVeAng = dVETCos + dVELSin;

    for ( int j=0; j<2; ++j )
    {
        T_BRDGEVAL_BEARING_BODY &BodyD = rResD.BodyD[j];

        double dFBCsum = 0.0;
        double dDispMaxL = 0.0;
        double dDispMaxT = 0.0;

		int enBearingTemp = 0;
        for (int x=0; x<rResD.AnchorParamAll.GetSize(); x++ )
        {
            double dFBC = 0.0;
            if (j == 0) // Longitudinal, Transverse
            {
                dFBC = rResD.AnchorParamAll[x].dCapaL * rResD.AnchorParamAll[x].dnBL;
				if(rResD.ParamD.ForceD.LinkL == rResD.AnchorParamAll[x].nLinKOrg)
				{
					dDispMaxL = rResD.AnchorParamAll[x].dDispL;
				}
            }
            else
            {
                dFBC = rResD.AnchorParamAll[x].dCapaT * rResD.AnchorParamAll[x].dnBT;
				if (rResD.ParamD.ForceD.LinkT == rResD.AnchorParamAll[x].nLinKOrg)
				{
					dDispMaxT = rResD.AnchorParamAll[x].dDispT;
				}

            }

			if(rResD.AnchorParamAll[x].nBearing == BEARING_ELAST) 
				enBearingTemp++;

            dFBCsum += dFBC;
        }

        BodyD.HorD.dFBC = dFBCsum;

		if (EvgpD.nType == 1) // 0:Pier, 1:Abutment
		{
			BodyD.HorD.dFBD = BodyD.HorD.dVeAng;
		}
		else
		{
    		BodyD.HorD.dFBD = min(BodyD.HorD.dVeAng, BodyD.HorD.dFnAng);
		}

        BodyD.HorD.dRat = SafeDiv(BodyD.HorD.dFBC, BodyD.HorD.dFBD, _INFI4);

        if (enBearingTemp>0)
        {
            BodyD.DispD.bCheck = TRUE;
            BodyD.DispD.dFBC = j==0 ? dDispMaxL : dDispMaxT;
            BodyD.DispD.dFBD = j==0 ? rResD.ParamD.ForceD.dDisp : rResD.ParamD.ForceD.dDispT;
            BodyD.DispD.dRat = SafeDiv(BodyD.DispD.dFBC, BodyD.DispD.dFBD, _INFI4);
        }
    }
}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
    //TODO khj0102, Bearing Property 가 여러개가 되므로 이 부분들은 전반적인 수정 필요
    Calc_BearingAnchor_Steel(BrevK, rResD);

	CDgnSeisAnchorShapeMgr::InitMapData4CalcShape();
    Calc_BearingAnchor_Concrete(BrevK, rResD);
    Calc_BearingAnchor_Pryout(BrevK, rResD);

    // Check anchor tesion
    rResD.AnchorD[0].bCheckN = FALSE;
    rResD.AnchorD[1].bCheckN = FALSE;
    if ( (rResD.ParamD.iEvalCode == KISTEC_2019 || rResD.ParamD.iEvalCode == KALIS_2023) && 
        rResD.ParamD.ForceD.dMaxP > 0.0 )
    {
        rResD.AnchorD[0].bCheckN = TRUE;
        rResD.AnchorD[1].bCheckN = TRUE;

        Calc_BearingAnchor_TenSteel(BrevK, rResD);
        Calc_BearingAnchor_TenConc (BrevK, rResD);
        Calc_BearingAnchor_TenPull (BrevK, rResD);
        Calc_BearingAnchor_TenSide (BrevK, rResD);
        Calc_BearingAnchor_TenRela (BrevK, rResD);
    }
}

double CBrdgSeisEvalKISTEC2015::Get_Vsamax(T_BRDGEVAL_BEARING& rResD)
{
    return DBL_MAX;
}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_Steel(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
    const int iMaxSize = rResD.AnchorParamAll.GetSize();

    for ( int i=0; i<2; ++i ) // Longitudinal, Transverse
    {
        double dVsaSum = 0.0;
        double dVsaParam = 0.0;
        double dAse  = 0.0;
        double dnA   = 0.0;
        double dfuta = 0.0;
        double dVsa  = 0.0;
        double dNumbBP = 0.0;
        double dFBD  = rResD.BodyD[i].HorD.dFBD;
		double dnn = 0.0;
        T_BRDGEVAL_BEARING_ANCHOR_FAS &FASD = rResD.AnchorD[i].FASD;
        for ( int j=0; j<iMaxSize; j++ )
        {
            T_BRDGEVAL_ANCHOR_PARAM& AnchorParam = rResD.AnchorParamAll[j];

            dVsaParam = (AnchorParam.enAnchType == ANCH_CAST_HEADED_STUD) ? 1.0 : 0.6;
            dAse      =  AnchorParam.dAse; // 포트받침인 경우 처리

			// 검토 되는 받침(고정단)
			if (i == 0) dnn = AnchorParam.dnBL;
			else        dnn = AnchorParam.dnBT;
			if (dnn == 0.0) continue;

            dnA       =  AnchorParam.nnL * AnchorParam.nnT;
            dfuta     =  AnchorParam.dfua;
            dVsa      =  dnA * dVsaParam * dAse * dfuta;

            dVsaSum += dVsa;
        }

        FASD.dFASC   = dVsaSum;
        FASD.dFASD   = dFBD;
        FASD.dRat    = SafeDiv(FASD.dFASC, FASD.dFASD, _INFI4);
        //// set for report
        //FASD.nnA   = dnA;
        //FASD.dAse  = dAse;
        //FASD.dfuta = dfuta;
        //FASD.dFBD  = dFBD;
    }
}

double CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_Concrete_Ratio(const T_BREV_K& BrevK, TConcFailBcCalc& MinConcFailCal, std::vector<std::pair<int, int>> aRepBearingIdx, T_BRDGEVAL_BEARING& rResD, const int& iDir, int nRatType[4])
{
    T_ANEV_D AnevD;
    T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);

    T_BREV_D BrevD;
    m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD);

    return  Calc_BearingAnchor_Concrete_Ratio(AnevD, BrevD, MinConcFailCal, aRepBearingIdx, rResD, iDir, nRatType);
}

double CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_Concrete_Ratio(T_ANEV_D& AnevD, T_BREV_D& BrevD, TConcFailBcCalc& MinConcFailCal,std::vector<std::pair<int, int>> aRepBearingIdx, T_BRDGEVAL_BEARING& rResD, const int& iDir, int nRatType[4])
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();	
	T_BRPR_D BrprD;
	std::vector<std::vector<T_BRPR_D>> aArBrprD;

	int nTotalBearingL = 0;
	int nTotalBearingT = 0;
	for (int i = 0; i < AnevD.BearingProp.aBrprKs.GetSize(); i++)
	{
		aArBrprD.push_back(std::vector<T_BRPR_D>());
		for (int j = 0; j < AnevD.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			if(AnevD.BearingProp.aBrprKs[i].aBrprK[j] > 0)
			{
				//if(!pDoc->m_pAttrCtrl2->GetBrpr(CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, i, j), BrprD)) ASSERT(0);
				if (!pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) ASSERT(0);
				aArBrprD[i].push_back(BrprD);
			}
			else
			{
				BrprD.Initialize();
				aArBrprD[i].push_back(BrprD);
			}

			//각 방향의 전열에서 고정인 BP 개수
			int nBrevType = AnevD.SuppType.aLayer[i].aBrevType[j];
			if (i == 0) nTotalBearingL += (nBrevType == 0 || nBrevType == 3) ? 1 : 0;    // 교축 고정단
			if (j == 0) nTotalBearingT += (nBrevType == 0 || nBrevType == 2) ? 1 : 0;    // 교직 고정단
		}
	}

    double dTheta=AnevD.dTheta_q2;
    double dFnL = 0.0;
    double dFnT = 0.0;
    BOOL bRoundSect = FALSE;
    Get_Fn(BrevD, dTheta, dFnL, dFnT, bRoundSect); // 다주식은 평균값

    CDgnSeisAnchorShapeMgr AnchorShapeMgr;
    AnchorShapeMgr.SetData(aArBrprD, AnevD);
	AnchorShapeMgr.SetDULR(nRatType[0], nRatType[1]);
	AnchorShapeMgr.SetPryoutDULR(nRatType[2], nRatType[3]);

	if(aArBrprD.size() == 0)
	{
		ASSERT(0);
		return 0.0;
	}
	BrprD = aArBrprD[0][0];

    int k = 0;

    if ( iDir == TRANS ) k = 1;
    
    T_BRDGEVAL_ANCHOR_PARAM_LIST AnchorParam;
    
    int nBearingPropNumb = 0;

    //AnchorShapeMgr.GetConcfailRepresentGrpBearing(iDir, aRepBearingIdx);

    nBearingPropNumb = (int)aRepBearingIdx.size();

    AnchorParam.SetSize(nBearingPropNumb);

    double dCa1 = 0.0;
    double dCa1P = 0.0;
    double dCa2 = 0.0;
    double dCa2P = 0.0;

    std::vector<TConcFailBcCalc> aConcFailBc;
	//AnchorShapeMgr.GetConcFailBc(iDir, aConcFailBc);
	aConcFailBc.push_back(MinConcFailCal);

    for ( int i = 0; i < aRepBearingIdx.size(); i++ )
    {
        int nBrI = aRepBearingIdx[i].first;
        int nBrJ = aRepBearingIdx[i].second;        
        T_ELEM_K BrprK = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, nBrI, nBrJ);		

        T_BRPR_D BrprD;
        if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(BrprK, BrprD) ) BrprD.Initialize();
        double dCa1Re = 0.0;
        AnchorShapeMgr.GetConcFailBearingCa(iDir, nBrI, nBrJ, dCa1, dCa1P, dCa2, dCa2P);
        AnchorParam[i].dCa1 = dCa1;
        AnchorParam[i].dCa1P = dCa1P;
        AnchorParam[i].dCa2 = dCa2;
        AnchorParam[i].dCa2P = dCa2P;
        AnchorParam[i].dCa1Re = dCa1;
        if ( AnchorShapeMgr.GetReCalcCa1(iDir, nBrI, nBrJ, dCa1Re, aConcFailBc) ) AnchorParam[i].dCa1 = dCa1Re;
        ConvertAnchorParam(nBrI, nBrJ, BrevD, AnevD, BrprD, rResD.ParamD, AnchorParam[i]);
    }

    BOOL bTypical= IsTypicalBearingArrange(AnevD, AnevD.dRelAngle);

    if ( AnchorParam.GetSize() > 1 )
    {
        bTypical = FALSE;
    }

    double dsax    = rResD.ParamD.dSax;
    double dsay    = rResD.ParamD.dSay;
    double dhcop   = rResD.ParamD.dhcop;
    double dfck    = rResD.ParamD.dfck;

    if ( !bTypical )
    {
        AnchorShapeMgr.GetConcFailSax(LONGI, dsax);
        AnchorShapeMgr.GetConcFailSax(TRANS, dsay);
    }

    const int iAnchorParaSize = AnchorParam.GetSize();

    double dVbSum   = 0.0;
    double dAvcoSum = 0.0;
    double dca1    = 0.0;
    double dca1p   = 0.0;
    double dca2    = 0.0;
    double dca2p   = 0.0;
    double d15ca1  = 0.0;
    double nBC     = 1.0;
    double nBC_org = 1.0;
    double dAvc    = 0.0;
    double dAvco   = 0.0;
    double dLp     = 0.0;
    double dpsi_edv = 1.0;

    double nn = AnchorShapeMgr.GetConcFailCa1AnchorNum(iDir, aRepBearingIdx);

    // Set Data Typical인 경우에만 사용
    double ds1     = iDir==LONGI ? AnchorParam[0].dS1 : AnchorParam[0].dS2;
    double ds2     = iDir==LONGI ? AnchorParam[0].dS2 : AnchorParam[0].dS1;
    double dha     = 0.0;
    double dnA     = iDir==LONGI ? AnchorParam[0].nnL : AnchorParam[0].nnT;

	if(rResD.ParamD.bUserConc)
	{
		const T_ANEV_CONC_FAIL_D& FailD = iDir == LONGI ? AnevD.Fail.ConcFailL : AnevD.Fail.ConcFailT;

		dca1 = FailD.dCa1;
		dca1p = FailD.dCa1_q;
		dca2 = FailD.dCa2;
		dca2p = FailD.dCa2_q;
		dha = FailD.dHa;
		nBC = static_cast<double>(FailD.nNum);
		dAvc = FailD.dAVc;
		dAvco = FailD.dAVco;
	}
	else
	{
		if(bTypical)
		{
			Get_ArrangeEdgeDimsConc(AnevD, (iDir == LONGI ? LONGI : TRANS), dca1, dca1p, dca2, dca2p);
		}
		else
		{
			int nBCnum = 0;
			AnchorShapeMgr.GetConcFailRepresentVal4Ratio(k, dLp, nBCnum, dca1, dca1p, dca2, dca2p, MinConcFailCal);
			nBC = static_cast<double>(nBCnum);
		}
	}

	int nTotalBearingNumb = nTotalBearingNumb = iDir == LONGI ? nTotalBearingL : nTotalBearingT;

    for ( int j=0; j<iAnchorParaSize; j++ )
    {
        double dhef    = AnchorParam[j].dhef;
        double da      = AnchorParam[j].da;

        d15ca1 = 1.5*dca1;
        if ( !rResD.ParamD.bUserConc ) dha = min(dhcop, d15ca1);
        double dca1org = dca1;

        int n1AnchorType = RESIS_MULTIANCHOR;
        // 투영길이 계산 - 장호
        int nAvcType = 0;
        if ( bTypical )
        {
            // 단일 앵커 및 앵커 그룹 판단
            BOOL bInter_s1  = (fabs(ds1)<2.0*d15ca1 ? TRUE : FALSE);
            BOOL bInter_s2  = (fabs(ds2)<2.0*d15ca1 ? TRUE : FALSE);
            BOOL bInter_sax = FALSE;
            if ( iDir==LONGI ) bInter_sax = (fabs(dsax)<2.0*d15ca1 ? TRUE : FALSE); // 교축
            else     bInter_sax = (fabs(dsay)<2.0*d15ca1 ? TRUE : FALSE); // 교축직각

            if ( fabs(ds1)<cDgn_Zero && fabs(ds2)<cDgn_Zero && !bInter_sax )
            {
                n1AnchorType = RESIST_1ANCHOR_1;
            }
            else if ( !rResD.ParamD.bUserConc )
            {
                if ( bInter_s1 && bInter_sax )
                {
                    nBC = iDir==LONGI? rResD.ParamD.nnBCL : rResD.ParamD.nnBCT;
                }
                else if ( fabs(ds1)<cDgn_Zero )
                {
                    n1AnchorType = RESIST_1ANCHOR_ALL;
                }
            }
            if ( !rResD.ParamD.bUserConc )
            {
                // 투영길이 계산
                dAvc = Get_Avc(dca1, dca2, dca2p, ds1, (iDir==LONGI ? dsax : dsay), nBC, dnA, dha, nAvcType, dLp);
            }
        }
        else if ( !rResD.ParamD.bUserConc )
        {
            nAvcType = 0;
            dAvc     = dLp * dha;
        }

        nBC_org = nBC;
        int n1AnchorType_org = n1AnchorType;
        // 콘크리트파괴 저항면적 상정시 3면 이상 가장자리의 영향 검토
        BOOL bChk2face = (dLp < 2.0*d15ca1) ? TRUE : FALSE;
        BOOL bEdgeEff  = Check_Inter3Faces(dca1, dca2, dca2p, dhcop, (iDir==LONGI ? dsax : dsay), dLp);

        if ( iDir==LONGI ) bEdgeEff = FALSE;

        if ( bEdgeEff && (fabs(dca1org-dca1) > cDgn_Zero) )
        {
            if ( !rResD.ParamD.bUserConc )
            {
                // 3면이 가장자리의 영향을 받으므로 하중작용방향으로 연단거리 재산정 필요 - Check_Inter3Faces 재산정함.
                d15ca1 = 1.5*dca1;

                if ( bTypical )
                {
                    // 단일 앵커 및 앵커 그룹 판단
                    BOOL bInter_s1  = (fabs(ds1)<2.0*d15ca1 ? TRUE : FALSE);
                    BOOL bInter_s2  = (fabs(ds2)<2.0*d15ca1 ? TRUE : FALSE);
                    BOOL bInter_sax = FALSE;
                    if ( iDir==LONGI ) bInter_sax = (fabs(dsax)<2.0*d15ca1 ? TRUE : FALSE); // 교축
                    else     bInter_sax = (fabs(dsay)<2.0*d15ca1 ? TRUE : FALSE); // 교축직각

                    if ( fabs(ds1)<cDgn_Zero && fabs(ds2)<cDgn_Zero && !bInter_sax )
                    {
                        n1AnchorType = RESIST_1ANCHOR_1;
                    }
                    else
                    {
                        n1AnchorType = RESIS_MULTIANCHOR;
                        if ( bInter_s1 && bInter_sax )
                        {
                            nBC = iDir==LONGI ? rResD.ParamD.nnBCL : rResD.ParamD.nnBCT;
                        }
                        else
                        {
                            if ( fabs(ds1)<cDgn_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                            nBC = 1.0;
                        }
                    }

                    // 투영길이 계산
                    dAvc = Get_Avc(dca1, dca2, dca2p, ds1, (iDir==LONGI ? dsax : dsay), nBC, dnA, dha, nAvcType, dLp);
                }
                else
                {
                    double dca1_recal = dca1;
                    int nBCnum=0;
					AnchorShapeMgr.GetConcFailRepresentVal4Ratio(k, dLp, nBCnum, dca1, dca1p, dca2, dca2p, MinConcFailCal);

                    nAvcType = 0;
                    nBC  = static_cast<double>(nBCnum);
                    dAvc = dLp * dha;
                }
            }
            else
            {
                dca1 = dca1org;
            }
        }

        //여기에서는 각 Bearing Property의 Ca 값들 필요
        if ( !rResD.ParamD.bUserConc )
        {
            dAvco = 4.5 * AnchorParam[j].dCa1Re * AnchorParam[j].dCa1Re;
        }

        dAvcoSum += dAvco;
        double dAvcLim = nn * dAvco;

        if ( dAvc > dAvcLim )
        {
            dAvc = dAvcLim;
        }

		double dca2min = min(dca2, dca2p);
        if (dca2min < d15ca1 )
        {
            dpsi_edv = 0.7 + 0.3*dca2min /d15ca1;
        }

        const double dVb = Calc_Vb(AnchorParam[j].dVbParam, AnchorParam[j].dle, AnchorParam[j].dap, dfck, dca1);
        dVbSum += dVb;
    }

    double dVbmax = Calc_Vbmax(dfck, dca1);
    double dVbAvg = SafeDiv(dVbSum, nBC);
    double dVb    = min(dVbSum, dVbmax) * nBC;
    double dPuD  = rResD.ParamD.ForceD.dPtopBearing;
    double dpsi_cv = Get_psi_cV(AnevD); // KALIS2023 사용자 입력값 rResD.ParamD.dpsi_cv;
    double dVcbg = fabs(dAvco)<cDgn_Zero ? 0.0 : (dAvc/dAvco) * dpsi_edv * dpsi_cv * dVb;
    double dFACC = Calc_FACC(dVcbg, dPuD);

    BOOL   bAnchorRebar = rResD.ParamD.bShearRebar;
    double dnas  = rResD.ParamD.nNas[k];
    double dAsas = rResD.ParamD.ddsas[k]* rResD.ParamD.ddsas[k]*CMathFunc::m_pi/4.0;
    double dfya  = rResD.ParamD.dfyas[k];
    double dVasv = 0.0;
    if ( bAnchorRebar )
    {
        dVasv = dnas * 1.0 * dAsas * dfya;
        dFACC = max(dFACC, dVasv);
    }

    double dFACD = rResD.AnchorD[iDir].FASD.dFASD * nBC / nTotalBearingNumb;

    return SafeDiv(dFACC, dFACD, _INFI4);
}

double CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_Pryout_Ratio(const T_BREV_K& BrevK, int nRepresentGrpIdx, std::vector<std::pair<int, int>> aRepBearingIdx, T_BRDGEVAL_BEARING& rResD, const int& iDir, int nRatType[4], BOOL bAnchorTenConc/* = FALSE*/)
{
    T_ANEV_D AnevD;
    T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);

    T_BREV_D BrevD;
    m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD);

    return  Calc_BearingAnchor_Pryout_Ratio(AnevD, BrevD, nRepresentGrpIdx, aRepBearingIdx, rResD, iDir, nRatType, bAnchorTenConc);
}

double CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_Pryout_Ratio(T_ANEV_D& AnevD, T_BREV_D& BrevD, int nRepresentGrpIdx, std::vector<std::pair<int, int>> aRepBearingIdx4Ratio, T_BRDGEVAL_BEARING& rResD, const int& iDir, int nRatType[4], BOOL bAnchorTenConc/* = FALSE*/)
{
    double dTheta=AnevD.dTheta_q2;
    double dFnL = 0.0;
    double dFnT = 0.0;
    BOOL bRoundSect = FALSE;
    Get_Fn(BrevD, dTheta, dFnL, dFnT, bRoundSect); // 다주식은 평균값

	CDBDoc* pDoc = CDBDoc::GetDocPoint();	
	T_BRPR_D BrprD;

	int nTotalAnchorL = 0;
	int nTotalAnchorT = 0;
	std::vector<std::vector<T_BRPR_D>> aArBrprD;
	for (int i = 0; i < AnevD.BearingProp.aBrprKs.GetSize(); i++)
	{
		aArBrprD.push_back(std::vector<T_BRPR_D>());
		for (int j = 0; j < AnevD.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			if(AnevD.BearingProp.aBrprKs[i].aBrprK[j]>0)
			{
				//if(!pDoc->m_pAttrCtrl2->GetBrpr(CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, i, j), BrprD)) ASSERT(0);
				if (!pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) ASSERT(0);
				aArBrprD[i].push_back(BrprD);
			}
			else
			{
				BrprD.Initialize();
				aArBrprD[i].push_back(BrprD);
			}

			//각 방향의 전열에서 고정인 BP
			//각 BP에서 프아이아웃 검토용 앵커 배열 합
			int nBrevType = AnevD.SuppType.aLayer[i].aBrevType[j];
			nTotalAnchorL += (nBrevType == 0 || nBrevType == 3) ? BrprD.nN1 : 0;
			nTotalAnchorT += (nBrevType == 0 || nBrevType == 2) ? BrprD.nN2 : 0;
		}
	}

    CDgnSeisAnchorShapeMgr AnchorShapeMgr;
	AnchorShapeMgr.SetData(aArBrprD, AnevD);
	AnchorShapeMgr.SetDULR(nRatType[0], nRatType[1]);
	AnchorShapeMgr.SetPryoutDULR(nRatType[2], nRatType[3]);

	AnchorShapeMgr.SetAnchorTenConcOption(bAnchorTenConc);
	AnchorShapeMgr.UpdatehefAuto();
	AnchorShapeMgr.SetRepresentGrpIdx4Ratio(nRepresentGrpIdx);
	AnchorShapeMgr.SetRatioChk(TRUE);

	if(aArBrprD.size() == 0)
	{
		ASSERT(0);
		return 0.0;
	}
	BrprD = aArBrprD[0][0];

    int i = 0;

    if ( iDir == TRANS ) i = 1;

    T_BRDGEVAL_ANCHOR_PARAM_LIST AnchorParam;

    int nBearingPropNumb = 0;

 //AnchorShapeMgr.GetConcfailRepresentGrpBearing(iDir, aRepBearingIdx);

    nBearingPropNumb = (int)aRepBearingIdx4Ratio.size();

    AnchorParam.SetSize(nBearingPropNumb);

    std::vector<TConcFailBcCalc> aConcFailBc;
    AnchorShapeMgr.GetConcFailBc(iDir, aConcFailBc);

    double dCa1 = 0.0;
    double dCa1P = 0.0;
    double dCa2 = 0.0;
    double dCa2P = 0.0;

    for ( int i = 0; i < aRepBearingIdx4Ratio.size(); i++ )
    {
        int nBrI = aRepBearingIdx4Ratio[i].first;
        int nBrJ = aRepBearingIdx4Ratio[i].second;
        T_ELEM_K BrprK = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, nBrI, nBrJ);		

        T_BRPR_D BrprD;
        if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(BrprK, BrprD) ) BrprD.Initialize();
        AnchorShapeMgr.GetPryoutBearingCa(iDir, nBrI, nBrJ, dCa1, dCa1P, dCa2, dCa2P);
        AnchorParam[i].dCa1 = dCa1;
        AnchorParam[i].dCa1P = dCa1P;
        AnchorParam[i].dCa2 = dCa2;
        AnchorParam[i].dCa2P = dCa2P;
        AnchorParam[i].dhefRe = BrprD.dhef;
        double dHefRe = 0.0;
        if ( AnchorShapeMgr.GetReCalcHef(iDir, nBrI, nBrJ, dHefRe) ) AnchorParam[i].dhefRe = dHefRe;
        ConvertAnchorParam(nBrI, nBrJ, BrevD, AnevD, BrprD, rResD.ParamD, AnchorParam[i]);
    }

    BOOL bTypical= IsTypicalBearingArrange(AnevD, AnevD.dRelAngle);

    if ( AnchorParam.GetSize() > 1 )
    {
        bTypical = FALSE;
    }

    double dsax    = rResD.ParamD.dSax;
    double dsay    = rResD.ParamD.dSay;
    double dhcop   = rResD.ParamD.dhcop;
    double dfck    = rResD.ParamD.dfck;

	// ratio 계산시 skip
//     if ( !bTypical )
//     {
//         double dsax_temp=0.0, dsay_temp=0.0;
// 		if(iDir== LONGI)
// 			AnchorShapeMgr.GetPryoutSaxSay(LONGI, dsax, dsay_temp);
// 		else 
// 			AnchorShapeMgr.GetPryoutSaxSay(TRANS, dsay, dsax_temp);
//     }

    const int iMaxSize = AnchorParam.GetSize();

    double dca1    = 0.0;
    double dca1p   = 0.0;
    double dca2    = 0.0;
    double dca2p   = 0.0;

    double nBC     = 1.0;
    double nBC_org = 1.0;

    double dANc  = 0.0;
    double dANco = 0.0;

    double dANcoSum = 0.0;

    double dcamin = 0.0;

    double d15hef = 0.0;
    double dhef = 0.0;
    double dhefSum = 0.0;

    int n1AnchorType_org = 0;
    int n1AnchorType = 0;
    int  n1AnchorAllResist = 0;
    double dNbSum = 0.0;

    double nn = AnchorShapeMgr.GetPryoutCa1AnchorNum(iDir, aRepBearingIdx4Ratio);

	// Set Data 밑의 값들은 Typical인 경우에만 사용
// 	double ds1 = iDir == LONGI ? rResD.AnchorParamPryLongi[0].dS1 : rResD.AnchorParamPryTrans[0].dS2;
// 	double ds2 = iDir == LONGI ? rResD.AnchorParamPryLongi[0].dS2 : rResD.AnchorParamPryTrans[0].dS1;
// 	double dnA = iDir == LONGI ? rResD.AnchorParamPryLongi[0].nnL : rResD.AnchorParamPryTrans[0].nnT;
// 	double dnB = iDir == LONGI ? rResD.AnchorParamPryLongi[0].nnT : rResD.AnchorParamPryTrans[0].nnL;

	// Set Data 밑의 값들은 Typical인 경우에만 사용
	double ds1 = i == 0 ? AnchorParam[0].dS1 : AnchorParam[0].dS2;
	double ds2 = i == 0 ? AnchorParam[0].dS2 : AnchorParam[0].dS1;
	double dnA = i == 0 ? AnchorParam[0].nnL : AnchorParam[0].nnT;
	double dnB = i == 0 ? AnchorParam[0].nnT : AnchorParam[0].nnL;

	double dANcOne, dca1One, dca1pOne, dca2One, dca2pOne;
	dANcOne = dca1One = dca1pOne = dca2One = dca2pOne = 0.0;
	BOOL bSingleResistOne = FALSE;
	int nBCnumOne =0;
	int n1AnchorAllResistOne=0;
	BOOL bFirst = TRUE;
    for ( int j=0; j<iMaxSize; j++ )
    {
        dhef  = AnchorParam[j].dhefRe;

        double nnBCL = rResD.ParamD.nnBCL;
        double nnBCT = rResD.ParamD.nnBCT;
        if ( static_cast<int>(dnB)==1 )
        {
            if ( i==0 )
            {
                if ( nnBCT-cDGN_Zero > 1.0 ) // 가력방향 반대편 앵커열의 앵커가 1개인 경우 제외
                {
                    nnBCT -= 1.0;
                }
            }
            else
            {
                if ( nnBCL-cDGN_Zero > 1.0 ) // 가력방향 반대편 앵커열의 앵커가 1개인 경우 제외
                {
                    nnBCL -= 1.0;
                }
            }
        }

        n1AnchorType = RESIS_MULTIANCHOR;
        n1AnchorAllResist = 0;
        if (rResD.ParamD.bUserPryOut )
        {
            const T_ANEV_PRYOUT_D& PryoutD = i==0 ? AnevD.Fail.PryoutL : AnevD.Fail.PryoutT;

            dca1   = PryoutD.dCa1;
            dca1p  = PryoutD.dCa1_q;
            dca2   = PryoutD.dCa2;
            dca2p  = PryoutD.dCa2_q;

            nBC    = static_cast<double>(PryoutD.nNum);
            dANc   = PryoutD.dANc;
            dANco  = PryoutD.dANco;
            dhef   = PryoutD.dHef;
        }
        else if ( bTypical )
        {
            Get_ArrangeEdgeDimsPryo(AnevD, (i==0?LONGI:TRANS), dca1, dca1p, dca2, dca2p);
        }
        else
        {
            int nBCnum=0;
            BOOL bSingleResist = FALSE;
			if(bFirst)
			{
				AnchorShapeMgr.GetPryoutRepresentVal(i, aRepBearingIdx4Ratio, dANc, nBCnum, dca1, dca1p, dca2, dca2p, bSingleResist, n1AnchorAllResist);

				dANcOne = dANc;
				nBCnumOne = nBCnum;
				dca1One = dca1;
				dca1pOne = dca1p;
				dca2One = dca2;
				dca2pOne = dca2p;
				bSingleResistOne = bSingleResist;
				n1AnchorAllResistOne = n1AnchorAllResist;

				bFirst = FALSE;
			}
			else
			{
				dANc = dANcOne;
				nBCnum= nBCnumOne;
				dca1= dca1One;
				dca1p= dca1pOne;
				dca2= dca2One;
				dca2p= dca2pOne;
				bSingleResist= bSingleResistOne;
				n1AnchorAllResist= n1AnchorAllResistOne;
			}
            if ( bSingleResist && n1AnchorAllResist>0 )
            {
                n1AnchorType = n1AnchorAllResist==1 ? RESIST_1ANCHOR_1 : RESIST_1ANCHOR_ALL;
            }
            nBC  = static_cast<double>(nBCnum);
        }
        d15hef  = 1.5*dhef;

        int    nANcType = 0;
        // 단일 앵커 및 앵커 그룹 판단
        if ( bTypical )
        {
            BOOL bInter_s1  = (fabs(ds1)<2.0*d15hef ? TRUE : FALSE);
            BOOL bInter_sax = FALSE;
            if ( i==0 ) bInter_sax = (fabs(dsax)<2.0*d15hef ? TRUE : FALSE); // 교축
            else     bInter_sax = (fabs(dsay)<2.0*d15hef ? TRUE : FALSE); // 교축직각
            BOOL bInter_s1say = FALSE;
            if ( i==0 ) bInter_s1say = fabs(ds2*(dnB-1.0)+dsay)<2.0*d15hef ? TRUE : FALSE; // 교축
            else     bInter_s1say = fabs(ds2*(dnB-1.0)+dsax)<2.0*d15hef ? TRUE : FALSE; // 교축직각

            if ( fabs(ds1)<cDgn_Zero && fabs(ds2)<cDgn_Zero && !bInter_sax )
            {
                n1AnchorType = RESIST_1ANCHOR_1;
            }
            else if ( !rResD.ParamD.bUserPryOut )
            {
                if ( bInter_s1 && bInter_sax && bInter_s1say )
                {
                    nBC = nnBCL * nnBCT;
                }
                else if ( bInter_s1 && bInter_sax )
                {
                    nBC = i==0 ? nnBCL : nnBCT;

                    if ( static_cast<int>(nBC) == 1 )
                    {
                        if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                    }
                }
                else if ( bInter_s1 && bInter_s1say )
                {
                    nBC = i==0 ? nnBCT : nnBCL;

                    if ( static_cast<int>(nBC) == 1 )
                    {
                        if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                    }
                }
                else if ( bInter_s1 && bInter_s1say )
                {
                    nBC = i==0 ? nnBCT : nnBCL;

                    if ( static_cast<int>(nBC) == 1 )
                    {
                        if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                    }
                }
                else
                {
                    nBC = 1.0;
                    if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                }

                if ( n1AnchorType==RESIST_1ANCHOR_ALL )
                    n1AnchorAllResist = i==0 ? static_cast<int>(rResD.ParamD.nnBCL) : static_cast<int>(rResD.ParamD.nnBCT);
            }
        }
        nBC_org = nBC;

        n1AnchorType_org = n1AnchorType;
        double dhef_org = dhef;
        BOOL bEdgeEff  = Check_Inter4Faces(dhef, i, dca1, dca1p, dca2, dca2p, dsax, dsay, dnB, ds1, ds2);
        if ( bEdgeEff && fabs(dhef-dhef_org) > cDgn_Zero )
        {
            if ( fabs(dhef-dhef_org) > cDgn_Zero )
            {
                d15hef  = 1.5*dhef;
                // 단일 앵커 및 앵커 그룹 판단
                BOOL bInter_s1  = fabs(ds1)<2.0*d15hef ? TRUE : FALSE;
                BOOL bInter_sax = FALSE;
                if ( i==0 ) bInter_sax = fabs(dsax)<2.0*d15hef ? TRUE : FALSE; // 교축
                else     bInter_sax = fabs(dsay)<2.0*d15hef ? TRUE : FALSE; // 교축직각
                BOOL bInter_s1say = FALSE;
                if ( i==0 ) bInter_s1say = fabs(ds2*(dnB-1.0)+dsay)<2.0*d15hef ? TRUE : FALSE; // 교축
                else     bInter_s1say = fabs(ds2*(dnB-1.0)+dsax)<2.0*d15hef ? TRUE : FALSE; // 교축직각

                if ( bTypical )
                {
                    if ( fabs(ds1)<cDgn_Zero && fabs(ds2)<cDgn_Zero && !bInter_sax )
                    {
                        n1AnchorType = RESIST_1ANCHOR_1;
                    }
                    else
                    {
                        if ( bInter_s1 && bInter_sax && bInter_s1say )
                        {
                            int nBCL = i==0 ? static_cast<int>(rResD.ParamD.nnBCL) : static_cast<int>(rResD.ParamD.nnBCT);
                            int nBCT = i==0 ? static_cast<int>(rResD.ParamD.nnBCT) : static_cast<int>(rResD.ParamD.nnBCL);
                            if ( nBCT > 1 && static_cast<int>(dnB)==1 ) // 가력방향 반대편 앵커열의 앵커가 1개인 경우 제외
                            {
                                nBCT -= 1;
                            }
                            nBC = static_cast<double>(nBCL * nBCT);
                        }
                        else if ( bInter_s1 && bInter_sax )
                        {
                            nBC = i==0 ? nnBCL : nnBCT;
                            if ( static_cast<int>(nBC) == 1 )
                            {
                                if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                            }
                        }
                        else if ( bInter_s1 && bInter_s1say )
                        {
                            nBC = i==0 ? nnBCT : nnBCL;
                            if ( static_cast<int>(nBC) == 1 )
                            {
                                if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                            }
                        }
                        else
                        {
                            nBC = 1.0;
                            if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                        }

                        if ( n1AnchorType==RESIST_1ANCHOR_ALL )
                            n1AnchorAllResist = i==0 ? static_cast<int>(rResD.ParamD.nnBCL) : static_cast<int>(rResD.ParamD.nnBCT);
                    }
                }
            }
            else
            {
                dhef = dhef_org;
            }
        }
        // 투영면적
        if ( !rResD.ParamD.bUserPryOut )
        {
            if ( bTypical )
            {
                int nBCL = i==0 ? static_cast<int>(rResD.ParamD.nnBCL) : static_cast<int>(rResD.ParamD.nnBCT);
                int nBCT = i==0 ? static_cast<int>(rResD.ParamD.nnBCT) : static_cast<int>(rResD.ParamD.nnBCL);
                dANc = Get_Anc(dca1, dca1p, dca2, dca2p, dhef, ds1, ds2, (i==0 ? dsax : dsay), (i==0 ? dsay : dsax), nBCL, nBCT, dnA, dnB, nANcType);
            }
            else
            {
                int nBCnum=0;
                BOOL bSingleResist;
                int nResistAnchorNum;
				if (bFirst)
				{
					AnchorShapeMgr.GetPryoutRepresentVal(i, aRepBearingIdx4Ratio, dANc, nBCnum, dca1, dca1p, dca2, dca2p, bSingleResist, nResistAnchorNum);

					dANcOne = dANc;
					nBCnumOne = nBCnum;
					dca1One = dca1;
					dca1pOne = dca1p;
					dca2One = dca2;
					dca2pOne = dca2p;
					bSingleResistOne = bSingleResist;
					n1AnchorAllResistOne = nResistAnchorNum;

					bFirst = FALSE;
				}
				else
				{
					dANc = dANcOne;
					nBCnum = nBCnumOne;
					dca1 = dca1One;
					dca1p = dca1pOne;
					dca2 = dca2One;
					dca2p = dca2pOne;
					bSingleResist = bSingleResistOne;
					n1AnchorAllResist = n1AnchorAllResistOne;
				}
                nBC  = static_cast<double>(nBCnum);
            }
        }

        if ( !rResD.ParamD.bUserPryOut ) dANco = 9.0 * AnchorParam[j].dhefRe * AnchorParam[j].dhefRe;

        dANcoSum += dANco;
        double dANcLim = nn * dANco;

        double dANc_cal = dANc;
        if ( dANc > dANcLim )
        {
            dANc = dANcLim;
        }

        dNbSum += AnchorParam[j].dNb;

        dhefSum += AnchorParam[j].dhefRe;
    } // iMaxSize

    double dhefAvg = dhefSum / AnchorParam.GetSize();
    double d15hefAvg = 1.5 * dhefAvg;
    double dNb = dNbSum / AnchorParam.GetSize();

    dcamin = min(min(dca1, dca1p), min(dca2, dca2p));

    double dpsi_edN = 1.0;
    if ( dcamin < d15hefAvg )
    {
        dpsi_edN = (d15hef==0.0)? 0.0 : 0.7 + 0.3*dcamin/d15hef;
    }
    double dpsi_cN = Get_psi_cN(AnchorParam[0].enAnchType); // KALIS2023 사용자

    double dANcoAvg = SafeDiv(dANcoSum, nBC);

	double dhefAvgMM = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dhefAvg);;

    double dkcp = dhefAvgMM < 65.0 ? 1.0 : 2.0;
    double dNcbg = (dANc/dANcoAvg) * dpsi_edN * dpsi_cN * dNb;
    double dVcpg = dkcp * dNcbg;

    BOOL   bAnchorRebar = rResD.ParamD.bTensRebar;
    double dnas  = 2.0;
    double dAsat = rResD.ParamD.ddsat*rResD.ParamD.ddsat*CMathFunc::m_pi/4.0;
    double dfya  = rResD.ParamD.dFyat;
    double dVast = 0.0;
    if ( bAnchorRebar )
    {
        dVast = 1.0 * dnas * dAsat * dfya;
    }

    if ( n1AnchorType==RESIST_1ANCHOR_ALL && n1AnchorAllResist > 1 )
    {
        dVcpg *= static_cast<double>(n1AnchorAllResist);
    }

    double dFAPC = max(dVcpg, dVast);
	dFAPC = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_FORCE, dFAPC);

	int nTotalBearingNumb = 0;
	//if (!rResD.ParamD.bUserPryOut)
	{
		//nTotalBearingNumb = dnA * rResD.ParamD.nnBCL * rResD.ParamD.nnBCT;
		nTotalBearingNumb = (i == 0) ? nTotalAnchorL : nTotalAnchorT;
	}

	double dnA_New = nn; // (fabs(dANc - dANco) < cDgn_Zero) ? 1.0 : dnA;
	double dFAPD = rResD.AnchorD[iDir].FASD.dFASD * dnA_New / nTotalBearingNumb;

    return SafeDiv(dFAPC, dFAPD, _INFI4);
}

BOOL CBrdgSeisEvalKISTEC2015::Calc_BearingAnchorForDlg(int nFailType, T_ANEV_D& AnevD, T_BRDGEVAL_BEARING& rResD)
{
	rResD.initialize();
	if (!Get_BearingParam(AnevD.BrevK, rResD, FALSE, TRUE)) // 정방향 기준으로 데이터 생성 
	{
		ASSERT(0);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 비어있는 곳 채움.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	m_pDoc->m_pAttrCtrl2->GetModifiedAnevD(AnevD);
	//////////////////////////////////////////////////////////////////////////

	//T_ANEV_D AnevDR = AnevD;
	//ConvertAnevToNegativeDir(AnevDR);

	T_BREV_D BrevD;
	if (!m_pDoc->m_pAttrCtrl2->GetBrev(AnevD.BrevK, BrevD)) BrevD.Initialize();
	T_EVGP_D EvgpD;
	if (!m_pDoc->m_pAttrCtrl2->GetEvgp(BrevD.EvgpK, EvgpD)) return FALSE;

	// nFailType: 0=Conc  1=Pryout  2=both 
	if (nFailType==0 || nFailType==2)
	{
		//////////////////////////////////////////////////////////////////////////
		// 콘크리트 파괴
		T_BRDGEVAL_BEARING rPosResD = rResD;
		T_BRDGEVAL_BEARING rNegResD = rResD;

		Calc_BearingAnchor_ConcreteSubData(AnevD, TRUE, rPosResD, TRUE); // 정방향, 기존
		Calc_BearingAnchor_ConcreteSubData(AnevD, FALSE, rNegResD, TRUE);

		//교축, 교직
		for (int i = 0; i < 2; i++)
		{
			int nType = 0;

			if (i == 0 && EvgpD.nType == 1 && EvgpD.nAbutPos == 0) // 교축, Abutment, Start
			{
				nType = ANCHOR_POS;
				rResD.AnchorD[i].FACD = rPosResD.AnchorD[i].FACD;
			}
			else if (i == 0 && EvgpD.nType == 1 && EvgpD.nAbutPos == 1) // 교축, Abutment, End
			{
				nType = ANCHOR_NEG;
				rResD.AnchorD[i].FACD = rNegResD.AnchorD[i].FACD;
			}
			else
			{
				if (rPosResD.AnchorD[i].FACD.dRat <= rNegResD.AnchorD[i].FACD.dRat)
				{
					nType = ANCHOR_POS;
					rResD.AnchorD[i].FACD = rPosResD.AnchorD[i].FACD;
				}
				else
				{
					nType = ANCHOR_NEG;
					rResD.AnchorD[i].FACD = rNegResD.AnchorD[i].FACD;
				}
			}

			rResD.AnchorD[i].FACD.nRatType = nType;
		}
	}

	if (nFailType == 1 || nFailType == 2)
	{
		//////////////////////////////////////////////////////////////////////////
		// 프라이아웃 파괴
		T_BRDGEVAL_BEARING rPosResD2 = rResD;
		T_BRDGEVAL_BEARING rNegResD2 = rResD;

		// DgnSeisAnchorShapeMgr에서 사용되는 static 변수 초기화.
		//CDgnSeisAnchorShapeMgr::InitMapData4CalcShape();
		Calc_BearingAnchor_PryoutSubData(AnevD, TRUE, rPosResD2, TRUE); // 정방향, 기존
		Calc_BearingAnchor_PryoutSubData(AnevD, FALSE, rNegResD2, TRUE);

		//교축, 교직
		for (int i = 0; i < 2; i++)
		{
			int nType = 0;

			if (i == 0 && EvgpD.nType == 1 && EvgpD.nAbutPos == 0) // 교축, Abutment, Start
			{
				nType = ANCHOR_POS;
				rResD.AnchorD[i].FAPD = rPosResD2.AnchorD[i].FAPD;
			}
			else if (i == 0 && EvgpD.nType == 1 && EvgpD.nAbutPos == 1) // 교축, Abutment, End
			{
				nType = ANCHOR_NEG;
				rResD.AnchorD[i].FAPD = rNegResD2.AnchorD[i].FAPD;
			}
			else
			{
				if (rPosResD2.AnchorD[i].FAPD.dRat <= rNegResD2.AnchorD[i].FAPD.dRat)
				{
					nType = ANCHOR_POS;
					rResD.AnchorD[i].FAPD = rPosResD2.AnchorD[i].FAPD;
				}
				else
				{
					nType = ANCHOR_NEG;
					rResD.AnchorD[i].FAPD = rNegResD2.AnchorD[i].FAPD;
				}
				rResD.AnchorD[i].FAPD.nRatType = nType;
			}
		}
	}

	return TRUE;
}


void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_Concrete(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
	T_BRDGEVAL_BEARING rPosResD = rResD;
	T_BRDGEVAL_BEARING rNegResD = rResD;

	Calc_BearingAnchor_ConcreteSub(BrevK, TRUE,  rPosResD); // 정방향, 기존
	Calc_BearingAnchor_ConcreteSub(BrevK, FALSE, rNegResD);

	T_BREV_D BrevD;
	if (!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) BrevD.Initialize();
	T_EVGP_D EvgpD;
	if (!m_pDoc->m_pAttrCtrl2->GetEvgp(BrevD.EvgpK, EvgpD)) return;

	//교축, 교직
	for (int i = 0; i < 2; i++)
	{
		int nType = 0;

		if (i==0 && EvgpD.nType == 1 && EvgpD.nAbutPos == 0) // 교축, Abutment, Start
		{
			nType = ANCHOR_POS;
			rResD.AnchorD[i].FACD = rPosResD.AnchorD[i].FACD;
		}
		else if (i==0 && EvgpD.nType == 1 && EvgpD.nAbutPos == 1) // 교축, Abutment, End
		{
			nType = ANCHOR_NEG;
			rResD.AnchorD[i].FACD = rNegResD.AnchorD[i].FACD;
		}
		else
		{
			if (rPosResD.AnchorD[i].FACD.dRat <= rNegResD.AnchorD[i].FACD.dRat)
			{
				nType = ANCHOR_POS;
				rResD.AnchorD[i].FACD = rPosResD.AnchorD[i].FACD;
			}
			else
			{
				nType = ANCHOR_NEG;
				rResD.AnchorD[i].FACD = rNegResD.AnchorD[i].FACD;
			}
		}
		rResD.AnchorD[i].FACD.nRatType = nType;

		if(i==0)
		{
			if     (nType == ANCHOR_POS) rResD.AnchorParamConcLongi.Copy(rPosResD.AnchorParamConcLongi);
			else if(nType == ANCHOR_NEG) rResD.AnchorParamConcLongi.Copy(rNegResD.AnchorParamConcLongi);
		}
		else
		{
			if     (nType == ANCHOR_POS) rResD.AnchorParamConcTrans.Copy(rPosResD.AnchorParamConcTrans);
			else if(nType == ANCHOR_NEG) rResD.AnchorParamConcTrans.Copy(rNegResD.AnchorParamConcTrans);
		}
	}

	T_BRDGEVAL_ANCHOR_PARAM_LIST AnchorParamConcLongi;
	T_BRDGEVAL_ANCHOR_PARAM_LIST AnchorParamConcTrans;

}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_ConcreteSub(const T_BREV_K& BrevK, BOOL bAnckorPos, T_BRDGEVAL_BEARING& rResD)
{
	T_ANEV_D AnevD;
	T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD, bAnckorPos);

	T_BREV_D BrevD;
	if (!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) BrevD.Initialize();

	Calc_BearingAnchor_ConcreteSubData(AnevD, bAnckorPos, rResD);
}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_ConcreteSubData(T_ANEV_D& AnevD, BOOL bAnckorPos, T_BRDGEVAL_BEARING& rResD, BOOL bDlg/* = FALSE*/)
{
	// DgnSeisAnchorShapeMgr에서 사용되는 static 변수 초기화.
	//CDgnSeisAnchorShapeMgr::InitMapData4CalcShape();

    T_BREV_D BrevD; BrevD.Initialize(); //사용 안함.
//    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) BrevD.Initialize();

    T_BRPR_D BrprD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BrprK, BrprD) ) BrprD.Initialize();

    //CDgnSeisAnchorShapeMgr AnchorShapeMgr(AnevK);
	CDgnSeisAnchorShapeMgr AnchorShapeMgr;

    int nTotalBearing = 0;
	int nTotalBearingL = 0;
	int nTotalBearingT = 0;
    std::vector<std::vector<T_BRPR_D>> aArBrprD;
    for ( int i = 0; i < AnevD.BearingProp.aBrprKs.GetSize(); i++ )
    {
        aArBrprD.push_back(std::vector<T_BRPR_D>());
        for ( int j = 0; j < AnevD.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++ )
        {
			if (AnevD.BearingProp.aBrprKs[i].aBrprK[j] > 0)
			{
				//if (!m_pDoc->m_pAttrCtrl2->GetBrpr(CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, i, j), BrprD) ) ASSERT(0);
				if (!m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) ASSERT(0);
				aArBrprD[i].push_back(BrprD);
			}
			else
			{
				BrprD.Initialize();
				aArBrprD[i].push_back(BrprD);
			}

			//각 방향의 전열에서 고정인 BP 개수
			int nBrevType = AnevD.SuppType.aLayer[i].aBrevType[j];
            nTotalBearing++;
			nTotalBearingL += (nBrevType == 0 || nBrevType == 3)? 1 : 0;    // 교축 고정단
			nTotalBearingT += (nBrevType == 0 || nBrevType == 2)? 1 : 0;    // 교직 고정단
        }
    }
	int nDIR_POS = (bAnckorPos)? 0 : 1;
    AnchorShapeMgr.SetData(aArBrprD, AnevD, bAnckorPos);
	AnchorShapeMgr.SetDULR(nDIR_POS, nDIR_POS);
	AnchorShapeMgr.SetPryoutDULR(nDIR_POS, nDIR_POS);
	AnchorShapeMgr.UpdateCa1Auto();

    if ( aArBrprD.size() == 0 )
    {
        ASSERT(0);
    }
    BrprD = aArBrprD[0][0];

    int nBearingPropNumbLongi = 0;
    int nBearingPropNumbTrans = 0;

    std::vector<std::pair<int, int>> aRepBearingIdxLong;
    std::vector<std::pair<int, int>> aRepBearingIdxTrans;

    AnchorShapeMgr.GetConcfailRepresentGrpBearing(LONGI, aRepBearingIdxLong);
    AnchorShapeMgr.GetConcfailRepresentGrpBearing(TRANS, aRepBearingIdxTrans);

    nBearingPropNumbLongi = (int)aRepBearingIdxLong.size();
    nBearingPropNumbTrans = (int)aRepBearingIdxTrans.size();

    rResD.AnchorParamConcLongi.SetSize(nBearingPropNumbLongi);
    rResD.AnchorParamConcTrans.SetSize(nBearingPropNumbTrans);

    double dCa1 = 0.0;
    double dCa1P = 0.0;
    double dCa2 = 0.0;
    double dCa2P = 0.0;

	T_BRDGEVAL_ANCHOR_PARAM AnchorParamAll_L;
	T_BRDGEVAL_ANCHOR_PARAM AnchorParamAll_T;

    std::vector<TConcFailBcCalc> aConcFailBc;

    AnchorShapeMgr.GetConcFailBc(LONGI, aConcFailBc);
    for ( int i = 0; i < aRepBearingIdxLong.size(); i++ )
    {
        int nBrI = aRepBearingIdxLong[i].first;
        int nBrJ = aRepBearingIdxLong[i].second;
        T_BRPR_D BrprD;
        T_ELEM_K BrprK = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, nBrI, nBrJ);
        if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(BrprK, BrprD) ) BrprD.Initialize();
        AnchorShapeMgr.GetConcFailBearingCa(LONGI, nBrI, nBrJ, dCa1, dCa1P, dCa2, dCa2P);
		T_BRDGEVAL_ANCHOR_PARAM& AnchorLongi = rResD.AnchorParamConcLongi[i];
		if (!rResD.ParamD.bUserConc)
		{
			AnchorLongi.dCa1 = dCa1;
			AnchorLongi.dCa1P = dCa1P;
			AnchorLongi.dCa2 = dCa2;
			AnchorLongi.dCa2P = dCa2P;
			AnchorLongi.dCa1Re = dCa1;
			double dCa1Re = 0.0;

			if (AnchorShapeMgr.GetReCalcCa1(LONGI, nBrI, nBrJ, dCa1Re, aConcFailBc)) AnchorLongi.dCa1Re = dCa1Re;
		}
		else
		{
			AnchorLongi.dCa1  = AnevD.Fail.ConcFailL.dCa1;
			AnchorLongi.dCa1P = AnevD.Fail.ConcFailL.dCa1_q;
			AnchorLongi.dCa2  = AnevD.Fail.ConcFailL.dCa2;
			AnchorLongi.dCa2P = AnevD.Fail.ConcFailL.dCa2_q;
			AnchorLongi.dCa1Re = AnevD.Fail.ConcFailL.dCa1;
		}
        ConvertAnchorParam(nBrI, nBrJ, BrevD, AnevD, BrprD, rResD.ParamD, AnchorLongi);
    }


    AnchorShapeMgr.GetConcFailBc(TRANS, aConcFailBc);
    for ( int i = 0; i < aRepBearingIdxTrans.size(); i++ )
    {
        int nBrI = aRepBearingIdxTrans[i].first;
        int nBrJ = aRepBearingIdxTrans[i].second;
        T_BRPR_D BrprD;
        T_ELEM_K BrprK = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, nBrI, nBrJ);
        if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(BrprK, BrprD) ) BrprD.Initialize();
        AnchorShapeMgr.GetConcFailBearingCa(TRANS, nBrI, nBrJ, dCa1, dCa1P, dCa2, dCa2P);
		T_BRDGEVAL_ANCHOR_PARAM& AnchorTrans = rResD.AnchorParamConcTrans[i];
		if (!rResD.ParamD.bUserConc)
		{
			AnchorTrans.dCa1 = dCa1;
			AnchorTrans.dCa1P = dCa1P;
			AnchorTrans.dCa2 = dCa2;
			AnchorTrans.dCa2P = dCa2P;
			AnchorTrans.dCa1Re = dCa1;
			double dCa1Re = 0.0;
			if (AnchorShapeMgr.GetReCalcCa1(TRANS, nBrI, nBrJ, dCa1Re, aConcFailBc)) AnchorTrans.dCa1Re = dCa1Re;
		}
		else
		{
			AnchorTrans.dCa1  = AnevD.Fail.ConcFailT.dCa1;
			AnchorTrans.dCa1P = AnevD.Fail.ConcFailT.dCa1_q;
			AnchorTrans.dCa2  = AnevD.Fail.ConcFailT.dCa2;
			AnchorTrans.dCa2P = AnevD.Fail.ConcFailT.dCa2_q;
			AnchorTrans.dCa1Re = AnevD.Fail.ConcFailT.dCa1;
		}
        ConvertAnchorParam(nBrI, nBrJ, BrevD, AnevD, BrprD, rResD.ParamD, AnchorTrans);
    }

    BOOL bTypical= IsTypicalBearingArrange(AnevD, rResD.ParamD.dRelAngle);

    double dsax    = rResD.ParamD.dSax;
    double dsay    = rResD.ParamD.dSay;
    double dhcop   = rResD.ParamD.dhcop;
    double dfck    = rResD.ParamD.dfck;

	std::vector<std::pair<int, int>> aRepBearingIdxNN_L;
	std::vector<std::pair<int, int>> aRepBearingIdxNN_T;
	std::vector<std::pair<int, int>> aRepBearingIdxNN;
    double nnAL    = AnchorShapeMgr.GetConcFailCa1AnchorNum(LONGI, aRepBearingIdxNN_L, TRUE);
    double nnAT    = AnchorShapeMgr.GetConcFailCa1AnchorNum(TRANS, aRepBearingIdxNN_T, TRUE);

    double nn = 0.0;

	int nTotalBearingNumb = 0;
    for ( int i=0; i<2; ++i ) // Longitudinal, Transverse
    {
		nTotalBearingNumb = (i == 0) ? nTotalBearingL : nTotalBearingT;

		if(nTotalBearingNumb == 0) continue;

        if ( i == 0 )
        {
            nn = nnAL;
            if ( rResD.AnchorParamConcLongi.GetSize() > 1 )
            {
                bTypical = FALSE;
            }
        }
        else
        {
            nn = nnAT;
            if ( rResD.AnchorParamConcTrans.GetSize() > 1 )
            {
                bTypical = FALSE;
            }
        }

        if ( !bTypical )
        {   // 여기서 dsax, dsay는 교축을 기준으로 계산한다.
			aRepBearingIdxNN = (i == LONGI) ? aRepBearingIdxNN_L : aRepBearingIdxNN_T;
            AnchorShapeMgr.GetConcFailSaxNew(LONGI, aRepBearingIdxNN, dsax);
            AnchorShapeMgr.GetConcFailSaxNew(TRANS, aRepBearingIdxNN, dsay);
//			AnchorShapeMgr.GetConcFailSax(LONGI, dsax);
//			AnchorShapeMgr.GetConcFailSax(TRANS, dsay);

        }

        double dca1    = 0.0;
        double dca1p   = 0.0;
        double dca2    = 0.0;
        double dca2p   = 0.0;
        double d15ca1  = 0.0;
		double d15ca1org = 0.0;
        double dha     = 0.0;
        double nBC     = 1.0;
        double nBC_org = 1.0;
        double dAvc    = 0.0;
        double dAvco   = 0.0;
        double dLp     = 0.0;

        double dVbSum   = 0.0;
        double dAvcoSum = 0.0;
        double dpsi_edv = 1.0;
		double dAvcSum  = 0.0;
		double d15ca1Sum = 0.0;

        int nMaxSize = i==0 ?rResD.AnchorParamConcLongi.GetSize() : rResD.AnchorParamConcTrans.GetSize();
        if(nMaxSize == 0) continue;

        //Bearing Property 가 하나인 경우 - 어차피 Bearing Property 여러개이면 Typical인 경우가 아님
        double ds1     = i==0 ? rResD.AnchorParamConcLongi[0].dS1 : rResD.AnchorParamConcTrans[0].dS2;
        double ds2     = i==0 ? rResD.AnchorParamConcLongi[0].dS2 : rResD.AnchorParamConcTrans[0].dS1;

        double dca1org = 0.0; //그룹내 대표값으로 사용해야함

        int n1AnchorType_org = 0; //그룹내에서 정해짐
        int n1AnchorType = 0; //그룹내에서 정해짐
        int nAvcType = 0; //Bearing Property가 여러개인 경우 사용 없음
        double dAvc_cal = 0.0;
        double dAvcLim = 0.0;
        BOOL bEdgeEff = FALSE;

		double iMaxSize = rResD.AnchorParamConcLongi.GetSize();
        if ( i == 1 ) iMaxSize = rResD.AnchorParamConcTrans.GetSize();
        for ( int j=0; j<iMaxSize; j++ )
        {
            T_BRDGEVAL_ANCHOR_PARAM AnchorParam; AnchorParam.initialize();

            if ( i == 0 )
            {
                AnchorParam = rResD.AnchorParamConcLongi[j];
            }
            else
            {
                AnchorParam = rResD.AnchorParamConcTrans[j];
            }

            double dhef    = AnchorParam.dhef;
            double da      = AnchorParam.da;
            // Set Data
            double dnA     = i==0 ? AnchorParam.nnL : AnchorParam.nnT;


            if ( rResD.ParamD.bUserConc )
            {
                const T_ANEV_CONC_FAIL_D& FailD = i==0 ? AnevD.Fail.ConcFailL : AnevD.Fail.ConcFailT;

                dca1   = FailD.dCa1;
                dca1p  = FailD.dCa1_q;
                dca2   = FailD.dCa2;
                dca2p  = FailD.dCa2_q;
                dha    = FailD.dHa;
                nBC    = static_cast<double>(FailD.nNum);
                dAvc   = FailD.dAVc;
                dAvco  = FailD.dAVco;

				dca1org = dca1;
            }
            else
            {
				//  dca1, dca1p, dca2, dca2p 계산은 뷰값과 동일시.
                //if ( bTypical )
                //{
                //    Get_ArrangeEdgeDimsConc(BrevK, (i==0?LONGI:TRANS), dca1, dca1p, dca2, dca2p);
                //}
                //else
                {
                    int nBCnum=0;
                    AnchorShapeMgr.GetConcFailRepresentVal(i, dLp, nBCnum, dca1org, dca1p, dca2, dca2p);         // dca1org : 각 방향별 원래 거리
                    nBC = static_cast<double>(nBCnum);

					//if(j == 0) // nBCnum 만큼 j가 돈다.
					{
						double dCa1Recal = AnchorParam.dCa1Re;
						int nMinCa1BearingI = 0;
						int nMinCa1BearingJ = 0;
						AnchorShapeMgr.GetRepresentCa1Bearing((i == 0 ? LONGI : TRANS), nMinCa1BearingI, nMinCa1BearingJ);
						AnchorShapeMgr.GetReCalcCa1((i == 0 ? LONGI : TRANS), nMinCa1BearingI, nMinCa1BearingJ, dCa1Recal);  // dCa1Recal : 각 방향별 재계산 거리
						AnchorParam.dCa1Re = dCa1Recal;
						if (i == 0)
						{
							rResD.AnchorParamConcLongi[j].dCa1Re = dCa1Recal;
						}
						else
						{
							rResD.AnchorParamConcTrans[j].dCa1Re = dCa1Recal;
						}
						dca1 = dCa1Recal;  // dca1 : 각 방향별 재계산 거리
					}
                }
            }
			d15ca1org = 1.5*dca1org;
			d15ca1    = 1.5*dca1;
            if ( !rResD.ParamD.bUserConc ) dha = min(dhcop, d15ca1);

            n1AnchorType = RESIS_MULTIANCHOR;
            // 투영길이 계산 - 장호
            nAvcType = 0;
            if ( bTypical )
            {
                // 단일 앵커 및 앵커 그룹 판단
                BOOL bInter_s1  = (fabs(ds1)<2.0*d15ca1org ? TRUE : FALSE);
                BOOL bInter_s2  = (fabs(ds2)<2.0*d15ca1org ? TRUE : FALSE);
                BOOL bInter_sax = FALSE;
                if ( i==0 ) bInter_sax = (fabs(dsax)<2.0*d15ca1org ? TRUE : FALSE); // 교축
                else        bInter_sax = (fabs(dsay)<2.0*d15ca1org ? TRUE : FALSE); // 교축직각

                if ( fabs(ds1)<cDgn_Zero && fabs(ds2)<cDgn_Zero && !bInter_sax )
                {
                    n1AnchorType = RESIST_1ANCHOR_1;
                }
                else if ( !rResD.ParamD.bUserConc )
                {
                    if ( bInter_s1 && bInter_sax )
                    {
                        nBC = i==0 ? rResD.ParamD.nnBCL : rResD.ParamD.nnBCT;
                    }
                    else if ( fabs(ds1)<cDgn_Zero )
                    {
                        n1AnchorType = RESIST_1ANCHOR_ALL;
                    }
                }
                if ( !rResD.ParamD.bUserConc )
                {
                    // 투영길이 계산
                    dAvc = Get_Avc(dca1, dca2, dca2p, ds1, (i==0 ? dsax : dsay), nBC, dnA, dha, nAvcType, dLp);
                }
            }
            else if ( !rResD.ParamD.bUserConc )
            {
                nAvcType = 0;
                dAvc     = dLp * dha;
            }

            nBC_org = nBC;
            n1AnchorType_org = n1AnchorType;

			double dca1orgTemp = dca1org;
			double dca1Temp = dca1org;;
			BOOL bChk2face = FALSE;
			if (!rResD.ParamD.bUserConc)
			{
				// 콘크리트파괴 저항면적 상정시 3면 이상 가장자리의 영향 검토
				BOOL bChk2face = (dLp < 2.0*d15ca1org) ? TRUE : FALSE;
				bEdgeEff = Check_Inter3Faces(dca1orgTemp, dca2, dca2p, dhcop, (i == 0 ? dsax : dsay), dLp);  //dca1  : 재계산된 ca1
				if (bEdgeEff) dca1Temp = dca1orgTemp;	// dca1Temp를 써야 하지만 dca1에서 이미 계산됨.	
			}

            if ( i == LONGI ) bEdgeEff = FALSE;

            if ( bEdgeEff && (fabs(dca1org-dca1) > cDgn_Zero) )
            {
                if ( !rResD.ParamD.bUserConc )
                {
                    // 3면이 가장자리의 영향을 받으므로 하중작용방향으로 연단거리 재산정 필요 - Check_Inter3Faces 재산정함.
                    //d15ca1 = 1.5*dca1; => d15ca1org로 대체
                    if ( bTypical )
                    {
                        // 단일 앵커 및 앵커 그룹 판단
                        BOOL bInter_s1  = (fabs(ds1)<2.0*d15ca1org ? TRUE : FALSE);
                        BOOL bInter_s2  = (fabs(ds2)<2.0*d15ca1org ? TRUE : FALSE);
                        BOOL bInter_sax = FALSE;
                        if ( i==0 ) bInter_sax = (fabs(dsax)<2.0*d15ca1org ? TRUE : FALSE); // 교축
                        else        bInter_sax = (fabs(dsay)<2.0*d15ca1org ? TRUE : FALSE); // 교축직각

                        if ( fabs(ds1)<cDgn_Zero && fabs(ds2)<cDgn_Zero && !bInter_sax )
                        {
                            n1AnchorType = RESIST_1ANCHOR_1;
                        }
                        else
                        {
                            n1AnchorType = RESIS_MULTIANCHOR;
                            if ( bInter_s1 && bInter_sax )
                            {
                                nBC = i==0 ? rResD.ParamD.nnBCL : rResD.ParamD.nnBCT;
                            }
                            else
                            {
                                if ( fabs(ds1)<cDgn_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                                nBC = 1.0;
                            }
                        }

                        // 투영길이 계산
                        dAvc = Get_Avc(dca1, dca2, dca2p, ds1, (i==0 ? dsax : dsay), nBC, dnA, dha, nAvcType, dLp);
                    }
                    else
                    {
                        double dca1_recal = dca1;
                        int nBCnum=0;
                        AnchorShapeMgr.GetConcFailRepresentVal(i, dLp, nBCnum, dca1, dca1p, dca2, dca2p);
                        nAvcType = 0;

                        nBC  = static_cast<double>(nBCnum);
                        dAvc = dLp * dha;
                    }
                }
                else
                {
                    dca1 = dca1org;
                }
            }

            if ( !rResD.ParamD.bUserConc )
            {
                dAvco = 4.5 * AnchorParam.dCa1Re * AnchorParam.dCa1Re;
				dca1 = AnchorParam.dCa1Re;
            }

			dAvcSum += dAvc;
			dAvcoSum += dAvco;
			d15ca1Sum += d15ca1;

            const double dVb = Calc_Vb(AnchorParam.dVbParam, AnchorParam.dle, AnchorParam.dap, dfck, dca1);
            dVbSum += dVb;

        } // iMaxSize

        double dFACD = rResD.AnchorD[i].FASD.dFASD * nBC / nTotalBearingNumb;

        double dVbmax = Calc_Vbmax(dfck, dca1);
        double dVbAvg = SafeDiv(dVbSum, nBC);
        double dAvcoAvg = SafeDiv(dAvcoSum, nBC);
        double dPuD  = rResD.ParamD.ForceD.dPtopBearing * nBC / nTotalBearing;

		// 재계산(평균)
		d15ca1 = d15ca1Sum / iMaxSize;
		dAvc  = dAvcSum / iMaxSize;
		dAvco = dAvcoSum / iMaxSize;
		dAvcLim = nn * dAvco;
		if (!rResD.ParamD.bUserConc) dha = min(dhcop, d15ca1);

		dAvc_cal = dAvc;
		if (dAvc > dAvcLim)
		{
			dAvc = dAvcLim;
		}
		//
		dpsi_edv = 1.0;
		double dca2min = min(dca2, dca2p);
		if (dca2min < d15ca1)
		{
			dpsi_edv = 0.7 + 0.3*dca2min / d15ca1;
		}
        double dpsi_cv = Get_psi_cV(AnevD); // KALIS2023 사용자 입력값 rResD.ParamD.dpsi_cv;

        double dVb = min(dVbmax, dVbAvg);
        double dVcbg = fabs(dAvco)<cDgn_Zero ? 0.0 : (dAvc/dAvco) * dpsi_edv * dpsi_cv * dVb;
        double dFACC = Calc_FACC(dVcbg, dPuD);
        double dVbrkec = dFACC;

        BOOL   bAnchorRebar = rResD.ParamD.bShearRebar;
        double dnas  = rResD.ParamD.nNas[i];
        double dAsas = rResD.ParamD.ddsas[i]*rResD.ParamD.ddsas[i]*CMathFunc::m_pi/4.0;
        double dfya  = rResD.ParamD.dfyas[i];
        double dVasv = 0.0;
        if ( bAnchorRebar )
        {
            dVasv = dnas * 1.0 * dAsas * dfya;
            dFACC = max(dFACC, dVasv);
        }

        double dRat  = SafeDiv(dFACC, dFACD, _INFI4);

        // set result
        rResD.AnchorD[i].FACD.ds1       = ds1;
        rResD.AnchorD[i].FACD.ds2       = ds2;
        rResD.AnchorD[i].FACD.dsax      = dsax;
        rResD.AnchorD[i].FACD.dsay      = dsay;
		rResD.AnchorD[i].FACD.d15ca1    = d15ca1;
        rResD.AnchorD[i].FACD.dca1_org  = dca1org;
        rResD.AnchorD[i].FACD.dca1p     = dca1p;
        rResD.AnchorD[i].FACD.dca2      = dca2;
        rResD.AnchorD[i].FACD.dca2p     = dca2p;
        rResD.AnchorD[i].FACD.n1AnchorType  = n1AnchorType; // 1열 받침에 1열 앵커가 배치되어 있어 단일 앵커 적용
        rResD.AnchorD[i].FACD.n1AnchorType_org = n1AnchorType_org; // 1열 받침에 1열 앵커가 배치되어 있어 단일 앵커 적용
        rResD.AnchorD[i].FACD.nnBC      = nBC;
        rResD.AnchorD[i].FACD.nnBC_org  = nBC_org;
		rResD.AnchorD[i].FACD.nTotalBearingNumb = nTotalBearingNumb; // iMaxSize;
        rResD.AnchorD[i].FACD.nTotalBearing = nTotalBearing; // All bearings
        rResD.AnchorD[i].FACD.bEdgeEff  = bEdgeEff;  // TRUE - 3면이 가장자리의 영향을 받으므로 하중작용방향으로 연단거리 재산정 필요
        rResD.AnchorD[i].FACD.dca1      = dca1;      // 최종 ca1
        rResD.AnchorD[i].FACD.dha       = dha;
        rResD.AnchorD[i].FACD.nAvcType  = nAvcType; // 0~3
        rResD.AnchorD[i].FACD.dAvc_cal  = dAvc_cal;
        rResD.AnchorD[i].FACD.dAvco     = dAvcoAvg;
        rResD.AnchorD[i].FACD.dAvcoSum  = dAvcoSum;
        rResD.AnchorD[i].FACD.nn        = nn;
        rResD.AnchorD[i].FACD.dAvcLim   = dAvcLim;
        rResD.AnchorD[i].FACD.dAvc      = dAvc;
        rResD.AnchorD[i].FACD.dpsi_edv  = dpsi_edv;
        rResD.AnchorD[i].FACD.dpsi_cv   = dpsi_cv;
        rResD.AnchorD[i].FACD.dVb       = dVb;
        rResD.AnchorD[i].FACD.dVbAvg    = dVbAvg;
        rResD.AnchorD[i].FACD.dVbMax    = dVbmax;
        rResD.AnchorD[i].FACD.dVb_Total = dVbSum;
        rResD.AnchorD[i].FACD.dVcbg     = dVcbg;
        rResD.AnchorD[i].FACD.dVbrkec   = dVbrkec;
        rResD.AnchorD[i].FACD.dPuD      = rResD.ParamD.ForceD.dPtopBearing;
        rResD.AnchorD[i].FACD.bAnchorRebar = bAnchorRebar;
        rResD.AnchorD[i].FACD.nnas      = dnas;
        rResD.AnchorD[i].FACD.dAsas     = dAsas;
        rResD.AnchorD[i].FACD.dfya      = dfya; // 철근강도
        rResD.AnchorD[i].FACD.dVsav     = dVasv;
        rResD.AnchorD[i].FACD.dFACC     = dFACC;
        rResD.AnchorD[i].FACD.dnnBC     = nBC;
        rResD.AnchorD[i].FACD.dFASD     = rResD.AnchorD[i].FASD.dFASD;
        rResD.AnchorD[i].FACD.dFACD     = dFACD;
        rResD.AnchorD[i].FACD.dRat      = dRat;
		rResD.AnchorD[i].FACD.bChk2face = (i == TRANS)? TRUE : FALSE;  // bChk2face
    }
}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_Pryout(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
	T_BRDGEVAL_BEARING rPosResD = rResD;
	T_BRDGEVAL_BEARING rNegResD = rResD;

	// DgnSeisAnchorShapeMgr에서 사용되는 static 변수 초기화.
    //CDgnSeisAnchorShapeMgr::InitMapData4CalcShape(); -> Calc_BearingAnchor_PryoutSub 함수 안쪽에 위치

	Calc_BearingAnchor_PryoutSub(BrevK, TRUE, rPosResD); // 정방향, 기존
	Calc_BearingAnchor_PryoutSub(BrevK, FALSE, rNegResD);

	T_BREV_D BrevD;
	if (!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) BrevD.Initialize();
	T_EVGP_D EvgpD;
	if (!m_pDoc->m_pAttrCtrl2->GetEvgp(BrevD.EvgpK, EvgpD)) return;

	//교축, 교직
	for (int i = 0; i < 2; i++)
	{
		int nType = 0;

		if (i==0 && EvgpD.nType == 1 && EvgpD.nAbutPos == 0) // 교축, Abutment, Start
		{
			nType = ANCHOR_POS;
			rResD.AnchorD[i].FAPD = rPosResD.AnchorD[i].FAPD;
		}
		else if (i==0 && EvgpD.nType == 1 && EvgpD.nAbutPos == 1) // 교축, Abutment, End
		{
			nType = ANCHOR_NEG;
			rResD.AnchorD[i].FAPD = rNegResD.AnchorD[i].FAPD;
		}
		else
		{
			if (rPosResD.AnchorD[i].FAPD.dRat <= rNegResD.AnchorD[i].FAPD.dRat)
			{
				nType = ANCHOR_POS;
				rResD.AnchorD[i].FAPD = rPosResD.AnchorD[i].FAPD;
			}
			else
			{
				nType = ANCHOR_NEG;
				rResD.AnchorD[i].FAPD = rNegResD.AnchorD[i].FAPD;
			}
		}
		rResD.AnchorD[i].FAPD.nRatType = nType;

		if(i==0)
		{
			if     (nType == ANCHOR_POS) rResD.AnchorParamPryLongi.Copy(rPosResD.AnchorParamPryLongi);
			else if(nType == ANCHOR_NEG) rResD.AnchorParamPryLongi.Copy(rNegResD.AnchorParamPryLongi);
		}
		else
		{
			if     (nType == ANCHOR_POS) rResD.AnchorParamPryTrans.Copy(rPosResD.AnchorParamPryTrans);
			else if(nType == ANCHOR_NEG) rResD.AnchorParamPryTrans.Copy(rNegResD.AnchorParamPryTrans);
		}
	}
}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_PryoutSub(const T_BREV_K& BrevK, BOOL bAnckorPos, T_BRDGEVAL_BEARING& rResD)
{
	T_ANEV_D AnevD;
	T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD, bAnckorPos);

	T_BREV_D BrevD;
	if (!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) BrevD.Initialize();

	Calc_BearingAnchor_PryoutSubData(AnevD, bAnckorPos, rResD);

}
void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_PryoutSubData(T_ANEV_D& AnevD, BOOL bAnckorPos, T_BRDGEVAL_BEARING& rResD, BOOL bDlg/* = FALSE*/)
{
	// DgnSeisAnchorShapeMgr에서 사용되는 static 변수 초기화.
	CDgnSeisAnchorShapeMgr::InitMapData4CalcShape();

    T_BREV_D BrevD; BrevD.Initialize(); // 사용안함.
//    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) BrevD.Initialize();

    T_BRPR_D BrprD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BrprK, BrprD) ) BrprD.Initialize();

    //CDgnSeisAnchorShapeMgr AnchorShapeMgr(AnevK);
	CDgnSeisAnchorShapeMgr AnchorShapeMgr;

    int nTotalBearing = 0;
	int nTotalBearingL = 0;
	int nTotalBearingT = 0;
	int nTotalAnchorL = 0;
	int nTotalAnchorT = 0;
    std::vector<std::vector<T_BRPR_D>> aArBrprD;
    for ( int i = 0; i < AnevD.BearingProp.aBrprKs.GetSize(); i++ )
    {
        aArBrprD.push_back(std::vector<T_BRPR_D>());
        for ( int j = 0; j < AnevD.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++ )
        {
			if (AnevD.BearingProp.aBrprKs[i].aBrprK[j] > 0)
			{
				//if (!m_pDoc->m_pAttrCtrl2->GetBrpr(CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, i, j), BrprD) ) ASSERT(0);
				if (!m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) ASSERT(0);
				aArBrprD[i].push_back(BrprD);
			}
			else
			{
				BrprD.Initialize();
				aArBrprD[i].push_back(BrprD);
			}

			//각 방향의 전열에서 고정인 BP
			//각 BP에서 프아이아웃 검토용 앵커 배열 합
			int nBrevType = AnevD.SuppType.aLayer[i].aBrevType[j];
            nTotalBearing++;
			nTotalAnchorL += (nBrevType == 0 || nBrevType == 3)? BrprD.nN1 : 0;
			nTotalAnchorT += (nBrevType == 0 || nBrevType == 2)? BrprD.nN2 : 0;
			nTotalBearingL += (nBrevType == 0 || nBrevType == 3) ? 1 : 0;    // 교축 고정단
			nTotalBearingT += (nBrevType == 0 || nBrevType == 2) ? 1 : 0;    // 교직 고정단
		}
    }

	int nDIR_POS = (bAnckorPos) ? 0 : 1;
    AnchorShapeMgr.SetData(aArBrprD, AnevD, bAnckorPos);
	AnchorShapeMgr.SetDULR(nDIR_POS, nDIR_POS);
	AnchorShapeMgr.SetPryoutDULR(nDIR_POS, nDIR_POS);
	AnchorShapeMgr.UpdatehefAuto();
    if ( aArBrprD.size() == 0 )
    {
        ASSERT(0);
    }
    BrprD = aArBrprD[0][0];

    int nBearingPropNumbLongi = 0;
    int nBearingPropNumbTrans = 0;

    std::vector<std::pair<int, int>> aRepBearingIdxLong;
    std::vector<std::pair<int, int>> aRepBearingIdxTrans;
	std::vector<std::pair<int, int>> aRepBearingIdxTemp;

	double dArea, dCa1, dCa1P, dCa2, dCa2P;
	int nBc, nResistAnchorNum;
	BOOL bSingleResist;
	if (!AnchorShapeMgr.GetPryoutRepresentVal(LONGI, aRepBearingIdxTemp, dArea, nBc, dCa1, dCa1P, dCa2, dCa2P, bSingleResist, nResistAnchorNum)){ /*ASSERT(0);*/}
    AnchorShapeMgr.GetPryoutRepresentGrpBearing(LONGI, aRepBearingIdxLong);

	if (!AnchorShapeMgr.GetPryoutRepresentVal(TRANS, aRepBearingIdxTemp, dArea, nBc, dCa1, dCa1P, dCa2, dCa2P, bSingleResist, nResistAnchorNum)){ /*ASSERT(0);*/}
    AnchorShapeMgr.GetPryoutRepresentGrpBearing(TRANS, aRepBearingIdxTrans);

    nBearingPropNumbLongi = (int)aRepBearingIdxLong.size();
    nBearingPropNumbTrans = (int)aRepBearingIdxTrans.size();
    rResD.AnchorParamPryLongi.SetSize(nBearingPropNumbLongi);
    rResD.AnchorParamPryTrans.SetSize(nBearingPropNumbTrans);

    dCa1 = 0.0;
    dCa1P = 0.0;
    dCa2 = 0.0;
    dCa2P = 0.0;

    for ( int i = 0; i < aRepBearingIdxLong.size(); i++ )
    {
        int nBrI = aRepBearingIdxLong[i].first;
        int nBrJ = aRepBearingIdxLong[i].second;
        T_BRPR_D BrprD;
        T_ELEM_K BrprK = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, nBrI, nBrJ);
        if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(BrprK, BrprD) ) BrprD.Initialize();
        AnchorShapeMgr.GetPryoutBearingCa(LONGI, nBrI, nBrJ, dCa1, dCa1P, dCa2, dCa2P);
        T_BRDGEVAL_ANCHOR_PARAM& AnchorLongi = rResD.AnchorParamPryLongi[i];
        AnchorLongi.dCa1 = dCa1;
        AnchorLongi.dCa1P = dCa1P;
        AnchorLongi.dCa2 = dCa2;
        AnchorLongi.dCa2P = dCa2P;
        double dHefRe = 0.0;
        rResD.AnchorParamPryLongi[i].dhefRe = BrprD.dhef;
        if ( AnchorShapeMgr.GetReCalcHef(LONGI, nBrI, nBrJ, dHefRe) ) AnchorLongi.dhefRe = dHefRe;
        ConvertAnchorParam(nBrI, nBrJ, BrevD, AnevD, BrprD, rResD.ParamD, AnchorLongi);
    }

    for ( int i = 0; i < aRepBearingIdxTrans.size(); i++ )
    {
        int nBrI = aRepBearingIdxTrans[i].first;
        int nBrJ = aRepBearingIdxTrans[i].second;
        T_BRPR_D BrprD;
        T_ELEM_K BrprK = CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, nBrI, nBrJ);
        if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(BrprK, BrprD) ) BrprD.Initialize();
        AnchorShapeMgr.GetPryoutBearingCa(TRANS, nBrI, nBrJ, dCa1, dCa1P, dCa2, dCa2P);
        T_BRDGEVAL_ANCHOR_PARAM& AnchorTrans = rResD.AnchorParamPryTrans[i];
        AnchorTrans.dCa1 = dCa1;
        AnchorTrans.dCa1P = dCa1P;
        AnchorTrans.dCa2 = dCa2;
        AnchorTrans.dCa2P = dCa2P;
        double dHefRe = 0.0;
        rResD.AnchorParamPryTrans[i].dhefRe = BrprD.dhef;
        if ( AnchorShapeMgr.GetReCalcHef(TRANS, nBrI, nBrJ, dHefRe) ) AnchorTrans.dhefRe = dHefRe;
        ConvertAnchorParam(nBrI, nBrJ, BrevD, AnevD, BrprD, rResD.ParamD, AnchorTrans);
    }

    BOOL bTypical= IsTypicalBearingArrange(AnevD, rResD.ParamD.dRelAngle);

    double dsax    = rResD.ParamD.dSax;
    double dsay    = rResD.ParamD.dSay;
    double dhcop   = rResD.ParamD.dhcop;
    double dfck    = rResD.ParamD.dfck;

    const int iMaxSizeLongi = rResD.AnchorParamPryLongi.GetSize();
    const int iMaxSizeTrans = rResD.AnchorParamPryTrans.GetSize();

    int n1AnchorType = 0;
    int n1AnchorAllResist = 0;
	int n1AnchorAllResistCal = 0;

    double dca1    = 0.0;
    double dca1p   = 0.0;
    double dca2    = 0.0;
    double dca2p   = 0.0;
    double dANc  = 0.0;
    double dANco = 0.0;
    double nBC     = 1.0;
    double nBC_org = 1.0;
    double nnBCL = rResD.ParamD.nnBCL;
    double nnBCT = rResD.ParamD.nnBCT;

    double ds1  = 0.0;
    double ds2  = 0.0;
    double dnA  = 0.0;
    double dnB  = 0.0;
    double dhef = 0.0;
    double d15hef = 0.0;

	// 영역안에서 실제 검토되는 앵커 수
    double nnAL    = AnchorShapeMgr.GetPryoutCa1AnchorNum(LONGI, aRepBearingIdxLong);
    double nnAT    = AnchorShapeMgr.GetPryoutCa1AnchorNum(TRANS, aRepBearingIdxTrans);

    double nn = 0.0;

    int    nANcType = 0;

    for ( int i=0; i<2; ++i ) // Longitudinal, Transverse
    {
		double dANcoSum = 0.0;
		double dNbSum = 0.0;
		double dhefSum = 0.0;

        if ( i == 0)
        {
            nn = nnAL;
            if ( rResD.AnchorParamPryLongi.GetSize() > 1 )
            {
                bTypical = FALSE;
            }
        }
        else
        {
            nn = nnAT;
            if ( rResD.AnchorParamPryTrans.GetSize() > 1 )
            {
                bTypical = FALSE;
            }
        }

        if ( !bTypical )
        {
            double dsax_temp=0.0, dsay_temp=0.0;
            AnchorShapeMgr.GetPryoutSaxSay(LONGI, dsax, dsay_temp);
            AnchorShapeMgr.GetPryoutSaxSay(TRANS, dsay, dsax_temp);
        }


        double iMaxSize = iMaxSizeLongi;
        if ( i == 1 ) iMaxSize = iMaxSizeTrans;

		if (iMaxSize == 0)
		{
			// 출력을 위해 디폴트 값 채움.
			T_BRDGEVAL_ANCHOR_PARAM  ANCHORTemp;
			if (i == 0) rResD.AnchorParamPryLongi.Add(ANCHORTemp);
			else        rResD.AnchorParamPryTrans.Add(ANCHORTemp);

			continue;
		}

        // Set Data 밑의 값들은 Typical인 경우에만 사용
        ds1     = i==0 ? rResD.AnchorParamPryLongi[0].dS1 : rResD.AnchorParamPryTrans[0].dS2;
        ds2     = i==0 ? rResD.AnchorParamPryLongi[0].dS2 : rResD.AnchorParamPryTrans[0].dS1;
        dnA     = i==0 ? rResD.AnchorParamPryLongi[0].nnL : rResD.AnchorParamPryTrans[0].nnT;
        dnB     = i==0 ? rResD.AnchorParamPryLongi[0].nnT : rResD.AnchorParamPryTrans[0].nnL;

		double dANcOne, dca1One, dca1pOne, dca2One, dca2pOne;
		dANcOne = dca1One = dca1pOne = dca2One = dca2pOne = 0.0;
		BOOL bSingleResistOne = FALSE;
		int nBCnumOne = 0;
		int n1AnchorAllResistOne = 0;
		BOOL bFirst = TRUE;

		int iMaxSizeCal = iMaxSize;
        for ( int j=0; j< iMaxSizeCal; j++ )
        {
            T_BRDGEVAL_ANCHOR_PARAM AnchorParam; AnchorParam.initialize();

            if ( i == 0 )
            {
                AnchorParam = rResD.AnchorParamPryLongi[j];
            }
            else
            {
                AnchorParam = rResD.AnchorParamPryTrans[j];
            }

            dhef    = AnchorParam.dhef;

            if ( static_cast<int>(dnB)==1 )
            {
                if ( i==0 )
                {
                    if ( nnBCT-cDGN_Zero > 1.0 ) // 가력방향 반대편 앵커열의 앵커가 1개인 경우 제외
                    {
                        nnBCT -= 1.0;
                    }
                }
                else
                {
                    if ( nnBCL-cDGN_Zero > 1.0 ) // 가력방향 반대편 앵커열의 앵커가 1개인 경우 제외
                    {
                        nnBCL -= 1.0;
                    }
                }
            }

            n1AnchorType = RESIS_MULTIANCHOR;
            n1AnchorAllResist = 0;
            if ( rResD.ParamD.bUserPryOut )
            {
                const T_ANEV_PRYOUT_D& PryoutD = i==0 ? AnevD.Fail.PryoutL : AnevD.Fail.PryoutT;

                dca1   = PryoutD.dCa1;
                dca1p  = PryoutD.dCa1_q;
                dca2   = PryoutD.dCa2;
                dca2p  = PryoutD.dCa2_q;

                nBC    = static_cast<double>(PryoutD.nNum);
                dANc   = PryoutD.dANc;
                dANco  = PryoutD.dANco;
                dhef   = PryoutD.dHef;
            }
			//  dca1, dca1p, dca2, dca2p 계산은 뷰값과 동일시.
            //else if ( bTypical )
            //{
            //    Get_ArrangeEdgeDimsPryo(BrevK, (i==0?LONGI:TRANS), dca1, dca1p, dca2, dca2p);
            //}
            else
            {
                int nBCnum=0;
                BOOL bSingleResist = FALSE;
              
				if (bFirst)
				{
					AnchorShapeMgr.GetPryoutRepresentVal(i, aRepBearingIdxTemp, dANc, nBCnum, dca1, dca1p, dca2, dca2p, bSingleResist, n1AnchorAllResist);

					dANcOne = dANc;
					nBCnumOne = nBCnum;
					dca1One = dca1;
					dca1pOne = dca1p;
					dca2One = dca2;
					dca2pOne = dca2p;
					bSingleResistOne = bSingleResist;
					n1AnchorAllResistOne = n1AnchorAllResist;

					bFirst = FALSE;
				}
				else
				{
					dANc = dANcOne;
					nBCnum = nBCnumOne;
					dca1 = dca1One;
					dca1p = dca1pOne;
					dca2 = dca2One;
					dca2p = dca2pOne;
					bSingleResist = bSingleResistOne;
					n1AnchorAllResist = n1AnchorAllResistOne;
				}

				n1AnchorAllResistCal = n1AnchorAllResist;
                if ( bSingleResist && n1AnchorAllResist>0 )
                {
                    n1AnchorType = n1AnchorAllResist==1 ? RESIST_1ANCHOR_1 : RESIST_1ANCHOR_ALL;
					n1AnchorAllResistCal = 1;
                }
                nBC  = static_cast<double>(nBCnum);
            }
            d15hef  = 1.5*dhef;

            // 단일 앵커 및 앵커 그룹 판단
            if ( bTypical )
            {
                BOOL bInter_s1  = (fabs(ds1)<2.0*d15hef ? TRUE : FALSE);
                BOOL bInter_sax = FALSE;
                if ( i==0 ) bInter_sax = (fabs(dsax)<2.0*d15hef ? TRUE : FALSE); // 교축
                else     bInter_sax = (fabs(dsay)<2.0*d15hef ? TRUE : FALSE); // 교축직각
                BOOL bInter_s1say = FALSE;
                if ( i==0 ) bInter_s1say = fabs(ds2*(dnB-1.0)+dsay)<2.0*d15hef ? TRUE : FALSE; // 교축
                else     bInter_s1say = fabs(ds2*(dnB-1.0)+dsax)<2.0*d15hef ? TRUE : FALSE; // 교축직각

                if ( fabs(ds1)<cDgn_Zero && fabs(ds2)<cDgn_Zero && !bInter_sax )
                {
                    n1AnchorType = RESIST_1ANCHOR_1;
                }
                else if ( !rResD.ParamD.bUserPryOut )
                {
                    if ( bInter_s1 && bInter_sax && bInter_s1say )
                    {
                        nBC = nnBCL * nnBCT;
                    }
                    else if ( bInter_s1 && bInter_sax )
                    {
                        nBC = i==0 ? nnBCL : nnBCT;

                        if ( static_cast<int>(nBC) == 1 )
                        {
                            if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                        }
                    }
                    else if ( bInter_s1 && bInter_s1say )
                    {
                        nBC = i==0 ? nnBCT : nnBCL;

                        if ( static_cast<int>(nBC) == 1 )
                        {
                            if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                        }
                    }
                    else if ( bInter_s1 && bInter_s1say )
                    {
                        nBC = i==0 ? nnBCT : nnBCL;

                        if ( static_cast<int>(nBC) == 1 )
                        {
                            if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                        }
                    }
                    else
                    {
                        nBC = 1.0;
                        if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                    }

					if (n1AnchorType == RESIST_1ANCHOR_ALL)
					{
						n1AnchorAllResist = i == 0 ? static_cast<int>(rResD.ParamD.nnBCL) : static_cast<int>(rResD.ParamD.nnBCT);
						n1AnchorAllResistCal = n1AnchorAllResist;
					}
                }
            }
            nBC_org = nBC;

            int n1AnchorType_org = n1AnchorType;
            double dhef_org = dhef;
            BOOL bEdgeEff  = Check_Inter4Faces(dhef, i, dca1, dca1p, dca2, dca2p, dsax, dsay, dnB, ds1, ds2);
            if ( bEdgeEff && fabs(dhef-dhef_org) > cDgn_Zero )
            {
                if ( fabs(dhef-dhef_org) > cDgn_Zero )
                {
                    d15hef  = 1.5*dhef;
                    // 단일 앵커 및 앵커 그룹 판단
                    BOOL bInter_s1  = fabs(ds1)<2.0*d15hef ? TRUE : FALSE;
                    BOOL bInter_sax = FALSE;
                    if ( i==0 ) bInter_sax = fabs(dsax)<2.0*d15hef ? TRUE : FALSE; // 교축
                    else     bInter_sax = fabs(dsay)<2.0*d15hef ? TRUE : FALSE; // 교축직각
                    BOOL bInter_s1say = FALSE;
                    if ( i==0 ) bInter_s1say = fabs(ds2*(dnB-1.0)+dsay)<2.0*d15hef ? TRUE : FALSE; // 교축
                    else     bInter_s1say = fabs(ds2*(dnB-1.0)+dsax)<2.0*d15hef ? TRUE : FALSE; // 교축직각

                    if ( bTypical )
                    {
                        if ( fabs(ds1)<cDgn_Zero && fabs(ds2)<cDgn_Zero && !bInter_sax )
                        {
                            n1AnchorType = RESIST_1ANCHOR_1;
                        }
                        else
                        {
                            if ( bInter_s1 && bInter_sax && bInter_s1say )
                            {
                                int nBCL = i==0 ? static_cast<int>(rResD.ParamD.nnBCL) : static_cast<int>(rResD.ParamD.nnBCT);
                                int nBCT = i==0 ? static_cast<int>(rResD.ParamD.nnBCT) : static_cast<int>(rResD.ParamD.nnBCL);
                                if ( nBCT > 1 && static_cast<int>(dnB)==1 ) // 가력방향 반대편 앵커열의 앵커가 1개인 경우 제외
                                {
                                    nBCT -= 1;
                                }
                                nBC = static_cast<double>(nBCL * nBCT);
                            }
                            else if ( bInter_s1 && bInter_sax )
                            {
                                nBC = i==0 ? nnBCL : nnBCT;
                                if ( static_cast<int>(nBC) == 1 )
                                {
                                    if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                                }
                            }
                            else if ( bInter_s1 && bInter_s1say )
                            {
                                nBC = i==0 ? nnBCT : nnBCL;
                                if ( static_cast<int>(nBC) == 1 )
                                {
                                    if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                                }
                            }
                            else
                            {
                                nBC = 1.0;
                                if ( fabs(ds1)<cDGN_Zero ) n1AnchorType = RESIST_1ANCHOR_ALL;
                            }

							if (n1AnchorType == RESIST_1ANCHOR_ALL)
							{
								n1AnchorAllResist = i == 0 ? static_cast<int>(rResD.ParamD.nnBCL) : static_cast<int>(rResD.ParamD.nnBCT);
								n1AnchorAllResistCal = n1AnchorAllResist;
							}
                        }
                    }
                }
                else
                {
                    dhef = dhef_org;
                }
            }
            // 투영면적
            if ( !rResD.ParamD.bUserPryOut )
            {
				BOOL bSingleResist = FALSE;

                if ( bTypical )
                {
                    int nBCL = i==0 ? static_cast<int>(rResD.ParamD.nnBCL) : static_cast<int>(rResD.ParamD.nnBCT);
                    int nBCT = i==0 ? static_cast<int>(rResD.ParamD.nnBCT) : static_cast<int>(rResD.ParamD.nnBCL);
                    dANc = Get_Anc(dca1, dca1p, dca2, dca2p, dhef, ds1, ds2, (i==0 ? dsax : dsay), (i==0 ? dsay : dsax), nBCL, nBCT, dnA, dnB, nANcType);
                }
                else
                {
                    int nBCnum=0;
                    int nResistAnchorNum;
					if (bFirst)
					{
						AnchorShapeMgr.GetPryoutRepresentVal(i, aRepBearingIdxTemp, dANc, nBCnum, dca1, dca1p, dca2, dca2p, bSingleResist, nResistAnchorNum);

						dANcOne = dANc;
						nBCnumOne = nBCnum;
						dca1One = dca1;
						dca1pOne = dca1p;
						dca2One = dca2;
						dca2pOne = dca2p;
						bSingleResistOne = bSingleResist;
						n1AnchorAllResistOne = nResistAnchorNum;

						bFirst = FALSE;
					}
					else
					{
						dANc = dANcOne;
						nBCnum = nBCnumOne;
						dca1 = dca1One;
						dca1p = dca1pOne;
						dca2 = dca2One;
						dca2p = dca2pOne;
						bSingleResist = bSingleResistOne;
						n1AnchorAllResist = n1AnchorAllResistOne;
					}

                    nBC  = static_cast<double>(nBCnum);
                }

				dANco = 9.0 * AnchorParam.dhefRe * AnchorParam.dhefRe;
				dANcoSum += dANco;
				dNbSum   += AnchorParam.dNb;
				dhefSum  += AnchorParam.dhefRe;

				if(bSingleResist)
				{
					// 하나의 결과만 가지도록 한다.
					iMaxSizeCal = 1;

					if (i == 0)
					{
						rResD.AnchorParamPryLongi.SetSize(1);
					}
					else
					{
						rResD.AnchorParamPryTrans.SetSize(1);
					}

					continue;
				}
            }
			else
			{
				dANcoSum += dANco;
				dNbSum   += AnchorParam.dNb;
				dhefSum  += AnchorParam.dhefRe;
			}

        } // iMaxSizeCal

		if (!rResD.ParamD.bUserPryOut) 
			   dANco   = dANcoSum/ iMaxSizeCal;

        double dNbAvg  = dNbSum  / iMaxSizeCal;
        double dANcLim = nn * dANco;
        double dhefAvg = dhefSum / iMaxSizeCal;

        double dANc_cal = dANc;
        if ( dANc > dANcLim )
        {
            dANc = dANcLim;
        }

        double dcamin = min(min(dca1, dca1p), min(dca2, dca2p));

        double dpsi_edN = 1.0;
        double d15hefAvg = 1.5 * dhefAvg;
        if ( dcamin < d15hefAvg )
        {
            dpsi_edN = 0.7 + 0.3*dcamin/d15hef;
        }
        double dpsi_cN = Get_psi_cN(i==0 ? rResD.AnchorParamPryLongi[0].enAnchType : rResD.AnchorParamPryTrans[0].enAnchType); // KALIS2023 사용자

		double dhefAvgMM = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, dhefAvg);;

        double dkcp = dhefAvgMM < 65.0 ? 1.0 : 2.0;
        double dNcbg = (dANc/dANco) * dpsi_edN * dpsi_cN * dNbAvg;
        double dVcpg = dkcp * dNcbg;

        BOOL   bAnchorRebar = IsCode4PryoutRebar() && MT0(rResD.ParamD.ddsat);
        double dnas  = 2.0;
        double dAsat = rResD.ParamD.ddsat*rResD.ParamD.ddsat*CMathFunc::m_pi/4.0;
        double dfya  = rResD.ParamD.dFyat;
        double dVast = [&]()
        {
            if(bAnchorRebar)
            {
                return dnas * dAsat * dfya;
            }
            return 0.0;
        }();

        if ( n1AnchorType==RESIST_1ANCHOR_ALL && n1AnchorAllResistCal > 1 )
        {
            dVcpg *= static_cast<double>(n1AnchorAllResistCal);
        }
		
		int nTotalAnchorNumb = (i==0)? nTotalAnchorL : nTotalAnchorT;
        int nTotalBearingNumb = (i == 0) ? nTotalBearingL : nTotalBearingT;
		double dnA_New = nn;
        double dFAPC = max(dVcpg, dVast);
        double dFAPD = rResD.AnchorD[i].FASD.dFASD * nn / nTotalAnchorNumb;
        double dRat  = SafeDiv(dFAPC, dFAPD, _INFI4);

        
        EN_BEARING_PRYOUT_CONSIDER_TYPE enIgnore = BEARING_PRYOUT_CONSIDER;
        if(rResD.ParamD.iEvalCode == KALIS_2023)
        {
			const double dD = 0.88 * rResD.ParamD.ForceD.dPtopBearing; // 받침에 작용하는 유효압축력, 고정하중에 의한 수직력의 88%
			const double dDbc = dD * nBC / nTotalBearing;
            if(ME(dD, 0.25 * dVcpg))
            {
                // not consider
                if(AnevD.bPryout) // 검토 불필요하지만 항상 검토하도록 옵션처리.
                {
                    enIgnore = BEARING_PRYOUT_IGNORE_CONSIDER;
                }
                else
                {
                    enIgnore = BEARING_PRYOUT_IGNORE;
					dFAPC = 0.0;
					dFAPD = 0.0;
					dRat = 1.0;
                }
            }

            // set result
			rResD.AnchorD[i].FAPD.dD = dD;
			rResD.AnchorD[i].FAPD.dDbc = dDbc;
        }

        // set result
        rResD.AnchorD[i].FAPD.ds1  = ds1;
        rResD.AnchorD[i].FAPD.ds2  = ds2;
        rResD.AnchorD[i].FAPD.dsax = dsax;
        rResD.AnchorD[i].FAPD.dsay = dsay;

        rResD.AnchorD[i].FAPD.dca1  = dca1;
        rResD.AnchorD[i].FAPD.dca1p = dca1p;
        rResD.AnchorD[i].FAPD.dca2  = dca2;
        rResD.AnchorD[i].FAPD.dca2p = dca2p;

        rResD.AnchorD[i].FAPD.n1AnchorType = n1AnchorType; // 1열 받침에 1열 앵커가 배치되어 있어 단일 앵커 적용
        rResD.AnchorD[i].FAPD.n1AnchorAllResist = n1AnchorAllResistCal;
        rResD.AnchorD[i].FAPD.nnBC_org = nBC_org;
        rResD.AnchorD[i].FAPD.nnBC = nBC;
        rResD.AnchorD[i].FAPD.dhef = dhefAvg;
        rResD.AnchorD[i].FAPD.nTotalBearing = nTotalBearing;
		rResD.AnchorD[i].FAPD.nTotalBearingNumb = nTotalBearingNumb;
        rResD.AnchorD[i].FAPD.nTotalAnchorNumb = nTotalAnchorNumb;
        rResD.AnchorD[i].FAPD.nANcType= nANcType; // 0~6
        rResD.AnchorD[i].FAPD.dANc_Cal= dANc_cal;
        rResD.AnchorD[i].FAPD.dANco   = dANco;
        rResD.AnchorD[i].FAPD.dANcoSum= dANcoSum;

		rResD.AnchorD[i].FAPD.nnA     = nn / nBC; //  dnA;
        rResD.AnchorD[i].FAPD.nnB     = dnB;
        rResD.AnchorD[i].FAPD.nn      = nn;
        rResD.AnchorD[i].FAPD.dANcLim = dANcLim;
        rResD.AnchorD[i].FAPD.dANc    = dANc;

        rResD.AnchorD[i].FAPD.dkcp     = dkcp;
        rResD.AnchorD[i].FAPD.dNcbg    = dNcbg;
        rResD.AnchorD[i].FAPD.dpsi_edN = dpsi_edN;
        rResD.AnchorD[i].FAPD.dpsi_cN  = dpsi_cN;
        if ( i == 0 )
        {
            rResD.AnchorD[i].FAPD.nNbType  = rResD.AnchorParamPryLongi[0].nNbType; // 1~3
        }
        else
        {
            rResD.AnchorD[i].FAPD.nNbType  = rResD.AnchorParamPryTrans[0].nNbType; // 1~3
        }
        rResD.AnchorD[i].FAPD.dNb    = dNbAvg;
		rResD.AnchorD[i].FAPD.dNbSum = dNbSum;

        rResD.AnchorD[i].FAPD.bAnchorRebar = bAnchorRebar;
        rResD.AnchorD[i].FAPD.dAsat = dAsat * dnas;
        rResD.AnchorD[i].FAPD.dfya  = dfya; // 철근강도
        rResD.AnchorD[i].FAPD.dVsat = dVast;

        rResD.AnchorD[i].FAPD.dVcpg = dVcpg;
        rResD.AnchorD[i].FAPD.dFAPC = dFAPC;

		rResD.AnchorD[i].FAPD.dFASD = rResD.AnchorD[i].FASD.dFASD;
        rResD.AnchorD[i].FAPD.dnnBC = nBC;
        rResD.AnchorD[i].FAPD.dFAPD = dFAPD;

		rResD.AnchorD[i].FAPD.dRat = dRat;

        rResD.AnchorD[i].FAPD.enIgnore = enIgnore;
    }
}

void CBrdgSeisEvalKISTEC2015::Calc_BearingSupport_Length(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
    rResD.SuppLengD.initialize();

    // Support Length
    T_SUPL_D SuplD;
    T_SUPL_K SuplK = GetSuplByBrev(BrevK, SuplD);
    if ( SuplK==0 ) return;

    double dPtop   = rResD.ParamD.ForceD.dPtop;
    double dRs     = rResD.ParamD.dRs[0];
    double dLamDRL = rResD.ParamD.dLadbdaDR[0];
    double dMe     = fabs(rResD.ParamD.ForceD.dForce[4]); // Longitudinal
    double dDe     = rResD.ParamD.ForceD.dDisp;
    double dDePD   = 0.0;
    double dDdem   = dDe;
    if ( dRs > 1.0 )
    {
        dDePD = (1.0 + fabs(dPtop*(1.5*dDe) / dMe)) * dDe;
        dDdem = dLamDRL * dDePD;
    }

    double dL    = SuplD.dL / 1000.0;// m 사용자 입력값
    double dH    = SuplD.dH / 1000.0;// m 사용자 입력값
    double dTheta= SuplD.dTheta;// 사용자 입력값
    double dNmin = (200.0 + 1.67*dL + 6.66*dH) * (1.0 + 0.000125*dTheta*dTheta);

    double dND  = max(dDdem, dNmin);
    double dNC  = SuplD.dNc; // 사용자 입력값
    double dRat = SafeDiv(dNC, dND, _INFI4);


    // set result
    rResD.SuppLengD.bCheck = TRUE;

    rResD.SuppLengD.dNC    = dNC;

    rResD.SuppLengD.dDe    = dDe;
    rResD.SuppLengD.dPtop  = dPtop;
    rResD.SuppLengD.dMe    = dMe;
    rResD.SuppLengD.dRs    = dRs;
    rResD.SuppLengD.bPDel  = (dRs > 1.0);
    rResD.SuppLengD.dDpd   = dDePD;
    rResD.SuppLengD.dLd_DR = dLamDRL;
    rResD.SuppLengD.dDdem  = dDdem;

    rResD.SuppLengD.Nmin   = dNmin;
    rResD.SuppLengD.dL     = dL*1000.0;
    rResD.SuppLengD.dH     = dH*1000.0;
    rResD.SuppLengD.dTheta = dTheta;
    rResD.SuppLengD.dND    = dND;

    rResD.SuppLengD.dRat   = dRat;
}

double CBrdgSeisEvalKISTEC2015::Get_Ptop(const T_BREV_K& BrevK, bool bTotal)
{
    double dPtop = DBL_MAX;

    T_BREV_D BrevD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) return dPtop;

    CArray<T_EVGP_K, T_EVGP_K> arEvgp;
    arEvgp.RemoveAll();
    arEvgp.Add(BrevD.EvgpK);

    INT_PTR nEvgp = arEvgp.GetSize();
    for ( INT_PTR i=0; i<nEvgp; ++i )
    {
        T_EVGP_D EvgpD;
        if ( !m_pDoc->m_pAttrCtrl2->GetEvgp(arEvgp[i], EvgpD) ) return dPtop;

        CMapEx<T_ELNK_K, T_ELNK_K, double, double&> mapERect;
        CMapEx<T_NLNK_K, T_NLNK_K, double, double&> mapNRect;
        double dP = Calc_PierCapSuppEnvAxialForce(BrevD.EvgpK, -1, mapERect, mapNRect, bTotal);
        if ( fabs(dP) < dPtop )
        {
            dPtop = dP;
        }
    }

    return dPtop;
}

void CBrdgSeisEvalKISTEC2015::Get_Disp(const T_BREV_K& BrevK, const double& dTheta, double& dDispL, double& dDispT, UINT& LinkL, UINT& LinkT)
{
    dDispL = 0.0;
    dDispT = 0.0;
	LinkL = 0;
	LinkT = 0;
    T_BREV_D BrevD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) return;

    CArray<T_EVGP_K, T_EVGP_K> arEvgp;
    arEvgp.RemoveAll();
    arEvgp.Add(BrevD.EvgpK);

    INT_PTR nEvgp = arEvgp.GetSize();
    for ( INT_PTR i=0; i<nEvgp; ++i )
    {
        T_EVGP_D EvgpD;
        if ( !m_pDoc->m_pAttrCtrl2->GetEvgp(arEvgp[i], EvgpD) ) return;

        double dCurDL = 0.0, dCurDT = 0.0;
		UINT CurLinkL = 0, CurLinkT = 0;

        if ( EvgpD.nType == 0 || EvgpD.nLink == 0 ) // 0:Elastic Link, 1:Support
        {
            Calc_PierSeisEnvDisp(arEvgp[i], 0, LONGI, 0, dCurDL, CurLinkL, TRUE, dTheta);
            Calc_PierSeisEnvDisp(arEvgp[i], 0, TRANS, 0, dCurDT, CurLinkT, TRUE, dTheta);
        }
        else
        {
            // node displacement
            T_DISP_D DispMax; DispMax.Initialize();

            CArray<T_NODE_K, T_NODE_K> arNode;
            INT_PTR nNode = arNode.GetSize();

            T_LCOM_K_LIST arLcomK;
            m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, arLcomK);
            INT_PTR nLcom = arLcomK.GetSize();
            for ( int i=0; i<nLcom; ++i )
            {
                T_LCOM_D LcomD;
                if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[i], LcomD) ) continue;
                if ( LcomD.nSeisLcomType == 0 ) continue;
                m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

                // Get reaction
                for ( INT_PTR j=0; j<nNode; ++j )
                {
                    T_DISP_D DispDataMax;  DispDataMax.Initialize();
					T_DISP_D DispDataMin;  DispDataMin.Initialize();
                    m_pDoc->m_pPostCtrl->GetDispNew(arNode[j], &DispDataMax, &DispDataMin, NULL); // Load Combi에 대한 반력 
                    DispMax.AbsMax(DispDataMax);
					DispMax.AbsMax(DispDataMin);
                }
            }

            ConvertToBearing(dTheta, DispMax.dblDisp[0], DispMax.dblDisp[1]);

            dCurDL = DispMax.dblDisp[0];
            dCurDT = DispMax.dblDisp[1];
        }

        if ( fabs(dDispL) < fabs(dCurDL) ) {dDispL = dCurDL; LinkL=CurLinkL;}
        if ( fabs(dDispT) < fabs(dCurDT) ) {dDispT = dCurDT; LinkT=CurLinkT;}
    }
}

void CBrdgSeisEvalKISTEC2015::Get_BeringForce(const T_BREV_K& BrevK, const double& dTheta, double& dVeL, double& dMeL, double& dVeT, double& dMeT)
{
    dVeL = 0.0;
    dMeL = 0.0;
    dVeT = 0.0;
    dMeT = 0.0;

    T_BREV_D BrevD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) return;

    CArray<T_EVGP_K, T_EVGP_K> arEvgp;
    arEvgp.RemoveAll();
    arEvgp.Add(BrevD.EvgpK);

    INT_PTR nEvgp = arEvgp.GetSize();
    for ( INT_PTR i=0; i<nEvgp; ++i )
    {
        T_EVGP_D EvgpD;
        if ( !m_pDoc->m_pAttrCtrl2->GetEvgp(arEvgp[i], EvgpD) ) return;

        if ( EvgpD.nType==0 ) // 0:Pier, 1:Abutment
        {
            INT_PTR nCol = EvgpD.aEvgpColm.GetSize();
            for ( INT_PTR j=0; j<nCol; ++j )
            {
                T_BRDGEVAL_FORCE ForceD;
                Calc_PierSeisEnvForce(BrevD.EvgpK, j, TOP, 0, ForceD);
                ConvertToBearing(dTheta, ForceD.dForce[2], ForceD.dForce[1]);
                dVeL += ForceD.dForce[2];
                dVeT += ForceD.dForce[1];

                Calc_PierSeisEnvForce(BrevD.EvgpK, j, BOTTOM, 0, ForceD);
                ConvertToBearing(dTheta, ForceD.dForce[4], ForceD.dForce[5]);
                dMeL += ForceD.dForce[4];
                dMeT += ForceD.dForce[5];
            }
        }
        else
        {
            T_LCOM_K_LIST arLcomK;
            m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, arLcomK);
            INT_PTR nLcom = arLcomK.GetSize();

            if ( EvgpD.nLink==1 ) // Node
            {
                // Node reaction
                INT_PTR nNode = EvgpD.aSupport.GetSize();

                T_REAC_D ReacMax; ReacMax.Initialize();
                for ( int j=0; j<nLcom; ++j )
                {
                    T_LCOM_D LcomD;
                    if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[j], LcomD) ) continue;
                    if ( LcomD.nSeisLcomType == 0 ) continue;
                    m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

                    // Get reaction
                    T_REAC_D ReacSum; ReacSum.Initialize();
                    for ( INT_PTR k=0; k<nNode; ++k )
                    {
                        T_REAC_D ReacMaxData;  ReacMaxData.Initialize();
                        m_pDoc->m_pPostCtrl->GetReacNew(EvgpD.aSupport[k], &ReacMaxData, NULL, NULL); // Load Combi에 대한 반력 
                        ReacSum.Add(ReacMaxData);
                    }

                    ConvertToBearing(dTheta, ReacSum.dblReac[0], ReacSum.dblReac[1]);
                    ConvertToBearing(dTheta, ReacSum.dblReac[4], ReacSum.dblReac[3]);

                    ReacMax.AbsMax(ReacSum);
                }

                dVeL = ReacMax.dblReac[0];
                dVeT = ReacMax.dblReac[1];
                dMeL = ReacMax.dblReac[4];
                dMeT = ReacMax.dblReac[3];
            }
            else
            {
                // Link Force
                CArray<T_ELNK_K, T_ELNK_K> aElnkK, aNlnkK;
                int nLink = m_pDoc->m_pAttrCtrl2->GetLinkList(EvgpD.aLink, aElnkK, aNlnkK);
                int nElnk = aElnkK.GetSize();
                int nNlnk = aNlnkK.GetSize();

                for ( int j=0; j<nLcom; ++j )
                {
                    T_LCOM_D LcomD;
                    if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[j], LcomD) ) continue;
                    if ( LcomD.nSeisLcomType == 0 ) continue;
                    m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

                    double dCurVeL = 0.0;
                    double dCurVeT = 0.0;
                    double dCurMeL = 0.0;
                    double dCurMeT = 0.0;

                    for ( int j=0; j<nElnk; ++j )
                    {
                        T_ELNK_K LinkK = aElnkK[j];

                        double dLocalVector[3][3];
                        T_ELNK_D LinkD;
                        if ( !m_pDoc->m_pAttrCtrl->GetElnk(LinkK, LinkD) ) continue;
                        T_NODE_D Node1D, Node2D;
                        if ( !m_pDoc->m_pAttrCtrl->GetNode(LinkD.Node1, Node1D) ) continue;
                        if ( !m_pDoc->m_pAttrCtrl->GetNode(LinkD.Node2, Node2D) ) continue;
                        if ( !m_pDoc->calcFrameLocalVector(Node1D, Node2D, 0.0/*LinkD.dBetaAngle*/, dLocalVector) ) continue;

						T_STEK_D StekMax; StekMax.Initialize();
						T_STEK_D StekMin; StekMin.Initialize();
						m_pDoc->m_pPostCtrl->GetStekNew(LinkK, &StekMax, &StekMin, NULL); // Load Combi에 대한 Link Force    
						// mathUCS2GCS 전 : StekMax.dblForce[0][0], StekMax.dblForce[0][1], StekMax.dblForce[0][2] => Link Local에 대한 x, y, z force
						// mathUCS2GCS 후 : StekMax.dblForce[0][0], StekMax.dblForce[0][1], StekMax.dblForce[0][2] => Global 축에 대한 X, Y, Z force => X, Y 값이 전단 성분임.
						CMathFunc::mathUCS2GCS(StekMax.dblForce[0][0], StekMax.dblForce[0][1], StekMax.dblForce[0][2], dLocalVector);
						CMathFunc::mathUCS2GCS(StekMin.dblForce[0][0], StekMin.dblForce[0][1], StekMin.dblForce[0][2], dLocalVector);

						double dForce00 = max(fabs(StekMax.dblForce[0][0]), fabs(StekMin.dblForce[0][0]));
						double dForce01 = max(fabs(StekMax.dblForce[0][1]), fabs(StekMin.dblForce[0][1]));
						double dForce03 = max(fabs(StekMax.dblForce[0][3]), fabs(StekMin.dblForce[0][3]));
						double dForce04 = max(fabs(StekMax.dblForce[0][4]), fabs(StekMin.dblForce[0][4]));

						ConvertToBearing(dTheta, dForce00, dForce01);
						ConvertToBearing(dTheta, dForce04, dForce03);

						dCurVeL += dForce00;
						dCurVeT += dForce01;
						dCurMeL += dForce04;
						dCurMeT += dForce03;

                        //ConvertToBearing(dTheta, StekMax.dblForce[0][2], StekMax.dblForce[0][1]);
                        //ConvertToBearing(dTheta, StekMax.dblForce[0][4], StekMax.dblForce[0][3]);
                        //dCurVeL += StekMax.dblForce[0][2];
                        //dCurVeT += StekMax.dblForce[0][1];
                        //dCurMeL += StekMax.dblForce[0][4];
                        //dCurMeT += StekMax.dblForce[0][3];
                    }


                    for ( int i=0; i<nNlnk; ++i )
                    {
                        T_NLNK_K LinkK = aNlnkK[i];

                        double dLocalVector[3][3];
                        T_NLNK_D LinkD;
                        if ( !m_pDoc->m_pAttrCtrl->GetNlnk(LinkK, LinkD) ) continue;
                        T_NODE_D Node1D, Node2D;
                        if ( !m_pDoc->m_pAttrCtrl->GetNode(LinkD.Node1, Node1D) ) continue;
                        if ( !m_pDoc->m_pAttrCtrl->GetNode(LinkD.Node2, Node2D) ) continue;
                        if ( !m_pDoc->calcFrameLocalVector(Node1D, Node2D, 0.0/*LinkD.dBetaAngle*/, dLocalVector) ) continue;

                        T_STEK_D StekMax; StekMax.Initialize();
						T_STEK_D StekMin; StekMin.Initialize();
                        m_pDoc->m_pPostCtrl->GetNlnkNew(LinkK, &StekMax, &StekMin, NULL); // Load Combi에 대한 Link Force     
						// mathUCS2GCS 전 : StekMax.dblForce[0][0], StekMax.dblForce[0][1], StekMax.dblForce[0][2] => Link Local에 대한 x, y, z force
						// mathUCS2GCS 후 : StekMax.dblForce[0][0], StekMax.dblForce[0][1], StekMax.dblForce[0][2] => Global 축에 대한 X, Y, Z force => X, Y 값이 전단 성분임.
						CMathFunc::mathUCS2GCS(StekMax.dblForce[0][0], StekMax.dblForce[0][1], StekMax.dblForce[0][2], dLocalVector);
						CMathFunc::mathUCS2GCS(StekMin.dblForce[0][0], StekMin.dblForce[0][1], StekMin.dblForce[0][2], dLocalVector);

						double dForce00 = max(fabs(StekMax.dblForce[0][0]), fabs(StekMin.dblForce[0][0]));
						double dForce01 = max(fabs(StekMax.dblForce[0][1]), fabs(StekMin.dblForce[0][1]));
						double dForce03 = max(fabs(StekMax.dblForce[0][3]), fabs(StekMin.dblForce[0][3]));
						double dForce04 = max(fabs(StekMax.dblForce[0][4]), fabs(StekMin.dblForce[0][4]));

						ConvertToBearing(dTheta, dForce00, dForce01);
						ConvertToBearing(dTheta, dForce04, dForce03);

						dCurVeL += dForce00;
						dCurVeT += dForce01;
						dCurMeL += dForce04;
						dCurMeT += dForce03;

                        //ConvertToBearing(dTheta, StekMax.dblForce[0][2], StekMax.dblForce[0][1]);
                        //ConvertToBearing(dTheta, StekMax.dblForce[0][4], StekMax.dblForce[0][3]);

                        //dCurVeL += StekMax.dblForce[0][2];
                        //dCurVeT += StekMax.dblForce[0][1];
                        //dCurMeL += StekMax.dblForce[0][4];
                        //dCurMeT += StekMax.dblForce[0][3];
                    }

                    if ( dVeL < fabs(dCurVeL) )    dVeL = fabs(dCurVeL);
                    if ( dVeT < fabs(dCurVeT) )    dVeT = fabs(dCurVeT);
                    if ( dMeL < fabs(dCurMeL) )    dMeL = fabs(dCurMeL);
                    if ( dMeT < fabs(dCurMeT) )    dMeT = fabs(dCurMeT);
                }
            }
        }
    }
}

void CBrdgSeisEvalKISTEC2015::Get_Fn(const T_BREV_K& BrevK, double& dTheta, double& dfnL, double& dfnT, BOOL& bRoundSect)
{
    dfnL = 0.0;
    dfnT = 0.0;

    T_BREV_D BrevD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) return;

    return Get_Fn(BrevD, dTheta, dfnL, dfnT, bRoundSect);
}

void CBrdgSeisEvalKISTEC2015::Get_Fn(const T_BREV_D& BrevD, double& dTheta, double& dfnL, double& dfnT, BOOL& bRoundSect)
{
    dfnL = 0.0;
    dfnT = 0.0;

	dTheta = 0.0; // 상대각도 고려이후 이부분 무시
    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); 
	if ( !pResult->ExistResult() )
	{ 
		T_EVGP_D EvgpD;
		if (!m_pDoc->m_pAttrCtrl2->GetEvgp(BrevD.EvgpK, EvgpD)) return;

		if (EvgpD.nType == 0) // 0:Pier, 1:Abutment
		{
			dfnL = 1.0;
			dfnT = 1.0;
		}
		return;
	}

    CArray<T_EVGP_K, T_EVGP_K> arEvgp;
    arEvgp.RemoveAll();
    arEvgp.Add(BrevD.EvgpK);

    INT_PTR nEvgp = arEvgp.GetSize();
    INT_PTR nChk = 0;
    INT_PTR nTotalCol = 0;
    for ( INT_PTR i=0; i<nEvgp; ++i )
    {
        T_EVGP_D EvgpD;
        if ( !m_pDoc->m_pAttrCtrl2->GetEvgp(arEvgp[i], EvgpD) ) continue;

        if ( EvgpD.nType==0 ) // 0:Pier, 1:Abutment
        {
            INT_PTR nCol = EvgpD.aEvgpColm.GetSize();
            nTotalCol += nCol;
            for ( INT_PTR j=0; j<nCol; ++j )
            {
                T_BRDGEVAL_EVALUATION EvalLD;
                if ( !pResult->ReadSeisEvalResultEval (arEvgp[i], j, LONGI, BOTTOM, 0, TRUE, EvalLD) ) continue;
                if ( !EvalLD.bCheck ) continue;

                T_BRDGEVAL_EVALUATION EvalTD;
                if ( !pResult->ReadSeisEvalResultEval (arEvgp[i], j, TRANS, BOTTOM, 0, TRUE, EvalTD) ) continue;
                if ( !EvalTD.bCheck ) continue;

                double dCurfnL = EvalLD.PalinConcD.bPlainConc ? EvalLD.PalinConcD.dVn : EvalLD.ModeD.dFn;
				double dCurfnT = EvalTD.PalinConcD.bPlainConc ? EvalTD.PalinConcD.dVn : EvalTD.ModeD.dFn;
                bRoundSect= EvalTD.ModeD.bRoundSect;

                ConvertToBearing(dTheta, dCurfnL, dCurfnT);

                dfnL += fabs(dCurfnL);
                dfnT += fabs(dCurfnT);
                nChk++;
            }
        }
    }
    // 해각 교각 기둥 중 일부만 검토된 경우 처리
    if ( nChk>0 )
    {
        dfnL *= (static_cast<double>(nTotalCol) / static_cast<double>(nChk));
        dfnT *= (static_cast<double>(nTotalCol) / static_cast<double>(nChk));
    }
}


void CBrdgSeisEvalKISTEC2015::Get_Rs_LabdaDR(const T_BREV_K& BrevK, double& dRsL, double& dLDRL, double& dRsT, double& dLDRT)
{
    dRsL  = 0.0;
    dLDRL = 0.0;
    dRsT  = 0.0;
    dLDRT = 0.0;

    T_BREV_D BrevD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) return;

    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if ( !pResult ) { ASSERT(0); }

    CArray<T_EVGP_K, T_EVGP_K> aEvgpK;
    aEvgpK.RemoveAll();
    aEvgpK.Add(BrevD.EvgpK); //??//

    INT_PTR nEvgp = aEvgpK.GetSize();
    for ( INT_PTR i=0; i<nEvgp; ++i )
    {
        T_EVGP_D EvgpD;
        if ( !m_pDoc->m_pAttrCtrl2->GetEvgp(aEvgpK[i], EvgpD) ) return;

        if ( EvgpD.nType==0 ) // 0:Pier, 1:Abutment
        {
            INT_PTR nCol = EvgpD.aEvgpColm.GetSize();
            for ( INT_PTR j=0; j<nCol; ++j )
            {
                T_BRDGEVAL_EVALUATION EvalLD;
                if ( !pResult->ReadSeisEvalResultEval (BrevD.EvgpK, j, LONGI, BOTTOM, 0, TRUE, EvalLD) ) continue;
                if ( !EvalLD.bCheck ) continue;

                T_BRDGEVAL_EVALUATION EvalTD;
                if ( !pResult->ReadSeisEvalResultEval (BrevD.EvgpK, j, TRANS, BOTTOM, 0, TRUE, EvalTD) ) continue;
                if ( !EvalTD.bCheck ) continue;

                // 검토 방향별 최대값
                if ( EvalLD.EvalD.dRs > dRsL )
                {
                    dRsL  = EvalLD.EvalD.dRs;
                    dLDRL = EvalLD.EvalD.dLadbdaDR;
                }
                if ( EvalTD.EvalD.dRs > dRsT )
                {
                    dRsT  = EvalTD.EvalD.dRs;
                    dLDRT = EvalTD.EvalD.dLadbdaDR;
                }
            }
        }
    }
}

//======================================================================================================================================================
// KISTEC2019 Anchor Tension
void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_TenSteel(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
    double dAse  = rResD.AnchorTens.dAse;
    double dnA   = rResD.AnchorTens.nnA;
    double dfuta = rResD.AnchorTens.dfua;
    for ( int i=0; i<2; ++i )
    {
        rResD.AnchorD[i].FNSD.dFNSC = dnA*dAse*dfuta;
        rResD.AnchorD[i].FNSD.dFNSD = rResD.ParamD.ForceD.dMaxP;
        rResD.AnchorD[i].FNSD.dRat  = SafeDiv(rResD.AnchorD[i].FNSD.dFNSC, rResD.AnchorD[i].FNSD.dFNSD, _INFI4); // rResD.ParamD.ForceD.dMaxP is always positive.

        rResD.AnchorD[i].FNSD.nn    = dnA;
        rResD.AnchorD[i].FNSD.dAse  = dAse;
        rResD.AnchorD[i].FNSD.dfuta = dfuta;
    }
}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_TenConc(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
	T_BRDGEVAL_BEARING rPosResD = rResD;
	T_BRDGEVAL_BEARING rNegResD = rResD;

	Calc_BearingAnchor_TenConcSub(BrevK, TRUE, rPosResD); // 정방향, 기존
	Calc_BearingAnchor_TenConcSub(BrevK, FALSE, rNegResD);

	//교축, 교직
	for (int i = 0; i < 2; i++)
	{
		if (rPosResD.AnchorD[i].FNCD.dRat <= rNegResD.AnchorD[i].FNCD.dRat)
		{
			rResD.AnchorD[i].FNCD = rPosResD.AnchorD[i].FNCD;
			//rResD.AnchorD[i].FNCD.nRatType = ANCHOR_POS;
		}
		else
		{
			rResD.AnchorD[i].FNCD = rNegResD.AnchorD[i].FNCD;
			//rResD.AnchorD[i].FNCD.nRatType = ANCHOR_NEG;
		}
	}

}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_TenConcSub(const T_BREV_K& BrevK, BOOL bAnckorPos, T_BRDGEVAL_BEARING& rResD)
{
    double dsax  = rResD.ParamD.dSax;
    double dsay  = rResD.ParamD.dSay;
    double dhcop = rResD.ParamD.dhcop;
    double dfck  = rResD.ParamD.dfck;

    double dAse  = rResD.AnchorTens.dAse;
    double dnA   = rResD.AnchorTens.nnA;
    double dfuta = rResD.AnchorTens.dfua;
    T_ANEV_D AnevD;
    T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD, bAnckorPos);
    T_BRPR_D BrprD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BrprK, BrprD) ) BrprD.Initialize();

	//////////////////////////////////////////////////////////////////////////
	int nTotalAnchorL = 0;
	int nTotalAnchorT = 0;
	std::vector<std::vector<T_BRPR_D>> aArBrprD;
	for (int i = 0; i < AnevD.BearingProp.aBrprKs.GetSize(); i++)
	{
		aArBrprD.push_back(std::vector<T_BRPR_D>());
		for (int j = 0; j < AnevD.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			if(CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, i, j) > 0)
			{
				if (!m_pDoc->m_pAttrCtrl2->GetBrpr(CDgnSeisAnchorShapeMgr::GetBrprK(AnevD.BearingProp, i, j), BrprD)) ASSERT(0);
				aArBrprD[i].push_back(BrprD);
			}
			else
			{
				BrprD.Initialize();
				aArBrprD[i].push_back(BrprD);
			}

			//각 방향의 전열에서 고정인 BP
			//각 BP에서 프아이아웃 검토용 앵커 배열 합
			int nBrevType = AnevD.SuppType.aLayer[i].aBrevType[j];
			if(i==0) nTotalAnchorL += (nBrevType == 0 || nBrevType == 3) ? 1/*BrprD.nN1*/ : 0;
			if(j==0) nTotalAnchorT += (nBrevType == 0 || nBrevType == 2) ? 1/*BrprD.nN2*/ : 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////

    CDgnSeisAnchorShapeMgr AnchorShapeMgr(AnevK);
	if(nTotalAnchorL==0 || nTotalAnchorT ==0)
	{
		AnchorShapeMgr.SetAnchorTenConcOption(TRUE);
	}

    BOOL bTypical = IsTypicalBearingArrange(BrevK, AnevD.dRelAngle);

    double dhef    = rResD.AnchorTens.dhef;
    double da      = rResD.AnchorTens.da;

    if ( !bTypical )
    {
        AnchorShapeMgr.GetConcFailSax(LONGI, dsax);
        AnchorShapeMgr.GetConcFailSax(TRANS, dsay);
    }

	std::vector<std::pair<int, int>> aRepBearingIdxNN;
	aRepBearingIdxNN.push_back(std::pair<int, int>(rResD.AnchorTens.nBrI, rResD.AnchorTens.nBrJ));
	double nnAL = AnchorShapeMgr.GetConcFailCa1AnchorNum(LONGI, aRepBearingIdxNN);
	double nnAT = AnchorShapeMgr.GetConcFailCa1AnchorNum(TRANS, aRepBearingIdxNN);

	double dCa1_L, dCa1P_L, dCa2_L, dCa2P_L;
	double dCa1_T, dCa1P_T, dCa2_T, dCa2P_T;
	AnchorShapeMgr.GetPryoutBearingCa(LONGI, rResD.AnchorTens.nBrI, rResD.AnchorTens.nBrJ, dCa1_L, dCa1P_L, dCa2_L, dCa2P_L);
	AnchorShapeMgr.GetPryoutBearingCa(TRANS, rResD.AnchorTens.nBrI, rResD.AnchorTens.nBrJ, dCa1_T, dCa1P_T, dCa2_T, dCa2P_T);

    for ( int i=0; i<2; ++i )
    {
        double dca1Temp = 0.0;
        double dca1pTemp = 0.0;
        double dca2Temp = 0.0;
        double dca2pTemp = 0.0;
        double dLp  = 0.0;
        int nBCnum  = 0;

			// Longi 일때는 그룹으로 검토해서 Ttans 결과만 활용하자..
		double dca1  = min(dCa2_L, dCa2P_L);  // (i == LONGI) ? dCa2P_T : dCa2_L;
        double dca1p = max(dCa2_L, dCa2P_L);  // (i == LONGI) ? dCa2_T  : dCa2P_L;
        double dca2  = min(dCa2_T, dCa2P_T);  // (i == LONGI) ? dCa2_L  : dCa2P_T;
        double dca2p = max(dCa2_T, dCa2P_T);  // (i == LONGI) ? dCa2P_L : dCa2_T;

		//  Anchor Tension은 pryout과는 별개로 검토
//        AnchorShapeMgr.GetConcFailRepresentVal(i, dLp, nBCnum, dca1Temp, dca1pTemp, dca2Temp, dca2pTemp, false);
// 
//         double d15ca1 = 1.5*dca1Temp;
//         double dca1org = dca1Temp;
//         // 콘크리트파괴 저항면적 상정시 3면 이상 가장자리의 영향 검토
//         BOOL bChk2face = (dLp < 2.0*d15ca1) ? TRUE : FALSE;
//         BOOL bEdgeEff  = Check_Inter3Faces(dca1Temp, dca2Temp, dca2pTemp, dhcop, (i==0 ? dsax : dsay), dLp);
// 
//         int n1AnchorType = RESIS_MULTIANCHOR;
// 
//         if ( bEdgeEff && (fabs(dca1org-dca1Temp) > cDgn_Zero) )
//         {
//             if ( !rResD.ParamD.bUserConc )
//             {
//                 // 3면이 가장자리의 영향을 받으므로 하중작용방향으로 연단거리 재산정 필요 - Check_Inter3Faces 재산정함.
//                 d15ca1 = 1.5*dca1Temp;
// 
//                 if ( !bTypical )
//                 {
//                     double dca1_recal = dca1Temp;
//                     int nBCnum=0;
//                     AnchorShapeMgr.GetConcFailRepresentVal(i, dLp, nBCnum, dca1Temp, dca1pTemp, dca2Temp, dca2pTemp);
//                 }
//             }
//             else
//             {
//                 dca1Temp = dca1org;
//             }
//         }
// 
//         double dca1  = rResD.AnchorD[i].FAPD.dca1;
//         double dca1p = rResD.AnchorD[i].FAPD.dca1p;
//         double dca2  = rResD.AnchorD[i].FAPD.dca2;
//         double dca2p = rResD.AnchorD[i].FAPD.dca2p;
// 
//         if ( !bTypical )
//         {
// 			// Longi 일때는 그룹으로 검토해서 Ttans 결과만 활용하자..
//             dca1  = (i == LONGI) ? dCa2P_T : dCa2_L;
//             dca1p = (i == LONGI) ? dCa2_T  : dCa2P_L;
//             dca2  = (i == LONGI) ? dCa2_L  : dCa2P_T;
//             dca2p = (i == LONGI) ? dCa2P_L : dCa2_T;
//         }
// 
// 		//////////////////////////////////////////////////////////////////////////
// 		double dca1 = 0.0;
// 		double dca1p = 0.0;
// 		double dca2 = 0.0;
// 		double dca2p = 0.0;
// 		double dANc = 0.0;
// 		int nBC = 0;
// 		BOOL bTypical = IsTypicalBearingArrange(BrevK);
// 		int n1AnchorType = RESIS_MULTIANCHOR;
// 		int  n1AnchorAllResist = 0;
// 		if (rResD.ParamD.bUserPryOut)
// 		{
// 			const T_ANEV_PRYOUT_D& PryoutD = i == 0 ? AnevD.Fail.PryoutL : AnevD.Fail.PryoutT;
// 
// 			dca1 = PryoutD.dCa1;
// 			dca1p = PryoutD.dCa1_q;
// 			dca2 = PryoutD.dCa2;
// 			dca2p = PryoutD.dCa2_q;
// 
// 			// 			nBC = static_cast<double>(PryoutD.nNum);
// 			// 			dANc = PryoutD.dANc;
// 			// 			dANco = PryoutD.dANco;
// 			//			dhef = PryoutD.dHef;
// 		}
// 		else if (bTypical)
// 		{
// 			Get_ArrangeEdgeDimsPryo(BrevK, (i == 0 ? LONGI : TRANS), dca1, dca1p, dca2, dca2p);
// 		}
// 		else
// 		{
// 			int nBCnum = 0;
// 			BOOL bSingleResist = FALSE;
// 			AnchorShapeMgr.GetPryoutRepresentVal(i, dANc, nBCnum, dca1, dca1p, dca2, dca2p, bSingleResist, n1AnchorAllResist);
// 			if (bSingleResist && n1AnchorAllResist > 0)
// 			{
// 				n1AnchorType = n1AnchorAllResist == 1 ? RESIST_1ANCHOR_1 : RESIST_1ANCHOR_ALL;
// 			}
// 			nBC = static_cast<double>(nBCnum);
// 		}
		//////////////////////////////////////////////////////////////////////////
		double dhef = rResD.AnchorTens.dhef;
		double d15hef = 1.5*dhef;

// 		double dhef  = 0.0;
//         if ( rResD.ParamD.bUserPryOut ) dhef = rResD.ParamD.dhefPryo[i];
//         else                         dhef = rResD.AnchorTens.dhef;
//         double d15hef = 1.5*dhef;
// 
//         dsax = i==0 ? rResD.AnchorD[i].FAPD.dsax : rResD.AnchorD[i].FAPD.dsay;
//         dsay = i==0 ? rResD.AnchorD[i].FAPD.dsay : rResD.AnchorD[i].FAPD.dsax;
// 
//         if ( dsax != 0 && dsay != 0 )
//         {
//             dca1p = min(dca1p, dsay);
//             dca2p = min(dca2p, dsax);
//         }
// 
//         double ds1 = rResD.AnchorD[i].FAPD.ds1;
//         double ds2 = rResD.AnchorD[i].FAPD.ds2;
//         double dm  = rResD.AnchorD[i].FAPD.nnA;
//         double dn  = rResD.AnchorD[i].FAPD.nnB;
		double ds1 = (i == 0) ? rResD.AnchorTens.dS1 : rResD.AnchorTens.dS2;
		double ds2 = (i == 0) ? rResD.AnchorTens.dS2 : rResD.AnchorTens.dS1;
		double dm  = (i == 0) ? rResD.AnchorTens.nnT : rResD.AnchorTens.nnL;
		double dn  = (i == 0) ? rResD.AnchorTens.nnL : rResD.AnchorTens.nnT;

        int    nProjectType=0; // 1~4
        double dAncCal  = 0.0;
		int nn = 0;
        if ( ds1 < 2.0*d15hef && ds2 < 2.0*d15hef )
        {
			nn = nnAL * nnAT;
            nProjectType = 1;
            dAncCal = (min(dca2, d15hef) + (dm-1.0)*ds2 + min(dca2p, d15hef)) *
                      (min(dca1, d15hef) + (dn-1.0)*ds1 + min(dca1p, d15hef));
        }
        else if ( ds1 < 2.0*1.5*dhef )
        {
			nn = nnAL;
            nProjectType = 2;
            dAncCal = (min(dca2, d15hef) + (dm-1.0)*ds1 + min(dca2p, d15hef)) *
                (min(dca1, d15hef) + d15hef);
        }
        else if ( ds2 < 2.0*1.5*dhef )
        {
			nn = nnAT;
            nProjectType = 3;
            dAncCal = (min(dca2, d15hef) + d15hef) *
                (min(dca1, d15hef) + (dn-1.0)*ds2 + min(dca1p, d15hef));
        }
        else
        {
			nn = 1;
            nProjectType = 4;
            dAncCal = (min(dca2, d15hef) + d15hef) *
                (min(dca1, d15hef) + d15hef);
        }

		//int nn = (i == LONGI) ? nnAL : nnAT; //(dm*dn) => 영역에서 검토되는 앵커 수
        double dAnco = 9.0 * dhef * dhef;
        double dAnc = (dAncCal < /*(dm*dn)*/nn*dAnco) ? dAncCal : nn*dAnco;
        double dpsi = 1.0;
        double dcamin = min(min(dca1, dca1p), min(dca2, dca2p));
        if ( dcamin < d15hef ) dpsi = 0.7 + 0.3*(dcamin/d15hef);
        double dpsi_cN = Get_psi_cN(rResD.AnchorTens.enAnchType); // KALIS2023 사용자
		double dNb = 0.0; // ResD.AnchorD[i].FAPD.dNb;
		if (rResD.AnchorTens.enAnchType == ANCH_CAST_HEADED_STUD || rResD.AnchorTens.enAnchType == ANCH_CAST_HEADED_BOLT ||
			rResD.AnchorTens.enAnchType == ANCH_CAST_HOOKEDBOLTL || rResD.AnchorTens.enAnchType == ANCH_CAST_HOOKEDBOLTJ)
		{
			if (dhef < 280.0 || dhef > 635.0) // 재산정된 hef가 맞음. 예제집이 틀렸음.
			{
				//iNbType = 1;
				dNb = 10.0 * sqrt(dfck) * pow(dhef, 1.5);
			}
			else
			{
				//iNbType = 3;
				dNb = 3.9 * sqrt(dfck) * pow(dhef, 5.0 / 3.0);
			}
		}
		else
		{
			//iNbType = 2;
			dNb = 7.0 * sqrt(dfck) * pow(dhef, 1.5);
		}

        double dNcbg = dAnc/dAnco * dpsi * dpsi_cN * dNb;
		double nnA = rResD.AnchorD[i].FNSD.nn; //*(dm*dn)*/

        rResD.AnchorD[i].FNCD.dFNCC = dNcbg;
        rResD.AnchorD[i].FNCD.dFNCDCal = rResD.ParamD.ForceD.dMaxP;
		rResD.AnchorD[i].FNCD.dFNCD = rResD.ParamD.ForceD.dMaxP * nn / nnA/*(dm*dn)*/;
        rResD.AnchorD[i].FNCD.dRat  = SafeDiv(rResD.AnchorD[i].FNCD.dFNCC, rResD.AnchorD[i].FNCD.dFNCD, _INFI4); // rResD.ParamD.ForceD.dMaxP is always positive.

        rResD.AnchorD[i].FNCD.dca1  = dca1;
        rResD.AnchorD[i].FNCD.dca1p = dca1p;
        rResD.AnchorD[i].FNCD.dca2  = dca2;
        rResD.AnchorD[i].FNCD.dca2p = dca2p;
		rResD.AnchorD[i].FNCD.ds1   = ds1;
		rResD.AnchorD[i].FNCD.ds2   = ds2;
        rResD.AnchorD[i].FNCD.dhef  = dhef;
        rResD.AnchorD[i].FNCD.nProjectType  = nProjectType; // 1~4
		rResD.AnchorD[i].FNCD.nn  = nn;  
		rResD.AnchorD[i].FNCD.nnA = nnA;
        rResD.AnchorD[i].FNCD.dANco = dAnco;
        rResD.AnchorD[i].FNCD.dANcorg = dAncCal;
        rResD.AnchorD[i].FNCD.dANc  = dAnc;
        rResD.AnchorD[i].FNCD.dpsi_edN = dpsi;
        rResD.AnchorD[i].FNCD.dpsi_cN = dpsi_cN;
        rResD.AnchorD[i].FNCD.dNb   = dNb;
        rResD.AnchorD[i].FNCD.dNcbg = dNcbg; // from T_BRDGEVAL_BEARING_ANCHOR_FAP
    }

}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_TenPull(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
    double dfck  = rResD.ParamD.dfck;

    double dAbrg = rResD.AnchorTens.dAbrg;
    double deh   = rResD.AnchorTens.deh;
    double dda   = (rResD.AnchorTens.nStlBearing/*rResD.ParamD.enBearing*/ == BEARING_STEEL_POTo) ? rResD.AnchorTens.ds : rResD.AnchorTens.da;

    double dNp = 0.0;
    if ( rResD.AnchorTens.enAnchType == ANCH_POST_EXPANSIONTC ||
         rResD.AnchorTens.enAnchType == ANCH_POST_UNDERCUTANC ||
         rResD.AnchorTens.enAnchType == ANCH_POST_EXPANSIONDC ||
         rResD.AnchorTens.enAnchType == ANCH_POST_ADHESIVEANC )
    {    
        dNp = rResD.AnchorTens.dNp; // User Input
    }
    else if ( rResD.AnchorTens.enAnchType == ANCH_CAST_HEADED_STUD ||
        rResD.AnchorTens.enAnchType == ANCH_CAST_HEADED_BOLT )
    {
        dNp = 8.0 * dAbrg * dfck;
    }
    else
    {
        dNp = 0.9 * dfck * deh * dda;
    }
    double dNpn = 1.0 * dNp; // ψc,p = 1.0

    for ( int i=0; i<2; ++i )
    {
        rResD.AnchorD[i].FNPD.dFNPC = rResD.AnchorTens.nnA*dNpn;
        rResD.AnchorD[i].FNPD.dFNPD = rResD.ParamD.ForceD.dMaxP;
        rResD.AnchorD[i].FNPD.dRat  = SafeDiv(rResD.AnchorD[i].FNPD.dFNPC, rResD.AnchorD[i].FNPD.dFNPD, _INFI4);

        rResD.AnchorD[i].FNPD.dNpn   = dNpn;
        rResD.AnchorD[i].FNPD.dNp    = dNp;
        rResD.AnchorD[i].FNPD.deh    = deh;
        rResD.AnchorD[i].FNPD.nn     = rResD.AnchorTens.nnA;
    }

}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_TenSide(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
    T_ANEV_D AnevD;
    T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);
    T_BRPR_D BrprD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BrprK, BrprD) ) BrprD.Initialize();

    CDgnSeisAnchorShapeMgr AnchorShapeMgr(AnevK);

    BOOL bTypical= IsTypicalBearingArrange(BrevK, AnevD.dRelAngle);

    if ( rResD.AnchorTens.enAnchType != ANCH_CAST_HEADED_STUD &&
        rResD.AnchorTens.enAnchType != ANCH_CAST_HEADED_BOLT )
    {
        rResD.AnchorD[0].FNRD.bCheck = FALSE;
        rResD.AnchorD[1].FNRD.bCheck = FALSE;
        rResD.AnchorD[0].FNRD.nType = 1;
        rResD.AnchorD[1].FNRD.nType = 1;
        return;
    }

    for ( int i=0; i<2; ++i )
    {
        double dca1Temp = 0.0;
        double dca1pTemp = 0.0;
        double dca2Temp = 0.0;
        double dca2pTemp = 0.0;
        double dLp  = 0.0;
        int nBCnum  = 0;

        AnchorShapeMgr.GetConcFailRepresentVal(i, dLp, nBCnum, dca1Temp, dca1pTemp, dca2Temp, dca2pTemp, false);

        double dca1  = rResD.AnchorD[i].FAPD.dca1;
        if ( dca1 == 0 && !bTypical ) dca1 = dca1Temp;

        double dhef  = 0.0;
        if ( rResD.ParamD.bUserPryOut ) dhef = rResD.ParamD.dhefPryo[i];
        else                         dhef = rResD.AnchorTens.dhef;

        if ( dhef < 2.5*dca1 )
        {
            rResD.AnchorD[i].FNRD.nType = 2;
        }
        else
        {
            rResD.AnchorD[i].FNRD.nType = 3;

            double ds1   = rResD.AnchorD[i].FAPD.ds1;
            double dAbrg = rResD.AnchorTens.dAbrg;
            double dfck  = rResD.ParamD.dfck;
            double dNsb  = 13.0*dca1*sqrt(dAbrg)*sqrt(dfck);
            double dNsbg = dNsb;
            if ( ds1 < 6.0*dca1 )
            {
                rResD.AnchorD[i].FNRD.nNsbgType = 1;
                dNsbg = (1.0+ds1/(6.0*dca1)) * dNsb;

            }
            else
            {
                rResD.AnchorD[i].FNRD.nNsbgType = 2;
            }

            rResD.AnchorD[i].FNRD.bCheck = TRUE;
            rResD.AnchorD[i].FNRD.dFNRC = dNsbg;
            rResD.AnchorD[i].FNRD.dFNRD = rResD.ParamD.ForceD.dMaxP;
            rResD.AnchorD[i].FNRD.dRat  = SafeDiv(rResD.AnchorD[i].FNRD.dFNRC, rResD.AnchorD[i].FNRD.dFNRD, _INFI4);

            rResD.AnchorD[i].FNRD.ds1   = ds1;
            rResD.AnchorD[i].FNRD.dca1  = dca1;
            rResD.AnchorD[i].FNRD.dNsb  = dNsb;
            rResD.AnchorD[i].FNRD.dNsbg = dNsbg;
        }
    }

}

void CBrdgSeisEvalKISTEC2015::Calc_BearingAnchor_TenRela(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
    for ( int i=0; i<2; ++i )
    {
        double dSmin = DBL_MAX;
        double dNmin = DBL_MAX;

        dSmin = min(dSmin, rResD.AnchorD[i].FASD.dRat);
        dSmin = min(dSmin, rResD.AnchorD[i].FACD.dRat);
        dSmin = min(dSmin, rResD.AnchorD[i].FAPD.dRat);

        dNmin = min(dNmin, rResD.AnchorD[i].FNSD.dRat);
        dNmin = min(dNmin, rResD.AnchorD[i].FNCD.dRat);
        dNmin = min(dNmin, rResD.AnchorD[i].FNPD.dRat);
        if ( rResD.AnchorD[i].FNRD.bCheck )
        {
            dNmin = min(dNmin, rResD.AnchorD[i].FNRD.dRat);
        }

        if ( 5.0 < dSmin+cDgn_Zero )
        {
            rResD.AnchorD[i].FNVD.nType = 1;
            rResD.AnchorD[i].FNVD.dRat  = dNmin;
            rResD.AnchorD[i].FNVD.dRatLim  = 1.0;
        }
        else if ( 5.0 < dNmin+cDgn_Zero )
        {
            rResD.AnchorD[i].FNVD.nType = 2;
            rResD.AnchorD[i].FNVD.dRat  = dSmin;
            rResD.AnchorD[i].FNVD.dRatLim  = 1.0;
        }
        else
        {
            rResD.AnchorD[i].FNVD.nType = 3;
            rResD.AnchorD[i].FNVD.dRat  = dNmin + dSmin;
            rResD.AnchorD[i].FNVD.dRatLim  = 5.0/6.0;
        }

        rResD.AnchorD[i].FNVD.dRatN = dNmin;
        rResD.AnchorD[i].FNVD.dRatS = dSmin;
    }
}

double CBrdgSeisEvalKISTEC2015::Get_MaxTension(const T_BREV_K& BrevK, const T_LCOM_K& LcomK)
{
    double dN = 0.0;

    T_BREV_D BrevD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) return dN;

    CArray<T_EVGP_K, T_EVGP_K> aEvgpK;
    aEvgpK.RemoveAll();
    aEvgpK.Add(BrevD.EvgpK); //??//   

    CArray<T_LCOM_K, T_LCOM_K> arLcomK; arLcomK.RemoveAll();
    if ( LcomK==0 )
    {
        m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, arLcomK);
    }
    else
    {
        arLcomK.Add(LcomK);
    }

    INT_PTR nLcom = arLcomK.GetSize();
    for ( INT_PTR i=0; i<nLcom; ++i )
    {
        T_LCOM_D LcomD;
        if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[i], LcomD) ) continue;
        if ( LcomD.nSeisLcomType == 0 ) continue;
        m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

        INT_PTR nEvgp = aEvgpK.GetSize();
        for ( INT_PTR j=0; j<nEvgp; ++j )
        {
            T_EVGP_D EvgpD;
            if ( !m_pDoc->m_pAttrCtrl2->GetEvgp(aEvgpK[j], EvgpD) ) continue;;

            if ( EvgpD.aLink.GetSize() > 0 ) // 0:Pier or 1:Abutment and Link
            {
                CArray<T_ELNK_K, T_ELNK_K> aElnkK, aNlnkK;
                int nLink = m_pDoc->m_pAttrCtrl2->GetLinkList(EvgpD.aLink, aElnkK, aNlnkK);
                int nElnk = aElnkK.GetSize();
                for ( int k=0; k<nElnk; ++k )
                {
                    T_ELNK_K LinkK = aElnkK[k];
                    double dFx = -1.0*GetLinkForce(LinkK, 0, 0); // minmax 타입에 따라서 0:all, 1:max, 2:min
                    dN = max(dN, dFx);
                }

                int nNlnk = aNlnkK.GetSize();
                for ( int k=0; k<nNlnk; ++k )
                {
                    T_NLNK_K LinkK = aNlnkK[k];
                    double dFx = -1.0*GetLinkForce(LinkK, 0, 1); // minmax 타입에 따라서 0:all, 1:max, 2:min
                    dN = max(dN, dFx);
                }
            }
            else
            {
                int nSup = EvgpD.aSupport.GetSize();
                for ( int k=0; k<nSup; ++k )
                {
                    T_NODE_K NodeK = EvgpD.aSupport[k];
                    T_REAC_D ReacMaxData;  ReacMaxData.Initialize();
                    m_pDoc->m_pPostCtrl->GetReacNew(NodeK, &ReacMaxData, NULL, NULL); // Load Combi에 대한 반력 
                    dN = max(dN, -1.0 * ReacMaxData.dblReac[2]);
                }
            }
        }
    }

    return dN;
}

double CBrdgSeisEvalKISTEC2015::Get_MaxTensionMulti(const T_BREV_K& BrevK, const int nBI, const int nBJ, CArray<T_ELNK_K, T_ELNK_K>& aElnkK, CArray<T_NLNK_K, T_NLNK_K>& aNlnkK, const T_LCOM_K& LcomK)
{
    double dN = 0.0;

    T_BREV_D BrevD;
    if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD) ) return dN;

    CArray<T_EVGP_K, T_EVGP_K> aEvgpK;
    aEvgpK.RemoveAll();
    aEvgpK.Add(BrevD.EvgpK); //??//   

    CArray<T_LCOM_K, T_LCOM_K> arLcomK; arLcomK.RemoveAll();
    if ( LcomK==0 )
    {
        m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, arLcomK);
    }
    else
    {
        arLcomK.Add(LcomK);
    }

    INT_PTR nLcom = arLcomK.GetSize();
    for ( INT_PTR i=0; i<nLcom; ++i )
    {
        T_LCOM_D LcomD;
        if ( !m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[i], LcomD) ) continue;
        if ( LcomD.nSeisLcomType == 0 ) continue;
        m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

        INT_PTR nEvgp = aEvgpK.GetSize();
        for ( INT_PTR j=0; j<nEvgp; ++j )
        {
            T_EVGP_D EvgpD;
            if ( !m_pDoc->m_pAttrCtrl2->GetEvgp(aEvgpK[j], EvgpD) ) continue;;

            if ( EvgpD.aLink.GetSize() > 0 ) // 0:Pier or 1:Abutment and Link
            {
                int nElnk = aElnkK.GetSize();
                for ( int k=0; k<nElnk; ++k )
                {
                    T_ELNK_K LinkK = aElnkK[k];
                    double dFx = -1.0*GetLinkForce(LinkK, 0, 0); // minmax 타입에 따라서 0:all, 1:max, 2:min
                    dN = max(dN, dFx);
                }

                int nNlnk = aNlnkK.GetSize();
                for ( int k=0; k<nNlnk; ++k )
                {
                    T_NLNK_K LinkK = aNlnkK[k];
                    double dFx = -1.0*GetLinkForce(LinkK, 0, 1); // minmax 타입에 따라서 0:all, 1:max, 2:min
                    dN = max(dN, dFx);
                }
            }
            else
            {
                int nSup = EvgpD.aSupport.GetSize();
                for ( int k=0; k<nSup; ++k )
                {
                    T_NODE_K NodeK = EvgpD.aSupport[k];
                    T_REAC_D ReacMaxData;  ReacMaxData.Initialize();
                    m_pDoc->m_pPostCtrl->GetReacNew(NodeK, &ReacMaxData, NULL, NULL); // Load Combi에 대한 반력 
                    dN = max(dN, -1.0 * ReacMaxData.dblReac[2]);
                }
            }
        }
    }

    return dN;
}

void CBrdgSeisEvalKISTEC2015::ConvertAnchorParam(const int nBI, const int nBJ, const T_BREV_D& BrevD, const T_ANEV_D& AnevD, const T_BRPR_D& BrprD, T_BRDGEVAL_BEARING_PARAM ParamD, T_BRDGEVAL_ANCHOR_PARAM& rAnchorParam)
{
    rAnchorParam.strName = BrprD.strName;
    rAnchorParam.ds   = BrprD.dds;
    rAnchorParam.dhef = BrprD.dhef; // 앵커근입깊이
    rAnchorParam.da = BrprD.dda;
    rAnchorParam.dS1 = BrprD.dS1;
    rAnchorParam.dS2 = BrprD.dS2;
    rAnchorParam.nnL  = static_cast<double>(BrprD.nN1);
    rAnchorParam.nnT  = static_cast<double>(BrprD.nN2);
    rAnchorParam.nnA  = rAnchorParam.nnL * rAnchorParam.nnT;
    rAnchorParam.dfya = BrprD.dfy;
    rAnchorParam.dfuaLim = min(1.9*rAnchorParam.dfya, 860.0); // MPa
    rAnchorParam.dfuaOrg = BrprD.dfuta;
    rAnchorParam.dfua    = min(rAnchorParam.dfuaOrg, rAnchorParam.dfuaLim);
    rAnchorParam.dB =  2.0 * BrprD.dSx + static_cast<double>(BrprD.nN1-1)*BrprD.dS1;
    rAnchorParam.dC =  2.0 * BrprD.dSy + static_cast<double>(BrprD.nN2-1)*BrprD.dS2;
    rAnchorParam.dCapaL = BrprD.dCapaL;
    rAnchorParam.dCapaT = BrprD.dCapaT;
    rAnchorParam.dDispL = BrprD.dDispL;
    rAnchorParam.dDispT = BrprD.dDispT;
    rAnchorParam.nBearing = BrprD.nBearing;

	if(BrprD.nStlBearing== BEARING_STEEL_POTo)
	{
		if(rAnchorParam.nBearing == BEARING_STEEL) rAnchorParam.nStlBearing = BrprD.nStlBearing;
		else                                       rAnchorParam.nStlBearing = BEARING_STEEL_POTx;
	}
	else 
		rAnchorParam.nStlBearing = BrprD.nStlBearing;

	// dnBL, dnBT : 0 or 1
    rAnchorParam.dnBL = Get_BearingNumb(LONGI, nBI, nBJ, rAnchorParam.nBearing, AnevD, BrevD, rAnchorParam.strName);
    rAnchorParam.dnBT = Get_BearingNumb(TRANS, nBI, nBJ, rAnchorParam.nBearing, AnevD, BrevD, rAnchorParam.strName);


    if ( rAnchorParam.nStlBearing == BEARING_STEEL_POTo ) // 포트받침이고 소켓매몰인 경우
    {
        rAnchorParam.dAse = rAnchorParam.ds*rAnchorParam.ds*CMathFunc::m_pi/4.0; // 소켓직경으로
    }
    else
    {
        rAnchorParam.dAse = rAnchorParam.da*rAnchorParam.da*CMathFunc::m_pi/4.0; // 앵커직경으로
    }

    rAnchorParam.deh   = BrprD.deh; // 갈고리볼트바
    double dAbrg = (BrprD.ddh*BrprD.ddh - BrprD.dda*BrprD.dda) * CMathFunc::m_pi / 4.0; //지압면적
    rAnchorParam.dAbrg = max(dAbrg, 0.0);
    rAnchorParam.dNp   = BrprD.dNp;

    switch ( BrprD.nInstall )
    {
        // 0:Cast In Place, 1:Post Installed    
    case 0:
        {
            switch ( BrprD.nAnchor )
            {
            case 0:
                rAnchorParam.enAnchType = ANCH_CAST_HEADED_STUD;
                break;
            case 1:
                rAnchorParam.enAnchType = ANCH_CAST_HEADED_BOLT;
                break;
            case 2:
                rAnchorParam.enAnchType = ANCH_CAST_HOOKEDBOLTL;
                break;
            case 3:
                rAnchorParam.enAnchType = ANCH_CAST_HOOKEDBOLTJ;
                break;
            default:
                ASSERT(0);
                break;
            }
        }
        break;
    case 1:
        {
            switch ( BrprD.nAnchor )
            {
            case 0:
                rAnchorParam.enAnchType = ANCH_POST_EXPANSIONTC;
                break;
            case 1:
                rAnchorParam.enAnchType = ANCH_POST_EXPANSIONDC;
                break;
            case 2:
                rAnchorParam.enAnchType = ANCH_POST_UNDERCUTANC;
                break;
            case 3:
                rAnchorParam.enAnchType = ANCH_POST_ADHESIVEANC;
                break;
            default:
                ASSERT(0);
                break;
            }
        }
        break;
    default:
        ASSERT(0);
        break;
    }

    if ( rAnchorParam.enAnchType == ANCH_CAST_HEADED_STUD )
    {
        rAnchorParam.dVsParam = 1.0;
    }
    else
    {
        rAnchorParam.dVsParam = 0.6;
    }

    rAnchorParam.dap = rAnchorParam.da;

    //if ( ParamD.enBearing == BEARING_STEEL_POTo )
	if (rAnchorParam.nStlBearing == BEARING_STEEL_POTo)
    {
        rAnchorParam.dap = rAnchorParam.ds;
    }

    if ( rAnchorParam.enAnchType == ANCH_POST_EXPANSIONTC )
    {
        rAnchorParam.dle = 2.0 * rAnchorParam.dap;
    }
    else
    {
        rAnchorParam.dle = min(rAnchorParam.dhef, 8.0 * rAnchorParam.dap);
    }

    rAnchorParam.dVbParam = 0.6;

    if ( BrprD.bContiWeld &&
        (rAnchorParam.enAnchType == ANCH_CAST_HEADED_STUD || rAnchorParam.enAnchType == ANCH_CAST_HEADED_BOLT ||
            rAnchorParam.enAnchType == ANCH_CAST_HOOKEDBOLTL || rAnchorParam.enAnchType == ANCH_CAST_HOOKEDBOLTJ) )
    {
        rAnchorParam.dVbParam = Get_VbFactor();
    }

	double dhefReMM = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, rAnchorParam.dhefRe);;
	double dfckMM   = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, ParamD.dfck);


    if ( rAnchorParam.enAnchType == ANCH_CAST_HEADED_STUD || rAnchorParam.enAnchType == ANCH_CAST_HEADED_BOLT ||
        rAnchorParam.enAnchType == ANCH_CAST_HOOKEDBOLTL || rAnchorParam.enAnchType == ANCH_CAST_HOOKEDBOLTJ )
    {
        if (dhefReMM < 280.0 || dhefReMM > 635.0 ) // 재산정된 hef가 맞음. 예제집이 틀렸음.
        {
            rAnchorParam.nNbType = 1;
            rAnchorParam.dNb = 10.0 * sqrt(dfckMM) * pow(dhefReMM, 1.5);
        }
        else
        {
            rAnchorParam.nNbType = 3;
            rAnchorParam.dNb = 3.9 * sqrt(dfckMM) * pow(dhefReMM, 5.0/3.0);
        }
    }
    else
    {
        rAnchorParam.nNbType = 2;
        rAnchorParam.dNb = 7.0 * sqrt(dfckMM) * pow(dhefReMM, 1.5);
    }

	rAnchorParam.dNb = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_FORCE, rAnchorParam.dNb);
}

double CBrdgSeisEvalKISTEC2015::Get_VbFactor()
{
    return 0.7;
}

double CBrdgSeisEvalKISTEC2015::Calc_Vb(const double& dFactor, const double& dle, const double& dap, const double& dfck, const double& dca1)
{
	double dVb = dFactor * pow((dle / dap), 0.2) * sqrt(dap) * sqrt(dfck) * pow(dca1, 1.5);
    return dVb;
}

double CBrdgSeisEvalKISTEC2015::Calc_Vbmax(const double& dfck, const double& dca1)
{
	return DBL_MAX;
}

double CBrdgSeisEvalKISTEC2015::Calc_FACC(const double& dVcbg, const double& dPu)
{
	return dVcbg;
}

double CBrdgSeisEvalKISTEC2015::Get_psi_cV(const T_ANEV_D& AnevD)
{
    return 1.0;
}

double CBrdgSeisEvalKISTEC2015::Get_psi_cN(const EN_ANCH_TYPE& enAnchType)
{
	return 1.0;
}