//splb
#include "stdafx.h"
#include "DB_SPLB.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPLB::CDB_SPLB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Splb.InitHashTable(HASHSIZESPLB);
}

CDB_SPLB::~CDB_SPLB()
{

}

void CDB_SPLB::Add(T_SPLB_K Key, T_SPLB_D& rData, CDB_ELEM* pElem)
{
	T_SPLB_D Data;
	BOOL bExist = m_Splb.Lookup(Key, Data);
	m_Splb.SetAt(Key, rData);
	if ( !bExist && pElem )pElem->AddListItem(Key, LT_SPLB_CMD, Key);
	//if(!bExist && pSect)pSect->AddListItem(Key, LT_SPLB_CMD, Key);
}

BOOL CDB_SPLB::Del(T_SPLB_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_Splb.RemoveKey(Key);
	ASSERT(ret);
	if ( ret && pElem )VERIFY(pElem->DelListItem(Key, LT_SPLB_CMD, Key));
	//if(ret && pSect)VERIFY(pSect->DelListItem(Key, LT_SPLB_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SPLB::Get(T_SPLB_K Key, T_SPLB_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return FALSE;
	return m_Splb.Lookup(Key, rData);
}

int CDB_SPLB::GetCount()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_Splb.GetCount();
}

POSITION CDB_SPLB::GetStart()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_Splb.GetStartPosition();
}

void CDB_SPLB::GetNext(POSITION& rNextPosition, T_SPLB_K& rKey, T_SPLB_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
	{
		ASSERT(0);
		return;
	}
	m_Splb.GetNextAssoc(rNextPosition, rKey, rData);
}