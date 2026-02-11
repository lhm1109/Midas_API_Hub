#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_CGTE_IRC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGTE_IRC::CDB_CGTE_IRC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgte_IRC.InitHashTable(HASHSIZE_IRC_CGTE);
}

CDB_CGTE_IRC::~CDB_CGTE_IRC()
{

}

void CDB_CGTE_IRC::Add(T_CGTE_IRC_K Key,T_CGTE_IRC_D& rData,CDB_NODE* pNode)
{
	T_CGTE_IRC_D Data;
	BOOL bExist=m_cgte_IRC.Lookup(Key, Data);
	m_cgte_IRC.SetAt(Key,rData);
	if(!bExist)pNode->AddListItem(Key, LT_CGTE_IRC_CMD, Key);
}

BOOL CDB_CGTE_IRC::Del(T_CGTE_IRC_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_cgte_IRC.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pNode->DelListItem(Key, LT_CGTE_IRC_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGTE_IRC::Get(T_CGTE_IRC_K Key,T_CGTE_IRC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgte_IRC.Lookup(Key,rData);
}

int CDB_CGTE_IRC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgte_IRC.GetCount();
}

POSITION CDB_CGTE_IRC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgte_IRC.GetStartPosition();
}

void CDB_CGTE_IRC::GetNext(POSITION& rNextPosition,T_CGTE_IRC_K& rKey,T_CGTE_IRC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgte_IRC.GetNextAssoc(rNextPosition,rKey,rData);
}
