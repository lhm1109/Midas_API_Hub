#ifndef __DB_RIPB_DB_H__
#define __DB_RIPB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"


class CDB_RIPB
{
public:
	CDB_RIPB();
	virtual ~CDB_RIPB();

public:
	T_RIPB_K m_nStartNum;
	T_RIPB_K m_nLastNum;

public:
	void Add(T_RIPB_K Key, T_RIPB_D& rData);
	BOOL Del(T_RIPB_K Key);

public:
	BOOL Get(T_RIPB_K Key,T_RIPB_D& rData)
		{return m_ripb.Lookup(Key,rData);}
	int GetCount()
		{return m_ripb.GetCount();}
	POSITION GetStart()
		{return m_ripb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RIPB_K& rKey,T_RIPB_D& rData)
		{m_ripb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RIPB_K,T_RIPB_K,T_RIPB_D,T_RIPB_D&>m_ripb;
};

#endif