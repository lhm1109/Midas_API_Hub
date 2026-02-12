#ifndef __DB_THRIU_DB_H__
#define __DB_THRIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THRIU
{
public:
	CDB_THRIU()
	{
		m_thri.InitHashTable(HASHSIZETHRI);
	}
	virtual ~CDB_THRIU(){};

public:
	void Add(T_UDRD_KEY Key,T_THRI_UDRD_D& rData)
		{m_thri.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thri.RemoveKey(Key);}
	void DelAll()
		{m_thri.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THRI_UDRD_D& rData)
		{return m_thri.Lookup(Key,rData);}
	int GetCount()
		{return m_thri.GetCount();}
	POSITION GetStart()
		{return m_thri.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THRI_UDRD_D& rData)
		{m_thri.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THRI_UDRD_D,T_THRI_UDRD_D&>m_thri;
};

#endif