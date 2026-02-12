#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CGSC.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGSC::CDB_CGSC()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgsc.InitHashTable(HASHSIZECGSC);
}

CDB_CGSC::~CDB_CGSC()
{

}

void CDB_CGSC::Add(T_CGSC_K Key,T_CGSC_D& rData,CDB_ELEM* pElem)
{
	T_CGSC_D Data;
	BOOL bExist=m_cgsc.Lookup(Key, Data);
	m_cgsc.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CGSC_CMD, Key);
}

BOOL CDB_CGSC::Del(T_CGSC_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cgsc.RemoveKey(Key);
	ASSERT(ret);
	if(ret)VERIFY(pElem->DelListItem(Key, LT_CGSC_CMD, Key));
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGSC::Get(T_CGSC_K Key,T_CGSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgsc.Lookup(Key,rData);
}

int CDB_CGSC::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgsc.GetCount();
}

POSITION CDB_CGSC::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgsc.GetStartPosition();
}

void CDB_CGSC::GetNext(POSITION& rNextPosition,T_CGSC_K& rKey,T_CGSC_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgsc.GetNextAssoc(rNextPosition,rKey,rData);
}
