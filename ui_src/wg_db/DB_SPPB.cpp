//sppb
#include "stdafx.h"
#include "DB_SPPB.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPPB::CDB_SPPB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_Sppb.InitHashTable(HASHSIZESPPB);
}

CDB_SPPB::~CDB_SPPB()
{

}

void CDB_SPPB::Add(T_SPPB_K Key, T_SPPB_D& rData, CDB_ELEM* pElem)
{
	T_SPPB_D Data;
	BOOL bExist = m_Sppb.Lookup(Key, Data);
	m_Sppb.SetAt(Key, rData);
	if ( !bExist && pElem )pElem->AddListItem(Key, LT_SPPB_CMD, Key);
	//if(!bExist && pSect)pSect->AddListItem(Key, LT_SPPB_CMD, Key);
}

BOOL CDB_SPPB::Del(T_SPPB_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_Sppb.RemoveKey(Key);
	ASSERT(ret);
	if ( ret && pElem )VERIFY(pElem->DelListItem(Key, LT_SPPB_CMD, Key));
	//if(ret && pSect)VERIFY(pSect->DelListItem(Key, LT_SPPB_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SPPB::Get(T_SPPB_K Key, T_SPPB_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return FALSE;
	return m_Sppb.Lookup(Key, rData);
}

int CDB_SPPB::GetCount()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_Sppb.GetCount();
}

POSITION CDB_SPPB::GetStart()
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
		return 0;
	return m_Sppb.GetStartPosition();
}

void CDB_SPPB::GetNext(POSITION& rNextPosition, T_SPPB_K& rKey, T_SPPB_D& rData)
{
	if ( m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag() )
	{
		ASSERT(0);
		return;
	}
	m_Sppb.GetNextAssoc(rNextPosition, rKey, rData);
}