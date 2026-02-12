#ifndef __DB_WSSFU_DB_H__
#define __DB_WSSFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WSSFU
{
public:
	CDB_WSSFU()
	{
		m_wssf.InitHashTable(HASHSIZEWSSF);
	}
	virtual ~CDB_WSSFU(){};

public:
	void Add(T_UDRD_KEY Key,T_WSSF_UDRD_D& rData)
		{m_wssf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_wssf.RemoveKey(Key);}
	void DelAll()
		{m_wssf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_WSSF_UDRD_D& rData)
		{return m_wssf.Lookup(Key,rData);}
	int GetCount()
		{return m_wssf.GetCount();}
	POSITION GetStart()
		{return m_wssf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_WSSF_UDRD_D& rData)
		{m_wssf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_WSSF_UDRD_D,T_WSSF_UDRD_D&>m_wssf;
};

#endif