#include "stdafx.h"
#include "DB_JDJP.h"
#include "DB_NODE.h"
#include "StagCtrl.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_JDJP::CDB_JDJP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_JDJP.InitHashTable(HASHSIZEJDJP);
}

CDB_JDJP::~CDB_JDJP()
{
	
}

void CDB_JDJP::Add(T_JDJP_K Key,T_JDJP_D& rData,CDB_NODE* pNode)
{
	T_JDJP_D Data;
	BOOL bExist=m_JDJP.Lookup(Key, Data);
	m_JDJP.SetAt(Key,rData);
	if(!bExist && pNode)pNode->AddListItem(Key, LT_JDJP_CMD, Key);
}

BOOL CDB_JDJP::Del(T_JDJP_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_JDJP.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pNode)VERIFY(pNode->DelListItem(Key, LT_JDJP_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_JDJP::Get(T_JDJP_K Key,T_JDJP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_JDJP.Lookup(Key,rData);
}

int CDB_JDJP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_JDJP.GetCount();
}

POSITION CDB_JDJP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_JDJP.GetStartPosition();
}

void CDB_JDJP::GetNext(POSITION& rNextPosition,T_JDJP_K& rKey,T_JDJP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_JDJP.GetNextAssoc(rNextPosition,rKey,rData);
}
