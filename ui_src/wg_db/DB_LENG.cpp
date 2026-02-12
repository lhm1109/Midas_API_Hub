#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_LENG.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LENG::CDB_LENG()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_leng.InitHashTable(HASHSIZELENG);
}

CDB_LENG::~CDB_LENG()
{

}

void CDB_LENG::Add(T_LENG_K Key,T_LENG_D& rData,CDB_ELEM* pElem)
{
	T_LENG_D Data;
	BOOL bExist=m_leng.Lookup(Key, Data);
	m_leng.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_LENG_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_LENG_CMD, Key);
	}
}

BOOL CDB_LENG::Del(T_LENG_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_leng.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_LENG_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_LENG_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_LENG::Get(T_LENG_K Key,T_LENG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_leng->Lookup(Key, uTemp)) return FALSE;
	}
	return m_leng.Lookup(Key,rData);
}

int CDB_LENG::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_leng->GetCount();
	}
	return m_leng.GetCount();
}

POSITION CDB_LENG::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_leng->GetStartPosition();
	}
	return m_leng.GetStartPosition();
}

void CDB_LENG::GetNext(POSITION& rNextPosition,T_LENG_K& rKey,T_LENG_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_leng->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_leng.Lookup(rKey, rData);
		return;
	}
	m_leng.GetNextAssoc(rNextPosition,rKey,rData);
}
