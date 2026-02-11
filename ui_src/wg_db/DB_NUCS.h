#ifndef __DB_NUCS_DB_H__
#define __DB_NUCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NUCS
{
public:
	CDB_NUCS();
	virtual ~CDB_NUCS();

public:
	T_NUCS_K m_nStartNum;
	T_NUCS_K m_nLastNum;

public:
	void Add(T_NUCS_K Key,T_NUCS_D& rData);
	BOOL Del(T_NUCS_K Key);

public:
	BOOL Get(T_NUCS_K Key,T_NUCS_D& rData)
		{return m_nucs.Lookup(Key,rData);}
	int GetCount()
		{return m_nucs.GetCount();}
	POSITION GetStart()
		{return m_nucs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_NUCS_K& rKey,T_NUCS_D& rData)
		{m_nucs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_NUCS_K,T_NUCS_K,T_NUCS_D,T_NUCS_D&>m_nucs;
};

#endif