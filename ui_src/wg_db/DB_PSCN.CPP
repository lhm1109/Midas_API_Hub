#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_PSCN.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_PSCN::CDB_PSCN()
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_pscn.InitHashTable(HASHSIZEPSCN);
}

CDB_PSCN::~CDB_PSCN()
{
    
}

void CDB_PSCN::Add(T_PSCN_K Key,T_PSCN_D& rData,CDB_ELEM* pElem)
{
    T_PSCN_D Data;
    BOOL bExist=m_pscn.Lookup(Key, Data);
    m_pscn.SetAt(Key,rData);
    if(!bExist)pElem->AddListItem(Key, LT_PSCN_CMD, Key);
    
    // Current Stage가 Final Stage이면 Stage DB를 Update한다.
    // 이미 있는 것이 아니면 새로 Active시킨다.
    if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
    {
        if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_PSCN_CMD, Key);
    }
}

BOOL CDB_PSCN::Del(T_PSCN_K Key,CDB_ELEM* pElem)
{
    BOOL ret=m_pscn.RemoveKey(Key);
    ASSERT(ret);
    if(ret)
    {
        VERIFY(pElem->DelListItem(Key, LT_PSCN_CMD, Key));
        // Current Stage가 Final Stage이면 Stage DB를 Update한다.
        if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
            m_pDoc->m_pStagCtrl->DeactiveDesign(LT_PSCN_CMD, Key);
    }
    return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_PSCN::Get(T_PSCN_K Key,T_PSCN_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
        UINT uTemp;
        if (!m_pDoc->m_pStagCtrl->m_pscn->Lookup(Key, uTemp)) return FALSE;
    }
    return m_pscn.Lookup(Key,rData);
}

int CDB_PSCN::GetCount()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
        return m_pDoc->m_pStagCtrl->m_pscn->GetCount();
    }
    return m_pscn.GetCount();
}

POSITION CDB_PSCN::GetStart()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
        return m_pDoc->m_pStagCtrl->m_pscn->GetStartPosition();
    }
    return m_pscn.GetStartPosition();
}

void CDB_PSCN::GetNext(POSITION& rNextPosition,T_PSCN_K& rKey,T_PSCN_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
        UINT uTemp;
        m_pDoc->m_pStagCtrl->m_pscn->GetNextAssoc(rNextPosition, rKey, uTemp);
        m_pscn.Lookup(rKey, rData);
        return;
    }
    m_pscn.GetNextAssoc(rNextPosition,rKey,rData);
}

