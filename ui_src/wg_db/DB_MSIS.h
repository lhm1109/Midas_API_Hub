#ifndef __DB_MSIS_DB_H__
#define __DB_MSIS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_MSIS
{
public:
	CDB_MSIS();
	virtual ~CDB_MSIS();

public:
	T_MSIS_K m_nStartNum;
	T_MSIS_K m_nLastNum;

public:
	void Add(T_MSIS_K Key, T_MSIS_D& rData);
	BOOL Del(T_MSIS_K Key);

public:
	BOOL Get(T_MSIS_K Key,T_MSIS_D& rData)
		{return m_msis.Lookup(Key,rData);}
	int GetCount()
		{return m_msis.GetCount();}
	POSITION GetStart()
		{return m_msis.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MSIS_K& rKey,T_MSIS_D& rData)
		{m_msis.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MSIS_K,T_MSIS_K,T_MSIS_D,T_MSIS_D&>m_msis;
};

#endif