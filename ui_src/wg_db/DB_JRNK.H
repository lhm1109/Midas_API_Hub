#ifndef __DB_JRNK_DB_H__
#define __DB_JRNK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_ELEM;
class CDB_JRNK
{
public:
    CDB_JRNK();
    virtual ~CDB_JRNK();
    CDBDoc* m_pDoc;
    
public:
    void Add(T_JRNK_K Key,T_JRNK_D& rData,CDB_ELEM* pStor);
    BOOL Del(T_JRNK_K Key,CDB_ELEM* pStor);
    
public:
    BOOL Get(T_JRNK_K Key,T_JRNK_D& rData);
    int GetCount();
    POSITION GetStart();
    void GetNext(POSITION& rNextPosition,T_JRNK_K& rKey,T_JRNK_D& rData);
    
protected:
    CMap<T_JRNK_K,T_JRNK_K,T_JRNK_D,T_JRNK_D&>m_JRNK;
};

#endif