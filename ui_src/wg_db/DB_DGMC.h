#ifndef __DB_DGMC_DB_H__
#define __DB_DGMC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STAG;

class CDB_DGMC
{
public:
	CDB_DGMC();
	virtual ~CDB_DGMC();

public:
	void Add(T_DGMC_K Key,T_DGMC_D& rData);
	BOOL Del(T_DGMC_K Key);

public:
	BOOL Get(T_DGMC_K Key,T_DGMC_D& rData)
		{return m_dgmc.Lookup(Key,rData);}
	int GetCount()
		{return m_dgmc.GetCount();}
	POSITION GetStart()
		{return m_dgmc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DGMC_K& rKey,T_DGMC_D& rData)
		{m_dgmc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DGMC_K,T_DGMC_K,T_DGMC_D,T_DGMC_D&>m_dgmc;
};

#endif