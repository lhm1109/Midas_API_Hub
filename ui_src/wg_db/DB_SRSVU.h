#ifndef __DB_SRSVU_DB_H__
#define __DB_SRSVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SRSVU
{
public:
	CDB_SRSVU()
	{
		m_srsv.InitHashTable(HASHSIZESRSV);
	}
	virtual ~CDB_SRSVU(){};

public:
	void Add(T_UDRD_KEY Key,T_SRSV_UDRD_D& rData)
		{m_srsv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_srsv.RemoveKey(Key);}
	void DelAll()
		{m_srsv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SRSV_UDRD_D& rData)
		{return m_srsv.Lookup(Key,rData);}
	int GetCount()
		{return m_srsv.GetCount();}
	POSITION GetStart()
		{return m_srsv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SRSV_UDRD_D& rData)
		{m_srsv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SRSV_UDRD_D,T_SRSV_UDRD_D&>m_srsv;
};

#endif