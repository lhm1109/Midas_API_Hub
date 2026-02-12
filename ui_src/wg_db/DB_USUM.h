#ifndef __DB_USUM_DB_H__
#define __DB_USUM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_USUM
{
public:
	CDB_USUM();
	virtual ~CDB_USUM();
	CDBDoc* m_pDoc;

public:
	T_USUM_K m_nLastNum;
	T_USUM_K m_nStartNum;

public:
	void Add(T_USUM_K Key,T_USUM_D& rData);
	BOOL Del(T_USUM_K Key);

public:
	BOOL Get(T_USUM_K Key,T_USUM_D& rData)
		{return m_usum.Lookup(Key,rData);}
	int GetCount()
		{return m_usum.GetCount();}
	POSITION GetStart()
		{return m_usum.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_USUM_K& rKey,T_USUM_D& rData)
		{m_usum.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_USUM_K,T_USUM_K,T_USUM_D,T_USUM_D&>m_usum;
};

#endif
