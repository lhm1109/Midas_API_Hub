#ifndef __DB_UFMTU_DB_H__
#define __DB_UFMTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UFMTU
{
public:
	CDB_UFMTU()
	{
		m_ufmt.InitHashTable(HASHSIZEUFMT);
	}
	virtual ~CDB_UFMTU(){};

public:
	void Add(T_UDRD_KEY Key,T_UFMT_UDRD_D& rData)
		{m_ufmt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ufmt.RemoveKey(Key);}
	void DelAll()
		{m_ufmt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_UFMT_UDRD_D& rData)
		{return m_ufmt.Lookup(Key,rData);}
	int GetCount()
		{return m_ufmt.GetCount();}
	POSITION GetStart()
		{return m_ufmt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_UFMT_UDRD_D& rData)
		{m_ufmt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_UFMT_UDRD_D,T_UFMT_UDRD_D&>m_ufmt;
};

#endif