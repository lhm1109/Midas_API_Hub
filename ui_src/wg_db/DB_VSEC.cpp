#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_NODE.h"
#include "DB_VSEC.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

CDB_VSEC::CDB_VSEC()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_vsec.InitHashTable(HASHSIZEVSEC);
	m_vseclist.InitHashTable(HASHSIZELISTVSEC);
}

CDB_VSEC::~CDB_VSEC()
{
	DelAllList();
}

void CDB_VSEC::Add(T_VSEC_K Key,T_VSEC_D& rData, CDB_ELEM* pElem, CDB_NODE* pNode)
{
	// 중복된 절점 요소 없앤다. (MxT Export시 문제 발생)
	CArrayUtil::RemoveDup(rData.arKeyElem);
	CArrayUtil::RemoveDup(rData.arKeyNode);
	
	T_VSEC_D Data;
	BOOL bExist=m_vsec.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemNew;
	T_ELEM_K KeyElemTemp;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNodeOld;
	CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNodeNew;
	T_NODE_K KeyNodeTemp;
	if(bExist)
	{
		// elem
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);
		for(int i = 0; i<Data.arKeyElem.GetSize(); i++)mapElemOld.SetAt(Data.arKeyElem[i], Data.arKeyElem[i]);
		for(int i = 0; i<rData.arKeyElem.GetSize(); i++)mapElemNew.SetAt(rData.arKeyElem[i], rData.arKeyElem[i]);

		int nGroup=Data.arKeyElem.GetSize();
		for(int i = 0; i<nGroup; i++)
		{
			if(!mapElemNew.Lookup(Data.arKeyElem[i], KeyElemTemp))
				VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_VSEC_CMD, Key));
		}

		// node
		mapNodeNew.InitHashTable(HASHSIZENODE);
		mapNodeOld.InitHashTable(HASHSIZENODE);
		//int i;
		for(int i = 0; i<Data.arKeyNode.GetSize(); i++)mapNodeOld.SetAt(Data.arKeyNode[i], Data.arKeyNode[i]);
		for(int i = 0; i<rData.arKeyNode.GetSize(); i++)mapNodeNew.SetAt(rData.arKeyNode[i], rData.arKeyNode[i]);

		/*int*/ nGroup=Data.arKeyNode.GetSize();
		for(int i = 0; i<nGroup; i++)
		{
			if(!mapNodeNew.Lookup(Data.arKeyNode[i], KeyNodeTemp))
				VERIFY(pNode->DelListItem(Data.arKeyNode[i], LT_VSEC_CMD, Key));
		}
	}
	
	m_vsec.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_VSEC_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	// elem
	int nGroupElem=rData.arKeyElem.GetSize();
	for(int i=0; i<nGroupElem; i++)
	{
		if(bExist)
		{
			if(!mapElemOld.Lookup(rData.arKeyElem[i], KeyElemTemp))  pElem->AddListItem(rData.arKeyElem[i], LT_VSEC_CMD, Key);
		}
		else
		{
			pElem->AddListItem(rData.arKeyElem[i], LT_VSEC_CMD, Key);
		}
	}

	// node
	int nGroupNode=rData.arKeyNode.GetSize();
	for(int i=0; i<nGroupNode; i++)
	{
		if(bExist)
		{
			if(!mapNodeOld.Lookup(rData.arKeyNode[i], KeyNodeTemp))  pNode->AddListItem(rData.arKeyNode[i], LT_VSEC_CMD, Key);
		}
		else
		{
			pNode->AddListItem(rData.arKeyNode[i], LT_VSEC_CMD, Key);
		}
	}

}

BOOL CDB_VSEC::Del(T_VSEC_K Key, CDB_ELEM* pElem, CDB_NODE* pNode)
{
	T_VSEC_D Data;
	BOOL bExist=m_vsec.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_vsec.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			int nGroup=0;
			// elem
			nGroup=Data.arKeyElem.GetSize();
			for(int i=0; i<nGroup; i++)VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_VSEC_CMD, Key));
			// node
			nGroup=Data.arKeyNode.GetSize();
			for(int i=0; i<nGroup; i++)VERIFY(pNode->DelListItem(Data.arKeyNode[i], LT_VSEC_CMD, Key));

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_VSEC_K key;
				T_VSEC_D data;
				if(Get(m_nLastNum-1,data)) { m_nLastNum--; }
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

void CDB_VSEC::AddItem(T_VSEC_K Key, T_NODE_K KeyNode, T_ELEM_K KeyElem, CDB_NODE* pNode, CDB_ELEM* pElem)
{
	T_VSEC_D Data;
	if(!m_vsec.Lookup(Key, Data))  { ASSERT(FALSE);  return; }

	// 기존 데이터에 없는 Elem Key만 들어와야 됨
	if(KeyElem != 0)
	{
		Data.arKeyElem.Add(KeyElem);
		m_vsec.SetAt(Key,Data);
		pElem->AddListItem(KeyElem, LT_VSEC_CMD, Key);
	}

	// 기존 데이터에 없는 Node Key만 들어와야 됨
	if(KeyNode != 0)
	{
		Data.arKeyNode.Add(KeyNode);
		m_vsec.SetAt(Key,Data);
		pNode->AddListItem(KeyNode, LT_VSEC_CMD, Key);
	}
}

BOOL CDB_VSEC::DelItem(T_VSEC_K Key, T_NODE_K KeyNode, T_ELEM_K KeyElem, CDB_NODE* pNode, CDB_ELEM* pElem)
{
	T_VSEC_D Data;
	if(!m_vsec.Lookup(Key, Data)) { ASSERT(FALSE);  return FALSE; }

	// elem
	if(KeyElem != 0)
	{
		int nGroup=Data.arKeyElem.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.arKeyElem[i] == KeyElem)
			{
				Data.arKeyElem.RemoveAt(i);
				m_vsec.SetAt(Key,Data);
				VERIFY(pElem->DelListItem(KeyElem, LT_VSEC_CMD, Key));
				return TRUE;
			}
		}
	}

	// node
	if(KeyNode != 0)
	{
		int nGroup=Data.arKeyNode.GetSize();
		for(int i=0; i<nGroup; i++)
		{
			if(Data.arKeyNode[i] == KeyNode)
			{
				Data.arKeyNode.RemoveAt(i);
				m_vsec.SetAt(Key,Data);
				VERIFY(pNode->DelListItem(KeyNode, LT_VSEC_CMD, Key));
				return TRUE;
			}
		}
	}

	ASSERT(FALSE);
	return FALSE;
}

void CDB_VSEC::AddList(T_VSEC_K Key)
{
	VsecList* pList = new VsecList;
	m_vseclist.SetAt(Key, pList);
}

BOOL CDB_VSEC::DelList(T_VSEC_K Key)
{
	VsecList* pList;
	BOOL bExist=m_vseclist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_vseclist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_VSEC::DelAllList()
{
	T_VSEC_K Key;
	VsecList* pList;
	POSITION pos=m_vseclist.GetStartPosition();
	while(pos != NULL)
	{
		m_vseclist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_vseclist.RemoveAll();
}

void CDB_VSEC::AddListItem(T_VSEC_K Key, int nCmd, int nKey)
{
	VsecList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_VSEC::DelListItem(T_VSEC_K Key, int nCmd, int nKey)
{
	VsecList* pList;
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
