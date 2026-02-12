#ifndef __DB_EQMFU_DB_H__
#define __DB_EQMFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EQMFU
{
public:
	CDB_EQMFU()
	{
		m_eqmf.InitHashTable(HASHSIZEEQMF);
	}
	virtual ~CDB_EQMFU(){};

public:
	void Add(T_UDRD_KEY Key,T_EQMF_UDRD_D& rData)
		{m_eqmf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_eqmf.RemoveKey(Key);}
	void DelAll()
		{m_eqmf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EQMF_UDRD_D& rData)
		{return m_eqmf.Lookup(Key,rData);}
	int GetCount()
		{return m_eqmf.GetCount();}
	POSITION GetStart()
		{return m_eqmf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EQMF_UDRD_D& rData)
		{m_eqmf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_EQMF_UDRD_D,T_EQMF_UDRD_D&>m_eqmf;
};

#endif