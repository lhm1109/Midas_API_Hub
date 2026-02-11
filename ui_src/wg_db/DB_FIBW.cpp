#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_FIBW.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FIBW::CDB_FIBW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_fibw.InitHashTable(HASHSIZEFIBW);
}

CDB_FIBW::~CDB_FIBW()
{
}

void CDB_FIBW::Add(T_FIBW_K Key,T_FIBW_D& rData)
{
	m_fibw.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_FIBW_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_FIBW::Del(T_FIBW_K Key)
{
	T_FIBW_D Data;
	BOOL bExist=m_fibw.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_fibw.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_FIBW_K key;
				T_FIBW_D data;
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
BOOL CDB_FIBW::Get(T_FIBW_K Key,T_FIBW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_fibw.Lookup(Key,rData);
}

T_FIBW_D* CDB_FIBW::GetFibwPair(T_FIBW_K Key)
{
	CMap<unsigned int, unsigned int, T_FIBW_D, T_FIBW_D&>::CPair* pair = m_fibw.PLookup(Key);
	if (pair == NULL)
		return NULL;
	return &pair->value;
}

int CDB_FIBW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fibw.GetCount();
}

POSITION CDB_FIBW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fibw.GetStartPosition();
}

void CDB_FIBW::GetNext(POSITION& rNextPosition,T_FIBW_K& rKey,T_FIBW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_fibw.GetNextAssoc(rNextPosition,rKey,rData);
}

CMap<unsigned int, unsigned int, T_FIBW_D, T_FIBW_D&>::CPair* CDB_FIBW::PGetFirstAssoc()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{

		//ASSERT(0);
		return nullptr;
	}
	return m_fibw.PGetFirstAssoc();
}
CMap<unsigned int, unsigned int, T_FIBW_D, T_FIBW_D&>::CPair* CDB_FIBW::PGetNext(CMap<T_FIBW_K, T_FIBW_K, T_FIBW_D, T_FIBW_D&>::CPair* pair)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return nullptr;
	}
	
	return m_fibw.PGetNextAssoc(pair);
}