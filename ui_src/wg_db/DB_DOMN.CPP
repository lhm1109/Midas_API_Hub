#include "stdafx.h"
#include "DB_NODE.h"
#include "DB_ELEM.h"
#include "DB_DOMN.h"

#include "..\wg_base\wg_base_ArrayUtil.h"

/*CDB_DOMN::CDB_DOMN()
{
    m_nStartNum=1;
    m_nLastNum=0;
    m_domn.InitHashTable(HASHSIZEDOMN);
    m_domnlist.InitHashTable(HASHSIZELISTDOMN);
}

CDB_DOMN::~CDB_DOMN()
{
    DelAllList();
}

void CDB_DOMN::Add(T_DOMN_K Key,T_DOMN_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem)
{
    // 중복된 절점 요소 없앤다. (MxT Export시 문제 발생)
    CArrayUtil::RemoveDup(rData.arKeyNode);
    
    T_DOMN_D Data;
    BOOL bExist=m_domn.Lookup(Key, Data);
    CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNodeOld;
    CMap<T_NODE_K, T_NODE_K, T_NODE_K, T_NODE_K>mapNodeNew;
    CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemOld;
    CMap<T_ELEM_K, T_ELEM_K, T_ELEM_K, T_ELEM_K>mapElemNew;
    T_NODE_K KeyNodeTemp;
    T_ELEM_K KeyElemTemp;
    
    if(bExist)
    {
        mapNodeNew.InitHashTable(HASHSIZENODE);
        mapNodeOld.InitHashTable(HASHSIZENODE);
        mapElemNew.InitHashTable(HASHSIZEELEM);
        mapElemOld.InitHashTable(HASHSIZEELEM);
        int i;
        for(int i = 0; i<Data.arKeyNode.GetSize(); i++)mapNodeOld.SetAt(Data.arKeyNode[i], Data.arKeyNode[i]);
        for(int i = 0; i<rData.arKeyNode.GetSize(); i++)mapNodeNew.SetAt(rData.arKeyNode[i], rData.arKeyNode[i]);
        for(int i = 0; i<Data.arKeyElem.GetSize(); i++)mapElemOld.SetAt(Data.arKeyElem[i], Data.arKeyElem[i]);
        for(int i = 0; i<rData.arKeyElem.GetSize(); i++)mapElemNew.SetAt(rData.arKeyElem[i], rData.arKeyElem[i]);
        
        int nDomain=Data.arKeyNode.GetSize();
        for(int i = 0; i<nDomain; i++)
        {
            if(!mapNodeNew.Lookup(Data.arKeyNode[i], KeyNodeTemp))
                VERIFY(pNode->DelListItem(Data.arKeyNode[i], LT_DOMN_CMD, Key));
        }    
        nDomain=Data.arKeyElem.GetSize();
        for(int i = 0; i<nDomain; i++)
        {
            if(!mapElemNew.Lookup(Data.arKeyElem[i], KeyElemTemp))
                VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_DOMN_CMD, Key));
        }
    }
    else
    {
        T_DOMN_K key;
        T_DOMN_D data;
        POSITION pos = GetStart();
        while (pos)
        {
            GetNext(pos, key, data);
            if (data.nDomainId >= rData.nDomainId) 
            {
                data.nDomainId++;
                m_domn.SetAt(key, data);
            }
        }
    }
    m_domn.SetAt(Key,rData);

    if(Key == m_nStartNum)
    {
        T_DOMN_D data;
        while(TRUE)
        {
            m_nStartNum++;
            if(!Get(m_nStartNum,data))break;
        }
    }
    if(Key > m_nLastNum)m_nLastNum=Key;

    int nDomain=rData.arKeyNode.GetSize();
    for(int i=0; i<nDomain; i++)
    {
        if(bExist)
        {
            if(!mapNodeOld.Lookup(rData.arKeyNode[i], KeyNodeTemp))
                pNode->AddListItem(rData.arKeyNode[i], LT_DOMN_CMD, Key);
        }
        else
            pNode->AddListItem(rData.arKeyNode[i], LT_DOMN_CMD, Key);
    }
    nDomain=rData.arKeyElem.GetSize();
    for(int i = 0; i<nDomain; i++)
    {
        if(bExist)
        {
            if(!mapElemOld.Lookup(rData.arKeyElem[i], KeyElemTemp))
                pElem->AddListItem(rData.arKeyElem[i], LT_DOMN_CMD, Key);
        }
        else
            pElem->AddListItem(rData.arKeyElem[i], LT_DOMN_CMD, Key);
    }
}

BOOL CDB_DOMN::Del(T_DOMN_K Key,CDB_NODE* pNode,CDB_ELEM* pElem)
{
    T_DOMN_D Data;
    BOOL bExist=m_domn.Lookup(Key, Data);
    if(bExist)
    {
        BOOL ret=m_domn.RemoveKey(Key);
        ASSERT(ret);
        if(ret)
        {
            int nDomain=Data.arKeyNode.GetSize();
            for(int i=0; i<nDomain; i++)VERIFY(pNode->DelListItem(Data.arKeyNode[i], LT_DOMN_CMD, Key));
            nDomain=Data.arKeyElem.GetSize();
            for(int i = 0; i<nDomain; i++)VERIFY(pElem->DelListItem(Data.arKeyElem[i], LT_DOMN_CMD, Key));
            if(Key < m_nStartNum)m_nStartNum=Key;
            if(Key==m_nLastNum)
            {
                T_DOMN_K key;
                T_DOMN_D data;
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

            T_DOMN_K key;
            T_DOMN_D data;
            POSITION pos = GetStart();
            while (pos)
            {
                GetNext(pos, key, data);
                if (data.nDomainId > Data.nDomainId)
                {
                    data.nDomainId--;
                    m_domn.SetAt(key, data);
                }
            }
        }
        return ret;
    }
    return bExist;
}

void CDB_DOMN::AddItem(T_DOMN_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem)
{
    T_DOMN_D Data;
    if(!m_domn.Lookup(Key, Data))
    {
        ASSERT(FALSE);
        return;
    }

    if(KeyNode != 0)
    {
        Data.arKeyNode.Add(KeyNode);
        m_domn.SetAt(Key,Data);
        pNode->AddListItem(KeyNode, LT_DOMN_CMD, Key);
    }  
    if(KeyElem != 0)
    {
        Data.arKeyElem.Add(KeyElem);
        m_domn.SetAt(Key,Data);
        pElem->AddListItem(KeyElem, LT_DOMN_CMD, Key);
    }
}

BOOL CDB_DOMN::DelItem(T_DOMN_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem)
{
    T_DOMN_D Data;
    if(!m_domn.Lookup(Key, Data))
    {
        ASSERT(FALSE);
        return FALSE;
    }

    if(KeyNode != 0)
    {
        int nDomain=Data.arKeyNode.GetSize();
        for(int i=0; i<nDomain; i++)
        {
            if(Data.arKeyNode[i] == KeyNode)
            {
                Data.arKeyNode.RemoveAt(i);
                m_domn.SetAt(Key,Data);
                VERIFY(pNode->DelListItem(KeyNode, LT_DOMN_CMD, Key));
                return TRUE;
            }
        }
    }  
    if(KeyElem != 0)
    {
        int nDomain=Data.arKeyElem.GetSize();
        for(int i=0; i<nDomain; i++)
        {
            if(Data.arKeyElem[i] == KeyElem)
            {
                Data.arKeyElem.RemoveAt(i);
                m_domn.SetAt(Key,Data);
                VERIFY(pElem->DelListItem(KeyElem, LT_DOMN_CMD, Key));
                return TRUE;
            }
        }
    }
    ASSERT(FALSE);
    return FALSE;
}

void CDB_DOMN::AddList(T_DOMN_K Key)
{
    DomnList* pList = new DomnList;
    m_domnlist.SetAt(Key, pList);
}

BOOL CDB_DOMN::DelList(T_DOMN_K Key)
{
    DomnList* pList;
    BOOL bExist=m_domnlist.Lookup(Key,pList);
    ASSERT(bExist);
    if(!bExist)return FALSE;
    m_domnlist.RemoveKey(Key);
    delete pList;
    return TRUE;
}

void CDB_DOMN::DelAllList()
{
    T_DOMN_K Key;
    DomnList* pList;
    POSITION pos=m_domnlist.GetStartPosition();
    while(pos != NULL)
    {
        m_domnlist.GetNextAssoc(pos,Key,pList);
        delete pList;
    }
    m_domnlist.RemoveAll();
}

void CDB_DOMN::AddListItem(T_DOMN_K Key, int nCmd, int nKey)
{
    DomnList* pList;
    if(!GetList(Key, pList))return;

    T_LIST_DATA ListData;
    ListData.nCmd=nCmd;
    ListData.nKey=nKey;
    pList->AddTail(ListData);
}

BOOL CDB_DOMN::DelListItem(T_DOMN_K Key, int nCmd, int nKey)
{
    DomnList* pList;
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
}*/
