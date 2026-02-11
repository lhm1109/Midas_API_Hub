#include "stdafx.h"
#include "DB_SPSC.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPSC::CDB_SPSC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Spsc.InitHashTable(HASHSIZESPSC);
}

CDB_SPSC::~CDB_SPSC()
{

}

void CDB_SPSC::Add(T_SPSC_K Key, T_SPSC_D& rData, CDB_ELEM* pElem)
{
	T_SPSC_D Data;
	BOOL bExist = m_Spsc.Lookup(Key, Data);
	m_Spsc.SetAt(Key, rData);
	if ( !bExist && pElem )pElem->AddListItem(Key, LT_SPSC_CMD, Key);
	//if(!bExist && pSect)pSect->AddListItem(Key, LT_SPSC_CMD, Key);
}

BOOL CDB_SPSC::Del(T_SPSC_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_Spsc.RemoveKey(Key);
	ASSERT(ret);
	if ( ret && pElem )VERIFY(pElem->DelListItem(Key, LT_SPSC_CMD, Key));
	//if(ret && pSect)VERIFY(pSect->DelListItem(Key, LT_SPSC_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SPSC::Get(T_SPSC_K Key, T_SPSC_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return FALSE;
	return m_Spsc.Lookup(Key, rData);
}

int CDB_SPSC::GetCount()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_Spsc.GetCount();
}

POSITION CDB_SPSC::GetStart()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_Spsc.GetStartPosition();
}

void CDB_SPSC::GetNext(POSITION& rNextPosition, T_SPSC_K& rKey, T_SPSC_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
	{
		ASSERT(0);
		return;
	}
	m_Spsc.GetNextAssoc(rNextPosition, rKey, rData);
}
