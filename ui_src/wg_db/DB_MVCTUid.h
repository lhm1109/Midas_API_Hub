#ifndef __DB_MVCTUid_DB_H__
#define __DB_MVCTUid_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVCTUid
{
public:
	CDB_MVCTUid()
	{
		m_mvctid.InitHashTable(HASHSIZEMVCTid);
	}
	virtual ~CDB_MVCTUid(){};

public:
	void Add(T_UDRD_KEY Key,T_MVCTid_UDRD_D& rData)
		{m_mvctid.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvctid.RemoveKey(Key);}
	void DelAll()
		{m_mvctid.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVCTid_UDRD_D& rData)
		{return m_mvctid.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVCTid_UDRD_D,T_MVCTid_UDRD_D&>m_mvctid;
};

#endif