#ifndef __DB_PBDC_DB_H__
#define __DB_PBDC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_PBDC
{
public:
	CDB_PBDC();
	virtual ~CDB_PBDC();

public:
	T_PBDC_K m_nStartNum;
	T_PBDC_K m_nLastNum;

public:
	void Add(T_PBDC_K Key, T_PBDC_D& rData);
	BOOL Del(T_PBDC_K Key);

public:
	BOOL Get(T_PBDC_K Key,T_PBDC_D& rData)
		{return m_pbdc.Lookup(Key,rData);}
	int GetCount()
		{return m_pbdc.GetCount();}
	POSITION GetStart()
		{return m_pbdc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PBDC_K& rKey,T_PBDC_D& rData)
		{m_pbdc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PBDC_K,T_PBDC_K,T_PBDC_D,T_PBDC_D&>m_pbdc;
};

#endif