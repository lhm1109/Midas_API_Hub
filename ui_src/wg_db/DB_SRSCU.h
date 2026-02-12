#ifndef __DB_SRSCU_DB_H__
#define __DB_SRSCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SRSCU
{
public:
	CDB_SRSCU()
	{
		m_srsc.InitHashTable(HASHSIZESRSC);
	}
	virtual ~CDB_SRSCU(){};

public:
	void Add(T_UDRD_KEY Key,T_SRSC_UDRD_D& rData)
		{m_srsc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_srsc.RemoveKey(Key);}
	void DelAll()
		{m_srsc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SRSC_UDRD_D& rData)
		{return m_srsc.Lookup(Key,rData);}
	int GetCount()
		{return m_srsc.GetCount();}
	POSITION GetStart()
		{return m_srsc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SRSC_UDRD_D& rData)
		{m_srsc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SRSC_UDRD_D,T_SRSC_UDRD_D&>m_srsc;
};

#endif