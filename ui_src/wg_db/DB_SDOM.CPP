#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_SDOM.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

CDB_SDOM::CDB_SDOM()
{
    m_nStartNum=1;
    m_nLastNum=0;
    m_sdom.InitHashTable(HASHSIZESDOM);
    m_sdomlist.InitHashTable(HASHSIZELISTSDOM);
}

CDB_SDOM::~CDB_SDOM()
{
    DelAllList();
}

void CDB_SDOM::Add(T_SDOM_K Key,T_SDOM_D& rData,CDB_ELEM* pElem)
{
    T_SDOM_D Data;
    BOOL bExist=m_sdom.Lookup(Key, Data);
    CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemOld;
    CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemNew;
    T_ELEM_K KeyElemTemp;
    
    if(bExist)
    {
        mapElemNew.InitHashTable(HASHSIZEELEM);
        mapElemOld.InitHashTable(HASHSIZEELEM);
        for(int i = 0; i<Data.arKeyElem.GetSize(); i++)mapElemOld.SetAt(Data.arKeyElem[i], Data.arKeyElem[i]);
        for(int i = 0; i<rData.arKeyElem.GetSize(); i++)mapElemNew.SetAt(rData.arKeyElem[i], rData.arKeyElem[i]);
        
        int nDomain=Data.arKeyElem.GetSize();
        for(int i = 0; i<nDomain; i++)
        {
            if(!mapElemNew.Lookup(Data.arKeyElem[i], KeyElemTemp))
                VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_SDOM_CMD, Key));
        }
    }
    else
    {
        T_SDOM_K key;
        T_SDOM_D data;
        POSITION pos = GetStart();
        while (pos)
        {
            GetNext(pos, key, data);
            if (data.nDomainId >= rData.nDomainId) 
            {
                data.nDomainId++;
                m_sdom.SetAt(key, data);
            }
        }
    }
    m_sdom.SetAt(Key,rData);

    if(Key == m_nStartNum)
    {
        T_SDOM_D data;
        while(TRUE)
        {
            m_nStartNum++;
            if(!Get(m_nStartNum,data))break;
        }
    }
    if(Key > m_nLastNum)m_nLastNum=Key;

    int nDomain=rData.arKeyElem.GetSize();
    for(int i=0; i<nDomain; i++)
    {
        if(bExist)
        {
            if(!mapElemOld.Lookup(rData.arKeyElem[i], KeyElemTemp))
                pElem->AddListItem(rData.arKeyElem[i], LT_SDOM_CMD, Key);
        }
        else
            pElem->AddListItem(rData.arKeyElem[i], LT_SDOM_CMD, Key);
    }
}

BOOL CDB_SDOM::Del(T_SDOM_K Key,CDB_ELEM* pElem)
{
    T_SDOM_D Data;
    BOOL bExist=m_sdom.Lookup(Key, Data);
    if(bExist)
    {
        BOOL ret=m_sdom.RemoveKey(Key);
        ASSERT(ret);
        if(ret)
        {
            int nDomain=Data.arKeyElem.GetSize();
            for(int i=0; i<nDomain; i++)VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_SDOM_CMD, Key));
            if(Key < m_nStartNum)m_nStartNum=Key;
            if(Key==m_nLastNum)
            {
                T_SDOM_K key;
                T_SDOM_D data;
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

            T_SDOM_K key;
            T_SDOM_D data;
            POSITION pos = GetStart();
            while (pos)
            {
                GetNext(pos, key, data);
                if (data.nDomainId > Data.nDomainId)
                {
                    data.nDomainId--;
                    m_sdom.SetAt(key, data);
                }
            }
        }
        return ret;
    }
    return bExist;
}

void CDB_SDOM::AddItem(T_SDOM_K Key,T_ELEM_K KeyElem,CDB_ELEM* pElem)
{
    T_SDOM_D Data;
    if(!m_sdom.Lookup(Key, Data))
    {
        ASSERT(FALSE);
        return;
    }

    if(KeyElem != 0)
    {
        Data.arKeyElem.Add(KeyElem);
        m_sdom.SetAt(Key,Data);
        pElem->AddListItem(KeyElem, LT_SDOM_CMD, Key);
    }
}

BOOL CDB_SDOM::DelItem(T_SDOM_K Key,T_ELEM_K KeyElem,CDB_ELEM* pElem)
{
    T_SDOM_D Data;
    if(!m_sdom.Lookup(Key, Data))
    {
        ASSERT(FALSE);
        return FALSE;
    }

    if(KeyElem != 0)
    {
        int nDomain=Data.arKeyElem.GetSize();
        for(int i=0; i<nDomain; i++)
        {
            if(Data.arKeyElem[i] == KeyElem)
            {
                Data.arKeyElem.RemoveAt(i);
                m_sdom.SetAt(Key,Data);
                VERIFY(pElem->DelListItem(KeyElem, LT_SDOM_CMD, Key));
                return TRUE;
            }
        }
    }
    ASSERT(FALSE);
    return FALSE;
}

void CDB_SDOM::AddList(T_SDOM_K Key)
{
    SdomList* pList = new SdomList;
    m_sdomlist.SetAt(Key, pList);
}

BOOL CDB_SDOM::DelList(T_SDOM_K Key)
{
    SdomList* pList;
    BOOL bExist=m_sdomlist.Lookup(Key,pList);
    ASSERT(bExist);
    if(!bExist)return FALSE;
    m_sdomlist.RemoveKey(Key);
    delete pList;
    return TRUE;
}

void CDB_SDOM::DelAllList()
{
    T_SDOM_K Key;
    SdomList* pList;
    POSITION pos=m_sdomlist.GetStartPosition();
    while(pos != NULL)
    {
        m_sdomlist.GetNextAssoc(pos,Key,pList);
        delete pList;
    }
    m_sdomlist.RemoveAll();
}

void CDB_SDOM::AddListItem(T_SDOM_K Key, int nCmd, int nKey)
{
    SdomList* pList;
    if(!GetList(Key, pList))return;

    T_LIST_DATA ListData;
    ListData.nCmd=nCmd;
    ListData.nKey=nKey;
    pList->AddTail(ListData);
}

BOOL CDB_SDOM::DelListItem(T_SDOM_K Key, int nCmd, int nKey)
{
    SdomList* pList;
    if(!GetList(Key, pList))return TRUE;

    T_LIST_DATA ListData;
    POSITION pos=pList->GetHeadPosition(), posPrv;
    while(pos != NULL)
    {
        posPrv=pos;
        ListData=pList->GetNext(pos);
        if(ListData.nCmd == nCmd && ListData.nKey == nKey)
        {
            pList->RemoveAt(posPrv);
            return TRUE;
        }
    }
    return FALSE;
}
