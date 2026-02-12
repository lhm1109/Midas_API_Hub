#include "stdafx.h"
#include "DB_SIDC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SIDC::CDB_SIDC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_sidc.InitHashTable(HASHSIZESIDC);
}

CDB_SIDC::~CDB_SIDC()
{

}

void CDB_SIDC::Add(T_SIDC_K Key, T_SIDC_D& rData)
{
	m_sidc.SetAt(Key, rData);

	if (Key == m_nStartNum)
	{
		T_SIDC_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!m_sidc.Lookup(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_SIDC::Del(T_SIDC_K Key)
{
	T_SIDC_D Data;
	BOOL bExist = m_sidc.Lookup(Key, Data);
	if (!bExist) return FALSE;

	BOOL ret = m_sidc.RemoveKey(Key);
	if (!ret) return FALSE;

	if (Key < m_nStartNum)m_nStartNum = Key;
	if (Key == m_nLastNum)
	{
		T_SIDC_K key;
		T_SIDC_D data;
		if (m_sidc.Lookup(m_nLastNum - 1, data))
		{
			m_nLastNum--;
		}
		else
		{
			m_nLastNum = 0;
			POSITION pos = m_sidc.GetStartPosition();
			while (pos != NULL)
			{
				m_sidc.GetNextAssoc(pos, key, data);
				if (key > m_nLastNum)m_nLastNum = key;
			}
		}
	}

	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SIDC::Get(T_SIDC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_sidc.Lookup(1,rData);
}

BOOL CDB_SIDC::Get(T_SIDC_K Key,T_SIDC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_sidc.Lookup(Key,rData);
}

int CDB_SIDC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_sidc.GetCount();
}

POSITION CDB_SIDC::GetStart()
{
	return m_sidc.GetStartPosition();
}

void CDB_SIDC::GetNext(POSITION& rNextPosition, T_SIDC_K& rKey, T_SIDC_D& rData)
{
	m_sidc.GetNextAssoc(rNextPosition, rKey, rData);
}