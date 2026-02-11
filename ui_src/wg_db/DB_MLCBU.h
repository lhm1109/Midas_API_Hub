#ifndef __DB_MLCBU_DB_H__
#define __DB_MLCBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MLCBU
{
public:
	CDB_MLCBU()
	{
		m_mlcb.InitHashTable(HASHSIZEMLCB);
	}
	virtual ~CDB_MLCBU(){};

public:
	void Add(T_UDRD_KEY Key,T_MLCB_UDRD_D& rData)
		{m_mlcb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mlcb.RemoveKey(Key);}
	void DelAll()
		{m_mlcb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MLCB_UDRD_D& rData)
		{return m_mlcb.Lookup(Key,rData);}
	
protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_MLCB_UDRD_D,T_MLCB_UDRD_D&>m_mlcb;
};

#endif