#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SCGD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SCGD::CDB_SCGD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_scgd.InitHashTable(HASHSIZESCGD);
	m_elemToShearConnGroup.InitHashTable(HASHSIZEELEM);
}

CDB_SCGD::~CDB_SCGD()
{

}

void CDB_SCGD::Add(T_SCGD_K Key,T_SCGD_D& rData,CDB_ELEM* pElem)
{
	T_SCGD_D Data;  
	BOOL bExist = m_scgd.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K> mapNew;
	T_ELEM_K KeyElemTemp;
	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapNew.InitHashTable(HASHSIZEELEM);
		mapOld.InitHashTable(HASHSIZEELEM);
		for(int i = 0; i<Data.arGroupElemKey.GetSize(); i++)  mapOld.SetAt(Data.arGroupElemKey[i], Data.arGroupElemKey[i]);   // 이전 멤버
		for(int i = 0; i<rData.arGroupElemKey.GetSize(); i++) mapNew.SetAt(rData.arGroupElemKey[i], rData.arGroupElemKey[i]); // 새로운 멤버   
		for(int i = 0; i<Data.arGroupElemKey.GetSize(); i++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapNew.Lookup(Data.arGroupElemKey[i], KeyElemTemp))   // 새로운 멤버의 요소리스트에 이전 멤버의 요소가 없으면 
				VERIFY(pElem->DelListItem(Data.arGroupElemKey[i], LT_SCGD_CMD, Key));  // pElem에서 없는 요소를 삭제한다.      
		}
	} 
	rData.ScgdKey = Key;
	m_scgd.SetAt(Key, rData);  // 이전 멤버가 있어도 새로운 멤버로 덮어쓴다(CMap에 저장).
	for(int i=0; i<rData.arGroupElemKey.GetSize(); i++)  // 새로운 멤버의 모든 요소에 대해
	{
		if(bExist) // 새로운 멤버의 키가 이전 멤버의 키와 같으면
		{
			if(!mapOld.Lookup(rData.arGroupElemKey[i], KeyElemTemp))  // 새로운 멤버의 요소중 이전 멤버의 요소리스트에 없는 것이 있으면 
				pElem->AddListItem(rData.arGroupElemKey[i], LT_SCGD_CMD, Key); // pElem에 없는 요소(새로운 멤버에만 있는 요소)를 추가한다.
		}
		else       // 새로운 멤버의 키가 이전 멤버의 키와 다르면
			pElem->AddListItem(rData.arGroupElemKey[i], LT_SCGD_CMD, Key);  // 새로운 멤버에 있는 모든 요소를 pElem에 추가한다.    
	}
	if(Key == m_nStartNum)
	{
		T_SCGD_D data;
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
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SCGD_CMD, Key);
	}

	CMap<T_ELEM_K,T_ELEM_K,T_SCGD_K,T_SCGD_K>  mapElemOld;
	CMap<T_ELEM_K,T_ELEM_K,T_SCGD_K,T_SCGD_K>  mapElemNew;  
	T_SCGD_K KeyMembTemp;
	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);   
		for(int i = 0; i<Data.arGroupElemKey.GetSize(); i++)  mapElemOld.SetAt(Data.arGroupElemKey[i], Key);  // 이전 멤버
		for(int i = 0; i<rData.arGroupElemKey.GetSize(); i++) mapElemNew.SetAt(rData.arGroupElemKey[i], Key); // 새로운 멤버    
		for(int i = 0; i<Data.arGroupElemKey.GetSize(); i++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapElemNew.Lookup(Data.arGroupElemKey[i], KeyMembTemp)) 
				m_elemToShearConnGroup.RemoveKey(Data.arGroupElemKey[i]);           
		}    
	} 
	for(int i = 0; i < rData.arGroupElemKey.GetSize(); i++)
	{
		m_elemToShearConnGroup.SetAt(rData.arGroupElemKey[i], Key);  
	}  
}

BOOL CDB_SCGD::Del(T_SCGD_K Key,CDB_ELEM* pElem)
{
	T_SCGD_D Data;
	T_SCGD_K KeyMemb;
	BOOL bExist=m_scgd.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_scgd.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			for(int i=0; i<Data.arGroupElemKey.GetSize(); i++)
			{
				VERIFY(pElem->DelListItem(Data.arGroupElemKey[i], LT_SCGD_CMD, Key));
				if(m_elemToShearConnGroup.Lookup(Data.arGroupElemKey[i], KeyMemb))
					m_elemToShearConnGroup.RemoveKey(Data.arGroupElemKey[i]);
			}      
			if(Key < m_nStartNum) m_nStartNum = Key;
			if(Key == m_nLastNum)
			{
				T_SCGD_K key;
				T_SCGD_D data;
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
				m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SCGD_CMD, Key);      
		}
		return ret;
	}
	return FALSE;
}

BOOL CDB_SCGD::GetScgdAssigned(T_ELEM_K KeyElem, T_SCGD_K& rKeyMemb)
{  
	return m_elemToShearConnGroup.Lookup(KeyElem, rKeyMemb);
}

//-------------------------------------------------------------------------
BOOL CDB_SCGD::Get(T_SCGD_K Key,T_SCGD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_scgd->Lookup(Key, uTemp)) return FALSE;
	}
	return m_scgd.Lookup(Key,rData);
}

int CDB_SCGD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_scgd->GetCount();
	}
	return m_scgd.GetCount();
}

POSITION CDB_SCGD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_scgd->GetStartPosition();
	}
	return m_scgd.GetStartPosition();
}

void CDB_SCGD::GetNext(POSITION& rNextPosition,T_SCGD_K& rKey,T_SCGD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_scgd->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_scgd.Lookup(rKey, rData);
		return;
	}
	m_scgd.GetNextAssoc(rNextPosition,rKey,rData);
}
