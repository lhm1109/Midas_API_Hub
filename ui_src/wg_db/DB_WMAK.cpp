#include "stdafx.h"
#include "DB_WMAK.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_WMAK::CDB_WMAK()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_wmak.InitHashTable(HASHSIZEWMAK);
	m_usrw.InitHashTable(HASHSIZEWALL);
}

CDB_WMAK::~CDB_WMAK()
{
}

void CDB_WMAK::Add(T_WMAK_K Key,T_WMAK_D& rData)
{
	T_WMAK_D DataWmak;
	if(m_wmak.Lookup(Key, DataWmak))
	{
		for(int i=0; i<DataWmak.arUserWallId.GetSize(); i++)m_usrw.RemoveKey(DataWmak.arUserWallId[i]);
	}
	m_wmak.SetAt(Key,rData);
	for(int i=0; i<rData.arUserWallId.GetSize(); i++)m_usrw.SetAt(rData.arUserWallId[i], Key);
	if(Key == m_nStartNum)
	{
		T_WMAK_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_WMAK::Del(T_WMAK_K Key)
{
	T_WMAK_D DataWmak;
	if(m_wmak.Lookup(Key, DataWmak))
	{
		for(int i=0; i<DataWmak.arUserWallId.GetSize(); i++)m_usrw.RemoveKey(DataWmak.arUserWallId[i]);
	}
	BOOL ret=m_wmak.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_WMAK_K key;
			T_WMAK_D data;
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
BOOL CDB_WMAK::Get(T_WMAK_K Key,T_WMAK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_wmak.Lookup(Key,rData);
}

int CDB_WMAK::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_wmak.GetCount();
}

POSITION CDB_WMAK::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_wmak.GetStartPosition();
}

void CDB_WMAK::GetNext(POSITION& rNextPosition,T_WMAK_K& rKey,T_WMAK_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_wmak.GetNextAssoc(rNextPosition,rKey,rData);
}

BOOL CDB_WMAK::GetWmak(int nUserWallId, T_WMAK_K& rKey)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_usrw.Lookup(nUserWallId, rKey);
}

BOOL CDB_WMAK::Exist(int nUserWallId)
{ 
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	T_WMAK_K KeyTemp;
	return m_usrw.Lookup(nUserWallId, KeyTemp);
}
