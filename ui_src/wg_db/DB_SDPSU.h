#ifndef __DB_SDPSU_DB_H__
#define __DB_SDPSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDPSU
{
public:
	CDB_SDPSU()
	{
		m_sdps.InitHashTable(HASHSIZESDPS);
	}
	virtual ~CDB_SDPSU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDPS_UDRD_D& rData)
		{m_sdps.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdps.RemoveKey(Key);}
	void DelAll()
		{m_sdps.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDPS_UDRD_D& rData)
		{return m_sdps.Lookup(Key,rData);}
	int GetCount()
		{return m_sdps.GetCount();}
	POSITION GetStart()
		{return m_sdps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDPS_UDRD_D& rData)
		{m_sdps.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDPS_UDRD_D,T_SDPS_UDRD_D&>m_sdps;
};

#endif