#ifndef __DB_CGFRU_DB_H__
#define __DB_CGFRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGFRU
{
public:
	CDB_CGFRU()
	{
		m_cgfr.InitHashTable(HASHSIZECGFR);
	}
	virtual ~CDB_CGFRU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGFR_UDRD_D& rData)
		{m_cgfr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgfr.RemoveKey(Key);}
	void DelAll()
		{m_cgfr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGFR_UDRD_D& rData)
		{return m_cgfr.Lookup(Key,rData);}
	int GetCount()
		{return m_cgfr.GetCount();}
	POSITION GetStart()
		{return m_cgfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGFR_UDRD_D& rData)
		{m_cgfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGFR_UDRD_D,T_CGFR_UDRD_D&>m_cgfr;
};

#endif