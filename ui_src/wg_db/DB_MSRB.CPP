#include "stdafx.h"
#include "DB_MSRB.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MSRB::CDB_MSRB()
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_msrb.InitHashTable(HASHSIZEMSRB);
}

CDB_MSRB::~CDB_MSRB()
{

}

void CDB_MSRB::Add(T_MSRB_D& rData)
{
    m_msrb.SetAt(1,rData);
}

BOOL CDB_MSRB::Del()
{
    T_MSRB_D Data;
    BOOL bExist=m_msrb.Lookup(1, Data);
    if(bExist)
    {
        BOOL ret=m_msrb.RemoveKey(1);
        ASSERT(ret);
        return ret;
    }
    return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_MSRB::Get(T_MSRB_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return FALSE;
    return m_msrb.Lookup(1,rData);
}

int CDB_MSRB::GetCount()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return 0;
    return m_msrb.GetCount();
}
