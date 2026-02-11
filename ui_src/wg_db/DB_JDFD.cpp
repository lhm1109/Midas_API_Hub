#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_JDFD.h"
#include "StagCtrl.h"

#include "DBDoc.h"

CDB_JDFD::CDB_JDFD()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_jdfd.InitHashTable(HASHSIZEJDFD);
}

CDB_JDFD::~CDB_JDFD()
{

}

void CDB_JDFD::Add(T_JDFD_K Key,T_JDFD_D& rData,CDB_STLD* pStor)
{
	T_JDFD_D Data;
	BOOL bExist=m_jdfd.Lookup(Key.keymap, Data);
	m_jdfd.SetAt(Key.keymap,rData);
	if(!bExist)pStor->AddListItem(Key.key.entity, LT_JDFD_CMD, Key.keymap);
}

BOOL CDB_JDFD::Del(T_JDFD_K Key,CDB_STLD* pStor)
{
	T_JDFD_D Data;
	BOOL bExist=m_jdfd.Lookup(Key.keymap, Data);
	if(bExist)
	{
		BOOL ret=m_jdfd.RemoveKey(Key.keymap);
		ASSERT(ret);
		if(ret)VERIFY(pStor->DelListItem(Key.key.entity, LT_JDFD_CMD, Key.keymap));
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_JDFD::Get(T_JDFD_K Key,T_JDFD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_jdfd.Lookup(Key.keymap,rData);
}

int CDB_JDFD::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_jdfd.GetCount();
}

POSITION CDB_JDFD::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_jdfd.GetStartPosition();
}

void CDB_JDFD::GetNext(POSITION& rNextPosition,T_JDFD_K& rKey,T_JDFD_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_jdfd.GetNextAssoc(rNextPosition,rKey.keymap,rData);
}
