#ifndef __DB_PFMPU_DB_H__
#define __DB_PFMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PFMPU
{
public:
	CDB_PFMPU()
	{
		m_pfmp.InitHashTable(HASHSIZEPFMP);
	}
	virtual ~CDB_PFMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_PFMP_UDRD_D& rData)
		{m_pfmp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pfmp.RemoveKey(Key);}
	void DelAll()
		{m_pfmp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PFMP_UDRD_D& rData)
		{return m_pfmp.Lookup(Key,rData);}
	int GetCount()
		{return m_pfmp.GetCount();}
	POSITION GetStart()
		{return m_pfmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PFMP_UDRD_D& rData)
		{m_pfmp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PFMP_UDRD_D,T_PFMP_UDRD_D&>m_pfmp;
};

#endif