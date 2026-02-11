#ifndef __DB_SDAPU_DB_H__
#define __DB_SDAPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDAPU
{
public:
	CDB_SDAPU()
	{
		m_sdap.InitHashTable(HASHSIZESDAP);
	}
	virtual ~CDB_SDAPU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDAP_UDRD_D& rData)
		{m_sdap.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdap.RemoveKey(Key);}
	void DelAll()
		{m_sdap.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDAP_UDRD_D& rData)
		{return m_sdap.Lookup(Key,rData);}
	int GetCount()
		{return m_sdap.GetCount();}
	POSITION GetStart()
		{return m_sdap.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDAP_UDRD_D& rData)
		{m_sdap.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDAP_UDRD_D,T_SDAP_UDRD_D&>m_sdap;
};

#endif