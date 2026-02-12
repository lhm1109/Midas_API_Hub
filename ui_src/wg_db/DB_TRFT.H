#ifndef __DB_TRFT_DB_H__
#define __DB_TRFT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_TRFT
{
public:
    CDB_TRFT();
    virtual ~CDB_TRFT();
    CDBDoc* m_pDoc;

public:
    void Add(T_TRFT_K Key,T_TRFT_D& rData,CDB_ELEM* pElem);
    BOOL Del(T_TRFT_K Key,CDB_ELEM* pElem);

public:
    BOOL Get(T_TRFT_K Key,T_TRFT_D& rData);
        //{return m_trft.Lookup(Key,rData);}
    int GetCount();
        //{return m_trft.GetCount();}
    POSITION GetStart();
        //{return m_trft.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_TRFT_K& rKey,T_TRFT_D& rData);
        //{m_trft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
    CMap<T_TRFT_K,T_TRFT_K,T_TRFT_D,T_TRFT_D&>m_trft;
};

#endif

