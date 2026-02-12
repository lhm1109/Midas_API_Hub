#ifndef __DB_PSCWU_DB_H__
#define __DB_PSCWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PSCWU
{
public:
	CDB_PSCWU()
	{
		m_pscw.InitHashTable(HASHSIZEPSCW);
	}
	virtual ~CDB_PSCWU(){};

public:
	void Add(T_UDRD_KEY Key,T_PSCW_UDRD_D& rData)
		{m_pscw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pscw.RemoveKey(Key);}
	void DelAll()
		{m_pscw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PSCW_UDRD_D& rData)
		{return m_pscw.Lookup(Key,rData);}
	int GetCount()
		{return m_pscw.GetCount();}
	POSITION GetStart()
		{return m_pscw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PSCW_UDRD_D& rData)
		{m_pscw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PSCW_UDRD_D,T_PSCW_UDRD_D&>m_pscw;
};

#endif