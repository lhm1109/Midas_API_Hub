#ifndef __DB_SDHYU_DB_H__
#define __DB_SDHYU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDHYU
{
public:
	CDB_SDHYU()
	{
		m_sdhy.InitHashTable(HASHSIZESDHY);
	}
	virtual ~CDB_SDHYU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDHY_UDRD_D& rData)
		{m_sdhy.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdhy.RemoveKey(Key);}
	void DelAll()
		{m_sdhy.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDHY_UDRD_D& rData)
		{return m_sdhy.Lookup(Key,rData);}
	int GetCount()
		{return m_sdhy.GetCount();}
	POSITION GetStart()
		{return m_sdhy.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDHY_UDRD_D& rData)
		{m_sdhy.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDHY_UDRD_D,T_SDHY_UDRD_D&>m_sdhy;
};

#endif