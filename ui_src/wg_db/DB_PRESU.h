#ifndef __DB_PRESU_DB_H__
#define __DB_PRESU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PRESU
{
public:
	CDB_PRESU()
	{
		m_pres.InitHashTable(HASHSIZEPRES);
	}
	virtual ~CDB_PRESU(){};

public:
	void Add(T_UDRD_KEY Key,T_PRES_UDRD_D& rData)
		{m_pres.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pres.RemoveKey(Key);}
	void DelAll()
		{m_pres.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PRES_UDRD_D& rData)
		{return m_pres.Lookup(Key,rData);}
	int GetCount()
		{return m_pres.GetCount();}
	POSITION GetStart()
		{return m_pres.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PRES_UDRD_D& rData)
		{m_pres.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PRES_UDRD_D,T_PRES_UDRD_D&>m_pres;
};

#endif