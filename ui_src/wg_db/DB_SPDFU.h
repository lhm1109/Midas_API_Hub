#ifndef __DB_SPDFU_DB_H__
#define __DB_SPDFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPDFU
{
public:
	CDB_SPDFU()
	{
		m_spdf.InitHashTable(HASHSIZESPDF);
	}
	virtual ~CDB_SPDFU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPDF_UDRD_D& rData)
		{m_spdf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_spdf.RemoveKey(Key);}
	void DelAll()
		{m_spdf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPDF_UDRD_D& rData)
		{return m_spdf.Lookup(Key,rData);}
	int GetCount()
		{return m_spdf.GetCount();}
	POSITION GetStart()
		{return m_spdf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SPDF_UDRD_D& rData)
		{m_spdf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPDF_UDRD_D,T_SPDF_UDRD_D&>m_spdf;
};

#endif