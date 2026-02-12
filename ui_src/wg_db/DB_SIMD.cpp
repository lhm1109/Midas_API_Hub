#include "stdafx.h"
#include "DB_THCR.h"
#include "DB_THIS.h"
#include "DB_SIMD.h"
#include "DB_SIPA.h"
#include "DB_SIGR.h"
#include "DB_SIGR_LOWER.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIMD::CDB_SIMD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_simd.InitHashTable(HASHSIZESIMD);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SIMD::~CDB_SIMD()
{

}

void CDB_SIMD::Add(T_SIMD_K Key, T_SIMD_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower, CDB_THIS* pThis)
{
	T_SIMD_D Data;
	BOOL bExist = m_simd.Lookup(Key, Data);

	T_THIS_D dThis;
	T_SIPA_D dSipa;
	T_SIGR_D dSigr;
	T_SIGR_LOWER_D dSigrLower;

	if (bExist)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SIMD_CMD, Key);
		}
		if (pSigr) {
			if (pSigr->Get(Data.kSigr, dSigr)) pSigr->DelListItem(Data.kSigr, LT_SIMD_CMD, Key);
		}
		if (pThis) {
			if (pThis->Get(Data.PLUS_D.kThis, dThis)) pThis->DelListItem(Data.PLUS_D.kThis, LT_SIMD_CMD, Key);
			if (Data.PLUS_D.kThis != Data.MINUS_D.kThis) {
				if (pThis->Get(Data.MINUS_D.kThis, dThis)) pThis->DelListItem(Data.MINUS_D.kThis, LT_SIMD_CMD, Key);
			}
		}
		if (pSigrLower) {
			if (pSigrLower->Get(Data.PLUS_D.kSigrLower, dSigrLower)) pSigr->DelListItem(dSigrLower.kSigr, LT_SIMD_CMD, Key);
			if (Data.PLUS_D.kSigrLower != Data.MINUS_D.kSigrLower) {
				if (pThis->Get(Data.MINUS_D.kSigrLower, dThis)) pThis->DelListItem(Data.MINUS_D.kSigrLower, LT_SIMD_CMD, Key);
			}
		}
	}

	m_simd.SetAt(Key, rData);

	if (pSipa) {
		if (pSipa->Get(rData.kSipa, dSipa)) pSipa->AddListItem(rData.kSipa, LT_SIMD_CMD, Key);
	}
	if (pSigr) {
		if (pSigr->Get(rData.kSigr, dSigr)) pSigr->AddListItem(rData.kSigr, LT_SIMD_CMD, Key);
	}
	if (pThis) {
		if (pThis->Get(rData.PLUS_D.kThis, dThis)) pThis->AddListItem(rData.PLUS_D.kThis, LT_SIMD_CMD, Key);
		if (rData.PLUS_D.kThis != rData.MINUS_D.kThis) {
			if (pThis->Get(rData.MINUS_D.kThis, dThis)) pThis->AddListItem(rData.MINUS_D.kThis, LT_SIMD_CMD, Key);
		}
	}
	if (pSigrLower) {
		if (pSigrLower->Get(rData.PLUS_D.kSigrLower, dSigrLower)) pSigr->AddListItem(rData.PLUS_D.kSigrLower, LT_SIMD_CMD, Key);
		if (rData.PLUS_D.kSigrLower != rData.MINUS_D.kSigrLower) {
			if (pThis->Get(rData.MINUS_D.kSigrLower, dThis)) pThis->AddListItem(rData.MINUS_D.kSigrLower, LT_SIMD_CMD, Key);
		}
	}

	if (Key == m_nStartNum)
	{
		T_SIMD_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_simd.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_SIMD::Del(T_SIMD_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower, CDB_THIS* pThis)
{
	T_SIMD_D Data;
	BOOL bExist = m_simd.Lookup(Key, Data);
	if (!bExist) return FALSE;

	T_THIS_D dThis;
	T_SIPA_D dSipa;
	T_SIGR_D dSigr;
	T_SIGR_LOWER_D dSigrLower;
	BOOL ret = m_simd.RemoveKey(Key);
	if (ret)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SIMD_CMD, Key);
		}
		if (pSigr) {
			if (pSigr->Get(Data.kSigr, dSigr)) pSigr->DelListItem(Data.kSigr, LT_SIMD_CMD, Key);
		}
		if (pThis) {
			if (pThis->Get(Data.PLUS_D.kThis, dThis)) pThis->DelListItem(Data.PLUS_D.kThis, LT_SIMD_CMD, Key);
			if (Data.PLUS_D.kThis != Data.MINUS_D.kThis) {
				if (pThis->Get(Data.MINUS_D.kThis, dThis)) pThis->DelListItem(Data.MINUS_D.kThis, LT_SIMD_CMD, Key);
			}
		}
		if (pSigrLower) {
			if (pSigrLower->Get(Data.PLUS_D.kSigrLower, dSigrLower)) pSigr->DelListItem(Data.PLUS_D.kSigrLower, LT_SIMD_CMD, Key);
			if (Data.PLUS_D.kSigrLower != Data.MINUS_D.kSigrLower) {
				if (pThis->Get(Data.MINUS_D.kSigrLower, dThis)) pThis->DelListItem(Data.MINUS_D.kSigrLower, LT_SIMD_CMD, Key);
			}
		}
	}

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SIMD_K key;
		T_SIMD_D data;
		if (m_simd.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_simd.GetStartPosition();
			while (pos != NULL)
			{
				m_simd.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIMD::Get(T_SIMD_K Key, T_SIMD_D& rData)
{
	return m_simd.Lookup(Key, rData);
}

int CDB_SIMD::GetCount()
{
	return m_simd.GetCount();
}

POSITION CDB_SIMD::GetStart()
{
	return m_simd.GetStartPosition();
}

void CDB_SIMD::GetNext(POSITION& rNextPosition, T_SIMD_K& rKey, T_SIMD_D& rData)
{
	m_simd.GetNextAssoc(rNextPosition, rKey, rData);
}