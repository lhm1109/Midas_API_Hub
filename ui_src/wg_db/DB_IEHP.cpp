#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_IEHP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IEHP::CDB_IEHP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_iehp.InitHashTable(HASHSIZEIEHP);
}

CDB_IEHP::~CDB_IEHP()
{
}

void CDB_IEHP::Add(T_IEHP_K Key,T_IEHP_D& rData)
{
	m_iehp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_IEHP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_IEHP::Del(T_IEHP_K Key)
{
	T_IEHP_D Data;
	BOOL bExist=m_iehp.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_iehp.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_IEHP_K key;
				T_IEHP_D data;
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
	return FALSE;
}

//-------------------------------------------------------------------------
BOOL CDB_IEHP::Get(T_IEHP_K Key,T_IEHP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_iehp.Lookup(Key,rData);
}

const T_IEHP_D* CDB_IEHP::GetConstPtr(T_IEHP_K Key)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return nullptr;

	auto pPair = m_iehp.PLookup(Key);
	if (pPair == nullptr) return nullptr;
	return &pPair->value;
}

int CDB_IEHP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_iehp.GetCount();
}

POSITION CDB_IEHP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_iehp.GetStartPosition();
}

void CDB_IEHP::GetNext(POSITION& rNextPosition,T_IEHP_K& rKey,T_IEHP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_iehp.GetNextAssoc(rNextPosition,rKey,rData);
}
