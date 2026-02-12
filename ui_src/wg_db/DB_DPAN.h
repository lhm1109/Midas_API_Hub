#ifndef __DB_DPAN_DB_H__
#define __DB_DPAN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_DPAN
{
public:
	CDB_DPAN();
	virtual ~CDB_DPAN();

public:
	T_DPAN_K m_nStartNum;
	T_DPAN_K m_nLastNum;

public:
	void Add(T_DPAN_K Key, T_DPAN_D& rData);
	BOOL Del(T_DPAN_K Key);

public:
	BOOL Get(T_DPAN_K Key,T_DPAN_D& rData)
		{return m_dpan.Lookup(Key,rData);}
	int GetCount()
		{return m_dpan.GetCount();}
	POSITION GetStart()
		{return m_dpan.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DPAN_K& rKey,T_DPAN_D& rData)
		{m_dpan.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DPAN_K,T_DPAN_K,T_DPAN_D,T_DPAN_D&>m_dpan;
};

#endif