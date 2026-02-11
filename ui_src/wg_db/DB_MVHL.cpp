#include "stdafx.h"
#include "DB_MVHL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVHL::CDB_MVHL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvhl.InitHashTable(HASHSIZEMVHL);
}

CDB_MVHL::~CDB_MVHL()
{
}

void CDB_MVHL::Add(T_MVHL_K Key,T_MVHL_D& rData)
{
	m_mvhl.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MVHL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MVHL::Del(T_MVHL_K Key)
{
	BOOL ret=m_mvhl.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MVHL_K key;
			T_MVHL_D data;
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
BOOL CDB_MVHL::Get(T_MVHL_K Key,T_MVHL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvhl.Lookup(Key,rData);
}

int CDB_MVHL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvhl.GetCount();
}

POSITION CDB_MVHL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvhl.GetStartPosition();
}

void CDB_MVHL::GetNext(POSITION& rNextPosition,T_MVHL_K& rKey,T_MVHL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_mvhl.GetNextAssoc(rNextPosition,rKey,rData);
}
