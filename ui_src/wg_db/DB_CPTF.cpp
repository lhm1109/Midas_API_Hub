#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CPTF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CPTF::CDB_CPTF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cptf.InitHashTable(HASHSIZECPTF);
}

CDB_CPTF::~CDB_CPTF()
{

}

void CDB_CPTF::Add(T_CPTF_K Key,T_CPTF_D& rData,CDB_ELEM* pElem)
{
	T_CPTF_D Data;
	BOOL bExist=m_cptf.Lookup(Key, Data);
	m_cptf.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CPTF_CMD, Key);
}

BOOL CDB_CPTF::Del(T_CPTF_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cptf.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_CPTF_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CPTF::Get(T_CPTF_K Key,T_CPTF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cptf.Lookup(Key,rData);
}

int CDB_CPTF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cptf.GetCount();
}

POSITION CDB_CPTF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cptf.GetStartPosition();
}

void CDB_CPTF::GetNext(POSITION& rNextPosition,T_CPTF_K& rKey,T_CPTF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cptf.GetNextAssoc(rNextPosition,rKey,rData);
}
