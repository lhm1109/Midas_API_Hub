#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_PHGT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PHGT::CDB_PHGT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_nLastSeq=0;
	m_phgt.InitHashTable(HASHSIZEPHGT);
}

CDB_PHGT::~CDB_PHGT()
{
}

void CDB_PHGT::Add(T_PHGT_K Key,T_PHGT_D& rData)
{
	m_phgt.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_PHGT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_PHGT::Del(T_PHGT_K Key)
{
	T_PHGT_D Data;
	BOOL bExist=m_phgt.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_phgt.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_PHGT_K key;
				T_PHGT_D data;
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
BOOL CDB_PHGT::Get(T_PHGT_K Key,T_PHGT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_phgt.Lookup(Key,rData);
}

const T_PHGT_D* CDB_PHGT::GetConstPtr(T_PHGT_K Key)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return nullptr;

	auto pPair = m_phgt.PLookup(Key);
	if(pPair == nullptr) return nullptr;
	return &pPair->value;
}

int CDB_PHGT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_phgt.GetCount();
}

POSITION CDB_PHGT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_phgt.GetStartPosition();
}

void CDB_PHGT::GetNext(POSITION& rNextPosition,T_PHGT_K& rKey,T_PHGT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_phgt.GetNextAssoc(rNextPosition,rKey,rData);
}
