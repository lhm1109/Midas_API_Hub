#ifndef __DB_CLWPU_DB_H__
#define __DB_CLWPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CLWPU
{
public:
	CDB_CLWPU()
	{
		m_clwp.InitHashTable(HASHSIZECLWP);
	}
	virtual ~CDB_CLWPU(){};

public:
	void Add(T_UDRD_KEY Key,T_CLWP_UDRD_D& rData)
		{m_clwp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_clwp.RemoveKey(Key);}
	void DelAll()
		{m_clwp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CLWP_UDRD_D& rData)
		{return m_clwp.Lookup(Key,rData);}
	int GetCount()
		{return m_clwp.GetCount();}
	POSITION GetStart()
		{return m_clwp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CLWP_UDRD_D& rData)
		{m_clwp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CLWP_UDRD_D,T_CLWP_UDRD_D&>m_clwp;
};

#endif