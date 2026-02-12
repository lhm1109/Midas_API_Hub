#ifndef __DB_SIHG_DB_H__
#define __DB_SIHG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIHG
{
public:
	CDB_SIHG();
	CDB_SIHG(CDB_SIHG& src);
	CDB_SIHG& operator=(CDB_SIHG& src);
	virtual ~CDB_SIHG();

public:
	T_SIHG_K m_nLastNum;
	T_SIHG_K m_nStartNum;

public:
	void Add(T_SIHG_K Key,T_SIHG_D& rData);
	BOOL Del(T_SIHG_K Key);

public:
	BOOL Get(T_SIHG_K Key,T_SIHG_D& rData)
	{return m_sihg.Lookup(Key,rData);}
	int GetCount()
	{return m_sihg.GetCount();}
	POSITION GetStart()
	{return m_sihg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SIHG_K& rKey,T_SIHG_D& rData)
	{m_sihg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SIHG_K,T_SIHG_K,T_SIHG_D,T_SIHG_D&>m_sihg;
};

#endif
