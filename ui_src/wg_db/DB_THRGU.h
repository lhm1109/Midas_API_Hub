#ifndef __DB_THRGU_DB_H__
#define __DB_THRGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THRGU
{
public:
	CDB_THRGU()
	{
		m_thrg.InitHashTable(HASHSIZETHRG);
	}
	virtual ~CDB_THRGU(){};

public:
	void Add(T_UDRD_KEY Key,T_THRG_UDRD_D& rData)
		{m_thrg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thrg.RemoveKey(Key);}
	void DelAll()
		{m_thrg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THRG_UDRD_D& rData)
		{return m_thrg.Lookup(Key,rData);}
	int GetCount()
		{return m_thrg.GetCount();}
	POSITION GetStart()
		{return m_thrg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THRG_UDRD_D& rData)
		{m_thrg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THRG_UDRD_D,T_THRG_UDRD_D&>m_thrg;
};

#endif