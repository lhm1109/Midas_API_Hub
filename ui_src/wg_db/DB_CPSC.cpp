#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CPSC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPSC::CDB_CPSC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cpsc.InitHashTable(HASHSIZECPSC);
}

CDB_CPSC::~CDB_CPSC()
{

}

void CDB_CPSC::Add(T_CPSC_K Key,T_CPSC_D& rData,CDB_ELEM* pElem)
{
	T_CPSC_D Data;
	BOOL bExist=m_cpsc.Lookup(Key, Data);
	m_cpsc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CPSC_CMD, Key);

	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CPSC_CMD, Key);
	}
}

BOOL CDB_CPSC::Del(T_CPSC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cpsc.RemoveKey(Key);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CPSC_CMD, Key));
		// Current Stage? Final Stage?? Stage DB? Update??.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CPSC_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPSC::Get(T_CPSC_K Key,T_CPSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cpsc.Lookup(Key,rData);
}

int CDB_CPSC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpsc.GetCount();
}

POSITION CDB_CPSC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpsc.GetStartPosition();
}

void CDB_CPSC::GetNext(POSITION& rNextPosition,T_CPSC_K& rKey,T_CPSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cpsc.GetNextAssoc(rNextPosition,rKey,rData);
}
