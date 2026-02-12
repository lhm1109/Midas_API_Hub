#ifndef __DB_TDAS_DB_H__
#define __DB_TDAS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDAS
{
public:
	CDB_TDAS();
	virtual ~CDB_TDAS();

public:
	T_TDAS_K m_nStartNum;
	T_TDAS_K m_nLastNum;

public:
	void Add(T_TDAS_K Key,T_TDAS_D& rData);
	BOOL Del(T_TDAS_K Key);

public:
	BOOL Get(T_TDAS_K Key,T_TDAS_D& rData)
		{return m_tdas.Lookup(Key,rData);}
	int GetCount()
		{return m_tdas.GetSize();}
	POSITION GetStart()
		{return m_tdas.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TDAS_K& rKey,T_TDAS_D& rData)
		{m_tdas.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	MHashMap<T_TDAS_K,T_TDAS_D>m_tdas;
};

#endif
