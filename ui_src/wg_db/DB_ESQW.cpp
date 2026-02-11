#include "stdafx.h"
#include "DB_ESQW.h"
#include "DB_STLD.h"
#include "DB_STOR.h"
#include "DB_MATL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ESQW::CDB_ESQW()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_esqw.InitHashTable(HASHSIZEESQW);
}

CDB_ESQW::~CDB_ESQW()
{

}

void CDB_ESQW::Add(T_ESQW_D& rData,CDB_STLD* pStld)
{
	T_ESQW_D Data;
	BOOL bExist=m_esqw.Lookup(1, Data);
	if(bExist)
	{
		if(Data.StldKey != rData.StldKey)
			VERIFY(pStld->DelListItem(Data.StldKey, LT_ESQW_CMD, 1));
	}
	m_esqw.SetAt(1,rData);
	if(!bExist || Data.StldKey != rData.StldKey)
		pStld->AddListItem(rData.StldKey, LT_ESQW_CMD, 1);
}

BOOL CDB_ESQW::Del(CDB_STLD* pStld)
{
	T_ESQW_D Data;
	BOOL bExist=m_esqw.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_esqw.RemoveKey(1);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.StldKey, LT_ESQW_CMD, 1));
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_ESQW::Get(T_ESQW_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_esqw.Lookup(1,rData);
}

int CDB_ESQW::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_esqw.GetCount();
}
