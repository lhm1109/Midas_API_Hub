#ifndef __DB_CLWP_DB_H__
#define __DB_CLWP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CLWP
{
public:
	CDB_CLWP();
	virtual ~CDB_CLWP();

public:
	T_CLWP_K m_nStartNum;
	T_CLWP_K m_nLastNum;

public:
	void Add(T_CLWP_K Key,T_CLWP_D& rData);
	BOOL Del(T_CLWP_K Key);

public:
	BOOL Get(T_CLWP_K Key,T_CLWP_D& rData)
		{return m_clwp.Lookup(Key,rData);}
	int GetCount()
		{return m_clwp.GetCount();}
	POSITION GetStart()
		{return m_clwp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CLWP_K& rKey,T_CLWP_D& rData)
		{m_clwp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CLWP_K,T_CLWP_K,T_CLWP_D,T_CLWP_D&>m_clwp;
};

#endif