#ifndef __DB_MVLDfr_DB_H__
#define __DB_MVLDfr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVLDfr
{
public:
	CDB_MVLDfr();
	virtual ~CDB_MVLDfr();
	CDBDoc* m_pDoc;

public:
	T_MVLDfr_K m_nStartNum;
	T_MVLDfr_K m_nLastNum;

public:
	void Add(T_MVLDfr_K Key,T_MVLDfr_D& rData);
	BOOL Del(T_MVLDfr_K Key);

public:
	BOOL Get(T_MVLDfr_K Key,T_MVLDfr_D& rData);
		//{return m_mvldfr.Lookup(Key,rData);}
	int GetCount();
		//{return m_mvldfr.GetCount();}
	POSITION GetStart();
		//{return m_mvldfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MVLDfr_K& rKey,T_MVLDfr_D& rData);
		//{m_mvldfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MVLDfr_K,T_MVLDfr_K,T_MVLDfr_D,T_MVLDfr_D&>m_mvldfr;
};

#endif
