#ifndef __DB_NONL_DB_H__
#define __DB_NONL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NONL
{
public:
	CDB_NONL();
	CDB_NONL(CDB_NONL& src);
	CDB_NONL& operator=(CDB_NONL& src);
	virtual ~CDB_NONL();

public:
	T_NONL_K m_nStartNum;
	T_NONL_K m_nLastNum;

public:
	void Add(T_NONL_K Key,T_NONL_D& rData);
	BOOL Del(T_NONL_K Key);

public:
	BOOL Get(T_NONL_K Key,T_NONL_D& rData)
		{return m_nonl.Lookup(Key,rData);}
	int GetCount()
		{return m_nonl.GetCount();}
	POSITION GetStart()
		{return m_nonl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_NONL_K& rKey,T_NONL_D& rData)
		{m_nonl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_NONL_K,T_NONL_K,T_NONL_D,T_NONL_D&>m_nonl;
};

#endif
