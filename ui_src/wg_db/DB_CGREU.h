#ifndef __DB_CGREU_DB_H__
#define __DB_CGREU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGREU
{
public:
	CDB_CGREU()
	{
		m_cgre.InitHashTable(HASHSIZECGRE);
	}
	virtual ~CDB_CGREU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGRE_UDRD_D& rData)
		{m_cgre.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgre.RemoveKey(Key);}
	void DelAll()
		{m_cgre.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGRE_UDRD_D& rData)
		{return m_cgre.Lookup(Key,rData);}
	int GetCount()
		{return m_cgre.GetCount();}
	POSITION GetStart()
		{return m_cgre.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGRE_UDRD_D& rData)
		{m_cgre.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGRE_UDRD_D,T_CGRE_UDRD_D&>m_cgre;
};

#endif