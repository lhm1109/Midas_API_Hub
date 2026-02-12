#ifndef __DB_UCCF_DB_H__
#define __DB_UCCF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_UCCF
{
public:
    CDB_UCCF();
    virtual ~CDB_UCCF();
    CDBDoc* m_pDoc;
    
public:
    void Add(T_UCCF_K Key,T_UCCF_D& rData,CDB_ELEM* pElem);
    BOOL Del(T_UCCF_K Key,CDB_ELEM* pElem);
    
public:
    BOOL Get(T_UCCF_K Key,T_UCCF_D& rData);
    
    int GetCount();
    
    POSITION GetStart();
    
    void GetNext(POSITION& rNextPosition,T_UCCF_K& rKey,T_UCCF_D& rData);
        
protected:
    CMap<T_UCCF_K,T_UCCF_K,T_UCCF_D,T_UCCF_D&>m_uccf;
};

#endif

