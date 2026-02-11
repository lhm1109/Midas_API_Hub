#include "stdafx.h"

#include "DB_ELEM.h"
#include "DB_BENDMODULUS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BENDMODULUS::CDB_BENDMODULUS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_BENDMODULUS.InitHashTable(HASHSIZEBENDMODULUS);
}

CDB_BENDMODULUS::~CDB_BENDMODULUS()
{

}

void CDB_BENDMODULUS::Add(T_BENDMODULUS_K Key,T_BENDMODULUS_D& rData,CDB_ELEM* pElem)
{
	T_BENDMODULUS_D Data;
	BOOL bExist=m_BENDMODULUS.Lookup(Key, Data);
	m_BENDMODULUS.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_BMRO_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_BMRO_CMD, Key);
	}
}

BOOL CDB_BENDMODULUS::Del(T_BENDMODULUS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_BENDMODULUS.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_BMRO_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_BMRO_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_BENDMODULUS::Get(T_BENDMODULUS_K Key,T_BENDMODULUS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_bendmodulus->Lookup(Key, uTemp)) return FALSE;
	}
	return m_BENDMODULUS.Lookup(Key,rData);
}

int CDB_BENDMODULUS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_bendmodulus->GetCount();
	}
	return m_BENDMODULUS.GetCount();
}

POSITION CDB_BENDMODULUS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_bendmodulus->GetStartPosition();
	}
	return m_BENDMODULUS.GetStartPosition();
}

void CDB_BENDMODULUS::GetNext(POSITION& rNextPosition,T_BENDMODULUS_K& rKey,T_BENDMODULUS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_bendmodulus->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_BENDMODULUS.Lookup(rKey, rData);
		return;
	}
	m_BENDMODULUS.GetNextAssoc(rNextPosition,rKey,rData);
}
