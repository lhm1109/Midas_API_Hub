#ifndef __DB_PSCVU_DB_H__
#define __DB_PSCVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PSCVU
{
public:
  CDB_PSCVU()
  {
    m_pscv.InitHashTable(HASHSIZEPSCV);
  }
  virtual ~CDB_PSCVU() {};

public:
  void Add(T_UDRD_KEY Key, T_PSCV_UDRD_D& rData)
  {
    m_pscv.SetAt(Key, rData);
  }
  BOOL Del(T_UDRD_KEY Key)
  {
    return m_pscv.RemoveKey(Key);
  }
  void DelAll()
  {
    m_pscv.RemoveAll();
  }
  BOOL Get(T_UDRD_KEY Key, T_PSCV_UDRD_D& rData)
  {
    return m_pscv.Lookup(Key, rData);
  }
  int GetCount()
  {
    return m_pscv.GetCount();
  }
  POSITION GetStart()
  {
    return m_pscv.GetStartPosition();
  }
  void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_PSCV_UDRD_D& rData)
  {
    m_pscv.GetNextAssoc(rNextPosition, rKey, rData);
  }

protected:
  CMap<T_UDRD_KEY, T_UDRD_KEY, T_PSCV_UDRD_D, T_PSCV_UDRD_D&>m_pscv;
};

#endif