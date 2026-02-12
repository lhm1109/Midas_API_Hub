#ifndef __DB_DOMNU_DB_H__
#define __DB_DOMNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DOMNU
{
/*public:
    CDB_DOMNU()
    {
        m_domn.InitHashTable(HASHSIZEDOMN);
        m_domnitem.InitHashTable(HASHSIZEDOMNITEM);
    }
    virtual ~CDB_DOMNU(){};

public:
    void Add(T_UDRD_KEY Key,T_DOMN_UDRD_D& rData)
        {m_domn.SetAt(Key,rData);}
    BOOL Del(T_UDRD_KEY Key)
        {return m_domn.RemoveKey(Key);}
    BOOL Get(T_UDRD_KEY Key,T_DOMN_UDRD_D& rData)
        {return m_domn.Lookup(Key,rData);}
    int GetCount()
        {return m_domn.GetCount();}
    POSITION GetStart()
        {return m_domn.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DOMN_UDRD_D& rData)
        {m_domn.GetNextAssoc(rNextPosition,rKey,rData);}

    void AddItem(T_UDRD_KEY Key,T_DOMN_ITEM_UDRD_D& rData)
        {m_domnitem.SetAt(Key,rData);}
    BOOL DelItem(T_UDRD_KEY Key)
        {return m_domnitem.RemoveKey(Key);}
    BOOL GetItem(T_UDRD_KEY Key,T_DOMN_ITEM_UDRD_D& rData)
        {return m_domnitem.Lookup(Key,rData);}
    int GetCountItem()
        {return m_domnitem.GetCount();}
    POSITION GetStartItem()
        {return m_domnitem.GetStartPosition();}
    void GetNextItem(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DOMN_ITEM_UDRD_D& rData)
        {m_domnitem.GetNextAssoc(rNextPosition,rKey,rData);}

    void DelAll()
        {
            m_domn.RemoveAll();
            m_domnitem.RemoveAll();
        }

protected:
     CMap<T_UDRD_KEY,T_UDRD_KEY,T_DOMN_UDRD_D,T_DOMN_UDRD_D&>m_domn;
     CMap<T_UDRD_KEY,T_UDRD_KEY,T_DOMN_ITEM_UDRD_D,T_DOMN_ITEM_UDRD_D&>m_domnitem;*/
};

#endif
