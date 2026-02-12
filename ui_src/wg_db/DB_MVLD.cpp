#include "stdafx.h"
#include "DB_MVLD.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVLD::CDB_MVLD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvld.InitHashTable(HASHSIZEMVLD);
}

CDB_MVLD::~CDB_MVLD()
{
}

void CDB_MVLD::Add(T_MVLD_K Key,T_MVLD_D& rData)
{
	m_mvld.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MVLD_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MVLD::Del(T_MVLD_K Key)
{
	BOOL ret=m_mvld.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MVLD_K key;
			T_MVLD_D data;
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
BOOL CDB_MVLD::Get(T_MVLD_K Key,T_MVLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mvld->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mvld.Lookup(Key,rData);
}

int CDB_MVLD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvld->GetCount();
	}
	return m_mvld.GetCount();
}

POSITION CDB_MVLD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvld->GetStartPosition();
	}
	return m_mvld.GetStartPosition();
}

void CDB_MVLD::GetNext(POSITION& rNextPosition,T_MVLD_K& rKey,T_MVLD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mvld->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mvld.Lookup(rKey, rData);
		return;
	}
	m_mvld.GetNextAssoc(rNextPosition,rKey,rData);
}
