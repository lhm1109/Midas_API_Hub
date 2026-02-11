#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_EDMP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_EDMP::CDB_EDMP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_edmp.InitHashTable(HASHSIZEEDMP);
}

CDB_EDMP::~CDB_EDMP()
{

}

void CDB_EDMP::Add(T_EDMP_K Key,T_EDMP_D& rData,CDB_ELEM* pElem)
{
	T_EDMP_D Data;
	BOOL bExist=m_edmp.Lookup(Key, Data);
	m_edmp.SetAt(Key,rData);
	if(!bExist && pElem != NULL)pElem->AddListItem(Key, LT_EDMP_CMD, Key);
}

BOOL CDB_EDMP::Del(T_EDMP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_edmp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		if (pElem != NULL)VERIFY(pElem->DelListItem(Key, LT_EDMP_CMD, Key));
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_EDMP::Get(T_EDMP_K Key,T_EDMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_edmp->Lookup(Key, uTemp)) return FALSE;
	}
	return m_edmp.Lookup(Key,rData);
}

int CDB_EDMP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_edmp->GetCount();
	}
	return m_edmp.GetCount();
}

POSITION CDB_EDMP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		return m_pDoc->m_pStagCtrl->m_edmp->GetStartPosition();
	}
	return m_edmp.GetStartPosition();
}

void CDB_EDMP::GetNext(POSITION& rNextPosition,T_EDMP_K& rKey,T_EDMP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_edmp->GetNextAssoc(rNextPosition,rKey,uTemp);
		m_edmp.Lookup(rKey, rData);
		return;
	}
	m_edmp.GetNextAssoc(rNextPosition,rKey,rData);
}
