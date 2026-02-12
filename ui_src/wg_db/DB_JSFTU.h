#ifndef __DB_JSFTU_DB_H__
#define __DB_JSFTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_JSFTU
{
public:
	CDB_JSFTU()
	{
		m_jsft.InitHashTable(HASHSIZEJSFT);
	}
	virtual ~CDB_JSFTU(){};

public:
	void Add(T_UDRD_KEY Key,T_JSFT_UDRD_D& rData)
		{m_jsft.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_jsft.RemoveKey(Key);}
	void DelAll()
		{m_jsft.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JSFT_UDRD_D& rData)
		{return m_jsft.Lookup(Key,rData);}
	int GetCount()
		{return m_jsft.GetCount();}
	POSITION GetStart()
		{return m_jsft.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_JSFT_UDRD_D& rData)
		{m_jsft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JSFT_UDRD_D,T_JSFT_UDRD_D&>m_jsft;
};

#endif