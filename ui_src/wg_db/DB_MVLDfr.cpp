#include "stdafx.h"
#include "DB_MVLDfr.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVLDfr::CDB_MVLDfr()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvldfr.InitHashTable(HASHSIZEMVLDfr);
}

CDB_MVLDfr::~CDB_MVLDfr()
{
}

void CDB_MVLDfr::Add(T_MVLDfr_K Key,T_MVLDfr_D& rData)
{
	m_mvldfr.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MVLDfr_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MVLDfr::Del(T_MVLDfr_K Key)
{
	BOOL ret=m_mvldfr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MVLDfr_K key;
			T_MVLDfr_D data;
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
BOOL CDB_MVLDfr::Get(T_MVLDfr_K Key,T_MVLDfr_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mvldfr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mvldfr.Lookup(Key,rData);
}

int CDB_MVLDfr::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvldfr->GetCount();
	}
	return m_mvldfr.GetCount();
}

POSITION CDB_MVLDfr::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mvldfr->GetStartPosition();
	}
	return m_mvldfr.GetStartPosition();
}

void CDB_MVLDfr::GetNext(POSITION& rNextPosition,T_MVLDfr_K& rKey,T_MVLDfr_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mvldfr->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mvldfr.Lookup(rKey, rData);
		return;
	}
	m_mvldfr.GetNextAssoc(rNextPosition,rKey,rData);
}
