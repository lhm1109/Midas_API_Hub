#include "stdafx.h"
#include "DB_FPSW.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_FPSW::CDB_FPSW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_fpsw.InitHashTable(HASHSIZEFPSW);
}

CDB_FPSW::~CDB_FPSW()
{

}

void CDB_FPSW::Add(T_FPSW_K Key,T_FPSW_D& rData)
{
	T_FPSW_D Data;
	BOOL bExist=m_fpsw.Lookup(Key, Data);
	m_fpsw.SetAt(Key,rData);  

	// PESC
	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
//   if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
//   {
//     if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_FPSW_CMD, Key);
//   }
}

BOOL CDB_FPSW::Del(T_FPSW_K Key)
{
	T_FPSW_D Data;
	BOOL bExist=m_fpsw.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_fpsw.RemoveKey(Key);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_FPSW::Get(T_FPSW_K Key,T_FPSW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_fpsw.Lookup(Key,rData);
}

int CDB_FPSW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fpsw.GetCount();
}

POSITION CDB_FPSW::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_fpsw.GetStartPosition();
}

void CDB_FPSW::GetNext(POSITION& rNextPosition,T_FPSW_K& rKey,T_FPSW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_fpsw.GetNextAssoc(rNextPosition,rKey,rData);
}