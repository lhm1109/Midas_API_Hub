#ifndef __DB_SSLC_DB_H__
#define __DB_SSLC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_SSLC
{
public:
	CDB_SSLC();
	virtual ~CDB_SSLC();

public:
	T_SSLC_K m_nStartNum;
	T_SSLC_K m_nLastNum;

public:
	void Add(T_SSLC_K Key, T_SSLC_D& rData);
	BOOL Del(T_SSLC_K Key);

public:
	BOOL Get(T_SSLC_K Key,T_SSLC_D& rData)
		{return m_sslc.Lookup(Key,rData);}
	int GetCount()
		{return m_sslc.GetCount();}
	POSITION GetStart()
		{return m_sslc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SSLC_K& rKey,T_SSLC_D& rData)
		{m_sslc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SSLC_K,T_SSLC_K,T_SSLC_D,T_SSLC_D&>m_sslc;
};

#endif