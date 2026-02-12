#ifndef __DB_MPCC_DB_H__
#define __DB_MPCC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MPCC
{
public:
	CDB_MPCC();
	virtual ~CDB_MPCC();

public:
	T_MPCC_K m_nStartNum;
	T_MPCC_K m_nLastNum;

public:
	void Add(T_MPCC_K Key,T_MPCC_D& rData);
	BOOL Del(T_MPCC_K Key);

public:
	BOOL Get(T_MPCC_K Key,T_MPCC_D& rData)
		{return m_mpcc.Lookup(Key,rData);}
	int GetCount()
		{return m_mpcc.GetCount();}
	POSITION GetStart()
		{return m_mpcc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MPCC_K& rKey,T_MPCC_D& rData)
		{m_mpcc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MPCC_K,T_MPCC_K,T_MPCC_D,T_MPCC_D&>m_mpcc;
};

#endif