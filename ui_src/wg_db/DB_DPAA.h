#ifndef __DB_DPAA_DB_H__
#define __DB_DPAA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_DPAA
{
public:
	CDB_DPAA();
	virtual ~CDB_DPAA();

public:
	T_DPAA_K m_nStartNum;
	T_DPAA_K m_nLastNum;

public:
	void Add(T_DPAA_K Key, T_DPAA_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_DPAA_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_DPAA_K Key,T_DPAA_D& rData)
	{return m_dpaa.Lookup(Key,rData);}
	int GetCount()
	{return m_dpaa.GetCount();}
	POSITION GetStart()
	{return m_dpaa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DPAA_K& rKey,T_DPAA_D& rData)
	{m_dpaa.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DPAA_K,T_DPAA_K,T_DPAA_D,T_DPAA_D&>m_dpaa;
};

#endif