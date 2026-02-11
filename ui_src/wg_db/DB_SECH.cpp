#include "stdafx.h"
#include "DB_SECH.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SECH::CDB_SECH()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SECH.InitHashTable(HASHSIZESECH);
}

CDB_SECH::~CDB_SECH()
{

}

void CDB_SECH::Add(T_SECH_K Key,T_SECH_D& rData,CDB_ELEM* pElem)
{
	T_SECH_D Data;
	BOOL bExist=m_SECH.Lookup(Key, Data);
	m_SECH.SetAt(Key,rData);
	if(!bExist && pElem)pElem->AddListItem(Key, LT_SECH_CMD, Key);
}

BOOL CDB_SECH::Del(T_SECH_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_SECH.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_SECH_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SECH::Get(T_SECH_K Key,T_SECH_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_SECH.Lookup(Key,rData);
}

int CDB_SECH::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SECH.GetCount();
}

POSITION CDB_SECH::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_SECH.GetStartPosition();
}

void CDB_SECH::GetNext(POSITION& rNextPosition,T_SECH_K& rKey,T_SECH_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_SECH.GetNextAssoc(rNextPosition,rKey,rData);
}
