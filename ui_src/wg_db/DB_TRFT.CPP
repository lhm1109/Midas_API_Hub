#include "stdafx.h"
#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_TRFT.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_TRFT::CDB_TRFT()
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_trft.InitHashTable(HASHSIZETRFT);
}

CDB_TRFT::~CDB_TRFT()
{

}

void CDB_TRFT::Add(T_TRFT_K Key,T_TRFT_D& rData,CDB_ELEM* pElem)
{
    T_TRFT_D Data;
    BOOL bExist=m_trft.Lookup(Key, Data);
    m_trft.SetAt(Key,rData);
    if(!bExist)pElem->AddListItem(Key, LT_TRFT_CMD, Key);

    // Current Stage가 Final Stage이면 Stage DB를 Update한다.
    // 이미 있는 것이 아니면 새로 Active시킨다.
    if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
    {
        if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_TRFT_CMD, Key);
    }
}

BOOL CDB_TRFT::Del(T_TRFT_K Key,CDB_ELEM* pElem)
{
    BOOL ret=m_trft.RemoveKey(Key);
    ASSERT(ret);
    if(ret)
    {
        VERIFY(pElem->DelListItem(Key, LT_TRFT_CMD, Key));
        // Current Stage가 Final Stage이면 Stage DB를 Update한다.
        if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
            m_pDoc->m_pStagCtrl->DeactiveDesign(LT_TRFT_CMD, Key);
    }
    return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_TRFT::Get(T_TRFT_K Key,T_TRFT_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return FALSE;
        UINT uTemp;
        if (!m_pDoc->m_pStagCtrl->m_trft->Lookup(Key, uTemp)) return FALSE;
    }
    return m_trft.Lookup(Key,rData);
}

int CDB_TRFT::GetCount()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
        return m_pDoc->m_pStagCtrl->m_trft->GetCount();
    }
    return m_trft.GetCount();
}

POSITION CDB_TRFT::GetStart()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        if (!m_pDoc->m_pStagCtrl->IsFinalStag()) return 0;
        return m_pDoc->m_pStagCtrl->m_trft->GetStartPosition();
    }
    return m_trft.GetStartPosition();
}

void CDB_TRFT::GetNext(POSITION& rNextPosition,T_TRFT_K& rKey,T_TRFT_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        if (!m_pDoc->m_pStagCtrl->IsFinalStag()) { ASSERT(0); return; }
        UINT uTemp;
        m_pDoc->m_pStagCtrl->m_trft->GetNextAssoc(rNextPosition, rKey, uTemp);
        m_trft.Lookup(rKey, rData);
        return;
    }
    m_trft.GetNextAssoc(rNextPosition,rKey,rData);
}

