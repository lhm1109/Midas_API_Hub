#include "stdafx.h"
#include "DB_MVLDch.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVLDch::CDB_MVLDch()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvldch.InitHashTable(HASHSIZEMVLDch);
}

CDB_MVLDch::~CDB_MVLDch()
{
}

void CDB_MVLDch::Add(T_MVLDch_K Key,T_MVLDch_D& rData)
{
	m_mvldch.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MVLDch_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MVLDch::Del(T_MVLDch_K Key)
{
	BOOL ret=m_mvldch.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MVLDch_K key;
			T_MVLDch_D data;
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
BOOL CDB_MVLDch::Get(T_MVLDch_K Key,T_MVLDch_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mvldch->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mvldch.Lookup(Key,rData);
}

int CDB_MVLDch::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvldch->GetCount();
	}
	return m_mvldch.GetCount();
}

POSITION CDB_MVLDch::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvldch->GetStartPosition();
	}
	return m_mvldch.GetStartPosition();
}

void CDB_MVLDch::GetNext(POSITION& rNextPosition,T_MVLDch_K& rKey,T_MVLDch_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mvldch->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mvldch.Lookup(rKey, rData);
		return;
	}
	m_mvldch.GetNextAssoc(rNextPosition,rKey,rData);
}
