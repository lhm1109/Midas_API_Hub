#include "stdafx.h"
#include "stdafx.h"
#include "DB_GRUP.h"
#include "DB_BLRG.h"

#include "DBDoc.h"
#include "StagCtrl.h"



CDB_BLRG::CDB_BLRG()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_blrg.InitHashTable(HASHSIZEBLRG);
}

CDB_BLRG::~CDB_BLRG()
{

}

void CDB_BLRG::Add(T_BLRG_K Key,T_BLRG_D& rData,CDB_GRUP* pGrup)
{
	T_BLRG_D Data;
	BOOL bExist=m_blrg.Lookup(Key, Data);
	m_blrg.SetAt(Key,rData);
	if(!bExist)pGrup->AddListItem(Key, LT_BLRG_CMD, Key);

}

BOOL CDB_BLRG::Del(T_BLRG_K Key,CDB_GRUP* pGrup)
{
	BOOL ret=m_blrg.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pGrup->DelListItem(Key, LT_BLRG_CMD, Key));
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_BLRG::Get(T_BLRG_K Key,T_BLRG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
	}
	return m_blrg.Lookup(Key,rData);
}

int CDB_BLRG::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		 if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	}
	return m_blrg.GetCount();
}

POSITION CDB_BLRG::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
			if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
	}
	return m_blrg.GetStartPosition();
}

void CDB_BLRG::GetNext(POSITION& rNextPosition,T_BLRG_K& rKey,T_BLRG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
	}
	m_blrg.GetNextAssoc(rNextPosition,rKey,rData);
}
