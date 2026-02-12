#ifndef __DB_TDCS_DB_H__
#define __DB_TDCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDCS
{
public:
	CDB_TDCS();
	CDB_TDCS(CDB_TDCS& src);
	CDB_TDCS& operator=(CDB_TDCS& src);
	virtual ~CDB_TDCS();

public:
	T_TDCS_K m_nLastNum;
	T_TDCS_K m_nStartNum;

public:
	void Add(T_TDCS_K Key,T_TDCS_D& rData);
	BOOL Del(T_TDCS_K Key);

public:
	BOOL Get(T_TDCS_K Key,T_TDCS_D& rData)
	{return m_tdcs.Lookup(Key,rData);}
	int GetCount()
	{return m_tdcs.GetCount();}
	POSITION GetStart()
	{return m_tdcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TDCS_K& rKey,T_TDCS_D& rData)
	{m_tdcs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_TDCS_K,T_TDCS_K,T_TDCS_D,T_TDCS_D&>m_tdcs;
};

#endif
