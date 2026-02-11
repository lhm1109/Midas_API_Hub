#include "stdafx.h"
#include "DB_DYLA.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DYLA::CDB_DYLA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_dyla.InitHashTable(HASHSIZEDYLA);
}

CDB_DYLA::~CDB_DYLA()
{
}

void CDB_DYLA::Add(T_DYLA_K Key,T_DYLA_D& rData)
{
	m_dyla.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_DYLA_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_DYLA::Del(T_DYLA_K Key)
{
	BOOL ret=m_dyla.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_DYLA_K key;
			T_DYLA_D data;
			if(Get(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DYLA::Get(T_DYLA_K Key,T_DYLA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dyla.Lookup(Key,rData);
}

int CDB_DYLA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dyla.GetCount();
}

POSITION CDB_DYLA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dyla.GetStartPosition();
}

void CDB_DYLA::GetNext(POSITION& rNextPosition,T_DYLA_K& rKey,T_DYLA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_dyla.GetNextAssoc(rNextPosition,rKey,rData);
}
