#include "stdafx.h"
#include "DB_SAFM.h"

#include "DBDoc.h"
#include "DB_ELEM.h"
#include "StagCtrl.h"

CDB_SAFM::CDB_SAFM()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_safm.InitHashTable(HASHSIZESAFM);
}

CDB_SAFM::~CDB_SAFM()
{

}

void CDB_SAFM::Add(T_SAFM_K Key,T_SAFM_D& rData,CDB_ELEM* pElem)
{
	T_SAFM_D data;
	if(m_safm.Lookup(Key, data))
	{
		if(pElem)
		{
			VERIFY(pElem->DelListItem(Key, LT_SAFM_CMD, Key));
		}
	}
	m_safm.SetAt(Key, rData);
	if(pElem)	pElem->AddListItem(Key, LT_SAFM_CMD, Key);
}

BOOL CDB_SAFM::Del(T_SAFM_K Key,CDB_ELEM* pElem)
{
	T_SAFM_D Data;
	BOOL bExist = m_safm.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret = m_safm.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pElem)	VERIFY(pElem->DelListItem(Key, LT_SAFM_CMD, Key));
		}
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SAFM::Get(T_SAFM_K Key,T_SAFM_D& rData)
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_safm.Lookup(Key, rData);
}

POSITION CDB_SAFM::GetStart()
{
	//if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	return m_pDoc->m_pStagCtrl->m_safm->GetStartPosition();
	//}
	return m_safm.GetStartPosition();
}

void CDB_SAFM::GetNext(POSITION& rNextPosition, T_SAFM_K& rKey, T_SAFM_D& rData)
{
	m_safm.GetNextAssoc(rNextPosition, rKey, rData);
}

int CDB_SAFM::GetCount()
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_safm.GetCount();
}
