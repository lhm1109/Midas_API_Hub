#include "stdafx.h"
#include "DB_MNDC.h"

#include "DBDoc.h"
#include "DB_ELEM.h"
#include "StagCtrl.h"

CDB_MNDC::CDB_MNDC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mndc.InitHashTable(HASHSIZEMNDC);
}

CDB_MNDC::~CDB_MNDC()
{

}

void CDB_MNDC::Add(T_MNDC_K Key,T_MNDC_D& rData,CDB_ELEM* pElem)
{
	T_MNDC_D data;
	if(m_mndc.Lookup(Key, data))
	{
		if(pElem)
		{
			VERIFY(pElem->DelListItem(Key, LT_MNDC_CMD, Key));
		}
	}
	m_mndc.SetAt(Key, rData);
	if(pElem)	pElem->AddListItem(Key, LT_MNDC_CMD, Key);
}

BOOL CDB_MNDC::Del(T_MNDC_K Key,CDB_ELEM* pElem)
{
	T_MNDC_D Data;
	BOOL bExist = m_mndc.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret = m_mndc.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pElem)	VERIFY(pElem->DelListItem(Key, LT_MNDC_CMD, Key));
		}
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MNDC::Get(T_MNDC_K Key,T_MNDC_D& rData)
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mndc.Lookup(Key, rData);
}

POSITION CDB_MNDC::GetStart()
{
	//if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	return m_pDoc->m_pStagCtrl->m_mndc->GetStartPosition();
	//}
	return m_mndc.GetStartPosition();
}

void CDB_MNDC::GetNext(POSITION& rNextPosition, T_MNDC_K& rKey, T_MNDC_D& rData)
{
	m_mndc.GetNextAssoc(rNextPosition, rKey, rData);
}

int CDB_MNDC::GetCount()
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mndc.GetCount();
}
