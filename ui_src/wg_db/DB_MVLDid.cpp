#include "stdafx.h"
#include "DB_MVLDid.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVLDid::CDB_MVLDid()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvldid.InitHashTable(HASHSIZEMVLDid);
}

CDB_MVLDid::~CDB_MVLDid()
{
}

void CDB_MVLDid::Add(T_MVLDid_K Key,T_MVLDid_D& rData)
{
	m_mvldid.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MVLDid_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MVLDid::Del(T_MVLDid_K Key)
{
	BOOL ret=m_mvldid.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MVLDid_K key;
			T_MVLDid_D data;
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
BOOL CDB_MVLDid::Get(T_MVLDid_K Key,T_MVLDid_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mvldid->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mvldid.Lookup(Key,rData);
}

int CDB_MVLDid::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvldid->GetCount();
	}
	return m_mvldid.GetCount();
}

POSITION CDB_MVLDid::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvldid->GetStartPosition();
	}
	return m_mvldid.GetStartPosition();
}

void CDB_MVLDid::GetNext(POSITION& rNextPosition,T_MVLDid_K& rKey,T_MVLDid_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mvldid->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mvldid.Lookup(rKey, rData);
		return;
	}
	m_mvldid.GetNextAssoc(rNextPosition,rKey,rData);
}
