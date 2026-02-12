#ifndef __DB_PFDN_DB_H__
#define __DB_PFDN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_PFDN
{
public:
  CDB_PFDN();
  virtual ~CDB_PFDN();
  CDBDoc* m_pDoc;

public:
  void Add(T_PFDN_D& rData);
  BOOL Del();

public:
  BOOL Get(T_PFDN_D& rData);
    //{return m_dctl.Lookup(1,rData);}
  int GetCount();
    //{return m_dctl.GetCount();}

protected:
  CMap<T_PFDN_K,T_PFDN_K,T_PFDN_D,T_PFDN_D&>m_dctl;
};

#endif