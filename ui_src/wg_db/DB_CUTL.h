#ifndef __DB_CUTL_DB_H__
#define __DB_CUTL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CUTL
{
public:
	CDB_CUTL();
	virtual ~CDB_CUTL();

public:
	T_CUTL_K m_nStartNum;
	T_CUTL_K m_nLastNum;

public:
	void Add(T_CUTL_K Key,T_CUTL_D& rData);
	BOOL Del(T_CUTL_K Key);

public:
	BOOL Get(T_CUTL_K Key,T_CUTL_D& rData)
		{return m_cutl.Lookup(Key,rData);}
	int GetCount()
		{return m_cutl.GetCount();}
	POSITION GetStart()
		{return m_cutl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CUTL_K& rKey,T_CUTL_D& rData)
		{m_cutl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CUTL_K,T_CUTL_K,T_CUTL_D,T_CUTL_D&>m_cutl;
};

#endif