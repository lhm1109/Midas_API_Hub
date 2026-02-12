#include "stdafx.h"
#include "DB_TDGR.h"

CDB_TDGR::CDB_TDGR()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_tdgr.InitHashTable(HASHSIZETDGR);
	m_tdgrlist.InitHashTable(HASHSIZELISTTDGR);
	AddList(0); // add default group
}

CDB_TDGR::~CDB_TDGR()
{
	DelAllList();
}

void CDB_TDGR::Add(T_TDGR_K Key,T_TDGR_D& rData)
{
	// 새로 추가된 경우 기존 Group의 GroupId가 추가된 GroupId보다 
	// 크거나 같으면 GroupId를 1씩 증가
	T_TDGR_D data;
	BOOL bExist = m_tdgr.Lookup(Key, data);
	if (!bExist)  
	{
		T_TDGR_K key;
		T_TDGR_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nGroupId >= rData.nGroupId) 
			{
				data.nGroupId++;
				m_tdgr.SetAt(key, data);
			}
		}
	}
	m_tdgr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_TDGR::Del(T_TDGR_K Key)
{
	T_TDGR_D Data;
	m_tdgr.Lookup(Key, Data);
	BOOL ret=m_tdgr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_TDGR_K key;
			T_TDGR_D data;
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

		T_TDGR_K key;
		T_TDGR_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nGroupId > Data.nGroupId)
			{
				data.nGroupId--;
				m_tdgr.SetAt(key, data);
			}
		}
	}
	return ret;
}

void CDB_TDGR::AddList(T_TDGR_K Key)
{
	TdgrList* pList = new TdgrList;
	m_tdgrlist.SetAt(Key, pList);
}

BOOL CDB_TDGR::DelList(T_TDGR_K Key)
{
	TdgrList* pList;
	BOOL bExist=m_tdgrlist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_tdgrlist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_TDGR::DelAllList()
{
	T_TDGR_K Key;
	TdgrList* pList;
	POSITION pos=m_tdgrlist.GetStartPosition();
	while(pos != NULL)
	{
		m_tdgrlist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_tdgrlist.RemoveAll();
}

void CDB_TDGR::AddListItem(T_TDGR_K Key, int nCmd, int nKey)
{
	TdgrList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_TDGR::DelListItem(T_TDGR_K Key, int nCmd, int nKey)
{
	TdgrList* pList;
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
