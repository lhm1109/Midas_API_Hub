#ifndef __DB_CPCOU_DB_H__
#define __DB_CPCOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPCOU
{
public:
	CDB_CPCOU()
	{
		m_cpco.InitHashTable(HASHSIZECPCO);
	}
	virtual ~CDB_CPCOU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPCO_UDRD_D& rData)
		{m_cpco.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cpco.RemoveKey(Key);}
	void DelAll()
		{m_cpco.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPCO_UDRD_D& rData)
		{return m_cpco.Lookup(Key,rData);}
	int GetCount()
		{return m_cpco.GetCount();}
	POSITION GetStart()
		{return m_cpco.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPCO_UDRD_D& rData)
		{m_cpco.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPCO_UDRD_D,T_CPCO_UDRD_D&>m_cpco;
};

#endif