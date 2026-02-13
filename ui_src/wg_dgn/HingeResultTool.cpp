#include "stdafx.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\PoverInfo.h"
#include "..\wg_db\ThisInfo.h"

#include "HingeResultTool.h"

enum EN_LOADTYPE_TH
{
	EN_LOADTYPE_NORMAL = 0,
	EN_LOADTYPE_MAX,
	EN_LOADTYPE_MIN,
	EN_LOADTYPE_ALL
};

CHingeResultTool::CHingeResultTool(void)
{
	m_pDoc = CDBDoc::GetDocPoint();
	if ( m_pDoc == nullptr ) { ASSERT(0); return; }
}

CHingeResultTool::~CHingeResultTool()
{

}

double CHingeResultTool::GetPOHingeThetamaxAbsMax(T_ELEM_K ElemK, int nDOF, T_POLC_K PolcK, int nStep, int nAddStepIdx)
{
	T_INELASTICDYN_HINGE_INFO_BASE InelstHngeInfoBase;
	std::vector<int> vSeqId;
	if ( !GetPOInelstHngeInfo(ElemK, nDOF, InelstHngeInfoBase, vSeqId) ) { return 0.0; }

	if (nAddStepIdx >= 0)
	{
		T_POAS_D PoasD;
		m_pDoc->m_pAttrCtrl->GetPoas(PoasD);
		nStep = PoasD.aStep[nAddStepIdx].nRefStep - 1;
	}
	const int nStepApp = nStep;
	const int nHngeType = InelstHngeInfoBase.nHingeType + 1;
	double dThetamax = 0.0;
	for ( auto i : vSeqId )
	{
		T_FHNE_D FhneD;
		if ( !m_pDoc->m_pPostCtrl->GetPhnePushover(PolcK, nStepApp, i, nHngeType, FhneD, nDOF) )
		{
			ASSERT(0);
			continue;
		}
		dThetamax = max(dThetamax, fabs(FhneD.dDisplacement));
	}

	return dThetamax;
}

double CHingeResultTool::GetTHHingeThetamaxAbsMax(T_ELEM_K ElemK, int nDOF, T_THIS_K ThisK, int nHingeTimeInex)
{
	T_INELASTICDYN_HINGE_INFO_BASE InelstHngeInfoBase;
	std::vector<int> vSeqId;
	if ( !GetTHInelstHngeInfo(ElemK, nDOF, InelstHngeInfoBase, vSeqId) ) { return 0.0; }
	
	double dThetamax = 0.0;
	for ( auto i : vSeqId )
	{
		T_NLHG_D NlhgD;
		if ( !GetTHHingeResult(ThisK, nHingeTimeInex, i, InelstHngeInfoBase.nHingeType+1, nDOF, NlhgD) ) { continue; }		
		dThetamax = max(dThetamax, fabs(NlhgD.dDisplacement));
	}
	
	return dThetamax;
}

bool CHingeResultTool::GetPOHingeThetamaxByPos(T_ELEM_K ElemK, int nDOF, T_POLC_K PolcK, int nStep, int nAddStepIdx, OUT double radThetamax[2])
{
	memset(radThetamax, 0.0, sizeof(double)*2);		

	T_INELASTICDYN_HINGE_INFO_BASE InelstHngeInfoBase;
	std::vector<int> vSeqId;
	if ( !GetPOInelstHngeInfo(ElemK, nDOF, InelstHngeInfoBase, vSeqId) ) { return false; }	

	if (nAddStepIdx >= 0)
	{
		T_POAS_D PoasD;
		m_pDoc->m_pAttrCtrl->GetPoas(PoasD);
		nStep = PoasD.aStep[nAddStepIdx].nRefStep - 1;
	}
	const int nStepApp = nStep;	
	const int nHngeType = InelstHngeInfoBase.nHingeType + 1;

	int anSeqId[2] = { vSeqId.at(0), vSeqId.at(vSeqId.size()-1) };
	for ( int i = 0; i < 2; ++i )
	{
		T_FHNE_D FhneD;
		if ( !m_pDoc->m_pPostCtrl->GetPhnePushover(PolcK, nStepApp, anSeqId[i], nHngeType, FhneD, nDOF) ) { continue; }
		radThetamax[i] = fabs(FhneD.dDisplacement);
	}

	return true;
}

bool CHingeResultTool::GetTHHingeThetamaxByPos(T_ELEM_K ElemK, int nDOF, T_THIS_K ThisK, int nHingeTimeInex, OUT double radThetamax[2])
{
	memset(radThetamax, 0.0, sizeof(double)*2);

	T_INELASTICDYN_HINGE_INFO_BASE InelstHngeInfoBase;
	std::vector<int> vSeqId;
	if ( !GetTHInelstHngeInfo(ElemK, nDOF, InelstHngeInfoBase, vSeqId) ) { return false; }

	const int nHngeType = InelstHngeInfoBase.nHingeType + 1;

	int anSeqId[2] = { vSeqId.at(0), vSeqId.at(vSeqId.size()-1)};	
	for ( int i = 0; i < 2; ++i )
	{
		T_NLHG_D NlhgD;
		if ( !GetTHHingeResult(ThisK, nHingeTimeInex, anSeqId[i], nHngeType, nDOF, NlhgD) ) { continue; }
		radThetamax[i] = fabs(NlhgD.dDisplacement);
	}

	return true;
}

bool CHingeResultTool::GetPOInelstHngeInfo(T_ELEM_K ElemK, int nDOF, OUT T_INELASTICDYN_HINGE_INFO_BASE& rHngeInfoBase, OUT std::vector<int>& rvSeqId)
{
	rHngeInfoBase.Initialize();
	rvSeqId.clear();

	T_ELEM_D ElemD;
	if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) )
	{
		ASSERT(0);
		return false;
	}

	if ( ElemD.eltyp != BEAM_EL && ElemD.eltyp != WALL_EL ) { return false; }

	T_INELASTICDYN_HINGE_INFO HngeInfo;
	if ( !m_pDoc->m_pPostCtrl->GetPoverInfo()->GetInelasticDynHngtDataElem(ElemK, HngeInfo) )
	{
		return false;
	}

	if ( !GetInelstHngeInfoBase(HngeInfo, nDOF, rHngeInfoBase) ) { return false; }
	return GetPOHingeSeqId(rHngeInfoBase, rvSeqId);
}

bool CHingeResultTool::GetTHInelstHngeInfo(T_ELEM_K ElemK, int nDOF, OUT T_INELASTICDYN_HINGE_INFO_BASE& rHngeInfoBase, OUT std::vector<int>& rvSeqId)
{
	rHngeInfoBase.Initialize();
	rvSeqId.clear();

	T_ELEM_D ElemD;
	if ( !m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD) )
	{
		ASSERT(0);
		return false;
	}

	if ( ElemD.eltyp != BEAM_EL && ElemD.eltyp != WALL_EL ) { return false; }

	T_INELASTICDYN_HINGE_INFO HngeInfo;
	if ( !m_pDoc->m_pPostCtrl->GetThisInfo()->GetInelasticDynHngtDataElem(ElemK, HngeInfo) )
	{
		return false;
	}

	if ( !GetInelstHngeInfoBase(HngeInfo, nDOF, rHngeInfoBase) ) { return false; }
	switch ( HngeInfo.nElemType )
	{
	case INELASTIC_ELEM_TYPE_BEAM:
		return GetTHHingeSeqIdElemBeam(rHngeInfoBase, rvSeqId);
	case INELASTIC_ELEM_TYPE_WALL:
		return GetTHHingeSeqIdElemWall(rHngeInfoBase, rvSeqId);
	default:
		ASSERT(0);
		return false;
	}
}

bool CHingeResultTool::GetInelstHngeInfoBase(const T_INELASTICDYN_HINGE_INFO& HngeInfo, int nDOF, OUT T_INELASTICDYN_HINGE_INFO_BASE& rHngeInfoBase)
{
	const INT_PTR nSize = HngeInfo.aInelasticDynHingeInfo.GetSize();
	for ( INT_PTR i=0; i<nSize; ++i )
	{
		if ( nDOF == HngeInfo.aInelasticDynHingeInfo[i].nHingeDirection )
		{
			rHngeInfoBase = HngeInfo.aInelasticDynHingeInfo[i];
			return true;
		}
	}
	return false;
}

bool CHingeResultTool::GetPOHingeSeqId(const T_INELASTICDYN_HINGE_INFO_BASE& HngeInfoBase, OUT std::vector<int>& rvSeqId)
{
	rvSeqId.clear();

	const int nSeqCnt = [HngeInfoBase] ()
	{
		if ( HngeInfoBase.bLumpType )
		{
			switch ( HngeInfoBase.nHingeLocation )
			{
			case D_PHGT_LOCATION_CENTER:
			case D_PHGT_LOCATION_I:
			case D_PHGT_LOCATION_J:
				return 0;
			case D_PHGT_LOCATION_IJ:
				return 1;
			default:
				ASSERT(0);
				return 0;
			}			
		}
		else
		{
			return HngeInfoBase.nNumHinge - 1;
		}
	}();	
	
	for ( int i=0; i<=nSeqCnt; ++i )
	{
		rvSeqId.push_back(HngeInfoBase.nStartHingeId + i);
	}

	return rvSeqId.size() < 1 ? false : true;
}

bool CHingeResultTool::GetTHHingeSeqIdElemBeam(const T_INELASTICDYN_HINGE_INFO_BASE& HngeInfoBase, OUT std::vector<int>& rvSeqId)
{
	rvSeqId.clear();

	for ( int i=0; i<HngeInfoBase.nNumHinge; ++i )
	{
		rvSeqId.push_back(HngeInfoBase.nStartHingeId + i);
	}

	return rvSeqId.size() < 1 ? false : true;
}

bool CHingeResultTool::GetTHHingeSeqIdElemWall(const T_INELASTICDYN_HINGE_INFO_BASE& HngeInfoBase, OUT std::vector<int>& rvSeqId)
{
	/// refer follwing function : BOOL CThisInfo::GetInelastHngeResultWall(
	rvSeqId.clear();

	if ( HngeInfoBase.nHingeDirection == 0 )
	{
		rvSeqId.push_back(HngeInfoBase.nStartHingeId + 1);
	}
	else
	{
		for ( int i=0; i<HngeInfoBase.nNumHinge; ++i )
		{
			rvSeqId.push_back(HngeInfoBase.nStartHingeId + i);
		}
	}

	return rvSeqId.size() < 1 ? false : true;
}

bool CHingeResultTool::GetTHHingeResult(T_THIS_K ThisK, int nHingeTimeInex, int nSeqId, int nHgsmHingeType, int nDOF, OUT T_NLHG_D& rNlhgD)
{
	const UINT nMaxMinAbs = [&, ThisK, nHingeTimeInex] ()
	{
		CArray<double, double> aTimeStep;
		m_pDoc->m_pAttrCtrl->GetTimeStep(ThisK, aTimeStep);
		const int nTimeStepCount = aTimeStep.GetSize();
		if ( nTimeStepCount <= nHingeTimeInex )
		{
			if ( nHingeTimeInex == nTimeStepCount )
			{
				return EN_LOADTYPE_ALL;
			}
			else if ( nHingeTimeInex == nTimeStepCount+1 )
			{
				return EN_LOADTYPE_MAX;
			}
			else if ( nHingeTimeInex == nTimeStepCount+2 )
			{
				return EN_LOADTYPE_MIN;
			}
		}
		return EN_LOADTYPE_NORMAL;
	}();

	const BOOL bRotateDof = (nDOF >= 3) ? TRUE : FALSE;	
	BOOL bForceSign = FALSE;	
	switch ( nMaxMinAbs )
	{
	case EN_LOADTYPE_NORMAL:
		return 	m_pDoc->m_pPostCtrl->GetNlhgHistory(nHingeTimeInex, nSeqId, nHgsmHingeType, bRotateDof, rNlhgD, nDOF) ? true : false;
	case EN_LOADTYPE_MAX:
		return m_pDoc->m_pPostCtrl->GetHgsm(nSeqId, nHgsmHingeType, bRotateDof, &rNlhgD, NULL, NULL, NULL, NULL, NULL, NULL, bForceSign, nDOF) ? true : false;
	case EN_LOADTYPE_MIN:
		return m_pDoc->m_pPostCtrl->GetHgsm(nSeqId, nHgsmHingeType, bRotateDof, NULL, &rNlhgD, NULL, NULL, NULL, NULL, NULL, bForceSign, nDOF) ? true : false;
	case EN_LOADTYPE_ALL:
		return m_pDoc->m_pPostCtrl->GetHgsm(nSeqId, nHgsmHingeType, bRotateDof, NULL, NULL, &rNlhgD, NULL, NULL, NULL, NULL, bForceSign, nDOF) ? true : false;
	default: ASSERT(0);
		return false;
	}
}