#ifndef __DB_CGDVU_DB_H__
#define __DB_CGDVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGDVU
{
public:
	CDB_CGDVU()
	{
		m_cgdv.InitHashTable(HASHSIZECGDV);
	}
	virtual ~CDB_CGDVU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGDV_UDRD_D& rData)
		{m_cgdv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgdv.RemoveKey(Key);}
	void DelAll()
		{m_cgdv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGDV_UDRD_D& rData)
		{return m_cgdv.Lookup(Key,rData);}
	int GetCount()
		{return m_cgdv.GetCount();}
	POSITION GetStart()
		{return m_cgdv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGDV_UDRD_D& rData)
		{m_cgdv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGDV_UDRD_D,T_CGDV_UDRD_D&>m_cgdv;
};

#endif