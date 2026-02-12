#ifndef __DB_CBFTU_DB_H__
#define __DB_CBFTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CBFTU
{
public:
	CDB_CBFTU()
	{
		m_cbft.InitHashTable(HASHSIZECBFT);
	}
	virtual ~CDB_CBFTU(){};

public:
	void Add(T_UDRD_KEY Key,T_CBFT_UDRD_D& rData)
		{m_cbft.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cbft.RemoveKey(Key);}
	void DelAll()
		{m_cbft.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CBFT_UDRD_D& rData)
		{return m_cbft.Lookup(Key,rData);}
	int GetCount()
		{return m_cbft.GetCount();}
	POSITION GetStart()
		{return m_cbft.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CBFT_UDRD_D& rData)
		{m_cbft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CBFT_UDRD_D,T_CBFT_UDRD_D&>m_cbft;
};

#endif