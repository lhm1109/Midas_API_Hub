#ifndef __DB_RKCEU_DB_H__
#define __DB_RKCEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RKCEU
{
public:
	CDB_RKCEU()
	{
		m_rkce.InitHashTable(HASHSIZERKCE);
	}
	virtual ~CDB_RKCEU(){};

public:
	void Add(T_UDRD_KEY Key,T_RKCE_UDRD_D& rData)
		{m_rkce.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rkce.RemoveKey(Key);}
	void DelAll()
		{m_rkce.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RKCE_UDRD_D& rData)
		{return m_rkce.Lookup(Key,rData);}
	int GetCount()
		{return m_rkce.GetCount();}
	POSITION GetStart()
		{return m_rkce.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RKCE_UDRD_D& rData)
		{m_rkce.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RKCE_UDRD_D,T_RKCE_UDRD_D&>m_rkce;
};

#endif