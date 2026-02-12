#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_REBB.h"
#include "DB_REBC.h"
#include "DB_REBR.h"
#include "DB_RBMW.h"
#include "DB_RBST.h"
#include "DB_MREB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MREB::CDB_MREB()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_mreb.InitHashTable(HASHSIZEMREB);
}

CDB_MREB::~CDB_MREB()
{

}

void CDB_MREB::Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem,CDB_REBB* pRebb)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	m_mreb.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_MREB_CMD, Key);
	
	if(bExist) pRebb->DelListItem(Data.RebbK, LT_MREB_CMD, Key);
	pRebb->AddListItem(rData.RebbK, LT_MREB_CMD, Key);
}

void CDB_MREB::Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem,CDB_REBC* pRebc)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	m_mreb.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_MREB_CMD, Key);
	
	if(bExist) pRebc->DelListItem(Data.RebcK, LT_MREB_CMD, Key);
	pRebc->AddListItem(rData.RebcK, LT_MREB_CMD, Key);
}

void CDB_MREB::Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem,CDB_REBR* pRebr)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	m_mreb.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_MREB_CMD, Key);
	
	if(bExist) pRebr->DelListItem(Data.RebrK, LT_MREB_CMD, Key);
	pRebr->AddListItem(rData.RebrK, LT_MREB_CMD, Key);
}

void CDB_MREB::Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem,CDB_RBMW* pRbmw)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	m_mreb.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_MREB_CMD, Key);
	
	if(bExist) pRbmw->DelListItem(Data.RbmwK, LT_MREB_CMD, Key);
	pRbmw->AddListItem(rData.RbmwK, LT_MREB_CMD, Key);
}

void CDB_MREB::Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	m_mreb.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_MREB_CMD, Key);  
}

void CDB_MREB::Add(T_MREB_K Key,T_MREB_D& rData,CDB_ELEM* pElem,CDB_RBST* pRbst)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	m_mreb.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_MREB_CMD, Key);

	for(int i=0; i<Data.aRbstK.GetSize(); i++)
	{
		if(bExist) pRbst->DelListItem(Data.aRbstK[i], LT_MREB_CMD, Key);
		pRbst->AddListItem(rData.aRbstK[i], LT_MREB_CMD, Key);
	}
}

BOOL CDB_MREB::Del(T_MREB_K Key,CDB_ELEM* pElem,CDB_REBB* pRebb)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	ASSERT(bExist);
	BOOL ret=m_mreb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_MREB_CMD, Key));
	if(ret)VERIFY(pRebb->DelListItem(Data.RebbK, LT_MREB_CMD, Key));
	return ret;
}

BOOL CDB_MREB::Del(T_MREB_K Key,CDB_ELEM* pElem,CDB_REBC* pRebc)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	ASSERT(bExist);
	BOOL ret=m_mreb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_MREB_CMD, Key));
	if(ret)VERIFY(pRebc->DelListItem(Data.RebcK, LT_MREB_CMD, Key));
	return ret;
}

BOOL CDB_MREB::Del(T_MREB_K Key,CDB_ELEM* pElem,CDB_REBR* pRebr)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	ASSERT(bExist);
	BOOL ret=m_mreb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_MREB_CMD, Key));
	if(ret)VERIFY(pRebr->DelListItem(Data.RebrK, LT_MREB_CMD, Key));
	return ret;
}

BOOL CDB_MREB::Del(T_MREB_K Key,CDB_ELEM* pElem,CDB_RBMW* pRbmw)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	ASSERT(bExist);
	BOOL ret=m_mreb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_MREB_CMD, Key));
	if(ret)VERIFY(pRbmw->DelListItem(Data.RbmwK, LT_MREB_CMD, Key));
	return ret;
}

BOOL CDB_MREB::Del(T_MREB_K Key,CDB_ELEM* pElem)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	ASSERT(bExist);
	BOOL ret=m_mreb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_MREB_CMD, Key));
	return ret;
}

BOOL CDB_MREB::Del(T_MREB_K Key,CDB_ELEM* pElem,CDB_RBST* pRbst)
{
	T_MREB_D Data;
	BOOL bExist=m_mreb.Lookup(Key, Data);
	ASSERT(bExist);
	BOOL ret=m_mreb.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_MREB_CMD, Key));
		for(int i=0; i<Data.aRbstK.GetSize(); i++)
		{
			VERIFY(pRbst->DelListItem(Data.aRbstK[i], LT_MREB_CMD, Key));
		}
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MREB::Get(T_MREB_K Key,T_MREB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_mreb.Lookup(Key,rData);
}

int CDB_MREB::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mreb.GetCount();
}

POSITION CDB_MREB::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_mreb.GetStartPosition();
}

void CDB_MREB::GetNext(POSITION& rNextPosition,T_MREB_K& rKey,T_MREB_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_mreb.GetNextAssoc(rNextPosition,rKey,rData);
}
