#include "stdafx.h"
#include "DB_HLFW.h"
#include "DB_ELEM.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HLFW::CDB_HLFW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_HLFW.InitHashTable(HASHSIZEHLFW);
}

CDB_HLFW::~CDB_HLFW()
{
}

void CDB_HLFW::Add(T_HLFW_K Key, T_HLFW_D& rData, CDB_ELEM* pElem)
{
	T_HLFW_D Data;
	BOOL bExist = m_HLFW.Lookup(Key, Data);
	m_HLFW.SetAt(Key, rData);
	if ( !bExist && pElem )
	{
		pElem->AddListItem(Key, LT_HLFW_CMD, Key);
	}
}

BOOL CDB_HLFW::Del(T_HLFW_K Key, CDB_ELEM* pElem)
{
	BOOL bRemove = m_HLFW.RemoveKey(Key);
	ASSERT(bRemove);
	if ( bRemove && pElem )
	{
		VERIFY(pElem->DelListItem(Key, LT_HLFW_CMD, Key));
	}
	return bRemove;
}

BOOL CDB_HLFW::Get(T_HLFW_K Key, T_HLFW_D& rData)
{
	if ( m_pDoc == nullptr ) return FALSE;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return FALSE;
	return m_HLFW.Lookup(Key, rData);
}

int CDB_HLFW::GetCount()
{
	if ( m_pDoc == nullptr ) return 0;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return 0;
	return m_HLFW.GetCount();
}

POSITION CDB_HLFW::GetStart()
{
	if ( m_pDoc == nullptr ) return nullptr;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) return nullptr;
	return m_HLFW.GetStartPosition();
}

void CDB_HLFW::GetNext(POSITION& rNextPosition, T_HLFW_K& rKey, T_HLFW_D& rData)
{
	if ( m_pDoc == nullptr ) return;
	if ( m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() ) { ASSERT(0); return; }
	m_HLFW.GetNextAssoc(rNextPosition, rKey, rData);
}