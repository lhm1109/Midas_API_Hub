#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_HAHS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HAHS::CDB_HAHS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_hahs.InitHashTable(HASHSIZEHAHS);
}

CDB_HAHS::~CDB_HAHS()
{

}

void CDB_HAHS::Add(T_HAHS_K Key,T_HAHS_D& rData,CDB_ELEM* pElem)
{
	T_HAHS_D Data;
	BOOL bExist=m_hahs.Lookup(Key, Data);
	m_hahs.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_HAHS_CMD, Key);
}

BOOL CDB_HAHS::Del(T_HAHS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_hahs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_HAHS_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_HAHS::Get(T_HAHS_K Key,T_HAHS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_hahs->Lookup(Key, uTemp)) return FALSE;
	}
	return m_hahs.Lookup(Key,rData);
}

int CDB_HAHS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_hahs->GetCount();
	}
	return m_hahs.GetCount();
}

POSITION CDB_HAHS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_hahs->GetStartPosition();
	}
	return m_hahs.GetStartPosition();
}

void CDB_HAHS::GetNext(POSITION& rNextPosition,T_HAHS_K& rKey,T_HAHS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		//if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_hahs->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_hahs.Lookup(rKey, rData);
		return;
	}
	m_hahs.GetNextAssoc(rNextPosition,rKey,rData);
}
