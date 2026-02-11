#ifndef __DB_CGTSU_DB_H__
#define __DB_CGTSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGTSU
{
public:
	CDB_CGTSU()
	{
		m_cgts.InitHashTable(HASHSIZECGTS);
	}
	virtual ~CDB_CGTSU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGTS_UDRD_D& rData)
		{m_cgts.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgts.RemoveKey(Key);}
	void DelAll()
		{m_cgts.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGTS_UDRD_D& rData)
		{return m_cgts.Lookup(Key,rData);}
	int GetCount()
		{return m_cgts.GetCount();}
	POSITION GetStart()
		{return m_cgts.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGTS_UDRD_D& rData)
		{m_cgts.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGTS_UDRD_D,T_CGTS_UDRD_D&>m_cgts;
};

#endif