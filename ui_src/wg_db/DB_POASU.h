#ifndef __DB_POASU_DB_H__
#define __DB_POASU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POASU
{
public:
	CDB_POASU()
	{
		m_poas.InitHashTable(HASHSIZEPOAS);
	}
	virtual ~CDB_POASU(){};

public:
	void Add(T_UDRD_KEY Key,T_POAS_UDRD_D& rData)
		{m_poas.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_poas.RemoveKey(Key);}
	void DelAll()
		{m_poas.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_POAS_UDRD_D& rData)
		{return m_poas.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_POAS_UDRD_D,T_POAS_UDRD_D&>m_poas;
};

#endif