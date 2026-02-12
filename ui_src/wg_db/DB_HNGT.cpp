#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_HNGT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HNGT::CDB_HNGT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_hngt.InitHashTable(HASHSIZEHNGT);
}

CDB_HNGT::~CDB_HNGT()
{
}

void CDB_HNGT::Add(T_HNGT_K Key,T_HNGT_D& rData)
{
	m_hngt.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_HNGT_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_HNGT::Del(T_HNGT_K Key)
{
	T_HNGT_D Data;
	BOOL bExist=m_hngt.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_hngt.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_HNGT_K key;
				T_HNGT_D data;
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
BOOL CDB_HNGT::Get(T_HNGT_K Key,T_HNGT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_hngt.Lookup(Key,rData);
}

int CDB_HNGT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hngt.GetCount();
}

POSITION CDB_HNGT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hngt.GetStartPosition();
}

void CDB_HNGT::GetNext(POSITION& rNextPosition,T_HNGT_K& rKey,T_HNGT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_hngt.GetNextAssoc(rNextPosition,rKey,rData);
}
