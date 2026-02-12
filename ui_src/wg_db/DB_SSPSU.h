#ifndef __DB_SSPSU_DB_H__
#define __DB_SSPSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SSPSU
{
public:
	CDB_SSPSU()
	{
		m_ssps.InitHashTable(HASHSIZEPRES);
	}
	virtual ~CDB_SSPSU(){};

public:
	void Add(T_UDRD_KEY Key,T_SSPS_UDRD_D& rData)
		{m_ssps.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ssps.RemoveKey(Key);}
	void DelAll()
		{m_ssps.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SSPS_UDRD_D& rData)
		{return m_ssps.Lookup(Key,rData);}
	int GetCount()
		{return m_ssps.GetCount();}
	POSITION GetStart()
		{return m_ssps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SSPS_UDRD_D& rData)
		{m_ssps.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SSPS_UDRD_D,T_SSPS_UDRD_D&>m_ssps;
};

#endif

