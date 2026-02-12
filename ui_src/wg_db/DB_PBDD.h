#ifndef __DB_PBDD_DB_H__
#define __DB_PBDD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_PBDD
{
public:
	CDB_PBDD();
	virtual ~CDB_PBDD();

public:
	T_PBDD_K m_nStartNum;
	T_PBDD_K m_nLastNum;

public:
	void Add(T_PBDD_K Key, T_PBDD_D& rData);
	BOOL Del(T_PBDD_K Key);

public:
	BOOL Get(T_PBDD_K Key,T_PBDD_D& rData)
		{return m_pbdd.Lookup(Key,rData);}
	int GetCount()
		{return m_pbdd.GetCount();}
	POSITION GetStart()
		{return m_pbdd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PBDD_K& rKey,T_PBDD_D& rData)
		{m_pbdd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PBDD_K,T_PBDD_K,T_PBDD_D,T_PBDD_D&>m_pbdd;
};

#endif