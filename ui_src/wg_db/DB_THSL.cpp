#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THSL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THSL::CDB_THSL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_thsl.InitHashTable(HASHSIZETHSL);
}

CDB_THSL::~CDB_THSL()
{

}

void CDB_THSL::Add(T_THSL_K Key,T_THSL_D& rData,CDB_THIS* pThis)
{
	T_THSL_D Data;
	BOOL bExist=m_thsl.Lookup(Key, Data);
	m_thsl.SetAt(Key,rData);
	if(!bExist)pThis->AddListItem(rData.nThisKey, LT_THSL_CMD, Key);
	if(Key == m_nStartNum)
	{
		T_THSL_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!m_thsl.Lookup(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_THSL::Del(T_THSL_K Key,CDB_THIS* pThis)
{
	T_THSL_D Data;
	BOOL bExist=m_thsl.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_thsl.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pThis->DelListItem(Data.nThisKey, LT_THSL_CMD, Key));
		}

		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_THSL_K key;
			T_THSL_D data;
			if(m_thsl.Lookup(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_thsl.GetStartPosition();
				while(pos != NULL)
				{
					m_thsl.GetNextAssoc(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THSL::Get(T_THSL_K Key,T_THSL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thsl.Lookup(Key,rData);
}

int CDB_THSL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thsl.GetCount();
}

POSITION CDB_THSL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thsl.GetStartPosition();
}

void CDB_THSL::GetNext(POSITION& rNextPosition,T_THSL_K& rKey,T_THSL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_thsl.GetNextAssoc(rNextPosition,rKey,rData);
}

