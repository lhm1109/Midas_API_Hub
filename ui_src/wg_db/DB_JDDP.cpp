#include "stdafx.h"
#include "DB_JDDP.h"
#include "DB_NODE.h"
#include "StagCtrl.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_JDDP::CDB_JDDP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_JDDP.InitHashTable(HASHSIZEJDDP);
}

CDB_JDDP::~CDB_JDDP()
{
	
}

void CDB_JDDP::Add(T_JDDP_K Key,T_JDDP_D& rData,CDB_NODE* pNode)
{
	T_JDDP_D Data;
	BOOL bExist=m_JDDP.Lookup(Key, Data);
	m_JDDP.SetAt(Key,rData);
	if(!bExist && pNode)pNode->AddListItem(Key, LT_JDDP_CMD, Key);
}

BOOL CDB_JDDP::Del(T_JDDP_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_JDDP.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pNode)VERIFY(pNode->DelListItem(Key, LT_JDDP_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_JDDP::Get(T_JDDP_K Key,T_JDDP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_JDDP.Lookup(Key,rData);
}

int CDB_JDDP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_JDDP.GetCount();
}

POSITION CDB_JDDP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_JDDP.GetStartPosition();
}

void CDB_JDDP::GetNext(POSITION& rNextPosition,T_JDDP_K& rKey,T_JDDP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_JDDP.GetNextAssoc(rNextPosition,rKey,rData);
}
