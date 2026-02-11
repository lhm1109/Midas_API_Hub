#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_STFE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_STFE::CDB_STFE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_stfe.InitHashTable(HASHSIZESTFE);
}

CDB_STFE::~CDB_STFE()
{

}

void CDB_STFE::Add(T_STFE_K Key,T_STFE_D& rData,CDB_SECT* pSect)
{
	T_STFE_D Data;
	BOOL bExist=m_stfe.Lookup(Key, Data);
	m_stfe.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_STFE_CMD, Key);
}

BOOL CDB_STFE::Del(T_STFE_K Key,CDB_SECT* pSect)
{
	T_STFE_D Data;
	BOOL bExist=m_stfe.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_stfe.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_STFE_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_STFE::Get(T_STFE_K Key,T_STFE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_stfe.Lookup(Key,rData);
}

int CDB_STFE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_stfe.GetCount();
}

POSITION CDB_STFE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_stfe.GetStartPosition();
}

void CDB_STFE::GetNext(POSITION& rNextPosition,T_STFE_K& rKey,T_STFE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_stfe.GetNextAssoc(rNextPosition,rKey,rData);
}
