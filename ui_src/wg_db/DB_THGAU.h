#ifndef __DB_THGAU_DB_H__
#define __DB_THGAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THGAU
{
public:
	CDB_THGAU()
	{
		m_thga.InitHashTable(HASHSIZETHGA);
	}
	virtual ~CDB_THGAU(){};

public:
	void Add(T_UDRD_KEY Key,T_THGA_UDRD_D& rData)
		{m_thga.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thga.RemoveKey(Key);}
	void DelAll()
		{m_thga.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THGA_UDRD_D& rData)
		{return m_thga.Lookup(Key,rData);}
	int GetCount()
		{return m_thga.GetCount();}
	POSITION GetStart()
		{return m_thga.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THGA_UDRD_D& rData)
		{m_thga.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THGA_UDRD_D,T_THGA_UDRD_D&>m_thga;
};

#endif