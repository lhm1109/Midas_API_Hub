#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_HCBM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HCBM::CDB_HCBM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_hcbm.InitHashTable(HASHSIZEHCBM);
	m_elemtohcbm.InitHashTable(HASHSIZEELEM);
}

CDB_HCBM::~CDB_HCBM()
{

}

void CDB_HCBM::Add(T_HCBM_K Key,T_HCBM_D& rData,CDB_ELEM* pElem)
{
	T_HCBM_D Data;  
	BOOL bExist = m_hcbm.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapNew;
	T_ELEM_K KeyElemTemp;

	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapNew.InitHashTable(HASHSIZEELEM);
		mapOld.InitHashTable(HASHSIZEELEM);
		
		// 이전 멤버를 찾는다...
		for(int i = 0; i<3; i++) for(int j=0; j<Data.aElemListPart[i].GetSize(); j++)
			mapOld.SetAt(Data.aElemListPart[i].GetAt(j), Data.aElemListPart[i].GetAt(j));   		
		// 새로운 멤버
		for(int i = 0; i<3; i++) for(int j=0; j<Data.aElemListPart[i].GetSize(); j++)
			mapNew.SetAt(rData.aElemListPart[i].GetAt(j), rData.aElemListPart[i].GetAt(j));    
		
		for(int i = 0; i<3; i++) for(int j=0; j<Data.aElemListPart[i].GetSize(); j++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapNew.Lookup(Data.aElemListPart[i].GetAt(j), KeyElemTemp))   // 새로운 멤버의 요소리스트에 이전 멤버의 요소가 없으면 
				VERIFY(pElem->DelListItem(Data.aElemListPart[i].GetAt(j), LT_HCBM_CMD, Key));  // pElem에서 없는 요소를 삭제한다.      
		}		
	} 
	m_hcbm.SetAt(Key, rData);  // 이전 멤버가 있어도 새로운 멤버로 덮어쓴다(CMap에 저장).

	// ElemList Part 
	for(int i = 0; i<3; i++) for(int j=0; j<rData.aElemListPart[i].GetSize(); j++)  // 새로운 멤버의 모든 요소에 대해
	{
		if(bExist) // 새로운 멤버의 키가 이전 멤버의 키와 같으면
		{
			if(!mapOld.Lookup(rData.aElemListPart[i].GetAt(j), KeyElemTemp))  // 새로운 멤버의 요소중 이전 멤버의 요소리스트에 없는 것이 있으면 
				pElem->AddListItem(rData.aElemListPart[i].GetAt(j), LT_HCBM_CMD, Key); // pElem에 없는 요소(새로운 멤버에만 있는 요소)를 추가한다.
		}
		else       // 새로운 멤버의 키가 이전 멤버의 키와 다르면
			pElem->AddListItem(rData.aElemListPart[i].GetAt(j), LT_HCBM_CMD, Key);  // 새로운 멤버에 있는 모든 요소를 pElem에 추가한다.    
	}
	
	if(Key == m_nStartNum)
	{
		T_HCBM_D data;
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
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_HCBM_CMD, Key);
	}

	CMap<T_ELEM_K,T_ELEM_K,T_HCBM_K,T_HCBM_K>  mapElemOld;
	CMap<T_ELEM_K,T_ELEM_K,T_HCBM_K,T_HCBM_K>  mapElemNew;  
	T_HCBM_K KeyHcbmTemp;
	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);   

		// 이전 멤버
		for(int i = 0; i<3; i++) for(int j=0; j<Data.aElemListPart[i].GetSize(); j++)  
			mapElemOld.SetAt(Data.aElemListPart[i].GetAt(j), Key);  		
		// 새로운 멤버
		for(int i = 0; i<3; i++) for(int j=0; j<rData.aElemListPart[i].GetSize(); j++)
			mapElemNew.SetAt(rData.aElemListPart[i].GetAt(j), Key);     
		
		for(int i = 0; i<3; i++) for(int j=0; j<Data.aElemListPart[i].GetSize(); j++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapElemNew.Lookup(Data.aElemListPart[i].GetAt(j), KeyHcbmTemp)) 
				m_elemtohcbm.RemoveKey(Data.aElemListPart[i].GetAt(j));           
		}    		
	} 

	for(int i = 0; i<3 ;i++) for(int j=0; j<rData.aElemListPart[i].GetSize(); j++)
	{
		m_elemtohcbm.SetAt(rData.aElemListPart[i].GetAt(j), Key);  
	}  
}

BOOL CDB_HCBM::Del(T_HCBM_K Key,CDB_ELEM* pElem)
{
	T_HCBM_D Data;
	T_HCBM_K KeyHcbm;
	BOOL bExist=m_hcbm.Lookup(Key, Data);	

	if(bExist)
	{
		BOOL ret=m_hcbm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			int i, j;
			for(i = 0; i<3; i++) for(j=0; j<Data.aElemListPart[i].GetSize(); j++)
			{
				VERIFY(pElem->DelListItem(Data.aElemListPart[i].GetAt(j), LT_HCBM_CMD, Key));
				if(m_elemtohcbm.Lookup(Data.aElemListPart[i].GetAt(j), KeyHcbm))
					m_elemtohcbm.RemoveKey(Data.aElemListPart[i].GetAt(j));
			}      			

			if(Key < m_nStartNum) m_nStartNum = Key;
			if(Key == m_nLastNum)
			{
				T_HCBM_K key;
				T_HCBM_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_HCBM_CMD, Key);      
		}
		return ret;
	}
	return FALSE;
}

BOOL CDB_HCBM::GetHcbmAssigned(T_ELEM_K KeyElem, T_HCBM_K& rKeyHcbm)
{  
//  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//    return m_pDoc->m_pStagCtrl->m_elemtohcbm->Lookup(KeyElem, KeyHcbm);
	return m_elemtohcbm.Lookup(KeyElem, rKeyHcbm);
}

//-------------------------------------------------------------------------
BOOL CDB_HCBM::Get(T_HCBM_K Key,T_HCBM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_hcbm->Lookup(Key, uTemp)) return FALSE;
	}
	return m_hcbm.Lookup(Key,rData);
}

int CDB_HCBM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_hcbm->GetCount();
	}
	return m_hcbm.GetCount();
}

POSITION CDB_HCBM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_hcbm->GetStartPosition();
	}
	return m_hcbm.GetStartPosition();
}

void CDB_HCBM::GetNext(POSITION& rNextPosition,T_HCBM_K& rKey,T_HCBM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_hcbm->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_hcbm.Lookup(rKey, rData);
		return;
	}
	m_hcbm.GetNextAssoc(rNextPosition,rKey,rData);
}
