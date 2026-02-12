#ifndef __DB_SDHY_DB_H__
#define __DB_SDHY_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_SDHY
{
public:
	CDB_SDHY();
	virtual ~CDB_SDHY();

public:
	T_SDHY_K m_nStartNum;
	T_SDHY_K m_nLastNum;

public:
	void Add(T_SDHY_K Key, T_SDHY_D& rData);
	BOOL Del(T_SDHY_K Key);

public:
	BOOL Get(T_SDHY_K Key,T_SDHY_D& rData)
	{return m_sdhy.Lookup(Key,rData);}
	int GetCount()
	{return m_sdhy.GetCount();}
	POSITION GetStart()
	{return m_sdhy.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDHY_K& rKey,T_SDHY_D& rData)
	{m_sdhy.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDHY_K,T_SDHY_K,T_SDHY_D,T_SDHY_D&>m_sdhy;
};

#endif