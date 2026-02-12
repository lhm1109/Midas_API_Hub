#ifndef __DB_PRCS_DB_H__
#define __DB_PRCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PRCS
{
public:
	CDB_PRCS();
	virtual ~CDB_PRCS();

public:
	T_PRCS_K m_nStartNum;
	T_PRCS_K m_nLastNum;

public:
	void Add(T_PRCS_K Key, T_PRCS_D& rData);
	BOOL Del(T_PRCS_K Key);

public:
	BOOL Get(T_PRCS_K Key,T_PRCS_D& rData)
		{return m_prcs.Lookup(Key,rData);}
	int GetCount()
		{return m_prcs.GetCount();}
	POSITION GetStart()
		{return m_prcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PRCS_K& rKey,T_PRCS_D& rData)
		{m_prcs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PRCS_K,T_PRCS_K,T_PRCS_D,T_PRCS_D&>m_prcs;
};

#endif