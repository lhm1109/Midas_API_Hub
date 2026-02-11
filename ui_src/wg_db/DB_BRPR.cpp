#include "stdafx.h"
#include "DB_BRPR.h"

#include "DBDoc.h"
#include "StagCtrl.h"


CDB_BRPR::CDB_BRPR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_brpr.InitHashTable(HASHSIZEBRPR);
	m_nStartNum = 1;
	m_nLastNum  = 0;
}

CDB_BRPR::~CDB_BRPR()
{

}

void CDB_BRPR::Add(T_BRPR_K Key,T_BRPR_D& rData)
{
	m_brpr.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_BRPR_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_brpr.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_BRPR::Del(T_BRPR_K Key)
{
	BOOL ret=m_brpr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_BRPR_K key;
			T_BRPR_D data;
			if(m_brpr.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_brpr.GetStartPosition();
				while(pos != NULL)
				{
					m_brpr.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}
		
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_BRPR::Get(T_BRPR_K Key,T_BRPR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_brpr.Lookup(Key,rData);
}

int CDB_BRPR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_brpr.GetCount();
}

POSITION CDB_BRPR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_brpr.GetStartPosition();
}

void CDB_BRPR::GetNext(POSITION& rNextPosition,T_BRPR_K& rKey,T_BRPR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_brpr.GetNextAssoc(rNextPosition,rKey,rData);
}

