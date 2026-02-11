#ifndef __DB_PSCV_DB_H__
#define __DB_PSCV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_PSCV
{
public:
  CDB_PSCV();
  virtual ~CDB_PSCV();
  CDBDoc* m_pDoc;

public:
  void Add(T_PSCV_K Key, T_PSCV_D& rData);
  BOOL Del(T_PSCV_K Key);

public:
  BOOL Get(T_PSCV_K Key, T_PSCV_D& rData);
  int GetCount();
  POSITION GetStart();
  void GetNext(POSITION& rNextPosition, T_PSCV_K& rKey, T_PSCV_D& rData);

protected:
  CMap<T_PSCV_K, T_PSCV_K, T_PSCV_D, T_PSCV_D&>m_pscv;
};

#endif

