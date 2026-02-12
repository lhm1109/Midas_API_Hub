#ifndef __DB_SDOM_DB_H__
#define __DB_SDOM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;

class CDB_SDOM
{
public:
    CDB_SDOM();
    virtual ~CDB_SDOM();

public:
    T_SDOM_K m_nStartNum;
    T_SDOM_K m_nLastNum;

public:
    void Add(T_SDOM_K Key,T_SDOM_D& rData,CDB_ELEM* pElem);
    BOOL Del(T_SDOM_K Key,CDB_ELEM* pElem);
    void AddItem(T_SDOM_K Key,T_ELEM_K KeyElem,CDB_ELEM* pElem);
    BOOL DelItem(T_SDOM_K Key,T_ELEM_K KeyElem,CDB_ELEM* pElem);

public:
    BOOL Get(T_SDOM_K Key,T_SDOM_D& rData)
        {return m_sdom.Lookup(Key,rData);}
    int GetCount()
        {return m_sdom.GetCount();}
    POSITION GetStart()
        {return m_sdom.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_SDOM_K& rKey,T_SDOM_D& rData)
        {m_sdom.GetNextAssoc(rNextPosition,rKey,rData);}

    void AddList(T_SDOM_K Key);
    BOOL DelList(T_SDOM_K Key);
    void DelAllList();
    BOOL GetList(T_SDOM_K Key,SdomList*& rpList){return m_sdomlist.Lookup(Key,rpList);}
    int GetCountList(){return m_sdomlist.GetCount();}
    void AddListItem(T_SDOM_K Key, int nCmd, int nKey);
    BOOL DelListItem(T_SDOM_K Key, int nCmd, int nKey);

protected:
    CMap<T_SDOM_K,T_SDOM_K,T_SDOM_D,T_SDOM_D&>m_sdom;
    CMap<T_SDOM_K,T_SDOM_K,SdomList*,SdomList*>m_sdomlist;
};

#endif