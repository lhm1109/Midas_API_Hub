#ifndef __DB_NMDV_DB_H__
#define __DB_NMDV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_NMDV
{
public:
	CDB_NMDV();
	virtual ~CDB_NMDV();
	CDBDoc* m_pDoc;

public:
	T_NMDV_K m_nStartNum;
	T_NMDV_K m_nLastNum;

public:
	void Add(T_NMDV_K Key, T_NMDV_D& rData);
	BOOL Del(T_NMDV_K Key);
	void Initialize();

public:
	BOOL Get(T_NMDV_K Key, T_NMDV_D& rData);
	//{return m_nmdv.Lookup(Key,rData);}
	int GetCount();
	//{return m_nmdv.GetCount();}
	POSITION GetStart();
	//{return m_nmdv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_NMDV_K& rKey, T_NMDV_D& rData);
	//{m_nmdv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_NMDV_K, T_NMDV_K, T_NMDV_D, T_NMDV_D&>m_nmdv;
};

#endif
