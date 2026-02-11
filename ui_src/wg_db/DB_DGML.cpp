#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_DGML.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DGML::CDB_DGML()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_nCmd = LT_DGML_CMD;
	m_dgml.InitHashTable(HASHSIZEDGML); 
}

CDB_DGML::~CDB_DGML()
{

}

void CDB_DGML::Add(T_DGML_K Key,T_DGML_D& rData)
{
	//m_dgml.SetAt(Key,rData);

	T_DGML_D Data;
	BOOL bExist = m_dgml.Lookup(Key, Data);
	m_dgml.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_DGML_D data;
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

BOOL CDB_DGML::Del(T_DGML_K Key)
{
	T_DGML_D Data;
	BOOL bExist=m_dgml.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_dgml.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_DGML_K key;
				T_DGML_D data;
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
		
	return bExist;
}


BOOL CDB_DGML::Get(T_DGML_K Key,T_DGML_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		
		CMap<T_DGML_K, T_DGML_K, UINT, UINT>* pDgmlMap = GetDgmlMap();
		UINT uTemp;
		if (!pDgmlMap->Lookup(Key, uTemp)) return FALSE;
	}
	return m_dgml.Lookup(Key,rData);
}

int CDB_DGML::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		CMap<T_DGML_K, T_DGML_K, UINT, UINT>* pDgmlMap = GetDgmlMap();
		return pDgmlMap->GetCount();
	}
	return m_dgml.GetCount();
}

POSITION CDB_DGML::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		CMap<T_DGML_K, T_DGML_K, UINT, UINT>* pDgmlMap = GetDgmlMap();
		return pDgmlMap->GetStartPosition();
	}
	return m_dgml.GetStartPosition();
}

void CDB_DGML::GetNext(POSITION& rNextPosition,T_DGML_K& rKey,T_DGML_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		CMap<T_DGML_K, T_DGML_K, UINT, UINT>* pDgmlMap = GetDgmlMap();
		UINT uTemp;
		pDgmlMap->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_dgml.Lookup(rKey, rData);
		return;
	}
	m_dgml.GetNextAssoc(rNextPosition,rKey,rData);
}

CMap<T_DGML_K, T_DGML_K, UINT, UINT>* CDB_DGML::GetDgmlMap()
{
	return m_pDoc->m_pStagCtrl->m_dgml;
}
