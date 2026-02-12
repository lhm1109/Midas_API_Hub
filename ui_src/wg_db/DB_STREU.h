#ifndef __DB_STREU_DB_H__
#define __DB_STREU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STREU
{
public:
	CDB_STREU()
	{
		m_stre.InitHashTable(HASHSIZESTRE);
	}
	virtual ~CDB_STREU(){};

public:
	void Add(T_UDRD_KEY Key,T_STRE_UDRD_D& rData)
		{m_stre.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stre.RemoveKey(Key);}
	void DelAll()
		{m_stre.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STRE_UDRD_D& rData)
		{return m_stre.Lookup(Key,rData);}
	int GetCount()
		{return m_stre.GetCount();}
	POSITION GetStart()
		{return m_stre.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STRE_UDRD_D& rData)
		{m_stre.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STRE_UDRD_D,T_STRE_UDRD_D&>m_stre;
};

#endif