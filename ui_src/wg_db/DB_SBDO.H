#ifndef __DB_SBDO_DB_H__
#define __DB_SBDO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_MADO;

class CDB_SBDO
{
public:
    CDB_SBDO();
    virtual ~CDB_SBDO();
    CDBDoc* m_pDoc;

public:
    T_SBDO_K m_nStartNum;
    T_SBDO_K m_nLastNum;

public:
    void Add(T_SBDO_K Key,T_SBDO_D& rData,CDB_MADO* pMado);
    BOOL Del(T_SBDO_K Key,CDB_MADO* pMado);

public:
    BOOL Get(T_SBDO_K Key,T_SBDO_D& rData);
        //{return m_sbdo.Lookup(Key,rData);}
    int GetCount();
        //{return m_sbdo.GetCount();}
    POSITION GetStart();
        //{return m_sbdo.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_SBDO_K& rKey,T_SBDO_D& rData);
        //{m_sbdo.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
    CMap<T_SBDO_K,T_SBDO_K,T_SBDO_D,T_SBDO_D&>m_sbdo;
};

#endif