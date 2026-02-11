#include "stdafx.h"
#include "DB_INDF.h"

#include "DBDoc.h"
#include "DB_ELEM.h"
#include "StagCtrl.h"

CDB_INDF::CDB_INDF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_indf.InitHashTable(HASHSIZEINDF);
}

CDB_INDF::~CDB_INDF()
{

}

void CDB_INDF::Add(T_INDF_K Key,T_INDF_D& rData,CDB_ELEM* pElem)
{
	T_INDF_D data;
	if(m_indf.Lookup(Key, data))
	{
		if(pElem)
		{
			VERIFY(pElem->DelListItem(Key, LT_INDF_CMD, Key));
		}
	}
	m_indf.SetAt(Key, rData);
	if(pElem)	pElem->AddListItem(Key, LT_INDF_CMD, Key);
}

BOOL CDB_INDF::Del(T_INDF_K Key,CDB_ELEM* pElem)
{
	T_INDF_D Data;
	BOOL bExist = m_indf.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret = m_indf.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			if(pElem)	VERIFY(pElem->DelListItem(Key, LT_INDF_CMD, Key));
		}
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_INDF::Get(T_INDF_K Key,T_INDF_D& rData)
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_indf.Lookup(Key, rData);
}

POSITION CDB_INDF::GetStart()
{
	//if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	return m_pDoc->m_pStagCtrl->m_indf->GetStartPosition();
	//}
	return m_indf.GetStartPosition();
}

void CDB_INDF::GetNext(POSITION& rNextPosition, T_INDF_K& rKey, T_INDF_D& rData)
{
	m_indf.GetNextAssoc(rNextPosition, rKey, rData);
}

int CDB_INDF::GetCount()
{
	if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_indf.GetCount();
}
