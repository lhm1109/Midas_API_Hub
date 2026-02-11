#ifndef __DB_LTSV_DB_H__
#define __DB_LTSV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_LTSV
{
public:
	CDB_LTSV();
	virtual ~CDB_LTSV();
	CDBDoc* m_pDoc;

public:
	void Add(T_LTSV_K Key,T_LTSV_D& rData);
	BOOL Del(T_LTSV_K Key);

public:
	BOOL Get(T_LTSV_K Key,T_LTSV_D& rData);
		//{return m_ltsv.Lookup(Key,rData);}
	int GetCount();
		//{return m_ltsv.GetCount();}
	POSITION GetStart();
		//{return m_ltsv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LTSV_K& rKey,T_LTSV_D& rData);
		//{m_ltsv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LTSV_K,T_LTSV_K,T_LTSV_D,T_LTSV_D&>m_ltsv;
};

#endif

