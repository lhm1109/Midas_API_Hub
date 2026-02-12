#ifndef __DB_MVLDjp_DB_H__
#define __DB_MVLDjp_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVLDjp
{
public:
	CDB_MVLDjp();
	virtual ~CDB_MVLDjp();
	CDBDoc* m_pDoc;

public:
	T_MVLDjp_K m_nStartNum;
	T_MVLDjp_K m_nLastNum;

public:
	void Add(T_MVLDjp_K Key,T_MVLDjp_D& rData);
	BOOL Del(T_MVLDjp_K Key);

public:
	BOOL Get(T_MVLDjp_K Key,T_MVLDjp_D& rData);
		//{return m_mvldjp.Lookup(Key,rData);}
	int GetCount();
		//{return m_mvldjp.GetCount();}
	POSITION GetStart();
		//{return m_mvldjp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MVLDjp_K& rKey,T_MVLDjp_D& rData);
		//{m_mvldjp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MVLDjp_K,T_MVLDjp_K,T_MVLDjp_D,T_MVLDjp_D&>m_mvldjp;
};

#endif
