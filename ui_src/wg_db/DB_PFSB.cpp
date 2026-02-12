#include "stdafx.h"
#include "DB_PFSB.h"

CDB_PFSB::CDB_PFSB()
{
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_mapData.InitHashTable(HASHSIZE_PFSB);
	m_mapList.InitHashTable(HASHSIZE_PFSB);
}

CDB_PFSB::~CDB_PFSB()
{
	DelAllList();
}

void CDB_PFSB::Add(T_PFSB_K Key, T_PFSB_D& rData)
{
	m_mapData.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_PFSB_D data;
		while (true)
		{
			m_nStartNum++;
			if (!Get(m_nStartNum, data))
			{
				break;
			}
		}
	}
	if (Key > m_nLastNum)
	{
		m_nLastNum = Key;
	}
}

BOOL CDB_PFSB::Del(T_PFSB_K Key)
{
	T_PFSB_D data;
	m_mapData.Lookup(Key, data);

	bool ret = m_mapData.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		if (Key < m_nStartNum)
		{
			m_nStartNum = Key;
		}
		if (Key == m_nLastNum)
		{
			T_PFSB_K key;
			T_PFSB_D data;
			if (Get(m_nLastNum - 1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum = 0;
				POSITION pos = GetStart();
				while (pos != NULL)
				{
					GetNext(pos, key, data);
					if (key > m_nLastNum)
					{
						m_nLastNum = key;
					}
				}
			}
		}
	}
	return ret;
}

BOOL CDB_PFSB::Get(T_PFSB_K Key, T_PFSB_D& rData)
{
	return m_mapData.Lookup(Key, rData);
}

int CDB_PFSB::GetCount()
{
	return m_mapData.GetCount();
}

POSITION CDB_PFSB::GetStart()
{
	return m_mapData.GetStartPosition();
}

void CDB_PFSB::GetNext(POSITION& rNextPosition, T_PFSB_K& rKey, T_PFSB_D& rData)
{
	m_mapData.GetNextAssoc(rNextPosition, rKey, rData);
}

void CDB_PFSB::AddList(T_PFSB_K Key)
{
	PfsbList* pList = new PfsbList;
	m_mapList.SetAt(Key, pList);
}
BOOL CDB_PFSB::DelList(T_PFSB_K Key)
{
	PfsbList* pList;
	BOOL bExist = m_mapList.Lookup(Key, pList);
	ASSERT(bExist);
	if (!bExist)
	{

		return FALSE;
	}
	m_mapList.RemoveKey(Key);
	delete pList;
	return TRUE;
}
void CDB_PFSB::DelAllList(void)
{
	T_PFSB_K Key;
	PfsbList* pList;
	POSITION pos = m_mapList.GetStartPosition();
	while (pos != NULL)
	{
		m_mapList.GetNextAssoc(pos, Key, pList);
		delete pList;
		pList = nullptr;
	}
	m_mapList.RemoveAll();
}
BOOL CDB_PFSB::GetList(T_PFSB_K Key, PfsbList*& rpList)
{
	return m_mapList.Lookup(Key, rpList);
}
int  CDB_PFSB::GetCountList(void)
{
	return m_mapList.GetCount();
}
void CDB_PFSB::AddListItem(T_PFSB_K Key, int nCmd, int nKey)
{
	PfsbList* pList;
	if (!GetList(Key, pList))
	{
		return;
	}

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	pList->AddTail(ListData);
}
BOOL CDB_PFSB::DelListItem(T_PFSB_K Key, int nCmd, int nKey)
{
	PfsbList* pList;
	if (!GetList(Key, pList))
	{
		return TRUE;
	}
	if (pList->GetCount() <= 0)
	{
		return TRUE;
	}

	T_LIST_DATA ListData;
	POSITION pos = pList->GetHeadPosition(), posPrv;
	while (pos != NULL)
	{
		posPrv = pos;
		ListData = pList->GetNext(pos);
		if (ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			return TRUE;
		}
	}
	return FALSE;
}