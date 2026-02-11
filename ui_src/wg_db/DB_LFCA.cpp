#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_LFCA.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LFCA::CDB_LFCA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_lfca.InitHashTable(HASHSIZELFCA);
}

CDB_LFCA::~CDB_LFCA()
{

}

void CDB_LFCA::Add(T_LFCA_K Key,T_LFCA_D& rData)
{
	T_LFCA_D Data;
	BOOL bExist=m_lfca.Lookup(Key.keymap, Data);
	m_lfca.SetAt(Key.keymap,rData);
}

BOOL CDB_LFCA::Del(T_LFCA_K Key)
{
	T_LFCA_D Data;
	BOOL bExist=m_lfca.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_lfca.RemoveKey(Key.keymap);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_LFCA::Get(T_LFCA_K Key,T_LFCA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_lfca.Lookup(Key.keymap,rData);
}

int CDB_LFCA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_lfca.GetCount();
}

POSITION CDB_LFCA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_lfca.GetStartPosition();
}

void CDB_LFCA::GetNext(POSITION& rNextPosition,T_LFCA_K& rKey,T_LFCA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_lfca.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
