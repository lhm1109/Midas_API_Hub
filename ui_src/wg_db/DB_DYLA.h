#ifndef __DB_DYLA_DB_H__
#define __DB_DYLA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DYLA
{
public:
	CDB_DYLA();
	virtual ~CDB_DYLA();
	CDBDoc* m_pDoc;

public:
	T_DYLA_K m_nStartNum;
	T_DYLA_K m_nLastNum;

public:
	void Add(T_DYLA_K Key,T_DYLA_D& rData);
	BOOL Del(T_DYLA_K Key);

public:
	BOOL Get(T_DYLA_K Key,T_DYLA_D& rData);
	//{return m_dyla.Lookup(Key,rData);}
	int GetCount();
	//{return m_dyla.GetCount();}
	POSITION GetStart();
	//{return m_dyla.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DYLA_K& rKey,T_DYLA_D& rData);
	//{m_dyla.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DYLA_K,T_DYLA_K,T_DYLA_D,T_DYLA_D&>m_dyla;
};

#endif