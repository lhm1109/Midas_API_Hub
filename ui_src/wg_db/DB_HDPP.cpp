#include "stdafx.h"
#include "DB_SECT.h"
#include "DB_HDPP.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_HDPP::CDB_HDPP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_hdpp.InitHashTable(HASHSIZEHDPP);
}

CDB_HDPP::~CDB_HDPP()
{

}

void CDB_HDPP::Add(T_HDPP_K Key,T_HDPP_D& rData,CDB_SECT* pSect)
{
	T_HDPP_D Data;
	BOOL bExist=m_hdpp.Lookup(Key, Data);
	m_hdpp.SetAt(Key,rData);
	if(!bExist)pSect->AddListItem(Key, LT_HDPP_CMD, Key);
}

BOOL CDB_HDPP::Del(T_HDPP_K Key,CDB_SECT* pSect)
{
	BOOL ret=m_hdpp.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pSect->DelListItem(Key, LT_HDPP_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_HDPP::Get(T_HDPP_K Key,T_HDPP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_hdpp.Lookup(Key,rData);
}

int CDB_HDPP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hdpp.GetCount();
}

POSITION CDB_HDPP::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_hdpp.GetStartPosition();
}

void CDB_HDPP::GetNext(POSITION& rNextPosition,T_HDPP_K& rKey,T_HDPP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_hdpp.GetNextAssoc(rNextPosition,rKey,rData);
}
