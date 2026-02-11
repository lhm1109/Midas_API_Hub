#include "stdafx.h"
#include "DB_SIPA.h"
#include "DB_THIS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIPA::CDB_SIPA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sipa.InitHashTable(HASHSIZESIPA);
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_sipalist.InitHashTable(HASHSIZELISTSIPA);
}

CDB_SIPA::~CDB_SIPA()
{

}

void CDB_SIPA::Add(T_SIPA_K Key, T_SIPA_D& rData, CDB_THIS* pThis)
{
	T_SIPA_D Data;
	BOOL bExist = m_sipa.Lookup(Key, Data);

	T_THIS_D ThisData;

	if (bExist)
	{
		for (int i = 0; i < Data.aSeisThisK.GetSize(); i++)
		{
			if (pThis)
			{
				if (pThis->Get(Data.aSeisThisK[i], ThisData))
					pThis->DelListItem(Data.aSeisThisK[i], LT_SIPA_CMD, Key);
			}
		}
	}

	m_sipa.SetAt(Key, rData);

	for (int i = 0; i < rData.aSeisThisK.GetSize(); i++)
	{
		if (pThis)
		{
			if (pThis->Get(rData.aSeisThisK[i], ThisData))
				pThis->AddListItem(rData.aSeisThisK[i], LT_SIPA_CMD, Key);
		}
	}

	if (Key == m_nStartNum)
	{
		T_SIPA_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sipa.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_SIPA::Del(T_SIPA_K Key, CDB_THIS* pThis)
{
	T_SIPA_D Data;
	BOOL bExist = m_sipa.Lookup(Key, Data);
	if (bExist)
	{
		T_THIS_D ThisData;
		BOOL ret = m_sipa.RemoveKey(Key);
		ASSERT(ret);
		if (ret)
		{
			for (int i = 0; i < Data.aSeisThisK.GetSize(); i++)
			{
				if (pThis)
				{
					if (pThis->Get(Data.aSeisThisK[i], ThisData))
						VERIFY(pThis->DelListItem(Data.aSeisThisK[i], LT_SIPA_CMD, Key));
				}
			}
		}

		if (Key < m_nStartNum)m_nStartNum = Key;
		if (Key == m_nLastNum)
		{
			T_SIPA_K key;
			T_SIPA_D data;
			if (m_sipa.Lookup(m_nLastNum - 1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum = 0;
				POSITION pos = m_sipa.GetStartPosition();
				while (pos != NULL)
				{
					m_sipa.GetNextAssoc(pos, key, data);
					if (key > m_nLastNum)m_nLastNum = key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIPA::Get(T_SIPA_K Key, T_SIPA_D& rData)
{
	return m_sipa.Lookup(Key, rData);
}

int CDB_SIPA::GetCount()
{
	return m_sipa.GetCount();
}

POSITION CDB_SIPA::GetStart()
{
	return m_sipa.GetStartPosition();
}

void CDB_SIPA::GetNext(POSITION& rNextPosition, T_SIPA_K& rKey, T_SIPA_D& rData)
{
	m_sipa.GetNextAssoc(rNextPosition, rKey, rData);
}

void CDB_SIPA::AddList(T_SIPA_K Key)
{
	DBDataList* pList = new DBDataList;
	m_sipalist.SetAt(Key, pList);
}

BOOL CDB_SIPA::DelList(T_SIPA_K Key)
{
	DBDataList* pList;
	BOOL bExist = m_sipalist.Lookup(Key, pList);
	ASSERT(bExist);
	if (!bExist)return FALSE;
	m_sipalist.RemoveKey(Key);

	delete pList;
	pList = NULL;

	return TRUE;
}

void CDB_SIPA::DelAllList()
{
	T_SIPA_K Key;
	DBDataList* pList;
	POSITION pos = m_sipalist.GetStartPosition();
	while (pos != NULL)
	{
		m_sipalist.GetNextAssoc(pos, Key, pList);
		delete pList;
	}
	m_sipalist.RemoveAll();
}

void CDB_SIPA::AddListItem(T_SIPA_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_sipalist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	POSITION pos = pList->AddTail(ListData);
}

BOOL CDB_SIPA::DelListItem(T_SIPA_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_sipalist.Lookup(Key, pList));

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

BOOL CDB_SIPA::GetList(T_SIPA_K Key, DBDataList*& rpList)
{
	return m_sipalist.Lookup(Key, rpList);
}