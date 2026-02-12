#ifndef __DB_SDIS_DB_H__
#define __DB_SDIS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_SDIS
{
public:
	CDB_SDIS();
	virtual ~CDB_SDIS();

public:
	T_SDIS_K m_nStartNum;
	T_SDIS_K m_nLastNum;

public:
	void Add(T_SDIS_K Key, T_SDIS_D& rData);
	BOOL Del(T_SDIS_K Key);

public:
	BOOL Get(T_SDIS_K Key,T_SDIS_D& rData)
	{return m_sdis.Lookup(Key,rData);}
	int GetCount()
	{return m_sdis.GetCount();}
	POSITION GetStart()
	{return m_sdis.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDIS_K& rKey,T_SDIS_D& rData)
	{m_sdis.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDIS_K,T_SDIS_K,T_SDIS_D,T_SDIS_D&>m_sdis;
};

#endif