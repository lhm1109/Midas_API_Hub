#ifndef __DB_RPRPU_DB_H__
#define __DB_RPRPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RPRPU
{
public:
	CDB_RPRPU()
	{
		m_rprp.InitHashTable(HASHSIZERPRP);
	}
	virtual ~CDB_RPRPU(){};

public:
	void Add(T_UDRD_KEY Key,T_RPRP_UDRD_D& rData)
	{m_rprp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_rprp.RemoveKey(Key);}
	void DelAll()
	{m_rprp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RPRP_UDRD_D& rData)
	{return m_rprp.Lookup(Key,rData);}
	int GetCount()
	{return m_rprp.GetCount();}
	POSITION GetStart()
	{return m_rprp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RPRP_UDRD_D& rData)
	{m_rprp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_RPRP_UDRD_D,T_RPRP_UDRD_D&>m_rprp;
};

#endif