#ifndef __DB_CGSCU_DB_H__
#define __DB_CGSCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGSCU
{
public:
	CDB_CGSCU()
	{
		m_cgsc.InitHashTable(HASHSIZECGSC);
	}
	virtual ~CDB_CGSCU(){};

public:
	void Add(T_UDRD_KEY Key,T_CGSC_UDRD_D& rData)
		{m_cgsc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgsc.RemoveKey(Key);}
	void DelAll()
		{m_cgsc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGSC_UDRD_D& rData)
		{return m_cgsc.Lookup(Key,rData);}
	int GetCount()
		{return m_cgsc.GetCount();}
	POSITION GetStart()
		{return m_cgsc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGSC_UDRD_D& rData)
		{m_cgsc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGSC_UDRD_D,T_CGSC_UDRD_D&>m_cgsc;
};

#endif