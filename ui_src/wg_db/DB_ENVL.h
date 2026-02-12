#ifndef __DB_ENVL_DB_H__
#define __DB_ENVL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_ENVL
{
public:
	CDB_ENVL();
	virtual ~CDB_ENVL();
	CDBDoc* m_pDoc;

public:
	T_ENVL_K m_nStartNum;
	T_ENVL_K m_nLastNum;

public:
	void Add(T_ENVL_K Key,T_ENVL_D& rData);
	BOOL Del(T_ENVL_K Key);

public:
	BOOL Get(T_ENVL_K Key,T_ENVL_D& rData);
		//{return m_envl.Lookup(Key,rData);}
	int GetCount();
		//{return m_envl.GetCount();}
	POSITION GetStart();
		//{return m_envl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ENVL_K& rKey,T_ENVL_D& rData);
		//{m_envl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ENVL_K,T_ENVL_K,T_ENVL_D,T_ENVL_D&>m_envl;
};

#endif