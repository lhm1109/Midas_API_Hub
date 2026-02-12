#ifndef __DB_SPTL_DB_H__
#define __DB_SPTL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPTL
{
public:
	CDB_SPTL();
	virtual ~CDB_SPTL();

public:
	T_SPTL_K m_nStartNum;
	T_SPTL_K m_nLastNum;

public:
	void Add(T_SPTL_K Key, T_SPTL_D& rData);
	BOOL Del(T_SPTL_K Key);

public:
	BOOL Get(T_SPTL_K Key, T_SPTL_D& rData)
		{return m_sptl.Lookup(Key,rData);}
	int GetCount()
		{return m_sptl.GetCount();}
	POSITION GetStart()
		{return m_sptl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SPTL_K& rKey, T_SPTL_D& rData)
		{m_sptl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPTL_K, T_SPTL_K, T_SPTL_D, T_SPTL_D&> m_sptl;
};

#endif
