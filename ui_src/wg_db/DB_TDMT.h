#ifndef __DB_TDMT_DB_H__
#define __DB_TDMT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDMT
{
public:
	CDB_TDMT();
	CDB_TDMT(CDB_TDMT& src);
	CDB_TDMT& operator=(CDB_TDMT& src);
	virtual ~CDB_TDMT();

public:
	T_TDMT_K m_nStartNum;
	T_TDMT_K m_nLastNum;

public:
	void Add(T_TDMT_K Key,T_TDMT_D& rData);
	BOOL Del(T_TDMT_K Key);

public:
	BOOL Get(T_TDMT_K Key,T_TDMT_D& rData)
		{return m_tdmt.Lookup(Key,rData);}
	int GetCount()
		{return (int)m_tdmt.GetCount();}
	POSITION GetStart()
		{return m_tdmt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TDMT_K& rKey,T_TDMT_D& rData)
		{m_tdmt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_TDMT_K,T_TDMT_K,T_TDMT_D,T_TDMT_D&>m_tdmt;
};

#endif
