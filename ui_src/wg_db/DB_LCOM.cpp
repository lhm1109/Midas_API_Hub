#include "stdafx.h"
#include "DB_LCOM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LCOM::CDB_LCOM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_lcom.InitHashTable(HASHSIZELCOM);
	m_nLcomType = 0;

	// DB가 연결될 때 LComMap 초기화 
	/*
	m_arLcomMaps[0] = nullptr;
	m_arLcomMaps[D_LCOMTYPE_GENERAL]	= m_pDoc->m_pStagCtrl->m_lcom;
	m_arLcomMaps[D_LCOMTYPE_STEEL]		= m_pDoc->m_pStagCtrl->m_lcst;
	m_arLcomMaps[D_LCOMTYPE_CONCRETE]	= m_pDoc->m_pStagCtrl->m_lcco;
	m_arLcomMaps[D_LCOMTYPE_FDN]		= m_pDoc->m_pStagCtrl->m_lcft;
	m_arLcomMaps[D_LCOMTYPE_SRC]		= m_pDoc->m_pStagCtrl->m_lcsc;
	m_arLcomMaps[D_LCOMTYPE_ALUMINUM]	= m_pDoc->m_pStagCtrl->m_lcal;
	m_arLcomMaps[D_LCOMTYPE_STLCOMP]	= m_pDoc->m_pStagCtrl->m_lcscm;
	m_arLcomMaps[D_LCOMTYPE_CFSTEEL]	= m_pDoc->m_pStagCtrl->m_lccf;
	m_arLcomMaps[D_LCOMTYPE_SEISMIC]	= m_pDoc->m_pStagCtrl->m_lcsm;
	m_arLcomMaps[D_LCOMTYPE_LINEAR_EVAL] = m_pDoc->m_pStagCtrl->m_lcln;
	*/
}

CDB_LCOM::~CDB_LCOM()
{
}

void CDB_LCOM::Add(T_LCOM_K Key,T_LCOM_D& rData)
{
	T_LCOM_D Data;
	BOOL bExist = m_lcom.Lookup(Key, Data);
	m_lcom.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_LCOM_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
	// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(m_nCmd, Key);
	}
}

BOOL CDB_LCOM::Del(T_LCOM_K Key)
{
	BOOL ret=m_lcom.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_LCOM_K key;
			T_LCOM_D data;
			if(Get(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
		// Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
		{
			m_pDoc->m_pStagCtrl->DeactiveEtc(m_nCmd, Key);
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_LCOM::Get(T_LCOM_K Key,T_LCOM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		//if (m_nLcomType != D_LCOMTYPE_GENERAL) return FALSE;
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		CMap<T_LCOM_K, T_LCOM_K, UINT, UINT>* pLcomMap = GetLcomMap();
		UINT uTemp;
		if (!pLcomMap->Lookup(Key, uTemp)) return FALSE;
	}
	return m_lcom.Lookup(Key,rData);
}

int CDB_LCOM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		//if (m_nLcomType != D_LCOMTYPE_GENERAL) return 0;
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		CMap<T_LCOM_K, T_LCOM_K, UINT, UINT>* pLcomMap = GetLcomMap();
		return pLcomMap->GetCount();
	}
	return m_lcom.GetCount();
}

POSITION CDB_LCOM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		//if (m_nLcomType != D_LCOMTYPE_GENERAL) return 0;
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		CMap<T_LCOM_K, T_LCOM_K, UINT, UINT>* pLcomMap = GetLcomMap();
		return pLcomMap->GetStartPosition();
	}
	return m_lcom.GetStartPosition();
}

void CDB_LCOM::GetNext(POSITION& rNextPosition,T_LCOM_K& rKey,T_LCOM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		//if (m_nLcomType != D_LCOMTYPE_GENERAL) { ASSERT(0); return; }
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		CMap<T_LCOM_K, T_LCOM_K, UINT, UINT>* pLcomMap = GetLcomMap();
		UINT uTemp;
		pLcomMap->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_lcom.Lookup(rKey, rData);
		return;
	}
	m_lcom.GetNextAssoc(rNextPosition,rKey,rData);
}

CMap<T_LCOM_K, T_LCOM_K, UINT, UINT>* CDB_LCOM::GetLcomMap()
{
	//return m_arLcomMaps[m_nLcomType];

#if 1 //cklee@midasit.com 조건문 제거 
	switch(m_nLcomType)
	{
	case D_LCOMTYPE_GENERAL:			return m_pDoc->m_pStagCtrl->m_lcom;
	case D_LCOMTYPE_STEEL:				return m_pDoc->m_pStagCtrl->m_lcst;
	case D_LCOMTYPE_CONCRETE:			return m_pDoc->m_pStagCtrl->m_lcco;
	case D_LCOMTYPE_FDN:					return m_pDoc->m_pStagCtrl->m_lcft;
	case D_LCOMTYPE_SRC:					return m_pDoc->m_pStagCtrl->m_lcsc;
	case D_LCOMTYPE_ALUMINUM:			return m_pDoc->m_pStagCtrl->m_lcal;
	case D_LCOMTYPE_STLCOMP:			return m_pDoc->m_pStagCtrl->m_lcscm;
	case D_LCOMTYPE_CFSTEEL:			return m_pDoc->m_pStagCtrl->m_lccf;
	case D_LCOMTYPE_SEISMIC:			return m_pDoc->m_pStagCtrl->m_lcsm;
	case D_LCOMTYPE_LINEAR_EVAL:  return m_pDoc->m_pStagCtrl->m_lcln;
	default: ASSERT(0);
	}
	return 0;
#endif
}
