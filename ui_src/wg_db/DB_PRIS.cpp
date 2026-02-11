#include "stdafx.h"
#include "DB_PRIS.h"

#include "DBDoc.h"

CDB_PRIS::CDB_PRIS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pris.InitHashTable(HASHSIZEPRIS);
 	m_prislist.InitHashTable(HASHSIZELISTPRIS);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_PRIS::~CDB_PRIS()
{
	DelAllList();
}

void CDB_PRIS::Add(T_PRIS_K Key, T_PRIS_D& rData)
{
	T_PRIS_D Data;
	BOOL bExist = m_pris.Lookup(Key, Data);

	m_pris.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_PRIS_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_pris.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_PRIS::Del(T_PRIS_K Key)
{
	T_PRIS_D Data;
	BOOL bExist = m_pris.Lookup(Key, Data);
	if (bExist)
	{
		BOOL ret = m_pris.RemoveKey(Key);
 		ASSERT(ret);

		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_PRIS_K key;
			T_PRIS_D data;
			if (m_pris.Lookup(m_nLastNum - 1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum = 0;
				POSITION pos = m_pris.GetStartPosition();
				while (pos != NULL)
				{
					m_pris.GetNextAssoc(pos, key, data);
					if (key > m_nLastNum)m_nLastNum = key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

BOOL CDB_PRIS::Get(T_PRIS_K Key, T_PRIS_D& rData)
{
	return m_pris.Lookup(Key, rData);
}


int CDB_PRIS::GetCount()
{
	return m_pris.GetCount();
}

POSITION CDB_PRIS::GetStart()
{
	return m_pris.GetStartPosition();
}

void CDB_PRIS::GetNext(POSITION& rNextPosition, T_PRIS_K& rKey, T_PRIS_D& rData)
{
	m_pris.GetNextAssoc(rNextPosition, rKey, rData);
}

////////////////////////////////////////////////////////////////
void CDB_PRIS::AddList(T_PRIS_K Key)
{
	DBDataList* pList = new DBDataList;
	m_prislist.SetAt(Key, pList);
}

BOOL CDB_PRIS::DelList(T_PRIS_K Key)
{
	DBDataList* pList;
	BOOL bExist = m_prislist.Lookup(Key, pList);
	ASSERT(bExist);
	if (!bExist)return FALSE;
	m_prislist.RemoveKey(Key);

	delete pList;
	return TRUE;
}

void CDB_PRIS::DelAllList()
{
	T_PRIS_K Key;
	DBDataList* pList;
	POSITION pos = m_prislist.GetStartPosition();
	while (pos != NULL)
	{
		m_prislist.GetNextAssoc(pos, Key, pList);
		delete pList;
	}
	m_prislist.RemoveAll();
}

void CDB_PRIS::AddListItem(T_PRIS_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_prislist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	POSITION pos = pList->AddTail(ListData);
}

BOOL CDB_PRIS::DelListItem(T_PRIS_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_prislist.Lookup(Key, pList));

	BOOL bDeleted = FALSE;

	T_LIST_DATA ListData;
	POSITION pos = pList->GetTailPosition(), posPrv;
	while (pos != NULL)
	{
		posPrv = pos;
		ListData = pList->GetPrev(pos);
		if (ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			bDeleted = TRUE;
			break;
		}
	}
	if (!bDeleted) return FALSE;

	return TRUE;
}

BOOL CDB_PRIS::GetList(T_PRIS_K Key, DBDataList*& rpList)
{
	return m_prislist.Lookup(Key, rpList);
}
