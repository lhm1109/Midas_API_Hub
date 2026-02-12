#ifndef __DB_SDMSU_DB_H__
#define __DB_SDMSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDMSU
{
public:
	CDB_SDMSU()
	{
		m_sdms.InitHashTable(HASHSIZESDMS);
	}
	virtual ~CDB_SDMSU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDMS_UDRD_D& rData)
		{m_sdms.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdms.RemoveKey(Key);}
	void DelAll()
		{m_sdms.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDMS_UDRD_D& rData)
		{return m_sdms.Lookup(Key,rData);}
	int GetCount()
		{return m_sdms.GetCount();}
	POSITION GetStart()
		{return m_sdms.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDMS_UDRD_D& rData)
		{m_sdms.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDMS_UDRD_D,T_SDMS_UDRD_D&>m_sdms;
};

#endif