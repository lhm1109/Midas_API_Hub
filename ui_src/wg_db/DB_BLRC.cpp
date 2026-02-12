#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_BLRC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BLRC::CDB_BLRC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_blrc.InitHashTable(HASHSIZEBLRC);
}

CDB_BLRC::~CDB_BLRC()
{

}

void CDB_BLRC::Add(T_BLRC_K Key,T_BLRC_D& rData)
{
	T_BLRC_D Data;
	BOOL bExist=m_blrc.Lookup(Key, Data);
	m_blrc.SetAt(Key,rData);
}

BOOL CDB_BLRC::Del(T_BLRC_K Key)
{
	BOOL ret=m_blrc.RemoveKey(Key);
	ASSERT(ret);
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_BLRC::Get(T_BLRC_K Key,T_BLRC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return FALSE;
	}
	return m_blrc.Lookup(Key,rData);
}

int CDB_BLRC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return 0;
	}
	return m_blrc.GetCount();
}

POSITION CDB_BLRC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0  && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		return 0;
	}
	return m_blrc.GetStartPosition();
}

void CDB_BLRC::GetNext(POSITION& rNextPosition,T_BLRC_K& rKey,T_BLRC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0); return;
	}
	m_blrc.GetNextAssoc(rNextPosition,rKey,rData);
}
