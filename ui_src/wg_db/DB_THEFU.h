#ifndef __DB_THEFU_DB_H__
#define __DB_THEFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THEFU
{
public:
	CDB_THEFU()
	{
		m_thef.InitHashTable(HASHSIZETHEF);
	}
	virtual ~CDB_THEFU(){};

public:
	void Add(T_UDRD_KEY Key,T_THEF_UDRD_D& rData)
		{m_thef.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thef.RemoveKey(Key);}
	void DelAll()
		{m_thef.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THEF_UDRD_D& rData)
		{return m_thef.Lookup(Key,rData);}
	int GetCount()
		{return m_thef.GetCount();}
	POSITION GetStart()
		{return m_thef.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THEF_UDRD_D& rData)
		{m_thef.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THEF_UDRD_D,T_THEF_UDRD_D&>m_thef;
};

#endif