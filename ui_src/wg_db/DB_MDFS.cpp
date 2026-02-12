#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_MDFS.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MDFS::CDB_MDFS()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mdfs.InitHashTable(HASHSIZEMDFS);
}

CDB_MDFS::~CDB_MDFS()
{

}

void CDB_MDFS::Add(T_MDFS_K Key,T_MDFS_D& rData,CDB_ELEM* pElem)
{
	T_MDFS_D Data;
	BOOL bExist=m_mdfs.Lookup(Key, Data);
	m_mdfs.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_MDFS_CMD, Key);

	// Current Stage가 Final Stage이면 Stage DB를 Update한다.
	// 이미 있는 것이 아니면 새로 Active시킨다.
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_MDFS_CMD, Key);
	}
}

BOOL CDB_MDFS::Del(T_MDFS_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_mdfs.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_MDFS_CMD, Key));
		// Current Stage가 Final Stage이면 Stage DB를 Update한다.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_MDFS_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MDFS::Get(T_MDFS_K Key,T_MDFS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
		UINT uTemp;
		if (!m_pDoc->m_pStagCtrl->m_mdfs->Lookup(Key, uTemp)) return FALSE;
	}
	return m_mdfs.Lookup(Key,rData);
}

int CDB_MDFS::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mdfs->GetCount();
	}
	return m_mdfs.GetCount();
}

POSITION CDB_MDFS::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
		return m_pDoc->m_pStagCtrl->m_mdfs->GetStartPosition();
	}
	return m_mdfs.GetStartPosition();
}

void CDB_MDFS::GetNext(POSITION& rNextPosition,T_MDFS_K& rKey,T_MDFS_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	{
		if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
		UINT uTemp;
		m_pDoc->m_pStagCtrl->m_mdfs->GetNextAssoc(rNextPosition, rKey, uTemp);
		m_mdfs.Lookup(rKey, rData);
		return;
	}
	m_mdfs.GetNextAssoc(rNextPosition,rKey,rData);
}
