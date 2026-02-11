#include "stdafx.h"
#include "DB_BEMW.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_BEMW::CDB_BEMW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_bemw.InitHashTable(HASHSIZEBEMW);
}

CDB_BEMW::~CDB_BEMW()
{

}

void CDB_BEMW::Add(T_BEMW_K Key,T_BEMW_D& rData)
{
	T_BEMW_D Data;
	BOOL bExist=m_bemw.Lookup(Key, Data);
	m_bemw.SetAt(Key,rData);  

	// PESC
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
//   if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
//   {
//     if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_BEMW_CMD, Key);
//   }
}

BOOL CDB_BEMW::Del(T_BEMW_K Key)
{
	T_BEMW_D Data;
	BOOL bExist=m_bemw.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_bemw.RemoveKey(Key);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_BEMW::Get(T_BEMW_K Key,T_BEMW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_bemw.Lookup(Key,rData);
}

int CDB_BEMW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_bemw.GetCount();
}

POSITION CDB_BEMW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_bemw.GetStartPosition();
}

void CDB_BEMW::GetNext(POSITION& rNextPosition,T_BEMW_K& rKey,T_BEMW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_bemw.GetNextAssoc(rNextPosition,rKey,rData);
}