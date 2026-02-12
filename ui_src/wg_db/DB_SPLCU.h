#ifndef __DB_SPLCU_DB_H__
#define __DB_SPLCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPLCU
{
public:
	CDB_SPLCU()
	{
		m_splc.InitHashTable(HASHSIZESPLC);
	}
	virtual ~CDB_SPLCU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPLC_UDRD_D& rData)
		{m_splc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_splc.RemoveKey(Key);}
	void DelAll()
		{m_splc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPLC_UDRD_D& rData)
		{return m_splc.Lookup(Key,rData);}
	int GetCount()
		{return m_splc.GetCount();}
	POSITION GetStart()
		{return m_splc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SPLC_UDRD_D& rData)
		{m_splc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPLC_UDRD_D,T_SPLC_UDRD_D&>m_splc;
};

#endif