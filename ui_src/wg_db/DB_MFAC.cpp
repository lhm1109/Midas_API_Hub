#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_MFAC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MFAC::CDB_MFAC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mfac.InitHashTable(HASHSIZEMFAC);
}

CDB_MFAC::~CDB_MFAC()
{

}

void CDB_MFAC::Add(T_MFAC_K Key,T_MFAC_D& rData,CDB_ELEM* pElem)
{
	T_MFAC_D Data;
	BOOL bExist=m_mfac.Lookup(Key, Data);
	m_mfac.SetAt(Key,rData);
	if(!bExist) pElem->AddListItem(Key, LT_MFAC_CMD, Key);
}

BOOL CDB_MFAC::Del(T_MFAC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_mfac.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_MFAC_CMD, Key));
	}
	return ret;
}

//--------------------------------------------------------------------------
BOOL CDB_MFAC::Get(T_MFAC_K Key,T_MFAC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mfac.Lookup(Key,rData);
}

int CDB_MFAC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mfac.GetCount();
}

POSITION CDB_MFAC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mfac.GetStartPosition();
}

void CDB_MFAC::GetNext(POSITION& rNextPosition,T_MFAC_K& rKey,T_MFAC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_mfac.GetNextAssoc(rNextPosition,rKey,rData);
}
