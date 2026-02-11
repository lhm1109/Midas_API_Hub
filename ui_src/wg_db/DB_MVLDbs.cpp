#include "stdafx.h"
#include "DB_MVLDbs.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVLDbs::CDB_MVLDbs()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvldbs.InitHashTable(HASHSIZEMVLDbs);
}

CDB_MVLDbs::~CDB_MVLDbs()
{
}

void CDB_MVLDbs::Add(T_MVLDbs_K Key,T_MVLDbs_D& rData)
{
	m_mvldbs.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MVLDbs_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MVLDbs::Del(T_MVLDbs_K Key)
{
	BOOL ret=m_mvldbs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MVLDbs_K key;
			T_MVLDbs_D data;
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
BOOL CDB_MVLDbs::Get(T_MVLDbs_K Key,T_MVLDbs_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mvldbs->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mvldbs.Lookup(Key,rData);
}

int CDB_MVLDbs::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvldbs->GetCount();
	}
	return m_mvldbs.GetCount();
}

POSITION CDB_MVLDbs::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvldbs->GetStartPosition();
	}
	return m_mvldbs.GetStartPosition();
}

void CDB_MVLDbs::GetNext(POSITION& rNextPosition,T_MVLDbs_K& rKey,T_MVLDbs_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mvldbs->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mvldbs.Lookup(rKey, rData);
		return;
	}
	m_mvldbs.GetNextAssoc(rNextPosition,rKey,rData);
}
