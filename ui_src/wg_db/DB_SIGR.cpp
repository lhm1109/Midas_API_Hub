#include "stdafx.h"
#include "DB_SIGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIGR::CDB_SIGR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sigr.InitHashTable(HASHSIZESIGR);
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_sigrlist.InitHashTable(HASHSIZELISTSIGR);
}

CDB_SIGR::~CDB_SIGR()
{

}

void CDB_SIGR::Add(T_SIGR_K Key, T_SIGR_D& rData)
{
	m_sigr.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_SIGR_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sigr.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_SIGR::Del(T_SIGR_K Key)
{
	T_SIGR_D Data;
	BOOL bExist = m_sigr.Lookup(Key, Data);
	if (!bExist) return FALSE;

	BOOL ret = m_sigr.RemoveKey(Key);
	if (!ret) return FALSE;

	if (Key < m_nStartNum)m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SIGR_K key;
		T_SIGR_D data;
		if (m_sigr.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sigr.GetStartPosition();
			while (pos != NULL)
			{
				m_sigr.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum)m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIGR::Get(T_SIGR_K Key, T_SIGR_D& rData)
{
	return m_sigr.Lookup(Key, rData);
}

int CDB_SIGR::GetCount()
{
	return m_sigr.GetCount();
}

POSITION CDB_SIGR::GetStart()
{
	return m_sigr.GetStartPosition();
}

void CDB_SIGR::GetNext(POSITION& rNextPosition, T_SIGR_K& rKey, T_SIGR_D& rData)
{
	m_sigr.GetNextAssoc(rNextPosition, rKey, rData);
}

void CDB_SIGR::AddList(T_SIGR_K Key)
{
	DBDataList* pList = new DBDataList;
	m_sigrlist.SetAt(Key, pList);
}

BOOL CDB_SIGR::DelList(T_SIGR_K Key)
{
	DBDataList* pList;
	BOOL bExist = m_sigrlist.Lookup(Key, pList);
	ASSERT(bExist);
	if (!bExist)return FALSE;
	m_sigrlist.RemoveKey(Key);

	delete pList;
	pList = NULL;

	return TRUE;
}

void CDB_SIGR::DelAllList()
{
	T_SIGR_K Key;
	DBDataList* pList;
	POSITION pos = m_sigrlist.GetStartPosition();
	while (pos != NULL)
	{
		m_sigrlist.GetNextAssoc(pos, Key, pList);
		delete pList;
	}
	m_sigrlist.RemoveAll();
}

void CDB_SIGR::AddListItem(T_SIGR_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_sigrlist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	POSITION pos = pList->AddTail(ListData);
}

BOOL CDB_SIGR::DelListItem(T_SIGR_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_sigrlist.Lookup(Key, pList));

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

BOOL CDB_SIGR::GetList(T_SIGR_K Key, DBDataList*& rpList)
{
	return m_sigrlist.Lookup(Key, rpList);
}