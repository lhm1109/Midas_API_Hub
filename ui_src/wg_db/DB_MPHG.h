#ifndef __DB_MPHG_DB_H__
#define __DB_MPHG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MATL;

class CDB_MPHG
{
public:
	CDB_MPHG();
	virtual ~CDB_MPHG();

public:
	void Add(T_MPHG_K Key,T_MPHG_D& rData,CDB_MATL* pMatl);
	BOOL Del(T_MPHG_K Key,CDB_MATL* pMatl);

public:
	BOOL Get(T_MPHG_K Key,T_MPHG_D& rData)
		{return m_mphg.Lookup(Key,rData);}
	int GetCount()
		{return m_mphg.GetCount();}
	POSITION GetStart()
		{return m_mphg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MPHG_K& rKey,T_MPHG_D& rData)
		{m_mphg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MPHG_K,T_MPHG_K,T_MPHG_D,T_MPHG_D&>m_mphg;
};

#endif