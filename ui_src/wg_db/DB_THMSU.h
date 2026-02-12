#ifndef __DB_THMSU_DB_H__
#define __DB_THMSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THMSU
{
public:
	CDB_THMSU()
	{
		m_thms.InitHashTable(HASHSIZETHMS);
	}
	virtual ~CDB_THMSU(){};

public:
	void Add(T_UDRD_KEY Key,T_THMS_UDRD_D& rData)
		{m_thms.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thms.RemoveKey(Key);}
	void DelAll()
		{m_thms.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THMS_UDRD_D& rData)
		{return m_thms.Lookup(Key,rData);}
	int GetCount()
		{return m_thms.GetCount();}
	POSITION GetStart()
		{return m_thms.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THMS_UDRD_D& rData)
		{m_thms.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THMS_UDRD_D,T_THMS_UDRD_D&>m_thms;
};

#endif