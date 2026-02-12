#include "stdafx.h"
#include "DB_MADO.h"
#include "DB_SBDO.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_SBDO::CDB_SBDO()
{
    m_pDoc = CDBDoc::GetDocPoint();
    m_nStartNum=1;
    m_nLastNum=0;
    m_sbdo.InitHashTable(HASHSIZESBDO);
}

CDB_SBDO::~CDB_SBDO()
{
}

void CDB_SBDO::Add(T_SBDO_K Key,T_SBDO_D& rData,CDB_MADO* pMado)
{
    T_SBDO_D Data;
    BOOL bExist=m_sbdo.Lookup(Key, Data);
    if(bExist)
    {
        //if(pMado != NULL)VERIFY(pMado->DelListItem(Data.KeyMado, LT_SBDO_CMD, Key));    
    }

    m_sbdo.SetAt(Key,rData);
    //if(pMado != NULL)pMado->AddListItem(rData.KeyMado, LT_SBDO_CMD, Key);  

    if(Key == m_nStartNum)
    {
        T_SBDO_D data;
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
        if (!bExist) m_pDoc->m_pStagCtrl->ActiveEtc(LT_SBDO_CMD, Key);
    }*/
}

BOOL CDB_SBDO::Del(T_SBDO_K Key,CDB_MADO* pMado)
{
    T_SBDO_D Data;
    m_sbdo.Lookup(Key, Data);
    BOOL ret=m_sbdo.RemoveKey(Key);
    ASSERT(ret);
    if(ret)
    {
        //if(pMado != NULL)VERIFY(pMado->DelListItem(Data.KeyMado, LT_SBDO_CMD, Key));
        
        if(Key < m_nStartNum)m_nStartNum=Key;
        if(Key==m_nLastNum)
        {
            T_SBDO_K key;
            T_SBDO_D data;
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
            m_pDoc->m_pStagCtrl->DeactiveEtc(LT_SBDO_CMD, Key);
        }*/
    }
    return ret;
}

//-------------------------------------------------------------------------
BOOL CDB_SBDO::Get(T_SBDO_K Key,T_SBDO_D& rData)
{
    /*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        UINT uTemp;
        if (!m_pDoc->m_pStagCtrl->m_sbdo->Lookup(Key, uTemp)) return FALSE;
    }*/
    return m_sbdo.Lookup(Key,rData);
}

int CDB_SBDO::GetCount()
{
    /*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        return m_pDoc->m_pStagCtrl->m_sbdo->GetCount();
    }*/
    return m_sbdo.GetCount();
}

POSITION CDB_SBDO::GetStart()
{
    /*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        return m_pDoc->m_pStagCtrl->m_sbdo->GetStartPosition();
    }*/
    return m_sbdo.GetStartPosition();
}

void CDB_SBDO::GetNext(POSITION& rNextPosition,T_SBDO_K& rKey,T_SBDO_D& rData)
{
    /*if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0)
    {
        UINT uTemp;
        m_pDoc->m_pStagCtrl->m_sbdo->GetNextAssoc(rNextPosition, rKey, uTemp);
        m_sbdo.Lookup(rKey, rData);
        return;
    }*/
    m_sbdo.GetNextAssoc(rNextPosition,rKey,rData);
}
