#ifndef __DB_CFTRU_DB_H__
#define __DB_CFTRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CFTRU
{
public:
	CDB_CFTRU()
	{
		m_cftr.InitHashTable(HASHSIZECFTR);
	}
	virtual ~CDB_CFTRU(){};

public:
	void Add(T_UDRD_KEY Key,T_CFTR_UDRD_D& rData)
		{m_cftr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cftr.RemoveKey(Key);}
	void DelAll()
		{m_cftr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CFTR_UDRD_D& rData)
		{return m_cftr.Lookup(Key,rData);}
	int GetCount()
		{return m_cftr.GetCount();}
	POSITION GetStart()
		{return m_cftr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CFTR_UDRD_D& rData)
		{m_cftr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CFTR_UDRD_D,T_CFTR_UDRD_D&>m_cftr;
};

#endif