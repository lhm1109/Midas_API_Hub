#ifndef __DB_HMTPU_DB_H__
#define __DB_HMTPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HMTPU
{
public:
	CDB_HMTPU()
	{
		m_hmtp.InitHashTable(HASHSIZEHMTP);
	}
	virtual ~CDB_HMTPU(){};

public:
	void Add(T_UDRD_KEY Key,T_HMTP_UDRD_D& rData)
		{m_hmtp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hmtp.RemoveKey(Key);}
	void DelAll()
		{m_hmtp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HMTP_UDRD_D& rData)
		{return m_hmtp.Lookup(Key,rData);}
	int GetCount()
		{return m_hmtp.GetCount();}
	POSITION GetStart()
		{return m_hmtp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HMTP_UDRD_D& rData)
		{m_hmtp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HMTP_UDRD_D,T_HMTP_UDRD_D&>m_hmtp;
};

#endif