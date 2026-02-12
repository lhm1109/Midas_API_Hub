#include "stdafx.h"
#include "DB_MVLDjp.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MVLDjp::CDB_MVLDjp()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mvldjp.InitHashTable(HASHSIZEMVLDjp);
}

CDB_MVLDjp::~CDB_MVLDjp()
{
}

void CDB_MVLDjp::Add(T_MVLDjp_K Key,T_MVLDjp_D& rData)
{
	m_mvldjp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MVLDjp_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MVLDjp::Del(T_MVLDjp_K Key)
{
	BOOL ret=m_mvldjp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MVLDjp_K key;
			T_MVLDjp_D data;
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
BOOL CDB_MVLDjp::Get(T_MVLDjp_K Key,T_MVLDjp_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mvldjp.Lookup(Key,rData);
}

int CDB_MVLDjp::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvldjp.GetCount();
}

POSITION CDB_MVLDjp::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mvldjp.GetStartPosition();
}

void CDB_MVLDjp::GetNext(POSITION& rNextPosition,T_MVLDjp_K& rKey,T_MVLDjp_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_mvldjp.GetNextAssoc(rNextPosition,rKey,rData);
}
