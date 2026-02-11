#ifndef __DB_MHTPU_DB_H__
#define __DB_MHTPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MHTPU
{
public:
	CDB_MHTPU()
	{
		m_mhtp.InitHashTable(HASHSIZEMHTP);
	}
	virtual ~CDB_MHTPU(){};

public:
	void Add(T_UDRD_KEY Key,T_MHTP_UDRD_D& rData)
		{m_mhtp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mhtp.RemoveKey(Key);}
	void DelAll()
		{m_mhtp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MHTP_UDRD_D& rData)
		{return m_mhtp.Lookup(Key,rData);}
	int GetCount()
		{return m_mhtp.GetCount();}
	POSITION GetStart()
		{return m_mhtp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MHTP_UDRD_D& rData)
		{m_mhtp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MHTP_UDRD_D,T_MHTP_UDRD_D&>m_mhtp;
};

#endif