#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RSDL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RSDL::CDB_RSDL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rsdl.InitHashTable(HASHSIZERSDL);
	m_elemtorsdl.InitHashTable(HASHSIZEELEM);
	m_nStartNum=1;
	m_nLastNum=0;
}

CDB_RSDL::~CDB_RSDL()
{

}

void CDB_RSDL::Add(T_RSDL_K Key,T_RSDL_D& rData,CDB_ELEM* pElem)
{
	T_RSDL_D Data;  
	BOOL bExist = m_rsdl.Lookup(Key, Data);
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
				VERIFY(pElem->DelListItem(Data.aElemList.GetAt(j), LT_RSDL_CMD, Key));  // pElem에서 없는 요소를 삭제한다.      
		}		
	} 
	m_rsdl.SetAt(Key, rData);  // 이전 멤버가 있어도 새로운 멤버로 덮어쓴다(CMap에 저장).

	// ElemList Part 
	for(j=0; j<rData.aElemList.GetSize(); j++)  // 새로운 멤버의 모든 요소에 대해
	{
		if(bExist) // 새로운 멤버의 키가 이전 멤버의 키와 같으면
		{
			if(!mapOld.Lookup(rData.aElemList.GetAt(j), KeyElemTemp))  // 새로운 멤버의 요소중 이전 멤버의 요소리스트에 없는 것이 있으면 
				pElem->AddListItem(rData.aElemList.GetAt(j), LT_RSDL_CMD, Key); // pElem에 없는 요소(새로운 멤버에만 있는 요소)를 추가한다.
		}
		else       // 새로운 멤버의 키가 이전 멤버의 키와 다르면
			pElem->AddListItem(rData.aElemList.GetAt(j), LT_RSDL_CMD, Key);  // 새로운 멤버에 있는 모든 요소를 pElem에 추가한다.    
	}
	
	if(Key == m_nStartNum)
	{
		T_RSDL_D data;
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
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RSDL_CMD, Key);
	}

	CMap<T_ELEM_K,T_ELEM_K,T_RSDL_K,T_RSDL_K>  mapElemOld;
	CMap<T_ELEM_K,T_ELEM_K,T_RSDL_K,T_RSDL_K>  mapElemNew;  
	T_RSDL_K KeyRsdlTemp;
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
			if(!mapElemNew.Lookup(Data.aElemList.GetAt(j), KeyRsdlTemp)) 
				m_elemtorsdl.RemoveKey(Data.aElemList.GetAt(j));           
		}    		
	} 

	for(j=0; j<rData.aElemList.GetSize(); j++)
	{
		m_elemtorsdl.SetAt(rData.aElemList.GetAt(j), Key);  
	}  
}

BOOL CDB_RSDL::Del(T_RSDL_K Key,CDB_ELEM* pElem)
{
	T_RSDL_D Data;
	T_RSDL_K KeyRsdl;
	BOOL bExist=m_rsdl.Lookup(Key, Data);	

	if(bExist)
	{
		BOOL ret=m_rsdl.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			int j;
			for(j=0; j<Data.aElemList.GetSize(); j++)
			{
				VERIFY(pElem->DelListItem(Data.aElemList.GetAt(j), LT_RSDL_CMD, Key));
				if(m_elemtorsdl.Lookup(Data.aElemList.GetAt(j), KeyRsdl))
					m_elemtorsdl.RemoveKey(Data.aElemList.GetAt(j));
			}      			

			if(Key < m_nStartNum) m_nStartNum = Key;
			if(Key == m_nLastNum)
			{
				T_RSDL_K key;
				T_RSDL_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RSDL_CMD, Key);      
		}
		return ret;
	}
	return FALSE;
}

BOOL CDB_RSDL::GetRsdlAssigned(T_ELEM_K KeyElem, T_RSDL_K& rKeyRsdl)
{  
//  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//    return m_pDoc->m_pStagCtrl->m_elemtorsdl->Lookup(KeyElem, KeyRsdl);
	return m_elemtorsdl.Lookup(KeyElem, rKeyRsdl);
}

//-------------------------------------------------------------------------
BOOL CDB_RSDL::Get(T_RSDL_K Key,T_RSDL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
	}
	return m_rsdl.Lookup(Key,rData);
}

int CDB_RSDL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	}
	return m_rsdl.GetCount();
}

POSITION CDB_RSDL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	}
	return m_rsdl.GetStartPosition();
}

void CDB_RSDL::GetNext(POSITION& rNextPosition,T_RSDL_K& rKey,T_RSDL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
	}
	m_rsdl.GetNextAssoc(rNextPosition,rKey,rData);
}
