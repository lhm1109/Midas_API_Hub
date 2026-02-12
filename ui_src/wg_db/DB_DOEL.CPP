#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_DOEL.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_DOEL::CDB_DOEL()
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_doel.InitHashTable(HASHSIZEDOEL);
}

CDB_DOEL::~CDB_DOEL()
{

}

void CDB_DOEL::Add(T_DOEL_K Key,T_DOEL_D& rData,CDB_ELEM* pElem)
{
    T_DOEL_D Data;
    BOOL bExist=m_doel.Lookup(Key, Data);
    m_doel.SetAt(Key,rData);
    if(!bExist)pElem->AddListItem(Key, LT_DOEL_CMD, Key);
}

BOOL CDB_DOEL::Del(T_DOEL_K Key,CDB_ELEM* pElem)
{
    BOOL ret=m_doel.RemoveKey(Key);
    ASSERT(ret);
    if(ret)VERIFY(pElem->DelListItem(Key, LT_DOEL_CMD, Key));
    return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_DOEL::Get(T_DOEL_K Key,T_DOEL_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        //if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
        UINT uTemp;
        if (!m_pDoc->m_pStagCtrl->m_doel->Lookup(Key, uTemp)) return FALSE;
    }
    return m_doel.Lookup(Key,rData);
}

int CDB_DOEL::GetCount()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        //if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
        return m_pDoc->m_pStagCtrl->m_doel->GetCount();
    }
    return m_doel.GetCount();
}

POSITION CDB_DOEL::GetStart()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        //if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
        return m_pDoc->m_pStagCtrl->m_doel->GetStartPosition();
    }
    return m_doel.GetStartPosition();
}

void CDB_DOEL::GetNext(POSITION& rNextPosition,T_DOEL_K& rKey,T_DOEL_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        //if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
        UINT uTemp;
        m_pDoc->m_pStagCtrl->m_doel->GetNextAssoc(rNextPosition, rKey, uTemp);
        m_doel.Lookup(rKey, rData);
        return;
    }
    m_doel.GetNextAssoc(rNextPosition,rKey,rData);
}
