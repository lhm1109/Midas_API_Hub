#ifndef __DB_CMFTU_DB_H__
#define __DB_CMFTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CMFTU
{
public:
	CDB_CMFTU()
	{
		m_cmft.InitHashTable(HASHSIZECMFT);
	}
	virtual ~CDB_CMFTU(){};

public:
	void Add(T_UDRD_KEY Key,T_CMFT_UDRD_D& rData)
		{m_cmft.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cmft.RemoveKey(Key);}
	void DelAll()
		{m_cmft.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CMFT_UDRD_D& rData)
		{return m_cmft.Lookup(Key,rData);}
	int GetCount()
		{return m_cmft.GetCount();}
	POSITION GetStart()
		{return m_cmft.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CMFT_UDRD_D& rData)
		{m_cmft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CMFT_UDRD_D,T_CMFT_UDRD_D&>m_cmft;
};

#endif