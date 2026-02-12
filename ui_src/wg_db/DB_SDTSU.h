#ifndef __DB_SDTSU_DB_H__
#define __DB_SDTSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDTSU
{
public:
	CDB_SDTSU()
	{
		m_sdts.InitHashTable(HASHSIZESDTS);
	}
	virtual ~CDB_SDTSU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDTS_UDRD_D& rData)
		{m_sdts.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdts.RemoveKey(Key);}
	void DelAll()
		{m_sdts.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDTS_UDRD_D& rData)
		{return m_sdts.Lookup(Key,rData);}
	int GetCount()
		{return m_sdts.GetCount();}
	POSITION GetStart()
		{return m_sdts.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDTS_UDRD_D& rData)
		{m_sdts.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDTS_UDRD_D,T_SDTS_UDRD_D&>m_sdts;
};

#endif