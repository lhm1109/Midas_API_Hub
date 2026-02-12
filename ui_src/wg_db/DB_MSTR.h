#ifndef __DB_MSTR_DB_H__
#define __DB_MSTR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MSTR
{
public:
	CDB_MSTR();
	virtual ~CDB_MSTR();

public:
	T_MSTR_K m_nStartNum;
	T_MSTR_K m_nLastNum;

public:
	void Add(T_MSTR_K Key, T_MSTR_D& rData);
	BOOL Del(T_MSTR_K Key);

public:
	BOOL Get(T_MSTR_K Key,T_MSTR_D& rData)
		{return m_mstr.Lookup(Key,rData);}
	int GetCount()
		{return m_mstr.GetCount();}
	POSITION GetStart()
		{return m_mstr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MSTR_K& rKey,T_MSTR_D& rData)
		{m_mstr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MSTR_K,T_MSTR_K,T_MSTR_D,T_MSTR_D&>m_mstr;
};

#endif