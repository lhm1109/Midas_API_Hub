#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_NODE.h"
#include "DB_BREQ.h"

#include "DBDoc.h"
#include "StagCtrl.h"
#include"..\wg_base\wg_base_ArrayUtil.h"

CDB_BREQ::CDB_BREQ()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_breq.InitHashTable(HASHSIZEBREQ);
	m_NodeElemToBreq.InitHashTable(HASHSIZEBREQ);
	m_ElemToBreqList.InitHashTable(HASHSIZEBREQ);
	m_NodeToBreqList.InitHashTable(HASHSIZEBREQ);
}

CDB_BREQ::~CDB_BREQ()
{

}

void CDB_BREQ::Add(T_BREQ_K Key,T_BREQ_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_BREQ_D Data;  
	BOOL bExist = m_breq.Lookup(Key, Data);
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
				VERIFY(pElem->DelListItem(Data.aElemList[i], LT_BREQ_CMD, Key));  // pElem에서 없는 요소를 삭제한다.      
		}
	} 
	m_breq.SetAt(Key, rData);  // 이전 멤버가 있어도 새로운 멤버로 덮어쓴다(CMap에 저장).

	if(!bExist)pNode->AddListItem(rData.NodeK, LT_BREQ_CMD, Key);
	for(int i=0; i<rData.aElemList.GetSize(); i++)  // 새로운 멤버의 모든 요소에 대해
	{
		if(bExist) // 새로운 멤버의 키가 이전 멤버의 키와 같으면
		{
			if(!mapOld.Lookup(rData.aElemList[i], KeyElemTemp))  // 새로운 멤버의 요소중 이전 멤버의 요소리스트에 없는 것이 있으면 
				pElem->AddListItem(rData.aElemList[i], LT_BREQ_CMD, Key); // pElem에 없는 요소(새로운 멤버에만 있는 요소)를 추가한다.
		}
		else       // 새로운 멤버의 키가 이전 멤버의 키와 다르면
			pElem->AddListItem(rData.aElemList[i], LT_BREQ_CMD, Key);  // 새로운 멤버에 있는 모든 요소를 pElem에 추가한다.    
	}
	if(Key == m_nStartNum)
	{
		T_BREQ_D data;
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
		if(!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_BREQ_CMD, Key);
	}

	CMap<T_ELEM_K,T_ELEM_K,T_BREQ_K,T_BREQ_K>  mapElemOld;
	CMap<T_ELEM_K,T_ELEM_K,T_BREQ_K,T_BREQ_K>  mapElemNew;  
	T_BREQ_K KeyBreqTemp;
	if(bExist)  // 새로운 멤버의 키가 이전 멤버의 키와 같으면(modify인 경우)
	{
		T_BREQ_K_LIST aBreqK;
		T_UINT2_INDEX_K NodeElemK;
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);   
		for(int i = 0; i<Data.aElemList.GetSize(); i++)  mapElemOld.SetAt(Data.aElemList[i], Key);  // 이전 멤버
		for(int i = 0; i<rData.aElemList.GetSize(); i++) mapElemNew.SetAt(rData.aElemList[i], Key); // 새로운 멤버    
		for(int i = 0; i<Data.aElemList.GetSize(); i++)   // 이전 멤버의 모든 요소에 대해
		{
			if(!mapElemNew.Lookup(Data.aElemList[i], KeyBreqTemp)) 
			{
				NodeElemK.Set(Data.NodeK, Data.aElemList[i]);
				if(!m_NodeElemToBreq.Lookup(NodeElemK, KeyBreqTemp)) continue;
				m_NodeElemToBreq.RemoveKey(NodeElemK);

				if(m_ElemToBreqList.Lookup(Data.aElemList[i], aBreqK))
				{
					if(aBreqK.aBreqK.GetSize() == 1) m_ElemToBreqList.RemoveKey(Data.aElemList[i]); 
					else
					{
						CArray<T_BREQ_K, T_BREQ_K> aOldBreq, aRemainBreq;
						aOldBreq.Add(Key);
						CArrayUtil::SubtractUINT(aBreqK.aBreqK, aOldBreq, aRemainBreq);
						if(aRemainBreq.GetSize() < 1) { ASSERT(0); m_ElemToBreqList.RemoveKey(Data.aElemList[i]); }
						else 
						{
							aBreqK.aBreqK.RemoveAll();
							aBreqK.aBreqK.Copy(aRemainBreq);
							m_ElemToBreqList.SetAt(Data.aElemList[i], aBreqK);
						} 
					}
				}
			}
		}  

		if(Data.NodeK != rData.NodeK)
		{
			if(m_NodeToBreqList.Lookup(Data.NodeK, aBreqK))
			{
				if(aBreqK.aBreqK.GetSize() == 1) m_NodeToBreqList.RemoveKey(Data.NodeK); 
				else
				{
					CArray<T_BREQ_K, T_BREQ_K> aOldBreq, aRemainBreq;
					aOldBreq.Add(Key);
					CArrayUtil::SubtractUINT(aBreqK.aBreqK, aOldBreq, aRemainBreq);
					if(aRemainBreq.GetSize() < 1) { ASSERT(0); m_NodeToBreqList.RemoveKey(Data.NodeK); }
					else 
					{
						aBreqK.aBreqK.RemoveAll();
						aBreqK.aBreqK.Copy(aRemainBreq);
						m_NodeToBreqList.SetAt(Data.NodeK, aBreqK);
					} 
				}
			}
		}
	} 

	T_BREQ_K_LIST aBreqK; 
	if(m_NodeToBreqList.Lookup(rData.NodeK, aBreqK))
	{
		aBreqK.aBreqK.Add(Key);
		CArrayUtil::RemoveDup(aBreqK.aBreqK);
		m_NodeToBreqList.SetAt(rData.NodeK, aBreqK);
	}
	else 
	{
		aBreqK.aBreqK.RemoveAll();
		aBreqK.aBreqK.Add(Key);
		m_NodeToBreqList.SetAt(rData.NodeK, aBreqK);  
	}

	T_UINT2_INDEX_K NodeElemK;
	for(int i = 0; i < rData.aElemList.GetSize(); i++)
	{
		NodeElemK.Set(rData.NodeK, rData.aElemList[i]);
		m_NodeElemToBreq.SetAt(NodeElemK, Key);

		aBreqK.Initialize();
		if(m_ElemToBreqList.Lookup(rData.aElemList[i], aBreqK))
		{
			aBreqK.aBreqK.Add(Key);
			CArrayUtil::RemoveDup(aBreqK.aBreqK);
			m_ElemToBreqList.SetAt(rData.aElemList[i], aBreqK);
		}
		else 
		{
			aBreqK.aBreqK.RemoveAll();
			aBreqK.aBreqK.Add(Key);
			m_ElemToBreqList.SetAt(rData.aElemList[i], aBreqK);  
		}
	}  
}

BOOL CDB_BREQ::Del(T_BREQ_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_BREQ_D Data;
	BOOL bExist=m_breq.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_breq.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			T_BREQ_K_LIST aBreqK;
			T_UINT2_INDEX_K NodeElemK;
			VERIFY(pNode->DelListItem(Data.NodeK, LT_BREQ_CMD, Key));
			for(int i=0; i<Data.aElemList.GetSize(); i++)
			{
				VERIFY(pElem->DelListItem(Data.aElemList[i], LT_BREQ_CMD, Key));

				NodeElemK.Set(Data.NodeK, Data.aElemList[i]);
				m_NodeElemToBreq.RemoveKey(NodeElemK);
				
				if(m_ElemToBreqList.Lookup(Data.aElemList[i], aBreqK))
				{
					if(aBreqK.aBreqK.GetSize()==1) m_ElemToBreqList.RemoveKey(Data.aElemList[i]);
					else
					{
						CArray<T_BREQ_K, T_BREQ_K> aRemoveK; aRemoveK.Add(Key); 
						CArray<T_BREQ_K, T_BREQ_K> aRemainK; aRemainK.RemoveAll();
						CArrayUtil::SubtractUINT(aBreqK.aBreqK, aRemoveK, aRemainK);
						aBreqK.aBreqK.RemoveAll();
						aBreqK.aBreqK.Copy(aRemainK);
						m_ElemToBreqList.SetAt(Data.aElemList[i], aBreqK);
					}
				}
			}  
			
			if(m_NodeToBreqList.Lookup(Data.NodeK, aBreqK))
			{
				if(aBreqK.aBreqK.GetSize()==1) m_NodeToBreqList.RemoveKey(Data.NodeK);
				else
				{
					CArray<T_BREQ_K, T_BREQ_K> aRemoveK; aRemoveK.Add(Key); 
					CArray<T_BREQ_K, T_BREQ_K> aRemainK; aRemainK.RemoveAll();
					CArrayUtil::SubtractUINT(aBreqK.aBreqK, aRemoveK, aRemainK);
					aBreqK.aBreqK.RemoveAll();
					aBreqK.aBreqK.Copy(aRemainK);
					m_NodeToBreqList.SetAt(Data.NodeK, aBreqK);
				}
			}


			if(Key < m_nStartNum) m_nStartNum = Key;
			if(Key == m_nLastNum)
			{
				T_BREQ_K key;
				T_BREQ_D data;
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
				m_pDoc->m_pStagCtrl->DeactiveDesign(LT_BREQ_CMD, Key);      
		}
		return ret;
	}
	return FALSE;
}

BOOL CDB_BREQ::GetBreqAssigned(T_ELEM_K KeyElem, CArray<T_BREQ_K, T_BREQ_K>& aKey)
{  
	BOOL bReturn=FALSE;
	T_BREQ_K_LIST aBreqK;
	if(m_ElemToBreqList.Lookup(KeyElem, aBreqK))
	{
		bReturn = TRUE;
		aKey.Copy(aBreqK.aBreqK);
	}

	return bReturn;
}

BOOL CDB_BREQ::GetBreqAssignedNode(T_NODE_K KeyNode, CArray<T_BREQ_K, T_BREQ_K>& aKey, CArray<T_BREQ_D, T_BREQ_D&>& aData)  // Node 속한 breq의 key와 데이터를 찾아준다.
{
	BOOL bReturn=FALSE;
	T_BREQ_K_LIST aBreqK;
	if(m_NodeToBreqList.Lookup(KeyNode, aBreqK))
	{
		aKey.Copy(aBreqK.aBreqK);
		for (int i=0; i<aKey.GetSize(); i++)
		{
			T_BREQ_D BreqD;
			if(!Get(aKey[i], BreqD)) { ASSERT(0); continue; }
			aData.Add(BreqD);
		}
		ASSERT(aKey.GetSize() == aData.GetSize());
		bReturn = TRUE;
	}

	return bReturn;
}

//-------------------------------------------------------------------------
BOOL CDB_BREQ::Get(T_BREQ_K Key,T_BREQ_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_breq->Lookup(Key, uTemp)) return FALSE;
	}
	return m_breq.Lookup(Key,rData);
}

int CDB_BREQ::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_breq->GetCount();
	}
	return m_breq.GetCount();
}

POSITION CDB_BREQ::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_breq->GetStartPosition();
	}
	return m_breq.GetStartPosition();
}

void CDB_BREQ::GetNext(POSITION& rNextPosition,T_BREQ_K& rKey,T_BREQ_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_breq->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_breq.Lookup(rKey, rData);
		return;
	}
	m_breq.GetNextAssoc(rNextPosition,rKey,rData);
}
