#ifndef __DB_MPCT_DB_H__
#define __DB_MPCT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MPCT
{
public:
	CDB_MPCT();
	CDB_MPCT(CDB_MPCT& src);
	CDB_MPCT& operator=(CDB_MPCT& src);
	virtual ~CDB_MPCT();

public:
	T_MPCT_K m_nStartNum;
	T_MPCT_K m_nLastNum;

public:
	void Add(T_MPCT_K Key,T_MPCT_D& rData);
	BOOL Del(T_MPCT_K Key);

public:
	BOOL Get(T_MPCT_K Key,T_MPCT_D& rData)
		{return m_mpct.Lookup(Key,rData);}
	int GetCount()
		{return m_mpct.GetCount();}
	POSITION GetStart()
		{return m_mpct.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MPCT_K& rKey,T_MPCT_D& rData)
		{m_mpct.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MPCT_K,T_MPCT_K,T_MPCT_D,T_MPCT_D&>m_mpct;
};

#endif
