#include "stdafx.h"
#include "DB_SESP.h"
#include "DB_ELEM.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SESP::CDB_SESP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SESP.InitHashTable(HASHSIZESESP);
}

CDB_SESP::~CDB_SESP()
{
}

void CDB_SESP::Add(T_SESP_K Key, T_SESP_D& rData, CDB_ELEM* pElem)
{
	T_SESP_D Data;
	BOOL bExist = m_SESP.Lookup(Key, Data);
	m_SESP.SetAt(Key, rData);
	if ( !bExist && pElem )
	{
		pElem->AddListItem(Key, LT_SESP_CMD, Key);
	}
}

BOOL CDB_SESP::Del(T_SESP_K Key, CDB_ELEM* pElem)
{
	BOOL bRemove = m_SESP.RemoveKey(Key);
	ASSERT(bRemove);
	if ( bRemove && pElem )
	{
		VERIFY(pElem->DelListItem(Key, LT_SESP_CMD, Key));
	}
	return bRemove;
}

BOOL CDB_SESP::Get(T_SESP_K Key, T_SESP_D& rData)
{
	if ( m_pDoc == nullptr ) return FALSE;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return FALSE;
	return m_SESP.Lookup(Key, rData);
}

int CDB_SESP::GetCount()
{
	if ( m_pDoc == nullptr ) return 0;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return 0;
	return m_SESP.GetCount();
}

POSITION CDB_SESP::GetStart()
{
	if ( m_pDoc == nullptr ) return nullptr;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return nullptr;
	return m_SESP.GetStartPosition();
}

void CDB_SESP::GetNext(POSITION& rNextPosition, T_SESP_K& rKey, T_SESP_D& rData)
{
	if ( m_pDoc == nullptr ) return;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) { ASSERT(0); return; }
	m_SESP.GetNextAssoc(rNextPosition, rKey, rData);
}