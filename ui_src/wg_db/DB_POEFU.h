#ifndef __DB_POEFU_DB_H__
#define __DB_POEFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POEFU
{
public:
	CDB_POEFU()
	{
		m_poef.InitHashTable(HASHSIZEPOEF);
	}
	virtual ~CDB_POEFU(){};

public:
	void Add(T_UDRD_KEY Key,T_POEF_UDRD_D& rData)
		{m_poef.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_poef.RemoveKey(Key);}
	void DelAll()
		{m_poef.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_POEF_UDRD_D& rData)
		{return m_poef.Lookup(Key,rData);}
	int GetCount()
		{return m_poef.GetCount();}
	POSITION GetStart()
		{return m_poef.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_POEF_UDRD_D& rData)
		{m_poef.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_POEF_UDRD_D,T_POEF_UDRD_D&>m_poef;
};

#endif