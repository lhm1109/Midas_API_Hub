#ifndef __DB_FIBR_DB_H__
#define __DB_FIBR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_FIBR
{
public:
	CDB_FIBR();
	virtual ~CDB_FIBR();
	CDBDoc* m_pDoc;

public:
	T_FIBR_K m_nStartNum;
	T_FIBR_K m_nLastNum;

public:
	void Add(T_FIBR_K Key,T_FIBR_D& rData);
	BOOL Del(T_FIBR_K Key);

public:
	BOOL Get(T_FIBR_K Key,T_FIBR_D& rData);
		//{return m_fibr.Lookup(Key,rData);}
	int GetCount();
		//{return m_fibr.GetCount();}
	POSITION GetStart();
		//{return m_fibr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FIBR_K& rKey,T_FIBR_D& rData);
		//{m_fibr.GetNextAssoc(rNextPosition,rKey,rData);}
	int GetCount(int nAssignType);

protected:
	CMap<T_FIBR_K,T_FIBR_K,T_FIBR_D,T_FIBR_D&>m_fibr;
};

#endif
