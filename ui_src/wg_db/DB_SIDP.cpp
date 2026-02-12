#include "stdafx.h"
#include "DB_SIDP.h"
#include "DB_SIPA.h"
#include "DB_SIGR.h"
#include "DB_SIGR_LOWER.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIDP::CDB_SIDP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sidp.InitHashTable(HASHSIZESIDP);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SIDP::~CDB_SIDP()
{

}

void CDB_SIDP::Add(T_SIDP_K Key, T_SIDP_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower)
{
	T_SIDP_D Data;
	BOOL bExist = m_sidp.Lookup(Key, Data);

	T_SIPA_D dSipa;
	T_SIGR_LOWER_D dSigrLower;

	if (bExist)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SIDP_CMD, Key);
		}
		if (pSigrLower) {
			if (pSigrLower->Get(Data.kSigrLower, dSigrLower)) pSigr->DelListItem(dSigrLower.kSigr, LT_SIDP_CMD, Key);
		}
	}

	m_sidp.SetAt(Key, rData);

	if (pSipa) {
		if (pSipa->Get(rData.kSipa, dSipa)) pSipa->AddListItem(rData.kSipa, LT_SIDP_CMD, Key);
	}
	if (pSigrLower) {
		if (pSigrLower->Get(rData.kSigrLower, dSigrLower)) pSigr->AddListItem(dSigrLower.kSigr, LT_SIDP_CMD, Key);
	}

	if (Key == m_nStartNum)
	{
		T_SIDP_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sidp.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_SIDP::Del(T_SIDP_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower)
{
	T_SIDP_D Data;
	BOOL bExist = m_sidp.Lookup(Key, Data);
	if (!bExist) return FALSE;

	T_SIPA_D dSipa;
	T_SIGR_LOWER_D dSigrLower;

	BOOL ret = m_sidp.RemoveKey(Key);
	if (ret)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SIDP_CMD, Key);
		}
		if (pSigrLower) {
			if (pSigrLower->Get(Data.kSigrLower, dSigrLower)) pSigr->DelListItem(dSigrLower.kSigr, LT_SIDP_CMD, Key);
		}
	}

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SIDP_K key;
		T_SIDP_D data;
		if (m_sidp.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sidp.GetStartPosition();
			while (pos != NULL)
			{
				m_sidp.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIDP::Get(T_SIDP_K Key, T_SIDP_D& rData)
{
	return m_sidp.Lookup(Key, rData);
}

int CDB_SIDP::GetCount()
{
	return m_sidp.GetCount();
}

POSITION CDB_SIDP::GetStart()
{
	return m_sidp.GetStartPosition();
}

void CDB_SIDP::GetNext(POSITION& rNextPosition, T_SIDP_K& rKey, T_SIDP_D& rData)
{
	m_sidp.GetNextAssoc(rNextPosition, rKey, rData);
}