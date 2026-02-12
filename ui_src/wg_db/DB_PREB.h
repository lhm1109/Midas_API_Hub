#ifndef __DB_PREB_DB_H__
#define __DB_PREB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PREB
{
public:
	CDB_PREB();
	virtual ~CDB_PREB();

public:
	T_PREB_K m_nStartNum;
	T_PREB_K m_nLastNum;

public:
	void Add(T_PREB_K Key, T_PREB_D& rData);
	BOOL Del(T_PREB_K Key);

public:
	BOOL Get(T_PREB_K Key,T_PREB_D& rData)
		{return m_preb.Lookup(Key,rData);}
	int GetCount()
		{return m_preb.GetCount();}
	POSITION GetStart()
		{return m_preb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PREB_K& rKey,T_PREB_D& rData)
		{m_preb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PREB_K,T_PREB_K,T_PREB_D,T_PREB_D&>m_preb;
};

#endif