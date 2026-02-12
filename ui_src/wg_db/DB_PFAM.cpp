#include "stdafx.h"
#include "DB_PFAM.h"

CDB_PFAM::CDB_PFAM()
{
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_mapData.InitHashTable(HASHSIZE_PFAM);
	m_mapList.InitHashTable(HASHSIZE_PFAM);
}

CDB_PFAM::~CDB_PFAM()
{
	DelAllList();
}

void CDB_PFAM::Add(T_PFAM_K Key, T_PFAM_D& rData)
{
	m_mapData.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_PFAM_D data;
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

BOOL CDB_PFAM::Del(T_PFAM_K Key)
{
	T_PFAM_D data;
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
			T_PFAM_K key;
			T_PFAM_D data;
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

BOOL CDB_PFAM::Get(T_PFAM_K Key, T_PFAM_D& rData)
{
	return m_mapData.Lookup(Key, rData);
}

int CDB_PFAM::GetCount()
{
	return m_mapData.GetCount();
}

POSITION CDB_PFAM::GetStart()
{
	return m_mapData.GetStartPosition();
}

void CDB_PFAM::GetNext(POSITION& rNextPosition, T_PFAM_K& rKey, T_PFAM_D& rData)
{
	m_mapData.GetNextAssoc(rNextPosition, rKey, rData);
}

void CDB_PFAM::AddList(T_PFAM_K Key)
{
	PfamList* pList = new PfamList;
	m_mapList.SetAt(Key, pList);
}
BOOL CDB_PFAM::DelList(T_PFAM_K Key)
{
	PfamList* pList;
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
void CDB_PFAM::DelAllList(void)
{
	T_PFAM_K Key;
	PfamList* pList;
	POSITION pos = m_mapList.GetStartPosition();
	while (pos != NULL)
	{
		m_mapList.GetNextAssoc(pos, Key, pList);
		delete pList;
		pList = nullptr;
	}
	m_mapList.RemoveAll();
}
BOOL CDB_PFAM::GetList(T_PFAM_K Key, PfamList*& rpList)
{
	return m_mapList.Lookup(Key, rpList);
}
int  CDB_PFAM::GetCountList(void)
{
	return m_mapList.GetCount();
}
void CDB_PFAM::AddListItem(T_PFAM_K Key, int nCmd, int nKey)
{
	PfamList* pList;
	if (!GetList(Key, pList))
	{
		return;
	}

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	pList->AddTail(ListData);
}
BOOL CDB_PFAM::DelListItem(T_PFAM_K Key, int nCmd, int nKey)
{
	PfamList* pList;
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