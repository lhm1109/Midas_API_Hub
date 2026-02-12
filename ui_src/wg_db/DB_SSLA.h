#ifndef __DB_SSLA_DB_H__
#define __DB_SSLA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_SSLA
{
public:
	CDB_SSLA();
	virtual ~CDB_SSLA();

public:
	T_SSLA_K m_nStartNum;
	T_SSLA_K m_nLastNum;

public:
	void Add(T_SSLA_K Key, T_SSLA_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SSLA_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SSLA_K Key,T_SSLA_D& rData)
	{return m_ssla.Lookup(Key,rData);}
	int GetCount()
	{return m_ssla.GetCount();}
	POSITION GetStart()
	{return m_ssla.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SSLA_K& rKey,T_SSLA_D& rData)
	{m_ssla.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SSLA_K,T_SSLA_K,T_SSLA_D,T_SSLA_D&>m_ssla;
};

#endif