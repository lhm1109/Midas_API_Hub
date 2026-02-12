#ifndef __DB_SSLAU_DB_H__
#define __DB_SSLAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SSLAU
{
public:
	CDB_SSLAU()
	{
		m_ssla.InitHashTable(HASHSIZESSLA);
	}
	virtual ~CDB_SSLAU(){};

public:
	void Add(T_UDRD_KEY Key,T_SSLA_UDRD_D& rData)
		{m_ssla.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ssla.RemoveKey(Key);}
	void DelAll()
		{m_ssla.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SSLA_UDRD_D& rData)
		{return m_ssla.Lookup(Key,rData);}
	int GetCount()
		{return m_ssla.GetCount();}
	POSITION GetStart()
		{return m_ssla.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SSLA_UDRD_D& rData)
		{m_ssla.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SSLA_UDRD_D,T_SSLA_UDRD_D&>m_ssla;
};

#endif