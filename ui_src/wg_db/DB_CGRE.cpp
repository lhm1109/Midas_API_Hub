#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CGRE.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CGRE::CDB_CGRE()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_cgre.InitHashTable(HASHSIZECGRE);
}

CDB_CGRE::~CDB_CGRE()
{

}

void CDB_CGRE::Add(T_CGRE_K Key,T_CGRE_D& rData,CDB_ELEM* pElem)
{
	T_CGRE_D Data;
	BOOL bExist=m_cgre.Lookup(Key, Data);
	m_cgre.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CGRE_CMD, Key);

	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CGRE_CMD, Key);
	}
}

BOOL CDB_CGRE::Del(T_CGRE_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_cgre.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CGRE_CMD, Key));
		// Current Stage? Final Stage?? Stage DB? Update??.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CGRE_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CGRE::Get(T_CGRE_K Key,T_CGRE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_cgre.Lookup(Key,rData);
}

int CDB_CGRE::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgre.GetCount();
}

POSITION CDB_CGRE::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_cgre.GetStartPosition();
}

void CDB_CGRE::GetNext(POSITION& rNextPosition,T_CGRE_K& rKey,T_CGRE_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_cgre.GetNextAssoc(rNextPosition,rKey,rData);
}
