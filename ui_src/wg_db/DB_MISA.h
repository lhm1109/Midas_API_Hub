#ifndef __DB_MISA_DB_H__
#define __DB_MISA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_MISA
{
public:
	CDB_MISA();
	virtual ~CDB_MISA();

public:
	T_MISA_K m_nStartNum;
	T_MISA_K m_nLastNum;

public:
	void Add(T_MISA_K Key, T_MISA_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_MISA_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_MISA_K Key,T_MISA_D& rData)
	{return m_misa.Lookup(Key,rData);}
	int GetCount()
	{return m_misa.GetCount();}
	POSITION GetStart()
	{return m_misa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MISA_K& rKey,T_MISA_D& rData)
	{m_misa.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MISA_K,T_MISA_K,T_MISA_D,T_MISA_D&>m_misa;
};

#endif