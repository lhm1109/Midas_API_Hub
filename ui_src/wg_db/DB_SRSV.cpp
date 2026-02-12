#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SRSV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SRSV::CDB_SRSV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_srsv.InitHashTable(HASHSIZESRSV);
}

CDB_SRSV::~CDB_SRSV()
{

}

void CDB_SRSV::Add(T_SRSV_K Key, T_SRSV_D& rData/*, CDB_ELEM* pElem*/)
{
	T_SRSV_D Data;
	BOOL bExist = m_srsv.Lookup(Key, Data);
	m_srsv.SetAt(Key, rData);
	//if (!bExist)pElem->AddListItem(Key, LT_SRSV_CMD, Key);
}

BOOL CDB_SRSV::Del(T_SRSV_K Key/*, CDB_ELEM* pElem*/)
{
	BOOL ret = m_srsv.RemoveKey(Key);
	ASSERT(ret);
	//if (ret)VERIFY(pElem->DelListItem(Key, LT_SRSV_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SRSV::Get(T_SRSV_K Key, T_SRSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_srsv.Lookup(Key, rData);
}

int CDB_SRSV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srsv.GetCount();
}

POSITION CDB_SRSV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srsv.GetStartPosition();
}

void CDB_SRSV::GetNext(POSITION& rNextPosition, T_SRSV_K& rKey, T_SRSV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_srsv.GetNextAssoc(rNextPosition, rKey, rData);
}
