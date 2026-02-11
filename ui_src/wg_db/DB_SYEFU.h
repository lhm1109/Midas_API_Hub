#ifndef __DB_SYEFU_DB_H__
#define __DB_SYEFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SYEFU
{
public:
	CDB_SYEFU()
	{
		m_syef.InitHashTable(HASHSIZESYEF);
	}
	virtual ~CDB_SYEFU(){};

public:
	void Add(T_UDRD_KEY Key,T_SYEF_UDRD_D& rData)
		{m_syef.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_syef.RemoveKey(Key);}
	void DelAll()
		{m_syef.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SYEF_UDRD_D& rData)
		{return m_syef.Lookup(Key,rData);}
	int GetCount()
		{return m_syef.GetCount();}
	POSITION GetStart()
		{return m_syef.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SYEF_UDRD_D& rData)
		{m_syef.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SYEF_UDRD_D,T_SYEF_UDRD_D&>m_syef;
};

#endif
