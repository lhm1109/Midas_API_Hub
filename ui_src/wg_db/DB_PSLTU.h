#ifndef __DB_PSLTU_DB_H__
#define __DB_PSLTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PSLTU
{
public:
	CDB_PSLTU()
	{
		m_pslt.InitHashTable(HASHSIZEPSLT);
	}
	virtual ~CDB_PSLTU(){};

public:
	void Add(T_UDRD_KEY Key,T_PSLT_UDRD_D& rData)
	{m_pslt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_pslt.RemoveKey(Key);}
	void DelAll()
	{m_pslt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PSLT_UDRD_D& rData)
	{return m_pslt.Lookup(Key,rData);}
	int GetCount()
	{return m_pslt.GetCount();}
	POSITION GetStart()
	{return m_pslt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PSLT_UDRD_D& rData)
	{m_pslt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_PSLT_UDRD_D,T_PSLT_UDRD_D&>m_pslt;
};

#endif