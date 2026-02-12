#include "stdafx.h"

#include "DB_ELEM.h"
#include "DB_DSAS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DSAS::CDB_DSAS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dsas.InitHashTable(HASHSIZEDSAS);
}

CDB_DSAS::~CDB_DSAS()
{

}

void CDB_DSAS::Add(T_DSAS_K Key,T_DSAS_D& rData,CDB_ELEM* pElem)
{
	T_DSAS_D Data;
	BOOL bExist=m_dsas.Lookup(Key, Data);
	m_dsas.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_DSAS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_DSAS_CMD, Key);
	}
}

BOOL CDB_DSAS::Del(T_DSAS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_dsas.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_DSAS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_DSAS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DSAS::Get(T_DSAS_K Key,T_DSAS_D& rData)
{
	return m_dsas.Lookup(Key,rData);
}

int CDB_DSAS::GetCount()
{
	return m_dsas.GetCount();
}

POSITION CDB_DSAS::GetStart()
{
	return m_dsas.GetStartPosition();
}

void CDB_DSAS::GetNext(POSITION& rNextPosition,T_DSAS_K& rKey,T_DSAS_D& rData)
{
	m_dsas.GetNextAssoc(rNextPosition,rKey,rData);
}
