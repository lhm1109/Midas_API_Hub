#ifndef __DB_IRST_DB_H__
#define __DB_IRST_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_STOR;
class CDB_IRST
{
public:
    CDB_IRST();
    virtual ~CDB_IRST();
    CDBDoc* m_pDoc;
    
public:
    void Add(T_IRST_K Key,T_IRST_D& rData,CDB_STOR* pStor);
    BOOL Del(T_IRST_K Key,CDB_STOR* pStor);
    
public:
    BOOL Get(T_IRST_K Key,T_IRST_D& rData);
    int GetCount();
    POSITION GetStart();
    void GetNext(POSITION& rNextPosition,T_IRST_K& rKey,T_IRST_D& rData);
    
protected:
    CMap<T_IRST_K,T_IRST_K,T_IRST_D,T_IRST_D&>m_IRST;
};

#endif