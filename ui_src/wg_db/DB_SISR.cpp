#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SISR.h"
#include "DB_SIPA.h"
#include "DB_SIGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SISR::CDB_SISR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sisr.InitHashTable(HASHSIZESISR);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SISR::~CDB_SISR()
{

}

void CDB_SISR::Add(T_SISR_K Key, T_SISR_D& rData, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_ELEM* pElem)
{
	T_SISR_D Data;
	BOOL bExist = m_sisr.Lookup(Key, Data);

	T_SIPA_D dSipa;
	T_SIGR_D dSigr;
	T_ELEM_D dElem;

	if (bExist)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SISR_CMD, Key);
		}
		if (pSigr) {
			if (pSigr->Get(Data.kSigr, dSigr)) pSigr->DelListItem(Data.kSigr, LT_SISR_CMD, Key);
		}
		for (int i = 0; i < Data.AUTO_D.aCcElements.GetSize(); i++)
		{
			if (pElem) {
				if (pElem->Get(Data.AUTO_D.aCcElements[i], dElem))
					VERIFY(pElem->DelListItem(Data.AUTO_D.aCcElements[i], LT_SISR_CMD, Key));
			}
		}
	}

	m_sisr.SetAt(Key, rData);

	if (pSipa) {
		if (pSipa->Get(rData.kSipa, dSipa)) pSipa->AddListItem(rData.kSipa, LT_SISR_CMD, Key);
	}
	if (pSigr) {
		if (pSigr->Get(rData.kSigr, dSigr)) pSigr->AddListItem(rData.kSigr, LT_SISR_CMD, Key);
	}
	for (int i = 0; i < rData.AUTO_D.aCcElements.GetSize(); i++)
	{
		if (pElem)
		{
			if (pElem->Get(rData.AUTO_D.aCcElements[i], dElem))
				pElem->AddListItem(rData.AUTO_D.aCcElements[i], LT_SISR_CMD, Key);
		}
	}

	if (Key == m_nStartNum)
	{
		T_SISR_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sisr.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_SISR::Del(T_SISR_K Key, CDB_SIPA* pSipa, CDB_SIGR* pSigr, CDB_ELEM* pElem)
{
	T_SISR_D Data;
	BOOL bExist = m_sisr.Lookup(Key, Data);
	if (!bExist) return FALSE;

	T_SIPA_D dSipa;
	T_SIGR_D dSigr;
	T_ELEM_D dElem;
	BOOL ret = m_sisr.RemoveKey(Key);
	if (ret)
	{
		if (pSipa) {
			if (pSipa->Get(Data.kSipa, dSipa)) pSipa->DelListItem(Data.kSipa, LT_SISR_CMD, Key);
		}
		if (pSigr) {
			if (pSigr->Get(Data.kSigr, dSigr)) pSigr->DelListItem(Data.kSigr, LT_SISR_CMD, Key);
		}
		for (int i = 0; i < Data.AUTO_D.aCcElements.GetSize(); i++)
		{
			if (pElem)
			{
				if (pElem->Get(Data.AUTO_D.aCcElements[i], dElem))
					pElem->DelListItem(Data.AUTO_D.aCcElements[i], LT_SISR_CMD, Key);
			}
		}
	}

	if (Key < m_nStartNum) m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SISR_K key;
		T_SISR_D data;
		if (m_sisr.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sisr.GetStartPosition();
			while (pos != NULL)
			{
				m_sisr.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum) m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SISR::Get(T_SISR_K Key, T_SISR_D& rData)
{
	return m_sisr.Lookup(Key, rData);
}

int CDB_SISR::GetCount()
{
	return m_sisr.GetCount();
}

POSITION CDB_SISR::GetStart()
{
	return m_sisr.GetStartPosition();
}

void CDB_SISR::GetNext(POSITION& rNextPosition, T_SISR_K& rKey, T_SISR_D& rData)
{
	m_sisr.GetNextAssoc(rNextPosition, rKey, rData);
}

void CDB_SISR::AddList(T_SISR_K Key)
{
	DBDataList* pList = new DBDataList;
	m_sisrlist.SetAt(Key, pList);
}

BOOL CDB_SISR::DelList(T_SISR_K Key)
{
	DBDataList* pList;
	BOOL bExist = m_sisrlist.Lookup(Key, pList);
	ASSERT(bExist);
	if (!bExist)return FALSE;
	m_sisrlist.RemoveKey(Key);

	delete pList;
	pList = NULL;

	return TRUE;
}

void CDB_SISR::DelAllList()
{
	T_SISR_K Key;
	DBDataList* pList;
	POSITION pos = m_sisrlist.GetStartPosition();
	while (pos != NULL)
	{
		m_sisrlist.GetNextAssoc(pos, Key, pList);
		delete pList;
	}
	m_sisrlist.RemoveAll();
}

void CDB_SISR::AddListItem(T_SISR_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_sisrlist.Lookup(Key, pList));

	T_LIST_DATA ListData;
	ListData.nCmd = nCmd;
	ListData.nKey = nKey;
	POSITION pos = pList->AddTail(ListData);
}

BOOL CDB_SISR::DelListItem(T_SISR_K Key, int nCmd, int nKey)
{
	DBDataList* pList;
	VERIFY(m_sisrlist.Lookup(Key, pList));

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

BOOL CDB_SISR::GetList(T_SISR_K Key, DBDataList*& rpList)
{
	return m_sisrlist.Lookup(Key, rpList);
}
