#include "stdafx.h"
#include "DB_TAIF.h"

#include "DBDoc.h"
#include "DB_ELEM.h"
#include "StagCtrl.h"

CDB_TAIF::CDB_TAIF()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_taif.InitHashTable(HASHSIZETAIF);
}

CDB_TAIF::~CDB_TAIF()
{

}

void CDB_TAIF::Add(T_TAIF_D& rData)
{
	m_taif.SetAt(1, rData);
}

BOOL CDB_TAIF::Del()
{
	T_TAIF_D Data;
	BOOL bExist = m_taif.Lookup(1, Data);
	if (bExist)
	{
		BOOL ret = m_taif.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_TAIF::Get(T_TAIF_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_taif.Lookup(1, rData);
}

POSITION CDB_TAIF::GetStart()
{
	//if(m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
	//{
	//	return m_pDoc->m_pStagCtrl->m_taif->GetStartPosition();
	//}
	return m_taif.GetStartPosition();
}

void CDB_TAIF::GetNext(POSITION& rNextPosition, T_TAIF_K& rKey, T_TAIF_D& rData)
{
	m_taif.GetNextAssoc(rNextPosition, rKey, rData);
}

int CDB_TAIF::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_taif.GetCount();
}
