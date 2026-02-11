#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SPHI.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPHI::CDB_SPHI()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SPHI.InitHashTable(HASHSIZESPHI);
}

CDB_SPHI::~CDB_SPHI()
{

}

void CDB_SPHI::Add(T_SPHI_K Key,T_SPHI_D& rData,CDB_ELEM* pElem)
{
	T_SPHI_D Data;
	BOOL bExist=m_SPHI.Lookup(Key, Data);
	m_SPHI.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SPHI_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SPHI_CMD, Key);
	}
}

BOOL CDB_SPHI::Del(T_SPHI_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_SPHI.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SPHI_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SPHI_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SPHI::Get(T_SPHI_K Key,T_SPHI_D& rData)
{
	return m_SPHI.Lookup(Key,rData);
}

int CDB_SPHI::GetCount()
{
	return m_SPHI.GetCount();
}

POSITION CDB_SPHI::GetStart()
{
	return m_SPHI.GetStartPosition();
}

void CDB_SPHI::GetNext(POSITION& rNextPosition,T_SPHI_K& rKey,T_SPHI_D& rData)
{
	m_SPHI.GetNextAssoc(rNextPosition,rKey,rData);
}
