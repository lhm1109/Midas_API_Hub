#include "stdafx.h"

#include "DB_ELEM.h"
#include "DB_IMEL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IMEL::CDB_IMEL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_imel.InitHashTable(HASHSIZEIMEL);
}

CDB_IMEL::~CDB_IMEL()
{

}

void CDB_IMEL::Add(T_IMEL_K Key,T_IMEL_D& rData,CDB_ELEM* pElem)
{
	T_IMEL_D Data;
	BOOL bExist=m_imel.Lookup(Key, Data);
	m_imel.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_IMEL_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_IMEL_CMD, Key);
	}
}

BOOL CDB_IMEL::Del(T_IMEL_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_imel.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_IMEL_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_IMEL_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_IMEL::Get(T_IMEL_K Key,T_IMEL_D& rData)
{
	return m_imel.Lookup(Key,rData);
}

int CDB_IMEL::GetCount()
{
	return m_imel.GetCount();
}

POSITION CDB_IMEL::GetStart()
{
	return m_imel.GetStartPosition();
}

void CDB_IMEL::GetNext(POSITION& rNextPosition,T_IMEL_K& rKey,T_IMEL_D& rData)
{
	m_imel.GetNextAssoc(rNextPosition,rKey,rData);
}
