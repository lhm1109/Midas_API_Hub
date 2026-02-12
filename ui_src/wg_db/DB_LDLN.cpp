#include "stdafx.h"
#include "DB_LDLN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LDLN::CDB_LDLN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_ldln.InitHashTable(HASHSIZELDLN);
}

CDB_LDLN::~CDB_LDLN()
{
}

void CDB_LDLN::Add(T_LDLN_K Key,T_LDLN_D& rData)
{
	T_LDLN_D Data;
	BOOL bExist=m_ldln.Lookup(Key, Data);
	m_ldln.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_LDLN_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_LDLN::Del(T_LDLN_K Key)
{
	T_LDLN_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_ldln.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_LDLN_K key;
			T_LDLN_D data;
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
BOOL CDB_LDLN::Get(T_LDLN_K Key,T_LDLN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_ldln.Lookup(Key,rData);
}

int CDB_LDLN::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ldln.GetCount();
}

POSITION CDB_LDLN::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_ldln.GetStartPosition();
}

void CDB_LDLN::GetNext(POSITION& rNextPosition,T_LDLN_K& rKey,T_LDLN_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_ldln.GetNextAssoc(rNextPosition,rKey,rData);
}
