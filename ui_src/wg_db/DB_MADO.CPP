#include "stdafx.h"
#include "DB_MATL.h"
#include "DB_THIK.h"
#include "DB_MADO.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_MADO::CDB_MADO()
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_nStartNum=1;
    m_nLastNum=0;
    m_mado.InitHashTable(HASHSIZEMADO);
}

CDB_MADO::~CDB_MADO()
{
}

void CDB_MADO::Add(T_MADO_K Key,T_MADO_D& rData,CDB_MATL* pMatl,CDB_THIK* pThik)
{
    T_MADO_D Data;
    BOOL bExist=m_mado.Lookup(Key, Data);
    if(bExist)
    {
        if(pMatl != NULL)VERIFY(pMatl->DelListItem(Data.elmat, LT_MADO_CMD, Key));    
        if(pThik != NULL)VERIFY(pThik->DelListItem(Data.elpro, LT_MADO_CMD, Key));    
    }

    m_mado.SetAt(Key,rData);
    if(pMatl != NULL)pMatl->AddListItem(rData.elmat, LT_MADO_CMD, Key);
    if(pThik != NULL)pThik->AddListItem(rData.elpro, LT_MADO_CMD, Key);  

    if(Key == m_nStartNum)
    {
        T_MADO_D data;
        while(TRUE)
        {
            m_nStartNum++;
            if(!Get(m_nStartNum,data))break;
        }
    }
    if(Key > m_nLastNum)m_nLastNum=Key;

    /*// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
    // Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
    if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_MADO_CMD, Key);
    }*/
}

BOOL CDB_MADO::Del(T_MADO_K Key,CDB_MATL* pMatl,CDB_THIK* pThik)
{
    T_MADO_D Data;
    m_mado.Lookup(Key, Data);
    BOOL ret=m_mado.RemoveKey(Key);
    ASSERT(ret);
    if(ret)
    {
        if(pMatl != NULL)VERIFY(pMatl->DelListItem(Data.elmat, LT_MADO_CMD, Key));
        if(pThik != NULL)VERIFY(pThik->DelListItem(Data.elpro, LT_MADO_CMD, Key));
        
        if(Key < m_nStartNum)m_nStartNum=Key;
        if(Key==m_nLastNum)
        {
            T_MADO_K key;
            T_MADO_D data;
            if(Get(m_nLastNum-1,data))
            {
                m_nLastNum--;
            }
            else
            {
                m_nLastNum=0;
                POSITION pos=GetStart();
                while(pos != NULL)
                {
                    GetNext(pos,key,data);
                    if(key > m_nLastNum)m_nLastNum=key;
                }
            }
        }
        /*// Stage Mode에서도 Add/Del/Mod 될 수 있으므로
        // Current Stage가 Base Stage가 아니면 Stage DB를 Update한다.
        if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
        {
            m_pDoc->m_pStagCtrl->DeactiveEtc(LT_MADO_CMD, Key);
        }*/
    }
    return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_MADO::Get(T_MADO_K Key,T_MADO_D& rData)
{
    /*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        UINT uTemp;
        if (!m_pDoc->m_pStagCtrl->m_mado->Lookup(Key, uTemp)) return FALSE;
    }*/
    return m_mado.Lookup(Key,rData);
}

int CDB_MADO::GetCount()
{
    /*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        return m_pDoc->m_pStagCtrl->m_mado->GetCount();
    }*/
    return m_mado.GetCount();
}

POSITION CDB_MADO::GetStart()
{
    /*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        return m_pDoc->m_pStagCtrl->m_mado->GetStartPosition();
    }*/
    return m_mado.GetStartPosition();
}

void CDB_MADO::GetNext(POSITION& rNextPosition,T_MADO_K& rKey,T_MADO_D& rData)
{
    /*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        UINT uTemp;
        m_pDoc->m_pStagCtrl->m_mado->GetNextAssoc(rNextPosition, rKey, uTemp);
        m_mado.Lookup(rKey, rData);
        return;
    }*/
    m_mado.GetNextAssoc(rNextPosition,rKey,rData);
}
