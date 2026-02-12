#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SERV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SERV::CDB_SERV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_serv.InitHashTable(HASHSIZESERV);
}

CDB_SERV::~CDB_SERV()
{

}

void CDB_SERV::Add(T_SERV_K Key,T_SERV_D& rData,CDB_ELEM* pElem)
{
	T_SERV_D Data;
	BOOL bExist=m_serv.Lookup(Key, Data);
	m_serv.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SERV_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SERV_CMD, Key);
	}
}

BOOL CDB_SERV::Del(T_SERV_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_serv.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SERV_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SERV_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SERV::Get(T_SERV_K Key,T_SERV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_serv->Lookup(Key, uTemp)) return FALSE;
	}
	return m_serv.Lookup(Key,rData);
}

int CDB_SERV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_serv->GetCount();
	}
	return m_serv.GetCount();
}

POSITION CDB_SERV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_serv->GetStartPosition();
	}
	return m_serv.GetStartPosition();
}

void CDB_SERV::GetNext(POSITION& rNextPosition,T_SERV_K& rKey,T_SERV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_serv->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_serv.Lookup(rKey, rData);
		return;
	}
	m_serv.GetNextAssoc(rNextPosition,rKey,rData);
}

