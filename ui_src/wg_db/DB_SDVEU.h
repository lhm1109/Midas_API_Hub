#ifndef __DB_SDVEU_DB_H__
#define __DB_SDVEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDVEU
{
public:
	CDB_SDVEU()
	{
		m_sdve.InitHashTable(HASHSIZESDVE);
	}
	virtual ~CDB_SDVEU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDVE_UDRD_D& rData)
		{m_sdve.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdve.RemoveKey(Key);}
	void DelAll()
		{m_sdve.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDVE_UDRD_D& rData)
		{return m_sdve.Lookup(Key,rData);}
	int GetCount()
		{return m_sdve.GetCount();}
	POSITION GetStart()
		{return m_sdve.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDVE_UDRD_D& rData)
		{m_sdve.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDVE_UDRD_D,T_SDVE_UDRD_D&>m_sdve;
};

#endif