#ifndef __DB_PTMPU_DB_H__
#define __DB_PTMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PTMPU
{
public:
	CDB_PTMPU()
	{
		m_ptmp.InitHashTable(HASHSIZEPTMP);
	}
	virtual ~CDB_PTMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_PTMP_UDRD_D& rData)
		{m_ptmp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ptmp.RemoveKey(Key);}
	void DelAll()
		{m_ptmp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PTMP_UDRD_D& rData)
		{return m_ptmp.Lookup(Key,rData);}
	int GetCount()
		{return m_ptmp.GetCount();}
	POSITION GetStart()
		{return m_ptmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PTMP_UDRD_D& rData)
		{m_ptmp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PTMP_UDRD_D,T_PTMP_UDRD_D&>m_ptmp;
};

#endif