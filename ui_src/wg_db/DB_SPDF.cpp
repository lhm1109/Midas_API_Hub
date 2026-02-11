#include "stdafx.h"

#include "DB_ELEM.h"
#include "DB_SPDF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SPDF::CDB_SPDF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_spdf.InitHashTable(HASHSIZESPDF);
}

CDB_SPDF::~CDB_SPDF()
{

}

void CDB_SPDF::Add(T_SPDF_K Key,T_SPDF_D& rData,CDB_ELEM* pElem)
{
	T_SPDF_D Data;
	BOOL bExist=m_spdf.Lookup(Key, Data);
	m_spdf.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SPDF_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SPDF_CMD, Key);
	}
}

BOOL CDB_SPDF::Del(T_SPDF_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_spdf.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SPDF_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SPDF_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SPDF::Get(T_SPDF_K Key,T_SPDF_D& rData)
{
	return m_spdf.Lookup(Key,rData);
}

int CDB_SPDF::GetCount()
{
	return m_spdf.GetCount();
}

POSITION CDB_SPDF::GetStart()
{
	return m_spdf.GetStartPosition();
}

void CDB_SPDF::GetNext(POSITION& rNextPosition,T_SPDF_K& rKey,T_SPDF_D& rData)
{
	m_spdf.GetNextAssoc(rNextPosition,rKey,rData);
}
