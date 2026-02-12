#ifndef __DB_RDGNU_DB_H__
#define __DB_RDGNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RDGNU
{
public:
	CDB_RDGNU()
	{
		m_rdgn.InitHashTable(HASHSIZERDGN);
	}
	virtual ~CDB_RDGNU(){};

public:
	void Add(T_UDRD_KEY Key,T_RDGN_UDRD_D& rData)
		{m_rdgn.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rdgn.RemoveKey(Key);}
	void DelAll()
		{m_rdgn.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RDGN_UDRD_D& rData)
		{return m_rdgn.Lookup(Key,rData);}
	int GetCount()
		{return m_rdgn.GetCount();}
	POSITION GetStart()
		{return m_rdgn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RDGN_UDRD_D& rData)
		{m_rdgn.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RDGN_UDRD_D,T_RDGN_UDRD_D&>m_rdgn;
};

#endif