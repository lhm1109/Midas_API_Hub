#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_BLCF.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

CDB_BLCF::CDB_BLCF()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_blcf.InitHashTable(HASHSIZEBLCF);
	m_blcflist.InitHashTable(HASHSIZELISTBLCF);
}

CDB_BLCF::~CDB_BLCF()
{
	DelAllList();
}

void CDB_BLCF::Add(T_BLCF_K Key,T_BLCF_D& rData,CDB_ELEM* pElem)
{
	// 중복된 절점 요소 없앤다. (MxT Export시 문제 발생)
	CArrayUtil::RemoveDup(rData.arKeyElem);
	
	T_BLCF_D Data;
	BOOL bExist=m_blcf.Lookup(Key, Data);
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemOld;
	CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemNew;
	T_ELEM_K KeyElemTemp;
	if(bExist)
	{
		mapElemNew.InitHashTable(HASHSIZEELEM);
		mapElemOld.InitHashTable(HASHSIZEELEM);    
		for(int i = 0; i<Data.arKeyElem.GetSize(); i++)mapElemOld.SetAt(Data.arKeyElem[i], Data.arKeyElem[i]);
		for(int i = 0; i<rData.arKeyElem.GetSize(); i++)mapElemNew.SetAt(rData.arKeyElem[i], rData.arKeyElem[i]);
	
		int nGroup=Data.arKeyElem.GetSize();
		for(int i = 0; i<nGroup; i++)
		{
			if(!mapElemNew.Lookup(Data.arKeyElem[i], KeyElemTemp))
				VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_BLCF_CMD, Key));
		}
	}
	else
	{
		T_BLCF_K key;
		T_BLCF_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nGroupId >= rData.nGroupId) 
			{
				data.nGroupId++;
				m_blcf.SetAt(key, data);
			}
		}
	}
	m_blcf.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_BLCF_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	int nGroup=rData.arKeyElem.GetSize();
	for(int i = 0; i<nGroup; i++)
	{
		if(bExist)
		{
			if(!mapElemOld.Lookup(rData.arKeyElem[i], KeyElemTemp))
				pElem->AddListItem(rData.arKeyElem[i], LT_BLCF_CMD, Key);
		}
		else
			pElem->AddListItem(rData.arKeyElem[i], LT_BLCF_CMD, Key);
	}
}

BOOL CDB_BLCF::Del(T_BLCF_K Key,CDB_ELEM* pElem)
{
	T_BLCF_D Data;
	BOOL bExist=m_blcf.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_blcf.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			int nGroup=Data.arKeyElem.GetSize();
			for(int i = 0; i<nGroup; i++)VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_BLCF_CMD, Key));
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_BLCF_K key;
				T_BLCF_D data;
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

			T_BLCF_K key;
			T_BLCF_D data;
			POSITION pos = GetStart();
			while (pos)
			{
				GetNext(pos, key, data);
				if (data.nGroupId > Data.nGroupId)
				{
					data.nGroupId--;
					m_blcf.SetAt(key, data);
				}
			}
		}
		return ret;
	}
	return bExist;
}


void CDB_BLCF::AddItem(T_BLCF_K Key,T_ELEM_K KeyElem,CDB_ELEM* pElem)
{
	T_BLCF_D Data;
	if(!m_blcf.Lookup(Key, Data))
	{
		ASSERT(FALSE);
		return;
	}
	
	// 기존 데이터에 없는 Element Key만 들어와야 됨  
	if(KeyElem != 0)
	{
		Data.arKeyElem.Add(KeyElem);
		m_blcf.SetAt(Key,Data);
		pElem->AddListItem(KeyElem, LT_BLCF_CMD, Key);
	}
}

BOOL CDB_BLCF::DelItem(T_BLCF_K Key,T_ELEM_K KeyElem,CDB_ELEM* pElem)
{
	T_BLCF_D Data;
	if(!m_blcf.Lookup(Key, Data))
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
				m_blcf.SetAt(Key,Data);
				VERIFY(pElem->DelListItem(KeyElem, LT_BLCF_CMD, Key));
				return TRUE;
			}
		}
	}
	ASSERT(FALSE);
	return FALSE;
}

void CDB_BLCF::AddList(T_BLCF_K Key)
{
	BlcfList* pList = new BlcfList;
	m_blcflist.SetAt(Key, pList);
}

BOOL CDB_BLCF::DelList(T_BLCF_K Key)
{
	BlcfList* pList;
	BOOL bExist=m_blcflist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_blcflist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_BLCF::DelAllList()
{
	T_BLCF_K Key;
	BlcfList* pList;
	POSITION pos=m_blcflist.GetStartPosition();
	while(pos != NULL)
	{
		m_blcflist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_blcflist.RemoveAll();
}

void CDB_BLCF::AddListItem(T_BLCF_K Key, int nCmd, int nKey)
{
	BlcfList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_BLCF::DelListItem(T_BLCF_K Key, int nCmd, int nKey)
{
	BlcfList* pList;
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
