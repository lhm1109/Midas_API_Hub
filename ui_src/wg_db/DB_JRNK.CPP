#include "stdafx.h"
#include "DB_JRNK.h"
#include "DB_ELEM.h"
#include "StagCtrl.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_JRNK::CDB_JRNK()
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_JRNK.InitHashTable(HASHSIZEJRNK);
}

CDB_JRNK::~CDB_JRNK()
{
    
}

void CDB_JRNK::Add(T_JRNK_K Key,T_JRNK_D& rData,CDB_ELEM* pElem)
{
    T_JRNK_D Data;
    BOOL bExist=m_JRNK.Lookup(Key, Data);
    m_JRNK.SetAt(Key,rData);
    if(!bExist && pElem)pElem->AddListItem(Key, LT_JRNK_CMD, Key);
}

BOOL CDB_JRNK::Del(T_JRNK_K Key,CDB_ELEM* pElem)
{
    BOOL ret=m_JRNK.RemoveKey(Key);
    ASSERT(ret);
    if(ret && pElem)VERIFY(pElem->DelListItem(Key, LT_JRNK_CMD, Key));
    return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_JRNK::Get(T_JRNK_K Key,T_JRNK_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return FALSE;
    return m_JRNK.Lookup(Key,rData);
}

int CDB_JRNK::GetCount()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return 0;
    return m_JRNK.GetCount();
}

POSITION CDB_JRNK::GetStart()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return 0;
    return m_JRNK.GetStartPosition();
}

void CDB_JRNK::GetNext(POSITION& rNextPosition,T_JRNK_K& rKey,T_JRNK_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
    { 
        ASSERT(0);
        return; 
    }
    m_JRNK.GetNextAssoc(rNextPosition,rKey,rData);
}
