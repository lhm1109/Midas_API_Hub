#include "stdafx.h"
#include "DB_STOR.h"
#include "DB_MDUL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MDUL::CDB_MDUL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_mdul.InitHashTable(HASHSIZEMDUL);
}

CDB_MDUL::~CDB_MDUL()
{
}

void CDB_MDUL::Add(T_MDUL_K Key,T_MDUL_D& rData,CDB_STOR* pStor)
{
	T_MDUL_D Data;
	BOOL bExist=m_mdul.Lookup(Key, Data);

	CMap<T_STOR_K, T_STOR_K, T_STOR_K, T_STOR_K>mapOld;
	CMap<T_STOR_K, T_STOR_K, T_STOR_K, T_STOR_K>mapNew;
	T_STOR_K KeyStorTemp;
	if(bExist)
	{
		mapNew.InitHashTable(HASHSIZESTOR);
		mapOld.InitHashTable(HASHSIZESTOR);
		for(int i = 0; i<Data.aStorK.GetSize(); i++)
			mapOld.SetAt(Data.aStorK[i], Data.aStorK[i]);
		for(int i = 0; i<rData.aStorK.GetSize(); i++)
			mapNew.SetAt(rData.aStorK[i], rData.aStorK[i]);

		int nCount=Data.aStorK.GetSize();
		for(int i = 0; i<nCount; i++)
		{
			if(!mapNew.Lookup(Data.aStorK[i], KeyStorTemp))
				pStor->DelListItem(Data.aStorK[i], LT_MDUL_CMD, Key);
		}
	}
	int nCount=rData.aStorK.GetSize();
	for(int i=0; i<nCount; i++)
	{
		if(bExist)
		{
			if(!mapOld.Lookup(rData.aStorK[i], KeyStorTemp))
				pStor->AddListItem(rData.aStorK[i], LT_MDUL_CMD, Key);
		}
		else
			pStor->AddListItem(rData.aStorK[i], LT_MDUL_CMD, Key);
	}
	m_mdul.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_MDUL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MDUL::Del(T_MDUL_K Key,CDB_STOR* pStor)
{
	T_MDUL_D data;
	if(!Get(Key, data))return FALSE;

	BOOL ret=m_mdul.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		int nCount=data.aStorK.GetSize();
		for(int i=0; i<nCount; i++)
			pStor->DelListItem(data.aStorK[i], LT_MDUL_CMD, Key);

		if(Key < m_nStartNum)	m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MDUL_K key;
			T_MDUL_D data;
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
BOOL CDB_MDUL::Get(T_MDUL_K Key,T_MDUL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
	}
	return m_mdul.Lookup(Key,rData);
}

int CDB_MDUL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	}
	return m_mdul.GetCount();
}

POSITION CDB_MDUL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	}
	return m_mdul.GetStartPosition();
}

void CDB_MDUL::GetNext(POSITION& rNextPosition,T_MDUL_K& rKey,T_MDUL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
	}
	m_mdul.GetNextAssoc(rNextPosition,rKey,rData);
}
