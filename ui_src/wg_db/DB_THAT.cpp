#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THAT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THAT::CDB_THAT()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_that.InitHashTable(HASHSIZETHAT);
}

CDB_THAT::~CDB_THAT()
{

}

void CDB_THAT::Add(T_THAT_K Key,T_THAT_D& rData)
{
	T_THAT_D Data;
	BOOL bExist=m_that.Lookup(Key, Data);
	m_that.SetAt(Key,rData);  
}

BOOL CDB_THAT::Del(T_THAT_K Key)
{
	T_THAT_D Data;
	BOOL bExist=m_that.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_that.RemoveKey(Key);
		ASSERT(ret);    
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_THAT::Get(T_THAT_K Key,T_THAT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_that.Lookup(Key,rData);
}

int CDB_THAT::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_that.GetCount();
}

POSITION CDB_THAT::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_that.GetStartPosition();
}

void CDB_THAT::GetNext(POSITION& rNextPosition,T_THAT_K& rKey,T_THAT_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_that.GetNextAssoc(rNextPosition,rKey,rData);
}
