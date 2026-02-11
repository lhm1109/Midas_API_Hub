#include "stdafx.h"
#include "DB_WEBV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_WEBV::CDB_WEBV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_webv.InitHashTable(HASHSIZEWEBV);
}

CDB_WEBV::~CDB_WEBV()
{

}

void CDB_WEBV::Add(T_WEBV_K Key, T_WEBV_D& rData/*, CDB_ELEM* pElem*/)
{
	T_WEBV_D Data;
 	BOOL bExist = m_webv.Lookup(Key, Data);
	m_webv.SetAt(Key, rData);  // 이전 멤버가 있어도 새로운 멤버로 덮어쓴다(CMap에 저장).

	if (Key == m_nStartNum)
	{
		T_WEBV_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!Get(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;


	//////////////////////////////////////////////////////////////////////////
	CMap<T_VBEM_K, T_VBEM_K, T_WEBV_K, T_WEBV_K>  mapElemOld;
	CMap<T_VBEM_K, T_VBEM_K, T_WEBV_K, T_WEBV_K>  mapElemNew;
	T_WEBV_K KeyWebpTemp;
	if (bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);
		for (int i = 0; i < Data.aElemList.GetSize(); i++)  mapElemOld.SetAt(Data.aElemList[i], Key);  // 이전 멤버
		for (int i = 0; i < rData.aElemList.GetSize(); i++) mapElemNew.SetAt(rData.aElemList[i], Key); // 새로운 멤버    
		for (int i = 0; i < Data.aElemList.GetSize(); i++)   // 이전 멤버의 모든 요소에 대해
		{
			if (!mapElemNew.Lookup(Data.aElemList[i], KeyWebpTemp))
				m_elemtowebv.RemoveKey(Data.aElemList[i]);
		}
	}
	for (int i = 0; i < rData.aElemList.GetSize(); i++)
	{
		m_elemtowebv.SetAt(rData.aElemList[i], Key);
	}
}

BOOL CDB_WEBV::Del(T_WEBV_K Key)
{
// 	BOOL ret = m_webv.RemoveKey(Key);
// 	ASSERT(ret);
// 	return ret;

	T_WEBV_D Data;
	T_WEBV_K KeyWebp;
	BOOL bExist = m_webv.Lookup(Key, Data);
	if (bExist)
	{
		BOOL ret = m_webv.RemoveKey(Key);
		ASSERT(ret);
		if (ret)
		{
			for (int i = 0; i < Data.aElemList.GetSize(); i++)
			{
				if (m_elemtowebv.Lookup(Data.aElemList[i], KeyWebp))
					m_elemtowebv.RemoveKey(Data.aElemList[i]);
			}
			if (Key < m_nStartNum) m_nStartNum = Key;
			if (Key == m_nLastNum)
			{
				T_WEBV_K key;
				T_WEBV_D data;
				if (Get(m_nLastNum - 1, data)) m_nLastNum--;
				else
				{
					m_nLastNum = 0;
					POSITION pos = GetStart();
					while (pos != NULL)
					{
						GetNext(pos, key, data);
						if (key > m_nLastNum) m_nLastNum = key;
					}
				}
			}
		}
		return ret;
	}
	return FALSE;

}

//-------------------------------------------------------------------------
BOOL CDB_WEBV::Get(T_WEBV_K Key, T_WEBV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;

	return m_webv.Lookup(Key, rData);
}

int CDB_WEBV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;

	return m_webv.GetCount();
}

POSITION CDB_WEBV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;

	return m_webv.GetStartPosition();
}

void CDB_WEBV::GetNext(POSITION& rNextPosition, T_WEBV_K& rKey, T_WEBV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}

	m_webv.GetNextAssoc(rNextPosition, rKey, rData);
}

BOOL CDB_WEBV::GetWebvAssigned(T_VBEM_K KeyElem, T_WEBV_K& rKeyWebv)
{
	return m_elemtowebv.Lookup(KeyElem, rKeyWebv);
}

