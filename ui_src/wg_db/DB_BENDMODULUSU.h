#ifndef __DB_BENDMODULUSU_DB_H__
#define __DB_BENDMODULUSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BENDMODULUSU
{
public:
	CDB_BENDMODULUSU()
	{
		m_bendmodulus.InitHashTable(HASHSIZEBENDMODULUS);
	}
	virtual ~CDB_BENDMODULUSU(){};

public:
	void Add(T_UDRD_KEY Key,T_BENDMODULUS_UDRD_D& rData)
		{m_bendmodulus.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_bendmodulus.RemoveKey(Key);}
	void DelAll()
		{m_bendmodulus.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BENDMODULUS_UDRD_D& rData)
		{return m_bendmodulus.Lookup(Key,rData);}
	int GetCount()
		{return m_bendmodulus.GetCount();}
	POSITION GetStart()
		{return m_bendmodulus.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BENDMODULUS_UDRD_D& rData)
		{m_bendmodulus.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BENDMODULUS_UDRD_D,T_BENDMODULUS_UDRD_D&>m_bendmodulus;
};

#endif