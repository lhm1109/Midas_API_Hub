#ifndef __DB_SIHP_DB_H__
#define __DB_SIHP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SIHP
{
public:
	CDB_SIHP();
	CDB_SIHP(CDB_SIHP& src);
	CDB_SIHP& operator=(CDB_SIHP& src);
	virtual ~CDB_SIHP();

public:
	T_SIHP_K m_nLastNum;
	T_SIHP_K m_nStartNum;

public:
	void Add(T_SIHP_K Key,T_SIHP_D& rData);
	BOOL Del(T_SIHP_K Key);

public:
	BOOL Get(T_SIHP_K Key,T_SIHP_D& rData)
	{return m_sihp.Lookup(Key,rData);}
	int GetCount()
	{return m_sihp.GetCount();}
	POSITION GetStart()
	{return m_sihp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SIHP_K& rKey,T_SIHP_D& rData)
	{m_sihp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SIHP_K,T_SIHP_K,T_SIHP_D,T_SIHP_D&>m_sihp;
};

#endif
