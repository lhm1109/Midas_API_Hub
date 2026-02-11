#ifndef __DB_EPMT_DB_H__
#define __DB_EPMT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EPMT
{
public:
	CDB_EPMT();
	CDB_EPMT(CDB_EPMT& src);
	CDB_EPMT& operator=(CDB_EPMT& src);
	virtual ~CDB_EPMT();

public:
	T_EPMT_K m_nStartNum;
	T_EPMT_K m_nLastNum;

public:
	void Add(T_EPMT_K Key,T_EPMT_D& rData);
	BOOL Del(T_EPMT_K Key);

public:
	BOOL Get(T_EPMT_K Key,T_EPMT_D& rData)
		{return m_epmt.Lookup(Key,rData);}
	int GetCount()
		{return m_epmt.GetCount();}
	POSITION GetStart()
		{return m_epmt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_EPMT_K& rKey,T_EPMT_D& rData)
		{m_epmt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_EPMT_K,T_EPMT_K,T_EPMT_D,T_EPMT_D&>m_epmt;
};

#endif
