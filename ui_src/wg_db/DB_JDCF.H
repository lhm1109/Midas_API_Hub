#ifndef __DB_JDCF_DB_H__
#define __DB_JDCF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDCF
{
public:
    CDB_JDCF();
    virtual ~CDB_JDCF();
    
public:
    void Add(T_JDCF_D& rData);
    BOOL Del();
    
public:
    BOOL Get(T_JDCF_D& rData)
    {return m_JDCF.Lookup(1,rData);}
    int GetCount()
    {return m_JDCF.GetCount();}
    
protected:
    CMap<T_JDCF_K,T_JDCF_K,T_JDCF_D,T_JDCF_D&> m_JDCF;
};

#endif