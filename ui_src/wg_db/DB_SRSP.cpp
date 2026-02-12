#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SRSP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SRSP::CDB_SRSP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_srsp.InitHashTable(HASHSIZESRSP);
}

CDB_SRSP::~CDB_SRSP()
{

}

void CDB_SRSP::Add(T_SRSP_K Key, T_SRSP_D& rData, CDB_ELEM* pElem)
{
	T_SRSP_D Data;
	BOOL bExist = m_srsp.Lookup(Key, Data);
	m_srsp.SetAt(Key, rData);
	if (!bExist)pElem->AddListItem(Key, LT_SRSP_CMD, Key);
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SRSP_CMD, Key);
	}
}

BOOL CDB_SRSP::Del(T_SRSP_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_srsp.RemoveKey(Key);
	ASSERT(ret);
	if (ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SRSP_CMD, Key));
		// Current Stage? Final Stage?? Stage DB? Update??.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SRSP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SRSP::Get(T_SRSP_K Key, T_SRSP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_srsp.Lookup(Key, rData);
}

int CDB_SRSP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srsp.GetCount();
}

POSITION CDB_SRSP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srsp.GetStartPosition();
}

void CDB_SRSP::GetNext(POSITION& rNextPosition, T_SRSP_K& rKey, T_SRSP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_srsp.GetNextAssoc(rNextPosition, rKey, rData);
}
