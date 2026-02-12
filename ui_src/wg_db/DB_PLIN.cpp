#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PLIN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PLIN::CDB_PLIN()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_plin.InitHashTable(HASHSIZEPLIN);
}

CDB_PLIN::~CDB_PLIN()
{

}

void CDB_PLIN::Add(T_PLIN_K Key,T_PLIN_D& rData,CDB_ELEM* pElem)
{
	T_PLIN_D Data;
	BOOL bExist=m_plin.Lookup(Key, Data);
	m_plin.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_PLIN_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PLIN_CMD, Key);
	}
}

BOOL CDB_PLIN::Del(T_PLIN_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_plin.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_PLIN_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PLIN_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PLIN::Get(T_PLIN_K Key,T_PLIN_D& rData)
{
	return m_plin.Lookup(Key,rData);
}

int CDB_PLIN::GetCount()
{
	return m_plin.GetCount();
}

POSITION CDB_PLIN::GetStart()
{
	return m_plin.GetStartPosition();
}

void CDB_PLIN::GetNext(POSITION& rNextPosition,T_PLIN_K& rKey,T_PLIN_D& rData)
{
	m_plin.GetNextAssoc(rNextPosition,rKey,rData);
}
