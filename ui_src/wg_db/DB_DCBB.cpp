#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DCBB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DCBB::CDB_DCBB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dcbb.InitHashTable(HASHSIZEDCBB);
}

CDB_DCBB::~CDB_DCBB()
{

}

void CDB_DCBB::Add(T_DCBB_K Key,T_DCBB_D& rData,CDB_ELEM* pElem)
{
	T_DCBB_D Data;
	BOOL bExist=m_dcbb.Lookup(Key, Data);
	m_dcbb.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_DCBB_CMD, Key);
}

BOOL CDB_DCBB::Del(T_DCBB_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_dcbb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_DCBB_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DCBB::Get(T_DCBB_K Key,T_DCBB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dcbb.Lookup(Key,rData);
}

int CDB_DCBB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcbb.GetCount();
}

POSITION CDB_DCBB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcbb.GetStartPosition();
}

void CDB_DCBB::GetNext(POSITION& rNextPosition,T_DCBB_K& rKey,T_DCBB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_dcbb.GetNextAssoc(rNextPosition,rKey,rData);
}
