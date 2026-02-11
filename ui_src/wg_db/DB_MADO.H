#ifndef __DB_MADO_DB_H__
#define __DB_MADO_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_MATL;
class CDB_THIK;

class CDB_MADO
{
public:
    CDB_MADO();
    virtual ~CDB_MADO();
    CDBDoc* m_pDoc;

public:
    T_MADO_K m_nStartNum;
    T_MADO_K m_nLastNum;

public:
    void Add(T_MADO_K Key,T_MADO_D& rData,CDB_MATL* pMatl,CDB_THIK* pThik);
    BOOL Del(T_MADO_K Key,CDB_MATL* pMatl,CDB_THIK* pThik);

public:
    BOOL Get(T_MADO_K Key,T_MADO_D& rData);
        //{return m_mado.Lookup(Key,rData);}
    int GetCount();
        //{return m_mado.GetCount();}
    POSITION GetStart();
        //{return m_mado.GetStartPosition();}
    void GetNext(POSITION& rNextPosition,T_MADO_K& rKey,T_MADO_D& rData);
        //{m_mado.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
    CMap<T_MADO_K,T_MADO_K,T_MADO_D,T_MADO_D&>m_mado;
};

#endif