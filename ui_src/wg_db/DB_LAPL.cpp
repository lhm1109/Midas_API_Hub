#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_LAPL.h"
#include "DB_LAGR.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

CDB_LAPL::CDB_LAPL()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_lapl.InitHashTable(HASHSIZELAPL);
	m_lapllist.InitHashTable(HASHSIZELISTLAPL);
}

CDB_LAPL::~CDB_LAPL()
{
	DelAllList();
}

void CDB_LAPL::Add(T_LAPL_K Key,T_LAPL_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem,CDB_LAGR* pLagr)
{
	// 중복된 절점 요소 없앤다. (MxT Export시 문제 발생)
	// 중복절점/요소는 없엔 상태로 들어온다....(순서가 중요하기 때문에)
//   CArrayUtil::RemoveDup(rData.arKeyElem);
//   CArrayUtil::RemoveDup(rData.arKeyNode);
	
	T_LAPL_D Data;
	BOOL bExist=m_lapl.Lookup(Key, Data);
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNodeOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNodeNew;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemNew;
	T_NODE_K KeyNodeTemp;
	T_ELEM_K KeyElemTemp;
	T_LAGR_K KeyLagrTempOld = 0;
	T_LAGR_K KeyLagrTempNew = rData.LagrK;
	if(bExist)
	{
		KeyLagrTempOld = Data.LagrK;
		if(KeyLagrTempOld != KeyLagrTempNew )
		{
			pLagr->DelListItem(KeyLagrTempOld, LT_LAPL_CMD, Key);
		}

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
				VERIFY(pNode->DelListItem(Data.arKeyNode[i], LT_LAPL_CMD, Key));
		}
		nGroup=Data.arKeyElem.GetSize();
		for(int i = 0; i<nGroup; i++)
		{
			if(!mapElemNew.Lookup(Data.arKeyElem[i], KeyElemTemp))
				VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_LAPL_CMD, Key));
		}
	}

	m_lapl.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_LAPL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	//Node Relation
	int nGroup=rData.arKeyNode.GetSize();
	for(int i=0; i<nGroup; i++)
	{
		if(bExist)
		{
			if(!mapNodeOld.Lookup(rData.arKeyNode[i], KeyNodeTemp))
				pNode->AddListItem(rData.arKeyNode[i], LT_LAPL_CMD, Key);
		}
		else
			pNode->AddListItem(rData.arKeyNode[i], LT_LAPL_CMD, Key);
	}
	//Elem Relation
	nGroup=rData.arKeyElem.GetSize();
	for(int i = 0; i<nGroup; i++)
	{
		if(bExist)
		{
			if(!mapElemOld.Lookup(rData.arKeyElem[i], KeyElemTemp))
				pElem->AddListItem(rData.arKeyElem[i], LT_LAPL_CMD, Key);
		}
		else
			pElem->AddListItem(rData.arKeyElem[i], LT_LAPL_CMD, Key);
	}
	//Lagr Relation
	pLagr->AddListItem(KeyLagrTempNew, LT_LAPL_CMD, Key);
	
}

BOOL CDB_LAPL::Del(T_LAPL_K Key,CDB_NODE* pNode,CDB_ELEM* pElem,CDB_LAGR* pLagr)
{
	T_LAPL_D Data;
	BOOL bExist=m_lapl.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_lapl.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Data.LagrK != 0) VERIFY(pLagr->DelListItem(Data.LagrK, LT_LAPL_CMD, Key));
			int nGroup=Data.arKeyNode.GetSize();
			for(int i=0; i<nGroup; i++)VERIFY(pNode->DelListItem(Data.arKeyNode[i], LT_LAPL_CMD, Key));
			nGroup=Data.arKeyElem.GetSize();
			for(int i = 0; i<nGroup; i++)VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_LAPL_CMD, Key));
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_LAPL_K key;
				T_LAPL_D data;
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
		}
		return ret;
	}
	return bExist;
}

void CDB_LAPL::AddItem(T_LAPL_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_LAPL_D Data;
	if(!m_lapl.Lookup(Key, Data))
	{
		ASSERT(FALSE);
		return;
	}

// 기존 데이터에 없는 Element Key만 들어와야 됨
	if(KeyNode != 0)
	{
		Data.arKeyNode.Add(KeyNode);
		m_lapl.SetAt(Key,Data);
		pNode->AddListItem(KeyNode, LT_LAPL_CMD, Key);
	}
	if(KeyElem != 0)
	{
		Data.arKeyElem.Add(KeyElem);
		m_lapl.SetAt(Key,Data);
		pElem->AddListItem(KeyElem, LT_LAPL_CMD, Key);
	}
}

BOOL CDB_LAPL::DelItem(T_LAPL_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_LAPL_D Data;
	if(!m_lapl.Lookup(Key, Data))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if(KeyNode != 0)
	{
		int nGroup=Data.arKeyNode.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.arKeyNode[i] == KeyNode)
			{
				Data.arKeyNode.RemoveAt(i);
				m_lapl.SetAt(Key,Data);
				VERIFY(pNode->DelListItem(KeyNode, LT_LAPL_CMD, Key));
				return TRUE;
			}
		}
	}
	if(KeyElem != 0)
	{
		int nGroup=Data.arKeyElem.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.arKeyElem[i] == KeyElem)
			{
				Data.arKeyElem.RemoveAt(i);
				m_lapl.SetAt(Key,Data);
				VERIFY(pElem->DelListItem(KeyElem, LT_LAPL_CMD, Key));
				return TRUE;
			}
		}
	}
	ASSERT(FALSE);
	return FALSE;
}

void CDB_LAPL::AddList(T_LAPL_K Key)
{
	LaplList* pList = new LaplList;
	m_lapllist.SetAt(Key, pList);
}

BOOL CDB_LAPL::DelList(T_LAPL_K Key)
{
	LaplList* pList;
	BOOL bExist=m_lapllist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_lapllist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_LAPL::DelAllList()
{
	T_LAPL_K Key;
	LaplList* pList;
	POSITION pos=m_lapllist.GetStartPosition();
	while(pos != NULL)
	{
		m_lapllist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_lapllist.RemoveAll();
}

void CDB_LAPL::AddListItem(T_LAPL_K Key, int nCmd, int nKey)
{
	LaplList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_LAPL::DelListItem(T_LAPL_K Key, int nCmd, int nKey)
{
	LaplList* pList;
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
