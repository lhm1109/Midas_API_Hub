#include "stdafx.h"
#include "DB_IRST.h"
#include "DB_STOR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_IRST::CDB_IRST()
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_IRST.InitHashTable(HASHSIZEIRST);
}

CDB_IRST::~CDB_IRST()
{
    
}

void CDB_IRST::Add(T_IRST_K Key,T_IRST_D& rData,CDB_STOR* pStor)
{
    T_IRST_D Data;
    BOOL bExist=m_IRST.Lookup(Key, Data);
    m_IRST.SetAt(Key,rData);
    if(!bExist && pStor)pStor->AddListItem(Key, LT_IRST_CMD, Key);
}

BOOL CDB_IRST::Del(T_IRST_K Key,CDB_STOR* pStor)
{
    BOOL ret=m_IRST.RemoveKey(Key);
    ASSERT(ret);
    if(ret && pStor)VERIFY(pStor->DelListItem(Key, LT_IRST_CMD, Key));
    return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_IRST::Get(T_IRST_K Key,T_IRST_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return FALSE;
    return m_IRST.Lookup(Key,rData);
}

int CDB_IRST::GetCount()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return 0;
    return m_IRST.GetCount();
}

POSITION CDB_IRST::GetStart()
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
        return 0;
    return m_IRST.GetStartPosition();
}

void CDB_IRST::GetNext(POSITION& rNextPosition,T_IRST_K& rKey,T_IRST_D& rData)
{
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
    { 
        ASSERT(0);
        return; 
    }
    m_IRST.GetNextAssoc(rNextPosition,rKey,rData);
}
