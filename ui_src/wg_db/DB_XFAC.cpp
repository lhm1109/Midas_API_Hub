#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_XFAC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_XFAC::CDB_XFAC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_xfac.InitHashTable(HASHSIZEXFAC);
}

CDB_XFAC::~CDB_XFAC()
{

}

void CDB_XFAC::Add(T_XFAC_K Key,T_XFAC_D& rData,CDB_ELEM* pElem)
{
	T_XFAC_D Data;
	BOOL bExist=m_xfac.Lookup(Key, Data);
	m_xfac.SetAt(Key,rData);
	if(!bExist) pElem->AddListItem(Key, LT_XFAC_CMD, Key);
}

BOOL CDB_XFAC::Del(T_XFAC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_xfac.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_XFAC_CMD, Key));
	}
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_XFAC::Get(T_XFAC_K Key,T_XFAC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_xfac.Lookup(Key,rData);
}

int CDB_XFAC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_xfac.GetCount();
}

POSITION CDB_XFAC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_xfac.GetStartPosition();
}

void CDB_XFAC::GetNext(POSITION& rNextPosition,T_XFAC_K& rKey,T_XFAC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_xfac.GetNextAssoc(rNextPosition,rKey,rData);
}
