#ifndef __DB_PCDD_DB_H__
#define __DB_PCDD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_PCDD
{
public:
	CDB_PCDD();
	virtual ~CDB_PCDD();

public:
	T_PCDD_K m_nStartNum;
	T_PCDD_K m_nLastNum;

public:
	void Add(T_PCDD_K Key, T_PCDD_D& rData);
	BOOL Del(T_PCDD_K Key);

public:
	BOOL Get(T_PCDD_K Key,T_PCDD_D& rData)
		{return m_pcdd.Lookup(Key,rData);}
	int GetCount()
		{return m_pcdd.GetCount();}
	POSITION GetStart()
		{return m_pcdd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PCDD_K& rKey,T_PCDD_D& rData)
		{m_pcdd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PCDD_K,T_PCDD_K,T_PCDD_D,T_PCDD_D&>m_pcdd;
};

#endif