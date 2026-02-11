#include "stdafx.h"
#include "DB_ANEV.h"

#include "DBDoc.h"
#include "StagCtrl.h"


CDB_ANEV::CDB_ANEV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_anev.InitHashTable(HASHSIZEANEV);
	m_nStartNum = 1;
	m_nLastNum  = 0;
}

CDB_ANEV::~CDB_ANEV()
{

}

void CDB_ANEV::Add(T_ANEV_K Key,T_ANEV_D& rData)
{
	m_anev.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_ANEV_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_anev.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_ANEV::Del(T_ANEV_K Key)
{
	BOOL ret=m_anev.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_ANEV_K key;
			T_ANEV_D data;
			if(m_anev.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_anev.GetStartPosition();
				while(pos != NULL)
				{
					m_anev.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}
		
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_ANEV::Get(T_ANEV_K Key,T_ANEV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_anev.Lookup(Key,rData);
}

int CDB_ANEV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_anev.GetCount();
}

POSITION CDB_ANEV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_anev.GetStartPosition();
}

void CDB_ANEV::GetNext(POSITION& rNextPosition,T_ANEV_K& rKey,T_ANEV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_anev.GetNextAssoc(rNextPosition,rKey,rData);
}

