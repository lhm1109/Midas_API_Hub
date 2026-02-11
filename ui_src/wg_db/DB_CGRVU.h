#ifndef __DB_CGRVU_DB_H__
#define __DB_CGRVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGRVU
{
public:
	CDB_CGRVU()
	{
		m_cgrv.InitHashTable(HASHSIZECGRV);
	}
	virtual ~CDB_CGRVU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGRV_UDRD_D& rData)
		{m_cgrv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgrv.RemoveKey(Key);}
	void DelAll()
		{m_cgrv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGRV_UDRD_D& rData)
		{return m_cgrv.Lookup(Key,rData);}
	int GetCount()
		{return m_cgrv.GetCount();}
	POSITION GetStart()
		{return m_cgrv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGRV_UDRD_D& rData)
		{m_cgrv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGRV_UDRD_D,T_CGRV_UDRD_D&>m_cgrv;
};

#endif