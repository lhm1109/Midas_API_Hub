#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_RCPL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_RCPL::CDB_RCPL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rcpl.InitHashTable(HASHSIZERCPL);
	m_elemtorcpl.InitHashTable(HASHSIZEELEM);
	m_nStartNum=1;
	m_nLastNum=0;
}

CDB_RCPL::~CDB_RCPL()
{

}

void CDB_RCPL::Add(T_RCPL_K Key,T_RCPL_D& rData,CDB_ELEM* pElem)
{
	T_RCPL_D Data;  
	BOOL bExist = m_rcpl.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapNew;
	T_ELEM_K KeyElemTemp;
	int j;

	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapNew.InitHashTable(HASHSIZEELEM);
		mapOld.InitHashTable(HASHSIZEELEM);
		
		// 이전 멤버를 찾는다...
		for(j=0; j<Data.arElemK.GetSize(); j++)  
			mapOld.SetAt(Data.arElemK.GetAt(j), Data.arElemK.GetAt(j));   		
		// 새로운 멤버
		for(j=0; j<rData.arElemK.GetSize(); j++)  
			mapNew.SetAt(rData.arElemK.GetAt(j), rData.arElemK.GetAt(j));    
		
		for(j=0; j<Data.arElemK.GetSize(); j++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapNew.Lookup(Data.arElemK.GetAt(j), KeyElemTemp))   // 새로운 멤버의 요소리스트에 이전 멤버의 요소가 없으면 
				VERIFY(pElem->DelListItem(Data.arElemK.GetAt(j), LT_RCPL_CMD, Key));  // pElem에서 없는 요소를 삭제한다.      
		}		
	} 
	m_rcpl.SetAt(Key, rData);  // 이전 멤버가 있어도 새로운 멤버로 덮어쓴다(CMap에 저장).

	// ElemList Part 
	for(j=0; j<rData.arElemK.GetSize(); j++)  // 새로운 멤버의 모든 요소에 대해
	{
		if(bExist) // 새로운 멤버의 키가 이전 멤버의 키와 같으면
		{
			if(!mapOld.Lookup(rData.arElemK.GetAt(j), KeyElemTemp))  // 새로운 멤버의 요소중 이전 멤버의 요소리스트에 없는 것이 있으면 
				pElem->AddListItem(rData.arElemK.GetAt(j), LT_RCPL_CMD, Key); // pElem에 없는 요소(새로운 멤버에만 있는 요소)를 추가한다.
		}
		else       // 새로운 멤버의 키가 이전 멤버의 키와 다르면
			pElem->AddListItem(rData.arElemK.GetAt(j), LT_RCPL_CMD, Key);  // 새로운 멤버에 있는 모든 요소를 pElem에 추가한다.    
	}
	
	if(Key == m_nStartNum)
	{
		T_RCPL_D data;
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
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_RCPL_CMD, Key);
	}

	CMap<T_ELEM_K,T_ELEM_K,T_RCPL_K,T_RCPL_K>  mapElemOld;
	CMap<T_ELEM_K,T_ELEM_K,T_RCPL_K,T_RCPL_K>  mapElemNew;  
	T_RCPL_K KeyRcplTemp;
	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);   

		// 이전 멤버
		for(j=0; j<Data.arElemK.GetSize(); j++)  
			mapElemOld.SetAt(Data.arElemK.GetAt(j), Key);  		
		// 새로운 멤버
		for(j=0; j<rData.arElemK.GetSize(); j++) 
			mapElemNew.SetAt(rData.arElemK.GetAt(j), Key);     
		
		for(j=0; j<Data.arElemK.GetSize(); j++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapElemNew.Lookup(Data.arElemK.GetAt(j), KeyRcplTemp)) 
				m_elemtorcpl.RemoveKey(Data.arElemK.GetAt(j));           
		}    		
	} 

	for(j=0; j<rData.arElemK.GetSize(); j++)
	{
		m_elemtorcpl.SetAt(rData.arElemK.GetAt(j), Key);  
	}  
}

BOOL CDB_RCPL::Del(T_RCPL_K Key,CDB_ELEM* pElem)
{
	T_RCPL_D Data;
	T_RCPL_K KeyRcpl;
	BOOL bExist=m_rcpl.Lookup(Key, Data);	

	if(bExist)
	{
		BOOL ret=m_rcpl.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			int j;
			for(j=0; j<Data.arElemK.GetSize(); j++)
			{
				VERIFY(pElem->DelListItem(Data.arElemK.GetAt(j), LT_RCPL_CMD, Key));
				if(m_elemtorcpl.Lookup(Data.arElemK.GetAt(j), KeyRcpl))
					m_elemtorcpl.RemoveKey(Data.arElemK.GetAt(j));
			}      			

			if(Key < m_nStartNum) m_nStartNum = Key;
			if(Key == m_nLastNum)
			{
				T_RCPL_K key;
				T_RCPL_D data;
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
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_RCPL_CMD, Key);      
		}
		return ret;
	}
	return FALSE;
}

BOOL CDB_RCPL::GetRcplAssigned(T_ELEM_K KeyElem, T_RCPL_K& rKeyRcpl)
{  
//  if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
//    return m_pDoc->m_pStagCtrl->m_elemtorcpl->Lookup(KeyElem, KeyRcpl);
	return m_elemtorcpl.Lookup(KeyElem, rKeyRcpl);
}

//-------------------------------------------------------------------------
BOOL CDB_RCPL::Get(T_RCPL_K Key,T_RCPL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_rcpl->Lookup(Key, uTemp)) return FALSE;
	}
	return m_rcpl.Lookup(Key,rData);
}

int CDB_RCPL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rcpl->GetCount();
	}
	return m_rcpl.GetCount();
}

POSITION CDB_RCPL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_rcpl->GetStartPosition();
	}
	return m_rcpl.GetStartPosition();
}

void CDB_RCPL::GetNext(POSITION& rNextPosition,T_RCPL_K& rKey,T_RCPL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_rcpl->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_rcpl.Lookup(rKey, rData);
		return;
	}
	m_rcpl.GetNextAssoc(rNextPosition,rKey,rData);
}
