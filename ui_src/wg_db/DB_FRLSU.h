#ifndef __DB_FRLSU_DB_H__
#define __DB_FRLSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FRLSU
{
public:
	CDB_FRLSU()
	{
		m_frls.InitHashTable(HASHSIZEFRLS);
	}
	virtual ~CDB_FRLSU(){};

public:
	void Add(T_UDRD_KEY Key,T_FRLS_UDRD_D& rData)
		{m_frls.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_frls.RemoveKey(Key);}
	void DelAll()
		{m_frls.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FRLS_UDRD_D& rData)
		{return m_frls.Lookup(Key,rData);}
	int GetCount()
		{return m_frls.GetCount();}
	POSITION GetStart()
		{return m_frls.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FRLS_UDRD_D& rData)
		{m_frls.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FRLS_UDRD_D,T_FRLS_UDRD_D&>m_frls;
};

#endif