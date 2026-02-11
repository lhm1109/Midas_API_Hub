#ifndef __DB_PFDNU_DB_H__
#define __DB_PFDNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PFDNU
{
public:
  CDB_PFDNU()
  {
    m_dctl.InitHashTable(HASHSIZEPFDN);
  }
  virtual ~CDB_PFDNU(){};

public:
  void Add(T_UDRD_KEY Key,T_PFDN_UDRD_D& rData)
    {m_dctl.SetAt(Key,rData);}
  BOOL Del(T_UDRD_KEY Key)
    {return m_dctl.RemoveKey(Key);}
  void DelAll()
    {m_dctl.RemoveAll();}
  BOOL Get(T_UDRD_KEY Key,T_PFDN_UDRD_D& rData)
    {return m_dctl.Lookup(Key,rData);}

protected:
   CMap<T_UDRD_KEY,T_UDRD_KEY,T_PFDN_UDRD_D,T_PFDN_UDRD_D&>m_dctl;
};

#endif