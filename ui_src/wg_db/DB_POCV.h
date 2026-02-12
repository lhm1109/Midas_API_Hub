#ifndef __DB_POCV_DB_H__
#define __DB_POCV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_POLC;

class CDB_POCV
{
public:
	CDB_POCV();
	virtual ~CDB_POCV();
	CDBDoc* m_pDoc;

public:
	T_POCV_K m_nStartNum;
	T_POCV_K m_nLastNum;

public:
	void Add(T_POCV_K Key,T_POCV_D& rData,CDB_POLC* pPolc);
	BOOL Del(T_POCV_K Key,CDB_POLC* pPolc);

public:
	BOOL Get(T_POCV_K Key,T_POCV_D& rData);
		//{return m_pocv.Lookup(Key,rData);}
	int GetCount();
		//{return m_pocv.GetCount();}
	POSITION GetStart();
		//{return m_pocv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_POCV_K& rKey,T_POCV_D& rData);
		//{m_pocv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_POCV_K,T_POCV_K,T_POCV_D,T_POCV_D&>m_pocv;
};

#endif