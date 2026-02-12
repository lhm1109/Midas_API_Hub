#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CGLV.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGLV::CDB_CGLV()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cglv.InitHashTable(HASHSIZECGLV);
}

CDB_CGLV::~CDB_CGLV()
{

}

void CDB_CGLV::Add(T_CGLV_K Key,T_CGLV_D& rData)
{
	T_CGLV_D Data;
	BOOL bExist=m_cglv.Lookup(Key, Data);
	m_cglv.SetAt(Key,rData);
	//if(!bExist)pElem->AddListItem(Key, LT_CGLV_CMD, Key);
}

BOOL CDB_CGLV::Del(T_CGLV_K Key)
{
	BOOL ret=m_cglv.RemoveKey(Key);
	ASSERT(ret);
	//if(ret)VERIFY(pElem->DelListItem(Key, LT_CGLV_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGLV::Get(T_CGLV_K Key,T_CGLV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cglv.Lookup(Key,rData);
}

int CDB_CGLV::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cglv.GetCount();
}

POSITION CDB_CGLV::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cglv.GetStartPosition();
}

void CDB_CGLV::GetNext(POSITION& rNextPosition,T_CGLV_K& rKey,T_CGLV_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cglv.GetNextAssoc(rNextPosition,rKey,rData);
}
