#ifndef __DB_CUTLU_DB_H__
#define __DB_CUTLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CUTLU
{
public:
	CDB_CUTLU()
	{
		m_cutl.InitHashTable(HASHSIZECUTL);
	}
	virtual ~CDB_CUTLU(){};

public:
	void Add(T_UDRD_KEY Key,T_CUTL_UDRD_D& rData)
		{m_cutl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cutl.RemoveKey(Key);}
	void DelAll()
		{m_cutl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CUTL_UDRD_D& rData)
		{return m_cutl.Lookup(Key,rData);}
	int GetCount()
		{return m_cutl.GetCount();}
	POSITION GetStart()
		{return m_cutl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CUTL_UDRD_D& rData)
		{m_cutl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CUTL_UDRD_D,T_CUTL_UDRD_D&>m_cutl;
};

#endif