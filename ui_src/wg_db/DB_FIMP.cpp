#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_FIMP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FIMP::CDB_FIMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_fimp.InitHashTable(HASHSIZEFIMP);
}

CDB_FIMP::~CDB_FIMP()
{
}

void CDB_FIMP::Add(T_FIMP_K Key,T_FIMP_D& rData)
{
	m_fimp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_FIMP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_FIMP::Del(T_FIMP_K Key)
{
	T_FIMP_D Data;
	BOOL bExist=m_fimp.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_fimp.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_FIMP_K key;
				T_FIMP_D data;
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
BOOL CDB_FIMP::Get(T_FIMP_K Key,T_FIMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_fimp.Lookup(Key,rData);
}

int CDB_FIMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fimp.GetCount();
}

POSITION CDB_FIMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fimp.GetStartPosition();
}

void CDB_FIMP::GetNext(POSITION& rNextPosition,T_FIMP_K& rKey,T_FIMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_fimp.GetNextAssoc(rNextPosition,rKey,rData);
}
