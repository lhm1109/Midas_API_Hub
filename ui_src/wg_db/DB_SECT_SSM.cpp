#include "stdafx.h"
#include "DB_SECT_SSM.h"
#include "DB_ELEM.h"

#include "DBDoc.h"

CDB_SECT_SSM::CDB_SECT_SSM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sectSsm.InitHashTable(HASHSIZESECTSSM);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SECT_SSM::~CDB_SECT_SSM()
{

}

void CDB_SECT_SSM::Add(T_SECT_SSM_K Key, T_SECT_SSM_D& rData, CDB_ELEM* pElem)
{
	T_SECT_SSM_D Data;
	BOOL bExist = m_sectSsm.Lookup(Key, Data);

	T_ELEM_D tElem;

	if (bExist)
	{
		if (pElem) {
			if (pElem->Get(Data.kElem, tElem)) VERIFY(pElem->DelListItem(Data.kElem, LT_SECT_SSM_CMD, Key));
		}
	}

	m_sectSsm.SetAt(Key, rData);

	if (pElem) {
		pElem->AddListItem(rData.kElem, LT_SECT_SSM_CMD, Key);
	}

	if (Key == m_nStartNum)
	{
		T_SECT_SSM_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sectSsm.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_SECT_SSM::Del(T_SECT_SSM_K Key, CDB_ELEM* pElem)
{
	T_SECT_SSM_D Data;
	BOOL bExist = m_sectSsm.Lookup(Key, Data);
	if (!bExist) return FALSE;

	T_ELEM_D tElem;

	BOOL ret = m_sectSsm.RemoveKey(Key);
	if (ret)
	{
		if (pElem) {
			if (pElem->Get(Data.kElem, tElem)) VERIFY(pElem->DelListItem(Data.kElem, LT_SECT_SSM_CMD, Key));
		}
	}

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SECT_SSM_K key;
		T_SECT_SSM_D data;
		if (m_sectSsm.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sectSsm.GetStartPosition();
			while (pos != NULL)
			{
				m_sectSsm.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SECT_SSM::Get(T_SECT_SSM_K Key, T_SECT_SSM_D& rData)
{
	return m_sectSsm.Lookup(Key, rData);
}

int CDB_SECT_SSM::GetCount()
{
	return m_sectSsm.GetCount();
}

POSITION CDB_SECT_SSM::GetStart()
{
	return m_sectSsm.GetStartPosition();
}

void CDB_SECT_SSM::GetNext(POSITION& rNextPosition, T_SECT_SSM_K& rKey, T_SECT_SSM_D& rData)
{
	m_sectSsm.GetNextAssoc(rNextPosition, rKey, rData);
}

void CDB_SECT_SSM::AddList(T_SECT_SSM_K Key)
{
	DBDataList* pList = new DBDataList;
	m_sectSsmlist.SetAt(Key, pList);
}

BOOL CDB_SECT_SSM::DelList(T_SECT_SSM_K Key)
{
	DBDataList* pList;
	BOOL bExist = m_sectSsmlist.Lookup(Key, pList);
	ASSERT(bExist);
	if (!bExist) return FALSE;
	m_sectSsmlist.RemoveKey(Key);

	delete pList;
	pList = NULL;

	return TRUE;
}

void CDB_SECT_SSM::DelAllList()
{
	T_SECT_SSM_K Key;
	DBDataList* pList;
	POSITION pos = m_sectSsmlist.GetStartPosition();
	while (pos != NULL)
	{
		m_sectSsmlist.GetNextAssoc(pos, Key, pList);
		delete pList;
	}
	m_sectSsmlist.RemoveAll();
}

void CDB_SECT_SSM::AddListItem(T_SECT_SSM_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_sectSsmlist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	POSITION pos = pList->AddTail(ListData);
}

BOOL CDB_SECT_SSM::DelListItem(T_SECT_SSM_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_sectSsmlist.Lookup(Key, pList));

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

BOOL CDB_SECT_SSM::GetList(T_SECT_SSM_K Key, DBDataList*& rpList)
{
	return m_sectSsmlist.Lookup(Key, rpList);
}
