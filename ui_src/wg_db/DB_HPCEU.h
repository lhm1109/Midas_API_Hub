#ifndef __DB_HPCEU_DB_H__
#define __DB_HPCEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HPCEU
{
public:
	CDB_HPCEU()
	{
		m_hpce.InitHashTable(HASHSIZEHPCE);
	}
	virtual ~CDB_HPCEU(){};

public:
	void Add(T_UDRD_KEY Key,T_HPCE_UDRD_D& rData)
		{m_hpce.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hpce.RemoveKey(Key);}
	void DelAll()
		{m_hpce.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HPCE_UDRD_D& rData)
		{return m_hpce.Lookup(Key,rData);}
	int GetCount()
		{return m_hpce.GetCount();}
	POSITION GetStart()
		{return m_hpce.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HPCE_UDRD_D& rData)
		{m_hpce.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HPCE_UDRD_D,T_HPCE_UDRD_D&>m_hpce;
};

#endif
