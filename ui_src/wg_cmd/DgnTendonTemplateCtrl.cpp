#include "StdAfx.h"
#include "DgnTendonTemplateCtrl.h"
#include "DgnTendonTemplateFileCtrl.h"
#include "DgnTendonElemLengthCtrl.h"
#include "DgnTendonAutoGenCtrl.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/UnitCtrl.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "../wg_base/wg_base_MsgDll.h"
#include "../wg_base/GlobalFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum EN_TNDN_TEMPLATE_ERR_MSG
{
	EN_TNDN_TEMPLATE_ERR_MSG_INVALID_ELEM,
	EN_TNDN_TEMPLATE_ERR_MSG_XY_SPANNUM,
	EN_TNDN_TEMPLATE_ERR_MSG_XZ_SPANNUM,
	EN_TNDN_TEMPLATE_ERR_MSG_XY_DFDE,
	EN_TNDN_TEMPLATE_ERR_MSG_XZ_DFDE,
	EN_TNDN_TEMPLATE_ERR_MSG_DIFF_SPAN_LEN,
};

#define SET_ERR_MSG_RETURN(msgVar, msg, bRet) { if(msgVar) *msgVar = msg;  return bRet; }

CDgnTendonTemplateCtrl::CDgnTendonTemplateCtrl(CDBDoc* pDoc)
{
	m_mapTndnTemplate.clear();
	m_pDoc = pDoc;
	m_pAttrCtrl = pDoc->m_pAttrCtrl;
	m_pUnitCtrl = pDoc->m_pUnitCtrl;

	m_lengthCtrl	= new CDgnTendonElemLengthCtrl(m_pAttrCtrl);
	m_FileCtrl		= new CDgnTendonTemplateFileCtrl();
	m_pAutoGenCtrl	= new CDgnTendonAutoGenCtrl(m_pAttrCtrl, this);

	m_bValidElem = FALSE;
	m_bCurveElem = FALSE;
	m_nNextKey = 0;
}


CDgnTendonTemplateCtrl::~CDgnTendonTemplateCtrl(void)
{
	if(m_lengthCtrl)	delete m_lengthCtrl;
	m_lengthCtrl = NULL;

	if(m_FileCtrl)		delete m_FileCtrl;
	m_FileCtrl = NULL;

	if(m_pAutoGenCtrl)	delete m_pAutoGenCtrl;
	m_pAutoGenCtrl = NULL;
}

BOOL CDgnTendonTemplateCtrl::Import(TCHAR* pFilePath, BOOL bReplace)
{
	std::vector<TndnTemplate> vecData;
	if(!bReplace)
		MapToVector(m_mapTndnTemplate, vecData);

	int nSizeOrg = (int)vecData.size();

	BOOL bUseRatio = FALSE;
	BOOL bRetVal = m_FileCtrl->Import(pFilePath, &vecData, bUseRatio);
	
	int i;
	int nSize = (int)vecData.size();

	for(i = 0 ; i < nSize ; i++)
	{
		if(vecData[i].bSameSpanInterval)
		{
			GetSameSpanStr(vecData[i], TRUE, vecData[i].tndnSubXY.strSpanLength);
		  GetSameSpanStr(vecData[i], FALSE, vecData[i].tndnSubXZ.strSpanLength);
		}
	}

	if(bUseRatio && (nSizeOrg < vecData.size()))
	{
		nSize = (int)vecData.size();
		for(i = nSizeOrg ; i < nSize; i++)
		{
			if(!ConvertRatioLength_DfDe(TRUE, vecData[i]))
			{
				ASSERT(0);
				continue;
			}
		}
	}
	
	VectorToMap(m_mapTndnTemplate, vecData);

	return bRetVal;
}

BOOL CDgnTendonTemplateCtrl::Export(TCHAR* pFilePath, vecTndnKey* nKeyList, BOOL bUseRatio)
{
	vecTndnTemp vecData;
	
	MapToVector(m_mapTndnTemplate, vecData, nKeyList); //데이터를 모두 저장해둔 맵에서 Vector를 복사해 온다.

	if(bUseRatio) // Use Ratio 라면 , de, df값을 비율로 바꾼다.
	{
		int i;
		int nSize = (int)vecData.size();
		for(i = 0; i < nSize; i++)
		{
			if(!ConvertRatioLength_DfDe(FALSE, vecData[i]))
			{
				ASSERT(0);
				continue;
			}
		}
	}
	
	BOOL bRetVal = m_FileCtrl->Export(pFilePath, &vecData, bUseRatio);

	return bRetVal;
}

BOOL CDgnTendonTemplateCtrl::GetTypeName( TNDN_TEMPLATE_TYPE enType, CString& strName )
{
	BOOL bRetVal = TRUE;

	switch (enType)
	{
	case TNDN_TEMPLATE_TYPE_STRAIGHT:
		strName = _LS(IDS_CMD_TNDN_TEMPLATE_TYPE_STRAIGHT_LINE);
		break;
	case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1:
		strName = _LS(IDS_CMD_TNDN_TEMPLATE_TYPE_STRAIGHT_BENT1);
		break;
	case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2:
		strName = _LS(IDS_CMD_TNDN_TEMPLATE_TYPE_STRAIGHT_BENT2);
		break;
	case TNDN_TEMPLATE_TYPE_CURVE_LINE:
		strName = _LS(IDS_CMD_TNDN_TEMPLATE_TYPE_CURVE_LINE);
		break;
	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1:
		strName = _LS(IDS_CMD_TNDN_TEMPLATE_TYPE_CURVE_1);
		break;
	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2:
		strName = _LS(IDS_CMD_TNDN_TEMPLATE_TYPE_CURVE_2);
		break;
	case TNDN_TEMPLATE_TYPE_CURVE_BENT_1:
		strName = _LS(IDS_CMD_TNDN_TEMPLATE_TYPE_CURVE_BENT1);
		break;
	case TNDN_TEMPLATE_TYPE_CURVE_ST:
		strName = _LS(IDS_CMD_TNDN_TEMPLATE_TYPE_CURVE_ST);
		break;
	default:
		strName.Format(_T(""));
		break;
	}

	return bRetVal;
}

BOOL CDgnTendonTemplateCtrl::GetTdnaElemType( const TndnTemplate& tndnTemp, T_TDNA_D& tTdna, BOOL bConvertOut, BOOL bMsg /*= FALSE*/)
{
	int i;
	int nElemSize = (int)m_vecElemKey.size();
	int nSpanSize;

	if(nElemSize <= 0)						return FALSE;
	if(!IsValidTndnTemplate(tndnTemp, bMsg))		return FALSE;

	tTdna.Initialize();
	
	//고정된 내용 세팅.
	tTdna.TendonTypeKey	= tndnTemp.keyProperty;		//프로퍼티
	tTdna.TendonName	= tndnTemp.strName;			//텐던 명
	tTdna.TendonName.TrimRight();
	tTdna.GroupKey		= tndnTemp.keyGroup;		//그룹.
	tTdna.nInputType	= 0;						//InputType 2D:0
	tTdna.bProjection	= FALSE;
	tTdna.bTypicalTendon= FALSE;
	tTdna.nTendonShape	= 2;						//Element Type... 추후에 Curve Straight 로 만드는 함수도 만들자.
	tTdna.InsertElemKey = m_vecElemKey[0];			//시작 요소는 첫번째.(이미 정렬이 되었을 것이다.
	tTdna.nXAxisDirectionIJ = 0;					//무조건 I->J 방향.
	tTdna.nInsertPoint	= 0;						//무조건 I에서 시작.

	T_TDNT_D TdntD;
	if (m_pAttrCtrl->GetTdnt(tTdna.TendonTypeKey, TdntD)) 
	{
		if (TdntD.nTendonType == 1 && TdntD.nLoadType == 0) // Internal-Pre
		{
			tTdna.dDebondBeginLen = tndnTemp.dOffsetFront;
			tTdna.dDebondEndLen   = tndnTemp.dOffsetEnd;
		}
	}

	//내용 변하는 값 세팅.
	tTdna.nCurveType	= tndnTemp.nType;						//Spline 기본.

	tTdna.aElemList.SetSize(nElemSize);				//할당 요소 세팅

	for(i = 0 ; i < nElemSize; i++)
	{
		tTdna.aElemList[i] = m_vecElemKey[i];
	}

	for(i = 0 ; i < 2; i++)
	{
		const TndnTemplateSub* pSub;

		if(i == 0)		pSub = &(tndnTemp.tndnSubXY);		// 0 : XY
		if(i == 1)		pSub = &(tndnTemp.tndnSubXZ);		// 1 : XZ

		nSpanSize = (int)pSub->aSpanInfo.size();
		
		caDouble caSpanLength;
		CString strLength = pSub->strSpanLength;
		CStrParser::GetUEDistance(strLength, caSpanLength);

		if(!GetTdnaSub(i, caSpanLength, 0.0/*tndnTemp.dOffsetFront*/, pSub, (i == 0 ? tTdna.aProfileY : tTdna.aProfileZ) ))
		{
			return FALSE;
		}
	}

	if(bConvertOut)
		m_pUnitCtrl->ConvertUnitTdnaOut(tTdna);

	// MQC 25504, 25413
	// Tendon Profile 을 넣을 때 XY 평면과 Yz 평면의 X 시작 끝 지점이 같아야 하는 에러처리가 있는데.
	// 여기서 톨러런스 문제가 발생하는 경우가 있음.. 그래서 거의 비슷한 경우엔 같은 값으로 맞추는 처리를 여기서 함
	ASSERT(tTdna.nInputType == 0);	
	if(fabs(tTdna.aProfileY[0].x - tTdna.aProfileZ[0].x) < 0.01)
	{
		tTdna.aProfileZ[0].x = tTdna.aProfileY[0].x;
	}
	else
	{
		ASSERT(0); // 이 상황이면 ErrorCheck 에 걸려서 안들어 갈 것 같음
	}

	int nSizeY, nSizeZ;
	nSizeY = tTdna.aProfileY.GetSize();
	nSizeZ = tTdna.aProfileZ.GetSize();
	if(fabs(tTdna.aProfileY[nSizeY - 1].x - tTdna.aProfileZ[nSizeZ - 1].x) < 0.01)
	{
		tTdna.aProfileZ[nSizeZ - 1].x = tTdna.aProfileY[nSizeY - 1].x;
	}
	else
	{
		ASSERT(0); // 이 상황이면 ErrorCheck 에 걸려서 안들어 갈 것 같음
	}

	return TRUE;
}

void CDgnTendonTemplateCtrl::SetAssignElemList(std::vector<T_ELEM_K>& vecElemKey)
{
	m_vecElemKey.clear();

	m_bAllSameSect = FALSE;
	m_keySect = 0;
	m_bValidElem = m_lengthCtrl->SetElemList( vecElemKey, m_bAllSameSect, m_keySect );
	m_bCurveElem = m_lengthCtrl->IsCurve(vecElemKey);

	if(m_bAllSameSect && m_keySect == 0)
	{
		ASSERT(0);
		m_bAllSameSect = FALSE;
	}

	m_vecElemKey = vecElemKey;
	SetExistSpanInfo();
}

BOOL CDgnTendonTemplateCtrl::GetTemplateNU( int nKey, TndnTemplate& TempInfo )
{
	itrMapTndn itr = m_mapTndnTemplate.find(nKey);
	
	if( itr != m_mapTndnTemplate.end() )
	{
		TempInfo = itr->second;
		return TRUE;
	}
	
	return FALSE;
}

BOOL CDgnTendonTemplateCtrl::GetTemplate( int nKey, TndnTemplate& TempInfo )
{
	itrMapTndn itr = m_mapTndnTemplate.find(nKey);
	
	if( itr != m_mapTndnTemplate.end() )
	{
		TempInfo = ConvertUnitOut(itr->second);
		return TRUE;
	}
	
	return FALSE;
}

void CDgnTendonTemplateCtrl::AddTemplateNU( const TndnTemplate& TempInfo )
{	//중립단위계로 들어올때 바로 쓰는것.
	std::pair<int, TndnTemplate> pairTemp;
	pairTemp.first = GetNewKey(TRUE);
	pairTemp.second = TempInfo;
	MakeZeroSpanValue(pairTemp.second);
	m_mapTndnTemplate.insert(pairTemp);
}

void CDgnTendonTemplateCtrl::AddTemplate( const TndnTemplate& TempInfo )
{	//현재 단위계 -> 중립 단위계
	std::pair<int, TndnTemplate> pairTemp;
	pairTemp.first = GetNewKey(TRUE);
	pairTemp.second = ConvertUnitIn( TempInfo );
	MakeZeroSpanValue(pairTemp.second);
	m_mapTndnTemplate.insert(pairTemp);
}

void CDgnTendonTemplateCtrl::DelTemplate( int nKey )
{
	itrMapTndn itr = m_mapTndnTemplate.find(nKey);
	if(itr != m_mapTndnTemplate.end())
	{
		m_mapTndnTemplate.erase(itr);
	}
}

void CDgnTendonTemplateCtrl::ModTemplateNU( int nKey, TndnTemplate& TempInfo )
{
	itrMapTndn itr = m_mapTndnTemplate.find(nKey);
	if(itr != m_mapTndnTemplate.end())
	{
		MakeZeroSpanValue(TempInfo);
		itr->second = TempInfo;	
	}
}
void CDgnTendonTemplateCtrl::ModTemplate( int nKey, TndnTemplate& TempInfo )
{
	itrMapTndn itr = m_mapTndnTemplate.find(nKey);
	if(itr != m_mapTndnTemplate.end())
	{
		MakeZeroSpanValue(TempInfo);
		itr->second = ConvertUnitIn( TempInfo );
	}
}


int CDgnTendonTemplateCtrl::GetKeyList(vecTndnKey& keyList)
{
	keyList.clear();

	itrMapTndn itr;
	for(itr = m_mapTndnTemplate.begin(); itr != m_mapTndnTemplate.end(); itr++)
	{
		int nKey = itr->first;
		keyList.push_back(nKey);
	}

	return (int)keyList.size();
}

void CDgnTendonTemplateCtrl::MapToVector( mapTndnTemp& mapData, vecTndnTemp& vecData , vecTndnKey* nKeyList)
{
	vecData.clear();

	itrMapTndn itrMap;
	if(nKeyList)
	{
		itrKey itrVec;
		for(itrVec = nKeyList->begin(); itrVec != nKeyList->end(); itrVec++)
		{
			itrMap = mapData.find(*itrVec);
			vecData.push_back(itrMap->second);
		}
	}
	else
	{
		for(itrMap = mapData.begin(); itrMap != mapData.end(); itrMap++)
		{
			vecData.push_back(itrMap->second);
		}
	}
}

void CDgnTendonTemplateCtrl::VectorToMap( mapTndnTemp& mapData, vecTndnTemp& vecData )
{
	mapData.clear();

	itrVecTndn itr;

	for(itr = vecData.begin(); itr != vecData.end(); itr++)
	{
		std::pair<int, TndnTemplate> pairTemp;
		pairTemp.first = GetNewKey(TRUE);
		pairTemp.second = *itr;
		mapData.insert(pairTemp);
	}
	
}

int CDgnTendonTemplateCtrl::GetNewKey( BOOL bWithInc /*= FALSE*/ )
{
	int nRetVal = m_nNextKey;
	if(bWithInc)
	{
		m_nNextKey++;
	}
	return nRetVal;

}

int CDgnTendonTemplateCtrl::CopyTemplateNU(const TndnTemplate& tempInfo, double dOffZIn, double dOffYIn,
																					 int nCopyNum, vecTndnTemp& vecTemp, BOOL bWithName)
{
	int i, j;
	vecTemp.clear();
	vecTemp.resize(nCopyNum);	
	
	double dOffZ = dOffZIn;
	double dOffY = dOffYIn;
	
	if(tempInfo.tndnSubXY.bXSymmetry)	dOffY *= -1.0;
	if(tempInfo.tndnSubXZ.bXSymmetry)	dOffZ *= -1.0;
	
	for(i = 1 ; i <= nCopyNum; i++)
	{
		TndnTemplate tempInfoCopy = tempInfo;

		if(bWithName)	  tempInfoCopy.strName.Format(_T("%s_%d"), tempInfo.strName, i);

		for(j = 0; j < tempInfoCopy.tndnSubXZ.aSpanInfo.size(); j++)
		{
			tempInfoCopy.tndnSubXZ.aSpanInfo[j].dDb		+=	dOffZ * i;
			tempInfoCopy.tndnSubXZ.aSpanInfo[j].dDb1	+=	dOffZ * i;
			tempInfoCopy.tndnSubXZ.aSpanInfo[j].dDt		-=	dOffZ * i;
			tempInfoCopy.tndnSubXZ.aSpanInfo[j].dDt1	-=	dOffZ * i;
		}
		for(j = 0; j < tempInfoCopy.tndnSubXY.aSpanInfo.size(); j++)
		{
			tempInfoCopy.tndnSubXY.aSpanInfo[j].dDb		+=	dOffY * i;
			tempInfoCopy.tndnSubXY.aSpanInfo[j].dDb1	+=	dOffY * i;
			tempInfoCopy.tndnSubXY.aSpanInfo[j].dDt		-=	dOffY * i;
			tempInfoCopy.tndnSubXY.aSpanInfo[j].dDt1	-=	dOffY * i;
		}
		
		vecTemp[i - 1] = tempInfoCopy;
	}

	return nCopyNum;
}

int CDgnTendonTemplateCtrl::CopyTemplateNU( int nKey, double dOffZIn, double dOffYIn, int nCopyNum,
											vecTndnTemp& vecTemp, BOOL bWithName)
{
	TndnTemplate tempInfo;
	if(!GetTemplateNU(nKey, tempInfo))
	{
		return 0;
	}

	return CopyTemplateNU(tempInfo, dOffZIn, dOffYIn, nCopyNum, vecTemp, bWithName);
}

void CDgnTendonTemplateCtrl::CopyTemplateNU( vecTndnKey keyList, double dOffZIn, double dOffYIn,
																						int nCopyNum, BOOL bWithName )
{
	std::vector<TndnTemplate> vecTndn;
	int nSizeKey;
	int nSizeTndn;
	int i, j;


	nSizeKey = (int)keyList.size();
	for(i = 0 ; i < nSizeKey; i++)
	{
		nSizeTndn = CopyTemplateNU(keyList[i], dOffZIn, dOffYIn, nCopyNum, vecTndn, bWithName);

		for(j = 0 ; j < nSizeTndn; j++)
		{
			AddTemplateNU(vecTndn[j]);
		}
	}
}

BOOL CDgnTendonTemplateCtrl::IsValidElem(BOOL bPrintMsg)
{
	if(!m_bValidElem)
	{
		if(bPrintMsg)
		{
			PrintMsg(EN_TNDN_TEMPLATE_ERR_MSG_INVALID_ELEM);
		}
	}

	return m_bValidElem;
}

TndnTemplate CDgnTendonTemplateCtrl::ConvertUnitIn( const TndnTemplate& data )
{
	TndnTemplate tndnData = data;
	
	tndnData.dOffsetFront	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, tndnData.dOffsetFront);
	tndnData.dOffsetEnd		= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, tndnData.dOffsetEnd);
	
	ConvertUnitIn(tndnData.tndnSubXY.strSpanLength);
	ConvertUnitIn(tndnData.tndnSubXZ.strSpanLength);
	
	for(int i = 0 ; i < 2; i++)
	{
		TndnTemplateSub* pSubInfo;
		if(i == 0)	 pSubInfo = &(tndnData.tndnSubXY);
		if(i == 1)	 pSubInfo = &(tndnData.tndnSubXZ);
		
		for(int j = 0 ; j < pSubInfo->aSpanInfo.size(); j++)
		{
			pSubInfo->aSpanInfo[j].dDb	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDb);
			pSubInfo->aSpanInfo[j].dDb1	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDb1);
			pSubInfo->aSpanInfo[j].dDt	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDt);
			pSubInfo->aSpanInfo[j].dDt1	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDt1);
			pSubInfo->aSpanInfo[j].dDe	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDe);
			pSubInfo->aSpanInfo[j].dDf	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDf);
						pSubInfo->aSpanInfo[j].dR1	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dR1);
						pSubInfo->aSpanInfo[j].dR2	= m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dR2);
		}
	}

	return tndnData;
}

TndnTemplate CDgnTendonTemplateCtrl::ConvertUnitOut( const TndnTemplate& data )
{
	TndnTemplate tndnData = data;
	
	tndnData.dOffsetFront	= m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, tndnData.dOffsetFront);
	tndnData.dOffsetEnd		= m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, tndnData.dOffsetEnd);
	
	ConvertUnitOut(tndnData.tndnSubXY.strSpanLength);
	ConvertUnitOut(tndnData.tndnSubXZ.strSpanLength);

	for(int i = 0 ; i < 2; i++)
	{
		TndnTemplateSub* pSubInfo;
		if(i == 0)	 pSubInfo = &(tndnData.tndnSubXY);
		if(i == 1)	 pSubInfo = &(tndnData.tndnSubXZ);
		
		for(int j = 0 ; j < pSubInfo->aSpanInfo.size(); j++)
		{
			pSubInfo->aSpanInfo[j].dDb	= m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDb);
			pSubInfo->aSpanInfo[j].dDb1	= m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDb1);
			pSubInfo->aSpanInfo[j].dDt	= m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDt);
			pSubInfo->aSpanInfo[j].dDt1	= m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDt1);
			pSubInfo->aSpanInfo[j].dDe	= m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDe);
			pSubInfo->aSpanInfo[j].dDf	= m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dDf);
						pSubInfo->aSpanInfo[j].dR1	= m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dR1);
						pSubInfo->aSpanInfo[j].dR2	= m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, pSubInfo->aSpanInfo[j].dR2);
		}
	}

	return tndnData;
}

void CDgnTendonTemplateCtrl::ConvertUnitIn(CString& strSpanLength)
{
	caDouble caSpanLength;
	if(CStrParser::GetUEDistance(strSpanLength, caSpanLength))
	{
		int i;
		for(i = 0 ; i < caSpanLength.GetSize(); i++)
		{
			caSpanLength[i] = m_pUnitCtrl->ConvertUnitDataIn(D_UNITSYS_BASE_LENGTH, caSpanLength[i]);
		}
		strSpanLength = CStrParser::ConvtDistArr2DistStr(caSpanLength);
	}
}

void CDgnTendonTemplateCtrl::ConvertUnitOut(CString& strSpanLength)
{
	caDouble caSpanLength;
	if(CStrParser::GetUEDistance(strSpanLength, caSpanLength))
	{
		int i;
		for(i = 0 ; i < caSpanLength.GetSize(); i++)
		{
			caSpanLength[i] = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, caSpanLength[i]);
		}
		strSpanLength = CStrParser::ConvtDistArr2DistStr(caSpanLength);
	}  
}

BOOL CDgnTendonTemplateCtrl::GetPosStartEnd(int nKey, double& nPosY_Start, double& nPosZ_Start, double& nPosY_End, double& nPosZ_End)
{
	TndnTemplate tempInfo;
	lengthInfo lenInfo;

	GetTemplate(nKey, tempInfo);
	
	int nSzSpanXZ = (int)tempInfo.tndnSubXZ.aSpanInfo.size();
	int nSzSpanXY = (int)tempInfo.tndnSubXY.aSpanInfo.size();

	if(nSzSpanXZ <= 0 || nSzSpanXY <= 0)
	{
		ASSERT(0);
		return FALSE;
	}
	
	m_lengthCtrl->GetLengthInfoByPos(0.0, lenInfo);
	ASSERT(0);

// 	switch (tempInfo.tndnSubXZ.nType)
// 	{
// 	case TNDN_TEMPLATE_TYPE_STRAIGHT:
// 	case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT1:
// 	case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT2:
// 	case TNDN_TEMPLATE_TYPE_CURVE:
// 		{
// 			nPosZ_Start = lenInfo.dTop_Z - tempInfo.tndnSubXZ.aSpanInfo[0].dDt;
// 			nPosZ_End	= lenInfo.dTop_Z - tempInfo.tndnSubXZ.aSpanInfo[nSzSpanXZ - 1].dDt1;
// 		}
// 		break;
// 	case TNDN_TEMPLATE_TYPE_STRAIGHT_BOT:
// 		{
// 			nPosZ_Start = -( lenInfo.dBot - tempInfo.tndnSubXZ.aSpanInfo[0].dDb );
// 			nPosZ_End	= -( lenInfo.dBot - tempInfo.tndnSubXZ.aSpanInfo[nSzSpanXZ - 1].dDb1 );
// 		}
// 		break;
// 	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE1:
// 	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE2:
// 	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE3:
// 	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE4:
// 	default:
// 		return FALSE;
// 		break;
// 	}
// 
// 	m_lengthCtrl->GetLengthInfoByPos(m_lengthCtrl->GetTotalLength(), lenInfo);
// 
// 	switch (tempInfo.tndnSubXY.nType)
// 	{
// 	case TNDN_TEMPLATE_TYPE_STRAIGHT_TOP:
// 	case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT1:
// 	case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT2:
// 	case TNDN_TEMPLATE_TYPE_CURVE:
// 		{
// 			nPosY_Start = lenInfo.dRight_Y - tempInfo.tndnSubXZ.aSpanInfo[0].dDt;
// 			nPosY_End	= lenInfo.dRight_Y - tempInfo.tndnSubXZ.aSpanInfo[nSzSpanXZ - 1].dDt1;
// 		}
// 		break;
// 	case TNDN_TEMPLATE_TYPE_STRAIGHT_BOT:
// 		{
// 			nPosY_Start = -( lenInfo.dLeft - tempInfo.tndnSubXZ.aSpanInfo[0].dDb );
// 			nPosY_End	= -( lenInfo.dLeft - tempInfo.tndnSubXZ.aSpanInfo[nSzSpanXZ - 1].dDb1 );
// 		}
// 		break;
// 	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE1:
// 	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE2:
// 	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE3:
// 	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE4:
// 	default:
// 		return FALSE;
// 		break;
// 	}

	return TRUE;
}

BOOL CDgnTendonTemplateCtrl::IsValidTndnTemplate( const TndnTemplate& tempInfo, BOOL bPrintMsg )
{
 	if(tempInfo.keyProperty == 0)
 		return FALSE;

	TndnTemplate tndnInfo = tempInfo;
	double tendonLength;
	int nErrMsg;

	MakeZeroSpanValue(tndnInfo);

	if(!GetTendonLength(tndnInfo, tendonLength, &nErrMsg))
	{
		PrintMsg(nErrMsg);
		return FALSE;
	}
 	
 	return TRUE;
}

double CDgnTendonTemplateCtrl::GetTdnaSub_Factor(double dPosX, double dFactor, UINT nType, BOOL bPositive, BOOL bSymmetry)
{
	lengthInfo info;
	double dt=0.0;
	
	m_lengthCtrl->GetLengthInfoByPos(dPosX, info);
	
	if(bPositive)
	{
		if(nType == 0) //XY
		{
			if(bSymmetry)	dt = - info.dLeft + dFactor;
			else			dt = info.dRight_Y - dFactor;
		}
		else if(nType == 1) //XZ
		{
			if(bSymmetry)	dt = - info.dBot + dFactor;
			else			dt = info.dTop_Z - dFactor;
		}
	}
	else
	{
		if(nType == 0) //XY
		{
			if(!bSymmetry)	dt = - info.dLeft + dFactor;
			else			dt = info.dRight_Y - dFactor;
		}
		else if(nType == 1) //XZ
		{
			if(!bSymmetry)	dt = - info.dBot + dFactor;
			else			dt = info.dTop_Z - dFactor;
		}
	}
	
	return dt;
}

BOOL CDgnTendonTemplateCtrl::GetTdnaSub( UINT nType, const caDouble& caSpanLength, double dOffFront,
										const TndnTemplateSub* pSub, caTdnaBase2D& aProfile)
{	//nType 0 : XY, 1 : XZ

	BOOL bRetVal = TRUE;

	switch (pSub->nType)
	{
	case TNDN_TEMPLATE_TYPE_STRAIGHT:
	case TNDN_TEMPLATE_TYPE_CURVE_LINE:
	case TNDN_TEMPLATE_TYPE_CURVE_ST:
		bRetVal = GetTdnaSub_DtDt1(nType, caSpanLength, dOffFront, pSub, aProfile);
		break;
	case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1:
	case TNDN_TEMPLATE_TYPE_CURVE_BENT_1:
		bRetVal = GetTdnaSub_DtDbDt1(nType, caSpanLength, dOffFront, pSub, aProfile);
		break;
	case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2:
		bRetVal = GetTdnaSub_DtDbDb1Dt(nType, caSpanLength, dOffFront, pSub, aProfile);
		break;
	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1:
		bRetVal = GetTdnaSub_DbDb1Dt1(nType, caSpanLength, dOffFront, pSub, aProfile);
		break;
	case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2:
		bRetVal = GetTdnaSub_DtDbDb1(nType, caSpanLength, dOffFront, pSub, aProfile);
		break;
	default:
		bRetVal = FALSE;
		break;
	}

	return bRetVal;
}

BOOL CDgnTendonTemplateCtrl::GetTdnaSub_DtDt1( UINT nType, const caDouble& caSpanLength, double dOffFront,
																							const TndnTemplateSub* pSub, caTdnaBase2D& aProfile )
{
	double dTemp;
	int i;
	BOOL bSymm = pSub->bXSymmetry;

	int nSize = (int)pSub->aSpanInfo.size();

	if(nSize != caSpanLength.GetSize())
		return FALSE;

	aProfile.SetSize( nSize + 1 );
	for(i = 0 ; i < aProfile.GetSize() ; i++) aProfile[i].Initiaize();
	
	dTemp = dOffFront;
	aProfile[0].x = dTemp;

	for(i = 0 ; i < nSize; i++)
	{
		dTemp += caSpanLength[i];
		aProfile[i + 1].x = dTemp;
	}
	
	aProfile[0].yz = GetTdnaSub_Factor(aProfile[0].x, pSub->aSpanInfo[0].dDt, nType, TRUE, bSymm);

	for(i = 0 ; i < nSize ; i++)
	{
		if(pSub->bSameSpanInfo)
		{
			aProfile[i + 1].yz = GetTdnaSub_Factor(aProfile[i + 1].x, pSub->aSpanInfo[0].dDt1, nType, TRUE, bSymm);
		}
		else
		{
			aProfile[i + 1].yz = GetTdnaSub_Factor(aProfile[i + 1].x, pSub->aSpanInfo[i].dDt1, nType, TRUE, bSymm);
		}
	}

	return TRUE;
}

BOOL CDgnTendonTemplateCtrl::GetTdnaSub_DtDbDt1( UINT nType, const caDouble& caSpanLength, double dOffFront,
																								const TndnTemplateSub* pSub, caTdnaBase2D& aProfile )
{
	int i;
	BOOL bSymm = pSub->bXSymmetry;
	
	int nSize = (int)pSub->aSpanInfo.size();
	
	if(nSize != caSpanLength.GetSize())
		return FALSE;
	
	aProfile.SetSize((nSize * 2) + 1);
	for(i = 0 ; i < aProfile.GetSize() ; i++) aProfile[i].Initiaize();
	
	aProfile[0].x = dOffFront;
	
	for(i = 0 ; i < nSize; i++)
	{
		if(pSub->aSpanInfo[i].dDf >= caSpanLength[i])		return FALSE;

		if(pSub->bSameSpanInfo)  aProfile[(i * 2) + 1].x = aProfile[(i * 2)].x + pSub->aSpanInfo[0].dDf;
		else                     aProfile[(i * 2) + 1].x = aProfile[(i * 2)].x + pSub->aSpanInfo[i].dDf;

		aProfile[(i * 2) + 2].x = aProfile[(i * 2)].x + caSpanLength[i];
	}
	
	aProfile[0].yz = GetTdnaSub_Factor(aProfile[0].x, pSub->aSpanInfo[0].dDt, nType, TRUE, bSymm);
	
	for(i = 0 ; i < nSize ; i++)
	{
		if(pSub->bSameSpanInfo)
		{
	    aProfile[(i * 2) + 1].yz = GetTdnaSub_Factor(aProfile[(i * 2) + 1].x, pSub->aSpanInfo[0].dDb, nType, FALSE, bSymm);
	    aProfile[(i * 2) + 2].yz = GetTdnaSub_Factor(aProfile[(i * 2) + 2].x, pSub->aSpanInfo[0].dDt1, nType, TRUE, bSymm);
		}
		else
		{
			aProfile[(i * 2) + 1].yz = GetTdnaSub_Factor(aProfile[(i * 2) + 1].x, pSub->aSpanInfo[i].dDb, nType, FALSE, bSymm);
	    aProfile[(i * 2) + 2].yz = GetTdnaSub_Factor(aProfile[(i * 2) + 2].x, pSub->aSpanInfo[i].dDt1, nType, TRUE, bSymm);
		}
	}

	for(i = 1 ; i < aProfile.GetSize() - 1; i++)
	{
		aProfile[i].bFix = TRUE;
	}

	return TRUE;
}

BOOL CDgnTendonTemplateCtrl::GetTdnaSub_DtDbDb1Dt( UINT nType, const caDouble& caSpanLength, double dOffFront,
																									const TndnTemplateSub* pSub, caTdnaBase2D& aProfile )
{
		int i;
		BOOL bSymm = pSub->bXSymmetry;
	
		int nSize = (int)pSub->aSpanInfo.size();
	
		if(nSize != caSpanLength.GetSize())
				return FALSE;
	
		aProfile.SetSize((nSize * 3) + 1);
		for(i = 0 ; i < aProfile.GetSize() ; i++) aProfile[i].Initiaize();
	
		aProfile[0].x = dOffFront;
	
		for(i = 0; i < nSize; i++)
		{
				if(pSub->aSpanInfo[i].dDf + pSub->aSpanInfo[i].dDe > caSpanLength[i])	return FALSE;

				if (pSub->bSameSpanInfo)
				{
						aProfile[(i * 3) + 1].x = aProfile[(i * 3)].x + pSub->aSpanInfo[0].dDf;
						aProfile[(i * 3) + 2].x = aProfile[(i * 3)].x + caSpanLength[i] - pSub->aSpanInfo[0].dDe;
						aProfile[(i * 3) + 1].dRadius = pSub->aSpanInfo[0].dR1;
						aProfile[(i * 3) + 2].dRadius = pSub->aSpanInfo[0].dR2;
				}
				else
				{
						aProfile[(i * 3) + 1].x = aProfile[(i * 3)].x + pSub->aSpanInfo[i].dDf;
						aProfile[(i * 3) + 2].x = aProfile[(i * 3)].x + caSpanLength[i] - pSub->aSpanInfo[i].dDe;
						aProfile[(i * 3) + 1].dRadius = pSub->aSpanInfo[i].dR1;
						aProfile[(i * 3) + 2].dRadius = pSub->aSpanInfo[i].dR2;
				}
				aProfile[(i * 3) + 3].x = aProfile[(i * 3)].x + caSpanLength[i];
		}
	
		aProfile[0].yz = GetTdnaSub_Factor(aProfile[0].x, pSub->aSpanInfo[0].dDt, nType, TRUE, bSymm);
	
		for(i = 0 ; i < nSize ; i++)
		{
				if(pSub->bSameSpanInfo)
				{
						aProfile[(i * 3) + 1].yz = GetTdnaSub_Factor(aProfile[(i * 3) + 1].x, pSub->aSpanInfo[0].dDb, nType, FALSE, bSymm);
						aProfile[(i * 3) + 2].yz = GetTdnaSub_Factor(aProfile[(i * 3) + 2].x, pSub->aSpanInfo[0].dDb1, nType, FALSE, bSymm);
						aProfile[(i * 3) + 3].yz = GetTdnaSub_Factor(aProfile[(i * 3) + 3].x, pSub->aSpanInfo[0].dDt1, nType, TRUE, bSymm);
				}
				else
				{
						aProfile[(i * 3) + 1].yz = GetTdnaSub_Factor(aProfile[(i * 3) + 1].x, pSub->aSpanInfo[i].dDb, nType, FALSE, bSymm);
						aProfile[(i * 3) + 2].yz = GetTdnaSub_Factor(aProfile[(i * 3) + 2].x, pSub->aSpanInfo[i].dDb1, nType, FALSE, bSymm);
						aProfile[(i * 3) + 3].yz = GetTdnaSub_Factor(aProfile[(i * 3) + 3].x, pSub->aSpanInfo[i].dDt1, nType, TRUE, bSymm);
				}
		}

		return TRUE;
}

BOOL CDgnTendonTemplateCtrl::GetTdnaSub_DtDbDb1( UINT nType, const caDouble& caSpanLength, double dOffFront,
																							 const TndnTemplateSub* pSub, caTdnaBase2D& aProfile )
{
	int i;
	BOOL bSymm = pSub->bXSymmetry;
	
	int nSize = (int)pSub->aSpanInfo.size();
	
	if(nSize != caSpanLength.GetSize())
		return FALSE;
	if(nSize != 1) //요런 경우는 크기 1만....
		return FALSE;
	
	aProfile.SetSize(3);
	for(i = 0 ; i < aProfile.GetSize() ; i++) aProfile[i].Initiaize();
	
	aProfile[0].x = dOffFront;
	aProfile[1].x = dOffFront + pSub->aSpanInfo[0].dDf;
	aProfile[2].x = dOffFront + caSpanLength[0];
	
	aProfile[0].yz = GetTdnaSub_Factor(aProfile[0].x, pSub->aSpanInfo[0].dDt, nType, TRUE, bSymm);
	aProfile[1].yz = GetTdnaSub_Factor(aProfile[1].x, pSub->aSpanInfo[0].dDb, nType, FALSE, bSymm);
	aProfile[2].yz = GetTdnaSub_Factor(aProfile[2].x, pSub->aSpanInfo[0].dDb1, nType, FALSE, bSymm);
	
	return TRUE;	
}

BOOL CDgnTendonTemplateCtrl::GetTdnaSub_DbDb1Dt1( UINT nType, const caDouble& caSpanLength, double dOffFront,
																								const TndnTemplateSub* pSub, caTdnaBase2D& aProfile )
{
	int i;
	BOOL bSymm = pSub->bXSymmetry;
	
	int nSize = (int)pSub->aSpanInfo.size();
	
	if(nSize != caSpanLength.GetSize())
		return FALSE;
	if(nSize != 1) //요런 경우는 크기 1만....
		return FALSE;
	
	aProfile.SetSize(3);
	for(i = 0 ; i < aProfile.GetSize() ; i++) aProfile[i].Initiaize();
	
	aProfile[0].x = dOffFront;
	aProfile[1].x = dOffFront + pSub->aSpanInfo[0].dDf;
	aProfile[2].x = dOffFront + caSpanLength[0];
	
	aProfile[0].yz = GetTdnaSub_Factor(aProfile[0].x, pSub->aSpanInfo[0].dDb, nType, FALSE, bSymm);
	aProfile[1].yz = GetTdnaSub_Factor(aProfile[1].x, pSub->aSpanInfo[0].dDb1, nType, FALSE, bSymm);
	aProfile[2].yz = GetTdnaSub_Factor(aProfile[2].x, pSub->aSpanInfo[0].dDt1, nType, TRUE, bSymm);
	
	return TRUE;
}

BOOL CDgnTendonTemplateCtrl::GetProfileDrawInfo( const TndnTemplate& tndnTemplate, BOOL bXY, std::vector<double>& dPosX,
																								std::vector<double>& dPosYZ, std::vector<BOOL>& bInputPt )
{
	T_TDNA_D tTdnaData;
	
	dPosX.clear();
	dPosYZ.clear();
	bInputPt.clear();
	
	if(!GetTdnaElemType(tndnTemplate, tTdnaData, FALSE))
	{
		return FALSE;
	}
	CArray<T_NODE_D, T_NODE_D&> aTendonProfile, aTendonProfileXY, aTendonProfileXZ;
		CArray<BOOL, BOOL> aProfileInputPoint, aProfileInputPointXY, aProfileInputPointXZ;
	
	if(!m_pDoc->m_pAttrCtrl->calcTendonProfile(tTdnaData, aTendonProfile, aProfileInputPoint, 20, TRUE, FALSE, FALSE,
		&aTendonProfileXY, &aProfileInputPointXY, &aTendonProfileXZ, &aProfileInputPointXZ))
	{
		GClearHistory();
		return FALSE;
	}

	int i, nSize;
	double dLength;
	double dTendonLength;
	double dFactor;
	double dFront;

	if(bXY)
	{
		nSize = aTendonProfileXY.GetSize();

		if(nSize != aProfileInputPointXY.GetSize())
			return FALSE;
		
		if(!GetTendonLength(tndnTemplate, dTendonLength))
			return FALSE;

		dLength = aTendonProfileXY[nSize - 1].x - aTendonProfileXY[0].x;
		dFactor = dTendonLength / dLength;
		dFront = aTendonProfileXY[0].x;

		dPosX.resize(nSize);
		dPosYZ.resize(nSize);
		bInputPt.resize(nSize);

		for(i = 0 ; i < nSize ; i++)		dPosX[i] = aTendonProfileXY[i].x - dFront;
		for(i = 0 ; i < nSize ; i++)		dPosX[i] *= dFactor;
		for(i = 0 ; i < nSize ; i++)		dPosX[i] += 0.0/*tndnTemplate.dOffsetFront*/;
		for(i = 0 ; i < nSize ; i++)		dPosYZ[i] = aTendonProfileXY[i].y;
		for(i = 0 ; i < nSize ; i++)		bInputPt[i] = aProfileInputPointXY[i];
	}
	else
	{
		nSize = aTendonProfileXZ.GetSize();

		if(nSize != aProfileInputPointXZ.GetSize())
			return FALSE;
		
		if(!GetTendonLength(tndnTemplate, dTendonLength))
			return FALSE;

		dLength = aTendonProfileXZ[nSize - 1].x - aTendonProfileXZ[0].x;
		dFactor = dTendonLength / dLength;
		dFront = aTendonProfileXZ[0].x;
		
		dPosX.resize(nSize);
		dPosYZ.resize(nSize);
		bInputPt.resize(nSize);
		
		for(i = 0 ; i < nSize ; i++)		dPosX[i] = aTendonProfileXZ[i].x - dFront;
		for(i = 0 ; i < nSize ; i++)		dPosX[i] *= dFactor;
		for(i = 0 ; i < nSize ; i++)		dPosX[i] += 0.0/*tndnTemplate.dOffsetFront*/;
		for(i = 0 ; i < nSize ; i++)		dPosYZ[i] = aTendonProfileXZ[i].z;
		for(i = 0 ; i < nSize ; i++)		bInputPt[i] = aProfileInputPointXZ[i];
	}

	return TRUE;
}

double CDgnTendonTemplateCtrl::GetTotalLength()
{
	return GetLengthCtrl()->GetTotalLength();
}

BOOL CDgnTendonTemplateCtrl::GetTendonLength(const TndnTemplate& tndnTemplate, double& dTndnLen, int* errMsg)
{
	int i;
	double dTemp, dTendonLength;
	CString strTemp;
	caDouble caSpanLength;

	if(tndnTemplate.tndnSubXY.nNumOfSpan != tndnTemplate.tndnSubXY.aSpanInfo.size())
	{
		SET_ERR_MSG_RETURN(errMsg, EN_TNDN_TEMPLATE_ERR_MSG_XY_SPANNUM, FALSE)
	}
	if(tndnTemplate.tndnSubXZ.nNumOfSpan != tndnTemplate.tndnSubXZ.aSpanInfo.size())
	{
		SET_ERR_MSG_RETURN(errMsg, EN_TNDN_TEMPLATE_ERR_MSG_XZ_SPANNUM, FALSE)
	}

	//XY
	strTemp = tndnTemplate.tndnSubXY.strSpanLength;
	CStrParser::GetUEDistance(strTemp, caSpanLength);
	
	if(tndnTemplate.tndnSubXY.nNumOfSpan != caSpanLength.GetSize()) //생성 결과과 입력된 스팬의 길이가 다르다.
	{
		SET_ERR_MSG_RETURN(errMsg, EN_TNDN_TEMPLATE_ERR_MSG_XY_SPANNUM, FALSE)
	}
	
	dTemp = 0.0;
	for(i = 0 ; i < caSpanLength.GetSize(); i++)
	{
		double dTempSum = tndnTemplate.tndnSubXY.aSpanInfo[i].dDe + tndnTemplate.tndnSubXY.aSpanInfo[i].dDf;
		if(dTempSum >= caSpanLength[i])
		{
			SET_ERR_MSG_RETURN(errMsg, EN_TNDN_TEMPLATE_ERR_MSG_XY_DFDE, FALSE)
		}
		dTemp += caSpanLength[i];
	}
	dTendonLength = dTemp;
	
	//XZ
	strTemp = tndnTemplate.tndnSubXZ.strSpanLength;
	CStrParser::GetUEDistance(strTemp, caSpanLength);

	if(tndnTemplate.tndnSubXZ.nNumOfSpan != caSpanLength.GetSize()) //생성 결과과 입력된 스팬의 길이가 다르다.
	{
		SET_ERR_MSG_RETURN(errMsg, EN_TNDN_TEMPLATE_ERR_MSG_XZ_SPANNUM, FALSE)
	}
	
	dTemp = 0.0;
	for(i = 0 ; i < caSpanLength.GetSize(); i++)
	{
		double dTempSum = tndnTemplate.tndnSubXZ.aSpanInfo[i].dDe + tndnTemplate.tndnSubXZ.aSpanInfo[i].dDf;
		if(dTempSum >= caSpanLength[i])
		{      
			SET_ERR_MSG_RETURN(errMsg, EN_TNDN_TEMPLATE_ERR_MSG_XZ_DFDE, FALSE)
		}
		dTemp += caSpanLength[i];
	}

	if( fabs(dTemp - dTendonLength) >= 1E-3 )
	{
		SET_ERR_MSG_RETURN(errMsg, EN_TNDN_TEMPLATE_ERR_MSG_DIFF_SPAN_LEN, FALSE)
	}
	
	dTndnLen = dTendonLength;
	return TRUE;
}

void CDgnTendonTemplateCtrl::GetSameSpanStr(const TndnTemplate& tndnTemplate, BOOL bXY, CString& strSpan)
{
	int i;
	int nSpanNum;
	double dTotalTendonLength;
	double dSpanLength;
	double dTotalLength = GetTotalLength();
	CString strOffset;
	
	double dLengthFront = 0.0; //tndnTemplate.dOffsetFront;
	double dLengthEnd = 0.0; //tndnTemplate.dOffsetEnd;
	
	caDouble aSpan;
	
	if(bXY)	nSpanNum = tndnTemplate.tndnSubXY.nNumOfSpan;
	else	nSpanNum = tndnTemplate.tndnSubXZ.nNumOfSpan;
	
	dTotalTendonLength = dTotalLength - (dLengthFront + dLengthEnd);
	
	dSpanLength = dTotalTendonLength / nSpanNum;
	
	aSpan.SetSize(nSpanNum);
	for(i = 0 ; i < nSpanNum; i++)
	{
		aSpan[i] = dSpanLength;
	}
	
	strSpan = CStrParser::ConvtDistArr2DistStr(aSpan);
}

void CDgnTendonTemplateCtrl::OnchangeTotalLength()
{
	int i;
	vecTndnKey keyList;
	int nSize = GetKeyList(keyList);
	TndnTemplate tempInfo;

	for(i = 0 ; i < nSize; i++)
	{
		if(!GetTemplateNU(keyList[i], tempInfo))
		{
			ASSERT(0);
			continue;
		}

		if(tempInfo.bSameSpanInterval)
		{
			GetSameSpanStr(tempInfo, TRUE, tempInfo.tndnSubXY.strSpanLength);
			GetSameSpanStr(tempInfo, FALSE, tempInfo.tndnSubXZ.strSpanLength);
		}

		ModTemplateNU(keyList[i], tempInfo);
	}
}

BOOL CDgnTendonTemplateCtrl::AutoGeneration( BOOL bReset, T_TDNT_K tdntKey, T_TDGR_K tdgrKey, const CString& namePrefix,
																						const CString& strCode, const CString& strT, const CString& strN, double* dOrigin )
{
	// 선택한 요소의 단면 정보가 모두 같다면 실행한다.
	if(m_bAllSameSect)
	{
		mapTndnTemp             mapTemplate_Temp;
		std::map<CString, UINT> mapNameSuffix_Temp;
		if(bReset)
		{
			mapTemplate_Temp = m_mapTndnTemplate;
			mapNameSuffix_Temp = m_mapNameSuffix;
			ClearTemplate();
		}

		T_AUTOTNDN_INFO autoInfo;
	autoInfo.strDBCode = strCode;
		autoInfo.strDBName = strT;
		autoInfo.strDBNameSub = strN;
		autoInfo.dOrigin[0] = dOrigin[0];
		autoInfo.dOrigin[1] = dOrigin[1];

		BOOL bRetVal = m_pAutoGenCtrl->AutoGeneration(m_keySect, tdntKey, tdgrKey, namePrefix, autoInfo);
		if(!bRetVal) //실패했을때는 지운걸 다시 살린다.
		{
			if(bReset)
			{
				m_mapTndnTemplate = mapTemplate_Temp;
				m_mapNameSuffix = mapNameSuffix_Temp;
			}
		}

		return bRetVal;
	}

	return FALSE;
}

void CDgnTendonTemplateCtrl::CancelNewNameNum( const CString strPrefix )
{
	std::map<CString, UINT>::iterator itrMap;
	itrMap = m_mapNameSuffix.find(strPrefix);
	if(itrMap != m_mapNameSuffix.end())
	{
		m_mapNameSuffix[strPrefix] = itrMap->second - 1;
	}
}

void CDgnTendonTemplateCtrl::GetNewName( const CString& strPrefix, CString& newName )
{
	UINT suffixIdx;

	std::map<CString, UINT>::iterator itrStr = m_mapNameSuffix.find(strPrefix);

	if(itrStr == m_mapNameSuffix.end())
		suffixIdx = 1;
	else
		suffixIdx = itrStr->second;

	BOOL bExistTemplate;
	BOOL bExistDB;
	while(TRUE)
	{
		newName.Format(_T("%s_%03d"), strPrefix, suffixIdx++);
		bExistTemplate = IsExistTemplate(newName);
		bExistDB = m_pAttrCtrl->ExistTdna(newName);
		if(bExistDB == FALSE && bExistTemplate == FALSE)
			break;
	}

	m_mapNameSuffix[strPrefix] = suffixIdx;
}

BOOL CDgnTendonTemplateCtrl::IsExistTemplate( CString& strName )
{
	vecTndnKey tndnKeyList;

	int i;
	int nSize = GetKeyList(tndnKeyList);
	
	TndnTemplate tndnInfo;
	for(i = 0 ; i < nSize; i++)
	{
		if(!GetTemplateNU(tndnKeyList[i], tndnInfo))
		{
			ASSERT(0);
			continue;
		}
		
		if(tndnInfo.strName.CompareNoCase(strName) == 0) return TRUE;
	}

	return FALSE;	
}

BOOL CDgnTendonTemplateCtrl::GetPosByElem( T_ELEM_K tElemKey, double dPosElem, double& dPos )
{
	int i;
	int nSize = (int)m_vecElemKey.size();
	double dLength = 0.0;
	BOOL bFind = FALSE;
	for(i = 0 ; i < nSize; i++)
	{
		if(m_vecElemKey[i] == tElemKey)
		{
			lengthInfo lengInfoI;
			lengthInfo lengInfoJ;
			if(m_lengthCtrl->GetLengInfoByElem(tElemKey, &lengInfoI, &lengInfoJ))
			{
				dLength += ((lengInfoJ.dPosX - lengInfoI.dPosX) * dPosElem);
				bFind = TRUE;
				break;
			}
		}
		else
		{
			lengthInfo lengInfoI;
			lengthInfo lengInfoJ;
			if(m_lengthCtrl->GetLengInfoByElem(tElemKey, &lengInfoI, &lengInfoJ))
			{
				dLength += (lengInfoJ.dPosX - lengInfoI.dPosX);
			}
		}
	}

	if(bFind)
	{
		dPos = dLength;
		return TRUE;
	}

	return FALSE;
}

BOOL CDgnTendonTemplateCtrl::GetDefaultSpanValue( TndnTemplate& tempInfo, BOOL bXY, int nfromIdx, BOOL bDfDe, BOOL bDtDb)
{
	int i, nSize;
	TNDN_TEMPLATE_TYPE nType;
	TndnTemplateSub* pSub = NULL;

	MakeZeroSpanValue(tempInfo);

	if(bXY)
	{
		nSize = (int)tempInfo.tndnSubXY.aSpanInfo.size();
		pSub = &(tempInfo.tndnSubXY);
	}
	else
	{
		nSize = (int)tempInfo.tndnSubXZ.aSpanInfo.size();
		pSub = &(tempInfo.tndnSubXZ);
	}

	if (nfromIdx >= nSize)
	    return FALSE;

	nType = pSub->nType;
	caDouble caSpanLength;
	CString strLength = pSub->strSpanLength;
	CStrParser::GetUEDistance(strLength, caSpanLength);

	//double dInitVal = m_pUnitCtrl->ConvertUnitDataOut(D_UNITSYS_BASE_LENGTH, 0.1);

	if (nSize != caSpanLength.GetSize())
	{
		return FALSE;
		}

		double r1 = 0.0, r2 = 0.0;
		int idxR = (nfromIdx > 0) ? nfromIdx-1 : 0;
		r1 = pSub->aSpanInfo[idxR].dR1;
		r2 = pSub->aSpanInfo[idxR].dR2;

		double dCurPos = 0.0;//tempInfo.dOffsetFront;
		for(i = 0 ; i < nfromIdx; i++)
		{
				dCurPos += caSpanLength[i];
		}

	for(i = nfromIdx ; i < nSize; i++)
	{
		switch(nType)
		{
		case TNDN_TEMPLATE_TYPE_STRAIGHT:
		case TNDN_TEMPLATE_TYPE_CURVE_ST:
			{
				if(bDtDb)
				{
					pSub->aSpanInfo[i].dDt = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos, 0.5);
					pSub->aSpanInfo[i].dDt1 = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + caSpanLength[i], 0.5);
				}
				break;
			}
		case TNDN_TEMPLATE_TYPE_CURVE_BENT_1:
		case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1:
			{
				if(bDfDe)
				{
					pSub->aSpanInfo[i].dDf = 0.5 * caSpanLength[i];
				}
				if(bDtDb)
				{
					pSub->aSpanInfo[i].dDt = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos, 0.1);
					pSub->aSpanInfo[i].dDb = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + pSub->aSpanInfo[i].dDf, 0.1);
					pSub->aSpanInfo[i].dDt1 = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + caSpanLength[i], 0.1);;
				}
				break;
			}
		case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2:
			{
				if(bDfDe)
				{
					pSub->aSpanInfo[i].dDf = 0.3333 * caSpanLength[i];
					pSub->aSpanInfo[i].dDe = 0.3333 * caSpanLength[i];
					pSub->aSpanInfo[i].dR1 = r1;
					pSub->aSpanInfo[i].dR2 = r2;
				}
				if(bDtDb)
				{
					pSub->aSpanInfo[i].dDt = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos, 0.1);
					pSub->aSpanInfo[i].dDb = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + pSub->aSpanInfo[i].dDf, 0.1);
					pSub->aSpanInfo[i].dDb1 = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + caSpanLength[i] - pSub->aSpanInfo[i].dDe, 0.1);
					pSub->aSpanInfo[i].dDt1 = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + caSpanLength[i], 0.1);
				}
				break;
			}
		case TNDN_TEMPLATE_TYPE_CURVE_LINE:
			{
				if(bDtDb)
				{
					pSub->aSpanInfo[i].dDt = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos, 0.1);
					pSub->aSpanInfo[i].dDt1 = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + caSpanLength[i], 0.1);
				}
				break;
			}
		case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1:
			{
				pSub->aSpanInfo[i].dDf = 0.5 * caSpanLength[i];
				pSub->aSpanInfo[i].dDb = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos, 0.1);
				pSub->aSpanInfo[i].dDb1 = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + pSub->aSpanInfo[i].dDf, 0.3);
				pSub->aSpanInfo[i].dDt1 = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + caSpanLength[i], 0.1);
				break;
			}
		case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2:
			{
				pSub->aSpanInfo[i].dDf = 0.5 * caSpanLength[i];
				pSub->aSpanInfo[i].dDt = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos, 0.1);;
				pSub->aSpanInfo[i].dDb = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + pSub->aSpanInfo[i].dDf, 0.3);
				pSub->aSpanInfo[i].dDb1 = m_lengthCtrl->GetLengthRatioByPos(bXY, dCurPos + caSpanLength[i], 0.1);
				break;
			}
		default:
			ASSERT(0);
			break;
		}
		//dCurPos에 더한다.
		dCurPos += caSpanLength[i];
	}

	return TRUE;
}

BOOL CDgnTendonTemplateCtrl::ConvertRatioLength_DfDe( BOOL bRatio2Len, TndnTemplate& tndnData )
{
	int j, k;
	TndnTemplateSub* pSub;

	for(j = 0 ; j < 2; j ++)
	{
		if (j == 0) pSub = &(tndnData.tndnSubXY);
		if (j == 1) pSub = &(tndnData.tndnSubXZ);
		int nSizeInfo = (int)pSub->aSpanInfo.size();
		
		caDouble caSpanLength;
		CString strLength = pSub->strSpanLength;
		CStrParser::GetUEDistance(strLength, caSpanLength);
		
		if(nSizeInfo != caSpanLength.GetSize())
		{
			return FALSE;
		}

		if(bRatio2Len)
		{
			for(k = 0 ; k < nSizeInfo; k++)
			{
				pSub->aSpanInfo[k].dDf = pSub->aSpanInfo[k].dDf * caSpanLength[k];
				pSub->aSpanInfo[k].dDe = pSub->aSpanInfo[k].dDe * caSpanLength[k];
			}
		}
		else
		{
			for(k = 0 ; k < nSizeInfo; k++)
			{
				pSub->aSpanInfo[k].dDf = pSub->aSpanInfo[k].dDf / caSpanLength[k];
				pSub->aSpanInfo[k].dDe = pSub->aSpanInfo[k].dDe / caSpanLength[k];
			}      
		}    
	}  
	return TRUE;
}

void CDgnTendonTemplateCtrl::PrintMsg( int nErrMsg )
{
//   if(m_nPreErrMsg != nErrMsg)   m_nPreErrMsg = nErrMsg;
//   else                          return;

	GClearHistory();
	switch(nErrMsg)
	{
	case EN_TNDN_TEMPLATE_ERR_MSG_INVALID_ELEM:
		m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_INVALID_ELEM));
		break;
	case EN_TNDN_TEMPLATE_ERR_MSG_XY_SPANNUM:
		m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_XY_SPANNUM));
		break;
	case EN_TNDN_TEMPLATE_ERR_MSG_XZ_SPANNUM:
		m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_XZ_SPANNUM));
		break;
	case EN_TNDN_TEMPLATE_ERR_MSG_XY_DFDE:
		m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_XY_DFDE));
		break;
	case EN_TNDN_TEMPLATE_ERR_MSG_XZ_DFDE:
		m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_XZ_DFDE));
		break;
	case EN_TNDN_TEMPLATE_ERR_MSG_DIFF_SPAN_LEN:
		m_pDoc->DisplayHistoryMessage(_LS(IDS_CMD_TNDN_TEMPLATE_MSG_WARN_DIFF_SPAN_LEN));
		break;
	}  
}

void CDgnTendonTemplateCtrl::MakeZeroSpanValue( TndnTemplate& TempInfo )
{  
	int i, j, nSize;
	TndnTemplateSub* pSub;

	for(i = 0 ; i < 2; i++)
	{
		if(i == 0)  pSub = &(TempInfo.tndnSubXY);
		if(i == 1)  pSub = &(TempInfo.tndnSubXZ);

		TNDN_TEMPLATE_TYPE nType = pSub->nType;
		nSize = (int)pSub->aSpanInfo.size();

		for(j = 0 ; j < nSize; j++)
		{
			switch(nType)
			{
			case TNDN_TEMPLATE_TYPE_STRAIGHT:
			case TNDN_TEMPLATE_TYPE_CURVE_LINE:
			case TNDN_TEMPLATE_TYPE_CURVE_ST:
				pSub->aSpanInfo[j].dDb = 0.0;
				pSub->aSpanInfo[j].dDb1 = 0.0;
				pSub->aSpanInfo[j].dDf = 0.0;
				pSub->aSpanInfo[j].dDe = 0.0;
				pSub->aSpanInfo[j].dDRad = 0.0;
				pSub->aSpanInfo[j].dR1 = 0.0;
				pSub->aSpanInfo[j].dR2 = 0.0;
				break;
			case TNDN_TEMPLATE_TYPE_CURVE_BENT_1:
			case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1:
				pSub->aSpanInfo[j].dDb1 = 0.0;
				pSub->aSpanInfo[j].dDe = 0.0;
				pSub->aSpanInfo[j].dDRad = 0.0;
				pSub->aSpanInfo[j].dR1 = 0.0;
				pSub->aSpanInfo[j].dR2 = 0.0;
				break;
			case TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2:
				pSub->aSpanInfo[j].dDRad = 0.0;
				break;
			case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1:
				pSub->aSpanInfo[j].dDt = 0.0;
				pSub->aSpanInfo[j].dDe = 0.0;
				pSub->aSpanInfo[j].dDRad = 0.0;
				pSub->aSpanInfo[j].dR1 = 0.0;
				pSub->aSpanInfo[j].dR2 = 0.0;
				break;
			case TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2:
				pSub->aSpanInfo[j].dDt1 = 0.0;
				pSub->aSpanInfo[j].dDe = 0.0;
				pSub->aSpanInfo[j].dDRad = 0.0;
				pSub->aSpanInfo[j].dR1 = 0.0;
				pSub->aSpanInfo[j].dR2 = 0.0;
				break;
			default:
				break;
			}
		}
	}  
}

BOOL CDgnTendonTemplateCtrl::GetTdnaCurveType(const TndnTemplate& tndnTemp, T_TDNA_D& tTdna, BOOL bConvertOut, BOOL bMsg /*= FALSE*/)
{
	std::vector<T_ELEM_K>& elemKeyList = *GetAssignElemList();
	int nSize = (int)elemKeyList.size();
	if(nSize < 2)
		return FALSE;
	
	T_ELEM_K keyElem_Start = elemKeyList[0]; //I를 본다.
	T_ELEM_K keyElem_Mid   = elemKeyList[nSize / 2]; //I를 본다.
	T_ELEM_K keyElem_End   = elemKeyList[nSize - 1]; //J를 본다.

	if(!GetTdnaElemType(tndnTemp, tTdna, bConvertOut, bMsg))  return FALSE;

	BOOL bCheckBent_Z = TRUE;
	//Z방향에 대해서 굽힌것은 허용하지 않는다.
	if(bCheckBent_Z && nSize > 1)
	{
		T_ELEM_D eData;
		T_NODE_D nData1;
		T_NODE_D nData2;
		
		int i, NumNode;
		double dTemp, dStandardDiff, dTempBot;
		for(i = 0 ; i < nSize; i++)
		{
			if(!m_pAttrCtrl->GetElemFromBaseDB(elemKeyList[i], eData))
				continue;
			
			NumNode = m_pAttrCtrl->GetNumNode(eData.eltyp, eData.elnod);
			if(NumNode != 2)      continue;
			
			if(!m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[0], nData1))
				continue;
			if(!m_pAttrCtrl->GetNodeFromBaseDB(eData.elnod[1], nData2))
				continue;

			dTempBot = nData2.x - nData1.x;
			if(fabs(dTempBot) <= 1E-5)
				dTempBot = nData2.y - nData1.y;
			if(fabs(dTempBot) <= 1E-5)
			{
				ASSERT(0);
				return FALSE;
			}

			if(i == 0)
				dStandardDiff = (nData2.z - nData1.z) / dTempBot; //기울기를 구한다.
			else
			{
				dTemp = (nData2.z - nData1.z) / dTempBot;

				if(fabs(dStandardDiff - dTemp) >= 1E-5) //기울기가다른게 존재한다면.
				{
					return FALSE;
				}
			}
		}
	}

	double dPosStart[3];
	double dPosCenter[2];
	double dRadAxis;
	int nCWType;
	if(!m_lengthCtrl->GetCircleStartCenterPos(keyElem_Start, keyElem_Mid, keyElem_End, dPosStart, dPosCenter, dRadAxis, nCWType))
		return FALSE;

	tTdna.nTendonShape = 1;
	tTdna.dInsertPointX = dPosStart[0];
	tTdna.dInsertPointY = dPosStart[1];
	tTdna.dInsertPointZ = dPosStart[2];
	tTdna.dCenterX = dPosCenter[0];
	tTdna.dCenterY = dPosCenter[1];
	tTdna.nGradRotAxis = 1;
	tTdna.dGradRotAngle = dRadAxis;
	tTdna.nCurveDir = nCWType;
	
	return TRUE;
}

void CDgnTendonTemplateCtrl::SetExistSpanInfo()
{
	int i, j;
	int nSize, nSizeJ;
	T_SPAN_D data;
	CArray<T_SPAN_K, T_SPAN_K> rKeyList;

	m_pDoc->m_pAttrCtrl->GetSpanKeyList(rKeyList);

	nSize = rKeyList.GetSize();

	m_keySpan = 0;
	for(i = 0 ; i < nSize; i++)
	{
		T_SPAN_K nSelK = rKeyList[i];
		if(!m_pDoc->m_pAttrCtrl->GetSpan(nSelK, data)) continue;
		nSizeJ = data.aElemBase.GetSize();
		vecElemKey elemKeyList;
		elemKeyList.resize(nSizeJ);
		for(j = 0 ; j < nSizeJ; j++)
		{
			elemKeyList[j] = data.aElemBase[j].ElemK;
		}
		if(IsSameElemList(elemKeyList))
		{
			m_keySpan = nSelK;
			break;
		}
	}  
}

BOOL CDgnTendonTemplateCtrl::IsSameElemList( const vecElemKey& keyListComp )
{
	int nSizeOrg = (int)m_vecElemKey.size();
	int nSizeComp = (int)keyListComp.size();
	int i, j;

	if(nSizeOrg != nSizeComp)
	{
		return FALSE;
	}

	BOOL bExist = FALSE;
	for(i = 0 ; i < nSizeComp; i++)
	{
		bExist = FALSE;
		for(j = 0 ; j < nSizeOrg; j++)
		{
			if(keyListComp[i] == m_vecElemKey[j])
			{
				bExist = TRUE;
				break;
			}  
		}
		if(!bExist)
			return FALSE;
	}

	return TRUE; 
}

BOOL CDgnTendonTemplateCtrl::GetInitSpanInfo( TndnTemplate& tempInfo )
{
	if(m_keySpan <= 0)
		return FALSE;
	
	T_SPAN_D data;
	if(!m_pDoc->m_pAttrCtrl->GetSpan(m_keySpan, data))
		return FALSE;

	int i;
	int nSize = data.aSpan.GetSize();
	caDouble aSpanInfo;

	if(nSize > 0) //Span Information에서 Exact 일때는 이쪽으로 들어온다.
	{
		aSpanInfo.SetSize(nSize);
		for(i = 0 ; i < nSize; i++)
		{
			aSpanInfo[i] = data.aSpan[i];
		}    
	}
	else          //아니면 Span Length를 직접 계산해준다.
	{
		m_pAttrCtrl->GetSpanLengthByElementLength(data, aSpanInfo);
		nSize = aSpanInfo.GetSize();
	}

	tempInfo.tndnSubXY.SetSpanNum(nSize);
	tempInfo.tndnSubXY.strSpanLength = CStrParser::ConvtDistArr2DistStr(aSpanInfo);
	if(!GetDefaultSpanValue(tempInfo, TRUE, 0, TRUE, TRUE))
		return FALSE;
	
	tempInfo.tndnSubXZ.SetSpanNum(nSize);
	tempInfo.tndnSubXZ.strSpanLength = CStrParser::ConvtDistArr2DistStr(aSpanInfo);
	if(!GetDefaultSpanValue(tempInfo, FALSE, 0, TRUE, TRUE))
		return FALSE;

	return TRUE;
}

T_SECT_K CDgnTendonTemplateCtrl::GetSectKeyByElemsStr( const CString& strElems )
{
	T_SECT_K sectKey = 0;
	vecElemKey keyList;  
	CArray<int,int> aElem;

	if(CStrParser::ParsingListByTo(strElems, aElem))
	{    
		int i;
		int nSize = aElem.GetSize();
		keyList.resize(nSize);
		for( i = 0 ; i < nSize; i++)
		{
			keyList[i] = aElem[i];
		}

		if(!GetLengthCtrl()->IsAllSameSect(keyList, sectKey))
			sectKey = 0;
	}

	return sectKey;
}

BOOL CDgnTendonTemplateCtrl::GetAutoGenSectName( CString& strCode, CString& strType, CString& strName )
{
	strCode.Empty();
	strType.Empty();
	strName.Empty();
	
	// 선택한 요소의 단면 정보가 모두 같다면 실행한다.
	if(m_bAllSameSect)
	{
		return m_pAutoGenCtrl->GetSectTypeName(m_keySect, strCode, strType, strName);
	}

	return FALSE;  
}