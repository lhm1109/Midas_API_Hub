#ifndef __DB_DOMN_DB_H__
#define __DB_DOMN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NODE;
class CDB_ELEM;

class CDB_DOMN
{
/*public:
    CDB_DOMN();
    virtual ~CDB_DOMN();

public:
    T_DOMN_K m_nStartNum;
    T_DOMN_K m_nLastNum;

public:
    void Add(T_DOMN_K Key,T_DOMN_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
    BOOL Del(T_DOMN_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);
    void AddItem(T_DOMN_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem);
    BOOL DelItem(T_DOMN_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
    BOOL Get(T_DOMN_K Key,T_DOMN_D& rData)
        {return m_domn.Lookup(Key,rData);}
    int GetCount()
        {return m_domn.GetCount();}
    POSITION GetStart()
        {return m_domn.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_DOMN_K& rKey,T_DOMN_D& rData)
        {m_domn.GetNextAssoc(rNextPosition,rKey,rData);}

    void AddList(T_DOMN_K Key);
    BOOL DelList(T_DOMN_K Key);
    void DelAllList();
    BOOL GetList(T_DOMN_K Key,DomnList*& rpList){return m_domnlist.Lookup(Key,rpList);}
    int GetCountList(){return m_domnlist.GetCount();}
    void AddListItem(T_DOMN_K Key, int nCmd, int nKey);
    BOOL DelListItem(T_DOMN_K Key, int nCmd, int nKey);

protected:
    CMap<T_DOMN_K,T_DOMN_K,T_DOMN_D,T_DOMN_D&>m_domn;
    CMap<T_DOMN_K,T_DOMN_K,DomnList*,DomnList*>m_domnlist;*/
};

#endif