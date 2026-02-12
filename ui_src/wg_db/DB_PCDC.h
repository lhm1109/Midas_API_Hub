#ifndef __DB_PCDC_DB_H__
#define __DB_PCDC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_PCDC
{
public:
	CDB_PCDC();
	virtual ~CDB_PCDC();

public:
	T_PCDC_K m_nStartNum;
	T_PCDC_K m_nLastNum;

public:
	void Add(T_PCDC_K Key, T_PCDC_D& rData);
	BOOL Del(T_PCDC_K Key);

public:
	BOOL Get(T_PCDC_K Key,T_PCDC_D& rData)
		{return m_pcdc.Lookup(Key,rData);}
	int GetCount()
		{return m_pcdc.GetCount();}
	POSITION GetStart()
		{return m_pcdc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PCDC_K& rKey,T_PCDC_D& rData)
		{m_pcdc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PCDC_K,T_PCDC_K,T_PCDC_D,T_PCDC_D&>m_pcdc;
};

#endif