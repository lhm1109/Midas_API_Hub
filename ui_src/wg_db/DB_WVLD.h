#ifndef __DB_WVLD_DB_H__
#define __DB_WVLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_WVLD
{
public:
	CDB_WVLD();
	virtual ~CDB_WVLD();
	CDBDoc* m_pDoc;

public:
	T_WVLD_K m_nStartNum;
	T_WVLD_K m_nLastNum;

public:
	void Add(T_WVLD_K Key,T_WVLD_D& rData,CDB_THIS* pThis);
	BOOL Del(T_WVLD_K Key,CDB_THIS* pThis);

public:
	BOOL Get(T_WVLD_K Key,T_WVLD_D& rData);
		//{return m_wvld.Lookup(Key,rData);}
	int GetCount();
		//{return m_wvld.GetCount();}
	POSITION GetStart();
		//{return m_wvld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_WVLD_K& rKey,T_WVLD_D& rData);
		//{m_wvld.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_WVLD_K,T_WVLD_K,T_WVLD_D,T_WVLD_D&>m_wvld;
};

#endif