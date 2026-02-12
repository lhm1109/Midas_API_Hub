#include "stdafx.h"
#include "DB_STLD.h"
#include "DB_LESF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_LESF::CDB_LESF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_lesf.InitHashTable(HASHSIZELESF);
}

CDB_LESF::~CDB_LESF()
{

}

void CDB_LESF::Add(T_LESF_K Key,T_LESF_D& rData,CDB_STLD* pStld)
{
	T_LESF_D Data;
	BOOL bExist=m_lesf.Lookup(Key, Data);
	m_lesf.SetAt(Key,rData);
	if(!bExist)pStld->AddListItem(Key, LT_LESF_CMD, Key);
}

BOOL CDB_LESF::Del(T_LESF_K Key,CDB_STLD* pStld)
{
	T_LESF_D Data;
	BOOL bExist=m_lesf.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_lesf.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Key, LT_LESF_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//--------------------------------------------------------------------------
BOOL CDB_LESF::Get(T_LESF_K Key,T_LESF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_lesf.Lookup(Key,rData);
}

int CDB_LESF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_lesf.GetCount();
}

POSITION CDB_LESF::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_lesf.GetStartPosition();
}

void CDB_LESF::GetNext(POSITION& rNextPosition,T_LESF_K& rKey,T_LESF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_lesf.GetNextAssoc(rNextPosition,rKey,rData);
}
