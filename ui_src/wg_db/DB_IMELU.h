#ifndef __DB_IMELU_DB_H__
#define __DB_IMELU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IMELU
{
public:
	CDB_IMELU()
	{
		m_imel.InitHashTable(HASHSIZEIMEL);
	}
	virtual ~CDB_IMELU(){};

public:
	void Add(T_UDRD_KEY Key,T_IMEL_UDRD_D& rData)
		{m_imel.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_imel.RemoveKey(Key);}
	void DelAll()
		{m_imel.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IMEL_UDRD_D& rData)
		{return m_imel.Lookup(Key,rData);}
	int GetCount()
		{return m_imel.GetCount();}
	POSITION GetStart()
		{return m_imel.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IMEL_UDRD_D& rData)
		{m_imel.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_IMEL_UDRD_D,T_IMEL_UDRD_D&>m_imel;
};

#endif