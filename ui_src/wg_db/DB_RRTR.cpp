#include "stdafx.h"
#include "DB_RRTR.h"

#include "DBDoc.h"
#include "DB_ELEM.h"
#include "StagCtrl.h"

CDB_RRTR::CDB_RRTR()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_rrtr.InitHashTable(HASHSIZERRTR);
}

CDB_RRTR::~CDB_RRTR()
{

}

void CDB_RRTR::Add(T_RRTR_K Key,T_RRTR_D& rData,CDB_ELEM* pElem)
{
	T_RRTR_D data;
	if(m_rrtr.Lookup(Key, data))
	{
		if(pElem)
		{
			VERIFY(pElem->DelListItem(Key, LT_RRTR_CMD, Key));
		}
	}
	m_rrtr.SetAt(Key, rData);
	if(pElem)	pElem->AddListItem(Key, LT_RRTR_CMD, Key);
}

BOOL CDB_RRTR::Del(T_RRTR_K Key,CDB_ELEM* pElem)
{
	T_RRTR_D Data;
	BOOL bExist = m_rrtr.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret = m_rrtr.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pElem)	VERIFY(pElem->DelListItem(Key, LT_RRTR_CMD, Key));
		}
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_RRTR::Get(T_RRTR_K Key,T_RRTR_D& rData)
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_rrtr.Lookup(Key, rData);
}

POSITION CDB_RRTR::GetStart()
{
	//if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	return m_pDoc->m_pStagCtrl->m_rrtr->GetStartPosition();
	//}
	return m_rrtr.GetStartPosition();
}

void CDB_RRTR::GetNext(POSITION& rNextPosition, T_RRTR_K& rKey, T_RRTR_D& rData)
{
	m_rrtr.GetNextAssoc(rNextPosition, rKey, rData);
}

int CDB_RRTR::GetCount()
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_rrtr.GetCount();
}
