#ifndef __DB_FOUN_DB_H__
#define __DB_FOUN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FOUN
{
public:
	CDB_FOUN();
	virtual ~CDB_FOUN();

public:
	T_FOUN_K m_nStartNum;
	T_FOUN_K m_nLastNum;

public:
	void Add(T_FOUN_K Key,T_FOUN_D& rData);
	BOOL Del(T_FOUN_K Key);

public:
	BOOL Get(T_FOUN_K Key,T_FOUN_D& rData)
		{return m_foun.Lookup(Key,rData);}
	int GetCount()
		{return m_foun.GetCount();}
	POSITION GetStart()
		{return m_foun.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FOUN_K& rKey,T_FOUN_D& rData)
		{m_foun.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_FOUN_K,T_FOUN_K,T_FOUN_D,T_FOUN_D&>m_foun;
};

#endif