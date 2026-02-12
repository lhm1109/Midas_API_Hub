#ifndef __DB_GSPRU_DB_H__
#define __DB_GSPRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GSPRU
{
public:
	CDB_GSPRU()
	{
		m_gspr.InitHashTable(HASHSIZEGSPR);
	}
	virtual ~CDB_GSPRU(){};

public:
	void Add(T_UDRD_KEY Key,T_GSPR_UDRD_D& rData)
		{m_gspr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gspr.RemoveKey(Key);}
	void DelAll()
		{m_gspr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GSPR_UDRD_D& rData)
		{return m_gspr.Lookup(Key,rData);}
	int GetCount()
		{return m_gspr.GetCount();}
	POSITION GetStart()
		{return m_gspr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GSPR_UDRD_D& rData)
		{m_gspr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GSPR_UDRD_D,T_GSPR_UDRD_D&>m_gspr;
};

#endif