#ifndef __DB_CPFPU_DB_H__
#define __DB_CPFPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPFPU
{
public:
	CDB_CPFPU()
	{
		m_cpfp.InitHashTable(HASHSIZECPFP);
	}
	virtual ~CDB_CPFPU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPFP_UDRD_D& rData)
		{m_cpfp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cpfp.RemoveKey(Key);}
	void DelAll()
		{m_cpfp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPFP_UDRD_D& rData)
		{return m_cpfp.Lookup(Key,rData);}
	int GetCount()
		{return m_cpfp.GetCount();}
	POSITION GetStart()
		{return m_cpfp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPFP_UDRD_D& rData)
		{m_cpfp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPFP_UDRD_D,T_CPFP_UDRD_D&>m_cpfp;
};

#endif