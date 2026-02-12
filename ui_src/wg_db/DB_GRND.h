#ifndef __DB_GRND_DB_H__
#define __DB_GRND_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GRND
{
public:
	CDB_GRND();
	virtual ~CDB_GRND();

public:
	T_GRND_K m_nStartNum;
	T_GRND_K m_nLastNum;

public:
	void Add(T_GRND_K Key,T_GRND_D& rData);
	BOOL Del(T_GRND_K Key);

public:
	BOOL Get(T_GRND_K Key,T_GRND_D& rData)
		{return m_grnd.Lookup(Key,rData);}
	int GetCount()
		{return m_grnd.GetCount();}
	POSITION GetStart()
		{return m_grnd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GRND_K& rKey,T_GRND_D& rData)
		{m_grnd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GRND_K,T_GRND_K,T_GRND_D,T_GRND_D&>m_grnd;
};

#endif