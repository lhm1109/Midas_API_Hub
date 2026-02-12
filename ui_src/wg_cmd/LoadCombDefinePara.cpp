#include "stdafx.h"
#include "LoadCombDefinePara.h"

CLoadCombItem::CLoadCombItem()
{
	m_dFactor = 1.0;
	m_sType   = _T("");
	m_key.Initialize();
	m_eType   = Type_Null;
}
BOOL CLoadCombItem::isValid() const
{
	return m_eType != Type_Null && m_sType.GetLength() > 0;
}


CLoadCombDefinePara::CLoadCombDefinePara(const CLoadCombDefineData *pData) : m_pData(pData)
{

}

void CLoadCombDefinePara::initial()
{
	m_mapTypeClassfy.clear();
	m_mapCase2Type.clear();
	m_mapType2Case.clear();
}


void CLoadCombDefinePara::test()
{
	CLoadCombDefineData *pData = const_cast<CLoadCombDefineData *>(m_pData);
	ASSERT(FALSE);
	pData->m_item.m_aType.Add(_T("D"));
	pData->m_item.m_aType.Add(_T("L"));
	pData->m_item.m_aType.Add(_T("W"));
	pData->m_item.m_aType.Add(_T("RS"));
	//
	pData->m_item.m_aFactor.SetSize(2);
	//
	pData->m_item.m_aFactor[0].Add(1.1);
	pData->m_item.m_aFactor[0].Add(1.2);
	pData->m_item.m_aFactor[0].Add(1.3);
	pData->m_item.m_aFactor[0].Add(0.0);
	//
	pData->m_item.m_aFactor[1].Add(1.1);
	pData->m_item.m_aFactor[1].Add(1.2);
	pData->m_item.m_aFactor[1].Add(1.3);
	pData->m_item.m_aFactor[1].Add(1.4);
	//
	m_mapTypeClassfy[NAME_LOADCASE_D]  = Type_A;
	m_mapTypeClassfy[NAME_LOADCASE_L]  = Type_B;
	m_mapTypeClassfy[NAME_LOADCASE_W]  = Type_BC;
	m_mapTypeClassfy[NAME_LOADCASE_RS] = Type_C;
	//
	LOAD_CASE_K_ARR &aDL = m_mapType2Case[NAME_LOADCASE_D];
	LOAD_CASE_K_ARR &aLL = m_mapType2Case[NAME_LOADCASE_L];
	LOAD_CASE_K_ARR &aWL = m_mapType2Case[NAME_LOADCASE_W];
	LOAD_CASE_K_ARR &aRS = m_mapType2Case[NAME_LOADCASE_RS];

	//活荷载不利布置
	pData->m_nSpecitlArrange = 1;
	pData->m_aLLUnfavorable.SetSize(2);
	pData->m_aLLUnfavorable[0].Add(aLL[0]);
	pData->m_aLLUnfavorable[0].Add(aLL[1]);
	pData->m_aLLUnfavorable[1].Add(aLL[0]);
	pData->m_aLLUnfavorable[1].Add(aLL[2]);
	//互斥
	pData->m_nSpecitlArrange = 2;
	pData->m_mapExclusion[aLL[1]].Add(aLL[0]);
	pData->m_mapExclusion[aLL[1]].Add(aLL[2]);
	//风+地震
	pData->m_mapBC2C[aWL[0]] = aRS[0];
	//双向地震
	pData->m_bBiEarthquake = true;
	pData->m_mapBiEarthquake[aRS[0]] = aRS[2];

}
//////////////////////////////////////////////////////////////////////////
void CLcomCombAutoRlt::add(LoadCombItemArr &data)
{
	m_aLcomb.Add(data);
}

void CLcomCombAutoRlt::add(CArrayEx<LoadCombItemArr, LoadCombItemArr&>  &aData)
{
	m_aLcomb.Append(aData);
}