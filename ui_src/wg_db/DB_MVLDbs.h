#ifndef __DB_MVLDbs_DB_H__
#define __DB_MVLDbs_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVLDbs
{
public:
	CDB_MVLDbs();
	virtual ~CDB_MVLDbs();
	CDBDoc* m_pDoc;

public:
	T_MVLDbs_K m_nStartNum;
	T_MVLDbs_K m_nLastNum;

public:
	void Add(T_MVLDbs_K Key,T_MVLDbs_D& rData);
	BOOL Del(T_MVLDbs_K Key);

public:
	BOOL Get(T_MVLDbs_K Key,T_MVLDbs_D& rData);
		//{return m_mvldbs.Lookup(Key,rData);}
	int GetCount();
		//{return m_mvldbs.GetCount();}
	POSITION GetStart();
		//{return m_mvldbs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MVLDbs_K& rKey,T_MVLDbs_D& rData);
		//{m_mvldbs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MVLDbs_K,T_MVLDbs_K,T_MVLDbs_D,T_MVLDbs_D&>m_mvldbs;
};

#endif
