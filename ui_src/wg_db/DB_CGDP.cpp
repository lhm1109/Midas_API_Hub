#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CGDP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGDP::CDB_CGDP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgdp.InitHashTable(HASHSIZECGDP);
}

CDB_CGDP::~CDB_CGDP()
{

}

void CDB_CGDP::Add(T_CGDP_K Key,T_CGDP_D& rData,CDB_ELEM* pElem)
{
	T_CGDP_D Data;
	BOOL bExist=m_cgdp.Lookup(Key, Data);
	m_cgdp.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CGDP_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CGDP_CMD, Key);
	}
}

BOOL CDB_CGDP::Del(T_CGDP_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cgdp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CGDP_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CGDP_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGDP::Get(T_CGDP_K Key,T_CGDP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgdp.Lookup(Key,rData);
}

int CDB_CGDP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgdp.GetCount();
}

POSITION CDB_CGDP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgdp.GetStartPosition();
}

void CDB_CGDP::GetNext(POSITION& rNextPosition,T_CGDP_K& rKey,T_CGDP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgdp.GetNextAssoc(rNextPosition,rKey,rData);
}
