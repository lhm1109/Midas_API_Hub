#ifndef __DB_THRS_DB_H__
#define __DB_THRS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_THRS
{
public:
	CDB_THRS();
	virtual ~CDB_THRS();
	CDBDoc* m_pDoc;

public:
	T_THRS_K m_nStartNum;
	T_THRS_K m_nLastNum;

public:
	void Add(T_THRS_K Key,T_THRS_D& rData,CDB_THIS* pThis);
	BOOL Del(T_THRS_K Key,CDB_THIS* pThis);

public:
	BOOL Get(T_THRS_K Key,T_THRS_D& rData);
		//{return m_thrs.Lookup(Key,rData);}
	int GetCount();
		//{return m_thrs.GetCount();}
	POSITION GetStart();
		//{return m_thrs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THRS_K& rKey,T_THRS_D& rData);
		//{m_thrs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THRS_K,T_THRS_K,T_THRS_D,T_THRS_D&>m_thrs;
};

#endif