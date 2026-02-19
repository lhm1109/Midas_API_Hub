/////////
// DgnSeisAnchorShapeMgr.cpp : implementation file
// 
#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorShapeMgr.h"
#include "BrdgSeisEvalKEC2012.h"
#include "BrdgSeisEvalKISTEC2015.h"
#include "BrdgSeisEvalKISTEC2019.h"
#include "BrdgSeisEvalKALIS2023.h"

#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\MathLib.h"
#include "..\wg_db\InitCtrl.h"
#include "..\wg_db\UnitCtrl.h"

#include <set>
#include <map>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorShapeMgrdialog
CDgnSeisAnchorShapeMgr::CDgnSeisAnchorShapeMgr()
{
	Init();
}

CDgnSeisAnchorShapeMgr::CDgnSeisAnchorShapeMgr(const T_ANEV_K& AnevK)
{
    Init();
    
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    T_ANEV_D AnevD;
    if(!pDoc->m_pAttrCtrl2->GetAnev(AnevK, AnevD)) ASSERT(0);
    T_BRPR_D BrprD;
	std::vector<std::vector<T_BRPR_D>> aArBrprD;
	for (int i = 0; i < AnevD.BearingProp.aBrprKs.GetSize(); i++)
	{
		aArBrprD.push_back(std::vector<T_BRPR_D>());
		for (int j = 0; j < AnevD.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			if(AnevD.BearingProp.aBrprKs[i].aBrprK[j]>0)
			{
				if (!pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) ASSERT(0);
				aArBrprD[i].push_back(BrprD);
			}
			else
			{
				BrprD.Initialize();
				aArBrprD[i].push_back(BrprD);
			}
		}
	}
    
    SetData(aArBrprD, AnevD);
}

CDgnSeisAnchorShapeMgr::~CDgnSeisAnchorShapeMgr()
{
	if(m_pSeisEval != nullptr)
	{
		delete m_pSeisEval;
		m_pSeisEval = nullptr;
	}
}

void CDgnSeisAnchorShapeMgr::Init()
{
	m_bSetBrpr = FALSE;
	m_bSetAnev = FALSE;

	m_dCopyingWidth = 0.0;
	m_dCopyingHeight = 0.0;
	m_aArBearingHeight.clear();
	m_aArBearingWidth.clear();
	m_dCopyingTopBotY[0] = m_dCopyingTopBotY[1] = 0.0;
	m_bAnchorDirPosi = TRUE;

	m_aArBrprD.clear();
	m_AnevD.Initialize();

	m_aArSetCa1Longi.clear();	
	m_aArSetCa1Trans.clear();	

	m_aArCa1Longi.clear();	
	m_aArCa1Trans.clear();	
	m_aArLastCa1Longi.clear();	
	m_aArLastCa1Trans.clear();	

	m_aArSethefLongi.clear();	
	m_aArSethefTrans.clear();	

	m_aArhefLongi.clear();	
	m_aArhefTrans.clear();	
	
	m_dBigZero = 0.00001;

	m_aPryoutGroupInfo.clear();
	m_nPryoutRepresentGrpIdx = -1;
	m_nLastPryoutGroupDir = -1;
	m_bRatioChk = FALSE;
	m_bAnchorTenConc = FALSE;

	m_nDownUp       = DIR_POS;
	m_nLeftRight    = DIR_POS;
	m_nDownUpPryMgr    = DIR_POS;
	m_nLeftRightPryMgr = DIR_POS;
	m_nLIdx = -1;

	m_pSeisEval = []() -> CBrdgSeisEval*
	{
		T_BSED_D BsedD; BsedD.Initialize();
		CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetDgnBsed(BsedD);
		switch(BsedD.nDesignCode)
		{
			case KEC_2012:	    return new CBrdgSeisEvalKEC2012();
			case KISTEC_2015:	return new CBrdgSeisEvalKISTEC2015();
			case KISTEC_2019:	return new CBrdgSeisEvalKISTEC2019();
			case KALIS_2023:	return new CBrdgSeisEvalKALIS2023();
			default:			ASSERT(0);  return new CBrdgSeisEvalKISTEC2015();
		}
	}();
}


BOOL CDgnSeisAnchorShapeMgr::CheckBrpr(T_BRPR_D& BearingData)
{
	Init();
	m_bSetBrpr = TRUE;
	m_aArBrprD.resize(1);
	m_aArBrprD[0].resize(1);
	m_aArBrprD[0][0] = BearingData;

	SetBearingWH();
	BOOL bReturnVal = CheckDataBrpr(0, 0);
	m_bSetBrpr = FALSE;
	return bReturnVal;
}

T_BRPR_K CDgnSeisAnchorShapeMgr::GetBrprK(const T_ANEV_BRPR_D& BearingProp, int nBearingI, int nBearingJ)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_BRPR_D BrprD;
	BrprD.Initialize();
	T_BRPR_K BrprK = BearingProp.aBrprKs[BearingProp.aBrprKs.GetSize() - nBearingI - 1].aBrprK[nBearingJ];		
	return BrprK;
}

void CDgnSeisAnchorShapeMgr::SetData(T_ANEV_D& CopyingData)
{
	Init();
	m_bSetAnev = TRUE;
	m_AnevD =  CopyingData;

	SetCopyingWH();

	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	m_aArSethefLongi.resize(nBearingNumI);
	m_aArSethefTrans.resize(nBearingNumI);

	for (int i = 0; i < nBearingNumI; i++)
	{
		m_aArSethefLongi[i].resize(aBearingNumJ[i]);
		m_aArSethefTrans[i].resize(aBearingNumJ[i]);

		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			m_aArSethefLongi[i][j] = FALSE;
			m_aArSethefTrans[i][j] = FALSE;
		}
	}
}

void CDgnSeisAnchorShapeMgr::SetData(std::vector<std::vector<T_BRPR_D>>& aArBearingData, T_ANEV_D& CopyingData, BOOL bAnckorPos/* = TRUE*/)
{
	Init();
	m_bAnchorDirPosi = bAnckorPos;

// 	if(bAnckorPos)
// 	{
// 		m_nDownUp = 0;
// 		m_nLeftRight = 0;
// 		m_nDownUpPryMgr = 0;
// 		m_nLeftRightPryMgr = 0;
// 	}
// 	else
// 	{
// 		m_nDownUp = 1;
// 		m_nLeftRight = 1;
// 		m_nDownUpPryMgr = 1;
// 		m_nLeftRightPryMgr = 1;
// 	}

	m_bSetBrpr = TRUE;
	m_aArBrprD.clear();
	for (int i = 0; i < aArBearingData.size(); i++)
	{
		// DB 에 들어가는 데이터는 I 가 위부터 시작하고 CDgnSeisAnchorShapeMgr 에서는 i 가 아래에서 시작 함
		// BearingProperty 를 다양하게 입력받기 전 상황에서 이미 I 방향이 반대인 채로 AnchorShape 이 개발되어 있어서.... 수정이 너무 힘든 상황이라 그대로 놔뒀음..
		m_aArBrprD.push_back(aArBearingData[aArBearingData.size() - i - 1]);
	}

	m_bSetAnev = TRUE;
	m_AnevD =  CopyingData;

	//////////////////////////////////////////////////////////////////////////
	// 비어있는 곳 채움.
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl2->GetModifiedAnevD(m_AnevD);
	//////////////////////////////////////////////////////////////////////////

	m_nLIdx = m_AnevD.aLayer.GetSize() - 1;
	SetBearingWH();
	SetCopyingWH();
	
	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	m_aArSethefLongi.resize(nBearingNumI);
	m_aArSethefTrans.resize(nBearingNumI);

	for (int i = 0; i < nBearingNumI; i++)
	{
		m_aArSethefLongi[i].resize(aBearingNumJ[i]);
		m_aArSethefTrans[i].resize(aBearingNumJ[i]);

		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			m_aArSethefLongi[i][j] = FALSE;
			m_aArSethefTrans[i][j] = FALSE;
		}
	}
}

void CDgnSeisAnchorShapeMgr::AllocCa1Var()
{
	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	m_aArSetCa1Longi.resize(nBearingNumI);
	m_aArSetCa1Trans.resize(nBearingNumI);
	m_aArCa1Longi.resize(nBearingNumI);
	m_aArCa1Trans.resize(nBearingNumI);
	m_aArLastCa1Longi	.resize(nBearingNumI);
	m_aArLastCa1Trans.resize(nBearingNumI);

	for (int i = 0; i < nBearingNumI; i++)
	{
		m_aArSetCa1Longi[i].resize(aBearingNumJ[i]);
		m_aArSetCa1Trans[i].resize(aBearingNumJ[i]);
		m_aArCa1Longi	[i].resize(aBearingNumJ[i]);
		m_aArCa1Trans[i].resize(aBearingNumJ[i]);
		m_aArLastCa1Longi[i].resize(aBearingNumJ[i]);
		m_aArLastCa1Trans[i].resize(aBearingNumJ[i]);

		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			m_aArSetCa1Longi[i][j] = FALSE;
			m_aArSetCa1Trans[i][j] = FALSE;
			m_aArCa1Longi	[i][j] = FALSE;
			m_aArCa1Trans[i][j] = FALSE;
			m_aArLastCa1Longi[i][j] = FALSE;
			m_aArLastCa1Trans[i][j] = FALSE;
		}
	}
}

void CDgnSeisAnchorShapeMgr::SetCa1(int nDir, int nBearingIdxI, int nBearingIdxJ, double dLastCa1, double dCa1)
{
	if(m_aArSetCa1Longi.size() == 0)
	{
		AllocCa1Var();
	}

	if(nDir == DIR_LONGI)
	{
		m_aArSetCa1Longi[nBearingIdxI][nBearingIdxJ]  = TRUE;
        m_aArLastCa1Longi[nBearingIdxI][nBearingIdxJ] = dLastCa1;
		m_aArCa1Longi[nBearingIdxI][nBearingIdxJ]     = dCa1;	
	}
	else if(nDir == DIR_TRANS)
	{
		m_aArSetCa1Trans[nBearingIdxI][nBearingIdxJ] = TRUE;
        m_aArLastCa1Trans[nBearingIdxI][nBearingIdxJ] = dLastCa1;
		m_aArCa1Trans[nBearingIdxI][nBearingIdxJ]     = dCa1;
	}
}

void CDgnSeisAnchorShapeMgr::Sethef(int nDir, int nBearingIdxI, int nBearingIdxJ, double dhef)
{
	if(nDir == DIR_LONGI)
	{
		m_aArSethefLongi[nBearingIdxI][nBearingIdxJ] = TRUE;
		m_aArhefLongi[nBearingIdxI][nBearingIdxJ] = dhef;	
	}
	else if(nDir == DIR_TRANS)
	{
		m_aArSethefTrans[nBearingIdxI][nBearingIdxJ] = TRUE;
		m_aArhefTrans[nBearingIdxI][nBearingIdxJ] = dhef;
	}
}

double CDgnSeisAnchorShapeMgr::Gethef(int nDir, int nBearingIdxI, int nBearingIdxJ)
{
	if(nDir == DIR_LONGI)
	{
		if(m_aArSethefLongi.size() > 0 && m_aArSethefLongi[nBearingIdxI][nBearingIdxJ])
		{
			return m_aArhefLongi[nBearingIdxI][nBearingIdxJ];
		}
		else
		{
			return m_aArBrprD[nBearingIdxI][nBearingIdxJ].dhef;
		}
	}
	else if(nDir == DIR_TRANS)
	{
		if(m_aArSethefTrans.size() > 0 && m_aArSethefTrans[nBearingIdxI][nBearingIdxJ])
		{
			return m_aArhefTrans[nBearingIdxI][nBearingIdxJ];
		}
		else
		{
			return m_aArBrprD[nBearingIdxI][nBearingIdxJ].dhef;
		}
	}
	ASSERT(0);
	return 0.0;
}

void CDgnSeisAnchorShapeMgr::GetRepresentCa1Bearing(int nDir, int& nBearingI, int& nBearingJ)
{
	nBearingI = nBearingJ = -1;

	std::vector<std::pair<int, int>> aRepBearingIdx;
	GetConcfailRepresentGrpBearing(nDir, aRepBearingIdx);

	double dTemp = 0.0;
	double dCa1, dCa1P, dCa2, dCa2P;
	double dMinCa1 = DBL_MAX;
	int nMinIdx = -1;
	for (int i = 0; i < aRepBearingIdx.size(); i++)
	{
		int nBrI = aRepBearingIdx[i].first;
		int nBrJ = aRepBearingIdx[i].second;

		GetConcFailBearingCa(nDir, nBrI, nBrJ, dCa1, dCa1P, dCa2, dCa2P);

		// 제일 아래쪽 열이 우선 선택되도록 하자.
		if (nBrI > 0)  dCa1 += m_dBigZero; //0.00001;
		if(dCa1 < dMinCa1)
		{
			dMinCa1 = dCa1;
			nMinIdx = i;
		}
	}
	if(nMinIdx == -1)
	{
		ASSERT(0);
		return;
	}

	nBearingI = aRepBearingIdx[nMinIdx].first;
	nBearingJ = aRepBearingIdx[nMinIdx].second;
}

void CDgnSeisAnchorShapeMgr::UpdateCa1Auto()
{	
	if(m_aArSetCa1Longi.size() == 0)
	{
		AllocCa1Var();
	}

	std::vector<TConcFailBcCalc> aConcFailBc;
	
	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	/* ConcFail 교축은 재계산 안한다.
		GetConcFailBc(DIR_LONGI, aConcFailBc);
		for (int i = 0; i < nBearingNumI; i++)
		{
			for (int j = 0; j < aBearingNumJ[i]; j++)
			{
			UpdateCa1Auto(DIR_LONGI, i, j, aConcFailBc);		
			}
		}
	*/

	GetConcFailBc(DIR_TRANS, aConcFailBc);
	for (int i = 0; i < nBearingNumI; i++)
	{
		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			UpdateCa1Auto(DIR_TRANS, i, j, aConcFailBc);
		}
	}

	// TestCode
	//double dSaxL, dSaxT;
	//GetConcFailSax(DIR_LONGI, dSaxL);
	//GetConcFailSax(DIR_TRANS, dSaxT);
	/////
	/*	
	
	CBrdgSeisEvalKISTEC2015 DgnCalc;
	double dLp, dCa1, dCa1P, dCa2, dCa2P, dSax;
	int nBc;

	if(!CheckData())
	{
		dLp = dCa1 = dCa1P = dCa2 = dCa2P = 0.0;;
		nBc = 0;
		return;
	}
	std::vector<TConcFailBcCalc> aConcFailBc;
	
	double Ca1Pos[4], Ca1PPos[4], Ca2Pos[4], Ca2PPos[4];
	int nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx;
	TConcFailBcCalc MinConcFailBc;
	// Bottom
	BOOL bCheckExist = FALSE;
	double dMinCa1 = DBL_MAX;
	GetConcFailBc(DIR_LONGI, aConcFailBc);
	GetMinConcFailBc(aConcFailBc, MinConcFailBc);
	GetConcFailCa(DIR_LONGI, dCa1, 
		Ca1Pos[0], Ca1Pos[1], Ca1Pos[2], Ca1Pos[3], 
		nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
		dCa1P, Ca1PPos[0], Ca1PPos[1], Ca1PPos[2], 
		Ca1PPos[3], dCa2, Ca2Pos[0], Ca2Pos[1], 
		Ca2Pos[2], Ca2Pos[3], dCa2P, Ca2PPos[0], 
		Ca2PPos[1], Ca2PPos[2], Ca2PPos[3],
		MinConcFailBc.aBearingIdx);
	m_aArLastCa1Bot = dCa1;
	
	int nSize;
	nSize = (int)aConcFailBc.size();
	for (int i = 0; i < nSize ; i++)
	{
		dLp = aConcFailBc[i].dLp;
		nBc = aConcFailBc[i].nBc;	
		dSax = aConcFailBc[i].dSax;	

		if(DgnCalc.Check_Inter3Faces(dCa1, dCa2, dCa2P, m_AnevD.dHcop, dSax, dLp))
		{
			bCheckExist = TRUE;			
			if(dMinCa1 > dCa1)
			{
				dMinCa1 = dCa1;
			}
		}
	}
	if(bCheckExist)
	{
		m_aArSetCa1Bot = TRUE;
		m_aArCa1Bot = dMinCa1;
	}

	// Left
	bCheckExist = FALSE;
	dMinCa1 = DBL_MAX;
	GetConcFailBc(DIR_TRANS, aConcFailBc);
	GetConcFailCa(DIR_TRANS, dCa1, 
		Ca1Pos[0], Ca1Pos[1], Ca1Pos[2], Ca1Pos[3], 
		nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
		dCa1P, Ca1PPos[0], Ca1PPos[1], Ca1PPos[2], 
		Ca1PPos[3], dCa2, Ca2Pos[0], Ca2Pos[1], 
		Ca2Pos[2], Ca2Pos[3], dCa2P, Ca2PPos[0], 
		Ca2PPos[1], Ca2PPos[2], Ca2PPos[3],
		MinConcFailBc.aBearingIdx);
	m_aArLastCa1Left = dCa1;	
	
	nSize = (int)aConcFailBc.size();
	for (int i = 0; i < nSize ; i++)
	{
		dLp = aConcFailBc[i].dLp;
		nBc = aConcFailBc[i].nBc;	
		dSax = aConcFailBc[i].dSax;	

		if(DgnCalc.Check_Inter3Faces(dCa1, dCa2, dCa2P, m_AnevD.dHcop, dSax, dLp))
		{
			bCheckExist = TRUE;			
			if(dMinCa1 > dCa1)
			{
				dMinCa1 = dCa1;
			}
		}
	}
	if(bCheckExist)
	{
		m_aArSetCa1Left = TRUE;
		m_aArCa1Left = dMinCa1;
	}
	*/
}

void CDgnSeisAnchorShapeMgr::GetConcFailSaxNew(int nDir, std::vector<std::pair<int, int>>& aRepBearingIdx, double& dSax)
{
	double dLp, dCa1, dCa2, dCa2P;
	int nBc;

	std::vector<TConcFailBcCalc> aConcFailBc;

	BOOL bCheckExist = FALSE;
	double dMinCa1 = DBL_MAX;
	double dSaxTmp;
	dSax = DBL_MAX;
	double dSaxNormalMin = DBL_MAX;
	if (nDir == DIR_LONGI)
	{
		GetConcFailBc(DIR_LONGI, aConcFailBc);
		int nSize;
		nSize = (int)aConcFailBc.size();
		for (int i = 0; i < nSize; i++)
		{
			dLp = aConcFailBc[i].dLp;
			nBc = aConcFailBc[i].nBc;
			dSaxTmp = aConcFailBc[i].dSax;

			if (dSaxNormalMin > dSaxTmp)
			{
				dSaxNormalMin = dSaxTmp;
			}

			if (m_pSeisEval->Check_Inter3Faces(dCa1, dCa2, dCa2P, m_AnevD.dHcop, dSaxTmp, dLp))
			{
				bCheckExist = TRUE;
				if (dMinCa1 > dCa1)
				{
					dMinCa1 = dCa1;
					dSax = dSaxTmp;
				}
			}
		}
		if (!bCheckExist)
		{
			dSax = dSaxNormalMin;
		}
	}
	else if (nDir == DIR_TRANS)
	{
		int nBearingI = 0;
		int nBearingJ = 0;
		if(aRepBearingIdx.size()>0)
		{
			nBearingI = aRepBearingIdx[0].first;
			nBearingJ = aRepBearingIdx[0].second;

			GetConcFailBc(DIR_TRANS, aConcFailBc, TRUE, nBearingI, nBearingJ);
		}

		int nSize = (int)aConcFailBc.size();
		for (int i = 0; i < nSize; i++)
		{
			dLp = aConcFailBc[i].dLp;
			nBc = aConcFailBc[i].nBc;
			dSaxTmp = aConcFailBc[i].dSax;

			if (dSaxNormalMin > dSaxTmp)
			{
				dSaxNormalMin = dSaxTmp;
			}

			if (m_pSeisEval->Check_Inter3Faces(dCa1, dCa2, dCa2P, m_AnevD.dHcop, dSaxTmp, dLp))
			{
				bCheckExist = TRUE;
				if (dMinCa1 > dCa1)
				{
					dMinCa1 = dCa1;
					dSax = dSaxTmp;
				}
			}
		}
		if (!bCheckExist)
		{
			dSax = dSaxNormalMin;
		}
	}
	else
	{
		ASSERT(0);
	}

	if (dSax == DBL_MAX)
	{
		dSax = 0.0;
	}

}


void CDgnSeisAnchorShapeMgr::GetConcFailSax(int nDir, double& dSax)
{
	double dLp, dCa1, dCa2, dCa2P;
	int nBc;

	std::vector<TConcFailBcCalc> aConcFailBc;
		
	BOOL bCheckExist = FALSE;
	double dMinCa1 = DBL_MAX;
	double dSaxTmp;
	dSax = DBL_MAX;
	double dSaxNormalMin = DBL_MAX;
	if(nDir == DIR_LONGI)
	{
		GetConcFailBc(DIR_LONGI, aConcFailBc);
		int nSize;
		nSize = (int)aConcFailBc.size();
		for (int i = 0; i < nSize ; i++)
		{
			dLp = aConcFailBc[i].dLp;
			nBc = aConcFailBc[i].nBc;	
			dSaxTmp = aConcFailBc[i].dSax;	

			if(dSaxNormalMin > dSaxTmp)
			{
				dSaxNormalMin = dSaxTmp;
			}

			if(m_pSeisEval->Check_Inter3Faces(dCa1, dCa2, dCa2P, m_AnevD.dHcop, dSaxTmp, dLp))
			{
				bCheckExist = TRUE;			
				if(dMinCa1 > dCa1)
				{
					dMinCa1 = dCa1;
					dSax = dSaxTmp;
				}
			}
		}
		if(!bCheckExist)
		{
			dSax = dSaxNormalMin;
		}
	}	
	else if(nDir == DIR_TRANS)
	{
		GetConcFailBc(DIR_TRANS, aConcFailBc);

		int nSize = (int)aConcFailBc.size();
		for (int i = 0; i < nSize ; i++)
		{
			dLp = aConcFailBc[i].dLp;
			nBc = aConcFailBc[i].nBc;	
			dSaxTmp = aConcFailBc[i].dSax;	

			if(dSaxNormalMin > dSaxTmp)
			{
				dSaxNormalMin = dSaxTmp;
			}

			if(m_pSeisEval->Check_Inter3Faces(dCa1, dCa2, dCa2P, m_AnevD.dHcop, dSaxTmp, dLp))
			{
				bCheckExist = TRUE;			
				if(dMinCa1 > dCa1)
				{
					dMinCa1 = dCa1;
					dSax = dSaxTmp;
				}
			}
		}
		if(!bCheckExist)
		{
			dSax = dSaxNormalMin;
		}
	}
	else
	{
		ASSERT(0);
	}

    if ( dSax == DBL_MAX)
    {
        dSax = 0.0;
    }
}

void CDgnSeisAnchorShapeMgr::GetPryoutSaxSay(int nDir, double& dSax, double& dSay)
{
	int nBc;
	double dArea;

	dArea = 0.0;
	nBc = 0;

	std::set<int> setBcIdx;
	CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&> aArProjShapeX, aArProjShapeY;
	
	BOOL bDummy;

	std::vector<std::pair<int, int>> aRepBearingIdxTemp;
	if(nDir == DIR_LONGI)
	{
		GetPryoutBc(DIR_LONGI, aRepBearingIdxTemp, aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bDummy);
	}
	else if(nDir == DIR_TRANS)
	{
		GetPryoutBc(DIR_TRANS, aRepBearingIdxTemp, aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bDummy);
	}
	else
	{
		ASSERT(0);
		GetPryoutBc(DIR_LONGI, aRepBearingIdxTemp, aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bDummy);
	}
}

void CDgnSeisAnchorShapeMgr::UpdateCa1Auto(int nDir, int nBearingI, int nBearingJ, std::vector<TConcFailBcCalc>& aConcFailBc)
{
	if(nDir == DIR_LONGI)
	{
		return; // ConcFail 교축은 재계산 안한다.
	}

	if(m_aArSetCa1Longi.size() == 0)
	{
		AllocCa1Var();
	}

	double dLp;
	BOOL bFind = FALSE;
	for (int i = 0; i < aConcFailBc.size(); i++)
	{
		for (int j = 0; j < aConcFailBc[i].aBearingIdx.size(); j++)
		{
			int nBrI, nBrJ;
			nBrI = aConcFailBc[i].aBearingIdx[j].first;
			nBrJ = aConcFailBc[i].aBearingIdx[j].second;

			if(nBrI == nBearingI && nBrJ == nBearingJ)
			{
				dLp = aConcFailBc[i].dLp;
				bFind = TRUE;
			}
		}
	}

	if(!bFind)
	{
		return; // aConcFailBc 에 없는 Bearing 은 대상이 아님
	}


	double dReCa2, dReCa2P; // 이 값은 검토 방향에 Bearing이 있으면 Sax 또는 Say값.. Bearing이 없으면 Ca 값을 세팅함.

	int nBearingNumI, nBearingNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[nBearingI];

	double dCa1, dCa1P, dCa2, dCa2P;
	GetConcFailBearingCa(nDir, nBearingI, nBearingJ, dCa1, dCa1P, dCa2, dCa2P);

	int BearingIdx[2][2];

	int nCa1Dir, nCa2Dir;
	nCa1Dir = DIR_TRANS;
	nCa2Dir = DIR_LONGI;

	// dReCa2		
	if (m_nLeftRight == DIR_POS)
	{
		if (nBearingI == nBearingNumI - 1)
		{
			dReCa2 = dCa2;
		}
		else
		{
			BearingIdx[0][0] = nBearingI;
			BearingIdx[0][1] = nBearingJ;
			BearingIdx[1][0] = nBearingI + 1;
			BearingIdx[1][1] = nBearingJ;
			dReCa2 = GetBearingGap(DIR_TRANS, BearingIdx) / 2;
		}
	}
	else if (m_nLeftRight == DIR_NEG)
	{
		if (nBearingI == 0)
		{
			dReCa2 = dCa2;
		}
		else
		{
			BearingIdx[0][0] = nBearingI;
			BearingIdx[0][1] = nBearingJ;
			BearingIdx[1][0] = nBearingI - 1;
			BearingIdx[1][1] = nBearingJ;
			dReCa2 = GetBearingGap(DIR_TRANS, BearingIdx) / 2;
		}
	}


	// dReCa2P
	if (m_nLeftRight == DIR_POS)
	{
		if (nBearingI == 0)
		{
			dReCa2P = dCa2P;
		}
		else
		{
			BearingIdx[0][0] = nBearingI - 1;
			BearingIdx[0][1] = nBearingJ;
			BearingIdx[1][0] = nBearingI;
			BearingIdx[1][1] = nBearingJ;
			dReCa2P = GetBearingGap(DIR_TRANS, BearingIdx) / 2;
		}
	}
	else if (m_nLeftRight == DIR_NEG)
	{
		if (nBearingI == nBearingNumI-1)
		{
			dReCa2P = dCa2P;
		}
		else
		{
			BearingIdx[0][0] = nBearingI;
			BearingIdx[0][1] = nBearingJ;
			BearingIdx[1][0] = nBearingI + 1;
			BearingIdx[1][1] = nBearingJ;
			dReCa2P = GetBearingGap(DIR_TRANS, BearingIdx) / 2;
		}
	}
	
	double d15Ca1 = 1.5*dCa1;
	
	// 재계산 Check
	
	m_aArLastCa1Trans[nBearingI][nBearingJ] = dCa1;
	
	BOOL bReCalc = FALSE;
	if(dLp < 2.0*d15Ca1)
	{
		if(dReCa2  < d15Ca1 && dReCa2P < d15Ca1 && m_AnevD.dHcop < d15Ca1)
		{
			bReCalc = TRUE;
		}
	}
	if(!bReCalc)
	{
		if(nDir == DIR_LONGI)
		{
			m_aArSetCa1Longi[nBearingI][nBearingJ] = FALSE;
			m_aArCa1Longi[nBearingI][nBearingJ] = 0.0;						
		}
		else
		{
			m_aArSetCa1Trans[nBearingI][nBearingJ] = FALSE;
			m_aArCa1Trans[nBearingI][nBearingJ] = 0.0;	
		}
		return;
	}


	double dMax = max(max(dReCa2, dReCa2P), m_AnevD.dHcop);

	if (nDir == DIR_LONGI)
	{
		m_aArSetCa1Longi[nBearingI][nBearingJ] = TRUE;
		m_aArCa1Longi[nBearingI][nBearingJ] = dMax / 1.5;
	}
	else
	{
		m_aArSetCa1Trans[nBearingI][nBearingJ] = TRUE;
		m_aArCa1Trans[nBearingI][nBearingJ] = dMax / 1.5;
	}
	
}

void CDgnSeisAnchorShapeMgr::UpdatehefAuto(int nDir, int nBearingI, int nBearingJ)
{
	double dReCa1, dReCa2, dReCa1P, dReCa2P; // 이 값은 검토 방향에 Bearing이 있으면 Sax 또는 Say값.. Bearing이 없으면 Ca 값을 세팅함.

	int nBearingNumI, nBearingNumJ;
	CArray<int, int> aBearingNumJ;

	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[nBearingI];

	double dCa1, dCa1P, dCa2, dCa2P;
	GetPryoutBearingCa(nDir, nBearingI, nBearingJ, dCa1, dCa1P, dCa2, dCa2P);
	
//	int BearingIdx[2][2];

	double ds1, ds2, dhef;
	int nnB;
	ds1 = m_aArBrprD[nBearingI][nBearingJ].dS1;
	ds2 = m_aArBrprD[nBearingI][nBearingJ].dS2;
	nnB = m_aArBrprD[nBearingI][nBearingJ].nN2;
	dhef = m_aArBrprD[nBearingI][nBearingJ].dhef;
	double dInnerAnchorGap;
	if (nDir == DIR_LONGI)
	{
		dInnerAnchorGap = ds2 * ((double)nnB - 1.0);
	}
	else
	{
		dInnerAnchorGap = ds2 * ((double)nnB - 1.0);
	}

	int nCa1Dir, nCa2Dir;
	if(nDir == DIR_LONGI)
	{	
		nCa1Dir = DIR_LONGI;
		nCa2Dir = DIR_TRANS;

		if (m_nDownUp == DIR_POS)
		{
			dReCa2  = dCa2;
			dReCa2P = dCa2P;
			dReCa1  = dCa1;
			dReCa1P = dCa1P;

// 			// dReCa2		
// 			if (nBearingJ == 0)
// 			{
// 				dReCa2 = dCa2;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ - 1;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa2 = GetBearingGap(DIR_LONGI, BearingIdx) / 2;
// 			}
// 
// 			// dReCa2P
// 			if (nBearingJ == nBearingNumJ - 1)
// 			{
// 				dReCa2P = dCa2P;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ + 1;
// 				dReCa2P = GetBearingGap(DIR_LONGI, BearingIdx) / 2;
// 			}
// 
// 			// dReCa1
// 			if (nBearingI == 0)
// 			{
// 				dReCa1 = dCa1;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI - 1;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa1 = GetBearingCenterGap(DIR_TRANS, BearingIdx) / 2;
// 			}
// 
// 			// dReCa1P
// 			if (nBearingI == nBearingNumI - 1)
// 			{
// 				dReCa1P = dCa1P;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI + 1;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa1P = GetBearingCenterGap(DIR_TRANS, BearingIdx) / 2;
// 			}
		}
		else if (m_nDownUp == DIR_NEG)
		{
			dReCa2  = dCa2;
			dReCa2P = dCa2P;
			dReCa1  = dCa1;
			dReCa1P = dCa1P;

// 			// dReCa2		
// 			if (nBearingJ == nBearingNumJ-1)
// 			{
// 				dReCa2 = dCa2;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ + 1;
// 				dReCa2 = GetBearingGap(DIR_LONGI, BearingIdx) / 2;
// 			}
// 
// 			// dReCa2P
// 			if (nBearingJ == 0)
// 			{
// 				dReCa2P = dCa2P;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ - 1;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa2P = GetBearingGap(DIR_LONGI, BearingIdx) / 2;
// 			}
// 
// 			// dReCa1
// 			if (nBearingI == nBearingNumI-1)
// 			{
// 				dReCa1 = dCa1;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI + 1;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa1 = GetBearingCenterGap(DIR_TRANS, BearingIdx) / 2;
// 			}
// 
// 			// dReCa1P
// 			if (nBearingI == 0)
// 			{
// 				dReCa1P = dCa1P;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI - 1;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa1P = GetBearingCenterGap(DIR_TRANS, BearingIdx) / 2;
// 			}
		}
		
	}
	else
	{
		nCa1Dir = DIR_LONGI;
		nCa2Dir = DIR_TRANS;

		if (m_nLeftRight == DIR_POS)
		{
			dReCa2  = dCa2;
			dReCa2P = dCa2P;
			dReCa1  = dCa1;
			dReCa1P = dCa1P;

// 			// dReCa2		
// 			if (nBearingI == nBearingNumI - 1)
// 			{
// 				dReCa2 = dCa2;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI + 1;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa2 = GetBearingGap(DIR_TRANS, BearingIdx) / 2;
// 			}
// 
// 			// dReCa2P
// 			if (nBearingI == 0)
// 			{
// 				dReCa2P = dCa2P;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI - 1;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa2P = GetBearingGap(DIR_TRANS, BearingIdx) / 2;
// 			}
// 
// 			// dReCa1
// 			if (nBearingJ == 0)
// 			{
// 				dReCa1 = dCa1;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ - 1;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa1 = GetBearingCenterGap(DIR_LONGI, BearingIdx) / 2;
// 			}
// 
// 			// dReCa1P
// 			if (nBearingJ == nBearingNumJ - 1)
// 			{
// 				dReCa1P = dCa1P;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ + 1;
// 				dReCa1P = GetBearingCenterGap(DIR_LONGI, BearingIdx) / 2;
// 			}
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			dReCa2  = dCa2;
			dReCa2P = dCa2P;
			dReCa1  = dCa1;
			dReCa1P = dCa1P;

// 			// dReCa2		
// 			if (nBearingI == 0)
// 			{
// 				dReCa2 = dCa2;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI - 1;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa2 = GetBearingGap(DIR_TRANS, BearingIdx) / 2;
// 			}
// 
// 			// dReCa2P
// 			if (nBearingI == nBearingNumI - 1)
// 			{
// 				dReCa2P = dCa2P;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI + 1;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa2P = GetBearingGap(DIR_TRANS, BearingIdx) / 2;
// 			}
// 
// 			// dReCa1
// 			if (nBearingJ == nBearingNumJ - 1)
// 			{
// 				dReCa1 = dCa1;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ + 1;
// 				dReCa1 = GetBearingCenterGap(DIR_LONGI, BearingIdx) / 2;
// 			}
// 
// 			// dReCa1P
// 			if (nBearingJ == 0)
// 			{
// 				dReCa1P = dCa1P;
// 			}
// 			else
// 			{
// 				BearingIdx[0][0] = nBearingI;
// 				BearingIdx[0][1] = nBearingJ - 1;
// 				BearingIdx[1][0] = nBearingI;
// 				BearingIdx[1][1] = nBearingJ;
// 				dReCa1P = GetBearingCenterGap(DIR_LONGI, BearingIdx) / 2;
// 			}
		}
	}

	// 재계산 Check	
	BOOL bReCalc = FALSE;
	if(dReCa2  < 1.5 * dhef && 
		dReCa2P < 1.5 * dhef && 
		dReCa1 < 1.5 * dhef &&
		dReCa1P < 1.5 * dhef)
	{
		bReCalc = TRUE;
	}
	if(!bReCalc)
	{
		if (nDir == DIR_LONGI)
		{
			m_aArSethefLongi[nBearingI][nBearingJ] = FALSE;
			m_aArhefLongi[nBearingI][nBearingJ] = 0.0;
		}
		else
		{
			m_aArSethefTrans[nBearingI][nBearingJ] = FALSE;
			m_aArhefTrans[nBearingI][nBearingJ] = 0.0;
		}
		return;
	}

	if (nDir == DIR_LONGI)
	{
		m_aArSethefLongi[nBearingI][nBearingJ] = TRUE;
		double dMax = max(max(dReCa1, dReCa1P), max(dReCa2, dReCa2P));
		m_aArhefLongi[nBearingI][nBearingJ] = dMax / 1.5;
	}
	else
	{
		m_aArSethefTrans[nBearingI][nBearingJ] = TRUE;
		double dMax = max(max(dReCa1, dReCa1P), max(dReCa2, dReCa2P));
		m_aArhefTrans[nBearingI][nBearingJ] = dMax / 1.5;
	}
}

void CDgnSeisAnchorShapeMgr::AllocHefVal()
{
	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	m_aArSethefLongi.resize(nBearingNumI);
	m_aArSethefTrans.resize(nBearingNumI);
	m_aArhefLongi.resize(nBearingNumI);
	m_aArhefTrans.resize(nBearingNumI);

	for (int i = 0; i < nBearingNumI; i++)
	{
		m_aArSethefLongi[i].resize(aBearingNumJ[i]);
		m_aArSethefTrans[i].resize(aBearingNumJ[i]);
		m_aArhefLongi[i].resize(aBearingNumJ[i]);
		m_aArhefTrans[i].resize(aBearingNumJ[i]);

		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			m_aArSethefLongi[i][j] = FALSE;
			m_aArSethefTrans[i][j] = FALSE;
			m_aArhefLongi[i][j] = 0.0;
			m_aArhefTrans[i][j] = 0.0;
		}
	}
}

int CDgnSeisAnchorShapeMgr::GetConcFailCa1AnchorNum(int nDir, std::vector<std::pair<int, int>>& aRepBearingIdxChk, BOOL bGetIdx/*=FALSE*/)
{
	// 대표 그룹에서 Ca1 확인 대상이 되는 Anchor 수.. 대표 그룹의 아랫쪽 Anchor 수라고 보면 됨
	std::vector<std::pair<int, int>> aRepBearingIdx;
	if(aRepBearingIdxChk.size()==0)
	{
		GetConcfailRepresentGrpBearing(nDir, aRepBearingIdx);
	}
	else
	{
		aRepBearingIdx = aRepBearingIdxChk;
	}
	
	int nNum = 0;	
	int nBearingI, nBearingJ, nAnchorNumI, nAnchorNumJ;
	for (int i = 0; i < aRepBearingIdx.size(); i++)
	{
		nBearingI = aRepBearingIdx[i].first;
		nBearingJ = aRepBearingIdx[i].second;
		GetAnchorNumber(nBearingI, nBearingJ, nAnchorNumI, nAnchorNumJ);
		if(nDir == DIR_LONGI)
		{
			nNum += nAnchorNumJ;
		}
		else
		{
			nNum += nAnchorNumI;
		}
	}

	if (bGetIdx)
	{
		aRepBearingIdxChk = aRepBearingIdx;
	}
	return nNum;
}

BOOL CDgnSeisAnchorShapeMgr::GetBearingLink(int nBearingI, int nBearingJ, CArray<T_ELNK_K, T_ELNK_K>& aElnkK, CArray<T_NLNK_K, T_NLNK_K>& aNlnkK, BOOL bOrgPos/* = FALSE*/)
{
	aElnkK.RemoveAll();
	aNlnkK.RemoveAll();

	if(!m_AnevD.bTensChk)
		return FALSE;

	CArray<T_NODE_K, T_NODE_K> aNodeK;
	int nSize = m_AnevD.TensChk.aElnkNodeKs.GetSize();
	const int iLinkIdx = bOrgPos ? nBearingI : nSize - nBearingI - 1;
	const int iNodeIdx = nBearingJ;
	if(nSize > iLinkIdx)
	{
		if(m_AnevD.TensChk.aElnkNodeKs[iLinkIdx].aNodeK1.GetSize() > iNodeIdx)
			aNodeK.Add(m_AnevD.TensChk.aElnkNodeKs[iLinkIdx].aNodeK1[iNodeIdx]);
		if(m_AnevD.TensChk.aElnkNodeKs[iLinkIdx].aNodeK2.GetSize() > iNodeIdx)
			aNodeK.Add(m_AnevD.TensChk.aElnkNodeKs[iLinkIdx].aNodeK2[iNodeIdx]);
	}
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl2->GetLinkList(aNodeK, aElnkK, aNlnkK);

	return TRUE;	
}

BOOL CDgnSeisAnchorShapeMgr::GetBearingLinkNode(int nBearingI, int nBearingJ, T_NODE_K& NodeK1, T_NODE_K& NodeK2, BOOL bOrgPos/* = FALSE*/)
{
	if(!m_AnevD.bTensChk)
		return FALSE;

	int nSize = m_AnevD.TensChk.aElnkNodeKs.GetSize();
	const int iLinkIdx = bOrgPos ? nBearingI : nSize - nBearingI - 1;
	const int iNodeIdx = nBearingJ;
	if(nSize > iLinkIdx)
	{
		if(m_AnevD.TensChk.aElnkNodeKs[iLinkIdx].aNodeK1.GetSize() > iNodeIdx)
			NodeK1 = m_AnevD.TensChk.aElnkNodeKs[iLinkIdx].aNodeK1[iNodeIdx];
		if(m_AnevD.TensChk.aElnkNodeKs[iLinkIdx].aNodeK2.GetSize() > iNodeIdx)
			NodeK2 = m_AnevD.TensChk.aElnkNodeKs[iLinkIdx].aNodeK2[iNodeIdx];
	}

	return TRUE;	
}

int CDgnSeisAnchorShapeMgr::GetPryoutCa1AnchorNum(int nDir, std::vector<std::pair<int, int>>& aRepBearingIdx)
{
	// 대표 그룹에서 Ca1 확인 대상이 되는 Anchor 수.. 대표 그룹의 아랫쪽 Anchor 수라고 보면 됨
	if(aRepBearingIdx.size() == 0) // 설계검토 시
	{
		GetPryoutRepresentGrpBearing(nDir, aRepBearingIdx);
	}
	else
	{
		// Pryout Ratio 검토시
	}
	
	int nBearingI, nBearingJ, nAnchorNumI, nAnchorNumJ;

	if (aRepBearingIdx.size() == 1)
	{
		nBearingI = aRepBearingIdx[0].first;
		nBearingJ = aRepBearingIdx[0].second;
		double dDist = Gethef(nDir, nBearingI, nBearingJ) * 1.5;

		BOOL bSingleResist;
		if (nDir == DIR_LONGI)
		{
			if (m_aArBrprD[nBearingI][nBearingJ].dS1 - dDist * 2 < -m_dBigZero)
			{
				bSingleResist = FALSE;
			}
			else
			{
				bSingleResist = TRUE;
			}
		}
		else if (nDir == DIR_TRANS)
		{
			if (m_aArBrprD[nBearingI][nBearingJ].dS2 - dDist * 2 < -m_dBigZero)
			{
				bSingleResist = FALSE;
			}
			else
			{
				bSingleResist = TRUE;
			}
		}

		if (bSingleResist) // Anchor 1개만 저항할 경우 저항 Anchor 수를 1로 넘겨줘야 함
		{
			return 1;
		}
	}
	int nNum = 0;
	for (int i = 0; i < aRepBearingIdx.size(); i++)
	{
		nBearingI = aRepBearingIdx[i].first;
		nBearingJ = aRepBearingIdx[i].second;
		GetAnchorNumber(nBearingI, nBearingJ, nAnchorNumI, nAnchorNumJ);
		if(nDir == DIR_LONGI)
		{
			nNum += nAnchorNumJ;
		}
		else
		{
			nNum += nAnchorNumI;
		}
	}
	return nNum;
}

BOOL CDgnSeisAnchorShapeMgr::GetReCalcCa1(int nDir, int nBearingIdxI, int nBearingIdxJ, double& dCa1)
{
	// 이 함수는 UpdateCa1Auto() 를 부른 다음에 써야 함.. 불필요한 연산이 적음

	if(nDir == DIR_LONGI)
	{
		//if(m_aArSetCa1Longi[nBearingIdxI][nBearingIdxJ])
		//{
		//	dCa1 = m_aArCa1Trans[nBearingIdxI][nBearingIdxJ];
		//	return TRUE;				 
		//}
		//else
		//{
		return FALSE; // ConcFail 교축은 재계산 안 한다.
		//}
	}
	else if(nDir == DIR_TRANS)
	{
		if(m_aArSetCa1Trans[nBearingIdxI][nBearingIdxJ])
		{
			dCa1 = m_aArCa1Trans[nBearingIdxI][nBearingIdxJ];
			return TRUE;				 
		}
		else
		{
			return FALSE;
		}
	}
	ASSERT(0);
	return FALSE;
}

BOOL CDgnSeisAnchorShapeMgr::GetReCalcCa1(int nDir, int nBearingI, int nBearingJ, double& dReCalcCa1, std::vector<TConcFailBcCalc>& aConcFailBc)
{
	// 이 함수는 UpdateCa1Auto() 를 안불렀을 때 써야 함.. 속도가 더 느림

	if(nDir == DIR_LONGI)
	{
		/*
		if(m_aArSetCa1Longi.size() == 0 || m_aArCa1Longi.size() == 0)
		{
			AllocCa1Var();
		}

		if(!m_aArSetCa1Longi[nBearingI][nBearingJ])
		{
			UpdateCa1Auto(DIR_LONGI, nBearingI, nBearingI, aConcFailBc);			
		}

		dReCalcCa1 = m_aArCa1Longi[nBearingI][nBearingJ];
		return m_aArSetCa1Longi[nBearingI][nBearingJ];
		*/
		return FALSE;
	}
	else if(nDir == DIR_TRANS)
	{
		if(m_aArSetCa1Trans.size() == 0 || m_aArCa1Trans.size() == 0)
		{
			AllocCa1Var();
		}

		if(!m_aArSetCa1Trans[nBearingI][nBearingJ])
		{
			UpdateCa1Auto(DIR_TRANS, nBearingI, nBearingI, aConcFailBc);			
		}

		dReCalcCa1 = m_aArCa1Trans[nBearingI][nBearingJ];
		return m_aArSetCa1Trans[nBearingI][nBearingJ];
	}
	ASSERT(0);
	return FALSE;
}

BOOL CDgnSeisAnchorShapeMgr::GetReCalcHef(int nDir, int nBearingI, int nBearingJ, double& dReCalcHef)
{
	if(nDir == DIR_LONGI)
	{
		if(m_aArSethefLongi.size() == 0 || m_aArhefLongi.size() == 0)
		{
			AllocHefVal();
		}

		if(!m_aArSethefLongi[nBearingI][nBearingJ])
		{
			UpdatehefAuto(DIR_LONGI, nBearingI, nBearingJ/*nBearingI*/);
		}
				
		dReCalcHef = m_aArhefLongi[nBearingI][nBearingJ];
		return m_aArSethefLongi[nBearingI][nBearingJ];
	}
	else if(nDir == DIR_TRANS)
	{
        if ( m_aArSethefTrans.size() == 0 || m_aArhefTrans.size() == 0 )
		{
			AllocHefVal();
		}

		if(!m_aArSethefTrans[nBearingI][nBearingJ])
		{
			UpdatehefAuto(DIR_TRANS, nBearingI, nBearingJ/*nBearingI*/);
		}

		dReCalcHef = m_aArhefTrans[nBearingI][nBearingJ];
		return m_aArSethefTrans[nBearingI][nBearingJ];
	}
	ASSERT(0);
	return FALSE;
}

void CDgnSeisAnchorShapeMgr::UpdatehefAuto()
{	
	AllocHefVal();

	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	for (int i = 0; i < nBearingNumI; i++)
	{
		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			UpdatehefAuto(DIR_LONGI, i, j);
		}
	}

	for (int i = 0; i < nBearingNumI; i++)
	{
		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			UpdatehefAuto(DIR_TRANS, i, j);
		}
	}


	/*
	CBrdgSeisEvalKISTEC2015 DgnCalc;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	int nBc, nnB;
	double dArea, dCa1, dCa1P, dCa2, dCa2P, ds1, ds2, dhef;

	dArea = 0.0;
	nBc = 0;

	double dSax, dSay;
	std::set<int> setBcIdx;
	CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&> aArProjShapeX, aArProjShapeY;

	double Ca1Pos[4], Ca1PPos[4], Ca2Pos[4], Ca2PPos[4];
	int nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx;

	BOOL bSingleResist;
	GetPryoutBc(DIR_LONGI, aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bSingleResist);
	if(aArProjShapeX.GetSize() == 0)
	{
		return; // 데이터를 입력하지 않은 경우
	}
	if(nBc > 1) // 그룹 저항일 경우에만 재계산 하면 됨
	{
		GetPryoutCa(DIR_LONGI, dCa1, Ca1Pos[0], Ca1Pos[1], Ca1Pos[2], Ca1Pos[3], 
			nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
			dCa1P, Ca1PPos[0], Ca1PPos[1], Ca1PPos[2], Ca1PPos[3], 
			dCa2, Ca2Pos[0], Ca2Pos[1], Ca2Pos[2], Ca2Pos[3], 
			dCa2P, Ca2PPos[0], Ca2PPos[1], Ca2PPos[2], Ca2PPos[3]);

		ds1 = m_aArBrprD.dS1;
		ds2 = m_aArBrprD.dS2;
		nnB = m_aArBrprD.nN2;
		dhef = m_aArBrprD.dhef;
		if(DgnCalc.Check_Inter4Faces(dhef, (int)DIR_LONGI, dCa1, dCa1P, dCa2, dCa2P, dSax, dSay, nnB, ds1, ds2))
		{
			m_aArSethefLongi = TRUE;
			m_aArhefLongi = dhef;
		}
	}

	GetPryoutBc(DIR_TRANS, aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bSingleResist);
	if(aArProjShapeX.GetSize() == 0)
	{
		return; // 데이터를 입력하지 않은 경우
	}
	if(nBc > 1) // 그룹 저항일 경우에만 재계산 하면 됨
	{
		GetPryoutCa(DIR_LONGI, dCa1, Ca1Pos[0], Ca1Pos[1], Ca1Pos[2], Ca1Pos[3], 
			nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
			dCa1P, Ca1PPos[0], Ca1PPos[1], Ca1PPos[2], Ca1PPos[3], 
			dCa2, Ca2Pos[0], Ca2Pos[1], Ca2Pos[2], Ca2Pos[3], 
			dCa2P, Ca2PPos[0], Ca2PPos[1], Ca2PPos[2], Ca2PPos[3]);

		ds1 = m_aArBrprD.dS2;
		ds2 = m_aArBrprD.dS1;
		nnB = m_aArBrprD.nN1;
		dhef = m_aArBrprD.dhef;
		if(DgnCalc.Check_Inter4Faces(dhef, (int)DIR_TRANS, dCa1, dCa1P, dCa2, dCa2P, dSay, dSax, nnB, ds1, ds2))
		{
			m_aArSethefTrans = TRUE;
			m_aArhefTrans = dhef;
		}
	}
	*/
}

void CDgnSeisAnchorShapeMgr::GetConcFailSideLine(int nDir, std::vector<T_LINE2D> &aSideLine, int nDW, int nLR)
{	
	m_SideLineIdx.clear();
	CArray<int, int> aBearingI, aBearingJ, aAnchorI, aAnchorJ;
	GetPrjTgtIdxConcFail(nDir, aBearingI, aBearingJ, aAnchorI, aAnchorJ);
	if(aBearingI.GetSize() < 1 || aBearingJ.GetSize() < 1 || aAnchorI.GetSize() < 1 || aAnchorJ.GetSize() < 1)
	{
		return;
	}

	double dZero = 0.00001; // 그림 그리는 곳에서만 쓰니깐 적당히 큰 값으로 처리 함..

	ASSERT(aBearingI.GetSize() == aBearingJ.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorI.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorJ.GetSize());

	double AnchorPos[2], Pos1[2], Pos2[2];

	CArray<double, double> aCopingX, aCopingY;
	GetCopyingShape(aCopingX, aCopingY);

	T_LINE2D SideLine, SmallSideLine;
	SideLine.P1.x = aCopingX[0];
	SideLine.P1.y = aCopingY[0];
	SideLine.P2.x = aCopingX[0];
	SideLine.P2.y = aCopingY[0];
	SmallSideLine = SideLine;

	T_POINT2D Left, Right, SmallLeft, SmallRight;

	int nSize;
	nSize = aBearingI.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		CalcAnchorPos(aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], AnchorPos[0], AnchorPos[1]);
		CalcAnchorProjPosConcFail(nDir, aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], Pos1[0], Pos1[1], Pos2[0], Pos2[1]);

		// Coping 아랫쪽의 강조선 계산
		if(nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
		{	

			if (m_nDownUp == DIR_POS)
			{
				if (fabs(Pos1[1] - aCopingY[0]) > dZero)
				{
					Left.x = aCopingX[0];
					Left.y = aCopingY[0];
				}

				else
				{
					Left.x = Pos1[0];
					Left.y = Pos1[1];
				}

				if (fabs(Pos2[1] - aCopingY[3]) > dZero)
				{
					Right.x = aCopingX[3];
					Right.y = aCopingY[3];
				}
				else
				{
					Right.x = Pos2[0];
					Right.y = Pos2[1];
				}
			}
			else if (m_nDownUp == DIR_NEG)
			{
				if (fabs(Pos1[1] - aCopingY[2]) > dZero)
				{
					Left.x = aCopingX[2];
					Left.y = aCopingY[2];
				}

				else
				{
					Left.x = Pos1[0];
					Left.y = Pos1[1];
				}

				if (fabs(Pos2[1] - aCopingY[1]) > dZero)
				{
					Right.x = aCopingX[1];
					Right.y = aCopingY[1];
				}

				else
				{
					Right.x = Pos2[0];
					Right.y = Pos2[1];
				}
			}
			

			SmallLeft = Left;
			SmallRight = Right;
			SetLineSmaller(SmallLeft.x, SmallLeft.y, SmallRight.x, SmallRight.y);

			if(i == 0)
			{
				SideLine.P1 = Left;
				SideLine.P2 = Right;

				SmallSideLine.P1 = SmallLeft;
				SmallSideLine.P2 = SmallRight;
			}

			if(fabs(Left.x - Right.x) > dZero)
			{				
				if(CMathFunc::mathIncludePointInLine(SmallLeft.x, SmallLeft.y, SmallRight.x, SmallRight.y, SmallSideLine.P2.x, SmallSideLine.P2.y, dZero))
				{
					SideLine.P2 = Right;
					SmallSideLine.P2 = SmallRight;
				}
				else
				{
					aSideLine.push_back(SideLine);
					m_SideLineIdx.push_back(make_pair(aBearingI[i-1], aBearingJ[i-1]));
					SideLine.P1 = Left;
					SideLine.P2 = Right;

					SmallSideLine.P1 = SmallLeft;
					SmallSideLine.P2 = SmallRight;
				}
			}
		}
		else if(nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
		{	
			if (m_nLeftRight == DIR_POS)
			{
				if (CMathFunc::mathIncludePointInLine(aCopingX[0], aCopingY[0], aCopingX[1], aCopingY[1], Pos1[0], Pos1[1], dZero))
				{
					Left.x = Pos1[0];
					Left.y = Pos1[1];
				}
				else
				{
					Left.x = aCopingX[0];
					Left.y = aCopingY[0];
				}

				if (CMathFunc::mathIncludePointInLine(aCopingX[0], aCopingY[0], aCopingX[1], aCopingY[1], Pos2[0], Pos2[1], dZero))
				{
					Right.x = Pos2[0];
					Right.y = Pos2[1];
				}
				else
				{
					Right.x = aCopingX[1];
					Right.y = aCopingY[1];
				}
			}
			else if (m_nLeftRight == DIR_NEG)
			{
				if (CMathFunc::mathIncludePointInLine(aCopingX[2], aCopingY[2], aCopingX[3], aCopingY[3], Pos1[0], Pos1[1], dZero))
				{
					Left.x = Pos1[0];
					Left.y = Pos1[1];
				}
				else
				{
					Left.x = aCopingX[2];
					Left.y = aCopingY[2];
				}

				if (CMathFunc::mathIncludePointInLine(aCopingX[2], aCopingY[2], aCopingX[3], aCopingY[3], Pos2[0], Pos2[1], dZero))
				{
					Right.x = Pos2[0];
					Right.y = Pos2[1];
				}
				else
				{
					Right.x = aCopingX[3];
					Right.y = aCopingY[3];
				}
			}

			SmallLeft = Left;
			SmallRight = Right;
			SetLineSmaller(SmallLeft.x, SmallLeft.y, SmallRight.x, SmallRight.y);

			if(i == 0)
			{
				SideLine.P1 = Left;
				SideLine.P2 = Right;

				SmallSideLine.P1 = SmallLeft;
				SmallSideLine.P2 = SmallRight;
			}

			if(fabs(Left.y - Right.y) > dZero)
			{
				if(CMathFunc::mathIncludePointInLine(SmallLeft.x, SmallLeft.y, SmallRight.x, SmallRight.y, SmallSideLine.P2.x, SmallSideLine.P2.y, dZero))
				{
					SideLine.P2 = Right;

					SmallSideLine.P2 = SmallRight;
				}
				else
				{
					aSideLine.push_back(SideLine);
					m_SideLineIdx.push_back(make_pair(aBearingI[i-1], aBearingJ[i-1]));
					SideLine.P1 = Left;
					SideLine.P2 = Right;

					SmallSideLine.P1 = SmallLeft;
					SmallSideLine.P2 = SmallRight;
				}
			}
		}
	}
	if(aSideLine.size() == 0)
	{
		aSideLine.push_back(SideLine);
	}
	else
	{
		if(nDir == CDgnSeisAnchorShapeMgr::DIR_LONGI)
		{	
			double dLastX = aSideLine[(int)aSideLine.size() - 1].P2.x;
			if (m_nDownUp == DIR_POS)
			{
				if (dLastX < SideLine.P2.x &&
					fabs(dLastX - SideLine.P2.x) > dZero)
				{
					aSideLine.push_back(SideLine);
					m_SideLineIdx.push_back(make_pair(aBearingI[nSize-1], aBearingJ[nSize - 1]));

				}
			}
			else if (m_nDownUp == DIR_NEG)
			{
				if (dLastX > SideLine.P2.x &&
					fabs(dLastX - SideLine.P2.x) > dZero)
				{
					aSideLine.push_back(SideLine);
					m_SideLineIdx.push_back(make_pair(aBearingI[nSize - 1], aBearingJ[nSize - 1]));
				}
			}
		}
		// 교축 직각방향의 경우, 파괴 영역이 왼쪽이든 오른쪽이든 읽는 방향이 아래에서 위이다. 따라서 마지막 라인 추가를 위한 부등식이 변화가 없다.
		else if(nDir == CDgnSeisAnchorShapeMgr::DIR_TRANS)
		{
			double dLastY = aSideLine[(int)aSideLine.size() - 1].P2.y;
			if (dLastY < SideLine.P2.y &&
				fabs(dLastY - SideLine.P2.y) > dZero)
			{
				aSideLine.push_back(SideLine);
				m_SideLineIdx.push_back(make_pair(aBearingI[nSize - 1], aBearingJ[nSize - 1]));
			}
		}
	}
}

BOOL CDgnSeisAnchorShapeMgr::CheckDataBrpr()
{
	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	for (int i = 0; i < nBearingNumI; i++)
	{
		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			if(!CheckDataBrpr(i, j))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CDgnSeisAnchorShapeMgr::CheckDataBrpr(int nBearingI, int nBearingJ)
{

	if(m_aArBrprD[nBearingI][nBearingJ].strName.IsEmpty())
	{
		return TRUE; // 비대칭 고려.
	}
	if(m_aArBrprD[nBearingI][nBearingJ].dda <= 0.0)
	{
		return FALSE;
	}
	if(m_aArBrprD[nBearingI][nBearingJ].dSx <= 0.0)
	{
		return FALSE;
	}
	if(m_aArBrprD[nBearingI][nBearingJ].nN1 <= 0)
	{
		return FALSE;
	}
	if(m_aArBrprD[nBearingI][nBearingJ].dS1 <= 0.0 && m_aArBrprD[nBearingI][nBearingJ].nN1 > 1)
	{
		return FALSE;
	}
	if(m_aArBrprD[nBearingI][nBearingJ].dSy <= 0.0)
	{
		return FALSE;
	}
	if(m_aArBrprD[nBearingI][nBearingJ].nN2 <= 0.0)
	{
		return FALSE;
	}
	if(m_aArBrprD[nBearingI][nBearingJ].dS2 <= 0.0 && m_aArBrprD[nBearingI][nBearingJ].nN2 > 1)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnSeisAnchorShapeMgr::CheckDataAnev()
{
	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	for (int i = 0; i < nBearingNumI; i++)
	{
		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			if(!CheckDataAnev(i, j))
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CDgnSeisAnchorShapeMgr::CheckDataAnev(int i, int j)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	if(m_dCopyingWidth <= m_aArBearingWidth[i][j])
	{
		return FALSE;
	}

	if(m_dCopyingHeight <= m_aArBearingHeight[i][j])
	{
		return FALSE;
	}

//	if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
	{
		T_ANEV_LAYER_D Layer;
		Layer = m_AnevD.aLayer[i];

		if (Layer.dD1 <= 0.0)
		{
			return FALSE;
		}
		if (Layer.dD1_q <= 0.0)
		{
			return FALSE;
		}
		int nSize;
		nSize = Layer.aSax.GetSize();

		if (nSize < 0)
		{
			return FALSE;
		}
		if (!(nSize == 1 && fabs(Layer.aSax[0]) < dZero))
		{
			for (int i = 0; i < nSize; i++)
			{
				//if (Layer.aSax[i] <= 0.0)
				if (Layer.aSax[i] < 0.0) // 비대칭 고려
				{
					return FALSE;
				}
			}
		}

		if (Layer.dD2 <= 0.0)
		{
			return FALSE;
		}
		if (Layer.dD2_q <= 0.0)
		{
			return FALSE;
		}
	}
// 	else
// 	{
// 		if (m_AnevD.dD1 <= 0.0)
// 		{
// 			return FALSE;
// 		}
// 		if (m_AnevD.dD1_q <= 0.0)
// 		{
// 			return FALSE;
// 		}
// 		int nSize;
// 		nSize = m_AnevD.aSax.GetSize();
// 		if (nSize < 0)
// 		{
// 			return FALSE;
// 		}
// 		if (!(nSize == 1 && fabs(m_AnevD.aSax[0]) < dZero))
// 		{
// 			for (int i = 0; i < nSize; i++)
// 			{
// 				if (m_AnevD.aSax[i] <= 0.0)
// 				{
// 					return FALSE;
// 				}
// 			}
// 		}
// 
// 		if (m_AnevD.dD2 <= 0.0)
// 		{
// 			return FALSE;
// 		}
// 		if (m_AnevD.dD2_q <= 0.0)
// 		{
// 			return FALSE;
// 		}
// 		nSize = m_AnevD.aSay.GetSize();
// 		if (nSize < 0)
// 		{
// 			return FALSE;
// 		}
// 		if (!(nSize == 1 && fabs(m_AnevD.aSay[0]) < dZero))
// 		{
// 			for (int i = 0; i < nSize; i++)
// 			{
// 				if (m_AnevD.aSay[i] <= 0.0)
// 				{
// 					return FALSE;
// 				}
// 			}
// 		}
// 	}

	return TRUE;
}

BOOL CDgnSeisAnchorShapeMgr::CheckData()
{
	if(m_bSetBrpr)
	{
		if(!CheckDataBrpr())
		{
			return FALSE;
		}
	}

	if(m_bSetAnev)
	{
		if(!m_bSetBrpr)
		{
			return FALSE;
		}
		if(!CheckDataAnev())
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CDgnSeisAnchorShapeMgr::SetBearingWH()
{
	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	if(m_aArBrprD.size() == 0)
	{
		m_aArBearingHeight.resize(1);
		m_aArBearingWidth.resize(1);
		m_aArBearingHeight[0].resize(1);
		m_aArBearingWidth[0].resize(1);
		m_aArBearingHeight[0][0]= 0.0;
		m_aArBearingWidth[0][0]= 0.0;

		T_BRPR_D BrprD;
		BrprD.Initialize();
		m_aArBrprD.resize(1);
		m_aArBrprD[0].resize(1);
		m_aArBrprD[0][0] = BrprD;
		return;
	}

	m_aArBearingHeight.clear();
	m_aArBearingWidth.clear();	
	m_aArBearingHeight.resize(nBearingNumI);
	m_aArBearingWidth.resize(nBearingNumI);

	for (int i = 0; i < nBearingNumI; i++)
	{
		m_aArBearingHeight[i].resize(aBearingNumJ[i]);
		m_aArBearingWidth[i].resize(aBearingNumJ[i]);

		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			m_aArBearingWidth[i][j] = m_aArBrprD[i][j].dSx * 2.0;	
			int nSize;
			nSize = m_aArBrprD[i][j].nN1 - 1;
			for (int k = 0; k < nSize ; k++)
			{
				m_aArBearingWidth[i][j] += m_aArBrprD[i][j].dS1;
			}

			m_aArBearingHeight[i][j] = m_aArBrprD[i][j].dSy * 2.0;
			nSize = m_aArBrprD[i][j].nN2 - 1;
			for (int k = 0; k < nSize ; k++)
			{
				m_aArBearingHeight[i][j] += m_aArBrprD[i][j].dS2;
			}
		}
	}
}

void CDgnSeisAnchorShapeMgr::SetCopyingWH()
{
	if(!m_bSetAnev)
	{
		m_dCopyingHeight = 0.0;
		m_dCopyingWidth = 0.0;
	}

//	if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
	{
		m_dCopyingHeight = m_AnevD.dH;
		m_dCopyingWidth = m_AnevD.dB;

		double dD2_q = 0.0;
		double dD2 = 0.0;

		if (m_AnevD.aLayer.GetSize() > 0)
		{
			dD2_q = dD2 = DBL_MAX;
		}

		for (int i = 0; i < m_AnevD.aLayer.GetSize(); i++)
		{	
			dD2_q = __min(m_AnevD.aLayer[i].dD2_q, dD2_q);
			dD2 = __min(m_AnevD.aLayer[i].dD2, dD2);
		}

		double dSumOfSay = 0.0;
		dSumOfSay = m_dCopyingHeight - dD2 - dD2_q;

		double dTopLen, dBotLen;
//check 		dTopLen = dD2_q + dSumOfSay * 0.5;		
//check 		dBotLen = dD2 + dSumOfSay * 0.5;

		dTopLen = dD2_q + dSumOfSay * 0.5;
		dBotLen = dD2 + dSumOfSay * 0.5;

		m_dCopyingTopBotY[0] = dTopLen;
		m_dCopyingTopBotY[1] = -dBotLen;
	}
// 	else
// 	{
// 		m_dCopyingHeight = m_AnevD.dD2 + m_AnevD.dD2_q;
// 		int nSize;
// 		nSize = m_AnevD.aSay.GetSize();
// 		for (int i = 0; i < nSize; i++)
// 		{
// 			m_dCopyingHeight += m_AnevD.aSay[i];
// 		}
// 
// 		m_dCopyingWidth = m_AnevD.dD1 + m_AnevD.dD1_q;
// 		nSize = m_AnevD.aSax.GetSize();
// 		for (int i = 0; i < nSize; i++)
// 		{
// 			m_dCopyingWidth += m_AnevD.aSax[i];
// 		}
// 
// 		double dSumOfSay = 0.0;
// 		dSumOfSay = m_dCopyingHeight - m_AnevD.dD2 - m_AnevD.dD2_q;
// 
// 		double dTopLen, dBotLen;
// 		dTopLen = m_AnevD.dD2_q + dSumOfSay * 0.5;
// 		dBotLen = m_AnevD.dD2 + dSumOfSay * 0.5;
// 
// 		m_dCopyingTopBotY[0] = dTopLen;
// 		m_dCopyingTopBotY[1] = -dBotLen;
// 	}
}

void CDgnSeisAnchorShapeMgr::CalcMainCoord(double dStation, double dOffset, double dAngle, double& dX, double& dY)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	if(fabs(dAngle) < dZero)
	{
		dX = dStation;
		dY = dOffset;
		return;
	}

	double dRad = dAngle * CMathFunc::m_trrad;

	double dLAdjust = fabs(dOffset) / tan(CMathFunc::m_pi*0.5-dRad);
	dLAdjust = ( dOffset*dRad < 0. ? -fabs(dLAdjust) : fabs(dLAdjust) );

	dX = dStation + dLAdjust;
	dY = dOffset;
}

void CDgnSeisAnchorShapeMgr::GetCopyingShape(CArray<double, double>& aX, CArray<double, double>& aY)
{
//	if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
	{
		aX.RemoveAll();
		aY.RemoveAll();

		if (m_AnevD.aLayer.GetSize() == 0)
		{
			aX.Add(0.0);
			aY.Add(0.0);
			return;
		}

		if (!m_bSetAnev)
		{
			return;
		}

		double dSumOfSay = 0.0;

		double dD2 = DBL_MAX;
		double dD2_q = DBL_MAX;

		for (int i = 0; i < m_AnevD.aLayer.GetSize(); i++)
		{
			dD2 = __min(dD2, m_AnevD.aLayer[i].dD2);
			dD2_q= __min(dD2, m_AnevD.aLayer[i].dD2_q);
		}

		dSumOfSay = m_dCopyingHeight - dD2 - dD2_q;

		double dTopLen, dBotLen;
 		
		double dTopLenTemp = dD2_q + dSumOfSay * 0.5;
		double dBotLenTemp = dD2 + dSumOfSay * 0.5;

		dTopLen = (dTopLenTemp + dBotLenTemp) / 2.0;
		dBotLen = (dTopLenTemp + dBotLenTemp) / 2.0;

		double dX, dY;
		CalcMainCoord(0.0, -dBotLen, m_AnevD.dTheta1, dX, dY);
		aX.Add(dX);
		aY.Add(dY);

		CalcMainCoord(0.0, dTopLen, m_AnevD.dTheta1, dX, dY);
		aX.Add(dX);
		aY.Add(dY);

		CalcMainCoord(m_dCopyingWidth, dTopLen, m_AnevD.dTheta2, dX, dY);
		aX.Add(dX);
		aY.Add(dY);

		CalcMainCoord(m_dCopyingWidth, -dBotLen, m_AnevD.dTheta2, dX, dY);
		aX.Add(dX);
		aY.Add(dY);
	}
// 	else
// 	{
// 		aX.RemoveAll();
// 		aY.RemoveAll();
// 		if (!m_bSetAnev)
// 		{
// 			return;
// 		}
// 		/*
// 		double dX, dY;
// 		CalcMainCoord(0.0, -m_dCopyingHeight * 0.5, m_AnevD.dTheta1, dX, dY);
// 		aX.Add(dX);
// 		aY.Add(dY);
// 
// 		CalcMainCoord(0.0, m_dCopyingHeight * 0.5, m_AnevD.dTheta1, dX, dY);
// 		aX.Add(dX);
// 		aY.Add(dY);
// 
// 		CalcMainCoord(m_dCopyingWidth, m_dCopyingHeight * 0.5, m_AnevD.dTheta2, dX, dY);
// 		aX.Add(dX);
// 		aY.Add(dY);
// 
// 		CalcMainCoord(m_dCopyingWidth, -m_dCopyingHeight * 0.5, m_AnevD.dTheta2, dX, dY);
// 		aX.Add(dX);
// 		aY.Add(dY);
// 		*/
// 
// 		double dSumOfSay = 0.0;
// 
// 		dSumOfSay = m_dCopyingHeight - m_AnevD.dD2 - m_AnevD.dD2_q;
// 
// 		double dTopLen, dBotLen;
// 		dTopLen = m_AnevD.dD2_q + dSumOfSay * 0.5;
// 		dBotLen = m_AnevD.dD2 + dSumOfSay * 0.5;
// 
// 		double dX, dY;
// 		CalcMainCoord(0.0, -dBotLen, m_AnevD.dTheta1, dX, dY);
// 		aX.Add(dX);
// 		aY.Add(dY);
// 
// 		CalcMainCoord(0.0, dTopLen, m_AnevD.dTheta1, dX, dY);
// 		aX.Add(dX);
// 		aY.Add(dY);
// 
// 		CalcMainCoord(m_dCopyingWidth, dTopLen, m_AnevD.dTheta2, dX, dY);
// 		aX.Add(dX);
// 		aY.Add(dY);
// 
// 		CalcMainCoord(m_dCopyingWidth, -dBotLen, m_AnevD.dTheta2, dX, dY);
// 		aX.Add(dX);
// 		aY.Add(dY);
// 	}

	
}

void CDgnSeisAnchorShapeMgr::GetBearingShape(int nIdxI, int nIdxJ, CArray<double, double>& aX, CArray<double, double>& aY)
{
	aX.RemoveAll();
	aY.RemoveAll();
	if(!m_bSetAnev || !m_bSetBrpr)
	{
		return;
	}

	if (nIdxJ >= m_aArBearingWidth[nIdxI].size()) 
		return;

	double dCenterX, dCenterY;

	CalcBearingCenterPos(nIdxI, nIdxJ, dCenterX, dCenterY);
	
	// 0.0 기준으로 Bearing 그리기
	aX.Add(m_aArBearingWidth[nIdxI][nIdxJ] * 0.5);
	aY.Add(-m_aArBearingHeight[nIdxI][nIdxJ] * 0.5);

	aX.Add(m_aArBearingWidth[nIdxI][nIdxJ] * 0.5);
	aY.Add(m_aArBearingHeight[nIdxI][nIdxJ] * 0.5);

	aX.Add(-m_aArBearingWidth[nIdxI][nIdxJ] * 0.5);
	aY.Add(m_aArBearingHeight[nIdxI][nIdxJ] * 0.5);

	aX.Add(-m_aArBearingWidth[nIdxI][nIdxJ] * 0.5);
	aY.Add(-m_aArBearingHeight[nIdxI][nIdxJ] * 0.5);

	// 회전
	for (int i = 0; i < aX.GetSize(); i++)
	{
//		if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
		{
			math_lib::rotate_2d(aX[i], aY[i], -m_AnevD.aLayer[m_AnevD.aLayer.GetSize()-1-nIdxI].dTheta_q2);	// aLayer에는 반대로 데이터가 입력되어있음...
		}
// 		else
// 		{
// 			math_lib::rotate_2d(aX[i], aY[i], -m_AnevD.dTheta_q2);
// 		}		
	}
	
	// Copying에 배치
	for (int i = 0; i < aX.GetSize(); i++)
	{
		aX[i] += dCenterX;
		aY[i] += dCenterY;
	}
}

BOOL CDgnSeisAnchorShapeMgr::GetDistAndPosByDir(int nBearingI, int nBearingJ, double dPos1X, double dPos1Y, double dPos2X, double dPos2Y, double dPntPosX, double dPntPosY, double dDirX, double dDirY, double& dDist, double& dRstPosX, double& dRstPosY)
{
	dDist = 0.0;
	double dLongDist;

	if(m_bSetAnev)
	{
		dLongDist = max(m_dCopyingWidth, m_dCopyingHeight) * 2.0;
	}
	else if(m_bSetBrpr)
	{
		dLongDist = max(m_aArBearingWidth[nBearingI][nBearingJ], m_aArBearingHeight[nBearingI][nBearingJ]) * 2.0;
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

	T_POINT2D Line[2];
	Line[0].x = Line[1].x = dPntPosX;
	Line[0].y = Line[1].y = dPntPosY;
	PntMove(dDirX, dDirY, dLongDist, Line[0].x, Line[0].y);

	double cross[2];
	if(!GetLineInterSectPoint(dPos1X, dPos1Y, dPos2X, dPos2Y, Line[0].x, Line[0].y, Line[1].x, Line[1].y, cross[0], cross[1]))
	{
		return FALSE;
	}
	dRstPosX = cross[0];
	dRstPosY = cross[1];

	dDist = math_lib::distance_2d(dPntPosX, dPntPosY, dRstPosX, dRstPosY);

	return TRUE;
}

void CDgnSeisAnchorShapeMgr::CalcEachCa(std::vector<TAnchorPosIdx>& aCaIdx, double Dir[2], 
									   double& dCaPos1X, double& dCaPos1Y, double& dCaPos2X, double& dCaPos2Y, double& dCa, 
									   int& nCaBearingIIdx, int& nCaBearingJIdx, int& nCaAnchorIIdx, int& nCaAnchorJIdx)
{
	dCaPos1X = dCaPos1Y = dCaPos2X = dCaPos2Y = dCa = 0.0;
	nCaBearingIIdx = nCaBearingJIdx = nCaAnchorIIdx = nCaAnchorJIdx = 0;

	int nSize;
	nSize = (int)aCaIdx.size();
	if(nSize == 0)
	{
		return;
	}
	
	CArray<double, double> aCopingX, aCopingY;
	GetCopyingShape(aCopingX, aCopingY);
	if (aCopingX.GetSize() == 0)
	{
		ASSERT(0);
		return ;
	}
	
	int nBearingI, nBearingJ, nAnchorI, nAnchorJ, nNextCopingIdx;
	double Anchor[2], dRstPos[2], dDist;
	
	dCa = DBL_MAX;
	for (int i = 0; i < nSize ; i++)
	{
		nBearingI  = aCaIdx[i].nBearingI;
		nBearingJ  = aCaIdx[i].nBearingJ;
		nAnchorI   = aCaIdx[i].nAnchorI;
		nAnchorJ   = aCaIdx[i].nAnchorJ;

		for (int j = 0; j < aCopingX.GetSize(); j++)
		{
			if(j == aCopingX.GetSize() - 1)
			{
				nNextCopingIdx = 0;
			}
			else
			{
				nNextCopingIdx = j + 1;
			}

			CalcAnchorPos(nBearingI, nBearingJ, nAnchorI, nAnchorJ, Anchor[0], Anchor[1]);
			if (!GetDistAndPosByDir(nBearingI, nBearingJ, aCopingX[j], aCopingY[j], aCopingX[nNextCopingIdx], aCopingY[nNextCopingIdx], Anchor[0], Anchor[1], Dir[0], Dir[1], dDist, dRstPos[0], dRstPos[1]))
				continue;

			if (dDist < dCa)
			{
				dCa = dDist;

				dCaPos1X = Anchor[0];
				dCaPos1Y = Anchor[1];
				dCaPos2X = dRstPos[0];
				dCaPos2Y = dRstPos[1];
				nCaBearingIIdx = nBearingI;
				nCaBearingJIdx = nBearingJ;
				nCaAnchorIIdx = nAnchorI;
				nCaAnchorJIdx = nAnchorJ;
			}
		}	
	}
}

BOOL CDgnSeisAnchorShapeMgr::GetPryoutBearingCa(int nDir, int nBearingIdxI, int nBearingIdxJ, double& dCa1, double& dCa1P, double& dCa2, double& dCa2P)
{
	std::vector<TAnchorPosIdx> aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx; 
	GetPryoutCaIdxAtBearing(nDir, nBearingIdxI, nBearingIdxJ, aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx);

	// Dummy
	double dCa1Pos1X, dCa1Pos1Y, dCa1Pos2X, dCa1Pos2Y, dCa1PPos1X, dCa1PPos1Y, dCa1PPos2X, dCa1PPos2Y, dCa2Pos1X, dCa2Pos1Y, dCa2Pos2X, dCa2Pos2Y, dCa2PPos1X, dCa2PPos1Y, dCa2PPos2X, dCa2PPos2Y;
	int nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx;		

	return GetCaByIdx(nDir, aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx,
		dCa1, dCa1Pos1X, dCa1Pos1Y, dCa1Pos2X, dCa1Pos2Y, 
		nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
		dCa1P, dCa1PPos1X, dCa1PPos1Y, dCa1PPos2X, dCa1PPos2Y, 
		dCa2, dCa2Pos1X, dCa2Pos1Y, dCa2Pos2X, dCa2Pos2Y, 
		dCa2P, dCa2PPos1X, dCa2PPos1Y, dCa2PPos2X, dCa2PPos2Y);
}

BOOL CDgnSeisAnchorShapeMgr::GetConcFailBearingCa(int nDir, int nBearingIdxI, int nBearingIdxJ, double& dCa1, double& dCa1P, double& dCa2, double& dCa2P)
{
	std::vector<TAnchorPosIdx> aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx;
	
	GetConcFailCaIdxAtBearing(nDir, nBearingIdxI, nBearingIdxJ, aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx);
	// Dummy
	double dCa1Pos1X, dCa1Pos1Y, dCa1Pos2X, dCa1Pos2Y, dCa1PPos1X, dCa1PPos1Y, dCa1PPos2X, dCa1PPos2Y, dCa2Pos1X, dCa2Pos1Y, dCa2Pos2X, dCa2Pos2Y, dCa2PPos1X, dCa2PPos1Y, dCa2PPos2X, dCa2PPos2Y;
	int nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx;		

	return GetCaByIdx(nDir, aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx,
		dCa1, dCa1Pos1X, dCa1Pos1Y, dCa1Pos2X, dCa1Pos2Y, 
		nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
		dCa1P, dCa1PPos1X, dCa1PPos1Y, dCa1PPos2X, dCa1PPos2Y, 
		dCa2, dCa2Pos1X, dCa2Pos1Y, dCa2Pos2X, dCa2Pos2Y, 
		dCa2P, dCa2PPos1X, dCa2PPos1Y, dCa2PPos2X, dCa2PPos2Y);
}

BOOL CDgnSeisAnchorShapeMgr::GetCaByIdx(int nDir, std::vector<TAnchorPosIdx>& aCa1Idx, std::vector<TAnchorPosIdx>& aCa1PIdx, std::vector<TAnchorPosIdx>& aCa2Idx, std::vector<TAnchorPosIdx>& aCa2PIdx,
										double& dCa1, double& dCa1Pos1X, double& dCa1Pos1Y, double& dCa1Pos2X, double& dCa1Pos2Y, 
										int& nCa1BearingIIdx, int& nCa1BearingJIdx, int& nCa1AnchorIIdx, int& nCa1AnchorJIdx,
										double& dCa1P, double& dCa1PPos1X, double& dCa1PPos1Y, double& dCa1PPos2X, double& dCa1PPos2Y, 
										double& dCa2, double& dCa2Pos1X, double& dCa2Pos1Y, double& dCa2Pos2X, double& dCa2Pos2Y, 
										double& dCa2P, double& dCa2PPos1X, double& dCa2PPos1Y, double& dCa2PPos2X, double& dCa2PPos2Y)
{
	CArray<double, double> aCopingX, aCopingY;
	GetCopyingShape(aCopingX, aCopingY);
	if (aCopingX.GetSize() == 0)
	{
		return FALSE;
	}

	double Dir[2], Dir90[2], DirRev[2], Dir90Rev[2];
	CArray<double, double> aBearingX, aBearingY;
	GetBearingShape(0, 0, aBearingX, aBearingY);
	if(aBearingX.GetSize() == 0)
	{
		return FALSE;
	}
	if(nDir == DIR_LONGI)
	{
		GetDir(aBearingX[2], aBearingY[2], aBearingX[1], aBearingY[1], Dir[0], Dir[1]);
	}
	else if(nDir == DIR_TRANS)
	{
		GetDir(aBearingX[0], aBearingY[0], aBearingX[1], aBearingY[1], Dir[0], Dir[1]);
	}
	Dir90[0] = Dir[1];
	Dir90[1] = -Dir[0];

	DirRev[0] = -Dir[0];
	DirRev[1] = -Dir[1];
		
	Dir90Rev[0] = -Dir90[0];
	Dir90Rev[1] = -Dir90[1];

	int nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx;
	
	// Ca1
	if(nDir == DIR_LONGI)
	{	
		if (m_nDownUp == DIR_POS)
		{
			CalcEachCa(aCa1Idx, Dir90,
				dCa1Pos1X, dCa1Pos1Y, dCa1Pos2X, dCa1Pos2Y, dCa1,
				nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx);
		}
		else if (m_nDownUp == DIR_NEG)
		{
			CalcEachCa(aCa1Idx, Dir90Rev,
				dCa1Pos1X, dCa1Pos1Y, dCa1Pos2X, dCa1Pos2Y, dCa1,
				nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx);
		}
		
	}
	else if(nDir == DIR_TRANS)
	{
		if (m_nLeftRight == DIR_POS)
		{
			CalcEachCa(aCa1Idx, Dir90Rev,
				dCa1Pos1X, dCa1Pos1Y, dCa1Pos2X, dCa1Pos2Y, dCa1,
				nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx);
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			CalcEachCa(aCa1Idx, Dir90,
				dCa1Pos1X, dCa1Pos1Y, dCa1Pos2X, dCa1Pos2Y, dCa1,
				nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx);
		}
	}
	else
	{
		ASSERT(0);
	}

    // Ca1P
    if ( nDir == DIR_LONGI )
	{
		if (m_nDownUp == DIR_POS)
		{
			CalcEachCa(aCa1PIdx, Dir90Rev,
				dCa1PPos1X, dCa1PPos1Y, dCa1PPos2X, dCa1PPos2Y, dCa1P,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
		else if (m_nDownUp == DIR_NEG)
		{
			CalcEachCa(aCa1PIdx, Dir90,
				dCa1PPos1X, dCa1PPos1Y, dCa1PPos2X, dCa1PPos2Y, dCa1P,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
        
    }
    else if ( nDir == DIR_TRANS )
	{
		if (m_nLeftRight == DIR_POS)
		{
			CalcEachCa(aCa1PIdx, Dir90,
				dCa1PPos1X, dCa1PPos1Y, dCa1PPos2X, dCa1PPos2Y, dCa1P,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			CalcEachCa(aCa1PIdx, Dir90Rev,
				dCa1PPos1X, dCa1PPos1Y, dCa1PPos2X, dCa1PPos2Y, dCa1P,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
        
    }
    else
    {
        ASSERT(0);
    }

    // Ca2
    if ( nDir == DIR_LONGI )
    {	
		if (m_nDownUp == DIR_POS)
		{
			CalcEachCa(aCa2Idx, DirRev,
				dCa2Pos1X, dCa2Pos1Y, dCa2Pos2X, dCa2Pos2Y, dCa2,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
		else if (m_nDownUp == DIR_NEG)
		{
			CalcEachCa(aCa2Idx, Dir,
				dCa2Pos1X, dCa2Pos1Y, dCa2Pos2X, dCa2Pos2Y, dCa2,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
        
    }
    else if ( nDir == DIR_TRANS )
    {	
		if (m_nLeftRight == DIR_POS)
		{
			CalcEachCa(aCa2Idx, Dir,
				dCa2Pos1X, dCa2Pos1Y, dCa2Pos2X, dCa2Pos2Y, dCa2,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			CalcEachCa(aCa2Idx, DirRev,
				dCa2Pos1X, dCa2Pos1Y, dCa2Pos2X, dCa2Pos2Y, dCa2,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
        
    }
    else
    {
        ASSERT(0);
    }

    // Ca2P
    if ( nDir == DIR_LONGI )
    {	
		if (m_nDownUp == DIR_POS)
		{
			CalcEachCa(aCa2PIdx, Dir,
				dCa2PPos1X, dCa2PPos1Y, dCa2PPos2X, dCa2PPos2Y, dCa2P,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
		else if (m_nDownUp == DIR_NEG)
		{
			CalcEachCa(aCa2PIdx, DirRev,
				dCa2PPos1X, dCa2PPos1Y, dCa2PPos2X, dCa2PPos2Y, dCa2P,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
       
    }
    else if ( nDir == DIR_TRANS )
    {	
		if (m_nLeftRight == DIR_POS)
		{
			CalcEachCa(aCa2PIdx, DirRev,
				dCa2PPos1X, dCa2PPos1Y, dCa2PPos2X, dCa2PPos2Y, dCa2P,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			CalcEachCa(aCa2PIdx, Dir,
				dCa2PPos1X, dCa2PPos1Y, dCa2PPos2X, dCa2PPos2Y, dCa2P,
				nCaBearingIIdx, nCaBearingJIdx, nCaAnchorIIdx, nCaAnchorJIdx);
		}

    }
    else
    {
        ASSERT(0);
    }

	return TRUE;
}

BOOL CDgnSeisAnchorShapeMgr::GetConcFailCa(int nDir, double& dCa1, double& dCa1Pos1X, double& dCa1Pos1Y, double& dCa1Pos2X, double& dCa1Pos2Y, 
										int& nCa1BearingIIdx, int& nCa1BearingJIdx, int& nCa1AnchorIIdx, int& nCa1AnchorJIdx,
										double& dCa1P, double& dCa1PPos1X, double& dCa1PPos1Y, double& dCa1PPos2X, double& dCa1PPos2Y, 
										double& dCa2, double& dCa2Pos1X, double& dCa2Pos1Y, double& dCa2Pos2X, double& dCa2Pos2Y, 
										double& dCa2P, double& dCa2PPos1X, double& dCa2PPos1Y, double& dCa2PPos2X, double& dCa2PPos2Y,
										std::vector<std::pair<int, int>>& aRepresentBearingIdx, BOOL bFixedCond)
{	

	std::vector<TAnchorPosIdx> aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx; 

	GetConcFailCaIdx(nDir, aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx, aRepresentBearingIdx, bFixedCond);
	
	return GetCaByIdx(nDir, aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx,
		dCa1, dCa1Pos1X, dCa1Pos1Y, dCa1Pos2X, dCa1Pos2Y, 
		nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
		dCa1P, dCa1PPos1X, dCa1PPos1Y, dCa1PPos2X, dCa1PPos2Y, 
		dCa2, dCa2Pos1X, dCa2Pos1Y, dCa2Pos2X, dCa2Pos2Y, 
		dCa2P, dCa2PPos1X, dCa2PPos1Y, dCa2PPos2X, dCa2PPos2Y);
}

void CDgnSeisAnchorShapeMgr::GetFirstLastBearingIdx(int nDir, int nType, int nRepresentGrp, int& nFirstIdx, int& nLastIdx, BOOL bFixedCond, int nFailType)
{
	// nType : 0=기본, 1=1열 배치시 제일 윗받침 제외
	// nRepresentGrp : -1=대표그룹고려 안함, 0=고려&ConcFail, 1=고려&Pryout

	int nBearingNumI, nBearingNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 계산이 가능하도록 수정 필요

	int nAnchorNumI, nAnchorNumJ;	
	GetAnchorNumber(0, 0, nAnchorNumI, nAnchorNumJ);

	if(nDir == DIR_LONGI)
	{
		int nFirstBotBearing = 0;
		int nLastTopBearing = 0;

		int m_nTempDownUp;
		m_nTempDownUp = (nFailType == 0) ? m_nDownUp : m_nDownUpPryMgr;
		if (m_nTempDownUp == 0)		{
			for (int i = 0; i < nBearingNumI; i++)
			{
				BOOL bFind = FALSE;
				for (int j = 0; j < nBearingNumJ; j++)
				{
					if (IsFixedBearing(i, j, nDir, bFixedCond) && IsSelectedBearing(i, j, nDir, nFailType))
					{
						if (nRepresentGrp == 1 && !IsPryoutRepresentGrp(nDir, i, j))
						{
							continue;
						}

						bFind = TRUE;
						nFirstBotBearing = i;
						break;
					}
				}
				if (bFind) break;
			}
			for (int i = nBearingNumI - 1; i >= 0; i--)
			{
				BOOL bFind = FALSE;
				for (int j = 0; j < nBearingNumJ; j++)
				{
					if (IsFixedBearing(i, j, nDir, bFixedCond) && IsSelectedBearing(i, j, nDir, nFailType))
					{
						if (nRepresentGrp == 1 && !IsPryoutRepresentGrp(nDir, i, j))
						{
							continue;
						}

						bFind = TRUE;
						nLastTopBearing = i;
						break;
					}
				}
				if (bFind) break;
			}
		}
		
		else if (m_nTempDownUp == 1)
		{
			for (int i = nBearingNumI-1; i >= 0; i--)
			{
				BOOL bFind = FALSE;
				for (int j = 0; j < nBearingNumJ; j++)
				{
					if (IsFixedBearing(i, j, nDir, bFixedCond) && IsSelectedBearing(i, j, nDir, nFailType))
					{
						if (nRepresentGrp == 1 && !IsPryoutRepresentGrp(nDir, i, j))
						{
							continue;
						}

						bFind = TRUE;
						nFirstBotBearing = i;
						break;
					}
				}
				if (bFind) break;
			}
			for (int i = 0; i < nBearingNumI; i++)
			{
				BOOL bFind = FALSE;
				for (int j = 0; j < nBearingNumJ; j++)
				{
					if (IsFixedBearing(i, j, nDir, bFixedCond) && IsSelectedBearing(i, j, nDir, nFailType))
					{
						if (nRepresentGrp == 1 && !IsPryoutRepresentGrp(nDir, i, j))
						{
							continue;
						}

						bFind = TRUE;
						nLastTopBearing = i;
						break;
					}
				}
				if (bFind) break;
 			}
		}

		if (nType == 1 && nFirstBotBearing != nLastTopBearing && nAnchorNumI == 1)
		{
			// Pryout일때.. anchor가 1행으로 배치되어 있으면.. 가장 윗쪽 행의 베어링은 대상에서 제외 됨.. 추가로 베어링이 한 행이 모두 가동단인 경우도 고려
			for (int i = nLastTopBearing - 1; i >= 0; i--)
			{
				BOOL bFind = FALSE;
				for (int j = 0; j < nBearingNumJ; j++)
				{
					if (IsFixedBearing(i, j, nDir, bFixedCond) && IsSelectedBearing(i, j, nDir, nFailType))
					{
						if (nRepresentGrp == 1 && !IsPryoutRepresentGrp(nDir, i, j))
						{
							continue;
						}
						bFind = TRUE;
						nLastTopBearing = i;
						break;
					}
				}
				if (bFind) break;
			}

		}
		nFirstIdx = nFirstBotBearing;
		nLastIdx = nLastTopBearing;
	}
	else if(nDir == DIR_TRANS)
	{
		int nFirstLeftBearing = 0;
		int nLastRightBearing = 0;
		
		int m_nTempLeftRight;
		m_nTempLeftRight = (nFailType == 0) ? m_nLeftRight : m_nLeftRightPryMgr;
		if (m_nTempLeftRight == 0)
		{
			for (int j = 0; j < nBearingNumJ; j++)
			{
				BOOL bFind = FALSE;
				for (int i = 0; i < nBearingNumI; i++)
				{
					if (IsFixedBearing(i, j, nDir, bFixedCond) && IsSelectedBearing(i, j, nDir, nFailType))
					{
						if (nRepresentGrp == 1 && !IsPryoutRepresentGrp(nDir, i, j))
						{
							continue;
						}
						bFind = TRUE;
						nFirstLeftBearing = j;
						break;
					}
				}
				if (bFind) break;
			}
			for (int j = nBearingNumJ - 1; j >= 0; j--)
			{
				BOOL bFind = FALSE;
				for (int i = 0; i < nBearingNumI; i++)
				{
					if (IsFixedBearing(i, j, nDir, bFixedCond) && IsSelectedBearing(i, j, nDir, nFailType))
					{
						if (nRepresentGrp == 1 && !IsPryoutRepresentGrp(nDir, i, j))
						{
							continue;
						}
						bFind = TRUE;
						nLastRightBearing = j;
						break;
					}
				}
				if (bFind) break;
			}
		}
		else if (m_nTempLeftRight == 1)
		{
			for (int j = nBearingNumJ-1; j >=0; j--)
			{
				BOOL bFind = FALSE;
				for (int i = nBearingNumI-1; i >= 0; i--)
				{
					if (IsFixedBearing(i, j, nDir, bFixedCond) && IsSelectedBearing(i, j, nDir, nFailType))
					{
						if (nRepresentGrp == 1 && !IsPryoutRepresentGrp(nDir, i, j))
						{
							continue;
						}
						bFind = TRUE;
						nFirstLeftBearing = j;
						break;
					}
				}
				if (bFind) break;
			}
			for (int j =  0; j < nBearingNumJ; j++)
			{
				BOOL bFind = FALSE;
				for (int i = nBearingNumI-1; i >= 0; i--)
				{
					if (IsFixedBearing(i, j, nDir, bFixedCond) && IsSelectedBearing(i, j, nDir, nFailType))
					{
						if (nRepresentGrp == 1 && !IsPryoutRepresentGrp(nDir, i, j))
						{
							continue;
						}
						bFind = TRUE;
						nLastRightBearing = j;
						break;
					}
				}
				if (bFind) break;
			}
		}
		if(nType == 1 && nFirstLeftBearing != nLastRightBearing && nAnchorNumJ == 1)
		{
			// Pryout일때.. anchor가 1행으로 배치되어 있으면.. 가장 윗쪽 행의 베어링은 대상에서 제외 됨.. 추가로 베어링이 한 행이 모두 가동단인 경우도 고려
			for (int j = nLastRightBearing; j >= 0; j--)		
			{
				BOOL bFind = FALSE;
				for (int i = 0; i < nBearingNumI; i++)
				{
                    if ( IsFixedBearing(i, j, nDir, bFixedCond) && IsSelectedBearing(i, j, nDir, nFailType))
                    {
						if(nRepresentGrp == 1 && !IsPryoutRepresentGrp(nDir, i, j))
						{
							continue;
						}
                        bFind = TRUE;
                        nLastRightBearing = j;
                        break;
                    }
                }
				if(bFind) break;
			}
		}

		nFirstIdx = nFirstLeftBearing;
		nLastIdx = nLastRightBearing;
	}
}

BOOL CDgnSeisAnchorShapeMgr::CheckFilter(int nBearingI, int nBearingJ, std::vector<std::pair<int, int>>& aRepresentBearingIdx)
{
	if(aRepresentBearingIdx.size() == 0)
		return TRUE;	

	for (int i = 0; i < aRepresentBearingIdx.size(); i++)
	{
		if(nBearingI == aRepresentBearingIdx[i].first && nBearingJ == aRepresentBearingIdx[i].second)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CDgnSeisAnchorShapeMgr::GetConcFailCaAnchorIdx(int nDir, int nType, int nBearingI, int nBearingJ, std::vector<int>& aIdxI, std::vector<int>& aIdxJ)
{
	// nType = 0:Ca1, 1:Ca1P, 2:Ca2, 3:Ca2P

	aIdxI.clear();
	aIdxJ.clear();

	int nAnchorNumI, nAnchorNumJ;
	GetAnchorNumber(nBearingI, nBearingJ, nAnchorNumI, nAnchorNumJ);
	if (nDir == 0)
	{
		if (nType == 0)
		{	
			// Ca1			
			if (m_nDownUp == DIR_POS)
			{
				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(0);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			else if (m_nDownUp == DIR_NEG)
			{
				aIdxI.push_back(0);
				aIdxJ.push_back(0);

				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
		}
		else if (nType == 1)
		{
			// Ca1P
			if (m_nDownUp == DIR_POS)
			{
				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(0);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			else if (m_nDownUp == DIR_NEG)
			{
				aIdxI.push_back(0);
				aIdxJ.push_back(0);

				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
		}
		else if (nType == 2)
		{
			// Ca2
			if (m_nDownUp == DIR_POS)
			{
				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(0);
			}
			else if (m_nDownUp == DIR_NEG)
			{
				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ-1);
			}
		}
		else if (nType == 3)
		{
			// Ca2P
			if (m_nDownUp == DIR_POS)
			{
				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			else if (m_nDownUp == DIR_NEG)
			{
				aIdxI.push_back(0);
				aIdxJ.push_back(0);
			}
		}
		else
		{
			ASSERT(0);
		}
	}
	else if (nDir == 1)
	{
		if (nType == 0)
		{
			// Ca1
			if (m_nLeftRight == DIR_POS)
			{
				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ - 1);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			else if (m_nLeftRight == DIR_NEG)
			{
				aIdxI.push_back(0);
				aIdxJ.push_back(0);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(0);
			}
		}
		else if (nType == 1)
		{
			// Ca1P
			if (m_nLeftRight == DIR_POS)
			{
				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ - 1);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			else if (m_nLeftRight == DIR_NEG)
			{
				aIdxI.push_back(0);
				aIdxJ.push_back(0);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(0);
			}
		}
		else if (nType == 2)
		{
			// Ca2
			if (m_nLeftRight == DIR_POS)
			{
				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			else if (m_nLeftRight == DIR_NEG)
			{
				aIdxI.push_back(0);
				aIdxJ.push_back(0);
				
			}
			
		}
		else if (nType == 3)
		{
			// Ca2P
			if (m_nLeftRight == DIR_POS)
			{
				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			else if (m_nLeftRight == DIR_NEG)
			{
				aIdxI.push_back(nAnchorNumI-1);
				aIdxJ.push_back(0);
			}
			
		}
		else
		{
			ASSERT(0);
		}
	}
}

void CDgnSeisAnchorShapeMgr::GetConcFailCaIdxAtBearing(int nDir, int nBearingIdxI, int nBearingIdxJ,
	std::vector<TAnchorPosIdx> &aCa1Idx, std::vector<TAnchorPosIdx> &aCa1PIdx, std::vector<TAnchorPosIdx> &aCa2Idx, std::vector<TAnchorPosIdx> &aCa2PIdx)
{
	std::vector<int> aAnchorIdxI, aAnchorIdxJ;
	
	// Ca1
	nBearingIdxI = nBearingIdxI;
	nBearingIdxJ = nBearingIdxJ;
	GetConcFailCaAnchorIdx(nDir, 0, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
	for (int j = 0; j < aAnchorIdxI.size(); j++)
	{
		aCa1Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
	}

	// Ca'1
	nBearingIdxI = nBearingIdxI;
	nBearingIdxJ = nBearingIdxJ;
	GetConcFailCaAnchorIdx(nDir, 1, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
	for (int j = 0; j < aAnchorIdxI.size(); j++)
	{
		aCa1PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
	}

	// Ca2
	nBearingIdxI = nBearingIdxI;
	nBearingIdxJ = nBearingIdxJ;
	GetConcFailCaAnchorIdx(nDir, 2, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
	for (int j = 0; j < aAnchorIdxI.size(); j++)
	{
		aCa2Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
	}

	// Ca'2
	nBearingIdxI = nBearingIdxI;
	nBearingIdxJ = nBearingIdxJ;
	GetConcFailCaAnchorIdx(nDir, 3, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
	for (int j = 0; j < aAnchorIdxI.size(); j++)
	{
		aCa2PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
	}
}

void CDgnSeisAnchorShapeMgr::GetConcFailCaIdx(int nDir, std::vector<TAnchorPosIdx> &aCa1Idx, std::vector<TAnchorPosIdx> &aCa1PIdx, std::vector<TAnchorPosIdx> &aCa2Idx, std::vector<TAnchorPosIdx> &aCa2PIdx,
											  std::vector<std::pair<int, int>>& aBearingIdxFilter, BOOL bFixedCond)
{
	int nBearingNumI, nBearingNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 게산이 가능하도록 수정 필요

	std::vector<int> aAnchorIdxI, aAnchorIdxJ;
	int nBearingIdxI, nBearingIdxJ;

	// 콘크리트 파괴용 Ca1P는 Ca1 앵커를 사용한다.
	
	int nFailType = 0;			// 0: Concrete Fail 1: Pryout Fail
	
	if (nDir == DIR_LONGI)
	{
		int nFirstBotBearing = 0;
		int nLastTopBearing = 0;
		GetFirstLastBearingIdx(nDir, 0, 0, nFirstBotBearing, nLastTopBearing, bFixedCond, nFailType);

		// Ca1
		for (int i = 0; i < nBearingNumJ; i++)
		{
			nBearingIdxI = nFirstBotBearing;
			nBearingIdxJ = i;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetConcFailCaAnchorIdx(nDir, 0, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa1Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca'1
		for (int i = 0; i < nBearingNumJ; i++)
		{
			nBearingIdxI = nFirstBotBearing;
			nBearingIdxJ = i;

			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetConcFailCaAnchorIdx(nDir, 1, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa1PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca2
		for (int i = 0; i < nBearingNumJ; i++)
		{
			nBearingIdxI = nFirstBotBearing;
			nBearingIdxJ = i;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetConcFailCaAnchorIdx(nDir, 2, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa2Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca'2
		for (int i = nBearingNumJ - 1; i >= 0; i--)
		{
			nBearingIdxI = nFirstBotBearing;
			nBearingIdxJ = i;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetConcFailCaAnchorIdx(nDir, 3, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa2PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}
	}
	else if (nDir == DIR_TRANS)
	{
		int nFirstLeftBearing = 0;
		int nLastRightBearing = 0;
		GetFirstLastBearingIdx(nDir, 0, 0, nFirstLeftBearing, nLastRightBearing, bFixedCond, nFailType);

// 		for (int j = 0; j < nBearingNumJ; j++)
// 		{
// 			BOOL bFind = FALSE;
// 			for (int i = 0; i < nBearingNumI; i++)
// 			{
// 				if (IsFixedBearing(i, j, nDir, bFixedCond) && CheckFilter(i, j, aBearingIdxFilter))
// 				{
// 					bFind = TRUE;
// 					nFirstLeftBearing = j;
// 					break;
// 				}
// 			}
// 			if (bFind) break;
// 		}
// 		for (int j = nBearingNumJ - 1; j >= 0; j--)
// 		{
// 			BOOL bFind = FALSE;
// 			for (int i = 0; i < nBearingNumI; i++)
// 			{
// 				if (IsFixedBearing(i, j, nDir, bFixedCond) && CheckFilter(i, j, aBearingIdxFilter))
// 				{
// 					bFind = TRUE;
// 					nLastRightBearing = j;
// 					break;
// 				}
// 			}
// 			if (bFind) break;
// 		}

		// Ca1
		for (int i = 0; i < nBearingNumI; i++)
		{
			nBearingIdxI = i;
			nBearingIdxJ = nFirstLeftBearing;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetConcFailCaAnchorIdx(nDir, 0, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa1Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca'1
		for (int i = 0; i < nBearingNumI; i++)
		{
			nBearingIdxI = i;
			nBearingIdxJ = nFirstLeftBearing;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetConcFailCaAnchorIdx(nDir, 1, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa1PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca2	
		for (int i = nBearingNumI - 1; i >= 0; i--)
		{
			nBearingIdxI = i;
			nBearingIdxJ = nFirstLeftBearing;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetConcFailCaAnchorIdx(nDir, 2, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa2Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca'2
		for (int i = 0; i < nBearingNumI; i++)
		{
			nBearingIdxI = i;
			nBearingIdxJ = nFirstLeftBearing;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetConcFailCaAnchorIdx(nDir, 3, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa2PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}
	}
	else
	{
		ASSERT(0);
	}
}

BOOL CDgnSeisAnchorShapeMgr::GetConcFailRepresentVal4Ratio(int nDir, double& dLp, int& nBc, double& dCa1, double& dCa1P, double& dCa2, double& dCa2P, TConcFailBcCalc& MinConcFailCal, BOOL bFixedCond)
{
	if (!CheckData())
	{
		dLp = dCa1 = dCa1P = dCa2 = dCa2P = 0.0;;
		nBc = 0;
		return FALSE;
	}

// 	TConcFailBcCalc MinConcFailBc;
// 	GetConcFailBc(nDir, MinConcFailBc, bFixedCond);

	dLp = MinConcFailCal.dLp;
	nBc = MinConcFailCal.nBc;

	double Ca1Pos[4], Ca1PPos[4], Ca2Pos[4], Ca2PPos[4];
	int nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx;
	return GetConcFailCa(nDir, dCa1,
		Ca1Pos[0], Ca1Pos[1], Ca1Pos[2], Ca1Pos[3],
		nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
		dCa1P, Ca1PPos[0], Ca1PPos[1], Ca1PPos[2],
		Ca1PPos[3], dCa2, Ca2Pos[0], Ca2Pos[1],
		Ca2Pos[2], Ca2Pos[3], dCa2P, Ca2PPos[0],
		Ca2PPos[1], Ca2PPos[2], Ca2PPos[3], MinConcFailCal.aBearingIdx, bFixedCond);
}


BOOL CDgnSeisAnchorShapeMgr::GetConcFailRepresentVal(int nDir, double& dLp, int& nBc, double& dCa1, double& dCa1P, double& dCa2, double& dCa2P, BOOL bFixedCond)
{ // 이 함수에서의 dCa1는 대표 그룹내에서의 bearing 들 중에서 최소 Ca1를 계산하고 있음.
  // 그래서 실제 검토를 위한 Ca1의 계산은 이 함수 바깥에서 고려됨.
	if(!CheckData())
	{
		dLp = dCa1 = dCa1P = dCa2 = dCa2P = 0.0;;
		nBc = 0;
		return FALSE;
	}

	TConcFailBcCalc MinConcFailBc;
	if (nDir == DIR_LONGI)
		GetConcFailBc(nDir, MinConcFailBc, bFixedCond);
	else if (nDir == DIR_TRANS)
		GetConcFailBcRecal(nDir, MinConcFailBc, bFixedCond);
	else
		ASSERT(0);

	dLp = MinConcFailBc.dLp;
	nBc = MinConcFailBc.nBc;

	double Ca1Pos[4], Ca1PPos[4], Ca2Pos[4], Ca2PPos[4];
	int nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx;
	BOOL bChk = GetConcFailCa(nDir, dCa1, 
		Ca1Pos[0], Ca1Pos[1], Ca1Pos[2], Ca1Pos[3], 
		nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
		dCa1P, Ca1PPos[0], Ca1PPos[1], Ca1PPos[2], 
		Ca1PPos[3], dCa2, Ca2Pos[0], Ca2Pos[1], 
		Ca2Pos[2], Ca2Pos[3], dCa2P, Ca2PPos[0], 
		Ca2PPos[1], Ca2PPos[2], Ca2PPos[3], MinConcFailBc.aBearingIdx, bFixedCond);

	double dCa2temp = min(dCa2, dCa2P);
	double dCa2Ptemp = max(dCa2, dCa2P);
	dCa2 = dCa2temp;
	dCa2P = dCa2Ptemp;

	return  bChk;
}

void CDgnSeisAnchorShapeMgr::SetLineSmaller(double& xi, double& yi, double& xj, double& yj)
{
	double dSmallVal = M_InitValueCurUnit(0.1, N, MM, D_UNITSYS_BASE_LENGTH);

	double Dir[2];
	GetDir(xi, yi, xj, yj, Dir[0], Dir[1]);
	PntMove(Dir[0], Dir[1], dSmallVal, xi, yi);
	PntMove(Dir[0], Dir[1], -dSmallVal, xj, yj);
}

BOOL CDgnSeisAnchorShapeMgr::SetLineSmallerAndCheckInclude(int nDir, double xi, double yi, double xj, double yj, double x, double y, double Tol)
{
	if(nDir == DIR_LONGI)
	{		
		yi = 0.0;
		yj = 0.0;
		y = 0.0;		
	}
	else if(nDir == DIR_TRANS)
	{
		xi = 0.0;
		xj = 0.0;
		x = 0.0;
	}
	else
	{
		ASSERT(0);
	}

	SetLineSmaller(xi, yi, xj, yj);
	return CMathFunc::mathIncludePointInLine(xi, yi, xj, yj, x, y, Tol);
}

void CDgnSeisAnchorShapeMgr::GetConcFailBc(int nDir, TConcFailBcCalc& MinConcFailBc, BOOL bFixedCond)
{
	MinConcFailBc.Init();

	std::vector<TConcFailBcCalc> aConcFailBc;
	GetConcFailBc(nDir, aConcFailBc, bFixedCond);
	if(aConcFailBc.size() == 0)
	{
		return;
	}
	//double dMinLp = DBL_MAX;
	//int nMaxBc = 0;

	GetMinConcFailBc(nDir, aConcFailBc, MinConcFailBc);
}

void CDgnSeisAnchorShapeMgr::GetMinConcFailBc(int nDir, std::vector<TConcFailBcCalc>& aConcFailBc, TConcFailBcCalc& MinConcFailBc)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
	double dMin = DBL_MAX;
	double dMax = -DBL_MAX;
	int nIdx = -1;

	int nSize;
	nSize = (int)aConcFailBc.size();
	for (int i = 0; i < nSize ; i++)
	{
		// 설계쪽 ratio 구현
		double dRatio = CalcConcFailBearingRepresentRatio(nDir, aConcFailBc[i]);
		if(dRatio < dMin && fabs(dRatio - dMin) > dZero)
		{
			dMin = dRatio;
			nIdx = i;
		}
	}
	if(nIdx == -1)
	{
		return;
	}

	MinConcFailBc = aConcFailBc[nIdx];
}


static std::map<KeyForConFailPos, ValForConFail> m_mapConFailData;
static std::map<KeyForPryoutPos, ValForPryout> m_mapPryoutData;

static std::map<KeyForPryoutBCPos, ValForPryoutBC> m_mapPryoutBCData;
static std::vector<TPryoutGroupInfo> m_aPryoutGroupInfo_X;
static std::vector<TPryoutGroupInfo> m_aPryoutGroupInfo_Y;
static int m_nPryoutRepresentGrpIdx_X;
static int m_nPryoutRepresentGrpIdx_Y;

void CDgnSeisAnchorShapeMgr::InitMapData4CalcShape()
{
	m_mapConFailData.clear();
	m_mapPryoutData.clear();

	m_mapPryoutBCData.clear();
	m_aPryoutGroupInfo_X.clear();
	m_aPryoutGroupInfo_Y.clear();
	m_nPryoutRepresentGrpIdx_X = -1;
	m_nPryoutRepresentGrpIdx_Y = -1;
}

double CDgnSeisAnchorShapeMgr::CalcConcFailBearingRepresentRatio(int nDir, TConcFailBcCalc& EachGroup)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();	

	T_BREV_D BrevD;
	if(!pDoc->m_pAttrCtrl2->GetBrev(m_AnevD.BrevK, BrevD)) ASSERT(0);

	int nRatType[4] = { m_nDownUp, m_nLeftRight, m_nDownUpPryMgr ,m_nLeftRightPryMgr };

	double dRatio = 0.0;
	KeyForConFailPos Key(m_AnevD.BrevK, nDir, EachGroup);
	auto itrFind = m_mapConFailData.find(Key);
	if (itrFind == m_mapConFailData.end())
	{ // 없으면
		T_BRDGEVAL_BEARING rResD;
		if(!m_pSeisEval->Get_BearingParam(m_AnevD.BrevK, rResD, FALSE, m_bAnchorDirPosi))
		{
			//ASSERT(0);
			return 0.0;
		}
		dRatio = m_pSeisEval->Calc_BearingAnchor_Concrete_Ratio(m_AnevD, BrevD, EachGroup, EachGroup.aBearingIdx, rResD, nDir, nRatType);

		ValForConFail Val;
		Val.Set(dRatio);
		m_mapConFailData[Key] = Val;
	}
	else
	{
		ValForConFail& Val = itrFind->second;

		dRatio = Val.dRatio;
	}

	return dRatio;

}

double CDgnSeisAnchorShapeMgr::CalcPryoutBearingRepresentRatio(int nDir, int nBearingI, int nBearingJ)
{
	for (int i = 0; i < m_aPryoutGroupInfo.size(); i++)
	{
		for (int j = 0; j < m_aPryoutGroupInfo[i].aBearingIdx.size(); j++)
		{
			if(m_aPryoutGroupInfo[i].aBearingIdx[j].first == nBearingI && m_aPryoutGroupInfo[i].aBearingIdx[j].second == nBearingJ)
			{
				return CalcPryoutBearingRepresentRatio(nDir, i);
			}
		}
	}
	ASSERT(0);
	return DBL_MAX;
}

double CDgnSeisAnchorShapeMgr::CalcPryoutBearingRepresentRatio(int nDir, int nRepresentGrpIdx)
{
// 	// 이거 내용 채워야 함
//	return m_aPryoutGroupInfo[nRepresentGrpIdx].dArea;
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();	

	T_BREV_D BrevD;
	if(!pDoc->m_pAttrCtrl2->GetBrev(m_AnevD.BrevK, BrevD)) ASSERT(0);

	int nRatType[4] = { m_nDownUp, m_nLeftRight, m_nDownUpPryMgr ,m_nLeftRightPryMgr };

	double dRatio = 0.0;
	KeyForPryoutPos Key(m_AnevD.BrevK, nDir, m_aPryoutGroupInfo[nRepresentGrpIdx]);
	auto itrFind = m_mapPryoutData.find(Key);
	if (itrFind == m_mapPryoutData.end())
	{ // 없으면
		T_BRDGEVAL_BEARING rResD;
		if(!m_pSeisEval->Get_BearingParam(m_AnevD.BrevK, rResD, FALSE))
		{
			ASSERT(0);
			return 0.0;
		}
		dRatio = m_pSeisEval->Calc_BearingAnchor_Pryout_Ratio(m_AnevD, BrevD, nRepresentGrpIdx, m_aPryoutGroupInfo[nRepresentGrpIdx].aBearingIdx, rResD, nDir, nRatType, m_bAnchorTenConc);
		dRatio = CMathFunc::mathRoundOff(dRatio, 5);

		ValForPryout Val;
		Val.Set(dRatio);
		m_mapPryoutData[Key] = Val;
	}
	else
	{
		ValForPryout& Val = itrFind->second;

		dRatio = Val.dRatio;

	}

	return dRatio;

}

void CDgnSeisAnchorShapeMgr::GetConcFailBc(int nDir, std::vector<TConcFailBcCalc>& aConcFailBc,
	                                       BOOL FixedCond/* = FALSE*/, int nBi/* = 0*/, int nBj/* = 0*/)
{
	aConcFailBc.clear();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
		
	double CopingLine[2][2];

	CArray<double, double> aCopingX, aCopingY;
	GetCopyingShape(aCopingX, aCopingY);

	if(nDir == DIR_LONGI)
	{	
		if (m_nDownUp == DIR_POS)
		{
			CopingLine[0][0] = aCopingX[0];
			CopingLine[0][1] = aCopingY[0];

			CopingLine[1][0] = aCopingX[3];
			CopingLine[1][1] = aCopingY[3];
		}
		else if (m_nDownUp == DIR_NEG)
		{
			CopingLine[0][0] = aCopingX[2];
			CopingLine[0][1] = aCopingY[2];

			CopingLine[1][0] = aCopingX[1];
			CopingLine[1][1] = aCopingY[1];
		}
	}
	else if(nDir == DIR_TRANS)
	{	
		if (m_nLeftRight == DIR_POS)
		{
			CopingLine[0][0] = aCopingX[0];
			CopingLine[0][1] = aCopingY[0];

			CopingLine[1][0] = aCopingX[1];
			CopingLine[1][1] = aCopingY[1];
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			CopingLine[0][0] = aCopingX[2];
			CopingLine[0][1] = aCopingY[2];

			CopingLine[1][0] = aCopingX[3];
			CopingLine[1][1] = aCopingY[3];
		}
	}
	else
	{
		ASSERT(0);
	}

	// Step1. 고정단/가동단을 고려해서 시작 선을 잡고... 다음 bearing 이랑 연결되면 선을 늘리고.. 연결 되지 않으면 aDataForMin 에 세팅
	CArray<int, int> aBearingI, aBearingJ, aAnchorI, aAnchorJ;
	GetPrjTgtIdxConcFail(nDir, aBearingI, aBearingJ, aAnchorI, aAnchorJ, FixedCond, nBi, nBj);

	if(aBearingI.GetSize() < 1 || aBearingJ.GetSize() < 1 || aAnchorI.GetSize() < 1 || aAnchorJ.GetSize() < 1)
	{		
		return;
	}
	ASSERT(aBearingI.GetSize() == aBearingJ.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorI.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorJ.GetSize());

	BOOL bLastLoopMerge = FALSE;

	TConcFailBcCalc EachData;
	double AnchorPos[2], ProjPos1[2], ProjPos2[2];
	AnchorPos[0] = AnchorPos[1] = ProjPos1[0] = ProjPos1[1] = ProjPos2[0] = ProjPos2[1]= 0.0;

	int BearingIdx[2][2] = {0,};
	double dSaxLeft, dSaxRight;

	BOOL bSetNext, bSetPrev;

	int nSize;
	nSize = aBearingI.GetSize();

	if(nSize == 1)
	{
		CalcAnchorPos(aBearingI[0], aBearingJ[0], aAnchorI[0], aAnchorJ[0], AnchorPos[0], AnchorPos[1]);
		CalcAnchorProjPosConcFail(nDir, aBearingI[0], aBearingJ[0], aAnchorI[0], aAnchorJ[0], ProjPos1[0], ProjPos1[1], ProjPos2[0], ProjPos2[1]);
		if(!CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos1[0], ProjPos1[1], dZero))
		{
			EachData.LpLine[0][0] = CopingLine[0][0];
			EachData.LpLine[0][1] = CopingLine[0][1];
		}
		else
		{
			EachData.LpLine[0][0] = ProjPos1[0];
			EachData.LpLine[0][1] = ProjPos1[1];
		}

		if(!CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos2[0], ProjPos2[1], dZero))
		{
			EachData.LpLine[1][0] = CopingLine[1][0];
			EachData.LpLine[1][1] = CopingLine[1][1];
		}
		else
		{
			EachData.LpLine[1][0] = ProjPos2[0];
			EachData.LpLine[1][1] = ProjPos2[1];
		}
		EachData.nBc = 1;
		EachData.aBearingIdx.push_back(std::pair<int, int>(aBearingI[0], aBearingJ[0]));
		aConcFailBc.push_back(EachData);
	}
	else
	{
		for (int i = 0; i < nSize - 1; i++)
		{	
			CalcAnchorPos(aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], AnchorPos[i], AnchorPos[1]);
			CalcAnchorProjPosConcFail(nDir, aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], ProjPos1[0], ProjPos1[1], ProjPos2[0], ProjPos2[1]);
			
			if(!CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos1[0], ProjPos1[1], dZero))
			{
				EachData.LpLine[0][0] = CopingLine[0][0];
				EachData.LpLine[0][1] = CopingLine[0][1];
			}
			else
			{
				EachData.LpLine[0][0] = ProjPos1[0];
				EachData.LpLine[0][1] = ProjPos1[1];
			}

			if(!CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos2[0], ProjPos2[1], dZero))
			{
				EachData.LpLine[1][0] = CopingLine[1][0];
				EachData.LpLine[1][1] = CopingLine[1][1];
			}
			else
			{
				EachData.LpLine[1][0] = ProjPos2[0];
				EachData.LpLine[1][1] = ProjPos2[1];
			}
			EachData.nBc = 1;
			EachData.aBearingIdx.push_back(std::pair<int, int>(aBearingI[i], aBearingJ[i]));

			bSetNext = FALSE;
		  //if(i < nSize - 2 && (aBearingI[i] != aBearingI[i + 1] || aBearingJ[i] != aBearingJ[i + 1]))
			if(i < nSize - 1 && (aBearingI[i] != aBearingI[i + 1] || aBearingJ[i] != aBearingJ[i + 1]))
			{
				bSetNext = TRUE;
			}
			bSetPrev = FALSE;
			if(i != 0 && (aBearingI[i] != aBearingI[i - 1] || aBearingJ[i] != aBearingJ[i - 1]))
			{
				bSetPrev = TRUE;
			}
			if(bSetNext)
			{
				BearingIdx[0][0] = aBearingI[i];
				BearingIdx[0][1] = aBearingJ[i];
				BearingIdx[1][0] = aBearingI[i + 1];
				BearingIdx[1][1] = aBearingJ[i + 1];
				dSaxRight = GetBearingGap(nDir, BearingIdx);
			}
			if(bSetPrev)
			{
				BearingIdx[0][0] = aBearingI[i];
				BearingIdx[0][1] = aBearingJ[i];
				BearingIdx[1][0] = aBearingI[i - 1];
				BearingIdx[1][1] = aBearingJ[i - 1];
				dSaxLeft = GetBearingGap(nDir, BearingIdx);
			}
			if(bSetNext && bSetPrev)
			{
				EachData.dSax = min(dSaxLeft ,dSaxRight);
			}
			else if(bSetNext)
			{
				EachData.dSax = dSaxRight;			
			}
			else if(bSetPrev)
			{
				EachData.dSax = dSaxLeft;
			}

            int j = 0;
			for (j = i + 1; j < nSize ; j++)
			{
				CalcAnchorProjPosConcFail(nDir, aBearingI[j], aBearingJ[j], aAnchorI[j], aAnchorJ[j], ProjPos1[0], ProjPos1[1], ProjPos2[0], ProjPos2[1]);
				BOOL bCheck01 = CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos1[0], ProjPos1[1], dZero);
				BOOL bCheck02;
				if (nDir == 0 && EachData.LpLine[0][0] == ProjPos1[0])
				{
					bCheck02 = TRUE;
				}
				else if (nDir == 0 && EachData.LpLine[0][0] != ProjPos1[0])
				{
					bCheck02 = SetLineSmallerAndCheckInclude(nDir, EachData.LpLine[0][0], EachData.LpLine[0][1], EachData.LpLine[1][0], EachData.LpLine[1][1], ProjPos1[0], ProjPos1[1], dZero);
				}
				else if (nDir == 1 && EachData.LpLine[0][1] == ProjPos1[1])
				{
					bCheck02 = TRUE;
				}
				else if (nDir == 1 && EachData.LpLine[0][1] != ProjPos1[1])
				{
					bCheck02 = SetLineSmallerAndCheckInclude(nDir, EachData.LpLine[0][0], EachData.LpLine[0][1], EachData.LpLine[1][0], EachData.LpLine[1][1], ProjPos1[0], ProjPos1[1], dZero);
				}
				else
				{
					ASSERT(0);
					bCheck02 = FALSE;
				}

				if(!bCheck01 || bCheck02 )
				{
// 					if (nDir == DIR_TRANS && m_nLeftRight == DIR_NEG)
// 					{
// 						ASSERT(ProjPos1[1] <= ProjPos2[1]);
// 						// ProjPos1
// 						if (CMathFunc::mathIncludePointInLine(EachData.LpLine[0][0], EachData.LpLine[0][1], EachData.LpLine[1][0], EachData.LpLine[1][1], ProjPos1[0], ProjPos1[1], dZero))
// 						{
// 							//ProjPos1이 기존 파괴 영역 안에 포함되는 경우이므로 EachData.LpLine[0][] 그대로 사용
// 							//EachData.LpLine[0][0]
// 							//EachData.LpLine[0][1]
// 						}
// 						// Lp 를 연장하는 경우
// 						else if (CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos1[0], ProjPos1[1], dZero))
// 						{
// 							// coping 의 끝부분에 부딧치지 않은 경우
// 							EachData.LpLine[0][0] = ProjPos2[0];
// 							EachData.LpLine[0][1] = ProjPos2[1];
// 						}
// 						else
// 						{
// 							// coping 의 끝부분에 부딧친 경우
// 							EachData.LpLine[0][0] = CopingLine[0][0];
// 							EachData.LpLine[0][1] = CopingLine[0][1];
// 						}
// 
// 						// ProjPos2
// 						if (CMathFunc::mathIncludePointInLine(EachData.LpLine[0][0], EachData.LpLine[0][1], EachData.LpLine[1][0], EachData.LpLine[1][1], ProjPos2[0], ProjPos2[1], dZero))
// 						{
// 							//ProjPos2가 기존 파괴 영역 안에 포함되는 경우이므로 EachData.LpLine[1][] 그대로 사용
// 							//EachData.LpLine[1][0]
// 							//EachData.LpLine[1][1]
// 						}
// 						// Lp 를 연장하는 경우
// 						else if (CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos2[0], ProjPos2[1], dZero))
// 						{
// 							// coping 의 끝부분에 부딧치지 않은 경우
// 							EachData.LpLine[1][0] = ProjPos2[0];
// 							EachData.LpLine[1][1] = ProjPos2[1];
// 						}
// 						else
// 						{
// 							// coping 의 끝부분에 부딧친 경우
// 							EachData.LpLine[1][0] = CopingLine[1][0];
// 							EachData.LpLine[1][1] = CopingLine[1][1];
// 						}
// 					}
// 					else
					{
						// Lp 를 연장하는 경우
						if (CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos2[0], ProjPos2[1], dZero))
						{
							// coping 의 끝부분에 부딧치지 않은 경우
							EachData.LpLine[1][0] = ProjPos2[0];
							EachData.LpLine[1][1] = ProjPos2[1];
						}
						else
						{
							// coping 의 끝부분에 부딧친 경우
							EachData.LpLine[1][0] = CopingLine[1][0];
							EachData.LpLine[1][1] = CopingLine[1][1];
						}
					}

					if(aBearingI[j] != aBearingI[j - 1] || aBearingJ[j] != aBearingJ[j - 1]) // Bc 다음 Bearing 으로 넘어 간 경우에만 증가시킴
					{
						EachData.nBc++;
						EachData.aBearingIdx.push_back(std::pair<int, int>(aBearingI[j], aBearingJ[j]));
					}

					bSetNext = FALSE;
					if(j < nSize - 2 && (aBearingI[j] != aBearingI[j + 1] || aBearingJ[j] != aBearingJ[j + 1]))
					{
						bSetNext = TRUE;
					}
					if(bSetNext)
					{
						BearingIdx[0][0] = aBearingI[j];
						BearingIdx[0][1] = aBearingJ[j];
						BearingIdx[1][0] = aBearingI[j + 1];
						BearingIdx[1][1] = aBearingJ[j + 1];
						dSaxRight = GetBearingGap(nDir, BearingIdx);
						EachData.dSax = EachData.dSax == 0.0 ? dSaxRight : min(EachData.dSax, dSaxRight);
					}
					bLastLoopMerge = TRUE;
				}
				else
				{	
					ASSERT(EachData.nBc != 0);

					aConcFailBc.push_back(EachData);
					EachData.Init();
					bLastLoopMerge = FALSE;			

					EachData.LpLine[0][0] = ProjPos1[0];
					EachData.LpLine[0][1] = ProjPos1[1];
					EachData.LpLine[1][0] = ProjPos2[0];
					EachData.LpLine[1][1] = ProjPos2[1];



					BearingIdx[0][0] = aBearingI[j - 1];
					BearingIdx[0][1] = aBearingJ[j - 1];
					BearingIdx[1][0] = aBearingI[j];
					BearingIdx[1][1] = aBearingJ[j];


					EachData.dSax = GetBearingGap(nDir, BearingIdx);

					EachData.nBc = 1;
					EachData.aBearingIdx.push_back(std::pair<int, int>(aBearingI[j], aBearingJ[j]));
				}
			}
			i = j - 1;
		}
		if(bLastLoopMerge)
		{
			aConcFailBc.push_back(EachData);
		}
		else
		{
			EachData.Init();
			EachData.nBc = 1;
			EachData.LpLine[0][0] = ProjPos1[0];
			EachData.LpLine[0][1] = ProjPos1[1];
			if(CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos2[0], ProjPos2[1], dZero))
			{
				// coping 의 끝부분에 부딧치지 않은 경우
				EachData.LpLine[1][0] = ProjPos2[0];
				EachData.LpLine[1][1] = ProjPos2[1];
			}
			else
			{
				// coping 의 끝부분에 부딧친 경우
				EachData.LpLine[1][0] = CopingLine[1][0];
				EachData.LpLine[1][1] = CopingLine[1][1];
			}

			bSetPrev = FALSE;

			int nIdxL, nIdxR;
			nIdxL = (nSize - 1 >= 0) ? nSize - 1 : 0;
			nIdxR = (nSize - 2 >= 0) ? nSize - 2 : 0;

			if(aBearingI[nIdxL] != aBearingI[nIdxR] || aBearingJ[nIdxL] != aBearingJ[nIdxR])
			{
				bSetPrev = TRUE;
			}
			if(bSetPrev)
			{
				BearingIdx[0][0] = aBearingI[nIdxL];
				BearingIdx[0][1] = aBearingJ[nIdxL];
				BearingIdx[1][0] = aBearingI[nIdxR];
				BearingIdx[1][1] = aBearingJ[nIdxR];
				dSaxLeft = GetBearingGap(nDir, BearingIdx);
				EachData.dSax = dSaxLeft;
			}
            EachData.aBearingIdx.push_back(std::pair<int, int>(BearingIdx[0][0], BearingIdx[0][1]));
			aConcFailBc.push_back(EachData);
		}
	}

	// Lp계산
	nSize = (int)aConcFailBc.size();
	for (int i = 0; i < nSize ; i++)
	{		
		aConcFailBc[i].dLp = math_lib::distance_2d(aConcFailBc[i].LpLine[0][0], aConcFailBc[i].LpLine[0][1], aConcFailBc[i].LpLine[1][0], aConcFailBc[i].LpLine[1][1]);
	}
}

BOOL CDgnSeisAnchorShapeMgr::GetPryoutCa(int nDir, double& dCa1, double& dCa1Pos1X, double& dCa1Pos1Y, double& dCa1Pos2X, double& dCa1Pos2Y, 
										 int& nCa1BearingIIdx, int& nCa1BearingJIdx, int& nCa1AnchorIIdx, int& nCa1AnchorJIdx,
										 double& dCa1P, double& dCa1PPos1X, double& dCa1PPos1Y, double& dCa1PPos2X, double& dCa1PPos2Y, 
										 double& dCa2, double& dCa2Pos1X, double& dCa2Pos1Y, double& dCa2Pos2X, double& dCa2Pos2Y, 
										 double& dCa2P, double& dCa2PPos1X, double& dCa2PPos1Y, double& dCa2PPos2X, double& dCa2PPos2Y)
{	
	if(m_nLastPryoutGroupDir != nDir)
	{
		int nBc;
		double dSax, dSay, dArea;
		BOOL bSingleResist;
		std::set<int> setBcIdx;
		CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&> aArProjShapeX, aArProjShapeY;
		std::vector<std::pair<int, int>> aRepBearingIdxTemp;
		GetPryoutBc(nDir, aRepBearingIdxTemp, aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bSingleResist);
	}

	std::vector<TAnchorPosIdx> aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx; 
	GetPryoutCaIdx(nDir, aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx, m_aPryoutGroupInfo[m_nPryoutRepresentGrpIdx].aBearingIdx);

	return GetCaByIdx(nDir, aCa1Idx, aCa1PIdx, aCa2Idx, aCa2PIdx,
		dCa1, dCa1Pos1X, dCa1Pos1Y, dCa1Pos2X, dCa1Pos2Y, 
		nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
		dCa1P, dCa1PPos1X, dCa1PPos1Y, dCa1PPos2X, dCa1PPos2Y, 
		dCa2, dCa2Pos1X, dCa2Pos1Y, dCa2Pos2X, dCa2Pos2Y, 
		dCa2P, dCa2PPos1X, dCa2PPos1Y, dCa2PPos2X, dCa2PPos2Y);
}

void CDgnSeisAnchorShapeMgr::GetPryoutCaAnchorIdx(int nDir, int nType, int nBearingI, int nBearingJ, std::vector<int>& aIdxI, std::vector<int>& aIdxJ)
{	
	// nType = 0:Ca1, 1:Ca1P, 2:Ca2, 3:Ca2P

	int nAnchorNumI, nAnchorNumJ;
	GetAnchorNumber(nBearingI, nBearingJ, nAnchorNumI, nAnchorNumJ);
	if (nDir == 0)
	{
		if (nType == 0)
		{	
			if (m_nDownUpPryMgr == DIR_POS)
			{
				// Ca1			
				aIdxI.push_back(0);
				aIdxJ.push_back(0);

				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			else if (m_nDownUpPryMgr == DIR_NEG)
			{
				// Ca1			
				aIdxI.push_back(nAnchorNumI-1);
				aIdxJ.push_back(0);

				aIdxI.push_back(nAnchorNumI-1);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			
		}
		else if (nType == 1)
		{	
			if (m_nDownUpPryMgr == DIR_POS)
			{
				// Ca1P
				aIdxI.push_back(0);
				aIdxJ.push_back(0);

				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			else if (m_nDownUpPryMgr == DIR_NEG)
			{
				// Ca1P
				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(0);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			
		}
		else if (nType == 2)
		{	
			if (m_nDownUpPryMgr == DIR_POS)
			{
				// Ca2
				aIdxI.push_back(0);
				aIdxJ.push_back(0);
			}
			else if (m_nDownUpPryMgr == DIR_NEG)
			{
				// Ca2
				aIdxI.push_back(nAnchorNumI-1);
				aIdxJ.push_back(nAnchorNumJ-1);
			}

		}
		else if (nType == 3)
		{
			if (m_nDownUpPryMgr == DIR_POS)
			{
				// Ca2P
				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}
			else if (m_nDownUpPryMgr == DIR_NEG)
			{
				// Ca2P
				aIdxI.push_back(nAnchorNumI-1);
				aIdxJ.push_back(0);
			}
			
		}
		else
		{
			ASSERT(0);
		}
	}
	else if (nDir == 1)
	{	
		if (nType == 0)
		{
			if (m_nLeftRightPryMgr == DIR_POS)
			{
				// Ca1
				aIdxI.push_back(0);
				aIdxJ.push_back(0);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(0);
			}
			else if (m_nLeftRightPryMgr == DIR_NEG)
			{
				// Ca1
				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ-1);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(nAnchorNumJ-1);
			}

		}
		else if (nType == 1)
		{
			if (m_nLeftRightPryMgr == DIR_POS)
			{
				// Ca1P
				aIdxI.push_back(0);
				aIdxJ.push_back(0);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(0);
			}
			else if (m_nLeftRightPryMgr == DIR_NEG)
			{
				// Ca1P
				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ - 1);

				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}

		}
		else if (nType == 2)
		{
			if (m_nLeftRightPryMgr == DIR_POS)
			{
				// Ca2
				aIdxI.push_back(nAnchorNumI - 1);
				aIdxJ.push_back(0);
			}
			else if (m_nLeftRightPryMgr == DIR_NEG)
			{
				// Ca2
				aIdxI.push_back(0);
				aIdxJ.push_back(nAnchorNumJ-1);
			}

		}
		else if (nType == 3)
		{
			if (m_nLeftRightPryMgr == DIR_POS)
			{
				// Ca2P
				aIdxI.push_back(0);
				aIdxJ.push_back(0);
			}
			else if (m_nLeftRightPryMgr == DIR_NEG)
			{
				// Ca2P
				aIdxI.push_back(nAnchorNumI-1);
				aIdxJ.push_back(nAnchorNumJ - 1);
			}

		}
		else
		{
			ASSERT(0);
		}
	}
}

void CDgnSeisAnchorShapeMgr::GetPryoutCaIdxAtBearing(int nDir, int nBearingIdxI, int nBearingIdxJ,
	std::vector<TAnchorPosIdx> &aCa1Idx, std::vector<TAnchorPosIdx> &aCa1PIdx, std::vector<TAnchorPosIdx> &aCa2Idx, std::vector<TAnchorPosIdx> &aCa2PIdx)
{
	std::vector<int> aAnchorIdxI, aAnchorIdxJ;
		
	// Ca1
	nBearingIdxI = nBearingIdxI;
	nBearingIdxJ = nBearingIdxJ;
	GetPryoutCaAnchorIdx(nDir, 0, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
	for (int j = 0; j < aAnchorIdxI.size(); j++)
	{
		aCa1Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
	}

	// Ca'1
	nBearingIdxI = nBearingIdxI;
	nBearingIdxJ = nBearingIdxJ;
	GetPryoutCaAnchorIdx(nDir, 1, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
	for (int j = 0; j < aAnchorIdxI.size(); j++)
	{
		aCa1PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
	}

	// Ca2
	nBearingIdxI = nBearingIdxI;
	nBearingIdxJ = nBearingIdxJ;
	GetPryoutCaAnchorIdx(nDir, 2, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
	for (int j = 0; j < aAnchorIdxI.size(); j++)
	{
		aCa2Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
	}

	// Ca'2
	nBearingIdxI = nBearingIdxI;
	nBearingIdxJ = nBearingIdxJ;
	GetPryoutCaAnchorIdx(nDir, 3, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
	for (int j = 0; j < aAnchorIdxI.size(); j++)
	{
		aCa2PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
	}
}

void CDgnSeisAnchorShapeMgr::GetPryoutCaIdx(int nDir, std::vector<TAnchorPosIdx> &aCa1Idx, std::vector<TAnchorPosIdx> &aCa1PIdx, std::vector<TAnchorPosIdx> &aCa2Idx, std::vector<TAnchorPosIdx> &aCa2PIdx, std::vector<std::pair<int, int>>& aBearingIdxFilter)
{
	int nBearingNumI, nBearingNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 게산이 가능하도록 수정 필요
	
	std::vector<int> aAnchorIdxI, aAnchorIdxJ;
	int nBearingIdxI, nBearingIdxJ;

	int nFailType = 1;	// 0: Concrete Fail		1: Pryout Fail

	if(nDir == DIR_LONGI)
	{
		int nFirstBotBearing = 0;
		int nLastTopBearing = 0;
		GetFirstLastBearingIdx(nDir, 0, 1, nFirstBotBearing, nLastTopBearing, TRUE, nFailType);

		// Ca1
		for (int i = 0; i < nBearingNumJ; i++)
		{
			nBearingIdxI = nFirstBotBearing;
			nBearingIdxJ = i;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetPryoutCaAnchorIdx(nDir, 0, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa1Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca'1
		for (int i = 0; i < nBearingNumJ; i++)
		{
			nBearingIdxI = nFirstBotBearing;
			nBearingIdxJ = i;

			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetPryoutCaAnchorIdx(nDir, 1, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa1PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca2
		for (int i = 0; i < aBearingIdxFilter.size(); i++)
		{
			nBearingIdxI = aBearingIdxFilter[i].first;
			nBearingIdxJ = aBearingIdxFilter[i].second;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetPryoutCaAnchorIdx(nDir, 2, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa2Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}
		/*
		for (int i = 0; i < nBearingNumJ; i++)
		{
			nBearingIdxI = nFirstBotBearing;
			nBearingIdxJ = i;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetPryoutCaAnchorIdx(nDir, 2, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa2Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}
		*/


		// Ca'2
		for (int i = aBearingIdxFilter.size() - 1; i >= 0; i--)
		{
			nBearingIdxI = aBearingIdxFilter[i].first;
			nBearingIdxJ = aBearingIdxFilter[i].second;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetPryoutCaAnchorIdx(nDir, 3, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa2PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}
		/*
		for (int i = nBearingNumJ - 1; i >= 0; i--)
		{
			nBearingIdxI = nFirstBotBearing;
			nBearingIdxJ = i;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetPryoutCaAnchorIdx(nDir, 3, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa2PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}
		*/
		
	}
	else if(nDir == DIR_TRANS)
	{
		int nFirstLeftBearing = 0;
		int nLastRightBearing = 0;
		GetFirstLastBearingIdx(nDir, 0, 1, nFirstLeftBearing, nLastRightBearing, TRUE, nFailType);

		
		// Ca1
		for (int i = 0; i < nBearingNumI; i++)
		{
			nBearingIdxI = i;
			nBearingIdxJ = nFirstLeftBearing;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetPryoutCaAnchorIdx(nDir, 0, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa1Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca'1
		for (int i = nBearingNumI - 1; i >= 0; i--)
		{
			nBearingIdxI = i;
			nBearingIdxJ = nFirstLeftBearing;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetPryoutCaAnchorIdx(nDir, 1, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa1PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca2
		for (int i = nBearingNumI - 1; i >= 0; i--)
		{
			nBearingIdxI = i;
			nBearingIdxJ = nFirstLeftBearing;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetPryoutCaAnchorIdx(nDir, 2, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa2Idx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}

		// Ca'2
		for (int i = 0; i < nBearingNumI; i++)
		{
			nBearingIdxI = i;
			nBearingIdxJ = nFirstLeftBearing;
			if (IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) && CheckFilter(nBearingIdxI, nBearingIdxJ, aBearingIdxFilter) && IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
			{
				GetPryoutCaAnchorIdx(nDir, 3, nBearingIdxI, nBearingIdxJ, aAnchorIdxI, aAnchorIdxJ);
				for (int j = 0; j < aAnchorIdxI.size(); j++)
				{
					aCa2PIdx.push_back(TAnchorPosIdx(nBearingIdxI, nBearingIdxJ, aAnchorIdxI[j], aAnchorIdxJ[j]));
				}
			}
		}
	}
	else
	{
		ASSERT(0);
	}
}

BOOL CDgnSeisAnchorShapeMgr::GetPryoutRepresentVal(int nDir, std::vector<std::pair<int, int>> aRepBearingIdx4Ratio, double& dArea, int& nBc, double& dCa1, double& dCa1P, double& dCa2, double& dCa2P, BOOL& bSingleResist, int& nResistAnchorNum)
{
	dArea = 0.0;
	nBc = 0;

	double dSax, dSay;
	std::set<int> setBcIdx;
	CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&> aArProjShapeX, aArProjShapeY;
	GetPryoutBc(nDir, aRepBearingIdx4Ratio, aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bSingleResist);
	
	if(bSingleResist)
	{
		nResistAnchorNum = 1; 
	}
	else
	{
		std::vector<std::pair<int, int>> aRepBearingIdx;
		if(aRepBearingIdx4Ratio.size()==0)
		{
			GetPryoutRepresentGrpBearing(nDir, aRepBearingIdx);
		}
		else
		{
			aRepBearingIdx = aRepBearingIdx4Ratio;
		}
		//ASSERT(aRepBearingIdx.size() == 1);
		nResistAnchorNum = 0;

		int nBearingI, nBearingJ;

		for (int i = 0; i < aRepBearingIdx.size(); i++)
		{
			nBearingI = aRepBearingIdx[i].first;
			nBearingJ = aRepBearingIdx[i].second;
			int nAnchorNumI, nAnchorNumJ;
			GetAnchorNumber(nBearingI, nBearingJ, nAnchorNumI, nAnchorNumJ);
			if (nDir == DIR_LONGI)
			{
				nResistAnchorNum += nAnchorNumJ;
			}
			else if (nDir == DIR_TRANS)
			{
				nResistAnchorNum += nAnchorNumI;
			}
			else
			{
				ASSERT(0);
			}
		}
	}

	if(aArProjShapeX.GetSize() == 0)
	{
		return FALSE;
	}

	double Ca1Pos[4], Ca1PPos[4], Ca2Pos[4], Ca2PPos[4];
	int nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx;

	return GetPryoutCa(nDir, dCa1, Ca1Pos[0], Ca1Pos[1], Ca1Pos[2], Ca1Pos[3], 
		nCa1BearingIIdx, nCa1BearingJIdx, nCa1AnchorIIdx, nCa1AnchorJIdx,
		dCa1P, Ca1PPos[0], Ca1PPos[1], Ca1PPos[2], Ca1PPos[3], 
		dCa2, Ca2Pos[0], Ca2Pos[1], Ca2Pos[2], Ca2Pos[3], 
		dCa2P, Ca2PPos[0], Ca2PPos[1], Ca2PPos[2], Ca2PPos[3]);
}

double CDgnSeisAnchorShapeMgr::GetBearingCenterGap(int nDir, int BearingIdx[2][2])
{
	if(!m_bSetBrpr || !m_bSetAnev)
	{
		ASSERT(0);
		return 0.0;
	}

	int nBearingNumI, nBearingNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 게산이 가능하도록 수정 필요

	if(nBearingNumI == 0 || nBearingNumJ == 0)
	{
		return 0.0;
	}

	if(BearingIdx[0][0] < 0.0 || BearingIdx[0][1] < 0.0 || BearingIdx[1][0] < 0.0 || BearingIdx[1][1] < 0.0)
	{
		ASSERT(0);
		return 0.0;
	}

	if(BearingIdx[0][0] >= nBearingNumI || BearingIdx[0][1] >= nBearingNumJ || BearingIdx[1][0] >= nBearingNumI || BearingIdx[1][1] >= nBearingNumJ)
	{
		//ASSERT(0);
		return 0.0;
	}

	double dCenter[2][2];
	CalcBearingCenterPos(BearingIdx[0][0], BearingIdx[0][1], dCenter[0][0], dCenter[0][1]);
	CalcBearingCenterPos(BearingIdx[1][0], BearingIdx[1][1], dCenter[1][0], dCenter[1][1]);

	double dDist = math_lib::distance_2d(dCenter[0], dCenter[1]);
	return dDist;
}

double CDgnSeisAnchorShapeMgr::GetBearingGap(int nDir, int BearingIdx[2][2])
{
	if(!m_bSetBrpr || !m_bSetAnev)
	{
		ASSERT(0);
		return 0.0;
	}

	double pl1_i[3], pl1_j[3], pl2_i[3], pl2_j[3]; 
	pl1_i[2] = pl1_j[2] = pl2_i[2] = pl2_j[2] = 0.0;

	int nBearingNumI, nBearingNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 게산이 가능하도록 수정 필요

	if(nBearingNumI == 0 || nBearingNumJ == 0)
	{
		return 0.0;
	}
	
	if(BearingIdx[0][0] < 0.0 || BearingIdx[0][1] < 0.0 || BearingIdx[1][0] < 0.0 || BearingIdx[1][1] < 0.0)
	{
		ASSERT(0);
		return 0.0;
	}

	if(BearingIdx[0][0] >= nBearingNumI || BearingIdx[0][1] >= nBearingNumJ || BearingIdx[1][0] >= nBearingNumI || BearingIdx[1][1] >= nBearingNumJ)
	{
		//ASSERT(0);
		return 0.0;
	}
	
	CArray<double, double> aFirstX, aFirstY, aSecondX, aSecondY;
	if(nDir == DIR_LONGI)
	{
		if(nBearingNumJ <= 1)
		{
			return 0.0;
		}

		if(BearingIdx[0][1] == BearingIdx[1][1])
		{	
			//ASSERT(0);
			return 0.0;
		}
		
		GetBearingShape(BearingIdx[0][0], BearingIdx[0][1], aFirstX, aFirstY);
		GetBearingShape(BearingIdx[1][0], BearingIdx[1][1], aSecondX, aSecondY);
		
		if(BearingIdx[0][1] < BearingIdx[1][1])
		{
			pl1_i[0] = aFirstX[0];
			pl1_i[1] = aFirstY[0];
			pl1_j[0] = aFirstX[1];
			pl1_j[1] = aFirstY[1];

			pl2_i[0] = aSecondX[3];
			pl2_i[1] = aSecondY[3];
			pl2_j[0] = aSecondX[2];
			pl2_j[1] = aSecondY[2];
		}
		else
		{
			pl1_i[0] = aFirstX[3];
			pl1_i[1] = aFirstY[3];
			pl1_j[0] = aFirstX[2];
			pl1_j[1] = aFirstY[2];

			pl2_i[0] = aSecondX[0];
			pl2_i[1] = aSecondY[0];
			pl2_j[0] = aSecondX[1];
			pl2_j[1] = aSecondY[1];
		}
	}
	else if(nDir == DIR_TRANS)
	{
		if(nBearingNumI <= 1)
		{
			return 0.0;
		}

		if(BearingIdx[0][0] == BearingIdx[1][0])
		{	
			//ASSERT(0);
			return 0.0;
		}
		
		GetBearingShape(BearingIdx[0][0], BearingIdx[0][1], aFirstX, aFirstY);
		GetBearingShape(BearingIdx[1][0], BearingIdx[1][1], aSecondX, aSecondY);

		if(BearingIdx[0][0] < BearingIdx[1][0])
		{
			pl1_i[0] = aFirstX[2];
			pl1_i[1] = aFirstY[2];
			pl1_j[0] = aFirstX[1];
			pl1_j[1] = aFirstY[1];

			pl2_i[0] = aSecondX[3];
			pl2_i[1] = aSecondY[3];
			pl2_j[0] = aSecondX[0];
			pl2_j[1] = aSecondY[0];
		}
		else
		{
			pl1_i[0] = aFirstX[3];
			pl1_i[1] = aFirstY[3];
			pl1_j[0] = aFirstX[0];
			pl1_j[1] = aFirstY[0];

			pl2_i[0] = aSecondX[2];
			pl2_i[1] = aSecondY[2];
			pl2_j[0] = aSecondX[1];
			pl2_j[1] = aSecondY[1];
		}
	}
	else
	{
		ASSERT(0);
	}

	double dDist = CMathFunc::mathDistanceFromIntersectPointToLine(pl1_i, pl1_j, pl2_i);

	//[CIVIL-1145] sax 의 측정위치를 앵커to앵커로 수정
	if(nDir == DIR_LONGI)
	{
		dDist += m_aArBrprD[BearingIdx[0][0]][BearingIdx[0][1]].dSx + m_aArBrprD[BearingIdx[1][0]][BearingIdx[1][1]].dSx;
	}
	else if(nDir == DIR_TRANS)
	{
		dDist += m_aArBrprD[BearingIdx[0][0]][BearingIdx[0][1]].dSy + m_aArBrprD[BearingIdx[1][0]][BearingIdx[1][1]].dSy;
	}
	else
	{
		ASSERT(0);
	}

	return dDist;
}

void CDgnSeisAnchorShapeMgr::GetPrjTgtIdxPryout(int nDir, CArray<int, int>& aBearingI, CArray<int, int>& aBearingJ, CArray<int, int>& aAnchorI, CArray<int, int>& aAnchorJ)
{
	int nBearingNumI, nAnchorNumI, nAnchorNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	
	int nFailType = 1;

	int nFirstIdx, nLastIdx;
	GetFirstLastBearingIdx(nDir, 1, 0, nFirstIdx, nLastIdx, TRUE, nFailType);

	if (nDir == DIR_LONGI)
	{
		if (m_nDownUpPryMgr == DIR_POS)
		{
			for (int nBearingIdxI = 0; nBearingIdxI <= nLastIdx; nBearingIdxI++)
			{
				for (int nBearingIdxJ = 0; nBearingIdxJ < aBearingNumJ[nBearingIdxI]; nBearingIdxJ++)
				{
					if (!IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) || !IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
					{
						continue;
					}
					GetAnchorNumber(nBearingIdxI, nBearingIdxJ, nAnchorNumI, nAnchorNumJ);
					for (int nAIdx = 0; nAIdx < nAnchorNumJ; nAIdx++)
					{
						aBearingI.Add(nBearingIdxI);
						aBearingJ.Add(nBearingIdxJ);
						aAnchorI.Add(0);
						aAnchorJ.Add(nAIdx);
					}
					
					// Last
					if (m_AnevD.bPryoutRow)
					{
						for (int nAIdx = 0; nAIdx < nAnchorNumJ; nAIdx++)
						{
							aBearingI.Add(nBearingIdxI);
							aBearingJ.Add(nBearingIdxJ);
							aAnchorI.Add(nAnchorNumI - 1);
							aAnchorJ.Add(nAIdx);
						}
					}
				}
			}
		}

		else if (m_nDownUpPryMgr == DIR_NEG)
		{
			for (int nBearingIdxI = nFirstIdx; nBearingIdxI >= 0; nBearingIdxI--)
			{
				for (int nBearingIdxJ = aBearingNumJ[nBearingIdxI]-1; nBearingIdxJ >=0; nBearingIdxJ--)
				{
					if (!IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) || !IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
					{
						continue;
					}
					GetAnchorNumber(nBearingIdxI, nBearingIdxJ, nAnchorNumI, nAnchorNumJ);
					for (int nAIdx = 0; nAIdx < nAnchorNumJ; nAIdx++)
					{
						aBearingI.Add(nBearingIdxI);
						aBearingJ.Add(nBearingIdxJ);
						aAnchorI.Add(0);
						aAnchorJ.Add(nAIdx);
					}

					// Last
					if (m_AnevD.bPryoutRow)
					{
						for (int nAIdx = 0; nAIdx < nAnchorNumJ; nAIdx++)
						{
							aBearingI.Add(nBearingIdxI);
							aBearingJ.Add(nBearingIdxJ);
							aAnchorI.Add(nAnchorNumI - 1);
							aAnchorJ.Add(nAIdx);
						}
					}
				}
			}
		}

	}
	else if (nDir == DIR_TRANS)
	{
		if (m_nLeftRightPryMgr == DIR_POS)
		{
			for (int nBearingIdxJ = 0; nBearingIdxJ <= nLastIdx; nBearingIdxJ++)
			{
				for (int nBearingIdxI = 0; nBearingIdxI < nBearingNumI; nBearingIdxI++)
				{
					if (!IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) || !IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
					{
						continue;
					}
					GetAnchorNumber(nBearingIdxI, nBearingIdxJ, nAnchorNumI, nAnchorNumJ);
					for (int nAIdx = 0; nAIdx < nAnchorNumI; nAIdx++)
					{
						aBearingI.Add(nBearingIdxI);
						aBearingJ.Add(nBearingIdxJ);
						aAnchorI.Add(nAIdx);
						aAnchorJ.Add(0);
					}

					// Last
					if (m_AnevD.bPryoutRow)
					{
						for (int nAIdx = 0; nAIdx < nAnchorNumI; nAIdx++)
						{
							aBearingI.Add(nBearingIdxI);
							aBearingJ.Add(nBearingIdxJ);
							aAnchorI.Add(nAIdx);
							aAnchorJ.Add(nAnchorNumJ - 1);
						}
					}
				}
			}
		}
		else if (m_nLeftRightPryMgr == DIR_NEG)
		{
			for (int nBearingIdxJ = nFirstIdx; nBearingIdxJ >= 0; nBearingIdxJ--)
			{
				for (int nBearingIdxI = nBearingNumI - 1; nBearingIdxI >= 0; nBearingIdxI--)
				{
					if (!IsFixedBearing(nBearingIdxI, nBearingIdxJ, nDir) || !IsSelectedBearing(nBearingIdxI, nBearingIdxJ, nDir, nFailType))
					{
						continue;
					}
					GetAnchorNumber(nBearingIdxI, nBearingIdxJ, nAnchorNumI, nAnchorNumJ);
					for (int nAIdx = 0; nAIdx < nAnchorNumI; nAIdx++)
					{
						aBearingI.Add(nBearingIdxI);
						aBearingJ.Add(nBearingIdxJ);
						aAnchorI.Add(nAIdx);
						aAnchorJ.Add(0);
					}

					// Last
					if (m_AnevD.bPryoutRow)
					{
						for (int nAIdx = 0; nAIdx < nAnchorNumI; nAIdx++)
						{
							aBearingI.Add(nBearingIdxI);
							aBearingJ.Add(nBearingIdxJ);
							aAnchorI.Add(nAIdx);
							aAnchorJ.Add(nAnchorNumJ - 1);
						}

					}
				}
			}
		}
	}
	else
	{
		ASSERT(0);
	}
}

void CDgnSeisAnchorShapeMgr::GetPrjTgtIdxConcFail(int nDir, CArray<int, int>& aBearingI, CArray<int, int>& aBearingJ, CArray<int, int>& aAnchorI, CArray<int, int>& aAnchorJ, 
	                                              BOOL bFixedCond/*=FALSE*/, int nBinx/*=0*/, int nBjnx/*=0*/)
{
	int nBearingNumI, nBearingNumJ, nAnchorNumI, nAnchorNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 게산이 가능하도록 수정 필요

	int nBi, nBj, nAi, nAj;

	int nFailType = 0;

	if (nDir == DIR_LONGI)
	{
		if (m_nDownUp == DIR_POS)
		{
			for (int k = 0; k < nBearingNumI; k++)
			{
				BOOL bDone = FALSE;
				for (int i = 0; i < nBearingNumJ; i++)
				{
					GetAnchorNumber(k, i, nAnchorNumI, nAnchorNumJ);
					for (int j = 0; j < nAnchorNumJ; j++)
					{
						nBi = k;
						nBj = i;
						nAi = nAnchorNumI - 1;
						nAj = j;

						if (!IsFixedBearing(nBi, nBj, nDir, bFixedCond) || !IsSelectedBearing(nBi, nBj, nDir, nFailType))
						{
							continue;
						}

						bDone = TRUE;
						aBearingI.Add(nBi);
						aBearingJ.Add(nBj);
						aAnchorI.Add(nAi);
						aAnchorJ.Add(nAj);
					}
				}
				if (bDone)
				{
					break;
				}
			}			
		}
		else if (m_nDownUp == DIR_NEG)
		{
			for (int k = nBearingNumI-1; k >= 0; k--)
			{
				BOOL bDone = FALSE;
				for (int i = aBearingNumJ[k]-1; i >=0; i--)
				{
					GetAnchorNumber(k, i, nAnchorNumI, nAnchorNumJ);
					for (int j = nAnchorNumJ-1; j >=0;  j--)
					{
						nBi = k;
						nBj = i;
						nAi = 0;
						nAj = j;

						if (!IsFixedBearing(nBi, nBj, nDir, bFixedCond) || !IsSelectedBearing(nBi, nBj, nDir, nFailType))
						{
							continue;
						}

						bDone = TRUE;
						aBearingI.Add(nBi);
						aBearingJ.Add(nBj);
						aAnchorI.Add(nAi);
						aAnchorJ.Add(nAj);
					}
				}
				if (bDone)
				{
					break;
				}
			}
		}
	}
	else if(nDir == DIR_TRANS)
	{	
		if (m_nLeftRight == DIR_POS)
		{
			for (int k = nBjnx; k < nBearingNumJ; k++)
			{
				BOOL bDone = FALSE;
				for (int i = nBinx; i < nBearingNumI; i++)
				{
					GetAnchorNumber(i, k, nAnchorNumI, nAnchorNumJ);
					for (int j = 0; j < nAnchorNumI; j++)
					{
						nBi = i;
						nBj = k;
						nAi = j;
						nAj = nAnchorNumJ - 1;

						if (!IsFixedBearing(nBi, nBj, nDir, bFixedCond) || !IsSelectedBearing(nBi, nBj, nDir, nFailType))
						{
							continue;
						}
						bDone = TRUE;
						aBearingI.Add(nBi);
						aBearingJ.Add(nBj);
						aAnchorI.Add(nAi);
						aAnchorJ.Add(nAj);
					}
				}
				if (bDone)
				{
					break;
				}
			}
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			
			for (int k = nBearingNumJ - 1; k >= 0; k--)
			{
				BOOL bDone = FALSE;
				for (int i = nBearingNumI - 1; i >= 0; i--)
				{
					GetAnchorNumber(i, k, nAnchorNumI, nAnchorNumJ);
					for (int j = nAnchorNumI-1; j >=0 ; j--)
					{
						nBi = i;
						nBj = k;
						nAi = j;
						nAj = 0;

						if (!IsFixedBearing(nBi, nBj, nDir, bFixedCond) || !IsSelectedBearing(nBi, nBj, nDir, nFailType))
						{
							continue;
						}
						bDone = TRUE;
						aBearingI.Add(nBi);
						aBearingJ.Add(nBj);
						aAnchorI.Add(nAi);
						aAnchorJ.Add(nAj);
					}
				}
				if (bDone)
				{
					break;
				}
			}
		}
	}
		
	/*
	else if(nDir == DIR_TOP)
	{
		for (int i = 0; i < nBearingNumJ; i++)
		{
			for (int j = 0; j < nAnchorNumJ; j++)
			{
				aBearingI.Add(nBearingNumI - 1);
				aBearingJ.Add(i);
				aAnchorI.Add(0);
				aAnchorJ.Add(j);
			}
		}
	}
	else if(nDir == DIR_RIGHT)
	{
		for (int i = 0; i < nBearingNumI; i++)
		{
			for (int j = 0; j < nAnchorNumI; j++)
			{
				aBearingI.Add(i);
				aBearingJ.Add(nBearingNumJ - 1);
				aAnchorI.Add(j);
				aAnchorJ.Add(0);
			}
		}
	}
	*/
	else
	{
		ASSERT(0);
	}
}

void CDgnSeisAnchorShapeMgr::CalcAnchorProjPosConcFail(int nDir, int nBearingI, int nBearingJ, int nAnchorI, int nAnchorJ, double &dX1, double &dY1, double &dX2, double &dY2)
{
	if(m_aArSetCa1Longi.size() == 0)
	{
		AllocCa1Var();
	}

	double dAnchorX, dAnchorY;
	CalcAnchorPos(nBearingI, nBearingJ, nAnchorI, nAnchorJ, dAnchorX, dAnchorY);

	double dPos1[2], dPos2[2];
	if(nDir == DIR_LONGI)
	{
		if(m_aArSetCa1Longi[nBearingI][nBearingJ])
		{
			if (m_nDownUp == DIR_POS)
			{
				dPos1[0] = -m_aArCa1Longi[nBearingI][nBearingJ] * 1.5;
				dPos1[1] = -m_aArLastCa1Longi[nBearingI][nBearingJ];

				dPos2[0] = m_aArCa1Longi[nBearingI][nBearingJ] * 1.5;
				dPos2[1] = -m_aArLastCa1Longi[nBearingI][nBearingJ];
			}
			else if (m_nDownUp == DIR_NEG)
			{
				dPos1[0] = m_aArCa1Longi[nBearingI][nBearingJ] * 1.5;
				dPos1[1] = m_aArLastCa1Longi[nBearingI][nBearingJ];

				dPos2[0] = -m_aArCa1Longi[nBearingI][nBearingJ] * 1.5;
				dPos2[1] = m_aArLastCa1Longi[nBearingI][nBearingJ];
			}
		}
		else
		{	
			if (m_nDownUp == DIR_POS)
			{
				dPos1[0] = -1.5;
				dPos1[1] = -1;

				dPos2[0] = 1.5;
				dPos2[1] = -1;
			}
			else if (m_nDownUp == DIR_NEG)
			{
				dPos1[0] = 1.5;
				dPos1[1] = 1;

				dPos2[0] = -1.5;
				dPos2[1] = 1;
			}
		}
	}
	else if(nDir == DIR_TRANS)
	{
		if(m_aArSetCa1Trans[nBearingI][nBearingJ])
		{	
			if (m_nLeftRight == DIR_POS)
			{
				dPos1[0] = -m_aArLastCa1Trans[nBearingI][nBearingJ];
				dPos1[1] = -m_aArCa1Trans[nBearingI][nBearingJ] * 1.5;

				dPos2[0] = -m_aArLastCa1Trans[nBearingI][nBearingJ];
				dPos2[1] = m_aArCa1Trans[nBearingI][nBearingJ] * 1.5;
			}
			else if (m_nLeftRight == DIR_NEG)
			{
				dPos1[0] = m_aArLastCa1Trans[nBearingI][nBearingJ];
				dPos1[1] = m_aArCa1Trans[nBearingI][nBearingJ] * 1.5;

				dPos2[0] = m_aArLastCa1Trans[nBearingI][nBearingJ];
				dPos2[1] = -m_aArCa1Trans[nBearingI][nBearingJ] * 1.5;
			}
		}
		else
		{	
			if (m_nLeftRight == DIR_POS)
			{
				dPos1[0] = -1;
				dPos1[1] = -1.5;

				dPos2[0] = -1;
				dPos2[1] = 1.5;
			}
			else if (m_nLeftRight == DIR_NEG)
			{
				dPos1[0] = 1;
				dPos1[1] = 1.5;

				dPos2[0] = 1;
				dPos2[1] = -1.5;
			}
		}
	}
	/*
	else if(nDir == DIR_TOP)
	{
		dPos1[0] = -1.5;
		dPos1[1] = 1;

		dPos2[0] = 1.5;
		dPos2[1] = 1;
	}
	else if(nDir == DIR_RIGHT)
	{
		dPos1[0] = 1;
		dPos1[1] = -1.5;

		dPos2[0] = 1;
		dPos2[1] = 1.5;
	}
	*/
	else
	{
		ASSERT(0);
	}

	/////
	if (nDir == DIR_LONGI)
	{
		if (m_nDownUp == DIR_POS)
		{
			dPos1[0] = -fabs(dPos1[1])*(1.5);
			dPos2[0] = fabs(dPos2[1])*(1.5);
		}
		else
		{
			dPos1[0] = fabs(dPos1[1])*(1.5);
			dPos2[0] = -fabs(dPos2[1])*(1.5);
		}

	}
	else if (nDir == DIR_TRANS)
	{
		dPos1[1] = - fabs(dPos1[0])*(1.5);
		dPos2[1] = fabs(dPos2[0])*(1.5);
	}


	/////

	math_lib::rotate_2d(dPos1[0], dPos1[1], -m_AnevD.aLayer[m_nLIdx-nBearingI].dTheta_q2);	//aLayer에는 데이터가 반대로 입력되어있음..
	math_lib::rotate_2d(dPos2[0], dPos2[1], -m_AnevD.aLayer[m_nLIdx-nBearingI].dTheta_q2);	//aLayer에는 데이터가 반대로 입력되어있음..

	
	dPos1[0] += dAnchorX;
	dPos1[1] += dAnchorY;
	dPos2[0] += dAnchorX;
	dPos2[1] += dAnchorY;

	double dDir1[2], dDir2[2];
	GetDir(dAnchorX, dAnchorY, dPos1[0], dPos1[1], dDir1[0], dDir1[1]);
	GetDir(dAnchorX, dAnchorY, dPos2[0], dPos2[1], dDir2[0], dDir2[1]);

	double dLongDist = m_dCopyingHeight + m_dCopyingWidth;
	PntMove(dDir1[0], dDir1[1], dLongDist, dPos1[0], dPos1[1]);
	PntMove(dDir2[0], dDir2[1], dLongDist, dPos2[0], dPos2[1]);

	CArray<double, double> aCopyingX, aCopyingY;
	GetCopyingShape(aCopyingX, aCopyingY);
	ASSERT(aCopyingX.GetSize() == aCopyingY.GetSize());
	int nSize;
	nSize = aCopyingX.GetSize();

	dX1 = dPos1[0];
	dY1 = dPos1[1];

	dX2 = dPos2[0];
	dY2 = dPos2[1];

	BOOL bSet = FALSE;
	int nIdxNext;
	for (int i = 0; i < nSize; i++)
	{
		nIdxNext = i + 1;
		if(i == nSize - 1)
		{
			nIdxNext = 0;
		}
		if(GetLineInterSectPoint(dAnchorX, dAnchorY, dPos1[0], dPos1[1], aCopyingX[i], aCopyingY[i], aCopyingX[nIdxNext], aCopyingY[nIdxNext], dX1, dY1))
		{
			bSet = TRUE;
			break;
		}		
	}
	if(bSet == FALSE)
	{
		//ASSERT(0);
		return ; // 이상황은 안 생기게 미리 에러처리 해야 될 듯?
	}

	bSet = FALSE;
	for (int i = 0; i < nSize; i++)
	{
		nIdxNext = i + 1;
		if(i == nSize - 1)
		{
			nIdxNext = 0;
		}
		if(GetLineInterSectPoint(dAnchorX, dAnchorY, dPos2[0], dPos2[1], aCopyingX[i], aCopyingY[i], aCopyingX[nIdxNext], aCopyingY[nIdxNext], dX2, dY2))
		{
			bSet = TRUE;
			break;
		}		
	}
	if(bSet == FALSE)
	{
		//ASSERT(0);
		return ; // 이상황은 안 생기게 미리 에러처리 해야 될 듯?
	}

	BOOL bSwap = FALSE;
	if (nDir == DIR_LONGI)
	{
		if (m_nDownUp == DIR_POS)
		{
		}
		else if (m_nDownUp == DIR_NEG)
		{
		}
	}
	else if (nDir == DIR_TRANS)
	{
		if (m_nLeftRight == DIR_POS)
		{
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			bSwap = TRUE;
		}
	}
	if(bSwap)
	{ 
		double dX1_temp = dX2;
		double dY1_temp = dY2;

		dX2 = dX1;
		dY2 = dY1;

		dX1 = dX1_temp;
		dY1 = dY1_temp;
	}
}

void CDgnSeisAnchorShapeMgr::SetPolygonSmaller(T_COMPLEX_POLYGON2D& rPoly)
{
	T_POINT2D CenterPos;
	int nSize;
	nSize = rPoly.OuterPolygon.aVertex.GetSize();
	if(nSize == 0)
	{
		ASSERT(0);
		return ;
	}
	for (int i = 0; i < nSize ; i++)
	{
		CenterPos.x += rPoly.OuterPolygon.aVertex[i].x;
		CenterPos.y += rPoly.OuterPolygon.aVertex[i].y;
	}
	CenterPos.x /= nSize;
	CenterPos.y /= nSize;

	double dSmallVal = M_InitValueCurUnit(0.1, N, MM, D_UNITSYS_BASE_LENGTH);

	double Dir[2];
	nSize = rPoly.OuterPolygon.aVertex.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		GetDir(rPoly.OuterPolygon.aVertex[i].x, rPoly.OuterPolygon.aVertex[i].y, CenterPos.x, CenterPos.y, Dir[0], Dir[1]);
		PntMove(Dir[0], Dir[1], dSmallVal, rPoly.OuterPolygon.aVertex[i].x, rPoly.OuterPolygon.aVertex[i].y);
	}
}

BOOL CDgnSeisAnchorShapeMgr::CheckUnionPolygon(const T_COMPLEX_POLYGON2D& rPoly1, const T_COMPLEX_POLYGON2D& rPoly2)
{
	// [CIVIL-941] 검토시 딱 맞을 경우 단일 저항으로 판단하게 처리.. 단, 계산 값은 작아지면 안됨...
	// 알고리즘상 rPoly2 는 사각형이고 rPoly1은 다양한 형상이 들어 올 수 있으므로.. rPoly2의 크기를 줄여서 체크 함

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	ASSERT(rPoly2.OuterPolygon.aVertex.GetSize() == 5); // 여기서 ASSERT 가 걸리면 rPoly2 가 사각형이라는 가정이 잘못 된 것이므로.. 형상을 줄여서 체크하는 알고리즘의 수정이 필요 함
	T_COMPLEX_POLYGON2D rPoly2Clone;
	rPoly2Clone = rPoly2;
	SetPolygonSmaller(rPoly2Clone);

	// Union 연산은 느리므로 선에 점이 포함되는지로 먼저 판단 함(아닌 경우도 있지만.. 사용 하는 대부분의 case 가 여기 들어 올 것 같음)
	int nSize, nSizeVtx1;
	const T_POLYLINE2D& Poly1Vtx = rPoly1.OuterPolygon.aVertex;
	nSizeVtx1 = Poly1Vtx.GetSize();
	double (*PolyLine)[2];		
	double Poly2ClonePos[2];
	PolyLine = new double[nSizeVtx1 - 1][2];
	for (int i = 0; i < nSizeVtx1 - 1; i++)
	{
		PolyLine[i][0] = Poly1Vtx[i].x;
		PolyLine[i][1] = Poly1Vtx[i].y;
	}

	T_POLYLINE2D& Poly2CloneVtx = rPoly2Clone.OuterPolygon.aVertex;
	nSize = Poly2CloneVtx.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		Poly2ClonePos[0] = Poly2CloneVtx[i].x;
		Poly2ClonePos[1] = Poly2CloneVtx[i].y;

		if(CMathFunc::mathIsInsidePoint2D(Poly2ClonePos, nSizeVtx1 - 1, PolyLine, FALSE))
		{
			delete []PolyLine;
			return TRUE;
		}
	}

	CArray<T_COMPLEX_POLYGON2D, T_COMPLEX_POLYGON2D&> raResultPoly;	
	Union2Polygon(rPoly1, rPoly2Clone, raResultPoly, dZero);
	if(raResultPoly.GetSize() == 1)
	{	
		delete []PolyLine;
		return TRUE;
	}
	else
	{
		delete []PolyLine;
		return FALSE;
	}
}

void CDgnSeisAnchorShapeMgr::Union2Polygon(const T_COMPLEX_POLYGON2D& rPoly1, const T_COMPLEX_POLYGON2D& rPoly2, CArray<T_COMPLEX_POLYGON2D, T_COMPLEX_POLYGON2D&>& raResultPoly, double tolerance)
{
	// CGeomFunc::Union2Polygon() 연산이 느리기 때문에 Center 기준 길이가 짧으면 skip 함.. 정확하게 계산하면 느리니깐 대충 계산함
	T_POINT2D Center1, Center2;
	T_POINT2D BndBox1[2], BndBox2[2];
	double dCntToBox1, dCntToBox2, dCntToCnt;
	
	BndBox1[0].x = BndBox1[0].y = BndBox2[0].x = BndBox2[0].y = DBL_MAX;
	BndBox1[1].x = BndBox1[1].y = BndBox2[1].x = BndBox2[1].y = -DBL_MAX;

	int nSize;
	nSize = rPoly1.OuterPolygon.aVertex.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		if(BndBox1[0].x > rPoly1.OuterPolygon.aVertex[i].x)
		{
			BndBox1[0].x = rPoly1.OuterPolygon.aVertex[i].x;
		}

		if(BndBox1[0].y > rPoly1.OuterPolygon.aVertex[i].y)
		{
			BndBox1[0].y = rPoly1.OuterPolygon.aVertex[i].y;
		}
		
		if(BndBox1[1].x < rPoly1.OuterPolygon.aVertex[i].x)
		{
			BndBox1[1].x = rPoly1.OuterPolygon.aVertex[i].x;
		}

		if(BndBox1[1].y < rPoly1.OuterPolygon.aVertex[i].y)
		{
			BndBox1[1].y = rPoly1.OuterPolygon.aVertex[i].y;
		}		
	}

	nSize = rPoly2.OuterPolygon.aVertex.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		if(BndBox2[0].x > rPoly2.OuterPolygon.aVertex[i].x)
		{
			BndBox2[0].x = rPoly2.OuterPolygon.aVertex[i].x;
		}

		if(BndBox2[0].y > rPoly2.OuterPolygon.aVertex[i].y)
		{
			BndBox2[0].y = rPoly2.OuterPolygon.aVertex[i].y;
		}

		if(BndBox2[1].x < rPoly2.OuterPolygon.aVertex[i].x)
		{
			BndBox2[1].x = rPoly2.OuterPolygon.aVertex[i].x;
		}

		if(BndBox2[1].y < rPoly2.OuterPolygon.aVertex[i].y)
		{
			BndBox2[1].y = rPoly2.OuterPolygon.aVertex[i].y;
		}		
	}

	Center1.x = (BndBox1[0].x + BndBox1[1].x) * 0.5;
	Center1.y = (BndBox1[0].y + BndBox1[1].y) * 0.5;

	Center2.x = (BndBox2[0].x + BndBox2[1].x) * 0.5;
	Center2.y = (BndBox2[0].y + BndBox2[1].y) * 0.5;
	
	dCntToBox1 = math_lib::distance_2d(Center1.x, Center1.y, BndBox1[0].x, BndBox1[0].y);
	dCntToBox2 = math_lib::distance_2d(Center2.x, Center2.y, BndBox2[0].x, BndBox2[0].y);
	dCntToCnt = math_lib::distance_2d(Center1.x, Center1.y, Center2.x, Center2.y);

	if(dCntToCnt > dCntToBox2 + dCntToBox1)
	{
		return ; 
	}
	else
	{
		CGeomFunc::Union2Polygon(rPoly1, rPoly2, raResultPoly, tolerance); // CGeomFunc::Union2Polygon 에서 연산을 안해도 TRUE 를 return 하므로 return 값으로 분기 하는 코드를 넣으면 안됨
	}
}

void CDgnSeisAnchorShapeMgr::CopyToCArray(T_POLYGON2D& Poly, CArray<double,double>& aX, CArray<double,double>& aY)
{
	aX.RemoveAll();
	aY.RemoveAll();			
	int nSizeK = Poly.aVertex.GetSize();
	for (int k = 0; k < nSizeK ; k++)
	{
		if(k == nSizeK -1)
		{
			if(Poly.aVertex[k].x == Poly.aVertex[0].x &&
				Poly.aVertex[k].y == Poly.aVertex[0].y)
			{
				break;
			}
		}

		aX.Add(Poly.aVertex[k].x);
		aY.Add(Poly.aVertex[k].y);
	}
}

void CDgnSeisAnchorShapeMgr::CopyToComplexPoly(CArray<double,double>& aX, CArray<double,double>& aY, T_POLYGON2D& Poly)
{
	BOOL bStEndSame = FALSE;
	if(aX.GetSize() > 1)
	{
		if(aX[0] == aX[aX.GetSize() - 1] && aY[0] == aY[aY.GetSize() - 1])
		{
			bStEndSame = TRUE;
		}
	}

	if(bStEndSame)
	{
		Poly.aVertex.SetSize(aX.GetSize());
	}
	else
	{
		Poly.aVertex.SetSize(aX.GetSize() + 1);
	}
	
	int nSize = aX.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		Poly.aVertex[i].x = aX[i];
		Poly.aVertex[i].y = aY[i];
	}

	if(!bStEndSame)
	{
		Poly.aVertex[nSize].x = Poly.aVertex[0].x;
		Poly.aVertex[nSize].y = Poly.aVertex[0].y;
	}
}

void CDgnSeisAnchorShapeMgr::MakeCycle(T_COMPLEX_POLYGON2D& Poly)
{
	T_POINT2D PosTmp;
	if(Poly.OuterPolygon.aVertex[0].x != Poly.OuterPolygon.aVertex[Poly.OuterPolygon.aVertex.GetSize() - 1].x || 
		Poly.OuterPolygon.aVertex[0].y != Poly.OuterPolygon.aVertex[Poly.OuterPolygon.aVertex.GetSize() - 1].y)
	{
		PosTmp.x = Poly.OuterPolygon.aVertex[0].x;
		PosTmp.y = Poly.OuterPolygon.aVertex[0].y;
		Poly.OuterPolygon.aVertex.Add(PosTmp);
	}

	int nSize;
	nSize = Poly.aInnerPolygon.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		if(Poly.aInnerPolygon[i].aVertex[0].x != Poly.aInnerPolygon[i].aVertex[Poly.aInnerPolygon[i].aVertex.GetSize() - 1].x || 
			Poly.aInnerPolygon[i].aVertex[0].y != Poly.aInnerPolygon[i].aVertex[Poly.aInnerPolygon[i].aVertex.GetSize() - 1].y)
		{
			PosTmp.x = Poly.aInnerPolygon[i].aVertex[0].x;
			PosTmp.y = Poly.aInnerPolygon[i].aVertex[0].y;
			Poly.aInnerPolygon[i].aVertex.Add(PosTmp);
		}
	}
}

void CDgnSeisAnchorShapeMgr::SetShorter(double& dX1 , double& dY1, double& dX2, double& dY2, double dVal)
{
	double Dir[2];
	GetDir(dX1, dY1, dX2, dY2, Dir[0], Dir[1]);
	PntMove(Dir[0], Dir[1], dVal, dX1, dY1);
	PntMove(Dir[0], Dir[1], -dVal, dX2, dY2);
}

void CDgnSeisAnchorShapeMgr::GetPryoutBcNoUnion(int nDir, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeInX, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeInY, 
												CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeOutX, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeOutY, double dSetShorter)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	int nSize, nSizeJ;

	aArProjShapeInX.RemoveAll();
	aArProjShapeInY.RemoveAll();
	aArProjShapeOutX.RemoveAll();
	aArProjShapeOutY.RemoveAll();
	
	CArray<int, int> aBearingI, aBearingJ, aAnchorI, aAnchorJ;
	GetPrjTgtIdxPryout(nDir, aBearingI, aBearingJ, aAnchorI, aAnchorJ);
	if(aBearingI.GetSize() < 1 || aBearingJ.GetSize() < 1 || aAnchorI.GetSize() < 1 || aAnchorJ.GetSize() < 1)
	{
		return;
	}
	ASSERT(aBearingI.GetSize() == aBearingJ.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorI.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorJ.GetSize());

	CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&> aArXTmp, aArYTmp;
	T_CARRAY<double, double> aShapeX, aShapeY;

	// Step1. 각 Anchor 에서 project 된 영역을 계산 함
	nSize = aBearingI.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		if(i != 0 && aBearingI[i - 1] == aBearingI[i] && aBearingJ[i - 1] == aBearingJ[i])
			continue;

		CalcAnchorProjPosPryoutEachBearing(nDir, aBearingI[i], aBearingJ[i], TRUE, aArXTmp, aArYTmp); // 이 함수를 통과 하면 [0],[2] 와 [1],[3] 으로 2개의 선을 만들어서 x 자를 그려야 함..
		
		DivByOutline(aArXTmp, aArYTmp); // 이 함수를 통과 하면 [0],[1] 의 끊어진 선이 됨..
		
		nSizeJ = aArXTmp.GetSize();
		for (int j = 0; j < nSizeJ ; j++)
		{
			if(aArXTmp[j].aData.GetSize() != 2)
			{
				ASSERT(0);
				return ;
			}
			// 선이 한픽셀씩 긴것들이 생겨서 눈에 너무 튐.. 그래서 차라리 짧은건 있어도 긴건 없도록 처리		
			SetShorter(aArXTmp[j].aData[0], aArYTmp[j].aData[0], aArXTmp[j].aData[1], aArYTmp[j].aData[1], dSetShorter);
		}
		
		nSizeJ = aArXTmp.GetSize();
		for (int j = 0; j < nSizeJ ; j++)
		{
			if(InnerBearing(aArXTmp[j], aArYTmp[j]))
			{
				aArProjShapeInX.Add(aArXTmp[j]);
				aArProjShapeInY.Add(aArYTmp[j]);	
			}
			else if(InnerCopping(aArXTmp[j], aArYTmp[j]))
			{
				aArProjShapeOutX.Add(aArXTmp[j]);
				aArProjShapeOutY.Add(aArYTmp[j]);	
			}
		}
	}
}

BOOL CDgnSeisAnchorShapeMgr::InnerBearing(T_CARRAY<double,double>& aX, T_CARRAY<double,double>& aY)
{
	if(aX.aData.GetSize() != 2)
	{
		ASSERT(0);
		return FALSE;
	}
	ASSERT(aX.aData.GetSize() == aY.aData.GetSize());

	double CenterPos[2];
	CenterPos[0] = (aX.aData[0] + aX.aData[1]) * 0.5;
	CenterPos[1] = (aY.aData[0] + aY.aData[1]) * 0.5;
	
	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	CArray<double, double> aBearingX, aBearingY;

	 double polyLine[4][2];

	for (int nIdxBearingI = 0; nIdxBearingI < nBearingNumI; nIdxBearingI++)
	{
		for (int nIdxBearingJ = 0; nIdxBearingJ < aBearingNumJ[nIdxBearingI]; nIdxBearingJ++)
		{
			GetBearingShape(nIdxBearingI, nIdxBearingJ, aBearingX, aBearingY);
			ASSERT(aBearingX.GetSize() == aBearingY.GetSize());

			if(aBearingX.GetSize() != 4)
			{
				ASSERT(0);
				continue;
			}

			for (int i = 0; i < 4; i++)
			{
				polyLine[i][0] = aBearingX[i];
				polyLine[i][1] = aBearingY[i];
			}

			if(CMathFunc::mathIsInsidePoint2D(CenterPos, 4, polyLine, FALSE))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CDgnSeisAnchorShapeMgr::InnerCopping(T_CARRAY<double,double>& aX, T_CARRAY<double,double>& aY)
{
	if(aX.aData.GetSize() != 2)
	{
		ASSERT(0);
		return FALSE;
	}
	ASSERT(aX.aData.GetSize() == aY.aData.GetSize());

	double CenterPos[2];
	CenterPos[0] = (aX.aData[0] + aX.aData[1]) * 0.5;
	CenterPos[1] = (aY.aData[0] + aY.aData[1]) * 0.5;

	CArray<double, double> aCopyingX, aCopyingY;
	GetCopyingShape(aCopyingX, aCopyingY);

	double polyLine[4][2];

	if(aCopyingX.GetSize() != 4)
	{
		ASSERT(0);
		return FALSE;
	}

	for (int i = 0; i < 4; i++)
	{
		polyLine[i][0] = aCopyingX[i];
		polyLine[i][1] = aCopyingY[i];
	}

	if(CMathFunc::mathIsInsidePoint2D(CenterPos, 4, polyLine, FALSE))
	{
		return TRUE;
	}

	return FALSE;
}

void CDgnSeisAnchorShapeMgr::DivByOutline(CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArX, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArY)
{
	double dZero = 0.001;
	
	CArray<double, double> aCopyingX, aCopyingY;
	GetCopyingShape(aCopyingX, aCopyingY);

	CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&> aArXTmp, aArYTmp;
	T_CARRAY<double,double> EachX, EachY;
	EachX.aData.SetSize(2);
	EachY.aData.SetSize(2);
	for (int i = 0; i < aArX.GetSize(); i++)
	{
		EachX.aData[0] = aArX[i].aData[0];
		EachY.aData[0] = aArY[i].aData[0];
		EachX.aData[1] = aArX[i].aData[2];
		EachY.aData[1] = aArY[i].aData[2];
		aArXTmp.Add(EachX);
		aArYTmp.Add(EachY);

		EachX.aData[0] = aArX[i].aData[1];
		EachY.aData[0] = aArY[i].aData[1];
		EachX.aData[1] = aArX[i].aData[3];
		EachY.aData[1] = aArY[i].aData[3];
		aArXTmp.Add(EachX);
		aArYTmp.Add(EachY);
	}
	
	double Pos1[2], Pos2[2], Pos3[2], Pos4[2], dRstX, dRstY;
	
	for (int i = 0; i < aArXTmp.GetSize(); i++)
	{
		Pos1[0] = aArXTmp[i].aData[0];
		Pos1[1] = aArYTmp[i].aData[0];

		Pos2[0] = aArXTmp[i].aData[1];
		Pos2[1] = aArYTmp[i].aData[1];
		
		for (int j = 0; j < aCopyingX.GetSize(); j++)
		{
			Pos3[0] = aCopyingX[j];
			Pos3[1] = aCopyingY[j];
			if(j == aCopyingX.GetSize() - 1)
			{
				Pos4[0] = aCopyingX[0];
				Pos4[1] = aCopyingY[0];
			}
			else
			{
				Pos4[0] = aCopyingX[j + 1];
				Pos4[1] = aCopyingY[j + 1];
			}

			if(CMathFunc::mathIntersectLine2D(Pos1[0], Pos1[1], Pos2[0], Pos2[1], Pos3[0], Pos3[1], Pos4[0], Pos4[1], dRstX, dRstY, TRUE))
			{
				if(math_lib::distance_2d(Pos1[0], Pos1[1], dRstX, dRstY) < dZero)
				{
					continue;
				}
				else if(math_lib::distance_2d(Pos2[0], Pos2[1], dRstX, dRstY) < dZero)
				{
					continue;
				}

				EachX.aData[0] = dRstX;
				EachY.aData[0] = dRstY;
				EachX.aData[1] = aArXTmp[i].aData[1];
				EachY.aData[1] = aArYTmp[i].aData[1];

				aArXTmp[i].aData[1] = dRstX;
				aArYTmp[i].aData[1] = dRstY;
				
				aArXTmp.Add(EachX);
				aArYTmp.Add(EachY);

				i--;
				break;;
			}
		}	
	}

	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	CArray<double, double> aBearingX, aBearingY;
		
	BOOL bContinue = FALSE;

	for (int i = 0; i < aArXTmp.GetSize(); i++)
	{
		Pos1[0] = aArXTmp[i].aData[0];
		Pos1[1] = aArYTmp[i].aData[0];

		Pos2[0] = aArXTmp[i].aData[1];
		Pos2[1] = aArYTmp[i].aData[1];

		bContinue = FALSE;

		for (int nIdxBearingI = 0; nIdxBearingI < nBearingNumI; nIdxBearingI++)
		{
			for (int nIdxBearingJ = 0; nIdxBearingJ < aBearingNumJ[nIdxBearingI]; nIdxBearingJ++)
			{
				GetBearingShape(nIdxBearingI, nIdxBearingJ, aBearingX, aBearingY);
				ASSERT(aBearingX.GetSize() == aBearingY.GetSize());

				for (int j = 0; j < aBearingX.GetSize(); j++)
				{
					Pos3[0] = aBearingX[j];
					Pos3[1] = aBearingY[j];
					if(j == aBearingX.GetSize() - 1)
					{
						Pos4[0] = aBearingX[0];
						Pos4[1] = aBearingY[0];
					}
					else
					{
						Pos4[0] = aBearingX[j + 1];
						Pos4[1] = aBearingY[j + 1];
					}

					if(CMathFunc::mathIntersectLine2D(Pos1[0], Pos1[1], Pos2[0], Pos2[1], Pos3[0], Pos3[1], Pos4[0], Pos4[1], dRstX, dRstY, TRUE))
					{
						if(math_lib::distance_2d(Pos1[0], Pos1[1], dRstX, dRstY) < dZero)
						{
							continue;
						}
						else if(math_lib::distance_2d(Pos2[0], Pos2[1], dRstX, dRstY) < dZero)
						{
							continue;
						}

						EachX.aData[0] = dRstX;
						EachY.aData[0] = dRstY;
						EachX.aData[1] = aArXTmp[i].aData[1];
						EachY.aData[1] = aArYTmp[i].aData[1];

						aArXTmp[i].aData[1] = dRstX;
						aArYTmp[i].aData[1] = dRstY;

						aArXTmp.Add(EachX);
						aArYTmp.Add(EachY);

						i--;						
						bContinue = TRUE;
						break;
					}
				}	
				if(bContinue) break;
			}
			if(bContinue) break;
		}	
	}
	
	aArX.RemoveAll();
	aArY.RemoveAll();	
	int nSize = aArXTmp.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		aArX.Add(aArXTmp[i]);
	}
	nSize = aArYTmp.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		aArY.Add(aArYTmp[i]);
	}
}

void CDgnSeisAnchorShapeMgr::GetPryoutBasicSaxy(int nDir, int nBearingIdxI, int nBearingIdxJ, double& dSax, double& dSay)
{
	int BearingIdx[2][2];

	int nBearingNumI, nBearingNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[nBearingIdxI];

	if(nBearingNumJ == 1)
	{
		dSax = 0.0;
	}
	else if(nBearingIdxJ == 0)
	{
		BearingIdx[0][0] = nBearingIdxI;
		BearingIdx[0][1] = nBearingIdxJ;
		BearingIdx[1][0] = nBearingIdxI;
		BearingIdx[1][1] = nBearingIdxJ + 1;
		dSax = GetBearingGap(0, BearingIdx);
	}
	else if(nBearingIdxJ == nBearingNumJ - 1)
	{
		BearingIdx[0][0] = nBearingIdxI;
		BearingIdx[0][1] = nBearingIdxJ - 1;
		BearingIdx[1][0] = nBearingIdxI;
		BearingIdx[1][1] = nBearingIdxJ;
		dSax = GetBearingGap(0, BearingIdx);
	}
	else
	{
		BearingIdx[0][0] = nBearingIdxI;
		BearingIdx[0][1] = nBearingIdxJ;
		BearingIdx[1][0] = nBearingIdxI;
		BearingIdx[1][1] = nBearingIdxJ + 1;
		dSax = GetBearingGap(0, BearingIdx);

		BearingIdx[0][0] = nBearingIdxI;
		BearingIdx[0][1] = nBearingIdxJ - 1;
		BearingIdx[1][0] = nBearingIdxI;
		BearingIdx[1][1] = nBearingIdxJ;
		dSax = min(dSax, GetBearingGap(0, BearingIdx));
	}

	if(nBearingNumI == 1)
	{
		dSay = 0.0;
	}
	else if(nBearingIdxI == 0)
	{
		BearingIdx[0][0] = nBearingIdxI;
		BearingIdx[0][1] = nBearingIdxJ;
		BearingIdx[1][0] = nBearingIdxI + 1;
		BearingIdx[1][1] = nBearingIdxJ;
		dSay = GetBearingGap(1, BearingIdx);
	}
	else if(nBearingIdxI == nBearingNumI - 1)
	{
		BearingIdx[0][0] = nBearingIdxI - 1;
		BearingIdx[0][1] = nBearingIdxJ;
		BearingIdx[1][0] = nBearingIdxI;
		BearingIdx[1][1] = nBearingIdxJ;
		dSay = GetBearingGap(1, BearingIdx);
	}
	else
	{
		BearingIdx[0][0] = nBearingIdxI;
		BearingIdx[0][1] = nBearingIdxJ;
		BearingIdx[1][0] = nBearingIdxI + 1;
		BearingIdx[1][1] = nBearingIdxJ;
		dSay = GetBearingGap(1, BearingIdx);

		BearingIdx[0][0] = nBearingIdxI - 1;
		BearingIdx[0][1] = nBearingIdxJ;
		BearingIdx[1][0] = nBearingIdxI;
		BearingIdx[1][1] = nBearingIdxJ;
		dSay = min(dSay, GetBearingGap(1, BearingIdx));
	}

	if(nDir == DIR_TRANS)
	{
		double dTmp;
		dTmp = dSax;
		dSax = dSay;
		dSay = dTmp;
	}
}

BOOL CDgnSeisAnchorShapeMgr::NextBearingIdx(int nDir, int nNextI, int nNextJ, int& nBearingIdxI, int& nBearingIdxJ)
{
	// nNextI, nNextJ : 0=none, 그외는 -1과 1만 가능
	if(nNextI != 0 && nNextJ != 0 )
	{
		ASSERT(0); // 이 상황은 코딩 안했음
		return FALSE;
	}

	int nBearingNumI, nBearingNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 게산이 가능하도록 수정 필요	
	
	int nFailType = 1;

	if(nNextI == 1)
	{
		for (int i = nBearingIdxI + 1; i < nBearingNumI; i++)
		{
			if(IsFixedBearing(i, nBearingIdxJ, nDir) && IsSelectedBearing(i, nBearingIdxJ, nDir, nFailType))
			{
				nBearingIdxI = i;
				return TRUE;
			}
		}
		return FALSE;
	}
	else if(nNextI == -1)
	{
		for (int i = nBearingIdxI - 1; i >= 0 ; i--)
		{
			if(IsFixedBearing(i, nBearingIdxJ, nDir) && IsSelectedBearing(i, nBearingIdxJ, nDir, nFailType))
			{
				nBearingIdxI = i;
				return TRUE;
			}
		}
		return FALSE;
	}
	else
	{
		ASSERT(nNextI == 0);
	}

	if(nNextJ == 1)
	{
		for (int j = nBearingIdxJ + 1; j < nBearingNumJ; j++)
		{
			if(IsFixedBearing(nBearingIdxI, j, nDir) && IsSelectedBearing(nBearingIdxI, j, nDir, nFailType))
			{
				nBearingIdxJ = j;
				return TRUE;
			}
		}
		return FALSE;
	}
	else if(nNextJ == -1)
	{
		for (int j = nBearingIdxJ - 1; j >= 0 ; j--)
		{
			if(IsFixedBearing(nBearingIdxI, j, nDir) && IsSelectedBearing(nBearingIdxI, j, nDir, nFailType))
			{
				nBearingIdxJ = j;
				return TRUE;
			}
		}
		return FALSE;
	}
	else
	{
		ASSERT(nNextI == 0);
	}
	ASSERT(0);
	return FALSE;	
}

void CDgnSeisAnchorShapeMgr::GetConcfailRepresentGrpBearing(int nDir, std::vector<std::pair<int, int>>& aRepBearingIdx)
{
	TConcFailBcCalc MinConcFailBc;
	GetConcFailBc(nDir, MinConcFailBc);
	aRepBearingIdx = MinConcFailBc.aBearingIdx;
}

void CDgnSeisAnchorShapeMgr::GetPryoutRepresentGrpBearing(int nDir, std::vector<std::pair<int, int>>& aRepBearingIdx)
{
	aRepBearingIdx.clear();
	if(m_nLastPryoutGroupDir != nDir)
	{
		int nBc;
		double dSax, dSay, dArea;
		BOOL bSingleResist;
		std::set<int> setBcIdx;
		CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&> aArProjShapeX, aArProjShapeY;
		std::vector<std::pair<int, int>> aRepBearingIdxTemp;
		GetPryoutBc(nDir, aRepBearingIdxTemp, aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bSingleResist);
	}

	if(m_nPryoutRepresentGrpIdx == -1)
	{
		//ASSERT(0);// GetPryoutBc() 에서 세팅해야 하는 상황인데.. 뭔가 이상함..
		return;
	}

	aRepBearingIdx = m_aPryoutGroupInfo[m_nPryoutRepresentGrpIdx].aBearingIdx;
}

void CDgnSeisAnchorShapeMgr::GetPryoutBc(int nDir, std::vector<std::pair<int, int>> aRepBearingIdx4Ratio, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeX, CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeY, std::set<int>& setBcIdx, int& nBc, double& dArea, double& dSax, double& dSay, BOOL& bSingleResist)
{
	//////////////////////////////////////////////////////////////////////////
	int nPryoutGrpIdx = (nDir == 0) ? m_nPryoutRepresentGrpIdx_X : m_nPryoutRepresentGrpIdx_Y;
	if (!m_bRatioChk && nPryoutGrpIdx > -1)
	{
		double dRatio = 0.0;
		KeyForPryoutBCPos Key(nDir, nPryoutGrpIdx, m_nDownUpPryMgr, m_nLeftRightPryMgr);
		auto itrFind = m_mapPryoutBCData.find(Key);
		if (itrFind != m_mapPryoutBCData.end())
		{ // 있으면
			ValForPryoutBC& Val = itrFind->second;

			bSingleResist = FALSE;
			dArea = 0.0;
			dSax = 0.0;
			dSay = 0.0;
			nBc = 1;

			aArProjShapeX.RemoveAll();
			aArProjShapeY.RemoveAll();
			setBcIdx.clear();

			aArProjShapeX.Copy(Val.aArProjShapeX);
			aArProjShapeY.Copy(Val.aArProjShapeY);
			for (double EachVal : Val.setBcIdx)
			{
				setBcIdx.insert(EachVal);
			}
			nBc = Val.nBc;
			dArea = Val.dArea;
			dSax = Val.dSax;
			dSay = Val.dSay;
			bSingleResist = Val.bSingleResist;

			m_nPryoutRepresentGrpIdx = (nDir == 0) ? m_nPryoutRepresentGrpIdx_X : m_nPryoutRepresentGrpIdx_Y;

			m_aPryoutGroupInfo.clear();

			if (nDir == 0)
			{
				for (int nIndx = 0; nIndx < m_aPryoutGroupInfo_X.size(); nIndx++)
				{
					m_aPryoutGroupInfo.push_back(m_aPryoutGroupInfo_X[nIndx]);
				}
			}
			else
			{
				for (int nIndx = 0; nIndx < m_aPryoutGroupInfo_Y.size(); nIndx++)
				{
					m_aPryoutGroupInfo.push_back(m_aPryoutGroupInfo_Y[nIndx]);
				}
			}

			m_nLastPryoutGroupDir = nDir;

			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	bSingleResist = FALSE;

	// CGeomFunc::Union2Polygon 연산이 매우 느리기 때문에 속도개선을 위해 수를 썼음..
	dSax = 0.0;
	dSay = 0.0;		

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	dArea = 0.0;

	int nSize, nSizeJ;

	aArProjShapeX.RemoveAll();
	aArProjShapeY.RemoveAll();
	nBc = 1;
	setBcIdx.clear();

	CArray<int, int> aBearingI, aBearingJ, aAnchorI, aAnchorJ;
	GetPrjTgtIdxPryout(nDir, aBearingI, aBearingJ, aAnchorI, aAnchorJ);
	if(aBearingI.GetSize() < 1 || aBearingJ.GetSize() < 1 || aAnchorI.GetSize() < 1 || aAnchorJ.GetSize() < 1)
	{
		m_nLastPryoutGroupDir = nDir;
		return;
	}
	ASSERT(aBearingI.GetSize() == aBearingJ.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorI.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorJ.GetSize());
	
	CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&> aArEachProjShapeX, aArEachProjShapeY, aArXTmp, aArYTmp;
	T_CARRAY<double, double> aShapeX, aShapeY;

	// Step1. 각 Anchor 에서 project 된 영역을 계산 함	
	int nLastI, nLastJ;
	int nFailType = 1;
	for (int i = 0; i < aBearingI.GetSize() ; i++)
	{
		/* 아래 조건은 고정단 가동단 개발피드백 과정에서 빠졌음.. 그 대신 Pryout 파괴 영역 계산은 Ca2 계산시 고려했던 대표 행의 bearing 을 1개 이상 포함 해야 한다는 조건이 들어 갔음
		if(i != 0 && aBearingI[i - 1] == aBearingI[i] && aBearingJ[i - 1] == aBearingJ[i])
			continue;
			*/
		if(!IsFixedBearing(aBearingI[i], aBearingJ[i], nDir) || !IsSelectedBearing(aBearingI[i], aBearingJ[i], nDir, nFailType))
			continue;
		
		CalcAnchorProjPosPryoutEachBearing(nDir, aBearingI[i], aBearingJ[i], FALSE, aArXTmp, aArYTmp);

		if(i != 0 && nLastI == aBearingI[i] && nLastJ == aBearingJ[i])
		{
			nLastI = aBearingI[i];
			nLastJ = aBearingJ[i];
			continue;
		}
		for (int j = 0; j < aArXTmp.GetSize() ; j++)
		{
			aArEachProjShapeX.Add(aArXTmp[j]);
			aArEachProjShapeY.Add(aArYTmp[j]);	
		}

		nLastI = aBearingI[i];
		nLastJ = aBearingJ[i];
	}

	if(aArEachProjShapeX.GetSize() == 0)
	{
		m_nLastPryoutGroupDir = nDir;
		return;
	}

	T_COMPLEX_POLYGON2D Poly;
	double dDist;
	/*
	// Bearing 밖으로 영역이 나가지 않으면 union을 할 필요가 없음.. 그냥 복사하고 끝 냄 >> Bearing 을 다르게 입력하게 된 이후로 이 알고리즘 못씀
	double dMin = min(m_aArBrprD.dSy, m_aArBrprD.dSx);	
	double dDist = Gethef(nDir) * 1.5;
	if(dMin + dZero > dDist)
	{
		aArProjShapeX.Copy(aArEachProjShapeX);
		aArProjShapeY.Copy(aArEachProjShapeY);

		Poly.OuterPolygon.Init();
		Poly.aInnerPolygon.RemoveAll();
		CopyToComplexPoly(aArProjShapeX[0].aData, aArProjShapeY[0].aData, Poly.OuterPolygon);
		MakeCycle(Poly);
		dArea = CGeomFunc::CalcPolyArea(Poly);

		nLastPryoutGroupDir = nDir;
		return;
	}
	*/
	
	T_COMPLEX_POLYGON2D PolyTmp;
	CArray<T_COMPLEX_POLYGON2D, T_COMPLEX_POLYGON2D&> aResultPoly;
	CArray<T_COMPLEX_POLYGON2D, T_COMPLEX_POLYGON2D&> aSrcPoly;
	
	nSize = aArEachProjShapeX.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		CopyToComplexPoly(aArEachProjShapeX[i].aData, aArEachProjShapeY[i].aData, PolyTmp.OuterPolygon);
		aSrcPoly.Add(PolyTmp);
	}
		
	nBc = 1;
	// Step2. 영역들을 union 함
	std::set<int> setDelIdx, setBcExceptIdx;
	std::vector<int> aBcOld, aBc;

	aBcOld.resize(aSrcPoly.GetSize());	
	nSize = (int)aBcOld.size();
	for (int i = 0; i < nSize ; i++)
	{
		aBcOld[i] = 1;
	}
	BOOL bRepeat = FALSE;
	for (int i = 0; i < aSrcPoly.GetSize() - 1; i++)
	{
		if(setDelIdx.find(i) != setDelIdx.end())
			continue;
		bRepeat = FALSE;

		for (int j = i + 1; j < aSrcPoly.GetSize(); j++)
		{
			if(setDelIdx.find(j) != setDelIdx.end())
				continue;

			aResultPoly.RemoveAll();			
			if(CheckUnionPolygon(aSrcPoly[i], aSrcPoly[j]))
			{
				Union2Polygon(aSrcPoly[i], aSrcPoly[j], aResultPoly, dZero);
				if(aResultPoly.GetSize() == 1)
				{	
					aBcOld[i]++;

					if(aResultPoly[0].OuterPolygon.aVertex.GetSize() == 0)
					{
						ASSERT(0);
						m_nLastPryoutGroupDir = nDir;
						return;
					}

					aSrcPoly[i] = aResultPoly[0];
					MakeCycle(aSrcPoly[i]);				
					setDelIdx.insert(j);

					bRepeat = TRUE;
					break;
				}
				else
				{
					//ASSERT(0); // 이경우는 CheckUnionPolygon() 에서 return FALSE 되어야 정상 임
				}
			}
		}

		if(bRepeat)
		{
			i--;
		}
	}

	// Step3. Coping 밖으로 나가는 부분을 잘라냄
	CArray<double, double> aCopyingX, aCopyingY;
	GetCopyingShape(aCopyingX, aCopyingY);	
	Poly.OuterPolygon.Init();
	Poly.aInnerPolygon.RemoveAll();
	CopyToComplexPoly(aCopyingX, aCopyingY, Poly.OuterPolygon);
	MakeCycle(Poly);

	CArray<int, int> aRemoveIdx;

	nSize = aSrcPoly.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		if(setDelIdx.find(i) != setDelIdx.end())
			continue;

		aResultPoly.RemoveAll();
		CGeomFunc::Intersect2Polygon(Poly, aSrcPoly[i], aResultPoly);
		if(aResultPoly.GetSize() == 1)
		{
			aSrcPoly[i] = aResultPoly[0];
		}
		else
		{
			// 베어링 끝부분이 코핑부의 경계에 맞닿은 경우 Pryout 형상오류 발생
			// 코핑부의 형상을 미세하게 조정.
			for (int nN = 0; nN < 10; nN++)
			{
				int nVertex = Poly.OuterPolygon.aVertex.GetSize();
				double dfactor = 0.999 - 0.001*nN;
				for (int j = 0; j < nVertex; j++)
				{
					Poly.OuterPolygon.aVertex[j].x *= dfactor;
					Poly.OuterPolygon.aVertex[j].y *= dfactor;
				}

				aResultPoly.RemoveAll();
				CGeomFunc::Intersect2Polygon(Poly, aSrcPoly[i], aResultPoly);
				if (aResultPoly.GetSize() == 1)
				{
					aSrcPoly[i] = aResultPoly[0];
					break;
				}
			}
		}
	}

	std::vector<double> aArea;

	nSize = aSrcPoly.GetSize();
	ASSERT(nSize == aBcOld.size());
	for (int i = 0; i < nSize ; i++)
	{
		if(setDelIdx.find(i) != setDelIdx.end())
			continue;
		
		aBc.push_back(aBcOld[i]);
		MakeCycle(aSrcPoly[i]);
		aArea.push_back(CGeomFunc::CalcPolyArea(aSrcPoly[i]));
	}

	// Step4. 대표 그룹 정보를 채우고, 대표 그룹을 정한다.
	double dAnchorX, dAnchorY;

	int nBearingNumI, nBearingNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 게산이 가능하도록 수정 필요

	int nIdx = -1;

	TPryoutGroupInfo EachGroupInfo;
	m_aPryoutGroupInfo.clear();
	for (int i = 0; i < aSrcPoly.GetSize(); i++)
	{
		if(setDelIdx.find(i) != setDelIdx.end())
			continue;

		nIdx++;

		EachGroupInfo.Init();
		BOOL bCheck01 = FALSE;
		BOOL bCheck02 = FALSE;
		if (nDir == DIR_LONGI)
		{
			if     (m_nDownUpPryMgr == DIR_POS) bCheck01 = TRUE;
			else if(m_nDownUpPryMgr == DIR_NEG) bCheck02 = TRUE;

		}
		else if (nDir == DIR_TRANS)
		{
			if(m_nLeftRightPryMgr == DIR_POS) bCheck01 = TRUE;
			if(m_nLeftRightPryMgr == DIR_NEG) bCheck02 = TRUE;
		}

		if(bCheck01) //if (m_nDownUpPryMgr == DIR_POS && m_nLeftRightPryMgr == DIR_POS)
		{
			for (int nBIdxI = 0; nBIdxI < nBearingNumI; nBIdxI++)
			{
				for (int nBIdxJ = 0; nBIdxJ < nBearingNumJ; nBIdxJ++)
				{
					CalcAnchorPos(nBIdxI, nBIdxJ, 0, 0, dAnchorX, dAnchorY);  // 한 Bearing 에서 각 Anchor 간격보다 hef 가 작은 경우에는 Bearing 내에서 0,0 지점의 Anchor 에 대해서만 정보를 넣어주면 됨.. 어차피 나머지 Anchor 도 값이 동일하니깐..
					if (IsInnerPolygon(dAnchorX, dAnchorY, aSrcPoly[i]))
					{
						EachGroupInfo.aBearingIdx.push_back(std::pair<int, int>(nBIdxI, nBIdxJ));
					}
				}
			}
		}
		else if(bCheck02) //else if (m_nDownUpPryMgr == DIR_NEG && m_nLeftRightPryMgr == DIR_NEG)
		{
			for (int nBIdxI = nBearingNumI-1; nBIdxI >= 0; nBIdxI--)
			{
				for (int nBIdxJ = nBearingNumJ-1; nBIdxJ >=0; nBIdxJ--)
				{	
					int nAnchorNumI, nAnchorNumJ;
					GetAnchorNumber(nBIdxI, nBIdxJ, nAnchorNumI, nAnchorNumJ);
					CalcAnchorPos(nBIdxI, nBIdxJ, nAnchorNumI-1, nAnchorNumJ-1, dAnchorX, dAnchorY);  // 한 Bearing 에서 각 Anchor 간격보다 hef 가 작은 경우에는 Bearing 내에서 0,0 지점의 Anchor 에 대해서만 정보를 넣어주면 됨.. 어차피 나머지 Anchor 도 값이 동일하니깐..
					if (IsInnerPolygon(dAnchorX, dAnchorY, aSrcPoly[i]))
					{
						EachGroupInfo.aBearingIdx.push_back(std::pair<int, int>(nBIdxI, nBIdxJ));
					}
				}
			}
		}
		else 
			ASSERT(0);
		
		if(EachGroupInfo.aBearingIdx.size() == 0)
		{
			continue; // 한 Bearing 에서 각 Anchor 간격보다 hef 가 작은 경우 [0][0] 위치의 Anchor 를 포함하는 Polygong 이 아니면 skip 한다.
		}
		EachGroupInfo.dArea = aArea[nIdx];
		EachGroupInfo.nBc = aBc[nIdx];
		m_aPryoutGroupInfo.push_back(EachGroupInfo);
	}

	//////////////////////////////////////////////////////////////////////////
	CArray<double, double> aEachRatio;
	aEachRatio.RemoveAll();
	if(!m_bRatioChk)
	{
		double dEachRatio = 0.0;
		double dMinRatio = dMinRatio = DBL_MAX;
		double dMaxRatio = dMaxRatio = -DBL_MAX;

		int nPryoutRepresentGrpIdxTemp = -1;
		for (int i = 0; i < m_aPryoutGroupInfo.size(); i++)
		{
			dEachRatio = CalcPryoutBearingRepresentRatio(nDir, i);

			aEachRatio.Add(dEachRatio);

			if (dEachRatio < dMinRatio)
			{
				dMinRatio = dEachRatio;
				nPryoutRepresentGrpIdxTemp = i;
			}
		}

		m_nPryoutRepresentGrpIdx = nPryoutRepresentGrpIdxTemp;

		if (nDir == 0) m_nPryoutRepresentGrpIdx_X = m_nPryoutRepresentGrpIdx;
		else           m_nPryoutRepresentGrpIdx_Y = m_nPryoutRepresentGrpIdx;
	}
	//////////////////////////////////////////////////////////////////////////

	int nRepSrcIdx = -1;
	nIdx = -1;
	for (int i = 0; i < aSrcPoly.GetSize(); i++)
	{
		if(setDelIdx.find(i) != setDelIdx.end())
			continue;

		nIdx++;
		if(nIdx == m_nPryoutRepresentGrpIdx)
		{
			nRepSrcIdx = i;
		}
	}
	if(nRepSrcIdx == -1)
	{
		ASSERT(0); // setDelIdx 은 지워진 녀석을 세팅한 정보인데.. 지워진 aSrcPoly 가 m_nPryoutRepresentGrpIdx 로 세팅될 순 없으니 발생하면 안되는 상황임
		return;
	}

	// Step5. 대표 그룹 기준으로 return 값을 채운다... sax, say 은 설계에서 쓴다
	// nBc가 2이상이면(그룹 저항이면) 그룹 내에서 sax 와 say 를 계산함
	
	nBc = m_aPryoutGroupInfo[m_nPryoutRepresentGrpIdx].nBc;
	dArea = m_aPryoutGroupInfo[m_nPryoutRepresentGrpIdx].dArea;

	std::vector<double> aRepAnchorX, aRepAnchorY;
	const std::vector<std::pair<int, int>>& aRepBearingIdx = m_aPryoutGroupInfo[m_nPryoutRepresentGrpIdx].aBearingIdx;
	int nAnchorNumI, nAnchorNumJ;
	for (int i = 0; i < aRepBearingIdx.size(); i++)	
	{
		int nRepBearingI = aRepBearingIdx[i].first;
		int nRepBearingJ = aRepBearingIdx[i].second;
		GetAnchorNumber(nRepBearingI, nRepBearingJ, nAnchorNumI, nAnchorNumJ);
		for (int j = 0; j < nAnchorNumI; j++)
		{
			for (int k = 0; k < nAnchorNumJ; k++)
			{
				CalcAnchorPos(nRepBearingI, nRepBearingJ, j, k, dAnchorX, dAnchorY);
				aRepAnchorX.push_back(dAnchorX);
				aRepAnchorY.push_back(dAnchorY);
			}
		}
	}

	nSize = aSrcPoly.GetSize();
	ASSERT(nSize == aBcOld.size());
	for (int i = 0; i < nSize ; i++)
	{
		if(setDelIdx.find(i) != setDelIdx.end())
			continue;

		aShapeX.aData.RemoveAll();
		aShapeY.aData.RemoveAll();
		CopyToCArray(aSrcPoly[i].OuterPolygon, aShapeX.aData, aShapeY.aData);

		aArProjShapeX.Add(aShapeX);
		aArProjShapeY.Add(aShapeY);

		BOOL bIsRepShape = FALSE;
		for (int k = 0; k < aRepAnchorX.size(); k++)
		{
			if (IsInnerPolygon(aRepAnchorX[k], aRepAnchorY[k], aSrcPoly[i]))
			{
				bIsRepShape = TRUE;
				break;
			}			
		}
		if(bIsRepShape)
		{
			setBcIdx.insert(aArProjShapeX.GetSize() - 1);
		}

		nSizeJ = aSrcPoly[i].aInnerPolygon.GetSize();
		for (int j = 0; j < nSizeJ ; j++)
		{
			aShapeX.aData.RemoveAll();
			aShapeY.aData.RemoveAll();
			CopyToCArray(aSrcPoly[i].aInnerPolygon[j], aShapeX.aData, aShapeY.aData);

			aArProjShapeX.Add(aShapeX);
			aArProjShapeY.Add(aShapeY);

			bIsRepShape = FALSE;
			for (int k = 0; k < aRepAnchorX.size(); k++)
			{
				if (IsInnerPolygon(aRepAnchorX[k], aRepAnchorY[k], aSrcPoly[i]))
				{
					bIsRepShape = TRUE;
					break;
				}
			}
			if (bIsRepShape)
			{
				setBcIdx.insert(aArProjShapeX.GetSize() - 1);
			}
		}
	}	

	if(nBc >= 2)
	{
		int BearingIdx[2][2];
				
		GetBearingNumber(nBearingNumI, aBearingNumJ);
		nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 게산이 가능하도록 수정 필요
		
		double dPosOther[2];

		std::set<T_INT_INT> setFixBearing;	
		nSize = aBearingI.GetSize();
		for (int i = 0; i < nSize ; i++)
		{
			setFixBearing.insert(T_INT_INT(aBearingI[i], aBearingJ[i]));
		}

		double dMinSax = DBL_MAX;
		double dMinSay = DBL_MAX;

		BOOL bFindFirst;

		// 가로방향
		bFindFirst = FALSE;
		for (T_INT_INT EachVal : setFixBearing)
		{
			int nBearingIdxI = EachVal.nVal1;
			int nBearingIdxJ = EachVal.nVal2;
			GetAnchorNumber(nBearingIdxI, nBearingIdxJ, nAnchorNumI, nAnchorNumJ);
			//Left			
			if(nBearingIdxJ != 0)
			{
				CalcAnchorPos(nBearingIdxI, nBearingIdxJ - 1, 0, nAnchorNumJ - 1, dPosOther[0], dPosOther[1]);
				if(!IsInnerPolygon(dPosOther[0], dPosOther[1], aSrcPoly[nRepSrcIdx]))
				{
					continue;
				}

				BearingIdx[0][0] = nBearingIdxI;
				BearingIdx[0][1] = nBearingIdxJ;
				BearingIdx[1][0] = BearingIdx[0][0];
				BearingIdx[1][1] = BearingIdx[0][1];
				if(NextBearingIdx(nDir, 0, -1, BearingIdx[1][0], BearingIdx[1][1]))
				{
					if(!bFindFirst)
					{
						bFindFirst = TRUE;
						dMinSax = GetBearingGap(0, BearingIdx);
					}
					else
					{
						dMinSax = min( dMinSax, GetBearingGap(0, BearingIdx));
					}
				}
				
			}
			//Right
			if(nBearingIdxJ < nBearingNumJ - 1)
			{
				CalcAnchorPos(nBearingIdxI, nBearingIdxJ + 1, 0, 0, dPosOther[0], dPosOther[1]);
				if(!IsInnerPolygon(dPosOther[0], dPosOther[1], aSrcPoly[nRepSrcIdx]))
				{
					continue;
				}

				BearingIdx[0][0] = nBearingIdxI;
				BearingIdx[0][1] = nBearingIdxJ;
				BearingIdx[1][0] = BearingIdx[0][0];
				BearingIdx[1][1] = BearingIdx[0][1];
				if(NextBearingIdx(nDir, 0, 1, BearingIdx[1][0], BearingIdx[1][1]))
				{
					if(!bFindFirst)
					{
						bFindFirst = TRUE;
						dMinSax = GetBearingGap(0, BearingIdx);
					}
					else
					{
						dMinSax = min( dMinSax, GetBearingGap(0, BearingIdx));
					}
				}
			}
		}
		if(!bFindFirst)
		{
			dMinSax = 0.0;
		}

		// 세로 방향
		bFindFirst = FALSE;
		for (T_INT_INT EachVal : setFixBearing)
		{
			int nBearingIdxI = EachVal.nVal1;
			int nBearingIdxJ = EachVal.nVal2;
			
			//Down
			if(nBearingIdxI != 0)
			{
				GetAnchorNumber(nBearingIdxI - 1, nBearingIdxJ, nAnchorNumI, nAnchorNumJ);
				CalcAnchorPos(nBearingIdxI - 1, nBearingIdxJ, nAnchorNumI - 1, 0, dPosOther[0], dPosOther[1]);
				if(!IsInnerPolygon(dPosOther[0], dPosOther[1], aSrcPoly[nRepSrcIdx]))
				{
					continue;
				}

				BearingIdx[0][0] = nBearingIdxI;
				BearingIdx[0][1] = nBearingIdxJ;
				BearingIdx[1][0] = BearingIdx[0][0];
				BearingIdx[1][1] = BearingIdx[0][1];
				if(NextBearingIdx(nDir, -1, 0, BearingIdx[1][0], BearingIdx[1][1]))
				{
					if(!bFindFirst)
					{
						bFindFirst = TRUE;
						dMinSay = GetBearingGap(1, BearingIdx);
					}
					else
					{
						dMinSay = min( dMinSay, GetBearingGap(1, BearingIdx));
					}
				}
			}
			//Up
			if(nBearingIdxI < nBearingNumI - 1)
			{
				CalcAnchorPos(nBearingIdxI + 1, nBearingIdxJ, 0, 0, dPosOther[0], dPosOther[1]);
				if(!IsInnerPolygon(dPosOther[0], dPosOther[1], aSrcPoly[nRepSrcIdx]))
				{
					continue;
				}

				BearingIdx[0][0] = nBearingIdxI;
				BearingIdx[0][1] = nBearingIdxJ;
				BearingIdx[1][0] = BearingIdx[0][0];
				BearingIdx[1][1] = BearingIdx[0][1];
				if(NextBearingIdx(nDir, 1, 0, BearingIdx[1][0], BearingIdx[1][1]))
				{
					if(!bFindFirst)
					{
						bFindFirst = TRUE;
						dMinSay = GetBearingGap(1, BearingIdx);
					}
					else
					{
						dMinSay = min( dMinSay, GetBearingGap(1, BearingIdx));
					}
				}
			}
		}
		if(!bFindFirst)
		{
			dMinSay = 0.0;
		}

		if(nDir == DIR_LONGI)
		{
			dSax = dMinSax;
			dSay = dMinSay;
		}
		else if(nDir == DIR_TRANS)
		{
			dSax = dMinSay;
			dSay = dMinSax;
		}
		else
		{
			ASSERT(0);
		}
	}
	else if(nBc == 1)
	{
		// 단일 저항 일 경우 고정단 가동단과 무관하게 양 옆의 sax, say 를 계산한다.
		int nIdxI, nIdxJ;
		if(FindFirstInnerBearingIdx(aSrcPoly[nRepSrcIdx], nIdxI, nIdxJ))
		{
			GetPryoutBasicSaxy(nDir, nIdxI, nIdxJ, dSax, dSay);
		}
		else
		{
			//ASSERT(0); // 이런 일은 없겠지만... 그래도 처리 해 놓자..
			GetPryoutBasicSaxy(nDir, 0, 0, dSax, dSay);
		}
		dDist = Gethef(nDir, nIdxI, nIdxJ) * 1.5;

		// [CIVIL-2018] 뽑힘검토 단일저항 시 해당 Bearing 에서의 저항하는 anchor 수를 고려한 계산을 해야 함.
		if(nDir == DIR_LONGI)
		{
			// CDgnSeisAnchorShapeMgr::CalcAnchorProjPosPryoutEachBearing 에서 anchor 가 단일로 저항하는지를 판단하는 처리와 동일하게 해야 함
			if(m_aArBrprD[nIdxI][nIdxJ].dS1 - dDist * 2 < -m_dBigZero)
			{
				bSingleResist = FALSE;
			}
			else 
			{
				bSingleResist = TRUE;
			}
		}
		else if(nDir == DIR_TRANS)
		{
			if(m_aArBrprD[nIdxI][nIdxJ].dS2 - dDist * 2 < -m_dBigZero)
			{
				bSingleResist = FALSE;
			}
			else 
			{
				bSingleResist = TRUE;
			}
		}
		else
		{
			ASSERT(0);
		}
	}

	m_nLastPryoutGroupDir = nDir;

	//////////////////////////////////////////////////////////////////////////
	nPryoutGrpIdx = (nDir == 0) ? m_nPryoutRepresentGrpIdx_X : m_nPryoutRepresentGrpIdx_Y;
	if (!m_bRatioChk && nPryoutGrpIdx > -1)
	{
		double dRatio = 0.0;
		KeyForPryoutBCPos Key(nDir, nPryoutGrpIdx, m_nDownUpPryMgr, m_nLeftRightPryMgr);
		auto itrFind = m_mapPryoutBCData.find(Key);
		if (itrFind == m_mapPryoutBCData.end())
		{ // 없으면
			ValForPryoutBC Val;
			Val.Set(aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bSingleResist);
			m_mapPryoutBCData[Key] = Val;

			if (nDir == 0)
			{
				m_aPryoutGroupInfo_X.clear();
				for (int nIndx = 0; nIndx < m_aPryoutGroupInfo.size(); nIndx++)
				{
					m_aPryoutGroupInfo_X.push_back(m_aPryoutGroupInfo[nIndx]);
				}
			}
			else
			{
				m_aPryoutGroupInfo_Y.clear();
				for (int nIndx = 0; nIndx < m_aPryoutGroupInfo.size(); nIndx++)
				{
					m_aPryoutGroupInfo_Y.push_back(m_aPryoutGroupInfo[nIndx]);
				}
			}
		}
		else
		{
			ASSERT(0);
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

BOOL CDgnSeisAnchorShapeMgr::FindFirstInnerBearingIdx(T_COMPLEX_POLYGON2D& Poly, int& nBearingIdxI, int& nBearingIdxJ)
{
	nBearingIdxI = 0;
	nBearingIdxJ = 0;		

	int nBearingNumI;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);

	double dX, dY;

	for (int i = 0; i < nBearingNumI; i++)
	{
		for (int j = 0; j < aBearingNumJ[i]; j++)
		{
			CalcAnchorPos(i, j, 0, 0, dX, dY);
			if(IsInnerPolygon(dX, dY, Poly))
			{
				nBearingIdxI = i;
				nBearingIdxJ = j;
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CDgnSeisAnchorShapeMgr::IsInnerPolygon(double dX, double dY, const T_COMPLEX_POLYGON2D& Polygon)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	T_POINT2D Pos;
	Pos.x = dX;
	Pos.y = dY;
	if(CGeomFunc::IsPointInPolygon2(Pos, Polygon.OuterPolygon.aVertex, FALSE, dZero))
	{
		int nSize;
		nSize = Polygon.aInnerPolygon.GetSize();
		for (int i = 0; i < nSize ; i++)
		{
			if(CGeomFunc::IsPointInPolygon2(Pos, Polygon.aInnerPolygon[i].aVertex, FALSE, dZero))
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void CDgnSeisAnchorShapeMgr::GetPryoutBc(int nDir, CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&>& aArProjShapeX, CArray<T_CARRAY<double, double>, T_CARRAY<double, double>&>& aArProjShapeY, std::set<int>& setBcIdx, BOOL& bSingleResist)
{
	int nBc;
	double dArea;
	double dSax, dSay;
	std::vector<std::pair<int, int>> aRepBearingIdxTemp;
	GetPryoutBc(nDir, aRepBearingIdxTemp, aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bSingleResist);
}

void CDgnSeisAnchorShapeMgr::CalcAnchorProjPosPryoutEachBearing(int nDir, int nBearingI, int nBearingJ, BOOL bNoUnion,
															CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeXEachBearing,
															CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&>& aArProjShapeYEachBearing)
{	
	// Anchor 의 간격이 균일하고, 한 Bearing 안에서 한 열 또는 행의 Anchor 만 Project 하는 규칙을 활용해서 Shape을 빠르게 얻어 오는 코드
	aArProjShapeXEachBearing.RemoveAll();
	aArProjShapeYEachBearing.RemoveAll();

	int nBearingNumI, nBearingNumJ, nAnchorNumI, nAnchorNumJ;
	CArray<int, int> aBearingNumJ;
	GetBearingNumber(nBearingNumI, aBearingNumJ);
	nBearingNumJ = aBearingNumJ[0]; // CivilSeismicTest <- 비대칭 게산이 가능하도록 수정 필요

	GetAnchorNumber(nBearingI, nBearingJ, nAnchorNumI, nAnchorNumJ);

	std::vector<double> aLocalAnchorPosX;
	std::vector<double> aLocalAnchorPosY;
	CalcLocalAnchorPos(nBearingI, nBearingJ, aLocalAnchorPosX, aLocalAnchorPosY);

	/*
	 아래 조건은 고정단 가동단 개발피드백 과정에서 빠졌음.. 그 대신 Pryout 파괴 영역 계산은 Ca2 계산시 고려했던 대표 행의 bearing 을 1개 이상 포함 해야 한다는 조건이 들어감
	if(nDir == DIR_LONGI)
	{
		if(nBearingNumI != 1 && nAnchorNumI == 1 && nBearingI == nBearingNumI - 1)
		{
			return; // Anchor가 내측이면서 외측인 Bearing 는 무시
		}
	}
	else if(nDir == DIR_TRANS)
	{
		if(nBearingNumJ != 1 && nAnchorNumJ == 1 && nBearingJ == nBearingNumJ - 1)
		{
			return; // Anchor가 내측이면서 외측인 Bearing 는 무시
		}
	}
	*/
	double dDist = Gethef(nDir, nBearingI, nBearingJ) * 1.5;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	T_CARRAY<double,double> aX, aY;

	const T_BRPR_D& BrprD = m_aArBrprD[nBearingI][nBearingJ];
	double dBearingHeight = m_aArBearingHeight[nBearingI][nBearingJ];
	double dBearingWidth = m_aArBearingWidth[nBearingI][nBearingJ];

	// [CIVIL-941] 검토시 딱 맞을 경우 단일 저항으로 판단하게 처리.. 단, 계산 값은 작아지면 안됨...
	// CDgnSeisAnchorShapeMgr::GetPryoutBc 에서 제일 끝 [CIVIL-CIVIL-2018] 에서 anchor 가 단일로 저항하는지를 판단하는 처리와 동일하게 해야 함
	if(!bNoUnion && (nDir == DIR_LONGI) && BrprD.dS1 - dDist * 2 < -m_dBigZero)
	{
		// 가로로 붙은 경우
		if (m_nDownUpPryMgr == DIR_POS)
		{
			aX.aData.Add(BrprD.dSx - dDist);
			aY.aData.Add(BrprD.dSy - dDist);

			aX.aData.Add(BrprD.dSx - dDist);
			if (m_AnevD.bPryoutRow)
				aY.aData.Add(BrprD.dSy + (BrprD.nN2 - 1) * BrprD.dS2 + dDist);  // Last
			else
				aY.aData.Add(BrprD.dSy + dDist);

			aX.aData.Add(dBearingWidth - BrprD.dSx + dDist);
			if (m_AnevD.bPryoutRow)
				aY.aData.Add(BrprD.dSy + (BrprD.nN2-1)*BrprD.dS2 + dDist);  // Last
			else
				aY.aData.Add(BrprD.dSy + dDist);

			aX.aData.Add(dBearingWidth - BrprD.dSx + dDist);
			aY.aData.Add(BrprD.dSy - dDist);
		}
		else if (m_nDownUpPryMgr == DIR_NEG)
		{
			aX.aData.Add(BrprD.dSx - dDist);
			aY.aData.Add(-(BrprD.dSy - dDist));

			aX.aData.Add(BrprD.dSx - dDist);
			if (m_AnevD.bPryoutRow)
				aY.aData.Add(-(BrprD.dSy + (BrprD.nN2-1)*BrprD.dS2  + dDist));  // Last
			else
				aY.aData.Add(-(BrprD.dSy + dDist));

			aX.aData.Add(dBearingWidth - BrprD.dSx + dDist);
			if (m_AnevD.bPryoutRow)
				aY.aData.Add(-(BrprD.dSy + (BrprD.nN2-1)*BrprD.dS2  + dDist));  // Last
			else
				aY.aData.Add(-(BrprD.dSy + dDist));

			aX.aData.Add(dBearingWidth - BrprD.dSx + dDist);
			aY.aData.Add(-(BrprD.dSy - dDist));
		}

	}
	else if(!bNoUnion && (nDir == DIR_TRANS) && BrprD.dS2 - dDist * 2 < -m_dBigZero)
	{
		// 세로로 붙은 경우
		if (m_nLeftRightPryMgr == DIR_POS)
		{
			aX.aData.Add(BrprD.dSx - dDist);
			aY.aData.Add(BrprD.dSy - dDist);

			aX.aData.Add(BrprD.dSx - dDist);
			aY.aData.Add(dBearingHeight - BrprD.dSy + dDist);

			if (m_AnevD.bPryoutRow)
				aX.aData.Add(dBearingWidth - BrprD.dSx + dDist); // Last
			else 
				aX.aData.Add(BrprD.dSx + dDist);
			aY.aData.Add(dBearingHeight - BrprD.dSy + dDist);

			if (m_AnevD.bPryoutRow)
				aX.aData.Add(dBearingWidth - BrprD.dSx + dDist); // Last
			else
				aX.aData.Add(BrprD.dSx + dDist);
			aY.aData.Add(BrprD.dSy - dDist);
		}
		else if (m_nLeftRightPryMgr == DIR_NEG)
		{
			double dAddX = aLocalAnchorPosX[nAnchorNumJ-1] - aLocalAnchorPosX[0];
		  
			if (m_AnevD.bPryoutRow)
				aX.aData.Add(BrprD.dSx - dDist);    // Last
			else
				aX.aData.Add(BrprD.dSx - dDist + dAddX);
			aY.aData.Add(-(BrprD.dSy - dDist));

			if (m_AnevD.bPryoutRow)
				aX.aData.Add(BrprD.dSx - dDist);    // Last
			else
				aX.aData.Add(BrprD.dSx - dDist + dAddX);
			aY.aData.Add(-(dBearingHeight - BrprD.dSy + dDist));

			aX.aData.Add(BrprD.dSx + dDist + dAddX);
			aY.aData.Add(-(dBearingHeight - BrprD.dSy + dDist));

			aX.aData.Add(BrprD.dSx + dDist + dAddX);
			aY.aData.Add(-(BrprD.dSy - dDist));
		}
	}
	else
	{
		// 안붙은 경우		
		if(nDir == DIR_LONGI)
		{
			if (m_nDownUpPryMgr == DIR_POS)
			{
				for (int i = 0; i < nAnchorNumJ; i++)
				{
					aX.aData.RemoveAll();
					aY.aData.RemoveAll();
					CalcEachAnchorProjPosPryout(nDir, nBearingI, nBearingJ, 0, i, aX.aData, aY.aData);
					aArProjShapeXEachBearing.Add(aX);
					aArProjShapeYEachBearing.Add(aY);
				}

				// Last
				if (m_AnevD.bPryoutRow)
				{
					for (int i = 0; i < nAnchorNumJ; i++)
					{
						aX.aData.RemoveAll();
						aY.aData.RemoveAll();
						CalcEachAnchorProjPosPryout(nDir, nBearingI, nBearingJ, nAnchorNumI - 1, i, aX.aData, aY.aData);
						aArProjShapeXEachBearing.Add(aX);
						aArProjShapeYEachBearing.Add(aY);
					}
				}
			}
			else if (m_nDownUpPryMgr == DIR_NEG)
			{
				for (int i = nAnchorNumJ-1; i >=0; i--)
				{
					aX.aData.RemoveAll();
					aY.aData.RemoveAll();
					CalcEachAnchorProjPosPryout(nDir, nBearingI, nBearingJ, nAnchorNumI - 1, i, aX.aData, aY.aData);
					aArProjShapeXEachBearing.Add(aX);
					aArProjShapeYEachBearing.Add(aY);
				}

				// Last
				if (m_AnevD.bPryoutRow)
				{
					for (int i = nAnchorNumJ - 1; i >= 0; i--)
					{
						aX.aData.RemoveAll();
						aY.aData.RemoveAll();
						CalcEachAnchorProjPosPryout(nDir, nBearingI, nBearingJ, 0, i, aX.aData, aY.aData);
						aArProjShapeXEachBearing.Add(aX);
						aArProjShapeYEachBearing.Add(aY);
					}
				}
			}
			
		}
		else if(nDir == DIR_TRANS)
		{

			if (m_nLeftRightPryMgr == DIR_POS)
			{	
				for (int i = 0; i < nAnchorNumI; i++)
				{
					aX.aData.RemoveAll();
					aY.aData.RemoveAll();
					CalcEachAnchorProjPosPryout(nDir, nBearingI, nBearingJ, i, 0, aX.aData, aY.aData);
					aArProjShapeXEachBearing.Add(aX);
					aArProjShapeYEachBearing.Add(aY);
				}

				// Last
				if (m_AnevD.bPryoutRow)
				{
					for (int i = 0; i < nAnchorNumI; i++)
					{
						aX.aData.RemoveAll();
						aY.aData.RemoveAll();
						CalcEachAnchorProjPosPryout(nDir, nBearingI, nBearingJ, i, nAnchorNumJ - 1, aX.aData, aY.aData);
						aArProjShapeXEachBearing.Add(aX);
						aArProjShapeYEachBearing.Add(aY);
					}
				}				
			}
			else if (m_nLeftRightPryMgr == DIR_NEG)
			{
				for (int i = nAnchorNumI - 1; i >= 0; i--)
				{
					aX.aData.RemoveAll();
					aY.aData.RemoveAll();
					CalcEachAnchorProjPosPryout(nDir, nBearingI, nBearingJ, i, nAnchorNumJ - 1, aX.aData, aY.aData);
					aArProjShapeXEachBearing.Add(aX);
					aArProjShapeYEachBearing.Add(aY);
				}

				// Last
				if (m_AnevD.bPryoutRow)
				{
					for (int i = nAnchorNumI - 1; i >= 0; i--)
					{
						aX.aData.RemoveAll();
						aY.aData.RemoveAll();
						CalcEachAnchorProjPosPryout(nDir, nBearingI, nBearingJ, i, 0, aX.aData, aY.aData);
						aArProjShapeXEachBearing.Add(aX);
						aArProjShapeYEachBearing.Add(aY);
					}
				}
			}
		}		
		else
		{
			ASSERT(0);
		}
		return;
	}
	
	double dBearingCenterX, dBearingCenterY;

	CalcBearingCenterPos(nBearingI, nBearingJ, dBearingCenterX, dBearingCenterY);

	// 회전 및 센터 반영
	int nSize;
	nSize = aX.aData.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		if (nDir == DIR_TRANS && m_nDownUpPryMgr == DIR_NEG && m_nLeftRightPryMgr == DIR_NEG)
		{
			aX.aData[i] -= dBearingWidth * 0.5;
		}
		else
		{
			aX.aData[i] -= dBearingWidth * 0.5;
		}

		if(nDir == DIR_LONGI)
		{
			if (m_nDownUpPryMgr == DIR_POS)
			{
				aY.aData[i] -= dBearingHeight * 0.5;
			}
			else if (m_nDownUpPryMgr == DIR_NEG)
			{
				aY.aData[i] += dBearingHeight * 0.5;
			}
		}
		else if (nDir == DIR_TRANS)
		{
			if(m_nLeftRightPryMgr == DIR_POS)
			{
				aY.aData[i] -= dBearingHeight * 0.5;
			}
			else if (m_nLeftRightPryMgr == DIR_NEG)
			{
				aY.aData[i] += dBearingHeight * 0.5;
			}
		}

// 		if (m_nDownUpPryMgr == DIR_POS && m_nLeftRightPryMgr == DIR_POS)
// 		{
// 			aY.aData[i] -= dBearingHeight * 0.5;
// 		}
// 		else if (m_nDownUpPryMgr == DIR_NEG && m_nLeftRightPryMgr == DIR_NEG)
// 		{
// 			aY.aData[i] += dBearingHeight * 0.5;
// 		}
// 		else
// 			ASSERT(0);

//		if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
		{	
			math_lib::rotate_2d(aX.aData[i], aY.aData[i], -m_AnevD.aLayer[m_AnevD.aLayer.GetSize() - 1-nBearingI].dTheta_q2);
		}
// 		else
// 		{
// 			math_lib::rotate_2d(aX.aData[i], aY.aData[i], -m_AnevD.dTheta_q2);
// 		}
		
		aX.aData[i] += dBearingCenterX;
		aY.aData[i] += dBearingCenterY;
	}

	aArProjShapeXEachBearing.Add(aX);
	aArProjShapeYEachBearing.Add(aY);
}

void CDgnSeisAnchorShapeMgr::CalcEachAnchorProjPosPryout(int nDir, int nBearingI, int nBearingJ, int nAnchorI, int nAnchorJ, CArray<double, double>& aProjShapeX, CArray<double, double>& aProjShapeY)
{
	double dBearingCenterX, dBearingCenterY;
	CalcAnchorPos(nBearingI, nBearingJ, nAnchorI, nAnchorJ, dBearingCenterX, dBearingCenterY);

	aProjShapeX.RemoveAll();
	aProjShapeY.RemoveAll();

	double dVal = Gethef(nDir, nBearingI, nBearingJ) * 1.5;
	
	aProjShapeX.Add(-dVal);
	aProjShapeY.Add(-dVal);

	aProjShapeX.Add(-dVal);
	aProjShapeY.Add(dVal);

	aProjShapeX.Add(dVal);
	aProjShapeY.Add(dVal);

	aProjShapeX.Add(dVal);
	aProjShapeY.Add(-dVal);
	
	//aProjShapeX.Add(-dVal);
	//aProjShapeY.Add(-dVal);
	//
	//aProjShapeX.Add(dVal);
	//aProjShapeY.Add(-dVal);
	//
	//aProjShapeX.Add(dVal);
	//aProjShapeY.Add(dVal);
	//
	//aProjShapeX.Add(-dVal);
	//aProjShapeY.Add(dVal);

	int nSize;
	nSize = aProjShapeX.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
//		if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
		{	
			math_lib::rotate_2d(aProjShapeX[i], aProjShapeY[i], -m_AnevD.aLayer[m_AnevD.aLayer.GetSize() - 1 - nBearingI].dTheta_q2);
		}
// 		else
// 		{
// 			math_lib::rotate_2d(aProjShapeX[i], aProjShapeY[i], -m_AnevD.dTheta_q2);
// 		}

		aProjShapeX[i] += dBearingCenterX;
		aProjShapeY[i] += dBearingCenterY;
	}
}

BOOL CDgnSeisAnchorShapeMgr::GetLineInterSectPoint(double dPos1X, double dPos1Y, double dPos2X, double dPos2Y, double dPos3X, double dPos3Y, double dPos4X, double dPos4Y, double& dRstX, double& dRstY)
{
	//double pl1_i[3], pl1_j[3], pl2_i[3], pl2_j[3], Tolerance, dblDistance, pInts[3];
	//
	//pl1_i[0] = dPos1X;
	//pl1_i[1] = dPos1Y;
	//pl1_i[2] = 0.0;
	//
	//pl1_j[0] = dPos2X;
	//pl1_j[1] = dPos2Y;
	//pl1_j[2] = 0.0;
	//
	//pl2_i[0] = dPos3X;
	//pl2_i[1] = dPos3Y;
	//pl2_i[2] = 0.0;
	//
	//pl2_j[0] = dPos4X;
	//pl2_j[1] = dPos4Y;
	//pl2_j[2] = 0.0;
	
	//CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//Tolerance = pDoc->m_pInitCtrl->GetPreferenceRegardZero();
		
	if(CMathFunc::mathIntersectLine2D(dPos1X, dPos1Y, dPos2X, dPos2Y, dPos3X, dPos3Y, dPos4X, dPos4Y, dRstX, dRstY, TRUE))
	//if(CMathFunc::mathIntersectLine2(pl1_i, pl1_j, pl2_i, pl2_j, Tolerance, dblDistance, pInts))
	{
		//dRstX = pInts[0];
		//dRstY = pInts[1];

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CDgnSeisAnchorShapeMgr::PntMove(double dDirX, double dDirY, double dDist, double& dX, double& dY)
{
	dX = dX + dDist * dDirX;
	dY = dY + dDist * dDirY;
}

void CDgnSeisAnchorShapeMgr::GetDir(double dX1 , double dY1, double dX2, double dY2, double& dDirX, double& dDirY)
{
	dDirX = dX2 - dX1;
	dDirY = dY2 - dY1;
	CMathFunc::mathNormalize(dDirX, dDirY, dDirX, dDirY);
}

void CDgnSeisAnchorShapeMgr::CalcAnchorPos(int nBearingI, int nBearingJ, int nAnchorI, int nAnchorJ, double &dX, double &dY)
{
	double dBearingCenterX, dBearingCenterY;
	CalcBearingCenterPos(nBearingI, nBearingJ, dBearingCenterX, dBearingCenterY);

	std::vector<double> aLocalAnchorPosX;
	std::vector<double> aLocalAnchorPosY;
	CalcLocalAnchorPos(nBearingI, nBearingJ, aLocalAnchorPosX, aLocalAnchorPosY);	
	
	ASSERT(aLocalAnchorPosX.size() == aLocalAnchorPosY.size());

	if(aLocalAnchorPosX.size() <= 0)
		return;

	int nIdx = m_aArBrprD[nBearingI][nBearingJ].nN1 * nAnchorI + nAnchorJ;

	if (nIdx >= aLocalAnchorPosX.size())
		return;

	dX = aLocalAnchorPosX[nIdx];
	dY = aLocalAnchorPosY[nIdx];

	dX -= m_aArBearingWidth[nBearingI][nBearingJ] * 0.5;
	dY -= m_aArBearingHeight[nBearingI][nBearingJ] * 0.5;

//	if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
	{
		math_lib::rotate_2d(dX, dY, -m_AnevD.aLayer[m_AnevD.aLayer.GetSize() - 1 - nBearingI].dTheta_q2);
	}
// 	else
// 	{
// 		math_lib::rotate_2d(dX, dY, -m_AnevD.dTheta_q2);
// 	}	
	
	dX += dBearingCenterX;
	dY += dBearingCenterY;
}

void CDgnSeisAnchorShapeMgr::CalcLocalAnchorPos(int nBearingI, int nBearingJ, std::vector<double>& aLocalAnchorPosX, std::vector<double>& aLocalAnchorPosY)
{
	// 이거 느리면... aLocalAnchorPosX 와 aLocalAnchorPosY 를 Bearing 수만큼 늘려서.. 재사용 하도록 해야 할 듯
	double dX, dY;

	const T_BRPR_D& BrprD = m_aArBrprD[nBearingI][nBearingJ];

	dX = BrprD.dSx;
	dY = BrprD.dSy;

	int nSize, nSizeJ;
	nSize = BrprD.nN2;
	for (int i = 0; i < nSize; i++)
	{
		nSizeJ = BrprD.nN1;		
		for (int j = 0; j < nSizeJ; j++)
		{
			aLocalAnchorPosX.push_back(dX);
			aLocalAnchorPosY.push_back(dY);

			if(j < BrprD.nN1 - 1)
			{
				dX += BrprD.dS1;
			}
		}

		dX = BrprD.dSx;
		if(i < BrprD.nN2 - 1)
		{
			dY += BrprD.dS2;
		}
	}
}

BOOL CDgnSeisAnchorShapeMgr::IsPryoutRepresentGrp(int nDir, int nBearingI, int nBearingJ)
{
	if(m_nLastPryoutGroupDir != nDir)
	{
		int nBc;
		double dSax, dSay, dArea;
		BOOL bSingleResist;
		std::set<int> setBcIdx;
		CArray<T_CARRAY<double,double>,T_CARRAY<double,double>&> aArProjShapeX, aArProjShapeY;
		std::vector<std::pair<int, int>> aRepBearingIdxTemp;
		GetPryoutBc(nDir, aRepBearingIdxTemp, aArProjShapeX, aArProjShapeY, setBcIdx, nBc, dArea, dSax, dSay, bSingleResist);
	}

	if(m_nPryoutRepresentGrpIdx == -1)
	{
		ASSERT(0);// GetPryoutBc() 에서 세팅해야 하는 상황인데.. 뭔가 이상함..
		return FALSE;
	}

	for (int i = 0; i < m_aPryoutGroupInfo[m_nPryoutRepresentGrpIdx].aBearingIdx.size(); i++)
	{
		const std::pair<int, int> pairIdx = m_aPryoutGroupInfo[m_nPryoutRepresentGrpIdx].aBearingIdx[i];
		if(pairIdx.first == nBearingI && pairIdx.second == nBearingJ)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDgnSeisAnchorShapeMgr::IsFixedBearing(int nIdxI, int nIdxJ, int nDir, BOOL bFixedCond)
{
	if(!bFixedCond)
		return TRUE;

	// Anchor_TenConc 검토에서는 첫번째 앵커에 대해서는 양방향 고정으로 보고 고려한다.
	if (m_bAnchorTenConc && nIdxI == 0 && nIdxJ == 0)
		return TRUE;

	// CDgnSeisAnchorShapeMgr 에서 I 는 아래에서 위로 커지고.. UI 상의 I 는 위에서 아래로 커진다...
	if(!m_bSetAnev)
	{
		ASSERT(0);
		return TRUE;
	}
	if(nIdxI >= m_AnevD.SuppType.aLayer.GetSize() || 0 > nIdxI)
	{
		ASSERT(0);
		return TRUE;
	}
	if(nIdxJ >= m_AnevD.SuppType.aLayer[0].aBrevType.GetSize() || 0 > nIdxJ)
	{
		ASSERT(0);
		return TRUE;
	}

	// 0:고정단, 1:양방향 가동단, 2:교축 가동단, 3:교직 가동단
	int nNum = m_AnevD.SuppType.aLayer.GetSize();
	int nFixType = m_AnevD.SuppType.aLayer[nNum - nIdxI - 1].aBrevType[nIdxJ];
	
	if(nFixType == 0)
	{
		return TRUE;
	}
	else if(nFixType == 1)
	{
		return FALSE;
	}
	else if(nFixType == 2)
	{
		if(nDir == DIR_LONGI)
		{
			return FALSE;
		}
		else if(nDir == DIR_TRANS)
		{
			return TRUE;
		}
		else
		{
			ASSERT(0);
		}	
	}
	else if(nFixType == 3)
	{
		if(nDir == DIR_LONGI)
		{
			return TRUE;
		}
		else if(nDir == DIR_TRANS)
		{
			return FALSE;
		}
		else
		{
			ASSERT(0);
		}	
	}
	else if (nFixType == -1)
	{
		// 비어있음
		return FALSE;
	}
	else
	{
		ASSERT(0);
	}
	
	ASSERT(0);
	return TRUE;
}

void CDgnSeisAnchorShapeMgr::CalcBearingCenterPos(int nIdxI, int nIdxJ, double& dCenterX, double& dCenterY)
{
//	if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
	{	
		CArray<double, double> aX, aY;
		GetCopyingShape(aX, aY);

		CArray<T_ANEV_LAYER_D, T_ANEV_LAYER_D> aBugLayer;

		for (int i = m_AnevD.aLayer.GetSize() - 1; i >= 0; i--)
		{
			aBugLayer.Add(m_AnevD.aLayer[i]);
		}

		double dStation, dOffset;
		dStation = aBugLayer[nIdxI].dD1;
		for (int i = 0; i < nIdxJ; i++)
		{
			if (i >= aBugLayer[nIdxI].aSax.GetSize())
			{
				ASSERT(0);
				return;
			}
			dStation += aBugLayer[nIdxI].aSax[i];
		}

		dOffset = aY[0]; //check dOffset = aBugLayer[0].dD2
		dOffset += aBugLayer[0].dD2;
		for (int i = 0; i < nIdxI; i++)
		{
		  //if (i >= m_AnevD.aSay.GetSize())
			if (i >= aBugLayer[nIdxI].aSax.GetSize())
			{
				ASSERT(0);
				return;
			}
			dOffset += (aBugLayer[i].dSay-aBugLayer[i+1].dSay);
		}

		CalcMainCoord(dStation, dOffset, aBugLayer[nIdxI].dTheta_q1, dCenterX, dCenterY);
	}
// 	else
// 	{
// 		double dStation, dOffset;
// 		dStation = m_AnevD.dD1;
// 		for (int i = 0; i < nIdxJ; i++)
// 		{
// 			if (i >= m_AnevD.aSax.GetSize())
// 			{
// 				ASSERT(0);
// 				return;
// 			}
// 			dStation += m_AnevD.aSax[i];
// 		}
// 
// 		dOffset = m_dCopyingTopBotY[1];
// 		dOffset += m_AnevD.dD2;
// 		for (int i = 0; i < nIdxI; i++)
// 		{
// 			if (i >= m_AnevD.aSay.GetSize())
// 			{
// 				ASSERT(0);
// 				return;
// 			}
// 			dOffset += m_AnevD.aSay[i];
// 		}
// 
// 		CalcMainCoord(dStation, dOffset, m_AnevD.dTheta_q1, dCenterX, dCenterY);
// 	}	
}

void CDgnSeisAnchorShapeMgr::GetBearingNumber(int& nNumI, CArray<int, int>& raNumJ)
{
	if (!m_bSetAnev)
	{
		nNumI = 0;
		raNumJ.RemoveAll();
		return;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	nNumI = 0;
	raNumJ.RemoveAll();

//	if (CTestEnvMgr::GetTestEnvST(_T("CivilSeismicTest")) == _T("yes"))
	{
		nNumI = m_AnevD.aLayer.GetSize();

// 		if(m_AnevD.aLayer.GetSize() == 1)
// 		{
// 			if (m_AnevD.aLayer[0].aSax.GetSize() == 1 && fabs(m_AnevD.aLayer[0].aSax[0]) < dZero)
// 			{
// 				raNumJ.Add(m_AnevD.aLayer[0].aSax.GetSize());
// 
// 				return;
// 			}
// 		}

		for (int i = m_AnevD.aLayer.GetSize() - 1; i >= 0; i--)
		{
			if (m_AnevD.aLayer[i].aSax.GetSize() == 1 && fabs(m_AnevD.aLayer[i].aSax[0]) < dZero)
			{
				raNumJ.Add(m_AnevD.aLayer[i].aSax.GetSize());
			}
			else
			{
				raNumJ.Add(m_AnevD.aLayer[i].aSax.GetSize() + 1);
			}
		}
	}
// 	else
// 	{
// 		int nNumJ = 0;
// 
// 		if (m_AnevD.aSay.GetSize() == 1 && fabs(m_AnevD.aSay[0]) < dZero)
// 		{
// 			nNumI = 1;
// 		}
// 		else
// 		{
// 			nNumI = m_AnevD.aSay.GetSize() + 1;
// 		}
// 
// 		if (m_AnevD.aSax.GetSize() == 1 && fabs(m_AnevD.aSax[0]) < dZero)
// 		{
// 			nNumJ = 1;
// 		}
// 		else
// 		{
// 			nNumJ = m_AnevD.aSax.GetSize() + 1;
// 		}
// 
// 		nNumI = nNumI;
// 
// 		for (int i = 0; i < nNumI; i++)
// 		{
// 			raNumJ.Add(nNumJ);
// 		}
// 	}
}

void CDgnSeisAnchorShapeMgr::GetBearingNumberNew2(const T_ANEV_D& AnevD, int& nNumX, int& nNumY)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	int nSay = AnevD.aLayer.GetSize();

	int nSax = 0;
	int nSaxMax = 0;
	CArray<double, double> aSax, aSay;
	T_ANEV_LAYER_D LayerD;
	int iMax = 0;
	for (int i = 0; i < AnevD.aLayer.GetSize(); i++)
	{
		LayerD = AnevD.aLayer[i];

		nSax = LayerD.aSax.GetSize();
		if (nSax == 0 || (nSax == 1 && fabs(AnevD.aLayer[i].aSax[0]) < dZero))
		{
			nSax = 1;
		}
		else
		{
			nSax = AnevD.aLayer[i].aSax.GetSize() + 1;
		}

		if (nSaxMax < nSax)
		{
			nSaxMax = nSax;
			iMax = i;
		}
	}
	nNumX = nSaxMax;

	nNumY = 0;
	if (nSay == 0 || nSay == 1)
	{
		nNumY = 1;
	}
	else
	{
		nNumY = nSay;
	}
}

void CDgnSeisAnchorShapeMgr::GetAnchorNumber(int nBearingI, int nBearingJ, int& nNumI, int& nNumJ)
{
	nNumI = 0;
	nNumJ = 0;

	if(!m_bSetBrpr)
	{
		return;
	}

	nNumI = m_aArBrprD[nBearingI][nBearingJ].nN2;
	nNumJ = m_aArBrprD[nBearingI][nBearingJ].nN1;
}

T_BRPR_D CDgnSeisAnchorShapeMgr::GetAnchorProp(int nBearingI, int nBearingJ)
{
	T_BRPR_D Brpr; Brpr.Initialize();

	if (!m_bSetBrpr)
	{
		ASSERT(0);
		return Brpr;
	}

	Brpr = m_aArBrprD[nBearingI][nBearingJ];
	return Brpr;
}

void CDgnSeisAnchorShapeMgr::SetDULR(int nDu, int nLR)
{
	m_nDownUp = nDu;
	m_nLeftRight = nLR;
}

void CDgnSeisAnchorShapeMgr::SetPryoutDULR(int nDu, int nLR)
{
	m_nDownUpPryMgr = nDu;
	m_nLeftRightPryMgr = nLR;
}

BOOL CDgnSeisAnchorShapeMgr::IsSelectedBearing(int nIdxI, int nIdxJ, int nDir, int nFailType)
{	
	int nNewIdxI = m_AnevD.aLayer.GetSize() - 1 - nIdxI;
	
	if (nFailType == 0)			// Concrete Fail
	{
		if (nDir == 0)			// 교축 방향
		{
			if (m_AnevD.Fail.ConcFailL.aFailSelLayer[nNewIdxI].aFailSel[nIdxJ] == TRUE)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else if (nDir == 1)		// 교축 직각 방향
		{
			if (m_AnevD.Fail.ConcFailT.aFailSelLayer[nNewIdxI].aFailSel[nIdxJ] == TRUE)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	else if (nFailType == 1)		// Pryout Fail
	{
		if (nDir == 0)			// 교축 방향
		{
			if (m_AnevD.Fail.PryoutL.aFailSelLayer[nNewIdxI].aFailSel[nIdxJ] == TRUE)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else if (nDir == 1)		// 교축 직각 방향
		{
			if (m_AnevD.Fail.PryoutT.aFailSelLayer[nNewIdxI].aFailSel[nIdxJ] == TRUE)
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		ASSERT(0);
	}
	ASSERT(0);
	return TRUE;
}

void CDgnSeisAnchorShapeMgr::CalcRecalAnchorProjPosConcFail(int nDir, int nBearingI, int nBearingJ, int nAnchorI, int nAnchorJ, double dDistMin, double &dX1, double &dY1, double &dX2, double &dY2)
{
	if (m_aArSetCa1Longi.size() == 0)
	{
		AllocCa1Var();
	}

	double dAnchorX, dAnchorY;
	CalcAnchorPos(nBearingI, nBearingJ, nAnchorI, nAnchorJ, dAnchorX, dAnchorY);

	double dPos1[2], dPos2[2], dPos3[2];
	
	if (nDir == DIR_LONGI)
	{
		if (m_nDownUp == DIR_POS)
		{
			dPos1[0] = -1.5;
			dPos1[1] = -1;

			dPos2[0] = 1.5;
			dPos2[1] = -1;

			dPos3[0] = 0;
			dPos3[1] = -1;

		}
		else if (m_nDownUp == DIR_NEG)
		{
			dPos1[0] = 1.5;
			dPos1[1] = 1;

			dPos2[0] = -1.5;
			dPos2[1] = 1;

			dPos3[0] = 0;
			dPos3[1] = 1;
		}
		
	}
	else if (nDir == DIR_TRANS)
	{
		if (m_nLeftRight == DIR_POS)
		{
			dPos1[0] = -1;
			dPos1[1] = -1.5;

			dPos2[0] = -1;
			dPos2[1] = 1.5;

			dPos3[0] = -1;
			dPos3[1] = 0;
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			dPos1[0] = 1;
			dPos1[1] = 1.5;

			dPos2[0] = 1;
			dPos2[1] = -1.5;

			dPos3[0] = 1;
			dPos3[1] = 0;
		}
	}
	else
	{
		ASSERT(0);
	}

	math_lib::rotate_2d(dPos1[0], dPos1[1], -m_AnevD.aLayer[m_nLIdx - nBearingI].dTheta_q2);	
	math_lib::rotate_2d(dPos2[0], dPos2[1], -m_AnevD.aLayer[m_nLIdx - nBearingI].dTheta_q2);	
	math_lib::rotate_2d(dPos3[0], dPos3[1], -m_AnevD.aLayer[m_nLIdx - nBearingI].dTheta_q2);	


	dPos1[0] += dAnchorX;
	dPos1[1] += dAnchorY;
	dPos2[0] += dAnchorX;
	dPos2[1] += dAnchorY;
	dPos3[0] += dAnchorX;
	dPos3[1] += dAnchorY;


	/// 교점을 구해줘야함....
	CArray<double, double> aCopyingX, aCopyingY;
	GetCopyingShape(aCopyingX, aCopyingY);
	ASSERT(aCopyingX.GetSize() == aCopyingY.GetSize());
	int nSize;
	nSize = aCopyingX.GetSize();

	double dCroD[2], dInterD[2];
	dCroD[0] = dAnchorX;
	dCroD[1] = dAnchorY;
	dInterD[0] = dAnchorX;
	dInterD[1] = dAnchorY;

	double dTempDir[2];
	GetDir(dAnchorX, dAnchorY, dPos3[0], dPos3[1], dTempDir[0], dTempDir[1]);

	double dLongDist = m_dCopyingHeight + m_dCopyingWidth;
	PntMove(dTempDir[0], dTempDir[1], dLongDist, dCroD[0], dCroD[1]);

	BOOL bSet = FALSE;
	int nIdxNext;
	for (int i = 0; i < nSize; i++)
	{
		nIdxNext = i + 1;
		if (i == nSize - 1)
		{
			nIdxNext = 0;
		}
		if (GetLineInterSectPoint(dAnchorX, dAnchorY, dCroD[0], dCroD[1], aCopyingX[i], aCopyingY[i], aCopyingX[nIdxNext], aCopyingY[nIdxNext], dInterD[0], dInterD[1]))
		{
			bSet = TRUE;
			break;
		}
	}
	if (bSet == FALSE)
	{
		/*ASSERT(0);*/
		return;
	}
	
	double dCa1;
	dCa1 = math_lib::distance_2d(dAnchorX, dAnchorY, dInterD[0], dInterD[1]);

	/////////////////////////////////////////////
	// dCa1 재계산
	double dCa1Min = 1.0E6;
	double dZero = 0.0000001;
	std::vector<std::pair<int, int>> aRepBearingIdx;
	GetConcfailRepresentGrpBearing(nDir, aRepBearingIdx);

	double dEachCa1, dEachCa1P, dEachCa2, dEachCa2P;
	double dCa1Temp, dCa1Recal;
	dCa1Recal = 0.0;
	for (int i = 0; i < aRepBearingIdx.size(); i++)
	{
		if (!GetReCalcCa1(nDir, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dCa1Temp))
		{
			GetConcFailBearingCa(nDir, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dEachCa1, dEachCa1P, dEachCa2, dEachCa2P);
			dCa1Temp = dEachCa1;
		}
		dCa1Min = min(dCa1Min, dCa1Temp);

		dCa1Recal += dCa1Temp;
	}
	if (aRepBearingIdx.size() > 0)
	{
		dCa1Recal /= aRepBearingIdx.size();
	}
	if (fabs(dCa1Recal) < dZero) dCa1Min = 0.0;

	//////////////////////////////////////////////


	double dDir1[2], dDir2[2], dDir3[2];
	GetDir(dAnchorX, dAnchorY, dPos1[0], dPos1[1], dDir1[0], dDir1[1]);
	GetDir(dAnchorX, dAnchorY, dPos2[0], dPos2[1], dDir2[0], dDir2[1]);
	GetDir(dAnchorX, dAnchorY, dPos3[0], dPos3[1], dDir3[0], dDir3[1]);

	dLongDist = m_dCopyingHeight + m_dCopyingWidth;
	PntMove(dDir1[0], dDir1[1], dLongDist, dPos1[0], dPos1[1]);
	PntMove(dDir2[0], dDir2[1], dLongDist, dPos2[0], dPos2[1]);

	double dTempDist = dCa1 - dCa1Min;
	if (nDir == DIR_TRANS) dTempDist = dDistMin;
	PntMove(dDir3[0], dDir3[1], dTempDist, dPos1[0], dPos1[1]);
	PntMove(dDir3[0], dDir3[1], dTempDist, dPos2[0], dPos2[1]);
	PntMove(dDir3[0], dDir3[1], dTempDist, dAnchorX, dAnchorY);


	dX1 = dPos1[0];
	dY1 = dPos1[1];

	dX2 = dPos2[0];
	dY2 = dPos2[1];

	bSet = FALSE;
	for (int i = 0; i < nSize; i++)
	{
		nIdxNext = i + 1;
		if (i == nSize - 1)
		{
			nIdxNext = 0;
		}
		if (GetLineInterSectPoint(dAnchorX, dAnchorY, dPos1[0], dPos1[1], aCopyingX[i], aCopyingY[i], aCopyingX[nIdxNext], aCopyingY[nIdxNext], dX1, dY1))
		{
			bSet = TRUE;
			break;
		}
	}
	if (bSet == FALSE)
	{
		//ASSERT(0);
		return;
	}

	bSet = FALSE;
	for (int i = 0; i < nSize; i++)
	{
		nIdxNext = i + 1;
		if (i == nSize - 1)
		{
			nIdxNext = 0;
		}
		if (GetLineInterSectPoint(dAnchorX, dAnchorY, dPos2[0], dPos2[1], aCopyingX[i], aCopyingY[i], aCopyingX[nIdxNext], aCopyingY[nIdxNext], dX2, dY2))
		{
			bSet = TRUE;
			break;
		}
	}
	if (bSet == FALSE)
	{
		//ASSERT(0);
		return;
	}
}

void CDgnSeisAnchorShapeMgr::CalcRecalAnchorProjPosConcFailForDist(int nDir, int nBearingI, int nBearingJ, int nAnchorI, int nAnchorJ, double& dDist)
{
	if (m_aArSetCa1Longi.size() == 0)
	{
		AllocCa1Var();
	}

	double dAnchorX, dAnchorY;
	CalcAnchorPos(nBearingI, nBearingJ, nAnchorI, nAnchorJ, dAnchorX, dAnchorY);

	double dPos1[2], dPos2[2], dPos3[2];

	if (nDir == DIR_LONGI)
	{
		if (m_nDownUp == DIR_POS)
		{
			dPos1[0] = -1.5;
			dPos1[1] = -1;

			dPos2[0] = 1.5;
			dPos2[1] = -1;

			dPos3[0] = 0;
			dPos3[1] = -1;

		}
		else if (m_nDownUp == DIR_NEG)
		{
			dPos1[0] = 1.5;
			dPos1[1] = 1;

			dPos2[0] = -1.5;
			dPos2[1] = 1;

			dPos3[0] = 0;
			dPos3[1] = 1;
		}

	}
	else if (nDir == DIR_TRANS)
	{
		if (m_nLeftRight == DIR_POS)
		{
			dPos1[0] = -1;
			dPos1[1] = -1.5;

			dPos2[0] = -1;
			dPos2[1] = 1.5;

			dPos3[0] = -1;
			dPos3[1] = 0;
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			dPos1[0] = 1;
			dPos1[1] = 1.5;

			dPos2[0] = 1;
			dPos2[1] = -1.5;

			dPos3[0] = 1;
			dPos3[1] = 0;
		}
	}
	else
	{
		ASSERT(0);
	}

	math_lib::rotate_2d(dPos1[0], dPos1[1], -m_AnevD.aLayer[m_nLIdx - nBearingI].dTheta_q2);
	math_lib::rotate_2d(dPos2[0], dPos2[1], -m_AnevD.aLayer[m_nLIdx - nBearingI].dTheta_q2);
	math_lib::rotate_2d(dPos3[0], dPos3[1], -m_AnevD.aLayer[m_nLIdx - nBearingI].dTheta_q2);


	dPos1[0] += dAnchorX;
	dPos1[1] += dAnchorY;
	dPos2[0] += dAnchorX;
	dPos2[1] += dAnchorY;
	dPos3[0] += dAnchorX;
	dPos3[1] += dAnchorY;


	/// 교점을 구해줘야함....
	CArray<double, double> aCopyingX, aCopyingY;
	GetCopyingShape(aCopyingX, aCopyingY);
	ASSERT(aCopyingX.GetSize() == aCopyingY.GetSize());
	int nSize;
	nSize = aCopyingX.GetSize();

	double dCroD[2], dInterD[2];
	dCroD[0] = dAnchorX;
	dCroD[1] = dAnchorY;
	dInterD[0] = dAnchorX;
	dInterD[1] = dAnchorY;

	double dTempDir[2];
	GetDir(dAnchorX, dAnchorY, dPos3[0], dPos3[1], dTempDir[0], dTempDir[1]);

	double dLongDist = m_dCopyingHeight + m_dCopyingWidth;
	PntMove(dTempDir[0], dTempDir[1], dLongDist, dCroD[0], dCroD[1]);

	BOOL bSet = FALSE;
	int nIdxNext;
	for (int i = 0; i < nSize; i++)
	{
		nIdxNext = i + 1;
		if (i == nSize - 1)
		{
			nIdxNext = 0;
		}
		if (GetLineInterSectPoint(dAnchorX, dAnchorY, dCroD[0], dCroD[1], aCopyingX[i], aCopyingY[i], aCopyingX[nIdxNext], aCopyingY[nIdxNext], dInterD[0], dInterD[1]))
		{
			bSet = TRUE;
			break;
		}
	}
	if (bSet == FALSE)
	{
		/*ASSERT(0);*/
		return;
	}

	double dCa1;
	dCa1 = math_lib::distance_2d(dAnchorX, dAnchorY, dInterD[0], dInterD[1]);

	/////////////////////////////////////////////
	// dCa1 재계산
	double dCa1Min = 1.0E6;
	double dZero = 0.0000001;
	std::vector<std::pair<int, int>> aRepBearingIdx;
	GetConcfailRepresentGrpBearing(nDir, aRepBearingIdx);

	double dEachCa1, dEachCa1P, dEachCa2, dEachCa2P;
	double dCa1Temp, dCa1Recal;
	dCa1Recal = 0.0;
	for (int i = 0; i < aRepBearingIdx.size(); i++)
	{
		if (!GetReCalcCa1(nDir, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dCa1Temp))
		{
			GetConcFailBearingCa(nDir, aRepBearingIdx[i].first, aRepBearingIdx[i].second, dEachCa1, dEachCa1P, dEachCa2, dEachCa2P);
			dCa1Temp = dEachCa1;
		}
		dCa1Min = min(dCa1Min, dCa1Temp);

		dCa1Recal += dCa1Temp;
	}
	if (aRepBearingIdx.size() > 0)
	{
		dCa1Recal /= aRepBearingIdx.size();
	}
	if (fabs(dCa1Recal) < dZero) dCa1Min = 0.0;

	//////////////////////////////////////////////
	dDist = dCa1 - dCa1Min;
}


void CDgnSeisAnchorShapeMgr::GetConcFailBcRecal(int nDir, std::vector<TConcFailBcCalc>& aConcFailBc,
	BOOL FixedCond/* = FALSE*/, int nBi/* = 0*/, int nBj/* = 0*/)
{
	aConcFailBc.clear();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	double dZero = pDoc->m_pInitCtrl->GetPreferenceRegardZero();

	double CopingLine[2][2];

	CArray<double, double> aCopingX, aCopingY;
	GetCopyingShape(aCopingX, aCopingY);

	if (nDir == DIR_LONGI)
	{
		if (m_nDownUp == DIR_POS)
		{
			CopingLine[0][0] = aCopingX[0];
			CopingLine[0][1] = aCopingY[0];

			CopingLine[1][0] = aCopingX[3];
			CopingLine[1][1] = aCopingY[3];
		}
		else if (m_nDownUp == DIR_NEG)
		{
			CopingLine[0][0] = aCopingX[2];
			CopingLine[0][1] = aCopingY[2];

			CopingLine[1][0] = aCopingX[1];
			CopingLine[1][1] = aCopingY[1];
		}
	}
	else if (nDir == DIR_TRANS)
	{
		if (m_nLeftRight == DIR_POS)
		{
			CopingLine[0][0] = aCopingX[0];
			CopingLine[0][1] = aCopingY[0];

			CopingLine[1][0] = aCopingX[1];
			CopingLine[1][1] = aCopingY[1];
		}
		else if (m_nLeftRight == DIR_NEG)
		{
			CopingLine[0][0] = aCopingX[2];
			CopingLine[0][1] = aCopingY[2];

			CopingLine[1][0] = aCopingX[3];
			CopingLine[1][1] = aCopingY[3];
		}
	}
	else
	{
		ASSERT(0);
	}

	// Step1. 고정단/가동단을 고려해서 시작 선을 잡고... 다음 bearing 이랑 연결되면 선을 늘리고.. 연결 되지 않으면 aDataForMin 에 세팅
	CArray<int, int> aBearingI, aBearingJ, aAnchorI, aAnchorJ;
	GetPrjTgtIdxConcFail(nDir, aBearingI, aBearingJ, aAnchorI, aAnchorJ, FixedCond, nBi, nBj);

	if (aBearingI.GetSize() < 1 || aBearingJ.GetSize() < 1 || aAnchorI.GetSize() < 1 || aAnchorJ.GetSize() < 1)
	{
		return;
	}
	ASSERT(aBearingI.GetSize() == aBearingJ.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorI.GetSize());
	ASSERT(aBearingI.GetSize() == aAnchorJ.GetSize());

	BOOL bLastLoopMerge = FALSE;

	TConcFailBcCalc EachData;
	double AnchorPos[2], ProjPos1[2], ProjPos2[2], ProjPos1P[2], ProjPos2P[2];
	AnchorPos[0] = AnchorPos[1] = ProjPos1[0] = ProjPos1[1] = ProjPos2[0] = ProjPos2[1] = ProjPos1P[0] = ProjPos1P[1] = ProjPos2P[0] = ProjPos2P[1] = 0.0;

	int BearingIdx[2][2] = { 0, };
	double dSaxLeft, dSaxRight;

	BOOL bSetNext, bSetPrev;

	int nSize;
	nSize = aBearingI.GetSize();

	if (nSize == 1)
	{
		double dDistMin = 1.0E7;
		if (nDir == DIR_TRANS)
		{
			double dDist = 0.0;
			for (int i = 0; i < nSize; i++)
			{
				CalcRecalAnchorProjPosConcFailForDist(nDir, aBearingI[0], aBearingJ[0], aAnchorI[0], aAnchorJ[0], dDist);
				dDistMin = min(dDistMin, dDist);
			}
		}

		CalcAnchorPos(aBearingI[0], aBearingJ[0], aAnchorI[0], aAnchorJ[0], AnchorPos[0], AnchorPos[1]);
		CalcRecalAnchorProjPosConcFail(nDir, aBearingI[0], aBearingJ[0], aAnchorI[0], aAnchorJ[0], dDistMin, ProjPos1P[0], ProjPos1P[1], ProjPos2P[0], ProjPos2P[1]);
		if (!CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos1P[0], ProjPos1P[1], dZero))
		{
			EachData.LpLine[0][0] = CopingLine[0][0];
			EachData.LpLine[0][1] = CopingLine[0][1];
		}
		else
		{
			EachData.LpLine[0][0] = ProjPos1P[0];
			EachData.LpLine[0][1] = ProjPos1P[1];
		}

		if (!CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos2P[0], ProjPos2P[1], dZero))
		{
			EachData.LpLine[1][0] = CopingLine[1][0];
			EachData.LpLine[1][1] = CopingLine[1][1];
		}
		else
		{
			EachData.LpLine[1][0] = ProjPos2P[0];
			EachData.LpLine[1][1] = ProjPos2P[1];
		}
		EachData.nBc = 1;
		EachData.aBearingIdx.push_back(std::pair<int, int>(aBearingI[0], aBearingJ[0]));
		aConcFailBc.push_back(EachData);
	}
	else
	{
		double dDistMin = 1.0E7;
		if (nDir == DIR_TRANS)
		{
			double dDist = 0.0;
			for (int i = 0; i < nSize; i++)
			{
				CalcRecalAnchorProjPosConcFailForDist(nDir, aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], dDist);
				dDistMin = min(dDistMin, dDist);
			}
		}

		for (int i = 0; i < nSize - 1; i++)
		{
			CalcAnchorPos(aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], AnchorPos[0], AnchorPos[1]);
			CalcRecalAnchorProjPosConcFail(nDir, aBearingI[i], aBearingJ[i], aAnchorI[i], aAnchorJ[i], dDistMin, ProjPos1P[0], ProjPos1P[1], ProjPos2P[0], ProjPos2P[1]);
			if (!CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos1P[0], ProjPos1P[1], dZero))
			{
				EachData.LpLine[0][0] = CopingLine[0][0];
				EachData.LpLine[0][1] = CopingLine[0][1];
			}
			else
			{
				EachData.LpLine[0][0] = ProjPos1P[0];
				EachData.LpLine[0][1] = ProjPos1P[1];
			}

			if (!CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos2P[0], ProjPos2P[1], dZero))
			{
				EachData.LpLine[1][0] = CopingLine[1][0];
				EachData.LpLine[1][1] = CopingLine[1][1];
			}
			else
			{
				EachData.LpLine[1][0] = ProjPos2P[0];
				EachData.LpLine[1][1] = ProjPos2P[1];
			}
			EachData.nBc = 1;
			EachData.aBearingIdx.push_back(std::pair<int, int>(aBearingI[i], aBearingJ[i]));

			bSetNext = FALSE;
			//if(i < nSize - 2 && (aBearingI[i] != aBearingI[i + 1] || aBearingJ[i] != aBearingJ[i + 1]))
			if (i < nSize - 1 && (aBearingI[i] != aBearingI[i + 1] || aBearingJ[i] != aBearingJ[i + 1]))
			{
				bSetNext = TRUE;
			}
			bSetPrev = FALSE;
			if (i != 0 && (aBearingI[i] != aBearingI[i - 1] || aBearingJ[i] != aBearingJ[i - 1]))
			{
				bSetPrev = TRUE;
			}
			if (bSetNext)
			{
				BearingIdx[0][0] = aBearingI[i];
				BearingIdx[0][1] = aBearingJ[i];
				BearingIdx[1][0] = aBearingI[i + 1];
				BearingIdx[1][1] = aBearingJ[i + 1];
				dSaxRight = GetBearingGap(nDir, BearingIdx);
			}
			if (bSetPrev)
			{
				BearingIdx[0][0] = aBearingI[i];
				BearingIdx[0][1] = aBearingJ[i];
				BearingIdx[1][0] = aBearingI[i - 1];
				BearingIdx[1][1] = aBearingJ[i - 1];
				dSaxLeft = GetBearingGap(nDir, BearingIdx);
			}
			if (bSetNext && bSetPrev)
			{
				EachData.dSax = min(dSaxLeft, dSaxRight);
			}
			else if (bSetNext)
			{
				EachData.dSax = dSaxRight;
			}
			else if (bSetPrev)
			{
				EachData.dSax = dSaxLeft;
			}

			int j = 0;
			for (j = i + 1; j < nSize; j++)
			{
				CalcRecalAnchorProjPosConcFail(nDir, aBearingI[j], aBearingJ[j], aAnchorI[j], aAnchorJ[j], dDistMin, ProjPos1P[0], ProjPos1P[1], ProjPos2P[0], ProjPos2P[1]);
				BOOL bCheck01 = CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos1P[0], ProjPos1P[1], dZero);
				BOOL bCheck02;
				if (nDir == 0 && EachData.LpLine[0][0] == ProjPos1P[0])
				{
					bCheck02 = TRUE;
				}
				else if (nDir == 0 && EachData.LpLine[0][0] != ProjPos1P[0])
				{
					bCheck02 = SetLineSmallerAndCheckInclude(nDir, EachData.LpLine[0][0], EachData.LpLine[0][1], EachData.LpLine[1][0], EachData.LpLine[1][1], ProjPos1P[0], ProjPos1P[1], dZero);
				}
				else if (nDir == 1 && EachData.LpLine[0][1] == ProjPos1P[1])
				{
					bCheck02 = TRUE;
				}
				else if (nDir == 1 && EachData.LpLine[0][1] != ProjPos1P[1])
				{
					bCheck02 = SetLineSmallerAndCheckInclude(nDir, EachData.LpLine[0][0], EachData.LpLine[0][1], EachData.LpLine[1][0], EachData.LpLine[1][1], ProjPos1P[0], ProjPos1P[1], dZero);
				}
				else
				{
					ASSERT(0);
					bCheck02 = FALSE;
				}

				if (!bCheck01 || bCheck02)
				{
					// Lp 를 연장하는 경우
					if (CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos2P[0], ProjPos2P[1], dZero))
					{
						// coping 의 끝부분에 부딧치지 않은 경우
						EachData.LpLine[1][0] = ProjPos2P[0];
						EachData.LpLine[1][1] = ProjPos2P[1];
					}
					else
					{
						// coping 의 끝부분에 부딧친 경우
						EachData.LpLine[1][0] = CopingLine[1][0];
						EachData.LpLine[1][1] = CopingLine[1][1];
					}
					if (aBearingI[j] != aBearingI[j - 1] || aBearingJ[j] != aBearingJ[j - 1]) // Bc 다음 Bearing 으로 넘어 간 경우에만 증가시킴
					{
						EachData.nBc++;
						EachData.aBearingIdx.push_back(std::pair<int, int>(aBearingI[j], aBearingJ[j]));
					}

					bSetNext = FALSE;
					if (j < nSize - 2 && (aBearingI[j] != aBearingI[j + 1] || aBearingJ[j] != aBearingJ[j + 1]))
					{
						bSetNext = TRUE;
					}
					if (bSetNext)
					{
						BearingIdx[0][0] = aBearingI[j];
						BearingIdx[0][1] = aBearingJ[j];
						BearingIdx[1][0] = aBearingI[j + 1];
						BearingIdx[1][1] = aBearingJ[j + 1];
						dSaxRight = GetBearingGap(nDir, BearingIdx);
						EachData.dSax = EachData.dSax == 0.0 ? dSaxRight : min(EachData.dSax, dSaxRight);
					}
					bLastLoopMerge = TRUE;
				}
				else
				{
					ASSERT(EachData.nBc != 0);

					aConcFailBc.push_back(EachData);
					EachData.Init();
					bLastLoopMerge = FALSE;

					EachData.LpLine[0][0] = ProjPos1P[0];
					EachData.LpLine[0][1] = ProjPos1P[1];
					EachData.LpLine[1][0] = ProjPos2P[0];
					EachData.LpLine[1][1] = ProjPos2P[1];



					BearingIdx[0][0] = aBearingI[j - 1];
					BearingIdx[0][1] = aBearingJ[j - 1];
					BearingIdx[1][0] = aBearingI[j];
					BearingIdx[1][1] = aBearingJ[j];


					EachData.dSax = GetBearingGap(nDir, BearingIdx);

					EachData.nBc = 1;
					EachData.aBearingIdx.push_back(std::pair<int, int>(aBearingI[j], aBearingJ[j]));
				}
			}
			i = j - 1;
		}
		if (bLastLoopMerge)
		{
			aConcFailBc.push_back(EachData);
		}
		else
		{
			EachData.Init();
			EachData.nBc = 1;
			EachData.LpLine[0][0] = ProjPos1P[0];
			EachData.LpLine[0][1] = ProjPos1P[1];
			if (CMathFunc::mathIncludePointInLine(CopingLine[0][0], CopingLine[0][1], CopingLine[1][0], CopingLine[1][1], ProjPos2P[0], ProjPos2P[1], dZero))
			{
				// coping 의 끝부분에 부딧치지 않은 경우
				EachData.LpLine[1][0] = ProjPos2P[0];
				EachData.LpLine[1][1] = ProjPos2P[1];
			}
			else
			{
				// coping 의 끝부분에 부딧친 경우
				EachData.LpLine[1][0] = CopingLine[1][0];
				EachData.LpLine[1][1] = CopingLine[1][1];
			}

			bSetPrev = FALSE;

			int nIdxL, nIdxR;
			nIdxL = (nSize - 1 >= 0) ? nSize - 1 : 0;
			nIdxR = (nSize - 2 >= 0) ? nSize - 2 : 0;

			if (aBearingI[nIdxL] != aBearingI[nIdxR] || aBearingJ[nIdxL] != aBearingJ[nIdxR])
			{
				bSetPrev = TRUE;
			}
			if (bSetPrev)
			{
				BearingIdx[0][0] = aBearingI[nIdxL];
				BearingIdx[0][1] = aBearingJ[nIdxL];
				BearingIdx[1][0] = aBearingI[nIdxR];
				BearingIdx[1][1] = aBearingJ[nIdxR];
				dSaxLeft = GetBearingGap(nDir, BearingIdx);
				EachData.dSax = dSaxLeft;
			}
			EachData.aBearingIdx.push_back(std::pair<int, int>(BearingIdx[0][0], BearingIdx[0][1]));
			aConcFailBc.push_back(EachData);
		}
	}

	// Lp계산
	nSize = (int)aConcFailBc.size();
	for (int i = 0; i < nSize; i++)
	{
		aConcFailBc[i].dLp = math_lib::distance_2d(aConcFailBc[i].LpLine[0][0], aConcFailBc[i].LpLine[0][1], aConcFailBc[i].LpLine[1][0], aConcFailBc[i].LpLine[1][1]);
	}
}


void CDgnSeisAnchorShapeMgr::GetConcFailBcRecal(int nDir, TConcFailBcCalc& MinConcFailBc, BOOL bFixedCond)
{
	MinConcFailBc.Init();

	std::vector<TConcFailBcCalc> aConcFailBc;
	GetConcFailBcRecal(nDir, aConcFailBc, bFixedCond);
	if (aConcFailBc.size() == 0)
	{
		return;
	}
	//double dMinLp = DBL_MAX;
	//int nMaxBc = 0;

	GetMinConcFailBc(nDir, aConcFailBc, MinConcFailBc);
}