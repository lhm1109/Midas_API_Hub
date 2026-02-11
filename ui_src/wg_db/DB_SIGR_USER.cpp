#include "stdafx.h"
#include "DB_SIGR_USER.h"
#include "DB_ELEM.h"
#include "DB_SIGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIGR_USER::CDB_SIGR_USER()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sigrUser.InitHashTable(HASHSIZESIGR_USER);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SIGR_USER::~CDB_SIGR_USER()
{

}

void CDB_SIGR_USER::Add(T_SIGR_USER_K Key, T_SIGR_USER_D& rData, CDB_ELEM* pElem, CDB_SIGR* pSigr)
{
	T_SIGR_USER_D Data;
	BOOL bExist = m_sigrUser.Lookup(Key, Data);

	T_ELEM_D dElem;

	if (bExist)
	{
		for (int i = 0; i < Data.aInvestigatedElem.GetSize(); i++)
		{
			if (pElem)
			{
				if (pElem->Get(Data.aInvestigatedElem[i], dElem))
					VERIFY(pElem->DelListItem(Data.aInvestigatedElem[i], LT_SIGR_USER_CMD, Key));
			}
		}
	}

	m_sigrUser.SetAt(Key, rData);

	for (int i = 0; i < rData.aInvestigatedElem.GetSize(); i++)
	{
		if (pElem) {
				pElem->AddListItem(rData.aInvestigatedElem[i], LT_SIGR_USER_CMD, Key);
		}
	}

	if (Key == m_nStartNum)
	{
		T_SIGR_USER_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sigrUser.Lookup(m_nStartNum, data)) break;
		}
	}
	if (Key > m_nLastNum) m_nLastNum = Key;
}

BOOL CDB_SIGR_USER::Del(T_SIGR_USER_K Key, CDB_ELEM* pElem)
{
	T_SIGR_USER_D Data;
	BOOL bExist = m_sigrUser.Lookup(Key, Data);
	if (!bExist) return FALSE;

	T_ELEM_D dElem;
	BOOL ret = m_sigrUser.RemoveKey(Key);
	if (ret)
	{
		for (int i = 0; i < Data.aInvestigatedElem.GetSize(); i++)
		{
			if (pElem)
			{
				if (pElem->Get(Data.aInvestigatedElem[i], dElem))
					VERIFY(pElem->DelListItem(Data.aInvestigatedElem[i], LT_SIGR_USER_CMD, Key));
			}
		}
	}

	if (Key < m_nStartNum)m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SIGR_USER_K key;
		T_SIGR_USER_D data;
		if (m_sigrUser.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sigrUser.GetStartPosition();
			while (pos != NULL)
			{
				m_sigrUser.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum)m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIGR_USER::Get(T_SIGR_USER_K Key, T_SIGR_USER_D& rData)
{
	return m_sigrUser.Lookup(Key, rData);
}

int CDB_SIGR_USER::GetCount()
{
	return m_sigrUser.GetCount();
}

POSITION CDB_SIGR_USER::GetStart()
{
	return m_sigrUser.GetStartPosition();
}

void CDB_SIGR_USER::GetNext(POSITION& rNextPosition, T_SIGR_USER_K& rKey, T_SIGR_USER_D& rData)
{
	m_sigrUser.GetNextAssoc(rNextPosition, rKey, rData);
}