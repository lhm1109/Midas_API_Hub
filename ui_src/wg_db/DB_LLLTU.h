#ifndef __DB_LLLTU_DB_H__
#define __DB_LLLTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LLLTU
{
public:
	CDB_LLLTU()
	{
		m_lllt.InitHashTable(HASHSIZELLLT);
	}
	virtual ~CDB_LLLTU(){};

public:
	void Add(T_UDRD_KEY Key,T_LLLT_UDRD_D& rData)
		{m_lllt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lllt.RemoveKey(Key);}
	void DelAll()
		{m_lllt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LLLT_UDRD_D& rData)
		{return m_lllt.Lookup(Key,rData);}
	int GetCount()
		{return m_lllt.GetCount();}
	POSITION GetStart()
		{return m_lllt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LLLT_UDRD_D& rData)
		{m_lllt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LLLT_UDRD_D,T_LLLT_UDRD_D&>m_lllt;
};

#endif