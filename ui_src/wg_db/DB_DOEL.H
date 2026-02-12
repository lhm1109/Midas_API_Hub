#ifndef __DB_DOEL_DB_H__
#define __DB_DOEL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DOEL
{
public:
    CDB_DOEL();
    virtual ~CDB_DOEL();
    CDBDoc* m_pDoc;

public:
    void Add(T_DOEL_K Key,T_DOEL_D& rData,CDB_ELEM* pElem);
    BOOL Del(T_DOEL_K Key,CDB_ELEM* pElem);

public:
    BOOL Get(T_DOEL_K Key,T_DOEL_D& rData);
        //{return m_doel.Lookup(Key,rData);}
    int GetCount();
        //{return m_doel.GetCount();}
    POSITION GetStart();
        //{return m_doel.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_DOEL_K& rKey,T_DOEL_D& rData);
        //{m_doel.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
    CMap<T_DOEL_K,T_DOEL_K,T_DOEL_D,T_DOEL_D&>m_doel;
};

#endif

