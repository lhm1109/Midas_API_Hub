#include "stdafx.h"
#include "DB_WVEP.h"

CDB_WVEP::CDB_WVEP()
{
	m_nStartNum = 1;
	m_nLastNum  = 0;
	m_wvep.InitHashTable(HASHSIZEWVEP);
	m_wveplist.InitHashTable(HASHSIZELISTWVEP);
}

CDB_WVEP::~CDB_WVEP()
{
	DelAllList();
}

void CDB_WVEP::Add(T_WVEP_K Key,T_WVEP_D& rData)
{
	T_WVEP_D data;
	m_wvep.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))
				break;
		}
	}
	if(Key > m_nLastNum)
		m_nLastNum=Key;
}

BOOL CDB_WVEP::Del(T_WVEP_K Key)
{
	BOOL ret=m_wvep.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_WVEP_K key;
			T_WVEP_D data;
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

void CDB_WVEP::AddList(T_WVEP_K Key)
{
	WvepList* pList = new WvepList;
	m_wveplist.SetAt(Key, pList);
}

BOOL CDB_WVEP::DelList(T_WVEP_K Key)
{
	WvepList* pList;
	BOOL bExist=m_wveplist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_wveplist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_WVEP::DelAllList()
{
	T_WVEP_K Key;
	WvepList* pList;
	POSITION pos=m_wveplist.GetStartPosition();
	while(pos != NULL)
	{
		m_wveplist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_wveplist.RemoveAll();
}

void CDB_WVEP::AddListItem(T_WVEP_K Key, int nCmd, int nKey)
{
	LaplList* pList;
	if(!GetList(Key, pList))return;
	
	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_WVEP::DelListItem(T_WVEP_K Key, int nCmd, int nKey)
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

BOOL CDB_WVEP::GetList(T_WVEP_K Key,WvepList*& rpList)
{
	return m_wveplist.Lookup(Key,rpList);
}
