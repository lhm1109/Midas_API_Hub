#include "stdafx.h"
#include "DB_PFSL.h"

CDB_PFSL::CDB_PFSL()
{
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_mapData.InitHashTable(HASHSIZE_PFSL);
	m_mapList.InitHashTable(HASHSIZE_PFSL);
}

CDB_PFSL::~CDB_PFSL()
{
	DelAllList();
}

void CDB_PFSL::Add(T_PFSL_K Key, T_PFSL_D& rData)
{
	m_mapData.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_PFSL_D data;
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

BOOL CDB_PFSL::Del(T_PFSL_K Key)
{
	T_PFSL_D data;
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
			T_PFSL_K key;
			T_PFSL_D data;
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

BOOL CDB_PFSL::Get(T_PFSL_K Key, T_PFSL_D& rData)
{
	return m_mapData.Lookup(Key, rData);
}

int CDB_PFSL::GetCount()
{
	return m_mapData.GetCount();
}

POSITION CDB_PFSL::GetStart()
{
	return m_mapData.GetStartPosition();
}

void CDB_PFSL::GetNext(POSITION& rNextPosition, T_PFSL_K& rKey, T_PFSL_D& rData)
{
	m_mapData.GetNextAssoc(rNextPosition, rKey, rData);
}

void CDB_PFSL::AddList(T_PFSL_K Key)
{
	PfslList* pList = new PfslList;
	m_mapList.SetAt(Key, pList);
}
BOOL CDB_PFSL::DelList(T_PFSL_K Key)
{
	PfslList* pList;
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
void CDB_PFSL::DelAllList(void)
{
	T_PFSL_K Key;
	PfslList* pList;
	POSITION pos = m_mapList.GetStartPosition();
	while (pos != NULL)
	{
		m_mapList.GetNextAssoc(pos, Key, pList);
		delete pList;
		pList = nullptr;
	}
	m_mapList.RemoveAll();
}
BOOL CDB_PFSL::GetList(T_PFSL_K Key, PfslList*& rpList)
{
	return m_mapList.Lookup(Key, rpList);
}
int  CDB_PFSL::GetCountList(void)
{ 
	return m_mapList.GetCount();
}
void CDB_PFSL::AddListItem(T_PFSL_K Key, int nCmd, int nKey)
{
	PfslList* pList;
	if (!GetList(Key, pList))
	{
		return;
	}

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	pList->AddTail(ListData);
}
BOOL CDB_PFSL::DelListItem(T_PFSL_K Key, int nCmd, int nKey)
{
	PfslList* pList;
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