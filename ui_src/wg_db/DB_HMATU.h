#ifndef __DB_HMATU_DB_H__
#define __DB_HMATU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HMATU
{
public:
	CDB_HMATU()
	{
		m_hmat.InitHashTable(HASHSIZEHMAT);
	}
	virtual ~CDB_HMATU(){};

public:
	void Add(T_UDRD_KEY Key,T_HMAT_UDRD_D& rData)
		{m_hmat.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hmat.RemoveKey(Key);}
	void DelAll()
		{m_hmat.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HMAT_UDRD_D& rData)
		{return m_hmat.Lookup(Key,rData);}
	int GetCount()
		{return m_hmat.GetCount();}
	POSITION GetStart()
		{return m_hmat.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HMAT_UDRD_D& rData)
		{m_hmat.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HMAT_UDRD_D,T_HMAT_UDRD_D&>m_hmat;
};

#endif