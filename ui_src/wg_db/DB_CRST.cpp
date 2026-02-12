#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_CRST.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CRST::CDB_CRST()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_crst.InitHashTable(HASHSIZECRST);
}

CDB_CRST::~CDB_CRST()
{

}

void CDB_CRST::Add(T_CRST_K Key,T_CRST_D& rData,CDB_ELEM* pElem)
{
	T_CRST_D Data;
	BOOL bExist=m_crst.Lookup(Key, Data);
	m_crst.SetAt(Key,rData);
	if(!bExist)pElem->AddListItem(Key, LT_CRST_CMD, Key);

	if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_CRST_CMD, Key);
	}
}

BOOL CDB_CRST::Del(T_CRST_K Key,CDB_ELEM* pElem)
{
	BOOL ret=m_crst.RemoveKey(Key);
	if(ret)
	{
		VERIFY(pElem->DelListItem(Key, LT_CRST_CMD, Key));
		// Current Stage? Final Stage?? Stage DB? Update??.
		if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
			m_pDoc->m_pStagCtrl->DeactiveDesign(LT_CRST_CMD, Key);
	}
	return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_CRST::Get(T_CRST_K Key,T_CRST_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_crst.Lookup(Key,rData);
}

int CDB_CRST::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_crst.GetCount();
}

POSITION CDB_CRST::GetStart()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_crst.GetStartPosition();
}

void CDB_CRST::GetNext(POSITION& rNextPosition,T_CRST_K& rKey,T_CRST_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
	{
		ASSERT(0);
		return;
	}
	m_crst.GetNextAssoc(rNextPosition,rKey,rData);
}
