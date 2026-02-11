#include "stdafx.h"

#include "DB_ELEM.h"
#include "DB_DSAU.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DSAU::CDB_DSAU()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dsau.InitHashTable(HASHSIZEDSAU);
}

CDB_DSAU::~CDB_DSAU()
{

}

void CDB_DSAU::Add(T_DSAU_K Key,T_DSAU_D& rData,CDB_ELEM* pElem)
{
	T_DSAU_D Data;
	BOOL bExist=m_dsau.Lookup(Key, Data);
	m_dsau.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_DSAU_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_DSAU_CMD, Key);
	}
}

BOOL CDB_DSAU::Del(T_DSAU_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_dsau.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_DSAU_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_DSAU_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DSAU::Get(T_DSAU_K Key,T_DSAU_D& rData)
{
	return m_dsau.Lookup(Key,rData);
}

int CDB_DSAU::GetCount()
{
	return m_dsau.GetCount();
}

POSITION CDB_DSAU::GetStart()
{
	return m_dsau.GetStartPosition();
}

void CDB_DSAU::GetNext(POSITION& rNextPosition,T_DSAU_K& rKey,T_DSAU_D& rData)
{
	m_dsau.GetNextAssoc(rNextPosition,rKey,rData);
}
