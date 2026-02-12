#ifndef __DB_POSCU_DB_H__
#define __DB_POSCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POSCU
{
public:
	CDB_POSCU()
	{
		m_posc.InitHashTable(HASHSIZEPOSC);
	}
	virtual ~CDB_POSCU(){};

public:
	void Add(T_UDRD_KEY Key,T_POSC_UDRD_D& rData)
		{m_posc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_posc.RemoveKey(Key);}
	void DelAll()
		{m_posc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_POSC_UDRD_D& rData)
		{return m_posc.Lookup(Key,rData);}
	int GetCount()
		{return m_posc.GetCount();}
	POSITION GetStart()
		{return m_posc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_POSC_UDRD_D& rData)
		{m_posc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_POSC_UDRD_D,T_POSC_UDRD_D&>m_posc;
};

#endif