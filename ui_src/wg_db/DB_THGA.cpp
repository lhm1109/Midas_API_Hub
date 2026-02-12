#include "stdafx.h"
#include "DB_THIS.h"
#include "DB_THGA.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_THGA::CDB_THGA()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_thga.InitHashTable(HASHSIZETHGA);
}

CDB_THGA::~CDB_THGA()
{

}

void CDB_THGA::Add(T_THGA_K Key,T_THGA_D& rData,CDB_THIS* pThis)
{
	T_THGA_D Data;
	BOOL bExist=m_thga.Lookup(Key, Data);
	m_thga.SetAt(Key,rData);
	if(!bExist)pThis->AddListItem(Key, LT_THGA_CMD, Key);
}

BOOL CDB_THGA::Del(T_THGA_K Key,CDB_THIS* pThis)
{
	T_THGA_D Data;
	BOOL bExist=m_thga.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_thga.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pThis->DelListItem(Key, LT_THGA_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_THGA::Get(T_THGA_K Key,T_THGA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_thga.Lookup(Key,rData);
}

int CDB_THGA::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thga.GetCount();
}

POSITION CDB_THGA::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_thga.GetStartPosition();
}

void CDB_THGA::GetNext(POSITION& rNextPosition,T_THGA_K& rKey,T_THGA_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_thga.GetNextAssoc(rNextPosition,rKey,rData);
}

