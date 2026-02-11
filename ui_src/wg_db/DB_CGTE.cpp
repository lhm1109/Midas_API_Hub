#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_CGTE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGTE::CDB_CGTE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgte.InitHashTable(HASHSIZECGTE);
}

CDB_CGTE::~CDB_CGTE()
{

}

void CDB_CGTE::Add(T_CGTE_K Key,T_CGTE_D& rData,CDB_NODE* pNode)
{
	T_CGTE_D Data;
	BOOL bExist=m_cgte.Lookup(Key, Data);
	m_cgte.SetAt(Key,rData);
	if(!bExist)pNode->AddListItem(Key, LT_CGTE_CMD, Key);
}

BOOL CDB_CGTE::Del(T_CGTE_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_cgte.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pNode->DelListItem(Key, LT_CGTE_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGTE::Get(T_CGTE_K Key,T_CGTE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgte.Lookup(Key,rData);
}

int CDB_CGTE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgte.GetCount();
}

POSITION CDB_CGTE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgte.GetStartPosition();
}

void CDB_CGTE::GetNext(POSITION& rNextPosition,T_CGTE_K& rKey,T_CGTE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgte.GetNextAssoc(rNextPosition,rKey,rData);
}
