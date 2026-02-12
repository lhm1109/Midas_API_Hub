#ifndef __DB_SDFCU_DB_H__
#define __DB_SDFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDFCU
{
public:
	CDB_SDFCU()
	{
		m_sdfc.InitHashTable(HASHSIZESDFC);
	}
	virtual ~CDB_SDFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDFC_UDRD_D& rData)
		{m_sdfc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdfc.RemoveKey(Key);}
	void DelAll()
		{m_sdfc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDFC_UDRD_D& rData)
		{return m_sdfc.Lookup(Key,rData);}
	int GetCount()
		{return m_sdfc.GetCount();}
	POSITION GetStart()
		{return m_sdfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDFC_UDRD_D& rData)
		{m_sdfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDFC_UDRD_D,T_SDFC_UDRD_D&>m_sdfc;
};

#endif