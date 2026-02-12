#ifndef __DB_EIGVU_DB_H__
#define __DB_EIGVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EIGVU
{
public:
	CDB_EIGVU()
	{
		m_eigv.InitHashTable(HASHSIZEEIGV);
	}
	virtual ~CDB_EIGVU(){};

public:
	void Add(T_UDRD_KEY Key,T_EIGV_UDRD_D& rData)
		{m_eigv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_eigv.RemoveKey(Key);}
	void DelAll()
		{m_eigv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EIGV_UDRD_D& rData)
		{return m_eigv.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_EIGV_UDRD_D,T_EIGV_UDRD_D&>m_eigv;
};

#endif