#ifndef __DB_TDNTU_DB_H__
#define __DB_TDNTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDNTU
{
public:
	CDB_TDNTU()
	{
		m_tdnt.InitHashTable(HASHSIZETDNT);
	}
	virtual ~CDB_TDNTU(){};

public:
	void Add(T_UDRD_KEY Key,T_TDNT_UDRD_D& rData)
		{m_tdnt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tdnt.RemoveKey(Key);}
	void DelAll()
		{m_tdnt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TDNT_UDRD_D& rData)
		{return m_tdnt.Lookup(Key,rData);}
	int GetCount()
		{return m_tdnt.GetCount();}
	POSITION GetStart()
		{return m_tdnt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TDNT_UDRD_D& rData)
		{m_tdnt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TDNT_UDRD_D,T_TDNT_UDRD_D&>m_tdnt;
};

#endif