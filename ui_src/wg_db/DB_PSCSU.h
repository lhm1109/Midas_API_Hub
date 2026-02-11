#ifndef __DB_PSCSU_DB_H__
#define __DB_PSCSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PSCSU
{
public:
	CDB_PSCSU()
	{
		m_pscs.InitHashTable(HASHSIZEPSCS);
	}
	virtual ~CDB_PSCSU(){};

public:
	void Add(T_UDRD_KEY Key,T_PSCS_UDRD_D& rData)
	{m_pscs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_pscs.RemoveKey(Key);}
	void DelAll()
	{m_pscs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PSCS_UDRD_D& rData)
	{return m_pscs.Lookup(Key,rData);}
	int GetCount()
	{return m_pscs.GetCount();}
	POSITION GetStart()
	{return m_pscs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PSCS_UDRD_D& rData)
	{m_pscs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_PSCS_UDRD_D,T_PSCS_UDRD_D&>m_pscs;
};

#endif