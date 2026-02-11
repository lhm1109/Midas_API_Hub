#include "stdafx.h"
#include "DB_SUPL.h"

#include "DBDoc.h"
#include "StagCtrl.h"


CDB_SUPL::CDB_SUPL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_supl.InitHashTable(HASHSIZESUPL);
	m_nStartNum = 1;
	m_nLastNum  = 0;
}

CDB_SUPL::~CDB_SUPL()
{

}

void CDB_SUPL::Add(T_SUPL_K Key,T_SUPL_D& rData)
{
	m_supl.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_SUPL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_supl.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SUPL::Del(T_SUPL_K Key)
{
	BOOL ret=m_supl.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SUPL_K key;
			T_SUPL_D data;
			if(m_supl.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_supl.GetStartPosition();
				while(pos != NULL)
				{
					m_supl.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}
		
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_SUPL::Get(T_SUPL_K Key,T_SUPL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_supl.Lookup(Key,rData);
}

int CDB_SUPL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_supl.GetCount();
}

POSITION CDB_SUPL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_supl.GetStartPosition();
}

void CDB_SUPL::GetNext(POSITION& rNextPosition,T_SUPL_K& rKey,T_SUPL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_supl.GetNextAssoc(rNextPosition,rKey,rData);
}

