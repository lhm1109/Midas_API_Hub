#ifndef __DB_SDVE_DB_H__
#define __DB_SDVE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_SDVE
{
public:
	CDB_SDVE();
	virtual ~CDB_SDVE();

public:
	T_SDVE_K m_nStartNum;
	T_SDVE_K m_nLastNum;

public:
	void Add(T_SDVE_K Key, T_SDVE_D& rData);
	BOOL Del(T_SDVE_K Key);

public:
	BOOL Get(T_SDVE_K Key,T_SDVE_D& rData)
	{return m_sdve.Lookup(Key,rData);}
	int GetCount()
	{return m_sdve.GetCount();}
	POSITION GetStart()
	{return m_sdve.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDVE_K& rKey,T_SDVE_D& rData)
	{m_sdve.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDVE_K,T_SDVE_K,T_SDVE_D,T_SDVE_D&>m_sdve;
};

#endif