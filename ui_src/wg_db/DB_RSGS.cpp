#include "stdafx.h"
#include "stdafx.h"
#include "DB_GRUP.h"
#include "DB_RSGS.h"

#include "DBDoc.h"
#include "StagCtrl.h"



CDB_RSGS::CDB_RSGS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rsgs.InitHashTable(HASHSIZERSGS);
}

CDB_RSGS::~CDB_RSGS()
{

}

void CDB_RSGS::Add(T_RSGS_K Key,T_RSGS_D& rData,CDB_GRUP* pGrup)
{
	T_RSGS_D Data;
	BOOL bExist=m_rsgs.Lookup(Key, Data);
	m_rsgs.SetAt(Key,rData);
	if(!bExist)pGrup->AddListItem(Key, LT_RSGS_CMD, Key);

}

BOOL CDB_RSGS::Del(T_RSGS_K Key,CDB_GRUP* pGrup)
{
	BOOL ret=m_rsgs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pGrup->DelListItem(Key, LT_RSGS_CMD, Key));
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_RSGS::Get(T_RSGS_K Key,T_RSGS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
	}
	return m_rsgs.Lookup(Key,rData);
}

int CDB_RSGS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		 if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	}
	return m_rsgs.GetCount();
}

POSITION CDB_RSGS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
			if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	}
	return m_rsgs.GetStartPosition();
}

void CDB_RSGS::GetNext(POSITION& rNextPosition,T_RSGS_K& rKey,T_RSGS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
	}
	m_rsgs.GetNextAssoc(rNextPosition,rKey,rData);
}
