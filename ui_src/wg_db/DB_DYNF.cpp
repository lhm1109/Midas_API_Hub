#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DYNF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DYNF::CDB_DYNF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dynf.InitHashTable(HASHSIZEDYNF);
}

CDB_DYNF::~CDB_DYNF()
{

}

void CDB_DYNF::Add(T_DYNF_K Key,T_DYNF_D& rData,CDB_ELEM* pElem)
{
	T_DYNF_D Data;
	BOOL bExist=m_dynf.Lookup(Key, Data);
	m_dynf.SetAt(Key,rData);
	if(!bExist) pElem->AddListItem(Key, LT_DYNF_CMD, Key);
}

BOOL CDB_DYNF::Del(T_DYNF_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_dynf.RemoveKey(Key);
	ASSERT(ret);
	if(ret) VERIFY(pElem->DelListItem(Key, LT_DYNF_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DYNF::Get(T_DYNF_K Key,T_DYNF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dynf.Lookup(Key,rData);
}

int CDB_DYNF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dynf.GetCount();
}

POSITION CDB_DYNF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dynf.GetStartPosition();
}

void CDB_DYNF::GetNext(POSITION& rNextPosition,T_DYNF_K& rKey,T_DYNF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_dynf.GetNextAssoc(rNextPosition,rKey,rData);
}
