#ifndef __DB_JDDPU_DB_H__
#define __DB_JDDPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDDPU
{
public:
	CDB_JDDPU()
	{
		m_JDDP.InitHashTable(HASHSIZEJDDP);
	}
	virtual ~CDB_JDDPU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDDP_UDRD_D& rData)
		{m_JDDP.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDDP.RemoveKey(Key);}
	void DelAll()
		{m_JDDP.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDDP_UDRD_D& rData)
		{return m_JDDP.Lookup(Key,rData);}
	int GetCount()
		{return m_JDDP.GetCount();}
	POSITION GetStart()
		{return m_JDDP.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_JDDP_UDRD_D& rData)
		{m_JDDP.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDDP_UDRD_D,T_JDDP_UDRD_D&>m_JDDP;
};

#endif
