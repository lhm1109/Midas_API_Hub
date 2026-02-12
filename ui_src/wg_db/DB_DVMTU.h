#ifndef __DB_DVMTU_DB_H__
#define __DB_DVMTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DVMTU
{
public:
	CDB_DVMTU()
	{
		m_dvmt.InitHashTable(HASHSIZEDVMT);
	}
	virtual ~CDB_DVMTU(){};

public:
	void Add(T_UDRD_KEY Key,T_DVMT_UDRD_D& rData)
		{m_dvmt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dvmt.RemoveKey(Key);}
	void DelAll()
		{m_dvmt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DVMT_UDRD_D& rData)
		{return m_dvmt.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DVMT_UDRD_D,T_DVMT_UDRD_D&>m_dvmt;
};

#endif