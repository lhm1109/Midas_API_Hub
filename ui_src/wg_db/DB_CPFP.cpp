#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CPFP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPFP::CDB_CPFP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cpfp.InitHashTable(HASHSIZECPFP);
}

CDB_CPFP::~CDB_CPFP()
{

}

void CDB_CPFP::Add(T_CPFP_K Key,T_CPFP_D& rData,CDB_ELEM* pElem)
{
	T_CPFP_D Data;
	BOOL bExist=m_cpfp.Lookup(Key, Data);
	m_cpfp.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CPFP_CMD, Key);
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CPFP_CMD, Key);
	}
}

BOOL CDB_CPFP::Del(T_CPFP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cpfp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CPFP_CMD, Key));
		// Current Stage? Final Stage?? Stage DB? Update??.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CPFP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPFP::Get(T_CPFP_K Key,T_CPFP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cpfp.Lookup(Key,rData);
}

int CDB_CPFP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpfp.GetCount();
}

POSITION CDB_CPFP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cpfp.GetStartPosition();
}

void CDB_CPFP::GetNext(POSITION& rNextPosition,T_CPFP_K& rKey,T_CPFP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cpfp.GetNextAssoc(rNextPosition,rKey,rData);
}
