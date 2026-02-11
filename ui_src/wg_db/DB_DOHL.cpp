#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DOHL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DOHL::CDB_DOHL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dohl.InitHashTable(HASHSIZEDOHL);
}

CDB_DOHL::~CDB_DOHL()
{

}

void CDB_DOHL::Add(T_DOHL_K Key,T_DOHL_D& rData,CDB_ELEM* pElem)
{
	T_DOHL_D Data;
	BOOL bExist=m_dohl.Lookup(Key, Data);
	m_dohl.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_DOHL_CMD, Key);
	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_DOHL_CMD, Key);
	}
}

BOOL CDB_DOHL::Del(T_DOHL_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_dohl.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_DOHL_CMD, Key));
		// Current Stage? Final Stage?? Stage DB? Update??.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_DOHL_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DOHL::Get(T_DOHL_K Key,T_DOHL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_dohl.Lookup(Key,rData);
}

int CDB_DOHL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dohl.GetCount();
}

POSITION CDB_DOHL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_dohl.GetStartPosition();
}

void CDB_DOHL::GetNext(POSITION& rNextPosition,T_DOHL_K& rKey,T_DOHL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_dohl.GetNextAssoc(rNextPosition,rKey,rData);
}
