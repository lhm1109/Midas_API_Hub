#ifndef __DB_COLC_DB_H__
#define __DB_COLC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_COLC
{
public:
	CDB_COLC();
	virtual ~CDB_COLC();

public:
	T_COLC_K m_nStartNum;
	T_COLC_K m_nLastNum;

public:
	void Add(T_COLC_K Key, T_COLC_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_COLC_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_COLC_K Key,T_COLC_D& rData)
	{return m_colc.Lookup(Key,rData);}
	int GetCount()
	{return m_colc.GetCount();}
	POSITION GetStart()
	{return m_colc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_COLC_K& rKey,T_COLC_D& rData)
	{m_colc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_COLC_K,T_COLC_K,T_COLC_D,T_COLC_D&>m_colc;
};

#endif