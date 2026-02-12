#ifndef __DB_CGLTU_DB_H__
#define __DB_CGLTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGLTU
{
public:
	CDB_CGLTU()
	{
		m_cglt.InitHashTable(HASHSIZECGLT);
	}
	virtual ~CDB_CGLTU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGLT_UDRD_D& rData)
		{m_cglt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cglt.RemoveKey(Key);}
	void DelAll()
		{m_cglt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGLT_UDRD_D& rData)
		{return m_cglt.Lookup(Key,rData);}
	int GetCount()
		{return m_cglt.GetCount();}
	POSITION GetStart()
		{return m_cglt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGLT_UDRD_D& rData)
		{m_cglt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGLT_UDRD_D,T_CGLT_UDRD_D&>m_cglt;
};

#endif