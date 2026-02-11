#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SRVP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SRVP::CDB_SRVP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_srvp.InitHashTable(HASHSIZESRVP);
}

CDB_SRVP::~CDB_SRVP()
{

}

void CDB_SRVP::Add(T_SRVP_K Key, T_SRVP_D& rData/*, CDB_ELEM* pElem*/)
{
	T_SRVP_D Data;
	BOOL bExist = m_srvp.Lookup(Key, Data);
	m_srvp.SetAt(Key, rData);
// 	if (!bExist)pElem->AddListItem(Key, LT_SRVP_CMD, Key);
// 	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
// 	{
// 		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SRVP_CMD, Key);
// 	}
}

BOOL CDB_SRVP::Del(T_SRVP_K Key/*, CDB_ELEM* pElem*/)
{
	BOOL ret = m_srvp.RemoveKey(Key);
	ASSERT(ret);
// 	if (ret)
// 	{
// 		VERIFY(pElem->DelListItem(Key, LT_SRVP_CMD, Key));
// 		// Current Stage? Final Stage?? Stage DB? Update??.
// 		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
// 			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SRVP_CMD, Key);
// 	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SRVP::Get(T_SRVP_K Key, T_SRVP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_srvp.Lookup(Key, rData);
}

int CDB_SRVP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srvp.GetCount();
}

POSITION CDB_SRVP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srvp.GetStartPosition();
}

void CDB_SRVP::GetNext(POSITION& rNextPosition, T_SRVP_K& rKey, T_SRVP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_srvp.GetNextAssoc(rNextPosition, rKey, rData);
}
