#include "StdAfx.h"

#include "DgnTendonElemLengthCtrl.h"

#include "..\wg_db\MathFunc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CDgnTendonElemLengthCtrl::CDgnTendonElemLengthCtrl(CAttrCtrl* pAttrCtrl)
{
	m_vecInfo_ElemOffset.clear();
	m_vecInfo_Centroid.clear();
	m_pAttrCtrl = pAttrCtrl;  
	m_pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
}


CDgnTendonElemLengthCtrl::~CDgnTendonElemLengthCtrl(void)
{
}

double CDgnTendonElemLengthCtrl::SetCalcLengthInfo( const vecElemKey& elemKey, BOOL bCentroid )
{
	double dTotalLength = 0.0;
	double dLength;
	
	UINT i;
	UINT nSize = (UINT)elemKey.size();
	
	if(bCentroid)
	{		
		m_vecInfo_Centroid.clear();
		m_vecInfo_Centroid.resize(nSize);
		
		for(i = 0 ; i < nSize; i++)
		{		
			if(!GetLengInfoByElem(elemKey[i], &m_vecInfo_Centroid[i].infoEndI, &m_vecInfo_Centroid[i].infoEndJ, bCentroid))
			{
				return -1.0;
			}
			
			dLength = m_vecInfo_Centroid[i].infoEndJ.dPosX;		
			m_vecInfo_Centroid[i].nElemKey = elemKey[i];
			m_vecInfo_Centroid[i].infoEndI.dPosX = dTotalLength;
			m_vecInfo_Centroid[i].infoEndJ.dPosX = dTotalLength + dLength;
			dTotalLength += dLength;
		}
	}
	else
	{
		m_vecInfo_ElemOffset.clear();
		m_vecInfo_ElemOffset.resize(nSize);
		
		for(i = 0 ; i < nSize; i++)
		{		
			if(!GetLengInfoByElem(elemKey[i], &m_vecInfo_ElemOffset[i].infoEndI, &m_vecInfo_ElemOffset[i].infoEndJ, bCentroid))
			{
				return -1.0;
			}
			
			dLength = m_vecInfo_ElemOffset[i].infoEndJ.dPosX;	
			m_vecInfo_ElemOffset[i].nElemKey = elemKey[i];	
			m_vecInfo_ElemOffset[i].infoEndI.dPosX = dTotalLength;
			m_vecInfo_ElemOffset[i].infoEndJ.dPosX = dTotalLength + dLength;
			dTotalLength += dLength;
		}
	}
	
	return dTotalLength;	
}

BOOL CDgnTendonElemLengthCtrl::SetElemList(vecElemKey& elemKey, BOOL& bAllSameSect, T_SECT_K& key)
{
	if(!AlignElemList(elemKey))	//정렬한다.
	{
		return FALSE;
	}

	m_dTotalLength = 0.0;
	
	bAllSameSect = IsAllSameSect(elemKey, key);

	double dLengthElem = SetCalcLengthInfo(elemKey, FALSE);
	double dLengthCent = SetCalcLengthInfo(elemKey, TRUE);

	//단면의 정보가 없거나 이상하면.
	if(dLengthCent < 0.0 || dLengthElem < 0.0)
		return FALSE;

	//두종류의 길이가 다르다면.
	if( fabs(dLengthCent - dLengthElem) >= 1E-4)
		return FALSE;

	m_dTotalLength = dLengthCent;
	return TRUE;
}

void CDgnTendonElemLengthCtrl::ExcludeDuplicate( vecElemKey& elemKey )
{
	std::map<T_ELEM_K, int> mapElemKey;
	std::map<T_ELEM_K, int>::iterator itr;
	std::pair<T_ELEM_K, int> pairElem;
	
	int i, NumNode;
	T_ELEM_D eData;
	
	for(i = 0 ; i < elemKey.size(); i++)
	{
		BOOL bExist = m_pAttrCtrl->GetElemFromBaseDB(elemKey[i], eData);
		if(!bExist)
			continue;

		NumNode = m_pAttrCtrl->GetNumNode(eData.eltyp, eData.elnod);

		if(NumNode != 2) //일단 노드가 2개인것만 사용하자.
			continue;

		itr = mapElemKey.find(elemKey[i]);
		if(itr == mapElemKey.end())
		{	//중복아니면 추가.
			pairElem.first =elemKey[i];
			pairElem.second = 1;
			mapElemKey.insert(pairElem);// .insert(elemKey[i], 1);
		}
		else
		{	//중복되는 요소 발견.
			ASSERT(0);
		}
	}

	elemKey.clear();

	for(itr = mapElemKey.begin(); itr != mapElemKey.end(); itr++)
	{
		elemKey.push_back(itr->first);
	}
}

BOOL CDgnTendonElemLengthCtrl::AlignElemList( vecElemKey& elemKey )
{
	if(elemKey.size() <= 0)
	{
		return FALSE;
	}

	ExcludeDuplicate(elemKey);//겹치는 요소가 있다면 제거.

	int i, j, NumNode;
	int nElem = (int)elemKey.size();
	BOOL bSuccess = FALSE;
 	T_ELEM_D eData;

	if(nElem <= 0) 	return FALSE;

	T_ELEM_K itrElem;
	std::map<T_NODE_K, T_ELEM_K>			mapNodeiElem;
	std::map<T_NODE_K, T_ELEM_K>::iterator	itrNodeElem;
	std::map<T_NODE_K, int>					mapInnerNode;
	std::map<T_NODE_K, int>::iterator		itrInnerNode;

	mapNodeiElem.clear();
	mapInnerNode.clear();

	int nMaxNum = nElem * 2;

	if(nMaxNum <= 0)
	{
		return FALSE;
	}

	T_NODE_K *nNodeIndex = new T_NODE_K[nMaxNum];   

	for(i = 0;  i < nMaxNum; i++)
	{
		nNodeIndex[i] = 0;    
	}

	for(i = 0 ; i < nElem; i++)
	{
		BOOL bExist = m_pAttrCtrl->GetElemFromBaseDB(elemKey[i], eData);
 		NumNode = m_pAttrCtrl->GetNumNode(eData.eltyp, eData.elnod);

		if(NumNode != 2)
		{
			delete []nNodeIndex; nNodeIndex=NULL;
			return FALSE;
		}

		nNodeIndex[2 * i]		= eData.elnod[0];
		nNodeIndex[2 * i + 1]	= eData.elnod[1];
		
		mapNodeiElem[eData.elnod[0]] = elemKey[i];
	}

	int nNumNode = (int)(2 * elemKey.size());

	for(i = 0; i < nNumNode; i++)    // 현재 Member의 내부 절점을 구한다.
	{
		for(j = i; j < nNumNode; j++)
		{
			if(j != i && nNodeIndex[i] == nNodeIndex[j])
			{
				mapInnerNode[nNodeIndex[i]] = 0;
				break;
			}
		}
	}

	T_NODE_K itrStartNode	= NULL;
	T_NODE_K itrLastNode	= NULL;  // 연속성을 파악할 첫번째 요소의 i 노드 및 마지막 요소의 j 노드

	for(i = 0; i < nElem; i++)
	{
		BOOL bExist = m_pAttrCtrl->GetElemFromBaseDB(elemKey[i], eData);

		itrInnerNode = mapInnerNode.find(eData.elnod[0]);
		if(itrInnerNode == mapInnerNode.end())
		{
			if(itrStartNode != NULL)
				return FALSE;
			else
				itrStartNode = eData.elnod[0];

		}

		itrInnerNode = mapInnerNode.find(eData.elnod[1]);
		if(itrInnerNode == mapInnerNode.end())
		{
			if(itrLastNode != NULL)
				return FALSE;
			else
				itrLastNode = eData.elnod[1];
		}
	}

	if(itrStartNode == NULL || itrLastNode == NULL)
	{
		//시작점 끝점을 잘 못찾음.....
		//modify by yskim
		ASSERT(0);
		return FALSE;
	}

	for(i = 0; i < nElem; i++)
	{
		elemKey[i] = NULL;  // 멤버의 요소 리스트를 초기화한다.
	}

	itrElem = mapNodeiElem[itrStartNode]; // 멤버의 첫번째 요소를 찾는다.
	elemKey[0] = itrElem;    // 멤버의 첫번째 요소를 설정한다.  

	for(i = 0; i < nElem; i++)
	{
		BOOL bExist = m_pAttrCtrl->GetElemFromBaseDB(itrElem, eData);
		if(i < nElem - 1) 
		{
			itrNodeElem = mapNodeiElem.find(eData.elnod[1]);
			itrElem = itrNodeElem->second;
			elemKey[i + 1] = itrElem;
		}
		else if(i == nElem - 1)
		{
			T_NODE_K itrLastTmp = eData.elnod[1];

			if(itrLastTmp == itrLastNode)
			{
				bSuccess = TRUE;
			}
		}
	}

	ASSERT(bSuccess);

	delete []nNodeIndex; nNodeIndex=NULL;

	return TRUE;
}

BOOL CDgnTendonElemLengthCtrl::IsSameLengthInfo( const lengthInfo& info1, const lengthInfo& info2, BOOL bWithLength /*= FALSE*/ )
{
	if(fabs(info1.dTop_Z - info2.dTop_Z) >= 1E-6)
		return FALSE;
	if(fabs(info1.dBot - info2.dBot) >= 1E-6)
		return FALSE;
	if(fabs(info1.dLeft - info2.dLeft) >= 1E-6)
		return FALSE;
	if(fabs(info1.dRight_Y - info2.dRight_Y) >= 1E-6)
		return FALSE;

	if(bWithLength && ( fabs(info1.dPosX - info2.dPosX) >= 1E-6 ))
		return FALSE;

	return TRUE;
}

BOOL CDgnTendonElemLengthCtrl::GetLengInfoByElem( T_ELEM_K key, lengthInfo* info_I, lengthInfo* info_J, BOOL bCentroid )
{
	// I 의 dPosX는 0, J 의 dPosY는 요소의 길이를 세팅하자.
	// 이 함수를 잘 짜야 한다. 요소의 순서/순서에 따른 각 요소의 방향 까지 고려해서 순차적으로 값을 세팅
	// 여기의 값들은 모두 절대값을 사용하자.

	T_ELEM_D eData;
	int i, NumNode;
	T_NODE_D nData;
	double xyz[D_ELEM_MAXNOD][3];
	double dLength = 0.0;
	BOOL bExist = m_pAttrCtrl->GetElemFromBaseDB(key, eData);

	if(!bExist)
	{
		return FALSE;
	}
	
	ASSERT(sizeof(nData) == sizeof(double) * 3);

	NumNode = m_pAttrCtrl->GetNumNode(eData.eltyp, eData.elnod);

	if(NumNode == 2)
	{
		m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[0], nData);
		memcpy(&xyz[0][0], &nData, sizeof(nData));
		m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[1], nData);
		memcpy(&xyz[1][0], &nData, sizeof(nData));
		dLength = CMathFunc::mathLength(xyz[0][0]-xyz[1][0], xyz[0][1]-xyz[1][1], xyz[0][2]-xyz[1][2]);
		dLength	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, dLength);
	}
	else
	{
		ASSERT(0); //이하는 사용안할 것으로 생각됨.
		for(i = 0; i < NumNode; i++)
		{
			m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[i], nData);
			memcpy(&xyz[i][0], &nData, sizeof(nData));
		}

		if(m_pAttrCtrl->IsFrameType(eData.eltyp))
		{
			return CMathFunc::mathLength(xyz[0][0]-xyz[1][0], xyz[0][1]-xyz[1][1], xyz[0][2]-xyz[1][2]);
		}
		else if(m_pAttrCtrl->IsPlaneType(eData.eltyp))
		{
			double Area1=CMathFunc::mathArea(xyz[0], xyz[1], xyz[2]);
			double Area2=0.;
			if(NumNode > 3)Area2=CMathFunc::mathArea(xyz[0], xyz[2], xyz[3]);
			return Area1+Area2;
		}
		else if(m_pAttrCtrl->IsSolidType(eData.eltyp))
		{
			double unit_mass_temp[D_ELEM_MAXNOD];
			return CMathFunc::mathVolume(NumNode, xyz, unit_mass_temp);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	T_SECT_K sectK = eData.elpro;
	T_SECT_D sectD;
	double hi_Dimen,	bi_Dimen,	hj_Dimen,	bj_Dimen;
	double hi_Cen,		bi_Cen,		hj_Cen,		bj_Cen;

	if(!m_pAttrCtrl->GetSect(sectK, sectD)) return FALSE;
	if(!m_pAttrCtrl->GetSectDimen(sectD,    hi_Dimen, bi_Dimen, hj_Dimen, bj_Dimen)) return FALSE;
	if(!m_pAttrCtrl->GetSectCentroid(sectD, hi_Cen,   bi_Cen,   hj_Cen,   bj_Cen)) return FALSE;
	
	hi_Dimen	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, hi_Dimen);
	bi_Dimen	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, bi_Dimen);
	hj_Dimen	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, hj_Dimen);
	bj_Dimen	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, bj_Dimen);
	hi_Cen	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, hi_Cen);
	bi_Cen	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, bi_Cen);
	hj_Cen	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, hj_Cen);
	bj_Cen	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, bj_Cen);

	if(bCentroid)
	{
		if(info_I)
		{
			info_I->dPosX		= 0.0;
			info_I->dTop_Z		= hi_Dimen - hi_Cen;
			info_I->dBot		= hi_Cen;
			info_I->dRight_Y	= bi_Dimen - bi_Cen;
			info_I->dLeft		= bi_Cen;
		}
		
		if(info_J)
		{
			info_J->dPosX		= dLength;
			info_J->dTop_Z		= hj_Dimen - hj_Cen;
			info_J->dBot		= hj_Cen;
			info_J->dRight_Y	= bj_Dimen - bj_Cen;
			info_J->dLeft		= bj_Cen;
		}
	}
	else
	{
		double hi_Off,		bi_Off,		hj_Off,		bj_Off;
		m_pAttrCtrl->GetSectOffsetFromCentroid(sectD,	hi_Off,		bi_Off,		hj_Off,		bj_Off);
		hi_Off	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, hi_Off);
		bi_Off	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, bi_Off);
		hj_Off	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, hj_Off);
		bj_Off	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, bj_Off);

		if(info_I)
		{
			info_I->dPosX		= 0.0;
			info_I->dTop_Z		= hi_Dimen - (hi_Cen + hi_Off);
			info_I->dBot		= hi_Cen + hi_Off;
			info_I->dRight_Y	= bi_Dimen - (bi_Cen + bi_Off);
			info_I->dLeft		= bi_Cen + bi_Off;
		}
		
		if(info_J)
		{
			info_J->dPosX		= dLength;
			info_J->dTop_Z		= hj_Dimen - (hj_Cen + hj_Off);
			info_J->dBot		= hj_Cen + hj_Off;
			info_J->dRight_Y	= bj_Dimen - (bj_Cen + bj_Off);
			info_J->dLeft		= bj_Cen + bj_Off;
		}
	}

	return TRUE;
}

BOOL CDgnTendonElemLengthCtrl::GetLengthInfoByPos( double dPosX, lengthInfo& info, BOOL bCentroid )
{
	UINT i;

	double dZero = 1E-5;

	if(dPosX < dZero)
	{
		dPosX = 0.0;
	}
	if( dPosX >= m_dTotalLength - dZero )
	{
		dPosX = m_dTotalLength;
	}

	if(bCentroid)
	{
		for( i = 0 ; i < m_vecInfo_Centroid.size(); i++)
		{
			if(dPosX <= m_vecInfo_Centroid[i].infoEndJ.dPosX)
			{
				const lengthInfo& infoPre = m_vecInfo_Centroid[i].infoEndI;
				const lengthInfo& infoNext = m_vecInfo_Centroid[i].infoEndJ;
				
				double dRatio = (dPosX - infoPre.dPosX) / (infoNext.dPosX - infoPre.dPosX);
				info.dTop_Z	= infoPre.dTop_Z	+ ((infoNext.dTop_Z	- infoPre.dTop_Z)		* dRatio);
				info.dBot	= infoPre.dBot	+ ((infoNext.dBot	- infoPre.dBot)		* dRatio);
				info.dLeft	= infoPre.dLeft	+ ((infoNext.dLeft	- infoPre.dLeft)	* dRatio);
				info.dRight_Y	= infoPre.dRight_Y+ ((infoNext.dRight_Y	- infoPre.dRight_Y)	* dRatio);
				info.dPosX = dPosX;
				
				return TRUE;
			}
		}
	}
	else
	{
		for( i = 0 ; i < m_vecInfo_ElemOffset.size(); i++)
		{
			if(dPosX <= m_vecInfo_ElemOffset[i].infoEndJ.dPosX)
			{
				const lengthInfo& infoPre = m_vecInfo_ElemOffset[i].infoEndI;
				const lengthInfo& infoNext = m_vecInfo_ElemOffset[i].infoEndJ;
				
				double dRatio = (dPosX - infoPre.dPosX) / (infoNext.dPosX - infoPre.dPosX);
				info.dTop_Z	= infoPre.dTop_Z	+ ((infoNext.dTop_Z	- infoPre.dTop_Z)		* dRatio);
				info.dBot	= infoPre.dBot	+ ((infoNext.dBot	- infoPre.dBot)		* dRatio);
				info.dLeft	= infoPre.dLeft	+ ((infoNext.dLeft	- infoPre.dLeft)	* dRatio);
				info.dRight_Y	= infoPre.dRight_Y+ ((infoNext.dRight_Y	- infoPre.dRight_Y)	* dRatio);
				info.dPosX = dPosX;
				
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CDgnTendonElemLengthCtrl::GetLengthInfoByIdx( int nIdx, lengthInfoIJ& info, BOOL bCentroid /*= FALSE*/ )
{
	if(nIdx < 0)
		return FALSE;

	if(bCentroid)
	{
		if(m_vecInfo_Centroid.size() <= nIdx)
			return FALSE;

		info = m_vecInfo_Centroid[nIdx];
	}
	else
	{
		if(m_vecInfo_ElemOffset.size() <= nIdx)
			return FALSE;
		
		info = m_vecInfo_ElemOffset[nIdx];
	}

	return TRUE;
}

int CDgnTendonElemLengthCtrl::GetLengthInfoCount( BOOL bCentroid /*= FALSE*/ )
{
	if(bCentroid)
	{
		return (int)m_vecInfo_Centroid.size();
	}
	else
	{
		return (int)m_vecInfo_ElemOffset.size();
	}

	ASSERT(0);
	return -1;
}

T_ELEM_K CDgnTendonElemLengthCtrl::GetElemKeyByIdx( int nIdx, BOOL bCentroid /*= FALSE*/ )
{
	if(nIdx < 0) return 0;

	if(bCentroid)
	{
		if(nIdx >= m_vecInfo_Centroid.size()) return 0;

		return m_vecInfo_Centroid[nIdx].nElemKey;
	}
	else
	{
		if(nIdx >= m_vecInfo_ElemOffset.size()) return 0;
		
		return m_vecInfo_ElemOffset[nIdx].nElemKey;
	}

	return 0;
}

BOOL CDgnTendonElemLengthCtrl::GetHeightWidthByElem( T_ELEM_K key, double& dHeight, double& dWidth, double& dGapY, double& dGapZ, double dPos )
{
	lengthInfo infoI_Elem;
	lengthInfo infoJ_Elem;
	lengthInfo infoI_Cent;
	lengthInfo infoJ_Cent;
	
	if(!GetLengInfoByElem(key, &infoI_Cent, &infoJ_Cent, TRUE))
		return FALSE;
	if(!GetLengInfoByElem(key, &infoI_Elem, &infoJ_Elem, FALSE))
		return FALSE;

	if(fabs(dPos) <= 1E-5)
	{
		dHeight = fabs(infoI_Elem.dTop_Z) + fabs(infoI_Elem.dBot);
		dWidth = fabs(infoI_Elem.dRight_Y) + fabs(infoI_Elem.dLeft);

		dGapY = infoI_Elem.dRight_Y - infoI_Cent.dRight_Y;
		dGapZ = infoI_Elem.dTop_Z - infoI_Cent.dTop_Z;
	}
	else if(fabs(1.0 - dPos) <= 1E-5)
	{
		dHeight = fabs(infoJ_Elem.dTop_Z) + fabs(infoJ_Elem.dBot);
		dWidth = fabs(infoJ_Elem.dRight_Y) + fabs(infoJ_Elem.dLeft);

		dGapY = infoJ_Elem.dRight_Y - infoJ_Cent.dRight_Y;
		dGapZ = infoJ_Elem.dTop_Z - infoJ_Cent.dTop_Z;
	}
	else
	{	//보간 해서 처리하자.
		double dTemp1, dTemp2;
		dTemp1 = fabs(infoI_Elem.dTop_Z) + fabs(infoI_Elem.dBot);
		dTemp2 = fabs(infoJ_Elem.dTop_Z) + fabs(infoJ_Elem.dBot);
		dHeight = (dPos * dTemp1) + ((1.0 - dPos) * dTemp2);
		dTemp1 = fabs(infoI_Elem.dRight_Y) + fabs(infoI_Elem.dLeft);
		dTemp2 = fabs(infoJ_Elem.dRight_Y) + fabs(infoJ_Elem.dLeft);
		dWidth = (dPos * dTemp1) + ((1.0 - dPos) * dTemp2);

		
		dTemp1 = infoI_Elem.dRight_Y - infoI_Cent.dRight_Y;
		dTemp2 = infoJ_Elem.dRight_Y - infoJ_Cent.dRight_Y;
		dGapY = (dPos * dTemp1) + ((1.0 - dPos) * dTemp2);
		dTemp1 = infoI_Elem.dTop_Z - infoI_Cent.dTop_Z;		
		dTemp2 = infoJ_Elem.dTop_Z - infoJ_Cent.dTop_Z;
		dGapZ = (dPos * dTemp1) + ((1.0 - dPos) * dTemp2);
	}

	return TRUE;
}

BOOL CDgnTendonElemLengthCtrl::IsAllSameSect( const vecElemKey& elemKey, T_SECT_K& key )
{
	int i;
	int nSize = (int)elemKey.size();
	if(nSize == 0) return FALSE;

	T_ELEM_D eData;
	T_SECT_K sectKey;

	for(i = 0 ; i < nSize; i++)
	{
		BOOL bExist = m_pAttrCtrl->GetElemFromBaseDB(elemKey[i], eData);
		if(bExist)
		{
			if(i == 0)							sectKey = eData.elpro;
			else if(sectKey != eData.elpro)		return FALSE;
		}
		else return FALSE;
	}

	key = sectKey;

	return TRUE;
}

BOOL CDgnTendonElemLengthCtrl::GetCircleStartCenterPos( T_ELEM_K keyStart, T_ELEM_K keyMid, T_ELEM_K keyEnd,
																											 double* pPosStart, double* pPosCenter, double& dRadAxis, int& nCWType )
{
	T_ELEM_D eData;
	T_NODE_D nData;

	int i, NumNode;

	double dPosStart[3];
	double dPosMid[3];
	double dPosEnd[3];
	double dPosCenter[3];
	double dLength = 0.0;
	double dRad;

	BOOL bExist;

	for(i = 0 ; i < 3; i++)
	{
		if (i == 0)       bExist = m_pAttrCtrl->GetElemFromBaseDB(keyStart, eData);
		else if (i == 1)  bExist = m_pAttrCtrl->GetElemFromBaseDB(keyMid, eData);
		else if (i == 2)  bExist = m_pAttrCtrl->GetElemFromBaseDB(keyEnd, eData);
		else              ASSERT(0);
		
		ASSERT(sizeof(nData) == sizeof(double) * 3);

		if(!bExist)         return FALSE;
		NumNode = m_pAttrCtrl->GetNumNode(eData.eltyp, eData.elnod);    
		if(NumNode != 2)    return FALSE;

		if(i == 0)
		{
			m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[0], nData);
			dPosStart[0] = nData.x;   dPosStart[1] = nData.y;   dPosStart[2] = nData.z;
		}
		else if(i == 1)
		{
			m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[0], nData);
			dPosMid[0] = nData.x;     dPosMid[1] = nData.y;     dPosMid[2] = nData.z;
		}
		else if(i == 2)
		{
			m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[1], nData);
			dPosEnd[0] = nData.x;     dPosEnd[1] = nData.y;     dPosEnd[2] = nData.z;
		}
	}

	if(CMathFunc::mathCircleForm3Point(dPosStart, dPosMid, dPosEnd, dPosCenter, dRad) != 1)  return FALSE;
	//구했당.

	if(fabs(dPosEnd[2] - dPosStart[2]) >= 1E-5)
	{
		double dVec1[3];
		double dVec2[3];
		dVec1[0] = dPosEnd[0] - dPosStart[0];
		dVec1[1] = dPosEnd[1] - dPosStart[1];
		dVec1[2] = dPosEnd[2] - dPosStart[2];
		dVec2[0] = dVec1[0];
		dVec2[1] = dVec1[1];
		dVec2[2] = 0.0;
		dRadAxis = CMathFunc::mathCrossAngleNormalize(dVec1, dVec2);
	}
	else
	{
		dRadAxis = 0.0;
	}


	if(dPosEnd[2] - dPosStart[2] >= 1E-5)
	{
		dRadAxis *= -1.0;
	}

	pPosStart[0] = dPosStart[0];
	pPosStart[1] = dPosStart[1];
	pPosStart[2] = dPosStart[2];
	pPosCenter[0] = dPosCenter[0];
	pPosCenter[1] = dPosCenter[1];
	dPosCenter[2] = 0.0;

	double dTemp = dPosCenter[0] * dPosStart[1] + dPosStart[0] * dPosMid[1]
		+ dPosMid[0] * dPosCenter[1] - dPosCenter[0] * dPosMid[1]
		- dPosStart[0] * dPosCenter[1] - dPosMid[0] * dPosStart[1];

	if(dTemp > 1E-5)
		nCWType = 1;
	else if(dTemp < -1E-5)
		nCWType = 0;
	else
	{
		ASSERT(0);
		nCWType = 0;
		return FALSE;
	}

	return TRUE;
}

BOOL CDgnTendonElemLengthCtrl::IsCurve( std::vector<T_ELEM_K>& vecElemKey )
{
	int i, nSize, NumNode;
	T_ELEM_D eData;
	T_NODE_D nData1;
	T_NODE_D nData2;
	BOOL bExist;
	double dVec[3];
	double dVecTemp[3];

	nSize = (int)vecElemKey.size();

	if(nSize < 2)         return FALSE;
	
	//첫번째 요소는 미리 구해놓고,
	bExist = m_pAttrCtrl->GetElemFromBaseDB(vecElemKey[0], eData);
	if(!bExist) return FALSE;  
	NumNode = m_pAttrCtrl->GetNumNode(eData.eltyp, eData.elnod);    
	if(NumNode != 2) return FALSE;  
	bExist = m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[0], nData1);
	if(!bExist) return FALSE;  
	bExist = m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[1], nData2);
	if(!bExist) return FALSE;  

	dVec[0] = nData2.x - nData1.x;
	dVec[1] = nData2.y - nData1.y;
	dVec[2] = nData2.z - nData1.z;

	for(i = 1 ; i < nSize; i++)
	{
		bExist = m_pAttrCtrl->GetElemFromBaseDB(vecElemKey[i], eData);
		if(!bExist)         return FALSE;
		NumNode = m_pAttrCtrl->GetNumNode(eData.eltyp, eData.elnod);    
		if(NumNode != 2)    return FALSE;
		bExist = m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[0], nData1);
		if(!bExist)         return FALSE;
		bExist = m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[1], nData2);
		if(!bExist)         return FALSE;
		
		dVecTemp[0] = nData2.x - nData1.x;
		dVecTemp[1] = nData2.y - nData1.y;
		dVecTemp[2] = nData2.z - nData1.z;

		double dRad = CMathFunc::mathCrossAngleNormalize(dVec, dVecTemp);

		if( (fabs(dRad)) >= 1E-5 )
			return TRUE;
	}
	return FALSE;
}

double CDgnTendonElemLengthCtrl::GetLengthRatioByPos( BOOL bXY, const double& dPosX, const double& dRatio )
{
	double dRetValue;
	if(-1E-6 > dRatio || 1.0 + 1E-6 < dRatio)
	{
		ASSERT(0);
		dRetValue = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, 0.1);
		return dRetValue;
	}
	
	lengthInfo lenInfo;
	
	if(!GetLengthInfoByPos(dPosX, lenInfo))
	{
		ASSERT(0);
		dRetValue = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, 0.1);
		return dRetValue;
	}
	
	double dTotalLength = 0.0;
	
	if(bXY)
		dTotalLength = fabs(lenInfo.dLeft) + fabs(lenInfo.dRight_Y);
	else
		dTotalLength = fabs(lenInfo.dTop_Z) + fabs(lenInfo.dBot);

	dRetValue = dTotalLength * dRatio;
	
	return m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, dRetValue);
}
