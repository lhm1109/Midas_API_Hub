#ifndef __DB_RESCU_DB_H__
#define __DB_RESCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RESCU
{
public:
	CDB_RESCU()
	{
		m_resc.InitHashTable(HASHSIZERESC);
	}
	virtual ~CDB_RESCU(){};

public:
	void Add(T_UDRD_KEY Key,T_RESC_UDRD_D& rData)
		{m_resc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_resc.RemoveKey(Key);}
	void DelAll()
		{m_resc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RESC_UDRD_D& rData)
		{return m_resc.Lookup(Key,rData);}
	int GetCount()
		{return m_resc.GetCount();}
	POSITION GetStart()
		{return m_resc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RESC_UDRD_D& rData)
		{m_resc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RESC_UDRD_D,T_RESC_UDRD_D&>m_resc;
};

#endif