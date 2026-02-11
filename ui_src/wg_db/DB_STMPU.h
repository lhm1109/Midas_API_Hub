#ifndef __DB_STMPU_DB_H__
#define __DB_STMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STMPU
{
public:
	CDB_STMPU()
	{
		m_stmp.InitHashTable(HASHSIZESTMP);
	}
	virtual ~CDB_STMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_STMP_UDRD_D& rData)
		{m_stmp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stmp.RemoveKey(Key);}
	void DelAll()
		{m_stmp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STMP_UDRD_D& rData)
		{return m_stmp.Lookup(Key,rData);}
	int GetCount()
		{return m_stmp.GetCount();}
	POSITION GetStart()
		{return m_stmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STMP_UDRD_D& rData)
		{m_stmp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STMP_UDRD_D,T_STMP_UDRD_D&>m_stmp;
};

#endif