#ifndef __DB_SDVI_DB_H__
#define __DB_SDVI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_SDVI
{
public:
	CDB_SDVI();
	virtual ~CDB_SDVI();

public:
	T_SDVI_K m_nStartNum;
	T_SDVI_K m_nLastNum;

public:
	void Add(T_SDVI_K Key, T_SDVI_D& rData);
	BOOL Del(T_SDVI_K Key);

public:
	BOOL Get(T_SDVI_K Key,T_SDVI_D& rData)
	{return m_sdvi.Lookup(Key,rData);}
	int GetCount()
	{return m_sdvi.GetCount();}
	POSITION GetStart()
	{return m_sdvi.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDVI_K& rKey,T_SDVI_D& rData)
	{m_sdvi.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDVI_K,T_SDVI_K,T_SDVI_D,T_SDVI_D&>m_sdvi;
};

#endif