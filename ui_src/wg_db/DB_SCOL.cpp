#include "stdafx.h"
#include "DB_SCOL.h"
#include "DB_ELEM.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SCOL::CDB_SCOL()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_scol.InitHashTable(HASHSIZESCOL);
}

CDB_SCOL::~CDB_SCOL()
{

}

void CDB_SCOL::Add(T_SCOL_K Key,T_SCOL_D& rData,CDB_ELEM* pElem)
{
	T_SCOL_D Data;
	BOOL bExist=m_scol.Lookup(Key, Data);
	m_scol.SetAt(Key,rData);
	if(!bExist && pElem)pElem->AddListItem(Key, LT_SCOL_CMD, Key);
}

BOOL CDB_SCOL::Del(T_SCOL_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_scol.RemoveKey(Key);
	ASSERT(ret);
	if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_SCOL_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SCOL::Get(T_SCOL_K Key,T_SCOL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_scol.Lookup(Key,rData);
}

int CDB_SCOL::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_scol.GetCount();
}

POSITION CDB_SCOL::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_scol.GetStartPosition();
}

void CDB_SCOL::GetNext(POSITION& rNextPosition,T_SCOL_K& rKey,T_SCOL_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{ 
		ASSERT(0);
		return; 
	}
	m_scol.GetNextAssoc(rNextPosition,rKey,rData);
}
