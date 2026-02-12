#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_BREP.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

CDB_BREP::CDB_BREP()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_brep.InitHashTable(HASHSIZEBREP);
	m_elemtobrep.InitHashTable(HASHSIZEELEM);
	m_name2brep.InitHashTable(HASHSIZEBREP);

}

CDB_BREP::~CDB_BREP()
{
}

void CDB_BREP::Add(T_BREP_K Key,T_BREP_D& rData,CDB_ELEM* pElem)
{
	T_BREP_D Data;
	BOOL bExist = m_brep.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapNew;
	T_ELEM_K KeyElemTemp;
	if (bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapNew.InitHashTable(HASHSIZEELEM);
		mapOld.InitHashTable(HASHSIZEELEM);

		for (int i = 0; i < Data.arKeyElem.GetSize(); i++)  mapOld.SetAt(Data.arKeyElem[i], Data.arKeyElem[i]);   // 이전 멤버
		for (int i = 0; i < rData.arKeyElem.GetSize(); i++) mapNew.SetAt(rData.arKeyElem[i], rData.arKeyElem[i]); // 새로운 멤버   
		for (int i = 0; i < Data.arKeyElem.GetSize(); i++)   // 이전 멤버의 모든 요소에 대해
		{
			if (!mapNew.Lookup(Data.arKeyElem[i], KeyElemTemp))   // 새로운 멤버의 요소리스트에 이전 멤버의 요소가 없으면 
				VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_BREP_CMD, Key));  // pElem에서 없는 요소를 삭제한다.      
		}
	}
	m_brep.SetAt(Key, rData);  // 이전 멤버가 있어도 새로운 멤버로 덮어쓴다(CMap에 저장).
	for (int i = 0; i < rData.arKeyElem.GetSize(); i++)  // 새로운 멤버의 모든 요소에 대해
	{
		if (bExist) // 새로운 멤버의 키가 이전 멤버의 키와 같으면
		{
			if (!mapOld.Lookup(rData.arKeyElem[i], KeyElemTemp))  // 새로운 멤버의 요소중 이전 멤버의 요소리스트에 없는 것이 있으면 
				pElem->AddListItem(rData.arKeyElem[i], LT_BREP_CMD, Key); // pElem에 없는 요소(새로운 멤버에만 있는 요소)를 추가한다.
		}
		else       // 새로운 멤버의 키가 이전 멤버의 키와 다르면
			pElem->AddListItem(rData.arKeyElem[i], LT_BREP_CMD, Key);  // 새로운 멤버에 있는 모든 요소를 pElem에 추가한다.    
	}
	if (Key == m_nStartNum)
	{
		T_BREP_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!Get(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;


	CMap<T_ELEM_K, T_ELEM_K, T_BREP_K, T_BREP_K>  mapElemOld;
	CMap<T_ELEM_K, T_ELEM_K, T_BREP_K, T_BREP_K>  mapElemNew;
	T_BREP_K KeyBrepTemp;
	if (bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);
		for (int i = 0; i < Data.arKeyElem.GetSize(); i++)  mapElemOld.SetAt(Data.arKeyElem[i], Key);  // 이전 멤버
		for (int i = 0; i < rData.arKeyElem.GetSize(); i++) mapElemNew.SetAt(rData.arKeyElem[i], Key); // 새로운 멤버    
		for (int i = 0; i < Data.arKeyElem.GetSize(); i++)   // 이전 멤버의 모든 요소에 대해
		{
			if (!mapElemNew.Lookup(Data.arKeyElem[i], KeyBrepTemp))
				m_elemtobrep.RemoveKey(Data.arKeyElem[i]);
		}
		if (Data.strName.GetLength() > 0)
			m_name2brep.RemoveKey(Data.strName);
	}
	for (int i = 0; i < rData.arKeyElem.GetSize(); i++)
	{
		m_elemtobrep.SetAt(rData.arKeyElem[i], Key);
	}
	if (rData.strName.GetLength() > 0)
		m_name2brep[rData.strName] = Key;
}

BOOL CDB_BREP::Del(T_BREP_K Key,CDB_ELEM* pElem)
{
	T_BREP_D Data;
	T_BREP_K KeyBrep;
	BOOL bExist = m_brep.Lookup(Key, Data);
	if (bExist)
	{
		BOOL ret = m_brep.RemoveKey(Key);
		ASSERT(ret);
		if (ret)
		{
			if (Data.strName.GetLength() > 0)
				m_name2brep.RemoveKey(Data.strName);
			for (int i = 0; i < Data.arKeyElem.GetSize(); i++)
			{
				VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_BREP_CMD, Key));
				if (m_elemtobrep.Lookup(Data.arKeyElem[i], KeyBrep))
					m_elemtobrep.RemoveKey(Data.arKeyElem[i]);
			}
			if (Key < m_nStartNum) m_nStartNum = Key;
			if (Key == m_nLastNum)
			{
				T_BREP_K key;
				T_BREP_D data;
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

BOOL CDB_BREP::GetBrepAssigned(T_ELEM_K KeyElem, T_BREP_K& rKeyBrep)
{
	//  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//    return m_pDoc->m_pStagCtrl->m_elemtobrep->Lookup(KeyElem, KeyBrep);
	return m_elemtobrep.Lookup(KeyElem, rKeyBrep);
}

//-------------------------------------------------------------------------
BOOL CDB_BREP::Get(T_BREP_K Key, T_BREP_D& rData)
{
	return m_brep.Lookup(Key, rData);
}

int CDB_BREP::GetCount()
{
	return m_brep.GetCount();
}

POSITION CDB_BREP::GetStart()
{
	return m_brep.GetStartPosition();
}

void CDB_BREP::GetNext(POSITION& rNextPosition, T_BREP_K& rKey, T_BREP_D& rData)
{
	m_brep.GetNextAssoc(rNextPosition, rKey, rData);
}

T_BREP_K CDB_BREP::FindByName(LPCTSTR sName) const
{
	T_BREP_K key = NULL;
	if (m_name2brep.Lookup(sName, key))
		return key;
	return NULL;
}