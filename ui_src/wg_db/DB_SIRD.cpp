#include "stdafx.h"
#include "DB_THCR.h"
#include "DB_THIS.h"
#include "DB_SIRD.h"
#include "DB_SIPA.h"
#include "DB_SIGR.h"
#include "DB_SIGR_LOWER.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIRD::CDB_SIRD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sird.InitHashTable(HASHSIZESIRD);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SIRD::~CDB_SIRD()
{

}

void CDB_SIRD::Add(T_SIRD_K Key, T_SIRD_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower, CDB_THIS* pThis)
{
	T_SIRD_D Data;
	BOOL bExist = m_sird.Lookup(Key, Data);

	T_THIS_D dThis;
	T_SIPA_D dSipa;
	T_SIGR_D dSigr;
	T_SIGR_LOWER_D dSigrLower;

	if (bExist)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SIRD_CMD, Key);
		}
		if (pSigr) {
			if (pSigr->Get(Data.kSigr, dSigr)) pSigr->DelListItem(Data.kSigr, LT_SIRD_CMD, Key);
		}
		if (pThis) {
			if (pThis->Get(Data.PLUS_D.kThis, dThis)) pThis->DelListItem(Data.PLUS_D.kThis, LT_SIRD_CMD, Key);
			if (Data.PLUS_D.kThis != Data.MINUS_D.kThis) {
				if (pThis->Get(Data.MINUS_D.kThis, dThis)) pThis->DelListItem(Data.MINUS_D.kThis, LT_SIRD_CMD, Key);
			}
		}
		if (pSigrLower) {
			if (pSigrLower->Get(Data.PLUS_D.kSigrLower, dSigrLower)) pSigr->DelListItem(dSigrLower.kSigr, LT_SIMD_CMD, Key);
			if (Data.PLUS_D.kSigrLower != Data.MINUS_D.kSigrLower) {
				if (pThis->Get(Data.MINUS_D.kSigrLower, dThis)) pThis->DelListItem(Data.MINUS_D.kSigrLower, LT_SIMD_CMD, Key);
			}
		}
	}

	m_sird.SetAt(Key, rData);

	if (pSipa) {
		if (pSipa->Get(rData.kSipa, dSipa)) pSipa->AddListItem(rData.kSipa, LT_SIRD_CMD, Key);
	}
	if (pSigr) {
		if (pSigr->Get(rData.kSigr, dSigr)) pSigr->AddListItem(rData.kSigr, LT_SIRD_CMD, Key);
	}
	if (pThis) {
		if (pThis->Get(rData.PLUS_D.kThis, dThis)) pThis->AddListItem(rData.PLUS_D.kThis, LT_SIRD_CMD, Key);
		if (rData.PLUS_D.kThis != rData.MINUS_D.kThis) {
			if (pThis->Get(rData.MINUS_D.kThis, dThis)) pThis->AddListItem(rData.MINUS_D.kThis, LT_SIRD_CMD, Key);
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
		T_SIRD_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sird.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_SIRD::Del(T_SIRD_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_SIGR_LOWER* pSigrLower, CDB_THIS* pThis)
{
	T_SIRD_D Data;
	BOOL bExist = m_sird.Lookup(Key, Data);
	if (!bExist) return FALSE;

	T_THIS_D dThis;
	T_SIPA_D dSipa;
	T_SIGR_D dSigr;
	T_SIGR_LOWER_D dSigrLower;
	BOOL ret = m_sird.RemoveKey(Key);
	if (ret)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SIRD_CMD, Key);
		}
		if (pSigr) {
			if (pSigr->Get(Data.kSigr, dSigr)) pSigr->DelListItem(Data.kSigr, LT_SIRD_CMD, Key);
		}
		if (pThis) {
			if (pThis->Get(Data.PLUS_D.kThis, dThis)) pThis->DelListItem(Data.PLUS_D.kThis, LT_SIRD_CMD, Key);
			if (Data.PLUS_D.kThis != Data.MINUS_D.kThis) {
				if (pThis->Get(Data.MINUS_D.kThis, dThis)) pThis->DelListItem(Data.MINUS_D.kThis, LT_SIRD_CMD, Key);
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
		T_SIRD_K key;
		T_SIRD_D data;
		if (m_sird.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sird.GetStartPosition();
			while (pos != NULL)
			{
				m_sird.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIRD::Get(T_SIRD_K Key, T_SIRD_D& rData)
{
	return m_sird.Lookup(Key, rData);
}

int CDB_SIRD::GetCount()
{
	return m_sird.GetCount();
}

POSITION CDB_SIRD::GetStart()
{
	return m_sird.GetStartPosition();
}

void CDB_SIRD::GetNext(POSITION& rNextPosition, T_SIRD_K& rKey, T_SIRD_D& rData)
{
	m_sird.GetNextAssoc(rNextPosition, rKey, rData);
}