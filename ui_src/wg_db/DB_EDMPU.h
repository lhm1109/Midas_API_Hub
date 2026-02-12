#ifndef __DB_EDMPU_DB_H__
#define __DB_EDMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EDMPU
{
public:
	CDB_EDMPU()
	{
		m_edmp.InitHashTable(HASHSIZEEDMP);
	}
	virtual ~CDB_EDMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_EDMP_UDRD_D& rData)
		{m_edmp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_edmp.RemoveKey(Key);}
	void DelAll()
		{m_edmp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EDMP_UDRD_D& rData)
		{return m_edmp.Lookup(Key,rData);}
	int GetCount()
		{return m_edmp.GetCount();}
	POSITION GetStart()
		{return m_edmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EDMP_UDRD_D& rData)
		{m_edmp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_EDMP_UDRD_D,T_EDMP_UDRD_D&>m_edmp;
};

#endif