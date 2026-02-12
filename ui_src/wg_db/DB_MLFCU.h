#ifndef __DB_MLFCU_DB_H__
#define __DB_MLFCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MLFCU
{
public:
	CDB_MLFCU()
	{
		m_mlfc.InitHashTable(HASHSIZEMLFC);
	}
	virtual ~CDB_MLFCU(){};

public:
	void Add(T_UDRD_KEY Key,T_MLFC_UDRD_D& rData)
		{m_mlfc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mlfc.RemoveKey(Key);}
	void DelAll()
		{m_mlfc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MLFC_UDRD_D& rData)
		{return m_mlfc.Lookup(Key,rData);}
	int GetCount()
		{return m_mlfc.GetCount();}
	POSITION GetStart()
		{return m_mlfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MLFC_UDRD_D& rData)
		{m_mlfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MLFC_UDRD_D,T_MLFC_UDRD_D&>m_mlfc;
};

#endif