#include "stdafx.h"
#include "DB_SIBD.h"
#include "DB_SIPA.h"
#include "DB_SIGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIBD::CDB_SIBD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sibd.InitHashTable(HASHSIZESIBD);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SIBD::~CDB_SIBD()
{

}

void CDB_SIBD::Add(T_SIBD_K Key, T_SIBD_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr)
{
	T_SIPA_D dSipa;
	T_SIGR_D dSigr;
	T_SIBD_D Data;
	BOOL bExist = m_sibd.Lookup(Key, Data);

	if (bExist)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SIBD_CMD, Key);
		}
		if (pSigr) {
			if (pSigr->Get(Data.kSigr, dSigr)) pSigr->DelListItem(Data.kSigr, LT_SIBD_CMD, Key);
		}
	}

	m_sibd.SetAt(Key, rData);

	if (pSipa) {
		if (pSipa->Get(rData.kSipa, dSipa)) pSipa->AddListItem(rData.kSipa, LT_SIBD_CMD, Key);
	}
	if (pSigr) {
		if (pSigr->Get(rData.kSigr, dSigr)) pSigr->AddListItem(rData.kSigr, LT_SIBD_CMD, Key);

	}
	if (Key == m_nStartNum)
	{
		T_SIBD_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sibd.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_SIBD::Del(T_SIBD_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr)
{
	T_SIPA_D dSipa;
	T_SIGR_D dSigr;
	T_SIBD_D Data;
	BOOL bExist = m_sibd.Lookup(Key, Data);
	if (!bExist) return FALSE;

	BOOL ret = m_sibd.RemoveKey(Key);
	if (ret)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SIBD_CMD, Key);
		}
		if (pSigr) {
			if (pSigr->Get(Data.kSigr, dSigr)) pSigr->DelListItem(Data.kSigr, LT_SIBD_CMD, Key);
		}
	}

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SIBD_K key;
		T_SIBD_D data;
		if (m_sibd.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sibd.GetStartPosition();
			while (pos != NULL)
			{
				m_sibd.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIBD::Get(T_SIBD_K Key, T_SIBD_D& rData)
{
	return m_sibd.Lookup(Key, rData);
}

int CDB_SIBD::GetCount()
{
	return m_sibd.GetCount();
}

POSITION CDB_SIBD::GetStart()
{
	return m_sibd.GetStartPosition();
}

void CDB_SIBD::GetNext(POSITION& rNextPosition, T_SIBD_K& rKey, T_SIBD_D& rData)
{
	m_sibd.GetNextAssoc(rNextPosition, rKey, rData);
}