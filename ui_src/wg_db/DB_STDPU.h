#ifndef __DB_STDPU_DB_H__
#define __DB_STDPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STDPU
{
public:
	CDB_STDPU()
	{
		m_stdp.InitHashTable(HASHSIZESTDP);
	}
	virtual ~CDB_STDPU(){};

public:
	void Add(T_UDRD_KEY Key,T_STDP_UDRD_D& rData)
		{m_stdp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stdp.RemoveKey(Key);}
	void DelAll()
		{m_stdp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STDP_UDRD_D& rData)
		{return m_stdp.Lookup(Key,rData);}
	int GetCount()
		{return m_stdp.GetCount();}
	POSITION GetStart()
		{return m_stdp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STDP_UDRD_D& rData)
		{m_stdp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STDP_UDRD_D,T_STDP_UDRD_D&>m_stdp;
};

#endif