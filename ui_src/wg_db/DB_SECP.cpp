#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_SECP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SECP::CDB_SECP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_secp.InitHashTable(HASHSIZESECP);
}

CDB_SECP::~CDB_SECP()
{

}

void CDB_SECP::Add(T_SECP_K Key,T_SECP_D& rData,CDB_SECT* pSect)
{
	T_SECP_D Data;
	BOOL bExist=m_secp.Lookup(Key, Data);
	m_secp.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_SECP_CMD, Key);
}

BOOL CDB_SECP::Del(T_SECP_K Key,CDB_SECT* pSect)
{
	T_SECP_D Data;
	BOOL bExist=m_secp.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_secp.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pSect != NULL)VERIFY(pSect->DelListItem(Key, LT_SECP_CMD, Key));
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_SECP::Get(T_SECP_K Key,T_SECP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_secp.Lookup(Key,rData);
}

int CDB_SECP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_secp.GetCount();
}

POSITION CDB_SECP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_secp.GetStartPosition();
}

void CDB_SECP::GetNext(POSITION& rNextPosition,T_SECP_K& rKey,T_SECP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_secp.GetNextAssoc(rNextPosition,rKey,rData);
}
