#ifndef __DB_CPFVU_DB_H__
#define __DB_CPFVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPFVU
{
public:
	CDB_CPFVU()
	{
		m_cpfv.InitHashTable(HASHSIZECPFV);
	}
	virtual ~CDB_CPFVU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPFV_UDRD_D& rData)
		{m_cpfv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cpfv.RemoveKey(Key);}
	void DelAll()
		{m_cpfv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPFV_UDRD_D& rData)
		{return m_cpfv.Lookup(Key,rData);}
	int GetCount()
		{return m_cpfv.GetCount();}
	POSITION GetStart()
		{return m_cpfv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPFV_UDRD_D& rData)
		{m_cpfv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPFV_UDRD_D,T_CPFV_UDRD_D&>m_cpfv;
};

#endif