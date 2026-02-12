#ifndef __DB_NTMPU_DB_H__
#define __DB_NTMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NTMPU
{
public:
	CDB_NTMPU()
	{
		m_ntmp.InitHashTable(HASHSIZENTMP);
	}
	virtual ~CDB_NTMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_NTMP_UDRD_D& rData)
		{m_ntmp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ntmp.RemoveKey(Key);}
	void DelAll()
		{m_ntmp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NTMP_UDRD_D& rData)
		{return m_ntmp.Lookup(Key,rData);}
	int GetCount()
		{return m_ntmp.GetCount();}
	POSITION GetStart()
		{return m_ntmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_NTMP_UDRD_D& rData)
		{m_ntmp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NTMP_UDRD_D,T_NTMP_UDRD_D&>m_ntmp;
};

#endif