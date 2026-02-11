#ifndef __DB_POCVU_DB_H__
#define __DB_POCVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POCVU
{
public:
	CDB_POCVU()
	{
		m_pocv.InitHashTable(HASHSIZEPOCV);
	}
	virtual ~CDB_POCVU(){};

public:
	void Add(T_UDRD_KEY Key,T_POCV_UDRD_D& rData)
		{m_pocv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pocv.RemoveKey(Key);}
	void DelAll()
		{m_pocv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_POCV_UDRD_D& rData)
		{return m_pocv.Lookup(Key,rData);}
	int GetCount()
		{return m_pocv.GetCount();}
	POSITION GetStart()
		{return m_pocv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_POCV_UDRD_D& rData)
		{m_pocv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_POCV_UDRD_D,T_POCV_UDRD_D&>m_pocv;
};

#endif