#ifndef __DB_WVEPU_DB_H__
#define __DB_WVEPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WVEPU
{
public:
	CDB_WVEPU()
	{
		m_wvep.InitHashTable(HASHSIZEWVEP);
	}
	virtual ~CDB_WVEPU(){};

public:
	void Add(T_UDRD_KEY Key,T_WVEP_UDRD_D& rData)
		{m_wvep.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_wvep.RemoveKey(Key);}
	void DelAll()
		{m_wvep.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_WVEP_UDRD_D& rData)
		{return m_wvep.Lookup(Key,rData);}
	int GetCount()
		{return m_wvep.GetCount();}
	POSITION GetStart()
		{return m_wvep.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_WVEP_UDRD_D& rData)
		{m_wvep.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_WVEP_UDRD_D,T_WVEP_UDRD_D&>m_wvep;
};

#endif