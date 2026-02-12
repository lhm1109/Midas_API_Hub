#ifndef __DB_MISAU_DB_H__
#define __DB_MISAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MISAU
{
public:
	CDB_MISAU()
	{
		m_misa.InitHashTable(HASHSIZEMISA);
	}
	virtual ~CDB_MISAU(){};

public:
	void Add(T_UDRD_KEY Key,T_MISA_UDRD_D& rData)
		{m_misa.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_misa.RemoveKey(Key);}
	void DelAll()
		{m_misa.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MISA_UDRD_D& rData)
		{return m_misa.Lookup(Key,rData);}
	int GetCount()
		{return m_misa.GetCount();}
	POSITION GetStart()
		{return m_misa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MISA_UDRD_D& rData)
		{m_misa.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MISA_UDRD_D,T_MISA_UDRD_D&>m_misa;
};

#endif