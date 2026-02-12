#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_SLBS.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

CDB_SLBS::CDB_SLBS()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_slbs.InitHashTable(HASHSIZESLBS);
	m_slbslist.InitHashTable(HASHSIZELISTSLBS);
}

CDB_SLBS::~CDB_SLBS()
{
	DelAllList();
}

void CDB_SLBS::Add(T_SLBS_K Key,T_SLBS_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{  
	// 중복된 절점 요소 없앤다. (MxT Export시 문제 발생)
	CArrayUtil::RemoveDup(rData.arKeyElem);
	//CArrayUtil::RemoveDup(rData.arKeyNode); // Node는 정렬하면 안된다. 영역지정순서 꼬임

	T_SLBS_D Data;
	BOOL bExist=m_slbs.Lookup(Key, Data);
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNodeOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNodeNew;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemNew;
	T_NODE_K KeyNodeTemp;
	T_ELEM_K KeyElemTemp;
	if(bExist)
	{
		mapNodeNew.InitHashTable(HASHSIZENODE);
		mapNodeOld.InitHashTable(HASHSIZENODE);
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);
		for(int i = 0; i<Data.arKeyNode.GetSize(); i++)mapNodeOld.SetAt(Data.arKeyNode[i], Data.arKeyNode[i]);
		for(int i = 0; i<rData.arKeyNode.GetSize(); i++)mapNodeNew.SetAt(rData.arKeyNode[i], rData.arKeyNode[i]);
		for(int i = 0; i<Data.arKeyElem.GetSize(); i++)mapElemOld.SetAt(Data.arKeyElem[i], Data.arKeyElem[i]);
		for(int i = 0; i<rData.arKeyElem.GetSize(); i++)mapElemNew.SetAt(rData.arKeyElem[i], rData.arKeyElem[i]);

		int nGroup=Data.arKeyNode.GetSize();
		for(int i = 0; i<nGroup; i++)
		{
			if(!mapNodeNew.Lookup(Data.arKeyNode[i], KeyNodeTemp))
				VERIFY(pNode->DelListItem(Data.arKeyNode[i], LT_SLBS_CMD, Key));
		}
		nGroup=Data.arKeyElem.GetSize();
		for(int i = 0; i<nGroup; i++)
		{
			if(!mapElemNew.Lookup(Data.arKeyElem[i], KeyElemTemp))
				VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_SLBS_CMD, Key));
		}
	}
	else
	{
		T_SLBS_K key;
		T_SLBS_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			m_slbs.SetAt(key, data);      
		}
	}
	m_slbs.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_SLBS_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	int nGroup=rData.arKeyNode.GetSize();
	for(int i=0; i<nGroup; i++)
	{
		if(bExist)
		{
			if(!mapNodeOld.Lookup(rData.arKeyNode[i], KeyNodeTemp))
				pNode->AddListItem(rData.arKeyNode[i], LT_SLBS_CMD, Key);
		}
		else
			pNode->AddListItem(rData.arKeyNode[i], LT_SLBS_CMD, Key);
	}
	nGroup=rData.arKeyElem.GetSize();
	for(int i = 0; i<nGroup; i++)
	{
		if(bExist)
		{
			if(!mapElemOld.Lookup(rData.arKeyElem[i], KeyElemTemp))
				pElem->AddListItem(rData.arKeyElem[i], LT_SLBS_CMD, Key);
		}
		else
			pElem->AddListItem(rData.arKeyElem[i], LT_SLBS_CMD, Key);
	}
}

BOOL CDB_SLBS::Del(T_SLBS_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_SLBS_D Data;
	BOOL bExist=m_slbs.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_slbs.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			int nGroup=Data.arKeyNode.GetSize();
			for(int i=0; i<nGroup; i++)VERIFY(pNode->DelListItem(Data.arKeyNode[i], LT_SLBS_CMD, Key));
			nGroup=Data.arKeyElem.GetSize();
			for(int i = 0; i<nGroup; i++)VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_SLBS_CMD, Key));
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_SLBS_K key;
				T_SLBS_D data;
				if(Get(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=GetStart();
					while(pos != NULL)
					{
						GetNext(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}

			T_SLBS_K key;
			T_SLBS_D data;
			POSITION pos = GetStart();
			while (pos)
			{
				GetNext(pos, key, data);
				m_slbs.SetAt(key, data);
			}
		}
		return ret;
	}
	return bExist;
}

void CDB_SLBS::AddItem(T_SLBS_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_SLBS_D Data;
	if(!m_slbs.Lookup(Key, Data))
	{
		ASSERT(FALSE);
		return;
	}

// 기존 데이터에 없는 Element Key만 들어와야 됨
	if(KeyNode != 0)
	{
		Data.arKeyNode.Add(KeyNode);
		m_slbs.SetAt(Key,Data);
		pNode->AddListItem(KeyNode, LT_SLBS_CMD, Key);
	}
	if(KeyElem != 0)
	{
		Data.arKeyElem.Add(KeyElem);
		m_slbs.SetAt(Key,Data);
		pElem->AddListItem(KeyElem, LT_SLBS_CMD, Key);
	}
}

BOOL CDB_SLBS::DelItem(T_SLBS_K Key,T_ELEM_K KeyElem,CDB_ELEM* pElem)
{
	T_SLBS_D Data;
	if(!m_slbs.Lookup(Key, Data))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	if(KeyElem != 0)
	{
		int nGroup=Data.arKeyElem.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.arKeyElem[i] == KeyElem)
			{
				Data.arKeyElem.RemoveAt(i);
				m_slbs.SetAt(Key,Data);
				VERIFY(pElem->DelListItem(KeyElem, LT_SLBS_CMD, Key));
				return TRUE;
			}
		}
	}
	ASSERT(FALSE);
	return FALSE;
}

void CDB_SLBS::AddList(T_SLBS_K Key)
{
	SlbsList* pList = new SlbsList;
	m_slbslist.SetAt(Key, pList);
}

BOOL CDB_SLBS::DelList(T_SLBS_K Key)
{
	SlbsList* pList;
	BOOL bExist=m_slbslist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_slbslist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_SLBS::DelAllList()
{
	T_SLBS_K Key;
	SlbsList* pList;
	POSITION pos=m_slbslist.GetStartPosition();
	while(pos != NULL)
	{
		m_slbslist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_slbslist.RemoveAll();
}

void CDB_SLBS::AddListItem(T_SLBS_K Key, int nCmd, int nKey)
{
	SlbsList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_SLBS::DelListItem(T_SLBS_K Key, int nCmd, int nKey)
{
	SlbsList* pList;
	if(!GetList(Key, pList))return TRUE;

	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			return TRUE;
		}
	}
	return FALSE;
}
