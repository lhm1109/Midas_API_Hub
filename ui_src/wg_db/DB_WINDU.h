#ifndef __DB_WINDU_DB_H__
#define __DB_WINDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WINDU
{
public:
	CDB_WINDU()
	{
		m_wind.InitHashTable(HASHSIZEWIND);
	}
	virtual ~CDB_WINDU(){};

public:
	void Add(T_UDRD_KEY Key,T_WIND_UDRD_D& rData)
		{m_wind.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_wind.RemoveKey(Key);}
	void DelAll()
		{m_wind.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_WIND_UDRD_D& rData)
		{return m_wind.Lookup(Key,rData);}
	int GetCount()
		{return m_wind.GetCount();}
	POSITION GetStart()
		{return m_wind.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_WIND_UDRD_D& rData)
		{m_wind.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_WIND_UDRD_D,T_WIND_UDRD_D&>m_wind;
};

#endif