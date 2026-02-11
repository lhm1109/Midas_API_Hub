#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_MEMB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MEMB::CDB_MEMB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_memb.InitHashTable(HASHSIZEMEMB);
	m_elemtomemb.InitHashTable(HASHSIZEELEM);
	m_name2memb.InitHashTable(HASHSIZEMEMB);
}

CDB_MEMB::~CDB_MEMB()
{

}

void CDB_MEMB::Add(T_MEMB_K Key,T_MEMB_D& rData,CDB_ELEM* pElem)
{
	T_MEMB_D Data;  
	BOOL bExist = m_memb.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapNew;
	T_ELEM_K KeyElemTemp;
	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapNew.InitHashTable(HASHSIZEELEM);
		mapOld.InitHashTable(HASHSIZEELEM);

		for(int i = 0; i<Data.aElemList.GetSize(); i++)  mapOld.SetAt(Data.aElemList[i], Data.aElemList[i]);   // 이전 멤버
		for(int i = 0; i<rData.aElemList.GetSize(); i++) mapNew.SetAt(rData.aElemList[i], rData.aElemList[i]); // 새로운 멤버   
		for(int i = 0; i<Data.aElemList.GetSize(); i++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapNew.Lookup(Data.aElemList[i], KeyElemTemp))   // 새로운 멤버의 요소리스트에 이전 멤버의 요소가 없으면 
				VERIFY(pElem->DelListItem(Data.aElemList[i], LT_MEMB_CMD, Key));  // pElem에서 없는 요소를 삭제한다.      
		}
	} 
	m_memb.SetAt(Key, rData);  // 이전 멤버가 있어도 새로운 멤버로 덮어쓴다(CMap에 저장).
	for(int i=0; i<rData.aElemList.GetSize(); i++)  // 새로운 멤버의 모든 요소에 대해
	{
		if(bExist) // 새로운 멤버의 키가 이전 멤버의 키와 같으면
		{
			if(!mapOld.Lookup(rData.aElemList[i], KeyElemTemp))  // 새로운 멤버의 요소중 이전 멤버의 요소리스트에 없는 것이 있으면 
				pElem->AddListItem(rData.aElemList[i], LT_MEMB_CMD, Key); // pElem에 없는 요소(새로운 멤버에만 있는 요소)를 추가한다.
		}
		else       // 새로운 멤버의 키가 이전 멤버의 키와 다르면
			pElem->AddListItem(rData.aElemList[i], LT_MEMB_CMD, Key);  // 새로운 멤버에 있는 모든 요소를 pElem에 추가한다.    
	}
	if(Key == m_nStartNum)
	{
		T_MEMB_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum, data))break;
		}
	}
	if(Key > m_nLastNum) m_nLastNum = Key;
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if(m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_MEMB_CMD, Key);
	}

	CMap<T_ELEM_K,T_ELEM_K,T_MEMB_K,T_MEMB_K>  mapElemOld;
	CMap<T_ELEM_K,T_ELEM_K,T_MEMB_K,T_MEMB_K>  mapElemNew;  
	T_MEMB_K KeyMembTemp;
	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);   
		for(int i = 0; i<Data.aElemList.GetSize(); i++)  mapElemOld.SetAt(Data.aElemList[i], Key);  // 이전 멤버
		for(int i = 0; i<rData.aElemList.GetSize(); i++) mapElemNew.SetAt(rData.aElemList[i], Key); // 새로운 멤버    
		for(int i = 0; i<Data.aElemList.GetSize(); i++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapElemNew.Lookup(Data.aElemList[i], KeyMembTemp)) 
				m_elemtomemb.RemoveKey(Data.aElemList[i]);           
		}    
		if (Data.strName.GetLength() > 0)
			m_name2memb.RemoveKey(Data.strName);
	} 
	for(int i = 0; i < rData.aElemList.GetSize(); i++)
	{
		m_elemtomemb.SetAt(rData.aElemList[i], Key);  
	}  
	if (rData.strName.GetLength() > 0)
		m_name2memb[rData.strName] = Key;
}

BOOL CDB_MEMB::Del(T_MEMB_K Key,CDB_ELEM* pElem)
{
	T_MEMB_D Data;
	T_MEMB_K KeyMemb;
	BOOL bExist=m_memb.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_memb.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (Data.strName.GetLength() > 0)
				m_name2memb.RemoveKey(Data.strName);
			for(int i=0; i<Data.aElemList.GetSize(); i++)
			{
				VERIFY(pElem->DelListItem(Data.aElemList[i], LT_MEMB_CMD, Key));
				if(m_elemtomemb.Lookup(Data.aElemList[i], KeyMemb))
					m_elemtomemb.RemoveKey(Data.aElemList[i]);
			}      
			if(Key < m_nStartNum) m_nStartNum = Key;
			if(Key == m_nLastNum)
			{
				T_MEMB_K key;
				T_MEMB_D data;
				if(Get(m_nLastNum - 1, data)) m_nLastNum--;
				else
				{
					m_nLastNum = 0;
					POSITION pos = GetStart();
					while(pos != NULL)
					{
						GetNext(pos,key,data);
						if(key > m_nLastNum) m_nLastNum = key;
					}
				}
			}
			// Current Stage가 Final Stage이면 Stage DB를 Update한다.
			if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_MEMB_CMD, Key);      
		}
		return ret;
	}
	return FALSE;
}

BOOL CDB_MEMB::GetMembAssigned(T_ELEM_K KeyElem, T_MEMB_K& rKeyMemb)
{  
//  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//    return m_pDoc->m_pStagCtrl->m_elemtomemb->Lookup(KeyElem, KeyMemb);
	return m_elemtomemb.Lookup(KeyElem, rKeyMemb);
}

//-------------------------------------------------------------------------
BOOL CDB_MEMB::Get(T_MEMB_K Key,T_MEMB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_memb->Lookup(Key, uTemp)) return FALSE;
	}
	return m_memb.Lookup(Key,rData);
}

int CDB_MEMB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_memb->GetCount();
	}
	return m_memb.GetCount();
}

POSITION CDB_MEMB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_memb->GetStartPosition();
	}
	return m_memb.GetStartPosition();
}

void CDB_MEMB::GetNext(POSITION& rNextPosition,T_MEMB_K& rKey,T_MEMB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_memb->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_memb.Lookup(rKey, rData);
		return;
	}
	m_memb.GetNextAssoc(rNextPosition,rKey,rData);
}

T_MEMB_K CDB_MEMB::FindByName(LPCTSTR sName) const
{
	T_MEMB_K key = NULL;
	if (m_name2memb.Lookup(sName, key))
		return key;
	return NULL;
}