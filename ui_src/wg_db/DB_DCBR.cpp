#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DCBR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DCBR::CDB_DCBR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dcbr.InitHashTable(HASHSIZEDCBR);
}

CDB_DCBR::~CDB_DCBR()
{

}

void CDB_DCBR::Add(T_DCBR_K Key,T_DCBR_D& rData,CDB_ELEM* pElem)
{
	T_DCBR_D Data;
	BOOL bExist=m_dcbr.Lookup(Key, Data);
	m_dcbr.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_DCBR_CMD, Key);
}

BOOL CDB_DCBR::Del(T_DCBR_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_dcbr.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_DCBR_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DCBR::Get(T_DCBR_K Key,T_DCBR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dcbr.Lookup(Key,rData);
}

int CDB_DCBR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcbr.GetCount();
}

POSITION CDB_DCBR::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dcbr.GetStartPosition();
}

void CDB_DCBR::GetNext(POSITION& rNextPosition,T_DCBR_K& rKey,T_DCBR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_dcbr.GetNextAssoc(rNextPosition,rKey,rData);
}
