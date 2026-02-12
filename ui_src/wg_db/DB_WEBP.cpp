#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_WEBP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_WEBP::CDB_WEBP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_webp.InitHashTable(HASHSIZEWEBP);
	m_elemtowebp.InitHashTable(HASHSIZEELEM);
}

CDB_WEBP::~CDB_WEBP()
{

}

void CDB_WEBP::Add(T_WEBP_K Key, T_WEBP_D& rData, CDB_ELEM* pElem)
{
	T_WEBP_D Data;
	BOOL bExist = m_webp.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapNew;
	T_ELEM_K KeyElemTemp;
	if (bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapNew.InitHashTable(HASHSIZEELEM);
		mapOld.InitHashTable(HASHSIZEELEM);
		for(int i = 0; i < Data.aElemList.GetSize(); i++)  mapOld.SetAt(Data.aElemList[i], Data.aElemList[i]);   // 이전 멤버
		for(int i = 0; i < rData.aElemList.GetSize(); i++) mapNew.SetAt(rData.aElemList[i], rData.aElemList[i]); // 새로운 멤버   
		for(int i = 0; i < Data.aElemList.GetSize(); i++)   // 이전 멤버의 모든 요소에 대해
		{
			if (!mapNew.Lookup(Data.aElemList[i], KeyElemTemp))   // 새로운 멤버의 요소리스트에 이전 멤버의 요소가 없으면 
				VERIFY(pElem->DelListItem(Data.aElemList[i], LT_WEBP_CMD, Key));  // pElem에서 없는 요소를 삭제한다.      
		}
	}
	m_webp.SetAt(Key, rData);  // 이전 멤버가 있어도 새로운 멤버로 덮어쓴다(CMap에 저장).
	for (int i = 0; i < rData.aElemList.GetSize(); i++)  // 새로운 멤버의 모든 요소에 대해
	{
		if (bExist) // 새로운 멤버의 키가 이전 멤버의 키와 같으면
		{
			if (!mapOld.Lookup(rData.aElemList[i], KeyElemTemp))  // 새로운 멤버의 요소중 이전 멤버의 요소리스트에 없는 것이 있으면 
				pElem->AddListItem(rData.aElemList[i], LT_WEBP_CMD, Key); // pElem에 없는 요소(새로운 멤버에만 있는 요소)를 추가한다.
		}
		else       // 새로운 멤버의 키가 이전 멤버의 키와 다르면
			pElem->AddListItem(rData.aElemList[i], LT_WEBP_CMD, Key);  // 새로운 멤버에 있는 모든 요소를 pElem에 추가한다.    
	}
	if (Key == m_nStartNum)
	{
		T_WEBP_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!Get(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_WEBP_CMD, Key);
	}

	CMap<T_ELEM_K, T_ELEM_K, T_WEBP_K, T_WEBP_K>  mapElemOld;
	CMap<T_ELEM_K, T_ELEM_K, T_WEBP_K, T_WEBP_K>  mapElemNew;
	T_WEBP_K KeyWebpTemp;
	if (bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);
		for(int i = 0; i < Data.aElemList.GetSize(); i++)  mapElemOld.SetAt(Data.aElemList[i], Key);  // 이전 멤버
		for(int i = 0; i < rData.aElemList.GetSize(); i++) mapElemNew.SetAt(rData.aElemList[i], Key); // 새로운 멤버    
		for(int i = 0; i < Data.aElemList.GetSize(); i++)   // 이전 멤버의 모든 요소에 대해
		{
			if (!mapElemNew.Lookup(Data.aElemList[i], KeyWebpTemp))
				m_elemtowebp.RemoveKey(Data.aElemList[i]);
		}
	}
	for(int i = 0; i < rData.aElemList.GetSize(); i++)
	{
		m_elemtowebp.SetAt(rData.aElemList[i], Key);
	}
}

BOOL CDB_WEBP::Del(T_WEBP_K Key, CDB_ELEM* pElem)
{
	T_WEBP_D Data;
	T_WEBP_K KeyWebp;
	BOOL bExist = m_webp.Lookup(Key, Data);
	if (bExist)
	{
		BOOL ret = m_webp.RemoveKey(Key);
		ASSERT(ret);
		if (ret)
		{
			for (int i = 0; i < Data.aElemList.GetSize(); i++)
			{
				VERIFY(pElem->DelListItem(Data.aElemList[i], LT_WEBP_CMD, Key));
				if (m_elemtowebp.Lookup(Data.aElemList[i], KeyWebp))
					m_elemtowebp.RemoveKey(Data.aElemList[i]);
			}
			if (Key < m_nStartNum) m_nStartNum = Key;
			if (Key == m_nLastNum)
			{
				T_WEBP_K key;
				T_WEBP_D data;
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
			// Current Stage가 Final Stage이면 Stage DB를 Update한다.
			if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
				m_pDoc->m_pStagCtrl->DeactiveDesign(LT_WEBP_CMD, Key);
		}
		return ret;
	}
	return FALSE;
}

BOOL CDB_WEBP::GetWebpAssigned(T_ELEM_K KeyElem, T_WEBP_K& rKeyWebp)
{
	//  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//    return m_pDoc->m_pStagCtrl->m_elemtowebp->Lookup(KeyElem, KeyWebp);
	return m_elemtowebp.Lookup(KeyElem, rKeyWebp);
}

//-------------------------------------------------------------------------
BOOL CDB_WEBP::Get(T_WEBP_K Key, T_WEBP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_webp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_webp.Lookup(Key, rData);
}

int CDB_WEBP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_webp->GetCount();
	}
	return m_webp.GetCount();
}

POSITION CDB_WEBP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_webp->GetStartPosition();
	}
	return m_webp.GetStartPosition();
}

void CDB_WEBP::GetNext(POSITION& rNextPosition, T_WEBP_K& rKey, T_WEBP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_webp->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_webp.Lookup(rKey, rData);
		return;
	}
	m_webp.GetNextAssoc(rNextPosition, rKey, rData);
}
