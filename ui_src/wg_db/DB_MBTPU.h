#ifndef __DB_MBTPU_DB_H__
#define __DB_MBTPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MBTPU
{
public:
	CDB_MBTPU()
	{
		m_mbtp.InitHashTable(HASHSIZEMBTP);
	}
	virtual ~CDB_MBTPU(){};

public:
	void Add(T_UDRD_KEY Key,T_MBTP_UDRD_D& rData)
		{m_mbtp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mbtp.RemoveKey(Key);}
	void DelAll()
		{m_mbtp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MBTP_UDRD_D& rData)
		{return m_mbtp.Lookup(Key,rData);}
	int GetCount()
		{return m_mbtp.GetCount();}
	POSITION GetStart()
		{return m_mbtp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MBTP_UDRD_D& rData)
		{m_mbtp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MBTP_UDRD_D,T_MBTP_UDRD_D&>m_mbtp;
};

#endif