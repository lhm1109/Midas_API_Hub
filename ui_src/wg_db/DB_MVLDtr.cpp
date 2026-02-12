#include "stdafx.h"
#include "DB_MVLDtr.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVLDtr::CDB_MVLDtr()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvldtr.InitHashTable(HASHSIZEMVLDtr);
}

CDB_MVLDtr::~CDB_MVLDtr()
{
}

void CDB_MVLDtr::Add(T_MVLDtr_K Key,T_MVLDtr_D& rData)
{
	m_mvldtr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MVLDtr_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MVLDtr::Del(T_MVLDtr_K Key)
{
	BOOL ret=m_mvldtr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MVLDtr_K key;
			T_MVLDtr_D data;
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
BOOL CDB_MVLDtr::Get(T_MVLDtr_K Key,T_MVLDtr_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mvldtr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mvldtr.Lookup(Key,rData);
}

int CDB_MVLDtr::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvldtr->GetCount();
	}
	return m_mvldtr.GetCount();
}

POSITION CDB_MVLDtr::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvldtr->GetStartPosition();
	}
	return m_mvldtr.GetStartPosition();
}

void CDB_MVLDtr::GetNext(POSITION& rNextPosition,T_MVLDtr_K& rKey,T_MVLDtr_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mvldtr->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mvldtr.Lookup(rKey, rData);
		return;
	}
	m_mvldtr.GetNextAssoc(rNextPosition,rKey,rData);
}
