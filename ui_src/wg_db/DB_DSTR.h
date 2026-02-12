#ifndef __DB_DSTR_DB_H__
#define __DB_DSTR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DSTR
{
public:
	CDB_DSTR();
	virtual ~CDB_DSTR();

public:
	T_DSTR_K m_nStartNum;
	T_DSTR_K m_nLastNum;

public:
	void Add(T_DSTR_K Key, T_DSTR_D& rData);
	BOOL Del(T_DSTR_K Key);

public:
	BOOL Get(T_DSTR_K Key, T_DSTR_D& rData)
		{return m_dstr.Lookup(Key,rData);}
	int GetCount()
		{return m_dstr.GetCount();}
	POSITION GetStart()
		{return m_dstr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_DSTR_K& rKey, T_DSTR_D& rData)
		{m_dstr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DSTR_K, T_DSTR_K, T_DSTR_D, T_DSTR_D&> m_dstr;
};

#endif