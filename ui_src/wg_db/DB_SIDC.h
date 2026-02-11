#ifndef __DB_SIDC_DB_H__
#define __DB_SIDC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SIDC
{
public:
	CDB_SIDC();
	virtual ~CDB_SIDC();
	CDBDoc* m_pDoc;

public:
	T_SIDC_K m_nStartNum;
	T_SIDC_K m_nLastNum;

public:
	void Add(T_SIDC_K Key, T_SIDC_D& rData);
	BOOL Del(T_SIDC_K Key);

public:
	BOOL Get(T_SIDC_D& rData);
	BOOL Get(T_SIDC_K Key,T_SIDC_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_SIDC_K& rKey, T_SIDC_D& rData);

protected:
	CMap<T_SIDC_K,T_SIDC_K,T_SIDC_D,T_SIDC_D&>m_sidc;
};

#endif