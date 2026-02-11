#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_IPCR.h"
#include "DB_LDGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IPCR::CDB_IPCR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_ipcr.InitHashTable(HASHSIZEIPCR);
}

CDB_IPCR::~CDB_IPCR()
{

}

void CDB_IPCR::Add(T_IPCR_K Key,T_IPCR_D& rData)
{
	T_IPCR_D Data;
	BOOL bExist=m_ipcr.Lookup(Key, Data);
	m_ipcr.SetAt(Key,rData);
	
	if(Key == m_nStartNum)
	{
		T_IPCR_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_ipcr.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_IPCR::Del(T_IPCR_K Key)
{
	T_IPCR_D Data;
	BOOL bExist=m_ipcr.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_ipcr.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_IPCR_K key;
				T_IPCR_D data;
				if(m_ipcr.Lookup(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=m_ipcr.GetStartPosition();
					while(pos != NULL)
					{
						m_ipcr.GetNextAssoc(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}      
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_IPCR::Get(T_IPCR_K Key,T_IPCR_D& rData)
{
	//if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//  T_KEY2 StagLdgrKey;
	//  if (!m_pDoc->m_pStagCtrl->m_ipcr->Lookup(Key, StagLdgrKey)) return FALSE;
	//}
	return m_ipcr.Lookup(Key,rData);
}

int CDB_IPCR::GetCount()
{
	return m_ipcr.GetCount();
}

POSITION CDB_IPCR::GetStart()
{
	return m_ipcr.GetStartPosition();
}

void CDB_IPCR::GetNext(POSITION& rNextPosition,T_IPCR_K& rKey,T_IPCR_D& rData)
{
	m_ipcr.GetNextAssoc(rNextPosition,rKey,rData);
}
