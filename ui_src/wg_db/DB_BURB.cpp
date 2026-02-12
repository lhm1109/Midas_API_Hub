#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_BURB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BURB::CDB_BURB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_burb.InitHashTable(HASHSIZEBURB);
	m_elemtoburb.InitHashTable(HASHSIZEELEM);
	m_nStartNum=1;
	m_nLastNum=0;
}

CDB_BURB::~CDB_BURB()
{

}

void CDB_BURB::Add(T_BURB_K Key,T_BURB_D& rData,CDB_ELEM* pElem)
{
	T_BURB_D Data;  
	BOOL bExist = m_burb.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapNew;
	T_ELEM_K KeyElemTemp;
	int j;

	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapNew.InitHashTable(HASHSIZEELEM);
		mapOld.InitHashTable(HASHSIZEELEM);
		
		// 이전 멤버를 찾는다...
		for(j=0; j<Data.aElemList.GetSize(); j++)  
			mapOld.SetAt(Data.aElemList.GetAt(j), Data.aElemList.GetAt(j));   		
		// 새로운 멤버
		for(j=0; j<rData.aElemList.GetSize(); j++)  
			mapNew.SetAt(rData.aElemList.GetAt(j), rData.aElemList.GetAt(j));    
		
		for(j=0; j<Data.aElemList.GetSize(); j++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapNew.Lookup(Data.aElemList.GetAt(j), KeyElemTemp))   // 새로운 멤버의 요소리스트에 이전 멤버의 요소가 없으면 
				VERIFY(pElem->DelListItem(Data.aElemList.GetAt(j), LT_BURB_CMD, Key));  // pElem에서 없는 요소를 삭제한다.      
		}		
	} 
	m_burb.SetAt(Key, rData);  // 이전 멤버가 있어도 새로운 멤버로 덮어쓴다(CMap에 저장).

	// ElemList Part 
	for(j=0; j<rData.aElemList.GetSize(); j++)  // 새로운 멤버의 모든 요소에 대해
	{
		if(bExist) // 새로운 멤버의 키가 이전 멤버의 키와 같으면
		{
			if(!mapOld.Lookup(rData.aElemList.GetAt(j), KeyElemTemp))  // 새로운 멤버의 요소중 이전 멤버의 요소리스트에 없는 것이 있으면 
				pElem->AddListItem(rData.aElemList.GetAt(j), LT_BURB_CMD, Key); // pElem에 없는 요소(새로운 멤버에만 있는 요소)를 추가한다.
		}
		else       // 새로운 멤버의 키가 이전 멤버의 키와 다르면
			pElem->AddListItem(rData.aElemList.GetAt(j), LT_BURB_CMD, Key);  // 새로운 멤버에 있는 모든 요소를 pElem에 추가한다.    
	}
	
	if(Key == m_nStartNum)
	{
		T_BURB_D data;
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
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_BURB_CMD, Key);
	}

	CMap<T_ELEM_K,T_ELEM_K,T_BURB_K,T_BURB_K>  mapElemOld;
	CMap<T_ELEM_K,T_ELEM_K,T_BURB_K,T_BURB_K>  mapElemNew;  
	T_BURB_K KeyRcplTemp;
	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);   

		// 이전 멤버
		for(j=0; j<Data.aElemList.GetSize(); j++)  
			mapElemOld.SetAt(Data.aElemList.GetAt(j), Key);  		
		// 새로운 멤버
		for(j=0; j<rData.aElemList.GetSize(); j++) 
			mapElemNew.SetAt(rData.aElemList.GetAt(j), Key);     
		
		for(j=0; j<Data.aElemList.GetSize(); j++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapElemNew.Lookup(Data.aElemList.GetAt(j), KeyRcplTemp)) 
				m_elemtoburb.RemoveKey(Data.aElemList.GetAt(j));           
		}    		
	} 

	for(j=0; j<rData.aElemList.GetSize(); j++)
	{
		m_elemtoburb.SetAt(rData.aElemList.GetAt(j), Key);  
	}  
}

BOOL CDB_BURB::Del(T_BURB_K Key,CDB_ELEM* pElem)
{
	T_BURB_D Data;
	T_BURB_K KeyRcpl;
	BOOL bExist=m_burb.Lookup(Key, Data);	

	if(bExist)
	{
		BOOL ret=m_burb.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			int j;
			for(j=0; j<Data.aElemList.GetSize(); j++)
			{
				VERIFY(pElem->DelListItem(Data.aElemList.GetAt(j), LT_BURB_CMD, Key));
				if(m_elemtoburb.Lookup(Data.aElemList.GetAt(j), KeyRcpl))
					m_elemtoburb.RemoveKey(Data.aElemList.GetAt(j));
			}      			

			if(Key < m_nStartNum) m_nStartNum = Key;
			if(Key == m_nLastNum)
			{
				T_BURB_K key;
				T_BURB_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_BURB_CMD, Key);      
		}
		return ret;
	}
	return FALSE;
}

BOOL CDB_BURB::GetRcplAssigned(T_ELEM_K KeyElem, T_BURB_K& rKeyRcpl)
{  
//  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//    return m_pDoc->m_pStagCtrl->m_elemtoburb->Lookup(KeyElem, KeyRcpl);
	return m_elemtoburb.Lookup(KeyElem, rKeyRcpl);
}

//-------------------------------------------------------------------------
BOOL CDB_BURB::Get(T_BURB_K Key,T_BURB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_burb->Lookup(Key, uTemp)) return FALSE;
	}
	return m_burb.Lookup(Key,rData);
}

int CDB_BURB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_burb->GetCount();
	}
	return m_burb.GetCount();
}

POSITION CDB_BURB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_burb->GetStartPosition();
	}
	return m_burb.GetStartPosition();
}

void CDB_BURB::GetNext(POSITION& rNextPosition,T_BURB_K& rKey,T_BURB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_burb->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_burb.Lookup(rKey, rData);
		return;
	}
	m_burb.GetNextAssoc(rNextPosition,rKey,rData);
}
