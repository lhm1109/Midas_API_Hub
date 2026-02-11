#include "stdafx.h"
#include "DB_ESEQ.h"
#include "DB_STLD.h"
#include "DB_STOR.h"
#include "DB_MATL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_ESEQ::CDB_ESEQ()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_eseq.InitHashTable(HASHSIZEESEQ);
}

CDB_ESEQ::~CDB_ESEQ()
{

}

void CDB_ESEQ::Add(T_ESEQ_D& rData,CDB_STLD* pStld)
{
	T_ESEQ_D Data;
	BOOL bExist=m_eseq.Lookup(1, Data);
	if(bExist)
	{
		if(Data.StldKey != rData.StldKey)
			VERIFY(pStld->DelListItem(Data.StldKey, LT_ESEQ_CMD, 1));
	}
	m_eseq.SetAt(1,rData);
	if(!bExist || Data.StldKey != rData.StldKey)
		pStld->AddListItem(rData.StldKey, LT_ESEQ_CMD, 1);
}

BOOL CDB_ESEQ::Del(CDB_STLD* pStld)
{
	T_ESEQ_D Data;
	BOOL bExist=m_eseq.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_eseq.RemoveKey(1);
		ASSERT(ret);
		if(ret)
		{
			VERIFY(pStld->DelListItem(Data.StldKey, LT_ESEQ_CMD, 1));
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_ESEQ::Get(T_ESEQ_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_eseq.Lookup(1,rData);
}

int CDB_ESEQ::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_eseq.GetCount();
}
