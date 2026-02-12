// LoadCombDefCtrl.cpp: implementation of the CLoadCombCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "LoadCombDefCtrl.h"
#include "LoadCombCtrl.h"
#include "..\\wg_db\\wg_db_AttrCtrl.h"
#include "..\\wg_db\\wg_db_AttrCtrl2.h"
#include "..\\wg_db\\wg_db_PostCtrl.h"
#include "..\\wg_db\\wg_db_DataCtrl.h"
#include "..\\wg_db\\wg_db_UndoCtrl.h"
#include "..\\wg_db\\wg_db_EditData.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\mvLoad.h"
#include "..\wg_db\DB_ST_DN_JUD.h"
#include "..\wg_db\VehlDef.h"
#include "..\wg_db\LateralLoad.h"
#include "..\wg_db\AluDgnTool.h"

#include <math.h>

#include "..\wg_base\wg_base_AppBase.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_CompFunc.h"

//
#include <fstream>       // for ofstream
using namespace std;
//

CLoadCombDefCtrl::CLoadCombDefCtrl()
{
	 m_aImperfectionLoad.RemoveAll();
	 m_tempLcom.RemoveAll();
	 m_nOption = 0;
	 m_strCodeName = _T("");
	 for (int i=0; i<6; i++)
	 {
		 m_bChkLCType[i] = FALSE;
	 }
}

CLoadCombDefCtrl::~CLoadCombDefCtrl()
{
	
}

BOOL CLoadCombDefCtrl::Set_DefaultLoadComb(int nOption, CLoadCombDefineData* pData)
{
	m_bStandComb = FALSE;
	m_nOption = nOption;
	m_bImperfection = pData->m_bImperfection;
	m_bGB55006_21 = pData->m_strCode == _T("GB55006-2021") ? TRUE : FALSE;
	return autoGenerate(pData) & autoGenerate_Stand(pData);
}

BOOL CLoadCombDefCtrl::autoGenerate_Stand(CLoadCombDefineData* pData)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_bStandComb = TRUE;
	m_bImperfection = FALSE;
	CLoadCombDefinePara para(pData);
	if (!generationType(&para))
		return FALSE;
	CLcomCombAutoRlt rltPool;
	generateDoubleDirC(&para);
	generatorDoubleDirC(&para, &rltPool);
	int nSize = pData->m_item.m_aFactor.GetCount();
	CaseCombInfoArr cbxInfo;
	for (int i = 0; i < nSize; i++)
	{
		if (getClassfyInfo(&para, i, cbxInfo) == FALSE)
			continue;

		for (int k = 0; k < cbxInfo.GetCount(); k++)
		{
			generatorLcomb(&para, &cbxInfo[k], &rltPool);
		}
	}
	int nType = 0;

	CArray<T_LCOM_K, T_LCOM_K> rKeyList;
	if (m_bChkLCType[0] == TRUE)
	{
		nType = D_LCOMTYPE_GENERAL;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}
	if (m_bChkLCType[1] == TRUE)
	{
		nType = D_LCOMTYPE_STEEL;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}
	if (m_bChkLCType[2] == TRUE)
	{
		nType = D_LCOMTYPE_CONCRETE;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}
	if (m_bChkLCType[3] == TRUE)
	{
		nType = D_LCOMTYPE_SRC;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}
	if (m_bChkLCType[4] == TRUE)
	{
		nType = D_LCOMTYPE_CFSTEEL;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}
	if (m_bChkLCType[5] == TRUE)
	{
		nType = D_LCOMTYPE_ALUMINUM;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}

	return TRUE;
}

BOOL CLoadCombDefCtrl::autoGenerate(CLoadCombDefineData* pData)
{
	m_pDoc = CDBDoc::GetDocPoint();

	CLoadCombDefinePara para(pData);
	if(!generationType (&para))
		return FALSE;
	CLcomCombAutoRlt rltPool;
	generateDoubleDirC(&para);
	generatorDoubleDirC(&para, &rltPool);
	int nSize = pData->m_item.m_aFactor.GetCount();
	CaseCombInfoArr cbxInfo;
	for (int i=0; i<nSize; i++)
	{		
		if(getClassfyInfo(&para, i, cbxInfo) == FALSE)
			continue;

		for (int k=0; k<cbxInfo.GetCount(); k++)
		{
			generatorLcomb(&para, &cbxInfo[k], &rltPool);
		}		
	}
	int nType = 0;
	
	CArray<T_LCOM_K, T_LCOM_K> rKeyList;
	if (m_bChkLCType[0] == TRUE)
	{
		nType = D_LCOMTYPE_GENERAL;
		if (m_nOption == 1)
			Delete_AllLoadComb(nType);
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}
	if (m_bChkLCType[1] == TRUE)
	{
		nType = D_LCOMTYPE_STEEL;
		if (m_nOption == 1)
			Delete_AllLoadComb(nType);
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}
	if (m_bChkLCType[2] == TRUE)
	{
		nType = D_LCOMTYPE_CONCRETE;
		if (m_nOption == 1)
			Delete_AllLoadComb(nType);
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}
	if (m_bChkLCType[3] == TRUE)
	{
		nType = D_LCOMTYPE_SRC;
		if (m_nOption == 1)
			Delete_AllLoadComb(nType);
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}
	if (m_bChkLCType[4] == TRUE)
	{
		nType = D_LCOMTYPE_CFSTEEL;
		if (m_nOption == 1)
			Delete_AllLoadComb(nType);
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}
	if (m_bChkLCType[5] == TRUE)
	{
		nType = D_LCOMTYPE_ALUMINUM;
		if (m_nOption == 1)
			Delete_AllLoadComb(nType);
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(nType, rKeyList);
		AddComb2Temp(rKeyList, nType);
		save2DB(&rltPool, nType);
	}

	return TRUE;
}

void CLoadCombDefCtrl::AddComb2Temp(const CArray<T_LCOM_K, T_LCOM_K> &rKeyList, int nLcomType)
{
	m_tempLcom.RemoveAll();
	T_LCOM_D LcomD;
	for (int i=0; i<rKeyList.GetCount(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, rKeyList[i], LcomD))
			continue;
		m_tempLcom.Add(LcomD);
	}
}

BOOL CLoadCombDefCtrl::Delete_AllLoadComb(int nType)
{
	CArray<CString, CString&> strLoadCombName;
	m_pDoc->m_pAttrCtrl->GetLcomList(nType,strLoadCombName);
	if(strLoadCombName.GetSize() == 0) return TRUE;
	else
	{
		if(!m_pDoc->m_pDataCtrl->DelLcom(nType,strLoadCombName))	return FALSE;
	}
	return TRUE;
}

BOOL CLoadCombDefCtrl::generationType (CLoadCombDefinePara *pPara)
{

	pPara->m_mapTypeClassfy[NAME_LOADCASE_D]  = Type_A;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_L]  = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_LR] = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_W]  = Type_BC;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_E]  = Type_C;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_EVT]= Type_CV;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_S]  = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_T]  = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_PS] = Type_A;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_EH] = Type_A;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_EV] = Type_A;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_WP] = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_FP] = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_SF] = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_B]  = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_CS] = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_CR] = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_SH] = Type_B;
	pPara->m_mapTypeClassfy[NAME_LOADCASE_COMB] = Type_C;


	CArray<T_STLD_K, T_STLD_K> rStldKeyLst;
	CArray<T_STLD_K, T_STLD_K> arExistStldKeyLst;
	if (!m_pDoc->m_pAttrCtrl2->GetUsedStldKeyList(arExistStldKeyLst))
		return FALSE;
	//ILoadCombDefineMgr::Instance()->GetUsedKeyList(FALSE, arExistStldKeyLst);
	m_pDoc->m_pAttrCtrl->GetStldKeyList(rStldKeyLst);
	
	T_STLD_D StldD;
	for (int i=0; i<rStldKeyLst.GetCount(); i++)
	{
		StldD.Initialize();
		BOOL bExist = FALSE;
		for (int j=0; j<arExistStldKeyLst.GetSize(); j++)
		{
			if (rStldKeyLst[i] == arExistStldKeyLst.GetAt(j))
			{
				bExist = TRUE;
				break;
			}
		}
		if(!m_pDoc->m_pAttrCtrl->GetStld(rStldKeyLst[i], StldD )|| !bExist)
			continue;
		LOAD_CASE_K key;
		key.Key = rStldKeyLst[i];
		key.nLoadCaseType = D_LCOM_STATIC;
		pPara->m_mapType2Case[StldD.LoadCaseType].Add(key);
	}

	//反应谱
	CArray<T_SPLC_K, T_SPLC_K> rSplcKeyLst;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(rSplcKeyLst);
	T_SPLC_D SplcD;
	for (int i=0; i<rSplcKeyLst.GetCount(); i++)
	{
		SplcD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetSplc(rSplcKeyLst[i], SplcD))
			continue;
		LOAD_CASE_K key;
		key.Key = rSplcKeyLst[i];
		key.nLoadCaseType = D_LCOM_SPECTRUM;

		if (SplcD.nDirection == 1)
			pPara->m_mapType2Case[NAME_LOADCASE_E].Add(key);
		else
			pPara->m_mapType2Case[NAME_LOADCASE_EVT].Add(key);
	}


	//考虑施工阶段
	//只考虑 CS时 需要移除 D L
	//如果需要考虑施工阶段，把如上的KEY 添加到
	CArray<T_SGLD_K, T_SGLD_K> arSgldKey;
	if (pPara->m_pData->m_nStage == 1 || pPara->m_pData->m_nStage == 2)
	{
		pPara->m_mapType2Case[NAME_LOADCASE_D].RemoveAll();
		pPara->m_mapType2Case[NAME_LOADCASE_L].RemoveAll();
		pPara->m_mapType2Case[NAME_LOADCASE_LR].RemoveAll();

		if (m_pDoc->IsPostMode())
			m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldKeyListPost(arSgldKey);
		else
			m_pDoc->m_pAttrCtrl->GetSgldKeyList(arSgldKey);
	}

	if(pPara->m_pData->m_nStage == 1)
	{
		for (int iKey = 0; iKey < arSgldKey.GetSize(); iKey++)
		{
			T_SGLD_K sgldKey;
			sgldKey = arSgldKey.GetAt(iKey);
			T_SGLD_D sgldData;
			m_pDoc->m_pAttrCtrl->GetSgld(sgldKey, sgldData);
			LOAD_CASE_K key;
			key.Key = sgldKey;
			key.nLoadCaseType = D_LCOM_STAGE;
			if (sgldData.LoadCaseName == _LS(IDS_DB_SGLD_SUM))
				continue;
			if (sgldData.LoadCaseName==_LS(IDS_DB_SGLD_DL) || sgldData.LoadCaseName==_LS(IDS_DB_SGLD_CR) || sgldData.LoadCaseName==_LS(IDS_DB_SGLD_SH))	   
				pPara->m_mapType2Case[NAME_LOADCASE_D].Add(key);
			else if (sgldData.LoadCaseName == _LS(IDS_DB_SGLD_LL))	
				pPara->m_mapType2Case[NAME_LOADCASE_L].Add(key);
		}
	}
	
	if (pPara->m_pData->m_nStage == 2)
	{
		for (int iKey = 0; iKey < arSgldKey.GetSize(); iKey++)
		{
			T_SGLD_K sgldKey;
			sgldKey = arSgldKey.GetAt(iKey);
			T_SGLD_D sgldData;
			m_pDoc->m_pAttrCtrl->GetSgld(sgldKey, sgldData);
			LOAD_CASE_K key;
			key.Key = sgldKey;
			key.nLoadCaseType = D_LCOM_STAGE;
			if (sgldData.LoadCaseName == _LS(IDS_DB_SGLD_SUM))
				continue;
			if (sgldData.LoadCaseName == _LS(IDS_DB_SGLD_DL) || sgldData.LoadCaseName == _LS(IDS_DB_SGLD_CR) || sgldData.LoadCaseName == _LS(IDS_DB_SGLD_SH))
				pPara->m_mapType2Case[NAME_LOADCASE_D].Add(key);
			else if (sgldData.LoadCaseName == _LS(IDS_DB_SGLD_LL))
				pPara->m_mapType2Case[NAME_LOADCASE_L].Add(key);

		}

		for (int i = 0; i < rStldKeyLst.GetCount(); i++)
		{
			StldD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetStld(rStldKeyLst[i], StldD))
				continue;

			if (StldD.LoadCaseType == NAME_LOADCASE_D)
			{
				LOAD_CASE_K key;
				key.Key = rStldKeyLst[i];
				key.nLoadCaseType = D_LCOM_STATIC;
				pPara->m_mapType2Case[NAME_LOADCASE_D].Add(key);
			}
			else if (StldD.LoadCaseType == NAME_LOADCASE_L)
			{
				LOAD_CASE_K key;
				key.Key = rStldKeyLst[i];
				key.nLoadCaseType = D_LCOM_STATIC;
				pPara->m_mapType2Case[NAME_LOADCASE_L].Add(key);
			}
			else if (StldD.LoadCaseType == NAME_LOADCASE_LR)
			{
				LOAD_CASE_K key;
				key.Key = rStldKeyLst[i];
				key.nLoadCaseType = D_LCOM_STATIC;
				pPara->m_mapType2Case[NAME_LOADCASE_LR].Add(key);
			}
		}
		
	}
	return TRUE;
}

void CLoadCombDefCtrl::addKey2Data(CLoadCombDefinePara *pPara, LPCTSTR sType, CaseClassfyType nClassType,  double dFactor, const LOAD_CASE_K_ARR &aKey, LoadCombItemArr &aData)
{
	CLoadCombItem data;
	for (int i=0; i<aKey.GetCount(); i++)
	{
		data.m_key	   = aKey[i];
		data.m_dFactor = dFactor;
		data.m_sType   = sType;
		data.m_eType   = nClassType;
		data.m_dFactor = getFactor1(pPara, data);
		aData.Add(data);
	}
}

BOOL CLoadCombDefCtrl::processLLUnfavorable(CLoadCombDefinePara *pPara, LoadCombItemArr &aTypeB, CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aDividTypeB)
{
	LoadCombItemArr aTemp = aTypeB;
	//移除L类型工况
	std::map<LOAD_CASE_K, CLoadCombItem>  mapRemove;
	for (int i=aTemp.GetCount()-1; i>=0; i--)
	{
		if(aTemp[i].m_sType == NAME_LOADCASE_L)
		{
			mapRemove[aTemp[i].m_key] = aTemp[i];
			aTemp.RemoveAt(i);
		}
	}
	//
	if(mapRemove.size() < 1)
	{
		aDividTypeB.Add(aTypeB);
		return TRUE;
	}
	//
	for (int i = 0; i < pPara->m_pData->m_aLLUnfavorable.GetCount(); i++)
	{
		LoadCombItemArr aNewData;
		for (int k=0; k<pPara->m_pData->m_aLLUnfavorable[i].GetCount(); k++)
		{
			LOAD_CASE_K key = pPara->m_pData->m_aLLUnfavorable[i][k];
			if(mapRemove.find(key) == mapRemove.end())
			{
				aNewData.RemoveAll();
				break;
			}
			aNewData.Add(mapRemove[key]);
		}
		if(aNewData.GetCount() > 0)
		{
			aNewData.Append(aTemp);
			aDividTypeB.Add(aNewData);
		}
	}
	return aDividTypeB.GetCount() > 0;
}

BOOL CLoadCombDefCtrl::processExclusionTypeB(CLoadCombDefinePara *pPara, LoadCombItemArr &aTypeB,CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aDividTypeB)
{
	aDividTypeB.Add(aTypeB);
	for (auto itr : pPara->m_pData->m_mapExclusion)
	{
		processExclusionTypeB(itr.first, itr.second, aDividTypeB);				
	}
	return TRUE;
}

BOOL CLoadCombDefCtrl::processExclusionTypeB(LOAD_CASE_K Key1, const LOAD_CASE_K_ARR aExclude, CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aDividTypeB)
{
	CArrayEx<LoadCombItemArr, LoadCombItemArr&> aTemp; 
	aTemp.Copy(aDividTypeB);
	aDividTypeB.RemoveAll();
	
	for (int i=0; i<aTemp.GetCount(); i++)
	{
		int idx1 = -1;
		CArray<UINT, UINT> aRemove;
		LoadCombItemArr &aData = aTemp[i];
		for (int k=0; k<aData.GetCount(); k++)
		{
			if(aData[k].m_key == Key1)
			{
				idx1 = k;
			}
			else
			{
				for (int j=0; j<aExclude.GetCount(); j++)
				{
					if(aExclude[j] == aData[k].m_key)
						aRemove.Add(k);
				}
				
			}
		}
		if(idx1 < 0 || aRemove.GetCount() < 1)
		{
			aDividTypeB.Add(aData);
			continue;
		}
		//
		LoadCombItemArr aData1 = aData;
		aData1.RemoveAt(idx1);
		aDividTypeB.Add(aData1);
		LoadCombItemArr aData2 = aData;
		for (int k=aRemove.GetCount()-1; k>=0; k--)
		{
			aData2.RemoveAt(aRemove[k]);
		}
		aDividTypeB.Add(aData2);
	}
	//D_LCOM_SPECTRUM
	return aDividTypeB.GetCount() > 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLoadCombDefCtrl::considerEccnC(T_SPLC_K SplcK)
{
	T_SPLC_D splcD;
	if (m_pDoc->m_pAttrCtrl->GetSplc(SplcK, splcD) && splcD.bConsiderEccen)
		return TRUE;
	return FALSE;
}
void CLoadCombDefCtrl::generateEccnC(T_SPLC_K key, double dFactor, const T_LCOM_D& LComIn, T_LCOM_D& NewLcom)
{
	NewLcom = LComIn;
	for (int i=0; i< LComIn.aCombination.GetCount(); i++)
	{
		if (LComIn.aCombination[i].AnalType == D_LCOM_ECCENSPEC)
		{
			NewLcom.aCombination[i].Factor *= -1.0;
			break;
		}
	}
}
void CLoadCombDefCtrl::generateDoubleDirC(CLoadCombDefinePara *pPara)
{
	if(pPara->m_pData->m_bBiEarthquake == FALSE)
		return;

	for (auto itr : pPara->m_pData->m_mapBiEarthquake)
	{
		LoadCombItemArr aTemp;
		CLoadCombItem item1;
		item1.m_eType	=  Type_C;
		item1.m_dFactor = 1.0;
		item1.m_key	    = itr.first;
		item1.m_sType	= pPara->m_mapCase2Type[itr.first];
		CLoadCombItem item2;
		item2.m_eType	=  Type_C;
		item2.m_dFactor = 0.85;
		item2.m_key	    = itr.second;
		item2.m_sType	= pPara->m_mapCase2Type[itr.second];
		aTemp.Add(item1);
		aTemp.Add(item2);
		pPara->m_aDoubleDirCombC.Add(aTemp);
		pPara->m_mapCaseC2DoubleDirEQ[itr.first] = pPara->m_aDoubleDirCombC.GetCount()-1;
		//
		aTemp.RemoveAll();
		item1.m_dFactor = 0.85;
		item2.m_dFactor = 1.0;
		aTemp.Add(item2);
		aTemp.Add(item1);
		pPara->m_aDoubleDirCombC.Add(aTemp);
		pPara->m_mapCaseC2DoubleDirEQ[itr.second] = pPara->m_aDoubleDirCombC.GetCount()-1;
	}
}
BOOL CLoadCombDefCtrl::getClassfyInfo(CLoadCombDefinePara *pPara, int idx, CaseCombInfoArr &aInfoPool)
{
	aInfoPool.RemoveAll();
	if(idx >= pPara->m_pData->m_item.m_aFactor.GetCount())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	const CArray<double, double> &aFactor = pPara->m_pData->m_item.m_aFactor[idx];
	int nSize = min(pPara->m_pData->m_item.m_aType.GetCount(), aFactor.GetCount());
	CaseCombDefineInfo	   tempInfo;
	LoadCombItemArr aTypeBC;
	LoadCombItemArr aTypeC;
	LoadCombItemArr aTypeCV;
	BOOL hasTypeCFactor = FALSE;
	BOOL hasTypeCVFactor = FALSE;
	CaseClassfyType nClassType = Type_Null;
	for (int i=0; i<nSize; i++)
	{		
		const CString &sType = pPara->m_pData->m_item.m_aType[i];
		nClassType = pPara->m_mapTypeClassfy[sType];
		if(nClassType == Type_C && fabs(aFactor[i]) > 1e-6)
			hasTypeCFactor = TRUE;
		if (nClassType == Type_CV && fabs(aFactor[i]) > 1e-6)
			hasTypeCVFactor = TRUE;
		else if(fabs(aFactor[i]) < 1e-6)
			continue;
		auto itr = pPara->m_mapType2Case.find(sType);
		if(itr == pPara->m_mapType2Case.end())
			continue;		
		if(nClassType == Type_A)
			addKey2Data(pPara, sType, nClassType, aFactor[i], itr->second, tempInfo.m_aTypeA);
		else if(nClassType == Type_B)
			addKey2Data(pPara, sType, nClassType, aFactor[i], itr->second, tempInfo.m_aTypeB);
		else if(nClassType == Type_C)
			addKey2Data(pPara, sType, nClassType, aFactor[i], itr->second, aTypeC);			
		else if(nClassType == Type_BC)
			addKey2Data(pPara, sType, nClassType, aFactor[i], itr->second, aTypeBC);
		else if (nClassType == Type_CV)
			addKey2Data(pPara, sType, nClassType, aFactor[i], itr->second, aTypeCV);
	}
	//没有TYPE C 和 TYPE CV 而有系数，则不考虑此组合
	if((hasTypeCFactor && aTypeC.GetCount() < 1 && !hasTypeCVFactor)||(hasTypeCVFactor && aTypeCV.GetCount() < 1 && !hasTypeCFactor) || (hasTypeCFactor && aTypeC.GetCount() < 1 && hasTypeCVFactor && aTypeCV.GetCount() < 1))
		return FALSE;
	if(aTypeC.GetCount() < 1 && aTypeCV.GetCount() < 1)
	{
		for (int i=0; i<aTypeBC.GetCount(); i++)
		{
			CaseCombDefineInfo curInfo = tempInfo;
			curInfo.m_aTypeB.Add(aTypeBC[i]);
			aInfoPool.Add(curInfo);
		}
		if(aInfoPool.GetCount() < 1)
			aInfoPool.Add(tempInfo);
	}
	else //判断地震
	{
		CaseCombInfoArr tmpPool;
		CaseCombInfoArr tmpPoolDone;
		std::map<int, int> hasDoneIdxTypeC;
		//对于BC类型一次添加一个到B中，如果有地震的时候且方向不同则不添加
		std::map<LOAD_CASE_K, CLoadCombItem> mapTypeC;
		for (int i=0; i<aTypeBC.GetCount(); i++)
		{
			CaseCombDefineInfo curInfo = tempInfo;
			auto itr = pPara->m_pData->m_mapBC2C.find(aTypeBC[i].m_key);
			//未找到则不添加
			if(itr == pPara->m_pData->m_mapBC2C.end())
				continue;
			//判断是否在TYPE C中
			for (int k=0; k<aTypeC.GetCount(); k++)
			{
				if(!(aTypeC[k].m_key == itr->second))
					continue;
				curInfo.m_aTypeB.Add(aTypeBC[i]);
				curInfo.m_itemC = aTypeC[k];
				//双向地震
				if(pPara->m_mapCaseC2DoubleDirEQ.find(curInfo.m_itemC.m_key) != pPara->m_mapCaseC2DoubleDirEQ.end())
				{
					curInfo.m_itemC.m_sType = NAME_LOADCASE_COMB;
					curInfo.m_itemC.m_key.Key   = pPara->m_mapCaseC2DoubleDirEQ[curInfo.m_itemC.m_key];
					curInfo.m_itemC.m_key.nLoadCaseType = D_LCOM_CB_GENERAL; 
				}
				hasDoneIdxTypeC[k] = 1;
				break;
			}
			tmpPoolDone.Add(curInfo);
		}

		if(tmpPool.GetCount() < 1)
			tmpPool.Add(tempInfo);
		//添加C类型
		for (int i=0; i<aTypeC.GetCount(); i++)
		{
			if(hasDoneIdxTypeC.find(i) != hasDoneIdxTypeC.end())
				continue;
			for (int k = 0; k < tmpPool.GetCount(); k++)
			{
				CaseCombDefineInfo info = tmpPool[k];
				info.m_itemC = aTypeC[i];
				//双向地震
				if (pPara->m_mapCaseC2DoubleDirEQ.find(info.m_itemC.m_key) != pPara->m_mapCaseC2DoubleDirEQ.end())
				{
					info.m_itemC.m_sType = NAME_LOADCASE_COMB;
					info.m_itemC.m_key.Key = pPara->m_mapCaseC2DoubleDirEQ[info.m_itemC.m_key];
					info.m_itemC.m_key.nLoadCaseType = D_LCOM_CB_GENERAL;
				}
				aInfoPool.Add(info);
			}
		}
		aInfoPool.Append(tmpPoolDone);
		//
		if (aTypeCV.GetCount() > 0)
		{
			tmpPool = aInfoPool; aInfoPool.RemoveAll();
			if (tmpPool.GetCount() < 1)
				tmpPool.Add(tempInfo);
			//添加CV类型
			for (int i = 0; i < aTypeCV.GetCount(); i++)
			{
				for (int k = 0; k < tmpPool.GetCount(); k++)
				{
					CaseCombDefineInfo info = tmpPool[k];
					info.m_itemCV = aTypeCV[i];
					aInfoPool.Add(info);
				}
			}
		}		
	}
	//活荷载不利布置
	if(pPara->m_pData->m_nSpecitlArrange == 1)
	{
		for (int i=0; i<aInfoPool.GetCount(); i++)
		{
			processLLUnfavorable(pPara, aInfoPool[i].m_aTypeB, aInfoPool[i].m_aTypeBDivid);
		}
	}
	else if(pPara->m_pData->m_nSpecitlArrange == 2)
	{
		for (int i=0; i<aInfoPool.GetCount(); i++)
		{
			processExclusionTypeB(pPara, aInfoPool[i].m_aTypeB, aInfoPool[i].m_aTypeBDivid);
		}
	}
	else
	{
		for (int i=0; i<aInfoPool.GetCount(); i++)
		{
			aInfoPool[i].m_aTypeBDivid.Add(aInfoPool[i].m_aTypeB);
		}
	}
	return TRUE;
}

BOOL CLoadCombDefCtrl::generatorDoubleDirC  (CLoadCombDefinePara *pPara, CLcomCombAutoRlt *pRltPool)
{	
	for (int i=0; i<pPara->m_aDoubleDirCombC.GetCount(); i++)
	{
		LoadCombItemArr lcomD;
		const LoadCombItemArr &aItem = pPara->m_aDoubleDirCombC[i];
		for (int k=0; k<aItem.GetCount(); k++)
		{
			addChild2LComb(aItem[k], aItem[k].m_dFactor, lcomD);
		}
		pRltPool->add(lcomD);
	}
	return TRUE;
}

BOOL CLoadCombDefCtrl::generatorLcomb(CLoadCombDefinePara *pPara, const CaseCombDefineInfo *pInfo, CLcomCombAutoRlt *pRltPool)
{
	//添加TYPE A
	LoadCombItemArr LcomD;
	BOOL bCtrlByTypeA = FALSE;
	for (int i=0; i<pInfo->m_aTypeA.GetCount(); i++)
	{
		addChild2LComb(pInfo->m_aTypeA[i], pInfo->m_aTypeA[i].m_dFactor, LcomD);
		if(pInfo->m_aTypeA[i].m_dFactor > 1.35 - 1e-6)
			bCtrlByTypeA = TRUE;
	}
	CArrayEx<LoadCombItemArr, LoadCombItemArr&> aRltLcomD;
	//添加TYPE B	
	BOOL bCtrlByTypeC = pInfo->m_itemC.isValid();
	BOOL bCtrlByTypeCV = pInfo->m_itemCV.isValid();
	//C类型控制时，不进行轮换，并且 B类型为分项系数*重力代表制系数
	//A类型控制时，不进行轮换，并且 B类型为分项系数*组合值系数*调整系数
	if((bCtrlByTypeC|| bCtrlByTypeCV) || bCtrlByTypeA)
	{
		for (int i=0; i<pInfo->m_aTypeBDivid.GetCount(); i++)
		{
			CArrayEx<LoadCombItemArr, LoadCombItemArr&> aLcombD;	aLcombD.Add(LcomD);
			const LoadCombItemArr &aSubItem = pInfo->m_aTypeBDivid[i];
			BOOL bTypeC = bCtrlByTypeC || bCtrlByTypeCV;
			for (int k=0; k<aSubItem.GetCount(); k++)
			{
				addBType2Lcomb(pPara, aSubItem[k], bTypeC ?0:1, aLcombD);
			}		
			aRltLcomD.Append(aLcombD);
		}
	}
	else //需要轮换
	{
		for (int i = 0; i < pInfo->m_aTypeBDivid.GetCount(); i++)
		{
			const LoadCombItemArr &aSubItem = pInfo->m_aTypeBDivid[i];
			for (int k = 0; k < aSubItem.GetCount(); k++)
			{
				CArrayEx<LoadCombItemArr, LoadCombItemArr&>aLcombD;	aLcombD.Add(LcomD);
				for (int t = 0; t < aSubItem.GetCount(); t++)
				{
					addBType2Lcomb(pPara, aSubItem[t], t == k ? 2 : 3, aLcombD);
				}
				aRltLcomD.Append(aLcombD);
			}
		}
	}
	//添加TYPE C
	if(bCtrlByTypeC)
		addCType2Lcomb(pPara, pInfo->m_itemC, aRltLcomD);
	if (bCtrlByTypeCV)
		addCType2Lcomb(pPara, pInfo->m_itemCV, aRltLcomD);
	pRltPool->add(aRltLcomD);
	BOOL bIncludeB = pInfo->m_aTypeB.GetCount() > 0;
	if(!bIncludeB && !bCtrlByTypeC && !bCtrlByTypeCV)
		pRltPool->add(LcomD);
	return TRUE;
}
BOOL CLoadCombDefCtrl::isWind(const CLoadCombItem &item)
{
	BOOL bWind = item.m_sType == NAME_LOADCASE_W;	// || item.m_sType == NAME_LOADCASE_WLK || item.m_sType == NAME_LOADCASE_WTK;
	return bWind;
}
BOOL CLoadCombDefCtrl::isTemperature(const CLoadCombItem &item)
{
	return item.m_sType == NAME_LOADCASE_T;
}
//分项系数
double CLoadCombDefCtrl::getFactor1(CLoadCombDefinePara *pPara, const CLoadCombItem &item)
{
	auto itr = pPara->m_pData->m_mapSpecialFactor.find(item.m_key);
	if(itr != pPara->m_pData->m_mapSpecialFactor.end())
		return itr->second;
	if (m_bStandComb && item.m_dFactor > 0)
		return 1.0;
	return item.m_dFactor;
}
//组合值系数
double CLoadCombDefCtrl::getFactor2(CLoadCombDefinePara *pPara, const CLoadCombItem &item)
{
	auto itr = pPara->m_pData->m_mapCombFactor.find(item.m_key);
	if(itr != pPara->m_pData->m_mapCombFactor.end())
		return itr->second;
	else if(isWind(item))
		return pPara->m_pData->m_dCombFactor[1]; 
	else if(isTemperature(item))
		return pPara->m_pData->m_dCombFactor[2]; 
	else 
		return pPara->m_pData->m_dCombFactor[0]; 
}
BOOL CLoadCombDefCtrl::addChild2LComb(const CLoadCombItem& item, double dNewFactor, LoadCombItemArr &Lcomb)
{
	if(fabs(dNewFactor) < 1e-9)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CLoadCombItem newItem = item;
	newItem.m_dFactor = dNewFactor;
	Lcomb.Add(newItem);
	return TRUE;
}
//重力代表值系数
double CLoadCombDefCtrl::getFactor3(CLoadCombDefinePara *pPara, const CLoadCombItem &item)
{
	auto itr = pPara->m_pData->m_mapGravityFactor.find(item.m_key);
	if(itr != pPara->m_pData->m_mapGravityFactor.end())
		return itr->second;
	if(isWind(item))
		return 0.2; // 固定为0.2?风与地震相组的时候
	return pPara->m_pData->m_dLLGravityFactor;
}
//nType: 0 C类型控制 1 A类型控制， 2 B类型控制的主类型 3 B类型控制的次类型
BOOL CLoadCombDefCtrl::addBType2Lcomb (CLoadCombDefinePara *pPara, const CLoadCombItem &item, int nType, CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aLcomb)
{
	CArrayEx<LoadCombItemArr, LoadCombItemArr&> aTemp; 
	aTemp.Copy(aLcomb); 
	aLcomb.RemoveAll();
	
	for (int i=0; i<aTemp.GetCount(); i++)
	{
		double dFactor1 = item.m_dFactor * pPara->m_pData->m_dLLAdjustCombFactor;
		double dFactor2 = 1.0;
		//C(地震)控制 dFactor2=重力代表制系数
		if(nType == 0)
			dFactor2 = getFactor3(pPara, item);
		else if(nType == 1)
			dFactor2 = getFactor2(pPara, item);
		else if(nType == 3)
			dFactor2 = getFactor2(pPara, item);
		//如果是B 类型则直接添加
		if(item.m_eType == Type_B)
		{
			addChild2LComb(item, dFactor1*dFactor2, aTemp[i]);
			aLcomb.Add(aTemp[i]);
		}
		else if(item.m_eType == Type_BC)
		{
			LoadCombItemArr data1 = aTemp[i];
			addChild2LComb(item, dFactor1*dFactor2, data1);
			aLcomb.Add(data1);
			if (!pPara->m_pData->m_bWindForward)
			{
				LoadCombItemArr data2 = aTemp[i];
				addChild2LComb(item, -dFactor1 * dFactor2, data2);
				aLcomb.Add(data2);
			}	
		}
		else
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	return TRUE;
}
BOOL CLoadCombDefCtrl::addCType2Lcomb (CLoadCombDefinePara *pPara, const CLoadCombItem &item , CArrayEx<LoadCombItemArr, LoadCombItemArr&> &aLcomb)
{
	CArrayEx<LoadCombItemArr, LoadCombItemArr&> aTemp; 
	aTemp.Copy(aLcomb); 
	aLcomb.RemoveAll();

	for (int i=0; i<aTemp.GetCount(); i++)
	{
		double dFactor1 = item.m_dFactor;
		LoadCombItemArr data1 = aTemp[i];
		addChild2LComb(item, dFactor1, data1);
		aLcomb.Add(data1);
		LoadCombItemArr data2 = aTemp[i];
		addChild2LComb(item, -dFactor1, data2);
		aLcomb.Add(data2);
	}
	return TRUE;
}

BOOL CLoadCombDefCtrl::save2DB(CLcomCombAutoRlt *pRltPool, int nLcomType)
{
	std::map<int, T_LCOM_K> mapLcombIdx2Key;
	std::map<CString, int>	 mapExistLcomb;
	T_LCOM_D lcomD;
	int		  nAddNormalComb = Get_MaxLoadCombNameNo(nLcomType)+1;
	CString   sName;
	m_pDoc->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_IDD_CMD_LCOMBDEFAULT_SEISEVAL_DLG_IDD_CMD_LCOMBDEFAULT_SEISEVAL_DLG)), CMDTYPE_REMOVE_DESIGN);
	
	int nNum = 0;
	for (int i=0; i<pRltPool->m_aLcomb.GetCount(); i++)
	{
		if(convert2Lcomb(pRltPool->m_aLcomb[i], mapLcombIdx2Key, lcomD, nLcomType) == FALSE && mapExistLcomb.find((LPCTSTR)lcomD.Description) != mapExistLcomb.end())
			continue;
		mapExistLcomb[(LPCTSTR)lcomD.Description] = 1;
#ifdef _MGEN_CH
		sName.Format(_T("%s%d"), _LSX(LcomD), nAddNormalComb);
		lcomD.LoadCombName = sName;
#else
		if(lcomD.LoadCombType == 3)
			lcomD.LoadCombName = lcomD.Description;
		else
		{
			sName.Format(_T("%s%d"), _LSX(LcomD), nAddNormalComb);
			lcomD.LoadCombName = sName;
	}
#endif
		if (IsExistLcom(lcomD, m_tempLcom))
			continue;			
		else
			m_tempLcom.Add(lcomD);

		//
		if (lcomD.LoadCombType==EN_DGN_ADD && IsSetDefaultImpf())
		{
			CArray<T_LCOM_D, const T_LCOM_D&> aNewLcom;
			Set_Each_LoadComb_Imperfection_CH(lcomD, aNewLcom);
			for (int k=0; k<aNewLcom.GetCount(); k++)
			{
				sName.Format(_T("%s%d"), _LSX(LcomD), nAddNormalComb);
				aNewLcom[k].LoadCombName = sName;
				aNewLcom[k].Description = MakeCombDescription(aNewLcom[k], nLcomType);
				if(m_pDoc->m_pEditData->AddLcom(nLcomType, aNewLcom[k]) == FALSE)
					continue;
				T_LCOM_K LcomK = m_pDoc->m_pAttrCtrl->GetLcomKey(nLcomType, sName);
				mapLcombIdx2Key[nNum] = LcomK;
				nAddNormalComb++;
				nNum++;
			}
		}
		else
		{
			T_LCOM_K LcomK = m_pDoc->m_pAttrCtrl->GetStartNumLcom(nLcomType);
			lcomD.LoadCombId = m_pDoc->m_pAttrCtrl->GetCountLcom(nLcomType) + 1;
			if (m_pDoc->m_pEditData->AddLcom(nLcomType, LcomK, lcomD) == FALSE)
				continue;
			mapLcombIdx2Key[nNum] = LcomK;
			nAddNormalComb++;
			nNum++;
		}

		//ECCN
		for (int k = 0; k < lcomD.aCombination.GetCount(); k++)
		{
			if (lcomD.aCombination[k].AnalType == D_LCOM_ECCENSPEC && considerEccnC(lcomD.aCombination[k].LoadCaseKey))
			{
				T_LCOM_D NewLcom;
				generateEccnC(lcomD.aCombination[k].LoadCaseKey, lcomD.aCombination[k].Factor, lcomD, NewLcom);
				NewLcom.Description = MakeCombDescription(NewLcom, nLcomType);
				if (NewLcom.LoadCombType == EN_DGN_ADD && IsSetDefaultImpf())
				{
					CArray<T_LCOM_D, const T_LCOM_D&> aNewLcom;
					Set_Each_LoadComb_Imperfection_CH(NewLcom, aNewLcom);
					for (int k = 0; k < aNewLcom.GetCount(); k++)
					{
						sName.Format(_T("%s%d"), _LSX(LcomD), nAddNormalComb);
						aNewLcom[k].LoadCombName = sName;
						aNewLcom[k].Description = MakeCombDescription(aNewLcom[k], nLcomType);
						if (m_pDoc->m_pEditData->AddLcom(nLcomType, aNewLcom[k]) == FALSE)
							continue;
						T_LCOM_K LcomK = m_pDoc->m_pAttrCtrl->GetLcomKey(nLcomType, sName);
						mapLcombIdx2Key[nNum] = LcomK;
						nAddNormalComb++;
						nNum++;
					}
				}
				else
				{
					T_LCOM_K LcomK = m_pDoc->m_pAttrCtrl->GetStartNumLcom(nLcomType);
					sName.Format(_T("%s%d"), _LSX(LcomD), nAddNormalComb);
					NewLcom.LoadCombName = sName;
					NewLcom.LoadCombId = m_pDoc->m_pAttrCtrl->GetCountLcom(nLcomType) + 1;
					if (m_pDoc->m_pEditData->AddLcom(nLcomType, LcomK, NewLcom) == FALSE)
						continue;
					mapLcombIdx2Key[nNum] = LcomK;
					nAddNormalComb++;
					nNum++;
				}
			}
		}
		
	}

	if (nLcomType == D_LCOMTYPE_GENERAL && mapLcombIdx2Key.size()>0)
	{
		T_LCOM_D LcomD;
		CString strName = m_bStandComb ?  _T("ENV_SER") : _T("ENV_STR");
		if (m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, strName, lcomD))
		{
			T_LCOM_K lcomEnvK = m_pDoc->m_pAttrCtrl->GetLcomKey(nLcomType, strName);
			if (!m_pDoc->m_pEditData->DelLcom(nLcomType, lcomEnvK))
				ASSERT(FALSE);
		}
	
		LcomD.nActive = 1;
		LcomD.Description = m_bStandComb ? _T("Serviceability Envelope") : _T("Strength Envelope");
		LcomD.LoadCombType = EN_DGN_ENV;
		LcomD.LoadCombName = strName;
		for (auto itr = mapLcombIdx2Key.begin(); itr != mapLcombIdx2Key.end(); itr++)
		{
			T_LCOM_K lcomK = itr->second;
			T_LCOM_D lcomD;
			if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, lcomK, lcomD) || lcomD.nActive == 0)
				continue;
#ifdef _DEBUG
			CString strName = lcomD.LoadCombName;
#endif
			T_LCOM_BASE LcomBase;
			LcomBase.AnalType = D_LCOM_CB_GENERAL;
			LcomBase.Factor = 1.0;
			LcomBase.LoadCaseKey = lcomK;
			LcomD.aCombination.Add(LcomBase);
		}
		if (!m_pDoc->m_pEditData->AddLcom(nLcomType, LcomD))
			ASSERT(FALSE);
	}
	m_pDoc->m_pUndoCtrl->CloseEditDB(TRUE);
	return TRUE;
}

BOOL CLoadCombDefCtrl::convert2Lcomb(const LoadCombItemArr &item, const std::map<int, T_LCOM_K> &mapIdx2Key, T_LCOM_D &lcomD, int nLcomType)
{
	bool bDoubleDirC = item.GetCount() == 2 && (item[0].m_eType == Type_C && item[1].m_eType == Type_C);
	lcomD.Initialize();
	lcomD.LoadCombType = 0;
	if (m_bStandComb && nLcomType != D_LCOMTYPE_GENERAL)
		lcomD.nActive = 2;	//标准组合
	else 
		lcomD.nActive = bDoubleDirC ? 0 : 1;
#ifdef _MGEN_CH
	if(bDoubleDirC)
		lcomD.LoadCombType = EN_DGN_SRSS;
#endif
	int nAnalType = D_LCOM_CB_GENERAL;
	if(nLcomType == D_LCOMTYPE_STEEL)
		nAnalType = D_LCOM_CB_STEEL;
	else if(nLcomType == D_LCOMTYPE_CFSTEEL)
		nAnalType = D_LCOM_CB_CFSTEEL;
	else if(nLcomType == D_LCOMTYPE_ALUMINUM)
		nAnalType = D_LCOM_CB_ALUMINUM;
	else if(nLcomType == D_LCOMTYPE_CONCRETE)
		nAnalType = D_LCOM_CB_CONCRETE;
	else if(nLcomType == D_LCOMTYPE_SRC)
		nAnalType = D_LCOM_CB_SRC;
	for (int i=0; i<item.GetCount(); i++)
	{
		int nKey = item[i].m_key.Key;
		T_LCOM_BASE baseD;
		baseD.Factor  = item[i].m_dFactor;
		baseD.AnalType = item[i].m_key.nLoadCaseType;	
		if(item[i].m_sType == NAME_LOADCASE_COMB)
		{
			auto itr = mapIdx2Key.find(nKey);
			if(itr == mapIdx2Key.end())
			{
				ASSERT(FALSE);
				return FALSE;
			}
			nKey = itr->second;
			baseD.AnalType = nAnalType;		
		}	

		baseD.LoadCaseKey = nKey;
		lcomD.aCombination.Add(baseD);

		if (item[i].m_key.nLoadCaseType == D_LCOM_SPECTRUM)
		{
			baseD.AnalType = D_LCOM_ECCENSPEC;
			lcomD.aCombination.Add(baseD);
		}
		
	}
	lcomD.Description = MakeCombDescription(lcomD, nLcomType);
	return  TRUE;
}

CString CLoadCombDefCtrl::MakeCombDescription(const T_LCOM_D &lcomD, int nLcomType)
{
	CString strDes = _T("");
	
	for (int i=0; i<lcomD.aCombination.GetCount(); i++)
	{
		CString strFac, strCase, strTemp;
		if (lcomD.aCombination[i].AnalType == D_LCOM_STAGE)
		{
			T_SGLD_D sgldData;
			if (m_pDoc->m_pAttrCtrl->GetSgld(lcomD.aCombination[i].LoadCaseKey, sgldData))
				strCase = sgldData.LoadCaseName;
		}
		else if (lcomD.aCombination[i].AnalType == D_LCOM_STATIC)
		{
			T_STLD_D stldD;
			if (!m_pDoc->m_pAttrCtrl->GetStld(lcomD.aCombination[i].LoadCaseKey, stldD))
				continue;
			strCase = stldD.LoadCaseName;
		}
		else if (lcomD.aCombination[i].AnalType == D_LCOM_SPECTRUM)
		{
			T_SPLC_D splcD;
			if (!m_pDoc->m_pAttrCtrl->GetSplc(lcomD.aCombination[i].LoadCaseKey, splcD))
				continue;
			strCase = splcD.LoadCaseName + _T("(RS)");
		}
		else if (lcomD.aCombination[i].AnalType == D_LCOM_ECCENSPEC)
		{
			if (considerEccnC(lcomD.aCombination[i].LoadCaseKey))
			{
				T_ESPC_D espcD;
				if (!m_pDoc->m_pAttrCtrl->GetEspc(lcomD.aCombination[i].LoadCaseKey, espcD))
					continue;
				strCase = espcD.LoadCaseName + _T("(ES)");
			}
			else
				continue;
		}
		else
		{
			T_LCOM_D TempD;
			if (!m_pDoc->m_pAttrCtrl->GetLcom(nLcomType, lcomD.aCombination[i].LoadCaseKey, TempD))
				continue;
			strCase = TempD.LoadCombName;
		}
		strFac.Format(_T("%.2f"), lcomD.aCombination[i].Factor);
		strTemp.Format(_T("%s*%s"), strFac, strCase);
		
		if (strDes.GetLength() + strTemp.GetLength() > D_LCOM_DESC_SIZE)
		{
			strDes += _T("…");
			break;
		}
			
		if (i == 0)
		{
			strDes += strTemp;
		}
		else
		{
			if (lcomD.aCombination[i].Factor < 0)
				strDes += strTemp;
			else
				strDes += _T("+") + strTemp;
		}	
	}
	if (m_bStandComb && nLcomType != D_LCOMTYPE_GENERAL)
		strDes = _T("SERV:") + strDes;
	return strDes;
}

int CLoadCombDefCtrl::MakeCombType()
{
	int nType = 0;
	if (m_strCodeName == _T("GB50018-02") || m_strCodeName == _T("GB/T50010-10"))
	{
		nType = 0;
	}
	else if (m_strCodeName == _T("GB50017-03") || m_strCodeName == _T("GB50010-02"))
	{
		nType = 1;
	}
	else if (m_strCodeName == _T("GB50017-17"))
	{
		nType = 2;
	}
	else if (m_strCodeName == _T("JGJ99-2015"))
	{
		nType = 3;
	}
	else if (m_strCodeName == _T("JGJ3-2010"))
	{
		nType = 4;
	}
	else if (m_strCodeName == _T("GB50069-02"))
	{
		nType = 5;
	}
	return nType;
}

void CLoadCombDefCtrl::Remove0FactorCase(T_LCOM_D& LComD)
{
	for (int i=LComD.aCombination.GetSize()-1; i>-1; i--)
	{
		T_LCOM_BASE& rLcomBase = LComD.aCombination[i];
		if (fabs(rLcomBase.Factor) < 0 || rLcomBase.LoadCaseKey == 0) 
			LComD.aCombination.RemoveAt(i);
	}
}

BOOL CLoadCombDefCtrl::IsExistLcom(T_LCOM_D& CurLcomD, const CArray<T_LCOM_D, T_LCOM_D&>& arLcomD)
{
	BOOL bSame = FALSE;

	Remove0FactorCase(CurLcomD);

	for (int k = 0; k < arLcomD.GetSize(); k++)
	{
		const T_LCOM_D& PreLcomD = arLcomD[k];
		if (IsSameLcomData(CurLcomD, PreLcomD))
		{
			bSame = TRUE;
			break;
		}
	}

	return bSame;
}

BOOL CLoadCombDefCtrl::IsSameLcomData(const T_LCOM_D& CurLcomD, const T_LCOM_D& PreLcomD)
{
	if (CurLcomD.LoadCombType  != PreLcomD.LoadCombType)  return FALSE;
	if (CurLcomD.nActive       != PreLcomD.nActive)       return FALSE;
	if (CurLcomD.bElasticStage != PreLcomD.bElasticStage) return FALSE;
	if (CurLcomD.nServLcomType != PreLcomD.nServLcomType) return FALSE;
	if (CurLcomD.nSubLcomType  != PreLcomD.nSubLcomType)  return FALSE;

	if(CurLcomD.aCombination.GetSize()!=PreLcomD.aCombination.GetSize()) return FALSE;
	for (int i=0; i<CurLcomD.aCombination.GetSize(); i++)
	{
		const T_LCOM_BASE& CurLcomBase = CurLcomD.aCombination[i];
		const T_LCOM_BASE& PreLcomBase = PreLcomD.aCombination[i];

		if (CurLcomBase.AnalType    != PreLcomBase.AnalType)    { return FALSE; }
		if (CurLcomBase.LoadCaseKey != PreLcomBase.LoadCaseKey) { return FALSE; }
		if (CurLcomBase.Factor      != PreLcomBase.Factor)      { return FALSE; }
	}

	return TRUE;
}

BOOL CLoadCombDefCtrl::IsSetDefaultImpf()
{
	SetDefaultImpfData();
	if(m_bImperfection && m_aImperfectionLoad.GetCount() > 0)
		return TRUE;
	return FALSE;
}

void CLoadCombDefCtrl::SetDefaultImpfData()
{
	m_aImperfectionLoad.RemoveAll();

	T_IPCR_D IpcrD;
	T_STLD_K StldK;
	T_STLD_D StldD, DummyStldD;
	_LCOM_IMPF LComImpf;

	CString strLoadCaseNameOri;
	CString strLoadCaseName;

	CArray<T_IPCR_K, T_IPCR_K> aIpcrK;
	m_pDoc->m_pAttrCtrl2->GetIpcrKeyList(aIpcrK);
	for (int i = 0; i < aIpcrK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetIpcr(aIpcrK[i], IpcrD)) {ASSERT(0); IpcrD.Initialize();}


		if(!m_pDoc->m_pAttrCtrl->GetStld(IpcrD.LoadCaseKey, StldD)) {ASSERT(0); StldD.Initialize();}

		strLoadCaseName = IpcrD.strImpLCaseName;
		CArray<T_STLD_K, T_STLD_K> aStldK;
		StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strLoadCaseName);
		if(StldK)
		{
			int nAnalType;
			GetLoadCombTypeByLoadCaseType(IpcrD.AnalType, nAnalType);
			LComImpf.bElem = FALSE;
			if(IpcrD.m_nCode == 1 && IpcrD.m_CHData.m_bElem)
				LComImpf.bElem = TRUE;
			LComImpf.LCase1K = StldK;
			LComImpf.nAnalType = nAnalType;
			LComImpf.LCase2K = IpcrD.LoadCaseKey;
			LComImpf.nDir = IpcrD.nDir;
			m_aImperfectionLoad.Add(LComImpf);
		}
	}
}

BOOL CLoadCombDefCtrl::GetLoadCombTypeByLoadCaseType(int nLoadCaseType, int& nAnalType)
{
	switch(nLoadCaseType)
	{
	case D_LOADCASE_STATIC			: nAnalType = D_LCOM_STATIC;		break;
	case D_LOADCASE_SPECTRUM		: nAnalType = D_LCOM_SPECTRUM;		break;
	case D_LOADCASE_SPECTRUM_ND		: nAnalType = D_LCOM_SPECTRUM_ND;	break;
	case D_LOADCASE_HISTORY			: nAnalType = D_LCOM_HISTORY;		break;
	case D_LOADCASE_MOVING			: nAnalType = D_LCOM_MOVING;		break;
	case D_LOADCASE_SETTLE			: nAnalType = D_LCOM_SETTLEMENT;	break;
	case D_LOADCASE_ENVELOPE		: nAnalType = D_LCOM_ENVELOPE;		break;
	case D_LOADCASE_COMB_GENERAL	: nAnalType = D_LCOM_CB_GENERAL;	break;
	case D_LOADCASE_COMB_STEEL		: nAnalType = D_LCOM_CB_STEEL;		break;
	case D_LOADCASE_COMB_CONCRETE	: nAnalType = D_LCOM_CB_CONCRETE;	break;
	case D_LOADCASE_COMB_FDN		: nAnalType = D_LCOM_CB_FOOTING;	break;
	case D_LOADCASE_COMB_SRC		: nAnalType = D_LCOM_CB_SRC;		break;
	case D_LOADCASE_COMB_ALUMINUM	: nAnalType = D_LCOM_CB_ALUMINUM;	break;
	case D_LOADCASE_COMB_STLCOMP	: nAnalType = D_LCOM_CB_STLCOMP;	break;
	case D_LOADCASE_PUSHOVER		: nAnalType = D_LCOM_PUSHOVER;		break;
	case D_LOADCASE_STAGE			: nAnalType = D_LCOM_STAGE;			break;
	case D_LOADCASE_ECCENSPEC		: nAnalType = D_LCOM_ECCENSPEC;		break;
	case D_LOADCASE_ECCENSPEC_ND	: nAnalType = D_LCOM_ECCENSPEC_ND;	break;
	case D_LOADCASE_GRIDMODEL		: nAnalType = D_LCOM_GRIDMODEL;		break;
	case D_LOADCASE_CRANE			: nAnalType = D_LCOM_CRANE;			break;      
	default:  ASSERT(0);    return FALSE;
	}
	return TRUE;
}

BOOL CLoadCombDefCtrl::Set_Each_LoadComb_Imperfection_CH(T_LCOM_D& LComIn, CArray<T_LCOM_D, const T_LCOM_D&> &aNewLcom)
{
	BOOL bExistEQ = IAluDgnTool::Get_SesiType(LComIn, m_pDoc) != 0;
	CArray<T_LCOM_D, const T_LCOM_D&> aNewLcomTemp;
	aNewLcom.Add(LComIn);
	for (int i=0; i<LComIn.aCombination.GetCount(); i++)
	{
		const T_LCOM_BASE &curBase = LComIn.aCombination[i];
		for (int k=0; k<m_aImperfectionLoad.GetCount(); k++)
		{
			const _LCOM_IMPF &impF = m_aImperfectionLoad[k];
			if(curBase.LoadCaseKey != impF.LCase2K || curBase.AnalType != impF.nAnalType)
				continue;
			int nType = 3; //1D 2W 3Other
			T_STLD_D StldD;
			if(m_pDoc->m_pAttrCtrl->GetStld(curBase.LoadCaseKey, StldD))
			{
				if(StldD.LoadCaseType == _T("D"))
					nType = 1;
				else if(StldD.LoadCaseType == _T("W"))
					nType = 2;
			}
			for (int nlcom=0; nlcom<aNewLcom.GetCount(); nlcom++)
			{
				T_LCOM_BASE LcomBase; LcomBase.Initialize();
				LcomBase.AnalType    = D_LCOM_STATIC;
				LcomBase.LoadCaseKey = impF.LCase1K;
				if (impF.bElem == FALSE)
				{
					if (m_bGB55006_21)
					{
						if (bExistEQ)
							LcomBase.Factor = 0.5;
						else
							LcomBase.Factor = 1.0;
					}
					else
						LcomBase.Factor = curBase.Factor*impF.dMutiRatio_CH;
				}
				else
				{
					if (bExistEQ)
					{
						if (nType == 1)
							LcomBase.Factor = 1.0;
						else if (nType == 2)
							LcomBase.Factor = 0.2;
						else
							LcomBase.Factor = 0.5;
					}
					else
					{
						if (nType == 1)
							LcomBase.Factor = 1.0;
						else
							LcomBase.Factor = curBase.Factor / 1.5;
					}
				}
				if (m_bStandComb)
				{
					/*if (m_bGB55006_21)
					{
						if (bExistEQ)
							LcomBase.Factor = 0.5;
						else
							LcomBase.Factor = 1.0;
					}
					else*/
						LcomBase.Factor = 1.0;
				}

				T_LCOM_D LcomDNew1 = aNewLcom[nlcom];         
				LcomDNew1.aCombination.Add(LcomBase);
				aNewLcomTemp.Add(LcomDNew1);

				T_LCOM_D LcomDNew2 = aNewLcom[nlcom];   
				LcomBase.Factor *= -1.0;
				LcomDNew2.aCombination.Add(LcomBase);
				aNewLcomTemp.Add(LcomDNew2);
			}   
			aNewLcom.Copy(aNewLcomTemp);
			aNewLcomTemp.RemoveAll();
		} 
	}

	return TRUE; 
}

int CLoadCombDefCtrl::Get_MaxLoadCombNameNo(int nLocmType)
{
	int MaxNo = 0;
	CArray<CString, CString&> strLcomList;
	m_pDoc->m_pAttrCtrl->GetLcomList(nLocmType, strLcomList);

	int Count = strLcomList.GetSize();
	for (int i = 0; i < Count; i++)
	{
		CString strName = strLcomList.GetAt(i);
		CString strPart = strName.Left(5);
		int nLen = strName.GetLength();
		CString strNo = _T("");
		if (strPart == _T("LcomD"))
			strNo = strName.Right(nLen - 5);
		else
			continue;
		int nCom = _ttoi(strNo);
		MaxNo = max(MaxNo, nCom);
	}
	return MaxNo;
}
