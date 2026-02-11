#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_UCCF.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_UCCF::CDB_UCCF()
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_uccf.InitHashTable(HASHSIZEUCCF);
}

CDB_UCCF::~CDB_UCCF()
{
    
}

void CDB_UCCF::Add(T_UCCF_K Key,T_UCCF_D& rData,CDB_ELEM* pElem)
{
    T_UCCF_D Data;
    BOOL bExist=m_uccf.Lookup(Key, Data);
    m_uccf.SetAt(Key,rData);  
    if(!bExist)pElem->AddListItem(Key, LT_UCCF_CMD, Key);

    // Current Stage가 Final Stage이면 Stage DB를 Update한다.
    // 이미 있는 것이 아니면 새로 Active시킨다.
    if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
    {
        if (!bExist) m_pDoc->m_pStagCtrl->ActiveDesign(LT_UCCF_CMD, Key);
    }
}

BOOL CDB_UCCF::Del(T_UCCF_K Key,CDB_ELEM* pElem)
{
    BOOL ret=m_uccf.RemoveKey(Key);
    ASSERT(ret);
    if(ret)
    {
        VERIFY(pElem->DelListItem(Key, LT_UCCF_CMD, Key));
        // Current Stage가 Final Stage이면 Stage DB를 Update한다.
        if (m_pDoc && m_pDoc->m_pStagCtrl->IsFinalStag())
            m_pDoc->m_pStagCtrl->DeactiveDesign(LT_UCCF_CMD, Key);
    }
    return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_UCCF::Get(T_UCCF_K Key,T_UCCF_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return FALSE;
    return m_uccf.Lookup(Key,rData);
}

int CDB_UCCF::GetCount()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return 0;
    return m_uccf.GetCount();
}

POSITION CDB_UCCF::GetStart()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return 0;
    return m_uccf.GetStartPosition();
}

void CDB_UCCF::GetNext(POSITION& rNextPosition,T_UCCF_K& rKey,T_UCCF_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
    {
        ASSERT(0);
        return;
    }
    m_uccf.GetNextAssoc(rNextPosition,rKey,rData);
}

