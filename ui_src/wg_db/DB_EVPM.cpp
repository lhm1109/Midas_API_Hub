#include "stdafx.h"
#include "DB_EVPM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EVPM::CDB_EVPM()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nStartNum=1;
	m_nLastNum=0;
	m_evpm.InitHashTable(HASHSIZEEVPM);	
}

CDB_EVPM::~CDB_EVPM()
{

}

void CDB_EVPM::Add(T_EVPM_K Key,T_EVPM_D& rData)
{
	m_evpm.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_EVPM_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum, data))break;
		}
	}
	if(Key > m_nLastNum) m_nLastNum=Key;
}

BOOL CDB_EVPM::Del(T_EVPM_K Key)
{
	BOOL ret=m_evpm.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum) m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_EVPM_K key;
			T_EVPM_D data;
			if(Get(m_nLastNum-1, data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key, data);
					if(key > m_nLastNum) m_nLastNum=key;
				}
			}
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_EVPM::Get(T_EVPM_K Key,T_EVPM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_evpm.Lookup(Key,rData);
}

int CDB_EVPM::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_evpm.GetCount();
}

POSITION CDB_EVPM::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_evpm.GetStartPosition();
}

void CDB_EVPM::GetNext(POSITION& rNextPosition,T_EVPM_K& rKey,T_EVPM_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_evpm.GetNextAssoc(rNextPosition,rKey,rData);
}
