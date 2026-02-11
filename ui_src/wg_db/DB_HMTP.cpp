#include "stdafx.h"
#include "DB_HMTP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HMTP::CDB_HMTP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_hmtp.InitHashTable(HASHSIZEHMTP);
}

CDB_HMTP::~CDB_HMTP()
{
}

void CDB_HMTP::Add(T_HMTP_K Key,T_HMTP_D& rData)
{
	m_hmtp.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		T_HMTP_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_HMTP::Del(T_HMTP_K Key)
{
	BOOL ret=m_hmtp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_HMTP_K key;
			T_HMTP_D data;
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
BOOL CDB_HMTP::Get(T_HMTP_K Key,T_HMTP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_hmtp.Lookup(Key,rData);
}

int CDB_HMTP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hmtp.GetCount();
}

POSITION CDB_HMTP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hmtp.GetStartPosition();
}

void CDB_HMTP::GetNext(POSITION& rNextPosition,T_HMTP_K& rKey,T_HMTP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_hmtp.GetNextAssoc(rNextPosition,rKey,rData);
}
