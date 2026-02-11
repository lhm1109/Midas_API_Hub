#ifndef __DB_SDISU_DB_H__
#define __DB_SDISU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDISU
{
public:
	CDB_SDISU()
	{
		m_sdis.InitHashTable(HASHSIZESDIS);
	}
	virtual ~CDB_SDISU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDIS_UDRD_D& rData)
		{m_sdis.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdis.RemoveKey(Key);}
	void DelAll()
		{m_sdis.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDIS_UDRD_D& rData)
		{return m_sdis.Lookup(Key,rData);}
	int GetCount()
		{return m_sdis.GetCount();}
	POSITION GetStart()
		{return m_sdis.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDIS_UDRD_D& rData)
		{m_sdis.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDIS_UDRD_D,T_SDIS_UDRD_D&>m_sdis;
};

#endif