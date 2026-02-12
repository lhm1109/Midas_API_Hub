#ifndef __DB_SDST_DB_H__
#define __DB_SDST_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_SDST
{
public:
	CDB_SDST();
	virtual ~CDB_SDST();

public:
	T_SDST_K m_nStartNum;
	T_SDST_K m_nLastNum;

public:
	void Add(T_SDST_K Key, T_SDST_D& rData);
	BOOL Del(T_SDST_K Key);

public:
	BOOL Get(T_SDST_K Key,T_SDST_D& rData)
	{return m_sdst.Lookup(Key,rData);}
	int GetCount()
	{return m_sdst.GetCount();}
	POSITION GetStart()
	{return m_sdst.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDST_K& rKey,T_SDST_D& rData)
	{m_sdst.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDST_K,T_SDST_K,T_SDST_D,T_SDST_D&>m_sdst;
};

#endif