#include "stdafx.h"
#include "DB_CSPA.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CSPA::CDB_CSPA()
{
	m_nStartNum = 1;
	m_nLastNum = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_cspa.InitHashTable(HASHSIZECSPA);
}

CDB_CSPA::~CDB_CSPA()
{
}

void CDB_CSPA::Add(T_CSPA_K Key, T_CSPA_D& rData) // CDB_CSBK* pCsbk
{
	T_CSPA_D Data;
	BOOL bExist = m_cspa.Lookup(Key, Data);

	if (bExist)
	{
		// pCsbk Key 를 지우는 동작 ?
	}
	else
	{
		// pCsbk Key 를 추가하는 동작 ?

		T_CSPA_K key;
		T_CSPA_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			m_cspa.SetAt(key, data);
		}
	}

	m_cspa.SetAt(Key, rData);
	if (Key == m_nStartNum)
	{
		T_CSPA_D data;
		while (TRUE)
		{
			m_nStartNum++;
			if (!Get(m_nStartNum, data))break;
		}
	}
	if (Key > m_nLastNum)m_nLastNum = Key;
}

BOOL CDB_CSPA::Del(T_CSPA_K Key)
{
	T_CSPA_D Data;
	BOOL bExist = m_cspa.Lookup(Key, Data);
	if (bExist)
	{
		BOOL ret = m_cspa.RemoveKey(Key);
		ASSERT(ret);
		if (ret)
		{
			if (Key < m_nStartNum)m_nStartNum = Key;
			if (Key == m_nLastNum)
			{
				T_CSPA_K key;
				T_CSPA_D data;
				if (Get(m_nLastNum - 1, data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum = 0;
					POSITION pos = GetStart();
					while (pos != NULL)
					{
						GetNext(pos, key, data);
						if (key > m_nLastNum)m_nLastNum = key;
					}
				}
			}
		}
		return ret;
	}

	return FALSE;
}


//-------------------------------------------------------------------------
BOOL CDB_CSPA::Get(T_CSPA_K Key, T_CSPA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsMinMaxStag())
		{
			return FALSE;
		}
		else
		{
			//return m_pDoc->m_pStagCtrl->m_cspa.Lookup(Key, rData);
			return m_cspa.Lookup(Key, rData);
		}
	}

	return m_cspa.Lookup(Key, rData);
}

int CDB_CSPA::GetCount()
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	if (m_pDoc->m_pStagCtrl->IsFinalStag() || m_pDoc->m_pStagCtrl->IsMinMaxStag())
	//	{
	//		return 0;
	//	}
	//	else
	//	{
	//		//return m_pDoc->m_pStagCtrl->m_cspa.GetCount();
	//		return m_cspa.GetCount();
	//	}
	//}

	return m_cspa.GetCount();
}

POSITION CDB_CSPA::GetStart()
{
	return m_cspa.GetStartPosition();
}
void CDB_CSPA::GetNext(POSITION& rNextPosition, T_CSPA_K& rKey, T_CSPA_D& rData)
{
	m_cspa.GetNextAssoc(rNextPosition, rKey, rData);
}