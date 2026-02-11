#ifndef __DB_CSCV_DB_H__
#define __DB_CSCV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECV;

class CDB_CSCV
{
public:
	CDB_CSCV();
	CDB_CSCV(CDB_CSCV& src);
	CDB_CSCV& operator=(CDB_CSCV& src);
	virtual ~CDB_CSCV();

public:
	T_CSCV_K m_nLastNum;
	T_CSCV_K m_nStartNum;

public:
	void Add(T_CSCV_K Key,T_CSCV_D& rData, CDB_SECV* pSect);
	BOOL Del(T_CSCV_K Key, CDB_SECV* pSect);

public:
	BOOL Get(T_CSCV_K Key,T_CSCV_D& rData)
		{return m_cscv.Lookup(Key,rData);}
	int GetCount()
		{return m_cscv.GetCount();}
	POSITION GetStart()
		{return m_cscv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CSCV_K& rKey,T_CSCV_D& rData)
		{m_cscv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CSCV_K,T_CSCV_K,T_CSCV_D,T_CSCV_D&>m_cscv;
};

#endif
