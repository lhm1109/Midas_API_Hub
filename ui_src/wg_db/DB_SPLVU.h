#ifndef __DB_SPLVU_DB_H__
#define __DB_SPLVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPLVU
{
public:
	CDB_SPLVU()
	{
		m_splv.InitHashTable(HASHSIZESPLV);
	}
	virtual ~CDB_SPLVU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPLV_UDRD_D& rData)
		{m_splv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_splv.RemoveKey(Key);}
	void DelAll()
		{m_splv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPLV_UDRD_D& rData)
		{return m_splv.Lookup(Key,rData);}
	int GetCount()
		{return m_splv.GetCount();}
	POSITION GetStart()
		{return m_splv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SPLV_UDRD_D& rData)
		{m_splv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPLV_UDRD_D,T_SPLV_UDRD_D&>m_splv;
};

#endif