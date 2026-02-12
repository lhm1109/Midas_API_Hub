#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SRSC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SRSC::CDB_SRSC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_srsc.InitHashTable(HASHSIZESRSC);
}

CDB_SRSC::~CDB_SRSC()
{

}

void CDB_SRSC::Add(T_SRSC_K Key, T_SRSC_D& rData, CDB_ELEM* pElem)
{
	T_SRSC_D Data;
	BOOL bExist = m_srsc.Lookup(Key, Data);
	m_srsc.SetAt(Key, rData);
	if (!bExist)pElem->AddListItem(Key, LT_SRSC_CMD, Key);
}

BOOL CDB_SRSC::Del(T_SRSC_K Key, CDB_ELEM* pElem)
{
	BOOL ret = m_srsc.RemoveKey(Key);
	ASSERT(ret);
	if (ret)VERIFY(pElem->DelListItem(Key, LT_SRSC_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SRSC::Get(T_SRSC_K Key, T_SRSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_srsc.Lookup(Key, rData);
}

int CDB_SRSC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srsc.GetCount();
}

POSITION CDB_SRSC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_srsc.GetStartPosition();
}

void CDB_SRSC::GetNext(POSITION& rNextPosition, T_SRSC_K& rKey, T_SRSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_srsc.GetNextAssoc(rNextPosition, rKey, rData);
}
