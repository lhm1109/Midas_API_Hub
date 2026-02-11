#include "stdafx.h"
#include "DB_SCDR.h"
#include "DB_STOR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SCDR::CDB_SCDR()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_SCDR.InitHashTable(HASHSIZESCDR);
}

CDB_SCDR::~CDB_SCDR()
{
}

void CDB_SCDR::Add(T_SCDR_K Key,T_SCDR_D& rData, CDB_STOR* pStor)
{
	T_SCDR_D Data;
	BOOL bExist=m_SCDR.Lookup(Key, Data);
	if(bExist)
	{
		VERIFY(pStor->DelListItem(rData.StorK, LT_SCDR_CMD, Key));
	}
	m_SCDR.SetAt(Key,rData);
	if (pStor)
	{
		pStor->AddListItem(rData.StorK, LT_SCDR_CMD, Key);
	}
	if(Key == m_nStartNum)
	{
		T_SCDR_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_SCDR::Del(T_SCDR_K Key, CDB_STOR* pStor)
{
	T_SCDR_D Data;
	BOOL bExist=m_SCDR.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_SCDR.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if (pStor)
			{
				VERIFY(pStor->DelListItem(Data.StorK, LT_SCDR_CMD, Key));
			}
		}
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_SCDR_K key;
			T_SCDR_D data;
			if(m_SCDR.Lookup(m_nLastNum-1,data))
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
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SCDR::Get(T_SCDR_K Key,T_SCDR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_SCDR.Lookup(Key,rData);
}

int CDB_SCDR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SCDR.GetCount();
}

POSITION CDB_SCDR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SCDR.GetStartPosition();
}

void CDB_SCDR::GetNext(POSITION& rNextPosition,T_SCDR_K& rKey,T_SCDR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_SCDR.GetNextAssoc(rNextPosition,rKey,rData);
}
