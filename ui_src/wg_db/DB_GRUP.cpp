#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_GRUP.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

CDB_GRUP::CDB_GRUP()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_grup.InitHashTable(HASHSIZEGRUP);
	m_gruplist.InitHashTable(HASHSIZELISTGRUP);
	m_grupcolor.InitHashTable(HASHSIZELISTGRUP);

	// Key == 0 : Duplicate Desgination
	T_COLOR_D DataColor;
	DataColor.Initialize();
	DataColor.WireFrame = RGB(58, 81, 89);
	DataColor.HiddenFill = RGB(79, 137, 156);
	DataColor.HiddenEdge = RGB(58, 81, 89);
	DataColor.bBlending = FALSE;
	DataColor.dBlendingFactor = 0.25;
	m_grupcolor.SetAt(0, DataColor);
}

CDB_GRUP::~CDB_GRUP()
{
	DelAllList();
}

void CDB_GRUP::Add(T_GRUP_K Key,T_GRUP_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	// 중복된 절점 요소 없앤다. (MxT Export시 문제 발생)
	CArrayUtil::RemoveDup(rData.arKeyElem);
	CArrayUtil::RemoveDup(rData.arKeyNode);
	
	T_GRUP_D Data;
	BOOL bExist=m_grup.Lookup(Key, Data);
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
				VERIFY(pNode->DelListItem(Data.arKeyNode[i], LT_GRUP_CMD, Key));
		}
		nGroup=Data.arKeyElem.GetSize();
		for(int i = 0; i<nGroup; i++)
		{
			if(!mapElemNew.Lookup(Data.arKeyElem[i], KeyElemTemp))
				VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_GRUP_CMD, Key));
		}
	}
	else
	{
		T_GRUP_K key;
		T_GRUP_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nGroupId >= rData.nGroupId) 
			{
				data.nGroupId++;
				m_grup.SetAt(key, data);
			}
		}
		T_COLOR_D DataColor;
		DataColor.Initialize();
		DataColor.WireFrame = RGB(99, 139, 152);
		DataColor.HiddenFill = RGB(146, 187, 200);
		DataColor.HiddenEdge = RGB(99, 139, 152);
		DataColor.bBlending = FALSE;
		DataColor.dBlendingFactor = 0.25;
		m_grupcolor.SetAt(Key, DataColor);
	}
	m_grup.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_GRUP_D data;
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
				pNode->AddListItem(rData.arKeyNode[i], LT_GRUP_CMD, Key);
		}
		else
			pNode->AddListItem(rData.arKeyNode[i], LT_GRUP_CMD, Key);
	}
	nGroup=rData.arKeyElem.GetSize();
	for(int i = 0; i<nGroup; i++)
	{
		if(bExist)
		{
			if(!mapElemOld.Lookup(rData.arKeyElem[i], KeyElemTemp))
				pElem->AddListItem(rData.arKeyElem[i], LT_GRUP_CMD, Key);
		}
		else
			pElem->AddListItem(rData.arKeyElem[i], LT_GRUP_CMD, Key);
	}
}

BOOL CDB_GRUP::Del(T_GRUP_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_GRUP_D Data;
	BOOL bExist=m_grup.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_grup.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(m_grupcolor.RemoveKey(Key));
			int nGroup=Data.arKeyNode.GetSize();
			for(int i=0; i<nGroup; i++)VERIFY(pNode->DelListItem(Data.arKeyNode[i], LT_GRUP_CMD, Key));
			nGroup=Data.arKeyElem.GetSize();
			for(int i = 0; i<nGroup; i++)VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_GRUP_CMD, Key));
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_GRUP_K key;
				T_GRUP_D data;
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

			T_GRUP_K key;
			T_GRUP_D data;
			POSITION pos = GetStart();
			while (pos)
			{
				GetNext(pos, key, data);
				if (data.nGroupId > Data.nGroupId)
				{
					data.nGroupId--;
					m_grup.SetAt(key, data);
				}
			}
		}
		return ret;
	}
	return bExist;
}

void CDB_GRUP::AddItem(T_GRUP_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_GRUP_D Data;
	if(!m_grup.Lookup(Key, Data))
	{
		ASSERT(FALSE);
		return;
	}

// 기존 데이터에 없는 Element Key만 들어와야 됨
	if(KeyNode != 0)
	{
		Data.arKeyNode.Add(KeyNode);
		m_grup.SetAt(Key,Data);
		pNode->AddListItem(KeyNode, LT_GRUP_CMD, Key);
	}
	if(KeyElem != 0)
	{
		Data.arKeyElem.Add(KeyElem);
		m_grup.SetAt(Key,Data);
		pElem->AddListItem(KeyElem, LT_GRUP_CMD, Key);
	}
}

BOOL CDB_GRUP::DelItem(T_GRUP_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem)
{
	T_GRUP_D Data;
	if(!m_grup.Lookup(Key, Data))
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
				m_grup.SetAt(Key,Data);
				VERIFY(pNode->DelListItem(KeyNode, LT_GRUP_CMD, Key));
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
				m_grup.SetAt(Key,Data);
				VERIFY(pElem->DelListItem(KeyElem, LT_GRUP_CMD, Key));
				return TRUE;
			}
		}
	}
	ASSERT(FALSE);
	return FALSE;
}

void CDB_GRUP::AddList(T_GRUP_K Key)
{
	GrupList* pList = new GrupList;
	m_gruplist.SetAt(Key, pList);
}

BOOL CDB_GRUP::DelList(T_GRUP_K Key)
{
	GrupList* pList;
	BOOL bExist=m_gruplist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_gruplist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_GRUP::DelAllList()
{
	T_GRUP_K Key;
	GrupList* pList;
	POSITION pos=m_gruplist.GetStartPosition();
	while(pos != NULL)
	{
		m_gruplist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_gruplist.RemoveAll();
}

void CDB_GRUP::AddListItem(T_GRUP_K Key, int nCmd, int nKey)
{
	GrupList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_GRUP::DelListItem(T_GRUP_K Key, int nCmd, int nKey)
{
	GrupList* pList;
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
