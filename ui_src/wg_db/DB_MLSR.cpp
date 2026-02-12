#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_MLSR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MLSR::CDB_MLSR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mlsr.InitHashTable(HASHSIZEMLSR);
}

CDB_MLSR::~CDB_MLSR()
{

}

void CDB_MLSR::Add(T_MLSR_K Key,T_MLSR_D& rData,CDB_NODE* pNode)
{
	T_MLSR_D Data;
	BOOL bExist=m_mlsr.Lookup(Key, Data);
	m_mlsr.SetAt(Key,rData);
	if(!bExist && pNode != NULL)pNode->AddListItem(Key, LT_MLSR_CMD, Key);
}

BOOL CDB_MLSR::Del(T_MLSR_K Key,CDB_NODE* pNode)
{
	BOOL ret=m_mlsr.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pNode != NULL)VERIFY(pNode->DelListItem(Key, LT_MLSR_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MLSR::Get(T_MLSR_K Key,T_MLSR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mlsr->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mlsr.Lookup(Key,rData);
}

int CDB_MLSR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if(!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mlsr->GetCount();
	}
	return m_mlsr.GetCount();
}

POSITION CDB_MLSR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mlsr->GetStartPosition();
	}
	return m_mlsr.GetStartPosition();
}

void CDB_MLSR::GetNext(POSITION& rNextPosition,T_MLSR_K& rKey,T_MLSR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{ 
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mlsr->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mlsr.Lookup(rKey, rData);
		return;
	}
	m_mlsr.GetNextAssoc(rNextPosition,rKey,rData);
}
