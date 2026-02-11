#ifndef __DB_GSBG_DB_H__
#define __DB_GSBG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GSBG
{
public:
	CDB_GSBG();
	virtual ~CDB_GSBG();

public:
	T_GSBG_K m_nStartNum;
	T_GSBG_K m_nLastNum;

public:
	void Add(T_GSBG_K Key,T_GSBG_D& rData);
	BOOL Del(T_GSBG_K Key);

public:
	BOOL Get(T_GSBG_K Key,T_GSBG_D& rData)
		{return m_gsbg.Lookup(Key,rData);}
	int GetCount()
		{return m_gsbg.GetCount();}
	POSITION GetStart()
		{return m_gsbg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GSBG_K& rKey,T_GSBG_D& rData)
		{m_gsbg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GSBG_K,T_GSBG_K,T_GSBG_D,T_GSBG_D&>m_gsbg;
};

#endif