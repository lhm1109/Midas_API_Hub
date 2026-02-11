#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SECTCLASSIFY.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SECTCLASSIFY::CDB_SECTCLASSIFY()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SECTCLASSIFY.InitHashTable(HASHSIZESECTCLASSIFY);
}

CDB_SECTCLASSIFY::~CDB_SECTCLASSIFY()
{

}

void CDB_SECTCLASSIFY::Add(T_SECTCLASSIFY_K Key,T_SECTCLASSIFY_D& rData,CDB_ELEM* pElem)
{
	T_SECTCLASSIFY_D Data;
	BOOL bExist=m_SECTCLASSIFY.Lookup(Key, Data);
	m_SECTCLASSIFY.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_SETY_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_SETY_CMD, Key);
	}
}

BOOL CDB_SECTCLASSIFY::Del(T_SECTCLASSIFY_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_SECTCLASSIFY.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_SETY_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_SETY_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SECTCLASSIFY::Get(T_SECTCLASSIFY_K Key,T_SECTCLASSIFY_D& rData)
{
	return m_SECTCLASSIFY.Lookup(Key,rData);
}

int CDB_SECTCLASSIFY::GetCount()
{
	return m_SECTCLASSIFY.GetCount();
}

POSITION CDB_SECTCLASSIFY::GetStart()
{
	return m_SECTCLASSIFY.GetStartPosition();
}

void CDB_SECTCLASSIFY::GetNext(POSITION& rNextPosition,T_SECTCLASSIFY_K& rKey,T_SECTCLASSIFY_D& rData)
{
	m_SECTCLASSIFY.GetNextAssoc(rNextPosition,rKey,rData);
}
