#include "stdafx.h"
#include "SeismicDesignBase.h"

#include "..\wg_base\wg_base_NumericOptimizer.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ThisInfo.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#pragma region // CSeismicDesignBase
CSeismicDesignBase::CSeismicDesignBase()
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	m_pProgress = nullptr;
	m_mMemberMap.clear();
}

CSeismicDesignBase::~CSeismicDesignBase()
{
}

void CSeismicDesignBase::GetStrKeyList(const CArray<unsigned int, unsigned int>& keyList, OUT CString& rStr)
{
	long nSize = keyList.GetSize();
	if (nSize < 1)
		return;

	long* aNum = new long[nSize];
	for (int i = 0; i < nSize; i++)
		aNum[i] = keyList[i];
	qsort((void*)aNum, nSize, sizeof(long), CNumericOptimizer::comparei);

	CNumericOptimizer optimizer;
	rStr = optimizer.Optimize(aNum, nSize);

	delete[]aNum;
}

BOOL CSeismicDesignBase::GetTimeIndex(const T_THIS_K& kThis, const double& nStep, OUT int& nTimeIndex)
{
#if _DEBUG
	T_THIS_D dThis;
	m_pDoc->m_pAttrCtrl->GetThis(kThis, dThis);
#endif
	//CGPSTHCalcDispFunc::TextoutTHistResult Âü°í
	CArray<double, double> arTimeStep;
	m_pDoc->m_pAttrCtrl->GetTimeStep(kThis, arTimeStep);

	double dTimeStart = nStep;
	double dTimeEnd = dTimeStart + 0.1;

	int nTimeCount = arTimeStep.GetCount();
	int nTimeStart = 0;
	int nTimeEnd = nTimeCount;
	BOOL bFoundStart = FALSE;

	if (dTimeStart < 0 || dTimeEnd < dTimeStart)
		return FALSE;

	for (int nIdx = 0; nIdx <= nTimeCount - 1; nIdx++)
	{
		if (dTimeStart > arTimeStep[nIdx]) continue;
		else
		{
			if (!bFoundStart)
			{
				nTimeStart = nIdx;
				bFoundStart = TRUE;
			}

			if (dTimeEnd > arTimeStep[nIdx])
				continue;
			else
			{
				nTimeEnd = nIdx;
				break;
			}
		}
	}

	nTimeIndex = nTimeStart;
	return bFoundStart;
}

int CSeismicDesignBase::GetCurrentDgnCode()
{
	if (m_pDoc == NULL) return 0;

	T_SIDC_D dSidc;
	dSidc.Initialize();
	if (m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(dSidc) == FALSE) return 0;

	return dSidc.nDesignCode;
}

void CSeismicDesignBase::MakeMemberMap(const T_SIGR_K& kSigr, OUT T_ELEM_K_LIST& rElemList)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == nullptr) return;

	T_SIGR_D dSigr;
	if (pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE)
		return;

	m_mMemberMap.clear();

	auto L_SetMemberData = [&](const T_ELEM_K& kElem, const DB_MEMB_TYPE& eType, const int& nDir, const int& nMat)
		{
			rElemList.Add(kElem);

			P_Member memb;
			memb.Init();
			memb.nType = eType;
			memb.nDirShear = (seis_jp::eDirShear)nDir;
			memb.nMaterialType = nMat;
			m_mMemberMap.insert(std::make_pair(kElem, memb));
		};

	switch (dSigr.nSubType)
	{
	case T_SIGR_D::kLower:
	{
		T_SIGR_LOWER_D dLower;
		if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, dLower) == FALSE) return;

		for (int i = 0; i < dLower.COLM_D.GetCount(); i++)
		{
			const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[i];
			for (int j = 0; j < colm.aInvestigatedElem.GetCount(); j++)
				L_SetMemberData(colm.aInvestigatedElem[j], D_MBTP_COLUMN, colm.nDirShear, dLower.nMaterialType);
		}
		for (int i = 0; i < dLower.BEAM_D.GetCount(); i++)
		{
			const T_SIGR_LOWER_BEAM_D& beam = dLower.BEAM_D[i];
			for (int j = 0; j < beam.aInvestigatedElem.GetCount(); j++)
				L_SetMemberData(beam.aInvestigatedElem[j], D_MBTP_BEAM, beam.nDirShear, dLower.nMaterialType);
		}
	}
	break;
	case T_SIGR_D::kUpper:
	{
		T_SIGR_UPPER_D dUpper;
		pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(kSigr, dUpper);
		for (int i = 0; i < dUpper.aElements.GetCount(); i++)
			L_SetMemberData(dUpper.aElements[i], D_MBTP_ETC, dUpper.nDirShear, T_SIGR_LOWER_D::kUnKnown);
	}
	break;
	case T_SIGR_D::kUser:
	{
		T_SIGR_USER_D dUser;
		pDoc->m_pAttrCtrl2->GetQSigrUser()->Get(kSigr, dUser);
		for (int i = 0; i < dUser.aInvestigatedElem.GetCount(); i++)
			L_SetMemberData(dUser.aInvestigatedElem[i], D_MBTP_ETC, dUser.nDirShear, T_SIGR_LOWER_D::kUnKnown);
	}
	break;
	default:
		ASSERT(FALSE);
		break;
	}
}

#pragma endregion