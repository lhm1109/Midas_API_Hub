#ifndef __DB_LESFU_DB_H__
#define __DB_LESFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LESFU
{
public:
	CDB_LESFU()
	{
		m_lesf.InitHashTable(HASHSIZELESF);
	}
	virtual ~CDB_LESFU(){};

public:
	void Add(T_UDRD_KEY Key,T_LESF_UDRD_D& rData)
		{m_lesf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lesf.RemoveKey(Key);}
	void DelAll()
		{m_lesf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LESF_UDRD_D& rData)
		{return m_lesf.Lookup(Key,rData);}
	int GetCount()
		{return m_lesf.GetCount();}
	POSITION GetStart()
		{return m_lesf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LESF_UDRD_D& rData)
		{m_lesf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LESF_UDRD_D,T_LESF_UDRD_D&>m_lesf;
};

#endif