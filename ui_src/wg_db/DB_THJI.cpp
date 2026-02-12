#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THJI.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THJI::CDB_THJI()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_nStartNum=1;
	m_nLastNum=0;
	m_thji.InitHashTable(HASHSIZETHJI);
}

CDB_THJI::~CDB_THJI()
{

}

void CDB_THJI::Add(T_THJI_K Key, T_THJI_D& rData)
{
	T_THJI_D Data;
	BOOL bExist=m_thji.Lookup(Key, Data);
	m_thji.SetAt(Key, rData);

	if ( Key == m_nStartNum )
	{
		T_THJI_D data;
		while ( TRUE )
		{
			m_nStartNum++;
			if ( !m_thji.Lookup(m_nStartNum, data) )break;
		}
	}
	if ( Key > m_nLastNum )m_nLastNum=Key;
}

BOOL CDB_THJI::Del(T_THJI_K Key)
{
	T_THJI_D Data;
	BOOL bExist=m_thji.Lookup(Key, Data);
	if ( bExist )
	{
		BOOL ret=m_thji.RemoveKey(Key);
		ASSERT(ret);

		if ( Key < m_nStartNum )m_nStartNum=Key;
		if ( Key==m_nLastNum )
		{
			T_THJI_K key;
			T_THJI_D data;
			if ( m_thji.Lookup(m_nLastNum-1, data) )
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=m_thji.GetStartPosition();
				while ( pos != NULL )
				{
					m_thji.GetNextAssoc(pos, key, data);
					if ( key > m_nLastNum )m_nLastNum=key;
				}
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THJI::Get(T_THJI_K Key, T_THJI_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return FALSE;
	return m_thji.Lookup(Key, rData);
}

int CDB_THJI::GetCount()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_thji.GetCount();
}

POSITION CDB_THJI::GetStart()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_thji.GetStartPosition();
}

void CDB_THJI::GetNext(POSITION& rNextPosition, T_THJI_K& rKey, T_THJI_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
	{
		ASSERT(0);
		return;
	}
	m_thji.GetNextAssoc(rNextPosition, rKey, rData);
}

