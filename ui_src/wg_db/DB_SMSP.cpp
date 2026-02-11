#include "stdafx.h"
#include "DB_SMSP.h"
#include "DB_ELEM.h"
#include "DB_SECT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SMSP::CDB_SMSP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Smsp.InitHashTable(HASHSIZESMSP);
}

CDB_SMSP::~CDB_SMSP()
{

}

void CDB_SMSP::Add(T_SMSP_K Key,T_SMSP_D& rData, CDB_ELEM* pElem)
{
	T_SMSP_D Data;
	BOOL bExist=m_Smsp.Lookup(Key, Data);
	m_Smsp.SetAt(Key,rData);
	if(!bExist && pElem)pElem->AddListItem(Key, LT_SMSP_CMD, Key);
	//if(!bExist && pSect)pSect->AddListItem(Key, LT_SMSP_CMD, Key);
}

BOOL CDB_SMSP::Del(T_SMSP_K Key, CDB_ELEM* pElem)
{
	BOOL ret=m_Smsp.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_SMSP_CMD, Key));
	//if(ret && pSect)VERIFY(pSect->DelListItem(Key, LT_SMSP_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SMSP::Get(T_SMSP_K Key,T_SMSP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_Smsp.Lookup(Key,rData);
}

int CDB_SMSP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_Smsp.GetCount();
}

POSITION CDB_SMSP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_Smsp.GetStartPosition();
}

void CDB_SMSP::GetNext(POSITION& rNextPosition,T_SMSP_K& rKey,T_SMSP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_Smsp.GetNextAssoc(rNextPosition,rKey,rData);
}
