#ifndef __DB_SDOMU_DB_H__
#define __DB_SDOMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDOMU
{
public:
    CDB_SDOMU()
    {
        m_sdom.InitHashTable(HASHSIZESDOM);
        m_sdomitem.InitHashTable(HASHSIZESDOMITEM);
    }
    virtual ~CDB_SDOMU(){};

public:
    void Add(T_UDRD_KEY Key,T_SDOM_UDRD_D& rData)
        {m_sdom.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_sdom.RemoveKey(Key);}
    BOOL Get(T_UDRD_KEY Key,T_SDOM_UDRD_D& rData)
        {return m_sdom.Lookup(Key,rData);}
    int GetCount()
        {return m_sdom.GetCount();}
    POSITION GetStart()
        {return m_sdom.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDOM_UDRD_D& rData)
        {m_sdom.GetNextAssoc(rNextPosition,rKey,rData);}

    void AddItem(T_UDRD_KEY Key,T_SDOM_ITEM_UDRD_D& rData)
        {m_sdomitem.SetAt(Key,rData);}
    BOOL DelItem(T_UDRD_KEY Key)
        {return m_sdomitem.RemoveKey(Key);}
    BOOL GetItem(T_UDRD_KEY Key,T_SDOM_ITEM_UDRD_D& rData)
        {return m_sdomitem.Lookup(Key,rData);}
    int GetCountItem()
        {return m_sdomitem.GetCount();}
    POSITION GetStartItem()
        {return m_sdomitem.GetStartPosition();}
    void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDOM_ITEM_UDRD_D& rData)
        {m_sdomitem.GetNextAssoc(rNextPosition,rKey,rData);}

    void DelAll()
        {
            m_sdom.RemoveAll();
            m_sdomitem.RemoveAll();
        }

protected:
     CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDOM_UDRD_D,T_SDOM_UDRD_D&>m_sdom;
     CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDOM_ITEM_UDRD_D,T_SDOM_ITEM_UDRD_D&>m_sdomitem;
};

#endif
