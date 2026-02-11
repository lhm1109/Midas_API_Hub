#include "stdafx.h"
#include "DB_SIET.h"
#include "DB_SIPA.h"
#include "DB_SIGR.h"
#include "DB_SIGR_LOWER.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIET::CDB_SIET()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_siet.InitHashTable(HASHSIZESIET);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SIET::~CDB_SIET()
{

}

void CDB_SIET::Add(T_SIET_K Key, T_SIET_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower)
{
	T_SIET_D Data;
	BOOL bExist = m_siet.Lookup(Key, Data);

	T_SIPA_D dSipa;
	T_SIGR_LOWER_D dSigrLower;

	if (bExist)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SIET_CMD, Key);
		}
		if (pSigrLower) {
			if (pSigrLower->Get(Data.kSigrLower, dSigrLower)) pSigr->DelListItem(dSigrLower.kSigr, LT_SIET_CMD, Key);
		}
	}

	m_siet.SetAt(Key, rData);

	if (pSipa) {
		if (pSipa->Get(rData.kSipa, dSipa)) pSipa->AddListItem(rData.kSipa, LT_SIET_CMD, Key);
	}
	if (pSigrLower) {
		if (pSigrLower->Get(rData.kSigrLower, dSigrLower)) pSigr->AddListItem(dSigrLower.kSigr, LT_SIET_CMD, Key);
	}

	if (Key == m_nStartNum)
	{
		T_SIET_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_siet.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_SIET::Del(T_SIET_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower)
{
	T_SIET_D Data;
	BOOL bExist = m_siet.Lookup(Key, Data);
	if (!bExist) return FALSE;

	T_SIPA_D dSipa;
	T_SIGR_LOWER_D dSigrLower;

	BOOL ret = m_siet.RemoveKey(Key);
	if (ret)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SIET_CMD, Key);
		}
		if (pSigrLower) {
			if (pSigrLower->Get(Data.kSigrLower, dSigrLower)) pSigr->DelListItem(dSigrLower.kSigr, LT_SIET_CMD, Key);
		}
	}

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SIET_K key;
		T_SIET_D data;
		if (m_siet.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_siet.GetStartPosition();
			while (pos != NULL)
			{
				m_siet.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIET::Get(T_SIET_K Key, T_SIET_D& rData)
{
	return m_siet.Lookup(Key, rData);
}

int CDB_SIET::GetCount()
{
	return m_siet.GetCount();
}

POSITION CDB_SIET::GetStart()
{
	return m_siet.GetStartPosition();
}

void CDB_SIET::GetNext(POSITION& rNextPosition, T_SIET_K& rKey, T_SIET_D& rData)
{
	m_siet.GetNextAssoc(rNextPosition, rKey, rData);
}