#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CPCB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPCB::CDB_CPCB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cpcb.InitHashTable(HASHSIZECPCB);
}

CDB_CPCB::~CDB_CPCB()
{

}

void CDB_CPCB::Add(T_CPCB_K Key,T_CPCB_D& rData,CDB_ELEM* pElem)
{
	T_CPCB_D Data;
	BOOL bExist=m_cpcb.Lookup(Key, Data);
	m_cpcb.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CPCB_CMD, Key);
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CPCB_CMD, Key);
	}
}

BOOL CDB_CPCB::Del(T_CPCB_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cpcb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CPCB_CMD, Key));
		// Current Stage? Final Stage?? Stage DB? Update??.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CPCB_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPCB::Get(T_CPCB_K Key,T_CPCB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cpcb.Lookup(Key,rData);
}

int CDB_CPCB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpcb.GetCount();
}

POSITION CDB_CPCB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpcb.GetStartPosition();
}

void CDB_CPCB::GetNext(POSITION& rNextPosition,T_CPCB_K& rKey,T_CPCB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cpcb.GetNextAssoc(rNextPosition,rKey,rData);
}
