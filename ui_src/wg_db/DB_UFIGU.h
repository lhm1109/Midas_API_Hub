#ifndef __DB_UFIGU_DB_H__
#define __DB_UFIGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UFIGU
{
public:
	CDB_UFIGU()
	{
		m_ufig.InitHashTable(HASHSIZEUFIG);
	}
	virtual ~CDB_UFIGU(){};

public:
	void Add(T_UDRD_KEY Key,T_UFIG_UDRD_D& rData)
		{m_ufig.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ufig.RemoveKey(Key);}
	void DelAll()
		{m_ufig.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_UFIG_UDRD_D& rData)
		{return m_ufig.Lookup(Key,rData);}
	int GetCount()
		{return m_ufig.GetCount();}
	POSITION GetStart()
		{return m_ufig.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_UFIG_UDRD_D& rData)
		{m_ufig.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_UFIG_UDRD_D,T_UFIG_UDRD_D&>m_ufig;
};

#endif