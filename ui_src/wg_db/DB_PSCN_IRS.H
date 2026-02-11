#ifndef __DB_PSCN_IRS_DB_H__
#define __DB_PSCN_IRS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PSCN_IRS
{
public:
    CDB_PSCN_IRS();
    virtual ~CDB_PSCN_IRS();
    CDBDoc* m_pDoc;
    
public:
    void Add(T_PSCN_IRS_K Key,T_PSCN_IRS_D& rData,CDB_ELEM* pElem);
    BOOL Del(T_PSCN_IRS_K Key,CDB_ELEM* pElem);
    
public:
    BOOL Get(T_PSCN_IRS_K Key,T_PSCN_IRS_D& rData);
    //{return m_PSCN.Lookup(Key,rData);}
    int GetCount();
    //{return m_PSCN.GetCount();}
    POSITION GetStart();
    //{return m_PSCN.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_PSCN_IRS_K& rKey,T_PSCN_IRS_D& rData);
    //{m_PSCN.GetNextAssoc(rNextPosition,rKey,rData);}
    
protected:
    CMap<T_PSCN_IRS_K,T_PSCN_IRS_K,T_PSCN_IRS_D,T_PSCN_IRS_D&>m_pscn;
};

#endif

