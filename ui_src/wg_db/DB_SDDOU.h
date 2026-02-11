#ifndef __DB_SDDOU_DB_H__
#define __DB_SDDOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDDOU
{
public:
	CDB_SDDOU()
	{
		m_sddo.InitHashTable(HASHSIZESDDO);
	}
	virtual ~CDB_SDDOU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDDO_UDRD_D& rData)
		{m_sddo.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sddo.RemoveKey(Key);}
	void DelAll()
		{m_sddo.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDDO_UDRD_D& rData)
		{return m_sddo.Lookup(Key,rData);}
	int GetCount()
		{return m_sddo.GetCount();}
	POSITION GetStart()
		{return m_sddo.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDDO_UDRD_D& rData)
		{m_sddo.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDDO_UDRD_D,T_SDDO_UDRD_D&>m_sddo;
};

#endif