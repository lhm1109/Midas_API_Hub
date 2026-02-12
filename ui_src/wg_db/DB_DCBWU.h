#ifndef __DB_DCBWU_DB_H__
#define __DB_DCBWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DCBWU
{
public:
	CDB_DCBWU()
	{
		m_dcbw.InitHashTable(HASHSIZEDCBW);
	}
	virtual ~CDB_DCBWU(){};

public:
	void Add(T_UDRD_KEY Key,T_DCBW_UDRD_D& rData)
		{m_dcbw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dcbw.RemoveKey(Key);}
	void DelAll()
		{m_dcbw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DCBW_UDRD_D& rData)
		{return m_dcbw.Lookup(Key,rData);}
	int GetCount()
		{return m_dcbw.GetCount();}
	POSITION GetStart()
		{return m_dcbw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DCBW_UDRD_D& rData)
		{m_dcbw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DCBW_UDRD_D,T_DCBW_UDRD_D&>m_dcbw;
};

#endif