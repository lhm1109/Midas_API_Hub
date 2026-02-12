#include "stdafx.h"
#include "DB_SIGR_UPPER.h"
#include "DB_ELEM.h"
#include "DB_NODE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIGR_UPPER::CDB_SIGR_UPPER()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sigrUpper.InitHashTable(HASHSIZESIGR_UPPER);
	m_nStartNum = 1;
	m_nLastNum = 0;
}

CDB_SIGR_UPPER::~CDB_SIGR_UPPER()
{

}

void CDB_SIGR_UPPER::Add(T_SIGR_UPPER_K Key, T_SIGR_UPPER_D& rData, CDB_ELEM* pElem, CDB_NODE* pNode)
{
	T_SIGR_UPPER_D Data;
	BOOL bExist = m_sigrUpper.Lookup(Key, Data);

	T_ELEM_D dElem;
	T_NODE_D dNode;

	if (bExist)
	{
		for (int i = 0; i < Data.aElements.GetSize(); i++)
		{
			if (pElem) {
				if (pElem->Get(Data.aElements[i], dElem))
					VERIFY(pElem->DelListItem(Data.aElements[i], LT_SIGR_UPPER_CMD, Key));
			}
		}

		if (pNode) {
			if (pNode->Get(Data.kNodeL, dNode)) VERIFY(pNode->DelListItem(Data.kNodeL, LT_SIGR_UPPER_CMD, Key));
			if (pNode->Get(Data.kNodeR, dNode)) VERIFY(pNode->DelListItem(Data.kNodeR, LT_SIGR_UPPER_CMD, Key));
		}
	}

	m_sigrUpper.SetAt(Key, rData);

	for (int i = 0; i < rData.aElements.GetSize(); i++)
	{
		if (pElem) {
			pElem->AddListItem(rData.aElements[i], LT_SIGR_UPPER_CMD, Key);
		}
	}

	if (pNode) {
		pNode->AddListItem(rData.kNodeL, LT_SIGR_UPPER_CMD, Key);
		pNode->AddListItem(rData.kNodeR, LT_SIGR_UPPER_CMD, Key);
	}

	if (Key == m_nStartNum)
	{
		T_SIGR_UPPER_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sigrUpper.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_SIGR_UPPER::Del(T_SIGR_UPPER_K Key, CDB_ELEM* pElem, CDB_NODE* pNode)
{
	T_SIGR_UPPER_D Data;
	BOOL bExist = m_sigrUpper.Lookup(Key, Data);
	if (!bExist) return FALSE;

	T_ELEM_D dElem;
	T_NODE_D dNode;
	BOOL ret = m_sigrUpper.RemoveKey(Key);
	if (ret)
	{
		for (int i = 0; i < Data.aElements.GetSize(); i++)
		{
			if (pElem) {
				if (pElem->Get(Data.aElements[i], dElem))
					VERIFY(pElem->DelListItem(Data.aElements[i], LT_SIGR_UPPER_CMD, Key));
			}
		}

		if (pNode) {
			if (pNode->Get(Data.kNodeL, dNode)) VERIFY(pNode->DelListItem(Data.kNodeL, LT_SIGR_UPPER_CMD, Key));
			if (pNode->Get(Data.kNodeR, dNode)) VERIFY(pNode->DelListItem(Data.kNodeR, LT_SIGR_UPPER_CMD, Key));
		}
	}

	if (Key < m_nStartNum)m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SIGR_UPPER_K key;
		T_SIGR_UPPER_D data;
		if (m_sigrUpper.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sigrUpper.GetStartPosition();
			while (pos != NULL)
			{
				m_sigrUpper.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum)m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_SIGR_UPPER::Get(T_SIGR_UPPER_K Key, T_SIGR_UPPER_D& rData)
{
	return m_sigrUpper.Lookup(Key, rData);
}

int CDB_SIGR_UPPER::GetCount()
{
	return m_sigrUpper.GetCount();
}

POSITION CDB_SIGR_UPPER::GetStart()
{
	return m_sigrUpper.GetStartPosition();
}

void CDB_SIGR_UPPER::GetNext(POSITION& rNextPosition, T_SIGR_UPPER_K& rKey, T_SIGR_UPPER_D& rData)
{
	m_sigrUpper.GetNextAssoc(rNextPosition, rKey, rData);
}