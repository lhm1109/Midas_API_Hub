#ifndef __DB_MVLD_DB_H__
#define __DB_MVLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVLD
{
public:
	CDB_MVLD();
	virtual ~CDB_MVLD();
	CDBDoc* m_pDoc;

public:
	T_MVLD_K m_nStartNum;
	T_MVLD_K m_nLastNum;

public:
	void Add(T_MVLD_K Key,T_MVLD_D& rData);
	BOOL Del(T_MVLD_K Key);

public:
	BOOL Get(T_MVLD_K Key,T_MVLD_D& rData);
		//{return m_mvld.Lookup(Key,rData);}
	int GetCount();
		//{return m_mvld.GetCount();}
	POSITION GetStart();
		//{return m_mvld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MVLD_K& rKey,T_MVLD_D& rData);
		//{m_mvld.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MVLD_K,T_MVLD_K,T_MVLD_D,T_MVLD_D&>m_mvld;
};

#endif
