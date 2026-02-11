#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_IFGS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IFGS::CDB_IFGS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_ifgs.InitHashTable(HASHSIZEIFGS);
}

CDB_IFGS::~CDB_IFGS()
{

}

void CDB_IFGS::Add(T_IFGS_K Key,T_IFGS_D& rData,CDB_ELEM* pElem)
{
	T_IFGS_D Data;
	BOOL bExist=m_ifgs.Lookup(Key, Data);
	m_ifgs.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_IFGS_CMD, Key);
}

BOOL CDB_IFGS::Del(T_IFGS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_ifgs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_IFGS_CMD, Key));
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_IFGS::Get(T_IFGS_K Key,T_IFGS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_ifgs->Lookup(Key, uTemp)) return FALSE;
	}
	return m_ifgs.Lookup(Key,rData);
}

int CDB_IFGS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ifgs->GetCount();
	}
	return m_ifgs.GetCount();
}

POSITION CDB_IFGS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_ifgs->GetStartPosition();
	}
	return m_ifgs.GetStartPosition();
}

void CDB_IFGS::GetNext(POSITION& rNextPosition,T_IFGS_K& rKey,T_IFGS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_ifgs->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_ifgs.Lookup(rKey, rData);
		return;
	}
	m_ifgs.GetNextAssoc(rNextPosition,rKey,rData);
}
